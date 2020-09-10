#pragma once

#include <g_mthread_ThreadTypes.h>

namespace g
{
namespace mthread
{


struct IThreadSimpleEvent 
{ 
	virtual void send () = 0; 
};

struct ThreadSimpleEvent
{
	template <class T> struct class_method { typedef void ( T::*Pointer_t ) (  ); };
	typedef void ( *FunctionPointer_t ) (  );

	struct Subscriber : IThreadSimpleEvent {};
	struct Sender : IThreadSimpleEvent {};

	typedef g::cont::ref<Subscriber>                 SubscriberRef_t;
	typedef g::cont::ref_lst<Subscriber>             ListSubscriber_t;
	typedef g::signal::signal_receiver<ThreadSimpleEvent> Receiver_t;

	struct FunctionSubscriber : public g::signal::function_subscriber_base<ThreadSimpleEvent>
	{ 
		virtual void send () { ( *functionPointer ) (  );	} 
	};
	
	template < class T > struct method_subscriber : public g::signal::method_subscriber_base<ThreadSimpleEvent,T>
	{ 
		virtual void send ()
		{
			((this->clsInstanceP)->*(this->methodPointer))();	
		} 
	};
			
	struct SenderReceiver : Sender , virtual Receiver_t 
	{
		virtual void send () 
		{ 
			for ( g::cont::ref_lst<Subscriber>::It_t it = mListSubscribers ; it.isIn ( ) ; it++ ) 
			{ 
				it->send (); 
			} 
		}
	};

	class SenderSerialized : public Sender , public g::signal::ISenderSerialized
	{		
	public:
		virtual void send ()
		{
			g::signal::QueueDataBufferAbstract::Ref_t data_buffer_ref = mGetQueueSenderP ( )->instanciateDataBufferRef ( );
			
			*data_buffer_ref;

			mGetQueueSenderP ( )->push ( data_buffer_ref );
		}
	};

	class ReceiverSerialized : private SenderReceiver , public virtual Receiver_t , public g::signal::IReceiverSerialized
	{
	protected:
		void mDrainEvent ( GTimeoutSec_t aTimeOut = 0.0 )
		{
			g::signal::QueueDataBufferAbstract::Ref_t data_buffer_ref = mGetQueueReceiverP ( )->pop (aTimeOut);

			data_buffer_ref->reset();

			
			*data_buffer_ref ;
		
			SenderReceiver::send ();
		}
	};
	
	class SenderReceiverSerializedAbstract : public SenderSerialized , public ReceiverSerialized
	{
	public:
		virtual void send () { SenderSerialized::send (); }
	};	
	
	template <class Q=g::signal::QueueTxRxSimple> struct sender_receiver_serialized : SenderReceiverSerializedAbstract , public Q {	};
	
	template <class T , class Q=g::signal::QueueTxRxSimple> class thread_base : 
		public T , public virtual Receiver_t , private sender_receiver_serialized<Q>
	{
	public:
		thread_base() {	}
	
		void start ()
		{
			G_EXC_SET_CONTEXT ( "void ThreadSimpleEvent::thread_base<T,Q>::start ()" );
			
			if ( mListSubscribers.getSize() > 0 )
			{		
				this->send (  );
	
				T::start ( this );
			}
			else
			{
				G_EXC_RAISE ( g::mthread::ThreadNotHavingSubscribersException );
			}
		}
	
	private:	
		virtual void mEntryPoint ( void* aPar ) 
		{
			this->mDrainEvent();			
		}
	
		g::signal::QueueReceiverAbstract* mGetQueueReceiverP ( ) { return this; }
		g::signal::QueueSenderAbstract* mGetQueueSenderP ( ) { return this; }
	};
	
};

} //namespace mthread
} //namespace g


#pragma once

#include <g_mthread_ThreadTypes.h>

namespace g
{
namespace mthread
{


struct IThreadWithParamEvent 
{ 
	virtual void send ( void * aPar ) = 0; 
};

struct ThreadWithParamEvent
{
	template <class T> struct class_method { typedef void ( T::*Pointer_t ) (  void * aPar  ); };
	typedef void ( *FunctionPointer_t ) (  void *   );

	struct Subscriber : IThreadWithParamEvent {};
	struct Sender : IThreadWithParamEvent {};

	typedef g::cont::ref<Subscriber>                 SubscriberRef_t;
	typedef g::cont::ref_lst<Subscriber>             ListSubscriber_t;
	typedef g::signal::signal_receiver<ThreadWithParamEvent> Receiver_t;

	struct FunctionSubscriber : public g::signal::function_subscriber_base<ThreadWithParamEvent>
	{ 
		virtual void send ( void * aPar ) { ( *functionPointer ) (  aPar  );	} 
	};
	
	template < class T > struct method_subscriber : public g::signal::method_subscriber_base<ThreadWithParamEvent,T>
	{ 
		virtual void send ( void * aPar )
		{
			((this->clsInstanceP)->*(this->methodPointer))( aPar );	
		} 
	};
			
	struct SenderReceiver : Sender , virtual Receiver_t 
	{
		virtual void send ( void * aPar ) 
		{ 
			for ( g::cont::ref_lst<Subscriber>::It_t it = mListSubscribers ; it.isIn ( ) ; it++ ) 
			{ 
				it->send ( aPar ); 
			} 
		}
	};

	class SenderSerialized : public Sender , public g::signal::ISenderSerialized
	{		
	public:
		virtual void send ( void * aPar )
		{
			g::signal::QueueDataBufferAbstract::Ref_t data_buffer_ref = mGetQueueSenderP ( )->instanciateDataBufferRef ( );
			
			*data_buffer_ref << aPar;

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

			void *  aPar;
			
			*data_buffer_ref  >> aPar;
		
			SenderReceiver::send ( aPar );
		}
	};
	
	class SenderReceiverSerializedAbstract : public SenderSerialized , public ReceiverSerialized
	{
	public:
		virtual void send ( void * aPar ) { SenderSerialized::send ( aPar ); }
	};	
	
	template <class Q=g::signal::QueueTxRxSimple> struct sender_receiver_serialized : SenderReceiverSerializedAbstract , public Q {	};
	
	template <class T , class Q=g::signal::QueueTxRxSimple> class thread_base : 
		public T , public virtual Receiver_t , private sender_receiver_serialized<Q>
	{
	public:
		thread_base() {	}
	
		void start ( void * aPar )
		{
			G_EXC_SET_CONTEXT ( "void ThreadWithParamEvent::thread_base<T,Q>::start ( void * aPar )" );
			
			if ( mListSubscribers.getSize() > 0 )
			{		
				this->send (  aPar  );
	
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


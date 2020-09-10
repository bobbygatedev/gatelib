#pragma once

#include <g_mthread_ThreadTypes.h>



struct IMySignal 
{ 
	virtual void send ( GInt32_t aPar1 , GUint64_t aPar2 ) = 0; 
};

struct MySignal
{
	template <class T> struct class_method { typedef void ( T::*Pointer_t ) (  GInt32_t aPar1 , GUint64_t aPar2  ); };
	typedef void ( *FunctionPointer_t ) (  GInt32_t  , GUint64_t   );

	struct Subscriber : IMySignal {};
	struct Sender : IMySignal {};

	typedef g::cont::ref<Subscriber>                 SubscriberRef_t;
	typedef g::cont::ref_lst<Subscriber>             ListSubscriber_t;
	typedef g::signal::signal_receiver<MySignal> Receiver_t;

	struct FunctionSubscriber : public g::signal::function_subscriber_base<MySignal>
	{ 
		virtual void send ( GInt32_t aPar1 , GUint64_t aPar2 ) { ( *functionPointer ) (  aPar1 , aPar2  );	} 
	};
	
	template < class T > struct method_subscriber : public g::signal::method_subscriber_base<MySignal,T>
	{ 
		virtual void send ( GInt32_t aPar1 , GUint64_t aPar2 )
		{
			((this->clsInstanceP)->*(this->methodPointer))( aPar1 , aPar2 );	
		} 
	};
			
	struct SenderReceiver : Sender , virtual Receiver_t 
	{
		virtual void send ( GInt32_t aPar1 , GUint64_t aPar2 ) 
		{ 
			for ( g::cont::ref_lst<Subscriber>::It_t it = mListSubscribers ; it.isIn ( ) ; it++ ) 
			{ 
				it->send ( aPar1 , aPar2 ); 
			} 
		}
	};

	class SenderSerialized : public Sender , public g::signal::ISenderSerialized
	{		
	public:
		virtual void send ( GInt32_t aPar1 , GUint64_t aPar2 )
		{
			g::signal::QueueDataBufferAbstract::Ref_t data_buffer_ref = mGetQueueSenderP ( )->instanciateDataBufferRef ( );
			
			*data_buffer_ref << aPar1 << aPar2;

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

			GInt32_t  aPar1;
			GUint64_t  aPar2;
			
			*data_buffer_ref  >> aPar1 >> aPar2;
		
			SenderReceiver::send ( aPar1 , aPar2 );
		}
	};
	
	class SenderReceiverSerializedAbstract : public SenderSerialized , public ReceiverSerialized
	{
	public:
		virtual void send ( GInt32_t aPar1 , GUint64_t aPar2 ) { SenderSerialized::send ( aPar1 , aPar2 ); }
	};	
	
	template <class Q=g::signal::QueueTxRxSimple> struct sender_receiver_serialized : SenderReceiverSerializedAbstract , public Q {	};
	
	template <class T , class Q=g::signal::QueueTxRxSimple> class thread_base : 
		public T , public virtual Receiver_t , private sender_receiver_serialized<Q>
	{
	public:
		thread_base() {	}
	
		void start ( GInt32_t aPar1 , GUint64_t aPar2 )
		{
			G_EXC_SET_CONTEXT ( "void MySignal::thread_base<T,Q>::start ( GInt32_t aPar1 , GUint64_t aPar2 )" );
			
			if ( mListSubscribers.getSize() > 0 )
			{		
				this->send (  aPar1 , aPar2  );
	
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



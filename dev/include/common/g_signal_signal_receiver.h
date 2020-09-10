#pragma once

#include "g_signal_subscribers.h"

namespace g
{
namespace signal
{
//!Class capable of receiving events
template < class E > class signal_receiver 
{
public:
	typedef E                              Event_t;
	typedef typename E::Subscriber         Subscriber_t;
	typedef typename E::SubscriberRef_t    SubscriberRef_t;
	typedef typename E::ListSubscriber_t   ListSubscriber_t;
	typedef typename E::FunctionSubscriber FunctionSubscriber_t;
	typedef typename E::FunctionPointer_t  FunctionPointer_t;

	void subscribe ( SubscriberRef_t aSubscriberRef )
	{
		mListSubscribers.pushTail ( aSubscriberRef );
	}

	SubscriberRef_t subscribe ( typename E::FunctionPointer_t aFunctionPointer )
	{
		SubscriberRef_t event_ref = FunctionSubscriber_t::create ( aFunctionPointer );
		subscribe ( event_ref );

		return event_ref;
	}

	template < class T > SubscriberRef_t subscribe ( T* aClsInstance , typename E::template class_method<T>::Pointer_t aMethodPointer )
	{
		SubscriberRef_t event_ref = E::template method_subscriber<T>::create( aMethodPointer , aClsInstance );
		subscribe ( event_ref );

		return event_ref;
	}

	void unsubscribe ( SubscriberRef_t aSubscriberRef )
	{
		mListSubscribers.removeAll ( aSubscriberRef );
	}

	void unsubscribe ( typename Event_t::FunctionPointer_t aFunctionPointer )
	{	
		ListSubscriber_t to_remove_list;

		for ( typename ListSubscriber_t::It_t it = mListSubscribers.getIterator ( ) ; it.isIn ( ) ; it++ )
		{
			g::cont::ref<FunctionSubscriber_t> subscriber_ref;

			//if 
			if ( subscriber_ref.tryRefTakeDynamically((SubscriberRef_t)it) )
			{
				if ( subscriber_ref->functionPointer == aFunctionPointer )
				{
					to_remove_list.pushTail ( it );
				}
			}
		}

		for ( typename ListSubscriber_t::It_t it = to_remove_list.getIterator ( ) ; it.isIn ( ) ; it++ )
		{
			mListSubscribers.tryRemove ( it );
		}
	}

	void doClearSubscribers() { mListSubscribers.setEmpty(); }

	template < class T > void unsubscribe ( T* aInstanceP ,  typename E::template class_method<T>::Pointer_t aClassMethodPointer )
	{
		ListSubscriber_t to_remove_list;

		for ( typename ListSubscriber_t::It_t it = mListSubscribers.getIterator ( ) ; it.isIn ( ) ; it++ )
		{
			g::cont::ref< typename E::template method_subscriber<T> > subscriber_ref;

			//if 
			if ( subscriber_ref.tryRefTakeDynamically((SubscriberRef_t)it) )
			{
				if ( subscriber_ref->clsInstanceP == aInstanceP && subscriber_ref->methodPointer == aClassMethodPointer )
				{
					to_remove_list.pushTail ( it );
				}
			}
		}

		for ( typename ListSubscriber_t::It_t it = to_remove_list.getIterator ( ) ; it.isIn ( ) ; it++ )
		{
			mListSubscribers.tryRemove ( it );
		}
	}

protected:
	ListSubscriber_t mListSubscribers;
};

//class able to receive events as own methods
template <class E,class T> class class_with_event_receiver : 
	//is included as virtual, in case of subclassing i.e.:
	//class MyClassWithEvent : public class_with_event_receiver<MyEvent,MyClassWithEvent>{}
	//class MyClassWithEventChild : public MyClassWithEvent,publicclass_with_event_receiver<MyEvent,MyClassWithEventChild>{}
	protected virtual signal_receiver<E> 
{
public:
	typedef typename E::template class_method<T>::Pointer_t MethodPointer_t;
	typedef typename E::SubscriberRef_t                     SubscriberRef_t;

	void subscribe ( MethodPointer_t aThisClassMethodPointer )
	{
		signal_receiver<E>::subscribe ( static_cast<T*>(this) , (MethodPointer_t)aThisClassMethodPointer );
	}

	void unsubscribe ( MethodPointer_t aThisClassMethodPointer )
	{
		signal_receiver<E>::unsubscribe ( static_cast<T*>(this) , aThisClassMethodPointer );	
	}

	void subscribe   ( SubscriberRef_t aSubscriberRef ) { signal_receiver<E>::subscribe(aSubscriberRef); }
	void unsubscribe ( SubscriberRef_t aSubscriberRef ) { signal_receiver<E>::unsubscribe(aSubscriberRef); }
};


//both a sender and a receiver ( simple)
template <class E,class T> struct class_with_event_sender_receiver : class_with_event_receiver<E,T> , public E::SenderReceiver{};

}//namespace signal
}//namespace g



#pragma once

#include "g_cont_ref.h"

namespace g
{
namespace signal
{

template < class E > class function_subscriber_base : public E::Subscriber
{
public:
	typedef typename E::SubscriberRef_t   SubscriberRef_t;
	typedef typename E::FunctionPointer_t FunctionPointer_t;

	static SubscriberRef_t create ( FunctionPointer_t sp , g::cont::AllocationPolicyAbstract* ap =  g::cont::AllocationPolicyAbstract::get_FromStandardPolicy() )
	{
		g::cont::ref<typename E::FunctionSubscriber> instance = g::cont::ref<typename E::FunctionSubscriber>::g_cont_new(ap);
		
		instance->functionPointer = sp;

		return instance;
	}

	typename E::FunctionPointer_t functionPointer;
};

template < class E , class T > class method_subscriber_base : public E::Subscriber
{
public:	
	typedef typename E::SubscriberRef_t                     SubscriberRef_t;
	typedef typename E::template class_method<T>::Pointer_t MethodPointer_t;
	typedef typename E::template method_subscriber<T>       MethodSubscriber_t;

	static SubscriberRef_t create ( MethodPointer_t sp , T* ip , g::cont::AllocationPolicyAbstract* ap = g::cont::AllocationPolicyAbstract::get_FromStandardPolicy() )
	{
		g::cont::ref<MethodSubscriber_t> instance = g::cont::ref<MethodSubscriber_t>::g_cont_new(ap);
		
		instance->methodPointer = sp;
		instance->clsInstanceP  = ip;

		return instance;
	}

	MethodPointer_t methodPointer;
	T*              clsInstanceP;
};

}//namespace signal
}//namespace g


#pragma once

#include "g_cont_ref_custom.h"
#include "g_cont_ref_const.h"

namespace g
{
namespace cont
{

template < class T > class ref : public ref_custom<T>
{
public:	
	ref(){ G_VERBOSE_MSG_L5("ref constructor."); }

#if G_AUTOPTION_ON_MSVC > 0 //Viual Studio Only
	ref(const ref& other){ G_VERBOSE_MSG_L5("ref copy constructor"); ref_custom<T>::mRefTake<T>(other); }
	ref& operator = (const ref& other) { G_VERBOSE_MSG_L5("ref copy operator"); ref_custom<T>::mRefTake<T>(other); return *this; }

	template <class O> ref(const ref<O>& other):ref_custom<T>() { G_VERBOSE_MSG_L5("ref copy constructor (OTHER)"); this->mRefTake<O>(other); }
	template <class O> ref& operator = ( const ref<O>& other ) { G_VERBOSE_MSG_L5("ref copy operator()"); this->mRefTake<O> (other); return *this; }
	template <class O> bool tryRefTakeDynamically ( ref<O>& o ) { return this->mRefTakeTryDynamically<O>(o); }
#else
	ref(const ref& other){ G_VERBOSE_MSG_L5("ref(const ref& other)"); this->mRefTake(other); }
	ref& operator = (const ref& other) { G_VERBOSE_MSG_L5("ref& operator = (ref& other )"); this->mRefTake(other); return *this; }

	template <class O> ref(const ref<O>& other):ref_custom<T>() { G_VERBOSE_MSG_L5("ref(const ref<O>& other)"); this->mRefTake(other); }
	template <class O> ref& operator = ( const ref<O>& other ) { G_VERBOSE_MSG_L5("ref& operator = ( ref<O> other )"); this->mRefTake (other); return *this; }
	template <class O> bool tryRefTakeDynamically ( ref<O>& o ) { return this->mRefTakeTryDynamically(o); }

#endif

	T& operator *()  { G_VERBOSE_MSG_L5("T& ref_custom::operator *()"); return *this->mGetObjP(); }
	T* operator ->() { G_VERBOSE_MSG_L5("T* ref_custom::operator ->()");return this->mGetObjP(); }
	const T& operator *() const  { G_VERBOSE_MSG_L5("const T& ref_custom::operator *()");return *this->mGetObjP(); }
	const T* operator ->() const { G_VERBOSE_MSG_L5("const T* ref_custom::operator ->()");return this->mGetObjP(); }

   static ref g_cont_new ( AllocationPolicyAbstract* d_alloc_policy = AllocationPolicyAbstract::get_FromStandardPolicy ( ) )
   {
      ref<T> result;

      new ( result.mAllocateMemForObject(d_alloc_policy) )T();
      result.mIncRef ( );//allocation has succeeded inc ref will cause deletion of T(), as ref count elapses.

      return result;
   }
};

}//namespace g
}//namespace cont




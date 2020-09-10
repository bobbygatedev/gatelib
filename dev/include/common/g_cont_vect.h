#pragma once

#include "g_cont_vect_base.h"
#include "private/g_cont_vect_priv.h"
#include "g_cont_ref_custom.h"
#include "g_cont_it.h"
#include <string.h>


namespace g
{
namespace cont
{

template < class T , class IT=it<T>, class IT_C=it_const<T> > class vect : public vect_base<T,T,const T,IT,IT_C>
{
public:
	template <typename> friend class priv::vect_positioner;
	template <typename> friend struct priv::vect_content;

	typedef vect_base<T,T,const T,IT,IT_C> Base_t;
	typedef IT   It_t;
	typedef IT_C ItConst_t;
	typedef ref_const< vect<T,IT,IT_C> > RefConst_t;

	vect ( AllocationPolicyAbstract* ap = AllocationPolicyAbstract::get_FromStandardPolicy() , int aAllocDeltaRBits = G_VECT_AL_DELTA_MASK_BITS ) : Base_t ( ap ,aAllocDeltaRBits ){}
	vect ( const T* v, int lbound , int ubound , AllocationPolicyAbstract* ap = AllocationPolicyAbstract::get_FromStandardPolicy() , int aAllocDeltaRBits = G_VECT_AL_DELTA_MASK_BITS):Base_t ( ap ,aAllocDeltaRBits ){ setContentFromVector (v,lbound,ubound); }
	vect ( const T* v , size_t s , AllocationPolicyAbstract* ap = AllocationPolicyAbstract::get_FromStandardPolicy() , int aAllocDeltaRBits = G_VECT_AL_DELTA_MASK_BITS ) : Base_t ( ap ,aAllocDeltaRBits ){ setContentFromVector (v,s); }
	vect ( const vect& o , AllocationPolicyAbstract* ap = AllocationPolicyAbstract::get_FromStandardPolicy() , int aAllocDeltaRBits = G_VECT_AL_DELTA_MASK_BITS ): Base_t ( ap ,aAllocDeltaRBits ) { this->mContent.replaceContent ( o.getLbound ( ) , o.getUbound ( ) , o.mContent.data ); }

	virtual ~vect ( ){}
	
	vect& operator = ( const vect& other ) { setContent ( other ); return *this; }
	
	//Copy the content of other value_base_cont and delete any previous content
	virtual void setContent ( const vect& other ){ this->mContent.replaceContent ( other.getLbound(), other.getUbound() , (const T*)other ); }

	void setContentFromVector   ( const T* v , size_t size ) { setContentFromVector ( v , 0 , (int)size-1 ); }
	void setContentFromVector   ( const T* v , int lbound , int ubound ) { this->mContent.replaceContent ( lbound , ubound , v ); }

	operator const T* ( ) const { return this->mContent.data; }
	operator       T* ( )       { return this->mContent.data; }

	class Ref : public ref_custom< vect<T,IT,IT_C> >
	{
	public:
		Ref(){}
		Ref(const Ref& other ){ G_VERBOSE_MSG_L5("vect::Ref::Ref(Ref& other )"); this->mRefTake(other); }

		typedef vect<T,IT,IT_C> Vect_t;

		Vect_t& operator *()  { G_VERBOSE_MSG_L5("T& vect::Ref::operator *()"); return *this->mGetObjP(); }
		Vect_t* operator ->() { G_VERBOSE_MSG_L5("T& vect::Ref::operator ->()");return this->mGetObjP(); }
		const Vect_t& operator *() const  { G_VERBOSE_MSG_L5("const T& vect::Ref::operator *()");return *this->mGetObjP(); }
		const Vect_t* operator ->() const { G_VERBOSE_MSG_L5("const T* vect::Ref::operator ->()");return this->mGetObjP(); }

		Ref& operator = ( const Ref& other ) { this->mRefTake(*this); return *this; }

      static Ref g_cont_new ( AllocationPolicyAbstract* aAllocPolicyP = AllocationPolicyAbstract::get_FromStandardPolicy ( ) , int aAllocDeltaRBits = G_VECT_AL_DELTA_MASK_BITS )
      {
         Ref result;

         new ( result.mAllocateMemForObject(aAllocPolicyP) )vect<T,IT,IT_C>(aAllocPolicyP,aAllocDeltaRBits);
         result.mIncRef ( );

         return result;
      }
	};
};	

}//namespace cont
}//namespace g


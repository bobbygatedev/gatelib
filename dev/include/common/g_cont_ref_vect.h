#pragma once

#include "g_cont_vect_base.h"
#include "private/g_cont_vect_priv.h"

namespace g
{
namespace cont
{

template<class T, class REF = ref<T> , class REF_C = ref_const<T> , class IT = it_ref<T,REF,REF_C> , class IT_C=it_ref_const<T,REF,REF_C> >
	class ref_vect : public vect_base<T,REF,REF_C,IT,IT_C>
{
public:
	template <typename> friend class priv::vect_positioner;
	template <typename> friend struct priv::vect_content;

	typedef REF                            Ref_t;
	typedef REF_C                          RefConst_t;
	typedef it_ref<T,REF,REF_C>            It_t;
	typedef it_ref_const<T,REF,REF_C>      ItConst_t;
	typedef vect_base<T,REF,REF_C,IT,IT_C> Base_t;

	ref_vect ( AllocationPolicyAbstract* ap = AllocationPolicyAbstract::get_FromStandardPolicy() , int aAllocDeltaRBits = G_VECT_AL_DELTA_MASK_BITS ) : Base_t ( ap ,aAllocDeltaRBits ) {}
	ref_vect ( ref_vect& o , AllocationPolicyAbstract* ap = AllocationPolicyAbstract::get_FromStandardPolicy() , int aAllocDeltaRBits = G_VECT_AL_DELTA_MASK_BITS ): Base_t ( ap ,aAllocDeltaRBits ) { this->mContent.replaceContent ( o.getLbound ( ) , o.getUbound ( ) , o.mContent.data ); }

	virtual ~ref_vect ( ) { }

	class Ref : public ref_custom< ref_vect<T> >
	{
	public:
		Ref(){}
		Ref(const Ref& other ){ G_VERBOSE_MSG_L5("ref_vect::Ref::Ref(Ref& other )"); this->mRefTake(other); }

		typedef ref_vect<T,REF,REF_C> RefVect_t;

		RefVect_t& operator *()  { G_VERBOSE_MSG_L5("T& ref_vect::Ref::operator *()"); return *this->mGetObjP(); }
		RefVect_t* operator ->() { G_VERBOSE_MSG_L5("T& ref_vect::Ref::operator ->()");return this->mGetObjP(); }
		const RefVect_t& operator *() const  { G_VERBOSE_MSG_L5("const T& ref_vect::Ref::operator *()");return *this->mGetObjP(); }
		const RefVect_t* operator ->() const { G_VERBOSE_MSG_L5("const T* ref_vect::Ref::operator ->()");return this->mGetObjP(); }

		Ref& operator = ( const Ref& other ) { this->mRefTake(*this); return *this; }

      static Ref g_cont_new ( AllocationPolicyAbstract* aAllocPolicyP = AllocationPolicyAbstract::get_FromStandardPolicy ( ) , int aAllocDeltaRBits = G_VECT_AL_DELTA_MASK_BITS )
      {
         Ref result;

         new ( result.mAllocateMemForObject(aAllocPolicyP) )ref_vect<T>(aAllocPolicyP,aAllocDeltaRBits);
         result.mIncRef ( );

         return result;
      }
	};
};


}//namespace cont
}//namespace g


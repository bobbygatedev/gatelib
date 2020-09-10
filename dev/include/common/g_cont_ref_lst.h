#pragma once

#include "g_cont_lst_base.h"

namespace g
{
namespace cont
{

template<class T, class REF = ref<T> , class REF_C = ref_const<T> , class IT=it_ref<T,REF,REF_C>, class IT_C=it_ref_const<T,REF,REF_C> > class ref_lst : public lst_base<T,REF,REF_C,IT,IT_C>
{
public:
	typedef REF                           Ref_t;
	typedef REF_C                         RefConst_t;
	typedef IT                            It_t;
	typedef IT_C                          ItConst_t;
	typedef lst_base<T,REF,REF_C,IT,IT_C> Base_t;

	template <class> friend class priv::list_positioner;

	ref_lst ( AllocationPolicyAbstract* ap = AllocationPolicyAbstract::get_FromStandardPolicy() , int aListPageRightBits = G_LST_PAGE_MASK_BITS , int aVectorAllocDeltaRightBits = G_VECT_AL_DELTA_MASK_BITS ) : Base_t(ap,aListPageRightBits,aVectorAllocDeltaRightBits){}

	virtual ~ref_lst ( ) {}

	class Ref : public ref_custom< ref_lst<T,REF,REF_C> >
	{
	public:
		Ref(){}
		Ref(const Ref& other ){ G_VERBOSE_MSG_L5("vect::Ref::Ref(Ref& other )"); this->mRefTake(other); }

		typedef ref_lst<T,REF,REF_C> Lst_t;

		Lst_t& operator *()  { G_VERBOSE_MSG_L5("T& vect::Ref::operator *()"); return *this->mGetObjP(); }
		Lst_t* operator ->() { G_VERBOSE_MSG_L5("T& vect::Ref::operator ->()");return this->mGetObjP(); }
		const Lst_t& operator *() const  { G_VERBOSE_MSG_L5("const T& vect::Ref::operator *()");return *this->mGetObjP(); }
		const Lst_t* operator ->() const { G_VERBOSE_MSG_L5("const T* vect::Ref::operator ->()");return this->mGetObjP(); }

		Ref& operator = ( const Ref& other ) { this->mRefTake(*this); return *this; }

      static Ref g_cont_new ( AllocationPolicyAbstract* aAllocPolicyP = AllocationPolicyAbstract::get_FromStandardPolicy() , int aListPageRightBits = G_LST_PAGE_MASK_BITS , int aVectorAllocDeltaRightBits = G_VECT_AL_DELTA_MASK_BITS )
      {
         Ref result;

         new ( result.mAllocateMemForObject(aAllocPolicyP) )ref_lst<T>(aAllocPolicyP,aListPageRightBits,aVectorAllocDeltaRightBits);
         result.mIncRef ( );

         return result;
      }
	};
};


}//namespace cont
}//namespace g


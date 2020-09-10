#pragma once

#include "g_cont_lst_base.h"

namespace g
{
namespace cont
{

template < class T , class IT = it<T> , class IT_C = it_const<T> > class lst : public lst_base<T,T,const T,IT,IT_C>
{
public:
	template<class> friend class priv::list_positioner;
	typedef lst_base<T,T,const T,IT,IT_C> Base_t;

	typedef typename Base_t::It_t It_t;
	typedef typename Base_t::ItConst_t ItConst_t;

	lst ( AllocationPolicyAbstract* = AllocationPolicyAbstract::get_FromStandardPolicy() , int = G_LST_PAGE_MASK_BITS , int = G_VECT_AL_DELTA_MASK_BITS );
	lst ( const T* , size_t , AllocationPolicyAbstract* = AllocationPolicyAbstract::get_FromStandardPolicy() , int = G_LST_PAGE_MASK_BITS , int = G_VECT_AL_DELTA_MASK_BITS );
	lst ( const lst& , AllocationPolicyAbstract* = AllocationPolicyAbstract::get_FromStandardPolicy() , int = G_LST_PAGE_MASK_BITS , int = G_VECT_AL_DELTA_MASK_BITS );
	
	virtual ~lst ( ) {}

	void setContentFromVector   ( const T* , size_t );

	class Ref : public ref_custom< lst<T,IT,IT_C> >
	{
	public:
		Ref(){}
		Ref(const Ref& other ){ G_VERBOSE_MSG_L5("vect::Ref::Ref(Ref& other )"); this->mRefTake(other); }

		typedef lst<T,IT,IT_C> Lst_t;

		Lst_t& operator *()  { G_VERBOSE_MSG_L5("T& vect::Ref::operator *()"); return *this->mGetObjP(); }
		Lst_t* operator ->() { G_VERBOSE_MSG_L5("T& vect::Ref::operator ->()");return this->mGetObjP(); }
		const Lst_t& operator *() const  { G_VERBOSE_MSG_L5("const T& vect::Ref::operator *()");return *this->mGetObjP(); }
		const Lst_t* operator ->() const { G_VERBOSE_MSG_L5("const T* vect::Ref::operator ->()");return this->mGetObjP(); }

		Ref& operator = ( const Ref& other ) { this->mRefTake(*this); return *this; }

      static Ref g_cont_new ( AllocationPolicyAbstract* aAllocPolicyP = AllocationPolicyAbstract::get_FromStandardPolicy() )
      {
         Ref result;

         new ( result.mAllocateMemForObject(aAllocPolicyP) )lst<T,IT,IT_C>(aAllocPolicyP);
         result.mIncRef ( );

         return result;
      }
	};
};	

template < class T , class IT, class IT_C> lst<T,IT,IT_C>::lst ( AllocationPolicyAbstract* aAllocPolicyP , int aListPageRightBits  , int aVectorAllocDeltaRightBits ) :
	Base_t(aAllocPolicyP, aListPageRightBits , aVectorAllocDeltaRightBits ) { }

template < class T , class IT, class IT_C> lst<T,IT,IT_C>::lst (const T* aVectorP , size_t aSize , AllocationPolicyAbstract* aAllocPolicyP , int aListPageRightBits  , int aVectorAllocDeltaRightBits ) :
	Base_t(aAllocPolicyP, aListPageRightBits , aVectorAllocDeltaRightBits )
{
	this->setContentFromVector (aVectorP, aSize);
}

template < class T , class IT, class IT_C> lst<T,IT,IT_C>::lst( const lst<T,IT,IT_C>& other , AllocationPolicyAbstract* aAllocPolicyP , int aListPageRightBits  , int aVectorAllocDeltaRightBits ) :
	Base_t(aAllocPolicyP, aListPageRightBits , aVectorAllocDeltaRightBits )
{
	this->setContent(other);
}

template < class T , class IT, class IT_C> void lst<T,IT,IT_C>::setContentFromVector   ( const T* aVectorP , size_t aSize )
{
	this->setEmpty ( );

	for ( size_t i = 0 ; i < aSize ; i++ )
	{
		this->pushTail ( aVectorP[i] );
	}
}


}//namespace cont
}//namespace g

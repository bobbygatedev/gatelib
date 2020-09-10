#pragma once

#include "g_cont_ref.h"
#include "g_cont_gmap_common.h"

namespace g
{
namespace cont
{

template < class KEY , class T , class IT > struct gmap_searcher 
{
	typedef T                                       Value_t;
	typedef KEY                                     Key_t;
	typedef typename comparer<KEY>::Operator_t KeyComparer_t;

	static bool search_Value ( const Value_t& aValue , IT& aIterator )
	{
		for ( ; aIterator.isIn ( ) ; aIterator++ )
		{
			if ( aIterator->value ( ) == aValue )
			{
				return true;
			}
		}

		return false;
	}

	static bool search_Key ( const Key_t& aKey , IT& aIterator , KeyComparer_t /*not used*/ )
	{
		for ( ; aIterator.isIn ( ) ; aIterator++ )
		{
			if ( aIterator->key ( ) == aKey )
			{
				return true;
			}
		}	

		return false;
	}
};

template < class KEY , class T , class REF_C = const T&> class gmap : public gmap_common <KEY,T,gmap_searcher,REF_C>
{
public:
	typedef gpair <KEY,T>                 Pair_t;
	typedef lst <Pair_t>                  PairLst_t;
	typedef typename PairLst_t::It_t      It_t;
	typedef typename PairLst_t::ItConst_t ItConst_t;
	typedef KEY                           Key_t;
	typedef T                             Value_t;
	typedef REF_C                         ConstRef_t;
	typedef gmap_common <KEY,T,gmap_searcher,REF_C> Base_t;

	gmap ( 
		AllocationPolicyAbstract* aAllocPolicyP              = AllocationPolicyAbstract::get_FromStandardPolicy() , 
		int                       aListPageRightBits         = G_LST_PAGE_MASK_BITS , 
		int                       aVectorAllocDeltaRightBits = G_VECT_AL_DELTA_MASK_BITS ) : 
			Base_t(aAllocPolicyP,aListPageRightBits,aVectorAllocDeltaRightBits) {}
	

	//returns true if the value is added, otherwise the value is updated
	virtual bool doAdd ( const Key_t& , const Value_t& );
};

template < class KEY , class T , class REF=ref<T> , class REF_C = ref_const<T> > class ref_gmap : public gmap <KEY,REF,REF_C>
{
public:
	typedef gpair <KEY,REF>               Pair_t;
	typedef lst <Pair_t>                  PairLst_t;
	typedef typename PairLst_t::It_t      It_t;
	typedef typename PairLst_t::ItConst_t ItConst_t;
	typedef KEY                           Key_t;
	typedef T                             Value_t;
	typedef REF                           Ref_t;
	typedef REF_C                         ConstRef_t;
	typedef gmap <KEY,REF,REF_C>           Base_t;

	ref_gmap ( 
		AllocationPolicyAbstract* aAllocPolicyP              = AllocationPolicyAbstract::get_FromStandardPolicy() , 
		int                       aListPageRightBits         = G_LST_PAGE_MASK_BITS , 
		int                       aVectorAllocDeltaRightBits = G_VECT_AL_DELTA_MASK_BITS ) : 
			Base_t(aAllocPolicyP,aListPageRightBits,aVectorAllocDeltaRightBits) {}
	
};

template < class KEY , class T , class REF_C > bool gmap<KEY,T,REF_C>::doAdd ( const Key_t& aKey, const Value_t& aValue)
{
	It_t it = PairLst_t::getIterator ( );

	if ( gmap_searcher<Key_t,Value_t,It_t>::search_Key ( aKey , it , this->getComparer() ) )
	{
		it->value ( ) = aValue;
		return false;
	}
	else 
	{
		this->pushTail ( Pair_t(aKey,aValue) ); 
		return true;
	}
}

}//namespace cont
}//namespace g


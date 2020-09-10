#pragma once

#include "g_cont_gmap_common.h"
#include "g_cont_ref.h"

namespace g
{
namespace cont
{

#define G_LOCAL_CLASS "g::cont::gmap_sorted<KEY_T>::"

template < class KEY , class T , class IT > struct map_sorted_searcher 
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

	// Returns true if the search succeeds 'aIterator' is positioned on the iten corresponding to aKey
	// or to the last item where aKey < content(aIterator)
	// i.e map={ {1,"one"} , {3,"three"} , {4,"four"} } search_Key ( 2 , it ) will return false and it point to {1,"one"}
	static bool search_Key ( const Key_t& aKey , IT& aIterator , KeyComparer_t aLessComparer )
	{
		G_VERBOSE_MSG_L1("gmap_sorted:search_Key");

		//uses the ordering 
		for ( ; aIterator.isIn ( ) ; aIterator++ )
		{
			if ( (*aLessComparer)( aIterator->key ( ) , aKey ) )
			{
				G_VERBOSE_MSG_L1( "aIterator->key ( ) = "  << aIterator->key ( ) << " is 'less' than " << aKey << endl );

				continue;
			}
			else
			{
				G_VERBOSE_MSG_L1( "aIterator->key ( ) = "  << aIterator->key ( ) << " is not 'less' than " << aKey << endl );

				return ( aIterator->key ( ) == aKey );
			}
		}	

		return false;
	}
};

template < class KEY , class T , class REF_C = const T&> class gmap_sorted : public gmap_common <KEY,T,map_sorted_searcher,REF_C>
{
public:
	typedef gpair <KEY,T>                                 Pair_t;
	typedef lst <Pair_t>                                  PairLst_t;
	typedef typename PairLst_t::It_t                      It_t;
	typedef typename PairLst_t::ItConst_t                 ItConst_t;
	typedef KEY                                           Key_t;
	typedef T                                             Value_t;
	typedef REF_C                                         ConstRef_t;
	typedef gmap_common <KEY,T,map_sorted_searcher,REF_C> Base_t;
	typedef typename comparer<KEY>::Operator_t       KeyComparer_t;

	gmap_sorted ( 
		KeyComparer_t             aKeyLessComparer           = comparer<KEY>::less , 
		AllocationPolicyAbstract* aAllocPolicyP              = AllocationPolicyAbstract::get_FromStandardPolicy() , 
		int                       aListPageRightBits         = G_LST_PAGE_MASK_BITS , 
		int                       aVectorAllocDeltaRightBits = G_VECT_AL_DELTA_MASK_BITS ) : 
			Base_t(aAllocPolicyP,aListPageRightBits,aVectorAllocDeltaRightBits), 
			mKeyLessComparer(aKeyLessComparer) {}

	//returns true if the value is added, otherwise the value is updated
	virtual bool doAdd    ( const Key_t& , const Value_t& );

	virtual KeyComparer_t getComparer ( ) const { return mKeyLessComparer; }

private:
	KeyComparer_t mKeyLessComparer;
};

template < class KEY , class T , class REF=ref<T> , class REF_C = ref_const<T> > class ref_gmap_sorted : public gmap_sorted <KEY,REF,REF_C>
{
public:
	typedef gpair <KEY,REF>                         Pair_t;
	typedef lst <Pair_t>                            PairLst_t;
	typedef typename PairLst_t::It_t                It_t;
	typedef typename PairLst_t::ItConst_t           ItConst_t;
	typedef KEY                                     Key_t;
	typedef T                                       Value_t;
	typedef REF                                     Ref_t;
	typedef REF_C                                   ConstRef_t;
	typedef gmap_sorted <KEY,REF,REF_C>             Base_t;
	typedef typename comparer<KEY>::Operator_t KeyComparer_t;

	ref_gmap_sorted ( 
		KeyComparer_t             aKeyLessComparer           = comparer<KEY>::less ,
		AllocationPolicyAbstract* aAllocPolicyP              = AllocationPolicyAbstract::get_FromStandardPolicy() , 
		int                       aListPageRightBits         = G_LST_PAGE_MASK_BITS , 
		int                       aVectorAllocDeltaRightBits = G_VECT_AL_DELTA_MASK_BITS ) : 
			Base_t(aKeyLessComparer,aAllocPolicyP,aListPageRightBits,aVectorAllocDeltaRightBits){}
};

template < class KEY , class T , class REF_C>  bool gmap_sorted <KEY,T,REF_C>::doAdd(const Key_t &aKey, const Value_t &aValue)
{
	It_t it = PairLst_t::getIterator ( );

	if ( map_sorted_searcher<Key_t,Value_t,It_t>::search_Key ( aKey , it , this->getComparer ()) )
	{
		G_VERBOSE_MSG_L1 ("Key: "  << aKey << " supreseeding value " << it->value ( ) );
		it->value ( ) = aValue;
		return false;
	}
	else 
	{
		if ( it.isIn ( ) )
		{
			G_VERBOSE_MSG_L1 ("Key: "  << aKey << " before of " << it->key ( ) );
			
			this->pushBefore ( Pair_t(aKey,aValue) , it ); 
		}
		else
		{
			G_VERBOSE_MSG_L1 ("Key: " << aKey << " at end!" );

			this->pushTail( Pair_t(aKey,aValue) ); 		
		}
		return true;
	}
}

#undef G_LOCAL_CLASS

}//namespace cont
}//namespace g


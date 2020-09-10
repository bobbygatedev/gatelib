#pragma once

#include "g_cont_base_cont.h"
#include "g_cont_ref_custom.h"
#include "private/g_cont_lst_priv.h"

namespace g
{
namespace cont
{

template<class T, class REF, class REF_C , class IT , class IT_C > class lst_base : public base_cont<T,REF,REF_C,IT,IT_C>
{
public:
	typedef REF   Ref_t;
	typedef REF_C RefConst_t;
	typedef IT    It_t;
	typedef IT_C  ItConst_t;

	template <typename> friend class priv::list_positioner;

protected:
	lst_base ( AllocationPolicyAbstract* , int aListPageRightBits , int aVectorAllocDeltaRightBits );

public:
	virtual ~lst_base ( ) {}

	virtual void pushAfter  ( const Ref_t& item , const It_t& where ) { mContent.pushNewNodeAfter ( item , this->mTestIterator(where) );	}
	virtual void pushBefore ( const Ref_t& item , const It_t& where ) { mContent.pushNewNodeBefore ( item , this->mTestIterator(where) );	}
	virtual void setEmpty    ( ) { mContent.empty(); }
	virtual void eraseMemory ( ) { mContent.erase(); }

	Ref_t&      operator [] ( int i ) { return *this->mSequentialGet (i); }
	RefConst_t& operator [] ( int i ) const { return *this->mSequentialGet (i); }

	virtual int getLbound ( ) const { return 0; }
	virtual int getUbound ( ) const { return (int)mContent.size - 1; }

	virtual Ref_t remove ( const It_t& where )	{ return mContent.remove ( this->mTestIterator(where,true) ); }

protected:
	priv::list_content<REF>    mContent;
	priv::list_positioner<REF> mPositioner;
};

template<class T, class REF, class REF_C , class IT , class IT_C > lst_base<T,REF,REF_C,IT,IT_C>::lst_base ( 
	AllocationPolicyAbstract* aAllocPolicyP , 
	int                       aListPageRightBits , 
	int                       aVectorDeltaRoghtBits ) : 
		base_cont<T,REF,REF_C,IT,IT_C> ( aAllocPolicyP , &mPositioner ),
		mContent(aAllocPolicyP,aListPageRightBits,aVectorDeltaRoghtBits) ,
		mPositioner(&mContent){}

}//namespace cont
}//namespace g


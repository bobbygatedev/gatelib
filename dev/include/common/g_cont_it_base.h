#pragma once

#include "g_cont_it_unspec.h"

namespace g
{
namespace cont
{
template<class T, class REF, class REF_C , class IT , class IT_C > class base_cont;
template<class T> class cont_with_positioner;

template < class T , class REF , class REF_C > class it_base : public ItUnspec
{
private:
	it_base(const it_base&) : mRecipientP ( NULL ) , mIndex ( G_CONT_INDEX_NOT_VALID ) {}//copy of iterator prohibited

public:
	typedef cont_with_positioner<REF> RecipientBase_t;
	friend class cont_with_positioner<REF>;
	template<class,class,class,class,class> friend class base_cont;

	typedef REF   Ref_t; 
	typedef REF_C RefConst_t; 

	virtual ~it_base() {}

	virtual bool isIn     ( ) const { return ( mRecipientP != NULL && mRecipientP->mPositionerP->isIn(mIndex) ); }
	virtual bool isMine   ( const RecipientBase_t& aRecipient ) const { return ( &aRecipient == mRecipientP ); }
	virtual void toBegin  ( ) { mIndex = mRecipientP->mPositionerP->first ( ); }
	virtual void toEnd    ( ) { mIndex = mRecipientP->mPositionerP->last ( ); }
	virtual void forward  ( GUint32_t inc = 1 ) { mRecipientP->mPositionerP->forward  ( mIndex , inc ); }
	virtual void backward ( GUint32_t dec = 1 ) { mRecipientP->mPositionerP->backward ( mIndex , dec ); }
	//if inc > 0 forward otherwise backward
	virtual void moveOf  ( int inc ) { (inc>0)?forward((GUint32_t)inc):backward((GUint32_t)-inc); }
	virtual bool isNull  ( ) const { return ( mIndex == G_CONT_INDEX_NOT_VALID && mRecipientP == NULL ); }

	bool operator == ( const it_base& aOther ) const { return aOther.mIndex == mIndex; }
	bool operator != ( const it_base& aOther ) const { return !operator==(aOther); }

protected:
	it_base ( ) : mRecipientP ( NULL ) , mIndex ( G_CONT_INDEX_NOT_VALID ) {}

	virtual void mInit           ( const RecipientBase_t& aRec , IterFrom_t aIterFrom );
	virtual void mCopy           ( const it_base& );

	const RecipientBase_t* mRecipientP;
	int                    mIndex;
};

template < class T , class REF , class REF_C > void it_base<T,REF,REF_C>::mInit ( const RecipientBase_t& aRec , IterFrom_t aIterFrom )
{
	mRecipientP = &aRec;

	switch ( aIterFrom )
	{
		case head: mIndex = aRec.mPositionerP->first ( );break;
		case tail: mIndex = aRec.mPositionerP->last  ( );break;
	}

	G_VERBOSE_MSG_L3("Init iterator recipient " << std::hex << (int)mRecipientP << std::dec );
}

template < class T , class REF , class REF_C > void it_base <T,REF,REF_C>::mCopy  ( const it_base<T,REF,REF_C>& aIt )
{
	mRecipientP = aIt.mRecipientP;
	mIndex      = aIt.mIndex;
}

}//namespace cont
}//namespace g


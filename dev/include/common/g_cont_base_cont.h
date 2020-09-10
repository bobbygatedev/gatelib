#pragma once

#include "g_cont_cont_with_positioner.h"
#include "g_cont_it_ref.h"
#include "g_cont_it.h"
#include "g_cont_ref.h"
#include "g_cont_alg.h"

namespace g
{
namespace cont
{
template < class T , class REF , class REF_C > class it_base;

G_EXC_DEFINE(NotAValidIteratorException,ContException);
G_EXC_DEFINE(EmptyContException,ContException);

//base class for recipient of references
template<class T, class REF, class REF_C , class IT , class IT_C > class base_cont : public cont_with_positioner<REF>
{
public:
	template <class,class,class> friend class it_base;
	template <class,class,class,class,class> friend class lst_base;
	template <class,class,class,class,class> friend class vect_base;

	typedef REF   Ref_t;
	typedef REF_C RefConst_t;
	typedef IT    It_t;
	typedef IT_C  ItConst_t;

	typedef cont_with_positioner<REF> RecipientBase_t;
	typedef positioner_abstract<REF>  Positioner_t;

	friend class positioner_abstract<REF>;

protected:
	base_cont( AllocationPolicyAbstract* aAllocPolicyP , Positioner_t* positioner_p ) : cont_with_positioner<REF>(aAllocPolicyP,positioner_p){}

public:
	virtual ~base_cont()	{}

	bool operator == ( const base_cont& other ) const;

	virtual Ref_t remove      ( const It_t& ) = 0;
	virtual void  eraseMemory ( ) = 0;

	virtual void pushAfter  ( const Ref_t& , const It_t& ) = 0;
	virtual void pushBefore ( const Ref_t& , const It_t& ) = 0;

	virtual void pushHead   ( const Ref_t& aItem )	{ this->pushBefore(aItem, It_t(*this, head)); }
	virtual void pushTail   ( const Ref_t& aItem )  { this->pushAfter(aItem, It_t(*this, tail)); }

	virtual Ref_t popHead()	{ return this->remove(It_t(*this, head)); }
	virtual Ref_t popTail()	{ return this->remove(It_t(*this, tail)); }

	virtual RefConst_t getHead() const { return ItConst_t(*this,head).underlying(); }
	virtual Ref_t      getHead()       { return It_t(*this,head).underlying(); }

	virtual RefConst_t getTail() const { return ItConst_t(*this,tail).underlying(); }
	virtual Ref_t      getTail()	   { return It_t(*this,tail).underlying(); }

	virtual It_t       getIterator ( IterFrom_t from = head ) { return It_t(*this,from);  }
	virtual ItConst_t  getIterator ( IterFrom_t from = head ) const { return ItConst_t(*this,from);  }

	virtual void setContent ( base_cont& other );

	bool      contains  ( const RefConst_t& value ) const { return search(value).isIn(); }
	It_t      search    ( const Ref_t& item ) { return alg::search ( getIterator(head) , item ); }
	ItConst_t search    ( const RefConst_t& item ) const { return alg::search ( getIterator(head) , item ); }
	bool      tryRemove ( const Ref_t& item ) { return alg::try_remove(*this,item); }
	size_t    removeAll ( const Ref_t& item ) { return alg::remove_all(*this,item); }

private:
	inline int mDeref ( const It_t& where ) const { return where.mIndex; }
	inline int mTestIterator( const It_t& , bool exc_if_empty = false ) const;
	inline Ref_t* mSequentialGet ( int i ) const;
};

template<class T, class REF, class REF_C , class IT , class IT_C > bool base_cont<T,REF,REF_C,IT,IT_C>::operator == ( const base_cont<T,REF,REF_C,IT,IT_C>& aOther ) const
{
	ItConst_t my_it(*this);

	if ( this->getSize() != aOther.getSize() )
	{
		return false;
	}

	for ( ItConst_t oth_it ( aOther ) ; oth_it.isIn ( ) ; oth_it++ , my_it++ )
	{
		if(my_it.underlying() != oth_it.underlying())
		{
			return false;
		}
	}

	return true;
}

template<class T, class REF, class REF_C , class IT , class IT_C >
	void base_cont<T,REF,REF_C,IT,IT_C>::setContent ( base_cont<T,REF,REF_C,IT,IT_C>& aOther )
{
	this->setEmpty();

	for ( It_t it(aOther) ; it.isIn() ; it++ )
	{
		pushTail ( it.underlying() );
	}
}

//Check the iterator is valid and throw the exception in case of the iterator is not belongin to THIS recipient
template<class T, class REF, class REF_C , class IT , class IT_C > int base_cont<T,REF,REF_C,IT,IT_C>::mTestIterator( const It_t& aWhere , bool aRaiseExcIfEmpty ) const
{
   G_EXC_SET_CONTEXT ( "int base_cont<T,REF,REF_C,IT,IT_C>::mTestIterator( const It_t& aWhere , bool aRaiseExcIfEmpty ) const"  );

	//Iterator not belonging to the object
	if (!aWhere.isMine(*this))
	{
		G_EXC_RAISE_MSG ( NotAValidIteratorException , "Iterator is not belonging to this ref_base_cont." );
	}

	if ( aWhere.isIn () )
	{
		return mDeref ( aWhere );
	}
	else if (this->getSize() > 0)
	{
		G_EXC_RAISE_MSG(NotAValidIteratorException , "Iterator pointer no longer valid(Probably content has changed or reallocated).");
	}
	else if(aRaiseExcIfEmpty)
	{
		G_EXC_RAISE_MSG ( EmptyContException , "Item not found!" );
	}

   return G_CONT_INDEX_NOT_VALID;	
}

//Used by list, iterates the positioner i-times
template<class T, class REF, class REF_C , class IT , class IT_C > REF* base_cont<T,REF,REF_C,IT,IT_C>::mSequentialGet(int aI) const
{
   G_EXC_SET_CONTEXT ( "REF* base_cont<T,REF,REF_C,IT,IT_C>::mSequentialGet(int aI) const" );

	if ( aI < 0 )
	{
		G_EXC_RAISE_CONT ( "Index < 0" );
      return (REF*)NULL;
	}
	else
	{
		int index = this->mPositionerP->first();

		if ( index != G_CONT_INDEX_NOT_VALID )
		{
			this->mPositionerP->forward(index,aI);
		}

		if ( index == G_CONT_INDEX_NOT_VALID )
		{
			G_EXC_RAISE_CONT( "Index out of bounds!" );
		}

      return this->mPositionerP->getPtr ( index );
	}
}

} //namespace cont
} //namespace g


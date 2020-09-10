#pragma once

#include "g_cont_base_cont.h"
#include "private/g_cont_vect_priv.h"

namespace g
{
namespace cont
{

G_EXC_DEFINE(IndexOutOfBoundsException,ContException);
G_EXC_DEFINE(ReducingCapacityException,ContException);

template<class T, class REF, class REF_C , class IT , class IT_C > class vect_base : public base_cont<T,REF,REF_C,IT,IT_C>
{
private:
	vect_base ( const vect_base& ) {} //prohibited copy constructor
public:
	template <class> friend class priv::vect_positioner;
	template <class> friend struct priv::vect_content;

	typedef REF   Ref_t;
	typedef REF_C RefConst_t;
	typedef IT    It_t;
	typedef IT_C  ItConst_t;

protected:
	vect_base ( AllocationPolicyAbstract* , int alloc_delta_right_bits );

public:
	virtual ~vect_base ( ) {}

	virtual void pushAfter  ( const Ref_t& item , const It_t& where );
	virtual void pushBefore ( const Ref_t& item , const It_t& where );

	Ref_t&      operator [] ( int i );
	RefConst_t& operator [] ( int i ) const;

	virtual int  getLbound  ( ) const { return mContent.lbound; }
	virtual int  getUbound  ( ) const { return mContent.ubound; }

	virtual void setEmpty    ( ) { mContent.empty(); }
	virtual void eraseMemory ( ) { mContent.erase(); }

	virtual Ref_t remove ( const It_t& where )	{ return mContent.remove (this->mTestIterator(where,true)); }

	void    setSize   ( int lbound , int ubound ) { mContent.setSize ( lbound , ubound ); }
	void    setSize   ( size_t size ) { setSize ( 0 , (int)size - 1 ); }

	void    reSize   ( int lbound , int ubound ) { mContent.resize ( lbound , ubound ); }
	void    reSize   ( size_t size ) { reSize ( 0 , (int)size-1 ); }

	void    setCapacity ( size_t );

	size_t getCapacity ( ) const { return mContent.capacity; }

protected:
	priv::vect_content<REF>    mContent;
	priv::vect_positioner<REF> mPositioner;
};

template<class T, class REF, class REF_C , class IT , class IT_C > vect_base<T,REF,REF_C,IT,IT_C>::vect_base ( AllocationPolicyAbstract* aAllocPolicyP , int aAllocDeltaRBits ):
	base_cont<T,REF,REF_C,IT,IT_C> ( aAllocPolicyP , &mPositioner ),
	mContent                       ( aAllocPolicyP , aAllocDeltaRBits ) , 
	mPositioner                    ( &mContent ) { }

template<class T, class REF, class REF_C , class IT , class IT_C > void vect_base<T,REF,REF_C,IT,IT_C>::setCapacity ( size_t aCapacity )
{
   G_EXC_SET_CONTEXT ( "vect_base<T,REF,REF_C,IT,IT_C>::setCapacity ( size_t aCapacity )" );
	
   if ( aCapacity >= this->getSize ( ) )
	{
		mContent.changeCapacity ( aCapacity );
	}
	else
	{
		G_EXC_RAISE_MSG ( ReducingCapacityException ,"Capacity can't be < of the size" );
	}
}

template<class T, class REF, class REF_C , class IT , class IT_C > typename vect_base<T,REF,REF_C,IT,IT_C>::Ref_t& vect_base<T,REF,REF_C,IT,IT_C>::operator [] ( int aI )
{	
   G_EXC_SET_CONTEXT ( "typename vect_base<T,REF,REF_C,IT,IT_C>::Ref_t& vect_base<T,REF,REF_C,IT,IT_C>::operator [] ( int aI )" );

	if ( this->isAnIndex ( aI ) )
	{
		return *(mContent.data + aI - getLbound()); 
	}
	else
	{
		G_EXC_RAISE_MSG ( IndexOutOfBoundsException , "Index out of bounds!" );			
      return *(mContent.data); //unreachable avoid warning
	}
}

template<class T, class REF, class REF_C , class IT , class IT_C > typename vect_base<T,REF,REF_C,IT,IT_C>::RefConst_t& vect_base<T,REF,REF_C,IT,IT_C>::operator [] ( int aI ) const
{
   G_EXC_SET_CONTEXT ( "typename vect_base<T,REF,REF_C,IT,IT_C>::RefConst_t& vect_base<T,REF,REF_C,IT,IT_C>::operator [] ( int aI ) const" );
	
   if ( this->isAnIndex ( aI ) )
	{
		return *(mContent.data + aI - getLbound()); 
	}
	else
	{
		G_EXC_RAISE_MSG ( IndexOutOfBoundsException , "Index out of bounds!" );
	}
}

template<class T, class REF, class REF_C , class IT , class IT_C > void vect_base<T,REF,REF_C,IT,IT_C>::pushAfter ( const Ref_t& aItem , const It_t& aWhere )
{
	if ( this->getSize() > 0 )
	{
		mContent.pushAfter ( this->mTestIterator(aWhere) , aItem );
	}
	else
	{
		mContent.replaceContent ( 0 , 0 , &aItem );
	}
}

template<class T, class REF, class REF_C , class IT , class IT_C > void vect_base<T,REF,REF_C,IT,IT_C>::pushBefore ( const Ref_t& aItem , const It_t& aWhere )
{
	if ( this->getSize() > 0 )
	{
		mContent.pushAfter ( this->mTestIterator(aWhere) - 1 , aItem );
	}
	else
	{
		mContent.replaceContent ( 0 , 0 , &aItem );
	}
}

}//namespace cont
}//namespace g


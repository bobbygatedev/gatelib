#pragma once

#include "g_cont_common.h"
#include "g_cont_lst.h"

namespace g
{
namespace cont
{

G_EXC_DEFINE(KeyNotFoundException,ContException);

template <class T> struct comparer
{
	typedef bool (*Operator_t) ( const T& a1 , const T& a2 );

	static bool less ( const T& a1 , const T& a2 ) { return (a1 < a2); } 
	static bool more ( const T& a1 , const T& a2 ) { return (a1 > a2); } 
};

template < class KEY , class T > class gpair 
{
public:
	typedef KEY Key_t;
	typedef T   Value_t;

	gpair ( const Key_t& aKey , const Value_t& aValue ) : mKey ( aKey ) , mValue ( aValue ) {}

	gpair& operator = ( const gpair& aPair ) { mKey = aPair.mKey ; mValue = aPair.mValue; return *this; }
	
	const Key_t&   key   ( ) const { return mKey; };
	const Value_t& value ( ) const { return mValue; }

	Key_t&   key   ( ) { return mKey; };
	Value_t& value ( ) { return mValue; }

private:
	Key_t   mKey;
	Value_t mValue; 
};

template < class KEY , class T , template<class,class,class>class SRC , class REF_C = const T& > class gmap_common : protected lst < gpair <KEY,T> >
{
public:
	typedef gpair <KEY,T>                           Pair_t;
	typedef lst < gpair <KEY,T> >                   PairLst_t;
	typedef typename PairLst_t::It_t                It_t;
	typedef typename PairLst_t::ItConst_t           ItConst_t;
	typedef KEY                                     Key_t;
	typedef T                                       Value_t;
	typedef REF_C                                   ConstRef_t;
	typedef typename comparer<KEY>::Operator_t KeyComparer_t;

protected:
	gmap_common ( AllocationPolicyAbstract* ap , int lst_page_mask_bits , int al_delta_mask_bits ):PairLst_t(ap,lst_page_mask_bits,al_delta_mask_bits){}

public:
	Value_t&   operator [] ( const Key_t& aKey );
	ConstRef_t operator [] ( const Key_t& aKey ) const;

	bool isValueContained ( const Value_t& ) const;
	bool isKeyContained   ( const Key_t& ) const;
	bool tryToGet         ( const Key_t& , Value_t& ); 

	virtual void    setEmpty ( ) { PairLst_t::setEmpty ( ); }

	bool            tryRemove ( const Key_t& , Value_t& );
	bool            tryRemove ( const Key_t& );
	Value_t         doRemove  ( const Key_t& );
		
	//returns true if the value is added, otherwise the value is updated
	virtual bool    doAdd    ( const Key_t& , const Value_t& ) = 0;

	size_t getSize ( ) const { return PairLst_t::getLen ( ); } 

	typename PairLst_t::ItConst_t getIterator ( ) const { return PairLst_t::getIterator ( );  }

	const PairLst_t& getPairList ( ) const { return (const PairLst_t&)(*this);  }
	
	//not used by not-sorted map 
	virtual KeyComparer_t getComparer ( ) const { return 0;  }
};

template < class KEY , class T , template<class,class,class>class SRC , class REF_C >  
	typename gmap_common<KEY,T,SRC,REF_C>::Value_t& gmap_common<KEY,T,SRC,REF_C>::operator [] ( const Key_t& aKey )
{
   G_EXC_SET_CONTEXT ( "gmap_common<KEY,T,SRC,REF_C>::Value_t& gmap_common<KEY,T,SRC,REF_C>::operator [] ( const Key_t& aKey )" );

	It_t it = PairLst_t::getIterator ( );

	if ( SRC<Key_t,Value_t,It_t>::search_Key ( aKey , it , this->getComparer() ) )
	{
		return it->value();
	}

	G_EXC_RAISE_MSG ( KeyNotFoundException , "Key not found!" );
   return it->value();//fake avoid warning
}

template < class KEY , class T , template<class,class,class>class SRC , class REF_C > 
	typename gmap_common<KEY,T,SRC,REF_C>::ConstRef_t gmap_common<KEY,T,SRC,REF_C>::operator [] ( const Key_t& aKey ) const 
{
   G_EXC_SET_CONTEXT ( "gmap_common<KEY,T,SRC,REF_C>::ConstRef_t gmap_common<KEY,T,SRC,REF_C>::operator [] ( const Key_t& aKey ) const " );

	ItConst_t it = PairLst_t::getIterator ( );

	if ( SRC<Key_t,Value_t,ItConst_t>::search_Key ( aKey , it , this->getComparer() ) )
	{
		return it->value();
	}

	G_EXC_RAISE_MSG ( KeyNotFoundException , "Key not found!" );
}

template < class KEY , class T , template<class,class,class>class SRC , class REF_C > 
	bool gmap_common<KEY,T,SRC,REF_C>::tryToGet ( const Key_t& aKey , Value_t& aValue )
{
	It_t it = PairLst_t::getIterator ( );

	if ( SRC<Key_t,Value_t,It_t>::search_Key ( aKey , it , this->getComparer() ) )
	{
		aValue = it->value();
		return true;
	}
	else
	{	
		return false;
	}
}

template < class KEY , class T , template<class,class,class>class SRC , class REF_C >
	bool gmap_common<KEY,T,SRC,REF_C>::isValueContained ( const Value_t& aValue ) const
{
	ItConst_t it = PairLst_t::getIterator ( );

	return SRC<Key_t,Value_t,ItConst_t>::search_Value ( aValue , it );
}

template < class KEY , class T , template<class,class,class>class SRC , class REF_C >
	bool gmap_common<KEY,T,SRC,REF_C>::isKeyContained ( const Key_t& aKey ) const
{
	ItConst_t it = PairLst_t::getIterator ( );

	return SRC<Key_t,Value_t,ItConst_t>::search_Key ( aKey , it , this->getComparer() );
}

template < class KEY , class T , template<class,class,class>class SRC , class REF_C >
	typename gmap_common<KEY,T,SRC,REF_C>::Value_t gmap_common<KEY,T,SRC,REF_C>::doRemove ( const Key_t& aKey )
{
   G_EXC_SET_CONTEXT ( "gmap_common<KEY,T,SRC,REF_C>::Value_t gmap_common<KEY,T,SRC,REF_C>::doRemove ( const Key_t& aKey )" );
   
   It_t it = PairLst_t::getIterator ( );

	if ( SRC<Key_t,Value_t,It_t>::search_Key ( aKey , it , this->getComparer() ) ) 
	{
		return this->remove ( it ).value();	
	}
	else
	{
		G_EXC_RAISE_MSG ( KeyNotFoundException , "Key not found!" );
      return it->value();//fake avoid 
	}
}


template < class KEY , class T , template<class,class,class>class SRC , class REF_C >
	bool gmap_common<KEY,T,SRC,REF_C>::tryRemove ( const Key_t& aKey , Value_t& aValue )
{
	It_t it = PairLst_t::getIterator ( );

	if ( SRC<Key_t,Value_t,It_t>::search_Key ( aKey , it , this->getComparer() ) )
	{
		aValue = it->value ( );
		this->remove ( it );
		return true;
	}
	else
	{
		return false;
	}
}

template < class KEY , class T , template<class,class,class>class SRC , class REF_C >
	bool gmap_common<KEY,T,SRC,REF_C>::tryRemove ( const Key_t& aKey )
{
	It_t it = PairLst_t::getIterator ( );

	if ( SRC<Key_t,Value_t,It_t>::search_Key ( aKey , it , this->getComparer() ) )
	{
		this->remove ( it );
		return true;
	}
	else
	{
		return false;
	}
}

#undef G_LOCAL_CLASS

}//namespace cont
}//namespace g


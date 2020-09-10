#pragma once

#include "g_common_def.h"
#include <sstream>

namespace g
{
namespace cont
{

template <class T> class gstr;

namespace alg
{

template < class IT1 , class IT2 > inline size_t copy ( IT1 aFrom , IT1 aTo , IT2 aDestination )
{
	size_t result=0;
	
	for (  ; aFrom != aTo && aFrom.isIn(); result++ , aDestination++ , aFrom++ )
	{
		aDestination.underlying() = aFrom.underlying();
	}

	return result;
}

template < class IT1 , class IT2 > inline size_t copy ( IT1 aFrom , IT2 aDestination )
{
	size_t result=0;

	for (  ; aFrom.isIn(); result++ , aDestination++ , aFrom++ )
	{
		aDestination.underlying() = aFrom.underlying();
	}

	return result;
}

template < class T , class IT2 > inline size_t copy ( const T* aVectorP , size_t aSize , IT2 aDestination )
{
	for ( int i = 0 ; i < (int)aSize ; i++ )
	{
		aDestination.underlying() = aVectorP[i];
	}

	return aSize;
}

template < class IT1 , class IT2 > inline  size_t copy_reverse ( IT1 aFrom , IT1 aTo , IT2 aDestination )
{
	size_t result=0;
	
	for (  ; aFrom != aTo ; result++ , aDestination++ , aFrom-- )
	{
		aDestination.underlying() = aFrom.underlying();
	}

	return result;
}

template < class T , class IT1 > inline IT1 search ( IT1 aFrom , IT1 aTo , T aValue )
{
	for ( ; aFrom.isIn() ; aFrom++ )
	{
		if ( aFrom.underlying() == aValue )
		{
			return aFrom;
		}

		if ( aFrom == aTo )
		{
			return IT1();//search failed
		}
	}

	return IT1();
}

template < class T , class IT1 > inline IT1 search ( IT1 aFrom , T aValue )
{
	for ( ; aFrom.isIn() ; aFrom++ )
	{
		if ( aFrom.underlying() == aValue )
		{
			return aFrom;
		}
	}

	return IT1();
}

template < class R, class T >
	inline gstr<T> get_cont_string ( const R& aCont , const T* aSeparator = (const T*)gstr<T>::space() )
{
	typename R::ItConst_t it(aCont);

	std::basic_stringstream<T> output;

	if(it.isIn())
	{
		output << *it;

		for( ++it ; it.isIn() ;  it++ )
		{
			output << aSeparator << *it;
		}
	}

	return output.str().c_str();
}

//tries to remove the first occurence of aItem returns true if it succeeds.
template < class T , class R > inline  bool try_remove ( R& aRecipient , T aItem )
{
	typename R::It_t it = search( aRecipient.getIterator(head) , aRecipient.getIterator(tail) , aItem );

	if ( it.isIn())
	{
		aRecipient.remove(it);

		return true;
	}
	else
	{
		return false;
	}
}

//removes all occurences item equal to a item, returns the number of 'remove' instances.
template < class T , class R > inline size_t remove_all ( R& aRecipient , T aItem )
{
	typename R::It_t it = search( aRecipient.getIterator(head) , aItem );
	size_t result = 0;//number of removed items

	while ( it.isIn() )
	{
		aRecipient.remove(it);
		result++;
		it = search ( it , aItem );
	}

	return result;
}

}//namespace alg
}//namespace cont
}//namespace g



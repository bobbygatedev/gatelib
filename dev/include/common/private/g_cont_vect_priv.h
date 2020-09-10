#pragma once

#include "g_common.h"

#if G_AUTOPTION_ON_WINDOWS
#include "new"
#endif

#include "g_cont_private_data_pointer.h"
#include "g_cont_AllocationPolicyAbstract.h"

#define VECTOR_ALLOC_DELTA 16 //MUST be power of 2

namespace g
{
namespace cont
{

template < class T , class IT , class IT_C > class vect;

namespace priv
{

template < class T> struct vect_content
{	
	vect_content ( AllocationPolicyAbstract* aAllocPolicyP , int aAllocRightBits ):
		data       ( aAllocPolicyP ) ,
		capacity   ( 0 ) ,
		lbound     ( 0 ) ,
		ubound     ( -1 )
	{
		aAllocRightBits = ( aAllocRightBits < 1 )?1:aAllocRightBits;
		allocDelta = 1 << aAllocRightBits;
	}

	~vect_content(){ erase(); }

	data_pointer<T> data;
	size_t          capacity;
	int	            lbound;
	int             ubound;
	int             allocDelta;


	int first    ( )
	{
		if (ubound>=lbound)
		{
			return 0;
		}
		else
		{
			return G_CONT_INDEX_NOT_VALID;
		}
	}

	int last     ( ) 
	{ 
		if (ubound>=lbound)
		{
			return (int)size()-1;
		}
		else
		{
			return G_CONT_INDEX_NOT_VALID;
		}
	}

	size_t size ( ) { return ubound-lbound+1; }

	//-1 means outside
	int abs_index ( const T* aItem )
	{
		if ( data != NULL )
		{
			int delta = ( int ) ( aItem - data );

			return ( delta >= 0 && delta < (int)size() )?delta:-1;
		}
		else
		{
			return -1;
		}
	}

	//call destructors then set object to empty (without unreserving memory )
	void empty ( )
	{
		for ( int i = 0 ; i < (int)size() ; i++ )
		{
			( data + i )->~T();
		}

		lbound = 0;
		ubound = -1;
	}

	void erase ( ) 
	{
		empty();			

		if ( data )
		{
			data.free ();
		}
	}
	
	void changeCapacity ( size_t aNewCapacity )
	{
		data.allocate ( aNewCapacity , capacity );
		capacity = aNewCapacity;
	}

	void reserveBytes ( int aLbound , int aUbound ) 
	{ 
		long long required_capacity = get_rounded_by_power_of_2( (long long)(aUbound - aLbound + 1) , (long long)allocDelta );

		changeCapacity ( (size_t)required_capacity );
	}

	void pushAfter ( int aIndex , const T& aItem )
	{ 
		reserveBytes ( lbound , ubound + 1 );
		
		move_data ( data + aIndex + 1 , data + aIndex + 2 , size() - aIndex - 1 );

		new ( data + aIndex + 1 ) T ( aItem );

		ubound++;
	}

	void setSize ( int aLbound , int aUbound )
	{
      G_EXC_SET_CONTEXT ( "void g::cont::vect_content<T>::setSize ( int aLbound , int aUbound )" );

      empty ( );

		if ( aUbound >= aLbound )
		{
			int new_size = aUbound - aLbound + 1;

			reserveBytes ( aLbound , aUbound );

			for ( int i = 0 ; i < new_size ; i++ )
			{
				new ( data + i ) T(); //emtpy objects
			}
			
			lbound = aLbound;
			ubound = aUbound;
		}
		else if ( aUbound != -1 || aLbound != 0 )
		{
			G_EXC_RAISE_CONT ( "Lbound > ubound!" );
		}
	}

	void replaceContent  ( int aLbound , int aUbound , const T* aOtherDataP )
	{
		empty ( );
		
		reserveBytes ( aLbound , aUbound );
		
		for ( int i = aLbound ; i <= aUbound ; i++ )
		{
			new ( data + i - aLbound ) T (aOtherDataP[i]);
		}

		lbound = aLbound;
		ubound = aUbound;	
	}

	void resize ( int aLbound , int aUbound )
	{
		if ( !size ( ) )
		{
			setSize ( aLbound , aUbound );
		}
		else if ( aUbound >= aLbound )
		{
			//first index not be delete
			int start = ( aLbound >= lbound ) ? aLbound : lbound;
			int stop  = ( aUbound <= ubound ) ? aUbound : ubound;
			int size_to_move = stop - start + 1;

			//change if required the capacity
			reserveBytes ( aLbound , aUbound );

			//delete unused (left)
			for ( int i = 0 ; i < start - lbound ;  i++ )
			{
				( data + i - lbound )->~T();
			}
			//delete unused (right)
			for ( int i = stop + 1 ; i <= ubound ;  i++ )
			{
				( data + i - lbound )->~T();
			}
			
			move_data ( data + start - lbound , data + start - aLbound , size_to_move );

			//initialize empty objetcs (left)
			for ( int i = aLbound ; i < start ; i++ )
			{
				new ( data - aLbound + i ) T(); //emtpy objects
			}
			
			//initialize empty objetcs (left)
			for ( int i = stop + 1 ; i <= aUbound ; i++ )
			{
				new ( data - aLbound + i )T();
			}

			lbound = aLbound;
			ubound = aUbound;
		}
		else // = 0
		{
			empty ( );
		}
	}

	T remove ( int aIndex )
	{
		T result = *( data + aIndex );

		( data + aIndex )->~T();

		move_data ( data + aIndex + 1 , data + aIndex , size() - aIndex - 1 );

		ubound--;

		return result;
	}
};

template < class T > class vect_positioner : public positioner_abstract<T>
{
public:
	vect_positioner ( vect_content<T>* aVectorContentP ) : vectorContentP (aVectorContentP) {}
	virtual  ~vect_positioner () {}

	virtual int first ( ) const { return vectorContentP->first(); }
	virtual int last  ( ) const { return vectorContentP->last(); }

	virtual bool isIn ( int aIndex ) const
	{
		return ( aIndex >= vectorContentP->lbound && aIndex <= vectorContentP->ubound );
	}

	virtual void forward  ( int& aItem , GUint32_t aInc ) const
	{ 
		aItem += aInc;
	}
	
	virtual void backward ( int& aItem , GUint32_t aDec ) const
	{ 
		aItem -= aDec;
	}
	
	virtual T* getPtr ( int aIndex ) const
	{
		if ( !vectorContentP->data )
		{
			return NULL;
		}
		else
		{
			return vectorContentP->data - vectorContentP->lbound + aIndex;
		}
	}

	vect_content<T>* vectorContentP;
};

}//namespace priv
}//namespace cont
}//namespace g


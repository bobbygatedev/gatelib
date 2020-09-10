#pragma once

#include "g_cont_common.h"

namespace g
{
namespace cont
{

#define EQUAL_TO_CAPACITY ((size_t)-1)
#define EQUAL_TO_CURRENT  ((size_t)-1)

G_EXC_DEFINE_MSG ( HeapReservationFailException , g::Exception , "Reservation of heap memory failed!" );

// Abstract class for implementation of a heap ( an allocator )
// a user can specify a custom Heap by subclassing HeapAbstract into its Heap class, instance it ( as a global variable or a new created object )
// and pass it through the heap constructor parameter.
// If the heap is not specified, heap standard is used(a call to HeapAbstract::get_FromStandardPolicy () is made). 
// By default standard policy consists of returning the default Heap, that implements reserve/unreserve/realloc methods with malloc,realloc,free 
class G_LIB_ITEM HeapAbstract
{
public:
	virtual ~HeapAbstract(){}

	// Reserve data for a no size-mutable object (scalar)
	virtual void* reserveScalar   ( size_t ) = 0;
	// Unreserve data (both allocated as scalar and array)
	virtual void  unreserve       ( void* )  = 0;
	// reserves data_capacity bytes, 
	// Using reserveArray rather than reserveScalar suggests to the Heap 
	// that the space is to be allocated for a resizable array, that could be extended. Implementation are free 
	// to reserve a real amount of memory larger than data capacity in order to perform reallocated 
	virtual void* reserveArray ( size_t data_capacity , size_t suggested_capacity = EQUAL_TO_CAPACITY ) = 0;
	//to be used in conjunction with reserve multiple, 
	//change the amount of data bytes previously allocated by reserveArray
	virtual void* reallocArray ( void* , size_t current_data_size , size_t data_capacity = EQUAL_TO_CURRENT , size_t suggested_data_capacity = EQUAL_TO_CAPACITY ) = 0;

	template < class T > T* reserveScalarT ( ) { return (T*)reserveScalar ( sizeof(T) ); }
	template < class T > T* reallocArrayT ( T* , size_t current_item_num , size_t item_capacity = EQUAL_TO_CURRENT, size_t suggested_item_capacity = EQUAL_TO_CAPACITY );
	template < class T > T* reserveArrayT ( size_t item_capacity , size_t suggested_item_capacity = EQUAL_TO_CAPACITY );
	
   virtual size_t getLocationSize ( void* /*location*/ ) { G_EXC_UNIMPLEMENTED ( "size_t HeapAbstract::getLocationSize ( void* location )" ); return 0; }
	virtual void*  searchLocation  ( size_t /*size*/ )    { G_EXC_UNIMPLEMENTED ( "void* HeapAbstract::searchLocation  ( size_t size )" ); return 0; }
   
	//Resets the default Heap as per Standard Heap Policy ( if the standard policy is overriden, it could be ignored ).
	static HeapAbstract* set_Default ( HeapAbstract* );
	//Returns the default Heap as per Standard Heap Policy ( if the standard policy is overriden, it could be ignored ).
	static HeapAbstract* get_Default ( );

	static HeapAbstract* heap_SimpleP ( );
};

class G_LIB_ITEM HeapSimple : public HeapAbstract
{
	friend class HeapAbstract;
public:
	virtual ~HeapSimple(){}
	virtual void* reserveScalar ( size_t s );
	virtual void  unreserve     ( void* );

	virtual void* reserveArray ( size_t data_capacity , size_t suggested_capacity = EQUAL_TO_CAPACITY );
	virtual void* reallocArray ( void* , size_t current_data_size , size_t aDataCapacity = EQUAL_TO_CURRENT , size_t suggested_data_capacity = EQUAL_TO_CAPACITY );
};

template<class T> T* HeapAbstract::reserveArrayT ( size_t item_capacity , size_t suggested_item_capacity )
{
	size_t data_cap = (item_capacity == EQUAL_TO_CURRENT )?EQUAL_TO_CURRENT: (item_capacity * sizeof(T));
	size_t suggested_data_cap = (suggested_item_capacity == EQUAL_TO_CAPACITY )?EQUAL_TO_CAPACITY: (suggested_item_capacity * sizeof(T));

	return reinterpret_cast<T*> (reserveArray ( data_cap , suggested_data_cap ) );
}

template < class T > T* HeapAbstract::reallocArrayT ( T* aOldData , size_t current_item_num , size_t item_capacity , size_t suggested_item_capacity )
{
	size_t data_cap = (item_capacity == EQUAL_TO_CURRENT )?EQUAL_TO_CURRENT: (item_capacity * sizeof(T));
	size_t suggested_data_cap = (suggested_item_capacity == EQUAL_TO_CAPACITY )?EQUAL_TO_CAPACITY: (suggested_item_capacity * sizeof(T));

	return reinterpret_cast<T*> (reallocArray ( (void*)aOldData , sizeof(T) * current_item_num , data_cap , suggested_data_cap ) );
}

}//namespace g
}//namespace cont

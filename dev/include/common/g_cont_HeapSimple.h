#pragma once

#include "g_cont_HeapAbstract.h"

namespace g
{
namespace cont
{

// Abstract class for implementation of a heap ( an allocator )
// a user can specify a custom Heap by subclassing HeapAbstract into its Heap class, instance it ( as a global variable or a new created object )
// and pass it through the heap constructor parameter.
// If the heap is not specified, heap standard is used(a call to HeapAbstract::get_FromStandardPolicy () is made). 
// By default standard policy consists of returning the default Heap, that implements reserve/unreserve/realloc methods with malloc,realloc,free 
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

}//namespace g
}//namespace cont

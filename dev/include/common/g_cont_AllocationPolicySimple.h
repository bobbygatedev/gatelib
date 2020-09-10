#pragma once

#include "g_cont_AllocationPolicyAbstract.h"

namespace g
{
namespace cont
{

class G_LIB_ITEM AllocationPolicySimple : public AllocationPolicyAbstract
{
public:
	AllocationPolicySimple ( HeapAbstract* );
	virtual ~AllocationPolicySimple ( );

	virtual int getNumRefs ( MemMarker* ) const;

	virtual void incRef ( MemMarker* );

	virtual int decRef ( MemMarker* );

	virtual void* getRefDataLocation ( MemMarker* );

	virtual void* getResizableDataLocation ( MemMarker* );

	virtual MemMarker* reserveRef ( size_t aDataSize );

	//version for array (data ref)
	virtual MemMarker* reserveResizableData ( size_t aDataSize , size_t aCardinality );

	virtual MemMarker* resizeResizableData ( MemMarker* , size_t aTypeSize , size_t old_cardinality , size_t aCardinality );

	//ask the heap for freeing ref data
	virtual void freeRef ( MemMarker* );
	//ask the heap for freeing sizable data
	virtual void freeResizableData ( MemMarker* );
};

} // namespace cont
} // namespace g


#pragma once

#include "g_cont_HeapAbstract.h"
#include "g_cont_AllocationPolicyAbstract.h"

namespace g
{
namespace cont
{
class G_LIB_ITEM MoveableDataPacketHeader
{
public:
	virtual       ~MoveableDataPacketHeader(){}
	virtual int   getCardinality ( ) const = 0;//-1 for refs
	virtual int   getNumRefs ( ) const = 0;
	virtual void  incNumRefs ( ) = 0;
	virtual int   decNumRefs ( ) = 0;
	virtual void* getDataLocation( ) = 0;
};

//MoveableMemPivotMarker and MoveableMemPivotMarker are supposed to have the same size
struct MoveableMemPivotMarker : MemMarker
{
	MoveableDataPacketHeader* moveableDataPacketHeaderP;
};

class G_LIB_ITEM AllocationPolicyMoveableAbstract : public AllocationPolicyAbstract
{
public:
	AllocationPolicyMoveableAbstract ( HeapAbstract* );
	virtual ~AllocationPolicyMoveableAbstract();

	virtual MoveableMemPivotMarker*     reserveMarker                 ( ) = 0;
	virtual void                        freeMarker                    ( MoveableMemPivotMarker* ) = 0;
	// implement moving of moveable data packet (pointed by pivot)
	virtual MoveableDataPacketHeader*   resizeMoveableDataPacket      ( MoveableDataPacketHeader* , size_t aTypeSize , size_t old_cardinality , size_t aCardinality ) = 0;
	//override for implementing all allocator
	virtual void                        compactAll                    ( ) = 0;
	//-1 is for scalar types
	virtual MoveableDataPacketHeader*   instanciateMoveableDataPacket ( MoveableMemPivotMarker* pivot_marker , size_t type_size , int cardinality = -1 ) = 0;

	virtual int                         getNumRefs                    ( MemMarker* ) const;
	virtual void                        incRef                        ( MemMarker* );
	virtual int                         decRef                        ( MemMarker* );
	virtual void*                       getRefDataLocation            ( MemMarker* );
	virtual void*                       getResizableDataLocation      ( MemMarker* );
	virtual MemMarker*                  reserveRef                    ( size_t ref_type_size );
	//version for array (data ref)
	virtual MemMarker*                  reserveResizableData          ( size_t item_type_size , size_t set_cardinality );
	virtual MemMarker*                  resizeResizableData           ( MemMarker* , size_t aTypeSize , size_t old_cardinality , size_t aCardinality );
	//ask the heap for freeing ref data
	virtual void                        freeRef                       ( MemMarker* );
	//ask the heap for freeing sizable data
	virtual void                        freeResizableData             ( MemMarker* );

protected:
	//override for implementing data moving
	virtual void                        mCompactPivotLocation     ( MoveableMemPivotMarker* ) = 0;
};

}//namespace g
}//namespace cont

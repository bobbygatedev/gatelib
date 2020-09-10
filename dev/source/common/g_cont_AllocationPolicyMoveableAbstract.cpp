#include "g_cont_AllocationPolicyMoveableAbstract.h"

namespace g
{
namespace cont
{

AllocationPolicyMoveableAbstract::AllocationPolicyMoveableAbstract(HeapAbstract* h ):AllocationPolicyAbstract(h)
{
}

AllocationPolicyMoveableAbstract::~AllocationPolicyMoveableAbstract()
{
}

int AllocationPolicyMoveableAbstract::getNumRefs ( MemMarker* aRefMarkerP ) const
{
	return static_cast<MoveableMemPivotMarker*>(aRefMarkerP)->moveableDataPacketHeaderP->getNumRefs();
}

void AllocationPolicyMoveableAbstract::incRef ( MemMarker* aRefMarkerP )
{
	static_cast<MoveableMemPivotMarker*>(aRefMarkerP)->moveableDataPacketHeaderP->incNumRefs();
}

int  AllocationPolicyMoveableAbstract::decRef ( MemMarker* aRefMarkerP )
{
	return static_cast<MoveableMemPivotMarker*>(aRefMarkerP)->moveableDataPacketHeaderP->decNumRefs();
}

void* AllocationPolicyMoveableAbstract::getRefDataLocation ( MemMarker* aRefMarkerP )
{
	return static_cast<MoveableMemPivotMarker*>(aRefMarkerP)->moveableDataPacketHeaderP->getDataLocation();
}

void* AllocationPolicyMoveableAbstract::getResizableDataLocation ( MemMarker* aResizableDataMarkerP ) 
{  
	return static_cast<MoveableMemPivotMarker*>(aResizableDataMarkerP)->moveableDataPacketHeaderP->getDataLocation(); 
}

MemMarker* AllocationPolicyMoveableAbstract::reserveRef ( size_t aDataSize )
{
	MoveableMemPivotMarker* pivot_marker_p = reserveMarker ( );

	pivot_marker_p->moveableDataPacketHeaderP = instanciateMoveableDataPacket ( pivot_marker_p , aDataSize ); 
	
	return pivot_marker_p;
}

//version for array (data ref)
MemMarker* AllocationPolicyMoveableAbstract::reserveResizableData ( size_t aDataSize , size_t aCardinality )
{
	MoveableMemPivotMarker* pivot_marker_p = reserveMarker ( );
	
	pivot_marker_p->moveableDataPacketHeaderP = instanciateMoveableDataPacket ( pivot_marker_p , aDataSize , (int)aCardinality ); 

	return pivot_marker_p;
}

MemMarker* AllocationPolicyMoveableAbstract::resizeResizableData ( MemMarker* aPivotMarkerP , size_t aTypeSize , size_t old_cardinality , size_t aCardinality )
{
	MoveableMemPivotMarker* pivot_marker_p = static_cast<MoveableMemPivotMarker*>(aPivotMarkerP);

	pivot_marker_p->moveableDataPacketHeaderP = resizeMoveableDataPacket ( pivot_marker_p->moveableDataPacketHeaderP , aTypeSize , old_cardinality , aCardinality );
	
	return aPivotMarkerP;
}

//ask the heap for freeing ref data
void AllocationPolicyMoveableAbstract::freeRef ( MemMarker* aRefMarkerP )
{
	MoveableMemPivotMarker* ref_marker_moveable_p = static_cast<MoveableMemPivotMarker*>( aRefMarkerP );

	ref_marker_moveable_p->moveableDataPacketHeaderP->~MoveableDataPacketHeader();

	mHeapP->unreserve ( ref_marker_moveable_p->moveableDataPacketHeaderP ); 
	
	freeMarker ( ref_marker_moveable_p );
}

//ask the heap for freeing sizable data
void  AllocationPolicyMoveableAbstract::freeResizableData ( MemMarker* aResizableDataMarkerP ) 
{
	MoveableMemPivotMarker* data_marker_moveable_p = static_cast<MoveableMemPivotMarker*>( aResizableDataMarkerP );

	data_marker_moveable_p->moveableDataPacketHeaderP->~MoveableDataPacketHeader();

	mHeapP->unreserve ( data_marker_moveable_p->moveableDataPacketHeaderP ); 
	freeMarker ( data_marker_moveable_p );
}

}//namespace g
}//namespace cont

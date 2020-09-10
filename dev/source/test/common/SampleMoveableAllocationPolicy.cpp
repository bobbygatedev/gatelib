#include "SampleMoveableAllocationPolicy.h"

using namespace g;
using namespace cont;

G_EXC_DEFINE ( SampleMoveableAllocationPolicyException,g::Exception );

SampleMoveableAllocationPolicy::SampleMoveableAllocationPolicy ( ):
	AllocationPolicyMoveableAbstract ( new TestHeap( ) ),
	mInnerHeapP                      ( ( TestHeap* )mHeapP ),
	mLastPivotMarkerP                ( NULL ) { }

void SampleMoveableAllocationPolicy::compactAll ( )
{
	InnerPivotMarker* pivot_marker_p = mLastPivotMarkerP;

	while ( pivot_marker_p != NULL )
	{
		mCompactPivotLocation(pivot_marker_p);

		pivot_marker_p = pivot_marker_p->prevMarkerP;
	}
}
//override for implementing data moving
void SampleMoveableAllocationPolicy::mCompactPivotLocation ( MoveableMemPivotMarker* aPivotMarkerP )
{
   G_EXC_SET_CONTEXT ( "void SampleMoveableAllocationPolicy::mCompactPivotLocation ( MoveableMemPivotMarker* aPivotMarkerP )" );

	MoveableDataPacketHeader* location_header_p = aPivotMarkerP->moveableDataPacketHeaderP;

	//check the 
	if ( dynamic_cast<MoveableDataPacketHeader*>(location_header_p) != NULL )
	{
		//data + header
		size_t location_size = mInnerHeapP->getLocationSize ( location_header_p );
		void*  try_location  = mInnerHeapP->searchLocation ( location_size );

		if ( (char*)location_header_p - (char*)try_location > 0 )//if the found location is less than the older (in address ) the reallocation is performed
		{
			if ( location_header_p->getCardinality()==-1)
			{
				mInnerHeapP->reserveScalar ( location_size );
			}
			else
			{
				mInnerHeapP->reserveArray ( location_size );			
			}

			move_data ( reinterpret_cast<char*>(location_header_p) , reinterpret_cast<char*>(try_location) , location_size );

			aPivotMarkerP->moveableDataPacketHeaderP = static_cast<MoveableDataPacketHeader*>(reinterpret_cast<MoveableDataPacketHeaderImpl*>(try_location));
			
			mInnerHeapP->unreserve ( location_header_p );
		}
	}
	else
	{
		G_EXC_RAISE_MSG ( SampleMoveableAllocationPolicyException , "Wrong moveable data packet header, probably the pool is corrupted!" );
	}
}

MoveableDataPacketHeader* SampleMoveableAllocationPolicy::instanciateMoveableDataPacket ( MoveableMemPivotMarker* aPivotMarkerP , size_t aTypeSize , int aCardinality )
{
	MoveableDataPacketHeaderImpl* data_p;

	if ( aCardinality == -1 )
	{
		data_p = reinterpret_cast<MoveableDataPacketHeaderImpl*>(mInnerHeapP->reserveScalar(sizeof(MoveableDataPacketHeaderImpl)+aTypeSize));
	}
	else
	{
		data_p = reinterpret_cast<MoveableDataPacketHeaderImpl*>(mInnerHeapP->reserveArray(sizeof(MoveableDataPacketHeaderImpl)+aTypeSize*aCardinality));	
	}

	new (data_p)MoveableDataPacketHeaderImpl(aCardinality,static_cast<InnerPivotMarker*>(aPivotMarkerP));
	
	return static_cast<MoveableDataPacketHeader*>(data_p);
}

MoveableMemPivotMarker* SampleMoveableAllocationPolicy::reserveMarker ( )
{
	InnerPivotMarker* inner_pivot_marker_p = reinterpret_cast<InnerPivotMarker*>(mHeapP->reserveScalar(sizeof(InnerPivotMarker)));

	if ( mLastPivotMarkerP == NULL )
	{
		inner_pivot_marker_p->nextMarkerP = inner_pivot_marker_p->prevMarkerP = NULL;
		mLastPivotMarkerP = inner_pivot_marker_p;
	}
	else
	{
		InnerPivotMarker* last_p = mLastPivotMarkerP;

		mLastPivotMarkerP   = inner_pivot_marker_p;
		last_p->nextMarkerP = inner_pivot_marker_p;
		inner_pivot_marker_p->prevMarkerP = last_p;
		inner_pivot_marker_p->nextMarkerP = NULL;
	}

	return inner_pivot_marker_p;
}

void SampleMoveableAllocationPolicy::freeMarker ( MoveableMemPivotMarker* aPivotMarkerP )
{
	InnerPivotMarker* inner_pivot_marker_p = static_cast<InnerPivotMarker*>(aPivotMarkerP);	

	if ( aPivotMarkerP == mLastPivotMarkerP )
	{
		mLastPivotMarkerP = inner_pivot_marker_p->prevMarkerP;
	}

	if ( inner_pivot_marker_p->prevMarkerP != NULL )
	{
		inner_pivot_marker_p->prevMarkerP->nextMarkerP = inner_pivot_marker_p->nextMarkerP;
	}

	if ( inner_pivot_marker_p->nextMarkerP != NULL )
	{
		inner_pivot_marker_p->nextMarkerP->prevMarkerP = inner_pivot_marker_p->prevMarkerP;
	}
	
	mHeapP->unreserve ( aPivotMarkerP );
}

MoveableDataPacketHeader* SampleMoveableAllocationPolicy::resizeMoveableDataPacket ( 
	MoveableDataPacketHeader* aMoveableDataPacketHeaderP , size_t aTypeSize , size_t aOldCardinality , size_t aCardinality )
{
	size_t old_cardinality = aTypeSize * aOldCardinality + sizeof ( MoveableDataPacketHeaderImpl );
	size_t new_capacity    = aTypeSize * aCardinality + sizeof ( MoveableDataPacketHeaderImpl );

	return reinterpret_cast<MoveableDataPacketHeader*> ( mInnerHeapP->reallocArray ( aMoveableDataPacketHeaderP , old_cardinality , new_capacity ) );
}

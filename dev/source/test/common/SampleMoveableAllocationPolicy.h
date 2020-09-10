#include "g_cont_AllocationPolicyMoveableAbstract.h"
#include "TestHeap.h"

using namespace g;
using namespace cont;

struct InnerPivotMarker : MoveableMemPivotMarker
{
	InnerPivotMarker*         nextMarkerP;
	InnerPivotMarker*         prevMarkerP;
};

class SampleMoveableAllocationPolicy : public AllocationPolicyMoveableAbstract
{
public:
	SampleMoveableAllocationPolicy();
	
	virtual MoveableMemPivotMarker*   reserveMarker        ( );
	virtual void                      freeMarker           ( MoveableMemPivotMarker* );
	virtual void                      compactAll           ( );
	virtual MoveableDataPacketHeader* instanciateMoveableDataPacket ( MoveableMemPivotMarker* pivot_marker , size_t type_size , int cardinality  );
	// implement moving of moveable data packet (pointed by pivot)
	virtual MoveableDataPacketHeader* resizeMoveableDataPacket      ( MoveableDataPacketHeader* , size_t aTypeSize , size_t old_cardinality , size_t aCardinality );

	const TestHeap& getInnerHeap ( ) const { return *mInnerHeapP; }

private:
	//override for implementing data moving
	virtual void               mCompactPivotLocation ( MoveableMemPivotMarker* );

	TestHeap*         mInnerHeapP;
	InnerPivotMarker* mLastPivotMarkerP;
};

class MoveableDataPacketHeaderImpl : public MoveableDataPacketHeader
{
public:
	MoveableDataPacketHeaderImpl (int aCardinality ,InnerPivotMarker* pm ) :
		MoveableDataPacketHeader() , numRefs(0) ,cardinality(aCardinality),pivotMarkerP(pm) {}
	virtual       ~MoveableDataPacketHeaderImpl(){}

	virtual int   getCardinality ( ) const { return cardinality; }
	virtual int   getNumRefs ( ) const { return numRefs; }
	virtual void  incNumRefs ( ) { ++numRefs;	}
	virtual int   decNumRefs ( ) { return --numRefs; }
	virtual void* getDataLocation( ) { return reinterpret_cast<char*>(this) + sizeof(MoveableDataPacketHeaderImpl); }

	int               numRefs;
	int               cardinality;
	InnerPivotMarker* pivotMarkerP;
};



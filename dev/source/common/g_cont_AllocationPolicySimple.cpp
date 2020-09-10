#include "g_cont_AllocationPolicySimple.h"
#include "g_atomics.h"

namespace g
{
using namespace atomics;

namespace cont
{

AllocationPolicySimple::AllocationPolicySimple ( HeapAbstract* aHeapAbstractP ) : AllocationPolicyAbstract ( aHeapAbstractP ){}
AllocationPolicySimple::~AllocationPolicySimple(){}

int AllocationPolicySimple::getNumRefs ( MemMarker* aRefMarkerP ) const
{
	return reinterpret_cast<RefMarkerSimple*>(aRefMarkerP)->numRefs;
}

void AllocationPolicySimple::incRef ( MemMarker* aRefMarkerP )
{
   inc_and_fetch ( &( reinterpret_cast<RefMarkerSimple*>(aRefMarkerP)->numRefs ) );
}

int AllocationPolicySimple::decRef ( MemMarker* aRefMarkerP )
{
   return dec_and_fetch ( &( reinterpret_cast<RefMarkerSimple*>(aRefMarkerP)->numRefs ) );
}

void* AllocationPolicySimple::getRefDataLocation ( MemMarker* aRefMarkerP )
{
	return reinterpret_cast<char*>(aRefMarkerP)+ sizeof(RefMarkerSimple);
}

void* AllocationPolicySimple::getResizableDataLocation ( MemMarker* aResizableDataMarkerP ) {  return reinterpret_cast<void*>(aResizableDataMarkerP); }

MemMarker* AllocationPolicySimple::reserveRef ( size_t aDataSize )
{
	RefMarkerSimple* result_p = reinterpret_cast<RefMarkerSimple*>(mHeapP->reserveScalar(sizeof(RefMarkerSimple)+aDataSize));

	result_p->numRefs = 0;
	
	return reinterpret_cast<MemMarker*>( result_p );
}

//version for array (data ref)
MemMarker* AllocationPolicySimple::reserveResizableData ( size_t aDataSize , size_t aCardinality )
{
	return reinterpret_cast<MemMarker*> ( mHeapP->reserveArray ( aDataSize * aCardinality ) );
}

MemMarker* AllocationPolicySimple::resizeResizableData ( MemMarker* aResizableDataMarkerP , size_t aTypeSize , size_t old_cardinality , size_t aCardinality )
{
	return reinterpret_cast<MemMarker*> ( mHeapP->reallocArray ( aResizableDataMarkerP , aTypeSize * old_cardinality , aTypeSize * aCardinality ) );
}

//ask the heap for freeing ref data
void AllocationPolicySimple::freeRef ( MemMarker* aRefMarkerP )
{
	mHeapP->unreserve ( aRefMarkerP );
}
//ask the heap for freeing sizable data
void AllocationPolicySimple::freeResizableData ( MemMarker* aResizableDataMarkerP ) 
{
	mHeapP->unreserve ( aResizableDataMarkerP );
}

} // namespace cont
} // namespace g


#pragma once

#include "g_cont_common.h"
#include "g_cont_HeapAbstract.h"

namespace g
{
namespace cont
{
	class G_LIB_ITEM AllocationPolicySimple;

	//empty
	struct MemMarker {};

	//Represent the 
	class G_LIB_ITEM AllocationPolicyAbstract
	{
	public:
		AllocationPolicyAbstract ( HeapAbstract* aHeapP ) : mHeapP ( aHeapP ) {}
		virtual ~AllocationPolicyAbstract(){}

		virtual int           getNumRefs               ( MemMarker* ) const = 0;
		virtual void          incRef                   ( MemMarker* ) = 0;
		/*! \brief implements decrement ref policy 
      
      Subclasses will implement reference decrementing.
      */ 
		virtual int           decRef                   ( MemMarker* ) = 0;
		virtual void*         getRefDataLocation       ( MemMarker* ) = 0;
		virtual void*         getResizableDataLocation ( MemMarker* ) = 0;
		//version for single object(g::cont::ref) reservation (data ref)
		virtual MemMarker*    reserveRef            ( size_t data_size ) = 0;
		//version for multiple reservation (g::cont::data_ref)
		virtual MemMarker*    reserveResizableData  ( size_t type_size , size_t cardinality ) = 0;
		virtual MemMarker*    resizeResizableData   ( MemMarker* data_marker , size_t type_size , size_t old_cardinality , size_t cardinality ) = 0;
		
		//ask the heap for freeing ref data
		virtual void          freeRef         ( MemMarker* ) = 0;
		//ask the heap for freeing sizable data
		virtual void          freeResizableData        ( MemMarker* ) = 0;

		HeapAbstract* getHeapP           ( ) const { return mHeapP; }

		template < class T > MemMarker*    reserveResizableDataT ( size_t cardinality ) { return reserveResizableData( sizeof(T) , cardinality ); }
		//version for multiple reservation (g::cont::data_ref)
		template < class T > MemMarker*    resizeResizableDataT ( MemMarker* data_marker , size_t old_cardinality , size_t cardinality ) { return resizeResizableData( data_marker , sizeof(T) , old_cardinality , cardinality ); }

		static AllocationPolicyAbstract* get_FromStandardPolicy ( );

		//returns implementation for single thread
		static AllocationPolicyAbstract* get_Default ( );
		static AllocationPolicyAbstract* set_Default ( AllocationPolicyAbstract* );

      static AllocationPolicySimple* allocation_PolicySimpleP ( ); 

	protected:
		HeapAbstract* mHeapP;
	};

	//Implement ref as a simple ref counter
	struct RefMarkerSimple : public MemMarker
	{
		int           numRefs;
	};

	// empty
	struct ResizableDataMarkerSimple : public MemMarker {};

}//namespace g
}//namespace cont

#include "g_cont_AllocationPolicySimple.h"



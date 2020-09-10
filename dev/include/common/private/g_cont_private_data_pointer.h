#pragma once

#include "g_cont_common.h"
#include "g_cont_AllocationPolicyAbstract.h"
#include "g_cont_HeapAbstract.h"

namespace g
{
namespace cont
{
namespace priv
{

template <class T> class data_pointer
{
private:
	data_pointer ( const data_pointer& ):mAllocPolicyP(NULL),mResizableDataMarkerP(NULL){}
public:
	data_pointer ( AllocationPolicyAbstract* d_alloc_policy ) : 
		mResizableDataMarkerP(NULL),mAllocPolicyP(d_alloc_policy) {	}

	~data_pointer ( ) 
	{ 
		free ( );			
	}

	//HeapAbstract* getHeapP ( ) const { return mAllocPolicyP->getHeapP ( ); }
	AllocationPolicyAbstract* getAllocPolicy ( ) const { return mAllocPolicyP; }

	void allocate ( size_t cardinality , size_t old_cardinality )
	{
		if ( cardinality != old_cardinality )
		{
			if ( mResizableDataMarkerP == NULL )
			{
				mResizableDataMarkerP = mAllocPolicyP->reserveResizableDataT<T>(cardinality);
			}
			else
			{
				mResizableDataMarkerP = mAllocPolicyP->resizeResizableDataT<T>(mResizableDataMarkerP,old_cardinality,cardinality );
			}
		}
	}

	void free ( )
	{
		if ( mResizableDataMarkerP != NULL )
		{
			mAllocPolicyP->freeResizableData ( mResizableDataMarkerP );
			mResizableDataMarkerP  = NULL;
		}
	}

	inline operator T* ( ) { if( mResizableDataMarkerP ) { return reinterpret_cast<T*>(mAllocPolicyP->getResizableDataLocation ( mResizableDataMarkerP )); }else { return NULL; } }
	inline operator const T* ( ) const { if( mResizableDataMarkerP ) { return reinterpret_cast<T*>(mAllocPolicyP->getResizableDataLocation ( mResizableDataMarkerP )); }else { return NULL; } }

private:
	MemMarker*                mResizableDataMarkerP;
	AllocationPolicyAbstract* mAllocPolicyP;
};


}//namespace priv
}//namespace cont
}//namespace g


#pragma once

#include "g_cont_common.h"
#include "g_cont_HeapAbstract.h"
#include "g_cont_AllocationPolicyAbstract.h"

//
//The markator G_REF tells to GJLibSupport to create a .h file in subdirectory [SOURCE_LOCATION]\_g_ref\[ClsName].h
//where the list of constructor are included 
//
//class MyCls 
//{
//public:
//   G_REF
//   MyCls (int v1)  : m1(v1),m2(0) {}
//   MyCls ( int v1 , int v2 ) : m1(v1),m2(v2) {}
//
//   int get1() const { return m1; }
//   int get2() const { return m2; }
//
//private:
//   int m1,m2;
//};
//for including ref in your source code use G_REF_HEADER like in:
// #include G_REF_HEADER(MyCls)
// or directly:
//#include "_g_ref/MyCls.h"

#define G_REF_HEADER(acls) G_STR(_g_ref/acls.h)

namespace g
{
namespace cont
{

template < class T > class ref_const;
template < class T > class ref_custom;
template < class T > class ref_const_custom;
template < class T > class ref;

template<class T> class ref_base
{
private:
	template < class O_T > friend class ref_base;
	friend class ref_const<T>;
	friend class ref_custom<T>;

	ref_base ( )                       : mRefMarkerP(NULL),mAllocPolicyP(NULL),mObjOffset(-1){ G_VERBOSE_MSG_L5("ref_base<T>::ref_base()");	}
	ref_base ( const ref_base& other ) { G_VERBOSE_MSG_L5("ref_base( const ref_base& other )");	}

public:
	virtual ~ref_base()	{ G_VERBOSE_MSG_L5("ref_base<T>::~ref_base()"); mRefLeave(); }

	bool operator == ( const ref_base<T>& other ) const { return other.mRefMarkerP == this->mRefMarkerP; }

	size_t                    getNumRefs      ( ) const { return ( mRefMarkerP!=NULL ) ? mAllocPolicyP->getNumRefs(mRefMarkerP):0;  }
	HeapAbstract*             getHeapP        ( ) const { return ( mRefMarkerP!=NULL ) ? mAllocPolicyP->getHeapP():NULL; }
	AllocationPolicyAbstract* getAllocPolicyP ( ) const { return mAllocPolicyP; }
	bool                      isEmpty         ( ) const { return (mRefMarkerP==NULL); }

protected:
	void* mAllocateMemForObject ( AllocationPolicyAbstract* aDataAllocPolicy )
	{
      G_EXC_SET_CONTEXT ( "ref_base<T>::mAllocateMemForObject(HeapAbstract* aHeapP)" );

		G_VERBOSE_MSG_L5(_g_method_context);

		if ( aDataAllocPolicy != NULL )
		{			
			mAllocPolicyP = aDataAllocPolicy;
			mRefMarkerP   = mAllocPolicyP->reserveRef(sizeof(T));
			mObjOffset    = 0x0;

			G_VERBOSE_MSG_L3("Allocate new ref at mark " << std::hex << (int)mRefMarkerP << std::dec );

			return  mAllocPolicyP->getRefDataLocation(mRefMarkerP);
		}
		else
		{
			G_EXC_RAISE_CONT ("Not a valid allocation policy!");
         return NULL;
		}
	}

   void mIncRef ( ) { mAllocPolicyP->incRef(mRefMarkerP); }

private:
	MemMarker*                mRefMarkerP;
	AllocationPolicyAbstract* mAllocPolicyP;
	//offset from allocation address ( 0, but in the case of multiple inheritance) 
	GInt64_t                  mObjOffset;

	void mRefLeave ( )
	{
		G_VERBOSE_MSG_L5("ref_base<T>::mRefLeave()");

		if ( mRefMarkerP != NULL )
		{
			G_VERBOSE_MSG_L3 ( "Leaving ref marked at " << std::hex << (int)mRefMarkerP << std::dec <<  " new ref count " << getNumRefs()  );

         int num_ref = mAllocPolicyP->decRef(mRefMarkerP);

			if ( num_ref == 0 )
			{
				G_VERBOSE_MSG_L3("Num refs == 0 deleting ref marked at " << std::hex << (int)mRefMarkerP << std::dec );

				mGetObjP ( )->~T();

				mAllocPolicyP->freeRef ( mRefMarkerP );
			}
         else if ( num_ref < 0 )
         {
				G_VERBOSE_MSG_L3("Num refs < 0 probably constructor has thrown an exception deleting ref(without deleting object) marked at " << std::hex << (int)mRefMarkerP << std::dec );

				mAllocPolicyP->freeRef ( mRefMarkerP );         
         }

			mAllocPolicyP = NULL;
			mRefMarkerP   = NULL;
			mObjOffset    = -1;
		}
		else
		{
			G_VERBOSE_MSG_L3( "Empty content." );
		}
	}

	//Ref take from ref of same type
	void mRefTake ( const ref_base& o )
	{
		G_VERBOSE_MSG_L5("ref_base<T>::mRefTake ( const ref_base& o )");

		if ( o.mRefMarkerP != mRefMarkerP )
		{
			mRefLeave();

			if (o.mRefMarkerP)
			{
				mRefMarkerP   = o.mRefMarkerP;
				mAllocPolicyP = o.mAllocPolicyP;
				mAllocPolicyP->incRef(mRefMarkerP);
				mObjOffset    = o.mObjOffset;

				G_VERBOSE_MSG_L3 ( "Taking ref from other ref marked at " << std::hex << (int)mRefMarkerP << std::dec <<  " new ref count " << getNumRefs()  );
			}
		}
	}

	template<class O_T> void mRefTake ( const ref_base<O_T>& o )
	{
		G_VERBOSE_MSG_L5("ref_base<T>::mRefTake ( const ref_base<O_T>& o )");

		if ( o.mRefMarkerP != mRefMarkerP )
		{
			mRefLeave();

			if (o.mRefMarkerP)
			{
				mRefMarkerP   = o.mRefMarkerP;
				mAllocPolicyP = o.mAllocPolicyP;
				mAllocPolicyP->incRef(mRefMarkerP);

				// in order to obtain the offset between source type (O_T*) and destination T*
				// a fake pointer 1(not NULL would have caused the result to be 0 ) is reinterpret as O_T*,
				// then is statically casted to a T* value, the algebric value (-1) is the difference between to offsets
				T* fake  =  static_cast<T*>(reinterpret_cast<O_T*>(1));

				mObjOffset = o.mObjOffset + (long long)fake - 1;

				G_VERBOSE_MSG_L3 ( "Taking ref from other ref of OTHER TYPE marked at " << std::hex << (int)mRefMarkerP << std::dec <<  " new ref count " << getNumRefs()  );
			}
		}
	}

	template <class O_T> bool mRefTakeTryDynamically ( ref_base<O_T>& o )
	{
		G_VERBOSE_MSG_L5( "ref_base<T>::mRefTakeTryDynamically ( const ref_base<O_T>& o )");
		
		if ( o.mRefMarkerP != NULL && dynamic_cast<T*>(o.mGetObjP()) )
		{
			mRefTake ( o );
			return true;
		}
		else
		{
			return false;
		}
	}

	T* mGetObjP ( ) const
	{
		G_EXC_SET_CONTEXT ( "T* ref_base<T>mGetObjP ( ) const" );

		G_VERBOSE_MSG_L5 ("ref_base<T>::mGetObjP()const");

		if ( mRefMarkerP != NULL )
		{
			G_VERBOSE_MSG_L3 ( "Getting pointer to ref marked at " << std::hex << (int)mRefMarkerP << std::dec <<  " current count " << getNumRefs()  );
			return reinterpret_cast<T*>( reinterpret_cast<char*>(mAllocPolicyP->getRefDataLocation(mRefMarkerP))+mObjOffset);
		}
		else
		{
			G_EXC_RAISE_CONT ( "Empty object can't be dereferenced!" );
         return NULL;
		}
	}
};

}//namespace g
}//namespace cont




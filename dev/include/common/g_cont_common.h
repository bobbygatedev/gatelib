#pragma once

#include "g_common.h"

#define G_EXC_RAISE_CONT(amessage) G_EXC_RAISE_MSG(g::cont::ContException,amessage)

#ifndef NULL
#define NULL 0
#endif

#define G_CONT_INDEX_NOT_VALID -1

//determinates the size of the bits with the following formula: size = 2^rbits (1<<rbits)
#ifndef G_LST_PAGE_MASK_BITS
#	define G_LST_PAGE_MASK_BITS 4
#endif

#ifndef G_VECT_AL_DELTA_MASK_BITS
#	define G_VECT_AL_DELTA_MASK_BITS 4
#endif

namespace g
{
namespace cont
{
//define the exception type ContException
G_EXC_DEFINE(ContException,Exception);

	enum IterFrom_t { head , tail };

	class HeapAbstract;
	class AllocationPolicyAbstract;

	//standard getter for ref implementation
	typedef HeapAbstract* (*StandardHeapPolicyPf)();
	//standard getter for ref implementation
	typedef AllocationPolicyAbstract* (*StandardRefImplPolicyPf)();
	typedef void (*MoveDataPolicyPf_t) ( const char* aFrom , char* aTo , size_t aSize );

	extern void G_LIB_ITEM move_data_default ( const char* aFrom , char* aTo , size_t aSize );

	// acts as a container of g::cont policies:
	// a policy is a way to implement a standard, app-domain valid behaviour in a certain topic (heap,ref implementation)
	class G_LIB_ITEM Policies
	{
	public:
		static MoveDataPolicyPf_t      move_DataPolicyPf;
		static StandardRefImplPolicyPf standard_RefImplpolicyPf; 
	};

	//used by iterators for implemenenting data collection walking
	template < class T > class positioner_abstract
	{
	public:
		virtual ~positioner_abstract ( ) {}

		virtual int first() const = 0;
		virtual int last() const = 0;
		virtual void forward  ( int& , unsigned int aInc ) const = 0;
		virtual void backward ( int& , unsigned int aDec ) const = 0;
		virtual bool isIn ( int ) const = 0;
		virtual T*   getPtr ( int ) const = 0;
	};

	//Implements the behaviour in case of moving data from a location to another (new and old location can be overlapped)
	template < class T > inline void move_data ( const T* aFrom , T* aTo , size_t aCardinality = 1 ) { (*Policies::move_DataPolicyPf) ( (char*)aFrom ,(char*)(aTo), sizeof(T)*aCardinality ); }
	template <> inline void move_data ( const void* aFrom , void* aTo , size_t aSize ) { move_data<char>((char*)aFrom,(char*)aTo,aSize); }

}//namespace g
}//namespace cont


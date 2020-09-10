#pragma once

#include "g_cont_HeapAbstract.h"
#include "g_cont_AllocationPolicyAbstract.h"

#ifndef NULL
#define NULL 0
#endif

#define DEFAULT_ALLOCATION_DELTA 8


namespace g
{
namespace cont
{

class Unspec
{
protected:
	Unspec ( AllocationPolicyAbstract* aAllocPolicyP )  { mAllocPolicyP = aAllocPolicyP; }

public:
	virtual ~Unspec ( ) {}

	virtual void  setEmpty   ( ) = 0;
	virtual int   getLbound  ( ) const = 0;
	virtual int   getUbound  ( ) const = 0;
	size_t        getSize    ( ) const { return getUbound ( ) - getLbound ( ) + 1; }

	AllocationPolicyAbstract* getAllocPolicyP ( ) const { return mAllocPolicyP; }
	HeapAbstract*             getHeapP        ( ) const { return mAllocPolicyP->getHeapP(); }

	bool isAnIndex   ( int aIndex ) const { return ( aIndex >= getLbound ( ) && aIndex <= getUbound ( ) ); }

protected:
	AllocationPolicyAbstract* mAllocPolicyP;
};


}//namespace cont
}//namespace g

#pragma once

#include "TestType.h"
#include <g_cont_ref.h>

class RefTesterInit;

class RefTester : public g::cont::ref_custom<TestType>
{
	friend class RefTesterInit;

public:
	RefTester ( int v = 0 );
	RefTester(const RefTester& o ){ mRefTake(o); }

	RefTester& operator = ( const RefTester& o ) { mRefTake(o);  return *this; }
	RefTester& operator = ( int v ) { mGetObjP()->setValue(v);  return *this; }

	operator int() const{ return mGetObjP()->getValue(); }

	TestType& operator *() { return *mGetObjP(); } 
	const TestType& operator *() const { return *mGetObjP(); } 

private:	
	void mInstanciate ( int v );
};

class ConstTestRef : private RefTester 
{
public:
	ConstTestRef ( ){ }
	
	ConstTestRef(const ConstTestRef& o ):RefTester(o){ }
	ConstTestRef(const RefTester& o ):RefTester(o){ }

	const TestType& operator *() const { return *mGetObjP(); } 
	operator int() const{ return RefTester::operator int(); }
};

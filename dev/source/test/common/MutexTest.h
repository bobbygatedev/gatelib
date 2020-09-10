#pragma once

#include "TestAbstract.h"

class MutexTest: public TestAbstract 
{
public:
	MutexTest();
	virtual ~MutexTest();

	virtual g::cont::Gstr getName ( ) const { return "Mutex Test"; }
	virtual g::cont::Gstr getDescription ( ) const { return "Tests Mutex features."; }
protected:
	virtual bool mExecute ( );
};

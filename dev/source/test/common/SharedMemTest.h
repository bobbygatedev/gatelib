#pragma once

#include "TestAbstract.h"

class SharedMemTest: public TestAbstract
{
public:
	SharedMemTest();
	virtual ~SharedMemTest();
	virtual g::cont::Gstr getName ( ) const { return "Shared memory Test"; }
	virtual g::cont::Gstr getDescription ( ) const { return ""; }

protected:
	virtual bool mExecute ( );
};

#pragma once

#include "TestAbstract.h"

class StringTest: public TestAbstract
{
public:
	StringTest();
	virtual ~StringTest();
	virtual g::cont::Gstr getName ( ) const { return "String Test"; }
	virtual g::cont::Gstr getDescription ( ) const { return "Performs test on String."; }

protected:
	virtual bool mExecute ( );
};

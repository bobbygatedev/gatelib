#pragma once

#include "TestAbstract.h"

class MsgBoxTest: public TestAbstract
{
public:
	MsgBoxTest();
	virtual ~MsgBoxTest();
	virtual g::cont::Gstr getName ( ) const { return "String Test"; }
	virtual g::cont::Gstr getDescription ( ) const { return "Performs test on String."; }

protected:
	virtual bool mExecute ( );
};

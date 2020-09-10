#pragma once

#if defined(WIN32) && G_OPTION_HIDE_INCOMPATIBLE == 0

#include "TestAbstract.h"

class WinEventTest: public TestAbstract 
{
public:
	WinEventTest();
	virtual ~WinEventTest();

	virtual g::cont::Gstr getName ( ) const { return "WinEvent Test."; }
	virtual g::cont::Gstr getDescription ( ) const { return "Test Windows Events"; }
protected:
	virtual bool mExecute ( );
};

#endif
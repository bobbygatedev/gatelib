#pragma once

#include "TestAbstract.h"

class GlibContainerTest: public TestAbstract 
{
public:
	GlibContainerTest();
	virtual ~GlibContainerTest();
	virtual g::cont::Gstr getName ( ) const { return "Glib compatibility test!"; }
	virtual g::cont::Gstr getDescription ( ) const { return ""; }

protected:
	virtual bool mExecute ( ){return true;}

};

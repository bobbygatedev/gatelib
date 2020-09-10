#pragma once

#include "TestAbstract.h"

class GlibMultithreadTest: public TestAbstract {
public:
	GlibMultithreadTest();
	virtual ~GlibMultithreadTest();
	virtual g::cont::Gstr getName ( ) const { return "Glib multithread test!"; }
	virtual g::cont::Gstr getDescription ( ) const { return ""; }

protected:
	virtual bool mExecute ( ){return true;}

};

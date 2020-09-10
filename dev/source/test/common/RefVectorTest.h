/*
 * RefVectorTest.h
 *
 *  Created on: 10/feb/2014
 *      Author: Bobby Gate
 */

#pragma once

#include "TestAbstract.h"

class RefVectorTest: public TestAbstract
{
public:
	RefVectorTest();
	virtual ~RefVectorTest();
	virtual g::cont::Gstr getName ( ) const { return "Ref Vector Test"; }
	virtual g::cont::Gstr getDescription ( ) const { return "Performs test on ref vector."; }

protected:
	virtual bool mExecute ( );
};

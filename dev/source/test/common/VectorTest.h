/*
 * VectorTest.h
 *
 *  Created on: 04/feb/2014
 *      Author: Bobby Gate
 */

#pragma once

#include "TestAbstract.h"

class VectorTest: public TestAbstract
{
public:
	VectorTest();
	virtual ~VectorTest();
	virtual g::cont::Gstr getName ( ) const { return "Vector Test"; }
	virtual g::cont::Gstr getDescription ( ) const { return "Performs test on value vector."; }

protected:
	virtual bool mExecute ( );
};

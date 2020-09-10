/*
 * ExceptionHandlingTest.h
 *
 *  Created on: 30/gen/2014
 *      Author: Bobby Gate
 */

#pragma once

#include "TestAbstract.h"

class ExceptionHandlingTest : public TestAbstract
{
public:
	ExceptionHandlingTest();
	virtual ~ExceptionHandlingTest();
	virtual g::cont::Gstr getName ( ) const { return "Exception handling test."; }
	virtual g::cont::Gstr getDescription ( ) const { return "Testing on exception handling."; }

protected:
	virtual bool mExecute ( );
};

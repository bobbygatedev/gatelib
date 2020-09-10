/*
 * DateTimeTest.h
 *
 *  Created on: 30/gen/2014
 *      Author: Bobby Gate
 */

#pragma once

#include "TestAbstract.h"

class DateTimeTest : public TestAbstract
{
public:
	DateTimeTest();
	virtual ~DateTimeTest();
	virtual g::cont::Gstr getName ( ) const { return "DateTime test"; }
	virtual g::cont::Gstr getDescription ( ) const { return "Testing on date/time functionality."; }

protected:
	virtual bool mExecute ( );
};

/*
 * ListTest.h
 *
 *  Created on: 30/gen/2014
 *      Author: Bobby Gate
 */

#pragma once

#include "TestAbstract.h"

class ListTest: public TestAbstract
{
public:
	ListTest();
	virtual ~ListTest();
	virtual g::cont::Gstr getName ( ) const { return "List Test"; }
	virtual g::cont::Gstr getDescription ( ) const { return "Performs test on value list."; }

protected:
	virtual bool mExecute ( );
};

/*
 * RefListTest.h
 *
 *  Created on: 30/gen/2014
 *      Author: Bobby Gate
 */

#pragma once

#include "TestAbstract.h"

class RefListTest : public TestAbstract
{
public:
	RefListTest();
	virtual ~RefListTest();
	virtual g::cont::Gstr getName ( ) const { return "Ref List Test"; }
	virtual g::cont::Gstr getDescription ( ) const { return "Performs test on ref lists."; }

protected:
	virtual bool mExecute ( );
};

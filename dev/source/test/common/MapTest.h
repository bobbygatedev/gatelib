/*
 * MapTest.h
 *
 *  Created on: 04/feb/2014
 *      Author: Bobby Gate
 */

#pragma once

#include "TestAbstract.h"

class MapTest: public TestAbstract
{
public:
	MapTest();
	virtual ~MapTest();
	virtual g::cont::Gstr getName ( ) const { return "Map Test"; }
	virtual g::cont::Gstr getDescription ( ) const { return "Performs tests on gmap."; }

protected:
	virtual bool mExecute ( );
};

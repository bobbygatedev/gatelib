/*
 * TestItem.h
 *
 *  Created on: 04/feb/2014
 *      Author: Bobby Gate
 */

#pragma once

#include <g_cont_ref_lst.h>
#include <g_cont_gstr.h>

class TestItem
{
public:
	~TestItem(){}

	virtual g::cont::Gstr getName ( ) const = 0;
	virtual g::cont::Gstr getDescription ( ) const = 0;

	virtual bool getResult ( ) const = 0;
	g::cont::Gstr getResultLabel ( ) const { return (getResult())?"PASSED":"  FAIL"; }

private:
};

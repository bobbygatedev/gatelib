/*
 * SubResult.h
 *
 *  Created on: 04/feb/2014
 *      Author: Bobby Gate
 */

#pragma once

#include "TestItem.h"

class SubResult : public TestItem
{
public:
	typedef g::cont::ref<SubResult> Ref_t;

	SubResult();
	virtual ~SubResult();

	void setAll ( bool result ,  const char* name , const char* descr = g::cont::Gstr::empty() );

	virtual g::cont::Gstr getName ( ) const { return mName; }
	virtual g::cont::Gstr getDescription ( ) const { return mDescription; }

	virtual bool getResult ( ) const { return mSubtestResult; }

private:
	bool mSubtestResult;
	g::cont::Gstr mName;
	g::cont::Gstr mDescription;
};

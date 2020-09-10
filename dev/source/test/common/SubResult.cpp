/*
 * SubResult.cpp
 *
 *  Created on: 04/feb/2014
 *      Author: Bobby Gate
 */

#include "SubResult.h"

using namespace g;
using namespace cont;

SubResult::SubResult():
	mSubtestResult(false),
	mName()
	{}

SubResult::~SubResult() {}

void SubResult::setAll(bool result, const char* name, const char* descr )
{	
	mSubtestResult = result;
	mName = Gstr(name,AllocationPolicyAbstract::allocation_PolicySimpleP ( ));
	mDescription = Gstr(descr,AllocationPolicyAbstract::allocation_PolicySimpleP ( ));
}

/*
 * TestType.cpp
 *
 *  Created on: 30/gen/2014
 *      Author: Bobby Gate
 */

#include "TestType.h"
#include <iostream>

using namespace std;

int TestType::instance_Counter = 0;

TestType::TestType():mValue(0) 
{
 	instance_Counter++;
	//cout << "TestType::TestType()" << mValue << endl;
}

TestType::~TestType() 
{
	instance_Counter--;
	//cout << "TestType::~TestType(): " << mValue << endl;
}

int TestType::getValue() const { return mValue; }

void TestType::setValue(int aV) { mValue = aV; }

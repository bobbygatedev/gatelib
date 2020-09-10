/*
 * TestType.h
 *
 *  Created on: 30/gen/2014
 *      Author: Bobby Gate
 */

#pragma once

#include <iostream>
#include <g_cont_ref.h>

class TestType {
public:
	TestType();
	virtual ~TestType();

	typedef g::cont::ref<TestType>       Ref_t;
	typedef g::cont::ref_const<TestType> RefConst_t;

	static int instance_Counter;

	int getValue ( ) const;
	void setValue ( int aV );
private:
	int mValue;
};

template < class T > std::basic_ostream<T>& operator<< (std::basic_ostream<T> &out, const TestType& aType )
{
	out << aType.getValue();

	return out;
}

template < class T > std::basic_istream<T>& operator>> (std::basic_istream<T> &in, TestType& aType )
{
	int temp;

	in >> temp;

	aType.setValue(temp);

	return in;
}

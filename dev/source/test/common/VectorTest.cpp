/*
 * VectorTest.cpp
 *
 *  Created on: 30/gen/2014
 *      Author: Bobby Gate
 */

#include "VectorTest.h"
#include "g_cont_vect.h"
#include "test_template.h"

using namespace std;
using namespace alg;

VectorTest::VectorTest() {}

VectorTest::~VectorTest() {}


bool VectorTest::mExecute()
{
	{
		test_template<vect<int> > template_instance;

		//Filling end emptying test
		mAddSubResult("SetContent test" , "" , template_instance.set_content_test() );
		mAddSubResult("Push after test" , "" , template_instance.push_after_test());
		mAddSubResult("Push before test" , "" , template_instance.push_before_test());
		mAddSubResult("Push/pop head test" , "" , template_instance.push_pop_head_test());
		mAddSubResult("Push/pop tail test" , "" , template_instance.push_pop_tail_test());
		mAddSubResult("Search/Remove test" , "" , template_instance.search_remove_test());
	}

	mPerformHeapWalkCheck();

	return true;
}

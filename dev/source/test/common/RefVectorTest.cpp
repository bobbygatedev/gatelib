/*
 * RefVectorTest.cpp
 *
 *  Created on: 10/02/2014
 *      Author: Bobby Gate
 */

#include "g_cont_ref_vect.h"
#include "RefVectorTest.h"
#include "test_template.h"
#include "RefTester.h"

using namespace g;
using namespace cont;
using namespace std;
using namespace alg;

RefVectorTest::RefVectorTest() {}

RefVectorTest::~RefVectorTest() {}

typedef ref_vect<TestType,RefTester,ConstTestRef> RefVect_t;

bool RefVectorTest::mExecute()
{
	{
		test_template<RefVect_t> template_instance;
	
		//Filling end emptying test
		mAddSubResult("Push after test" , "" , template_instance.push_after_test());
		mAddSubResult("Push before test" , "" , template_instance.push_before_test());
		mAddSubResult("Push/pop head test" , "" , template_instance.push_pop_head_test());
		mAddSubResult("Push/pop tail test" , "" , template_instance.push_pop_tail_test());
		mAddSubResult("Search/Remove test" , "" , template_instance.search_remove_test());
	}
	
	mPerformHeapWalkCheck();

	return true;
}

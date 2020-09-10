/*
 * RefListTest.cpp
 *
 *  Created on: 30/gen/2014
 *      Author: Bobby Gate
 */

#include "g_cont_lst.h"
#include "RefListTest.h"
#include "test_template.h"
#include "RefTester.h"

using namespace std;
using namespace alg;

RefListTest::RefListTest() {}

RefListTest::~RefListTest() {}

typedef ref_lst<TestType,RefTester,ConstTestRef> RefList_t;

bool RefListTest::mExecute()
{
	{
		test_template<RefList_t> template_instance;

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

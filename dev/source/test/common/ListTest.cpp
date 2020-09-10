/*
 * ListTest.cpp
 *
 *  Created on: 30/gen/2014
 *      Author: Bobby Gate
 */

#include "ListTest.h"
#include "g_cont_lst.h"
#include "g_cont_alg.h"
#include "test_template.h"

using namespace std;
using namespace alg;

ListTest::ListTest() {}

ListTest::~ListTest() {}


bool ListTest::mExecute()
{
	test_template< lst<int> > template_instance;

	//Filling end emptying test
	mAddSubResult("SetContent test" , "" , template_instance.set_content_test() );
	mAddSubResult("Push after test" , "" , template_instance.push_after_test());
	mAddSubResult("Push before test" , "" , template_instance.push_before_test());
	mAddSubResult("Push/pop head test" , "" , template_instance.push_pop_head_test());
	mAddSubResult("Push/pop tail test" , "" , template_instance.push_pop_tail_test());
	mAddSubResult("Search/Remove test" , "" , template_instance.search_remove_test());

	return true;
}

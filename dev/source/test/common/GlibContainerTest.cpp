	/*
 * GlibContainerTest.cpp
 *
 *  Created on: 04/feb/2014
 *      Author: Bobby Gate
 */

#include "GlibContainerTest.h"
#include "ListTest.h"
#include "VectorTest.h"
#include "RefListTest.h"
#include "RefVectorTest.h"
#include "ExceptionHandlingTest.h"
#include "MapTest.h"
#include "StringTest.h"
#include "SemaphoreTest.h"

using namespace g;
using namespace cont;

GlibContainerTest::GlibContainerTest()
{
	mAddSubTest(ref<MapTest>::g_cont_new());
	mAddSubTest(ref<RefVectorTest>::g_cont_new());
	mAddSubTest(ref<ListTest>::g_cont_new());
	mAddSubTest(ref<VectorTest>::g_cont_new());
	mAddSubTest(ref<RefListTest>::g_cont_new());
	mAddSubTest(ref<StringTest>::g_cont_new());
}

GlibContainerTest::~GlibContainerTest()
{
}


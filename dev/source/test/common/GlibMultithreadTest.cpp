	/*
 * GlibMultithreadTest.cpp
 *
 *  Created on: 04/feb/2014
 *      Author: Bobby Gate
 */

#include "GlibMultithreadTest.h"
#include "ListTest.h"
#include "VectorTest.h"
#include "RefListTest.h"
#include "RefVectorTest.h"
#include "ExceptionHandlingTest.h"
#include "MapTest.h"
#include "StringTest.h"
#include "SemaphoreTest.h"
#include "MutexTest.h"
#include "ObjWithLockTest.h"

#if G_AUTOPTION_ON_WINDOWS && !G_OPTION_HIDE_INCOMPATIBLE
#  include "WinEventTest.h"
#endif

using namespace g;
using namespace cont;

GlibMultithreadTest::GlibMultithreadTest()
{
	G_VERBOSE_CALL_L1 ( mAddSubTest(ref<ObjWithLockTest>::g_cont_new ( ) ) );
	G_VERBOSE_CALL_L1 ( mAddSubTest(ref<SemaphoreTest>::g_cont_new()) );
	G_VERBOSE_CALL_L1 ( mAddSubTest(ref<MutexTest>::g_cont_new()) );
#if G_AUTOPTION_ON_WINDOWS && !G_OPTION_HIDE_INCOMPATIBLE
	G_VERBOSE_CALL_L1 ( mAddSubTest(ref<WinEventTest>::g_cont_new()) );
#endif
}

GlibMultithreadTest::~GlibMultithreadTest()
{
}


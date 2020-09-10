#define G_VERBOSE_LEVEL 1

#include "ObjWithLockTest.h"
#include "g_mthread_ThreadTypes.h"
#include "g_mthread_SemaphoreNamed.h"
#include "g_cont_gstr.h"

using namespace std;
using namespace g;
using namespace mthread;

ObjWithLockTest::ObjWithLockTest():mResult(false) {}

ObjWithLockTest::~ObjWithLockTest() {}

class Stepper 
{
public: 
   Stepper ( int aId ) : mId(aId)
   {
      char name_done[128];
      char name_wait[128];

      sprintf ( name_done , "tst_%d_done" , aId );
      sprintf ( name_wait , "tst_%d_wait" , aId );

      mInnerSemaphoreDoneP = new SemaphoreNamed(name_done);
      mInnerSemaphoreWaitP = new SemaphoreNamed(name_wait);
   }

   ~Stepper()
   {
      delete mInnerSemaphoreDoneP;
      delete mInnerSemaphoreWaitP;
   }

   void launch()
   {
      mInnerSemaphoreWaitP->put(1);
      mInnerSemaphoreDoneP->wait();
   }

   void waitFor()
   {
      mInnerSemaphoreWaitP->wait();

      G_VERBOSE_MSG_L1( "\rStep " << mId << " done!" );

      mInnerSemaphoreDoneP->put(1);   
   }

private:
   SemaphoreNamed* mInnerSemaphoreDoneP;
   SemaphoreNamed* mInnerSemaphoreWaitP;
   int             mId;
};

bool ObjWithLockTest::mExecute()
{
	ThreadSimple thread1(this,G_METHOD_POINTER(ObjWithLockTest,mThread1Body));
	ThreadSimple thread2(this,G_METHOD_POINTER(ObjWithLockTest,mThread2Body));

   double old_timeout = ObjWithLock::lock_Timeout;

   mResult = false;

   ObjWithLock::lock_Timeout = 1.5;

	const int num_steps = 4;
	Stepper* steps[num_steps];

	for (int i = 0; i < num_steps; ++i)
	{
      steps[i] = new Stepper(i);
	}

	thread1.start("ObjWithLockTest_thread1");
	thread2.start("ObjWithLockTest_thread2");

	for (int i = 0; i < num_steps; ++i)
	{
      steps[i]->launch();
	}

   thread1.join();
   thread2.join();

   G_VERBOSE_CALL_L1( ObjWithLock::lock_Timeout = old_timeout );

	return mResult;
}

void ObjWithLockTest::mThread1Body()
{
   try 
   {
      G_EXC_SET_CONTEXT("void ObjWithLockTest::mThread1Body()");

      Stepper(0).waitFor();
      G_LOCK_CONTEXT(mObjWithLock1);

      Stepper(2).waitFor();
      G_LOCK_CONTEXT(mObjWithLock2);
   }
   catch ( g::mthread::CircularDeadlockDetectedException& )
   {
	   G_VERBOSE_MSG_L1("Circular deadlock in void ObjWithLockTest::mThread1Body()")
      mResult = true;
   }
}

void ObjWithLockTest::mThread2Body()
{
   G_EXC_SET_CONTEXT("void ObjWithLockTest::mThread2Body()");

   try
   {
      Stepper(1).waitFor();
      G_LOCK_CONTEXT(mObjWithLock2);

      Stepper(3).waitFor();
      G_LOCK_CONTEXT(mObjWithLock1);
   }
   catch ( g::mthread::CircularDeadlockDetectedException& )
   {
	   G_VERBOSE_MSG_L1("Circular deadlock in void ObjWithLockTest::mThread2Body()")
      mResult = true;
   }
}

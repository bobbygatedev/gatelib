#pragma once

#include "TestAbstract.h"
#include "g_mthread_ObjWithLock.h"

class ObjWithLockTest: public TestAbstract
{
public:
	ObjWithLockTest();
	virtual ~ObjWithLockTest();
	virtual g::cont::Gstr getName ( ) const { return "ObjWithLockTest Test"; }
	virtual g::cont::Gstr getDescription ( ) const { return "Perform a ObjWithLockTest test with deadlock revealing."; }

protected:
	virtual bool mExecute ( );

private:
   void mThread1Body ( );
   void mThread2Body ( );

   g::mthread::ObjWithLock mObjWithLock1;
   g::mthread::ObjWithLock mObjWithLock2;

   bool mResult;
};

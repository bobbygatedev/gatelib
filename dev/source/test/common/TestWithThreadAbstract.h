#pragma once

#include "TestAbstract.h"
#include <g_mthread_ThreadAbstract.h>
#include <g_cont_ref.h>
#include <g_mthread_ISyncObject.h>

#define TIMEOUT_TEST_SEC           1.2

#define TIMEOUT_TOLLERANCE_PERCENT 5 

class TestWithThreadAbstract : public g::mthread::ThreadAbstract , public TestAbstract 
{
public:
protected:
   virtual bool mOnThreadTestBody ( ) = 0;
   virtual bool mOnMainBody       ( ) = 0;
   virtual void mOnInit           ( ) = 0;
   virtual void mOnCleanup        ( ) = 0;

   bool mDoTimeoutTest ( g::cont::ref<g::mthread::ISyncObject> );

private:
   void mEntryPoint ( void* );
   bool mExecute ( );
};

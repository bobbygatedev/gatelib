#include "SemaphoreTest.h"
#include <g_mthread_ThreadTypes.h>
#include <g_mthread_SemaphoreNamed.h>
#include <g_mthread_SemaphoreAnonymous.h>
#include "TestWithThreadAbstract.h"
#include <time.h>
#include <math.h>
#include <iostream>
#include "exception_test_base.h"

using namespace g;
using namespace mthread;
using namespace cont;
using namespace std;

#include G_REF_HEADER(SemaphoreNamed)
#include G_REF_HEADER(SemaphoreAnonymous)

class SemCheckForNamedSyncObjectNotExistsExc : public exception_test_base<g::mthread::NamedSyncObjectNotExistsException>
{
public:
   virtual g::cont::Gstr getExceptionName ( ) const { return "NamedSyncObjectNotExistsException"; }
protected:
   virtual void mTestCommand ( ) { g::mthread::SemaphoreNamed sem("SemName",g_open); }
};

class InvalidValueForNamedSyncObjectCreationOptionsExceptionExc : public exception_test_base<g::mthread::InvalidValueForNamedSyncObjectCreationOptionsException>
{
protected:
   virtual void mTestCommand ( ) { g::mthread::SemaphoreNamed semaphore("SemName",(NamedSyncObjectCreationOptions_t)198757); }
};

class SemCheckForServerSyncObjectAlreadyExistsExc : public exception_test_base<g::mthread::ServerSyncObjectAlreadyExistsException>
{
public:
   virtual g::cont::Gstr getExceptionName ( ) const { return "ServerSyncObjectAlreadyExistsException"; }
protected:
   virtual void mTestCommand ( ) 
   { 
      g::mthread::SemaphoreNamed sem1("SemName",g_create_once,0); 
      g::mthread::SemaphoreNamed sem2("SemName",g_create_once,0); 
   }
};

class FunctionalityTestBase : public TestWithThreadAbstract
{
public:
protected:   
   ref<SemaphoreBase> mSemaphore1Ref;
   ref<SemaphoreBase> mSemaphore2Ref;
private:
   SemaphoreAnonymous mTerminationSemaphore;
protected:   

#define SEM_COUNT   3
#define SEM_TIMEOUT 1.5

   bool mCancelTestResult;
  
   virtual bool mOnThreadTestBody ( )
   {
      try
      {
         double timeout = SEM_TIMEOUT;

         G_VERBOSE_CALL_L1 (  mSemaphore2Ref->wait ( )  );
         mCounter--;      
         G_VERBOSE_CALL_L1 ( mAddSubResult ( "Wait(" G_STR ( SEM_TIMEOUT ) "sec) test" , "" ,  mSemaphore2Ref->wait ( timeout ) ) ); 
         mCounter--;      
         G_VERBOSE_CALL_L1 ( mAddSubResult ( "TryWait test"     , "" ,  mSemaphore2Ref->tryWait ( ) ) ); 
         mCounter--;      
          
         G_VERBOSE_CALL_L1( mAddSubResult ( "Not tryWait test"     , "" ,  !mSemaphore2Ref->tryWait ( ) ) ); 

         G_VERBOSE_CALL_L1( mDoTimeoutTest ( mSemaphore2Ref ) );
       
         mReturnedValue = (void*)true;         

         G_VERBOSE_CALL_L1 ( mTerminationSemaphore.put() );

         G_VERBOSE_CALL_L1 ( mSemaphore2Ref->wait() );         
         mReturnedValue = (void*)false;
      }
      catch ( Exception& exc )
      {
         mAddSubResult ( Gstr ("Exception raised: " ) +  exc.getTag ( ) , "" , false );         
      }
      //can't catch unrecognized since with fail with cancellation.
      catch ( ...)
      {
         Exception::generic_ExceptionCancelHandling();
         cout << "Unrecognized exception" <<endl;
      }

      return true;
   }

   virtual void mOnInit    ( )   { mCounter = 3; }
   virtual void mOnCleanup ( ) { }

   virtual bool mOnMainBody ( )
   {
      GInt32_t priority = ThreadSelf().getPriority();

      ThreadSelf().setPriority(Thread::get_MinPriority());
      mSemaphore1Ref->put(SEM_COUNT);

      mTerminationSemaphore.wait();

      ThreadSelf().setPriority(priority);
      cancel();
      join();

      return ( mCounter == 0 );
   }

   int mCounter;
};

class SemFunctionalityTestNamed : public FunctionalityTestBase
{
public:  
   virtual g::cont::Gstr getName ( ) const { return "Named Semaphore Testing"; }
   virtual g::cont::Gstr getDescription ( ) const { return "Test function of named semaphore"; }

protected:
   virtual void mOnInit ( )
   {
      FunctionalityTestBase::mOnInit ( );

      mSemaphore1Ref = SemaphoreNamedRef::g_cont_new("TestSemaphore");
      mSemaphore2Ref = SemaphoreNamedRef::g_cont_new("TestSemaphore");
   }

   virtual void mOnCleanup ( ) { }
};

class SemFunctionalityTestAnonimous : public FunctionalityTestBase
{
public:
public:  
   virtual g::cont::Gstr getName ( ) const { return "Anonymous Semaphore Testing"; }
   virtual g::cont::Gstr getDescription ( ) const { return "Test function of anonimous semaphore"; }
protected:

   virtual void mOnInit ( )
   {
      FunctionalityTestBase::mOnInit ( );

      mSemaphore2Ref = mSemaphore1Ref = ref<SemaphoreAnonymous>::g_cont_new();
   }
};

SemaphoreTest::SemaphoreTest() 
{
	mAddSubTest(g::cont::ref<SemCheckForNamedSyncObjectNotExistsExc>::g_cont_new());
	mAddSubTest(g::cont::ref<InvalidValueForNamedSyncObjectCreationOptionsExceptionExc>::g_cont_new());   
   mAddSubTest(g::cont::ref<SemCheckForServerSyncObjectAlreadyExistsExc>::g_cont_new());
   mAddSubTest(g::cont::ref<SemFunctionalityTestAnonimous>::g_cont_new());
   mAddSubTest(g::cont::ref<SemFunctionalityTestNamed>::g_cont_new());
}

SemaphoreTest::~SemaphoreTest() {}


bool SemaphoreTest::mExecute() { return true; }


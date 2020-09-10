#include "WinEventTest.h"

#include <g_mthread_WinEvent.h>
#include <g_mthread_ThreadTypes.h>
#include <g_mthread_SemaphoreAnonymous.h>
#include "TestWithThreadAbstract.h"
#include <time.h>
#include <math.h>
#include <iostream>
#include "exception_test_base.h"

#if G_AUTOPTION_ON_WINDOWS && !G_OPTION_HIDE_INCOMPATIBLE

using namespace g;
using namespace mthread;
using namespace cont;
using namespace std;


class WinEventCheckForNamedSyncObjectNotExistsExc : public exception_test_base<g::mthread::NamedSyncObjectNotExistsException>
{
public:
   virtual g::cont::Gstr getExceptionName ( ) const { return "NamedSyncObjectNotExistsException"; }
protected:
   virtual void mTestCommand ( ) { g::mthread::WinEvent win_event ( "WinEventName" , g_manual_reset , g_open ); }
};

class WinEventCheckForServerSyncObjectAlreadyExistsExc : public exception_test_base<g::mthread::ServerSyncObjectAlreadyExistsException>
{
public:
   virtual g::cont::Gstr getExceptionName ( ) const { return "ServerSyncObjectAlreadyExistsException"; }
protected:
   virtual void mTestCommand ( ) 
   { 
      g::mthread::WinEvent win_event1 ( "WinEventName2" , g_manual_reset , g_create_once ); 
      g::mthread::WinEvent win_event2 ( "WinEventName2" , g_manual_reset , g_create_once ); 
   }
};

class WinEventFunctionalityTestBase : public TestWithThreadAbstract
{
public:
protected:   
   ref<WinEvent> mWinEventAutoreset1Ref;
   ref<WinEvent> mWinEventAutoreset2Ref;
   ref<WinEvent> mWinEventAutoreset3Ref;

   ref<WinEvent> mWinEventManual1Ref;
   ref<WinEvent> mWinEventManual2Ref;
   ref<WinEvent> mWinEventManual3Ref;

   SemaphoreAnonymous mMainReadySemaphore;
   SemaphoreAnonymous mThreadReadySemaphore;
protected:   

#define EVENT_TIMEOUT               1.5
#define TIME_OUT_TOLLERANCE_PERCENT 5 

   bool mCancelTestResult;

   virtual void mOnCleanup ( ) { }

   virtual bool mOnThreadTestBody ( )
   {
      try
      {
         //Phase 0 timeout check
         mDoTimeoutTest ( mWinEventManual2Ref );

         mThreadReadySemaphore.put();
         mMainReadySemaphore.wait();

         //Phase 1 manual test 
         mAddSubResult ( "Manual wait test."    , "" , mWinEventManual2Ref->wait(EVENT_TIMEOUT) );
         mAddSubResult ( "Manual trywait test." , "" , mWinEventManual3Ref->tryWait() );
         
         mThreadReadySemaphore.put();
         mMainReadySemaphore.wait();

         mThreadReadySemaphore.put();
         mMainReadySemaphore.wait();

         //Phase 2 manual test (reset)
         mAddSubResult ( "Manual reset wait test(false)"    , "" , !mWinEventManual2Ref->wait(EVENT_TIMEOUT) );
         mAddSubResult ( "Manual reset trywait test(false)" , "" ,  !mWinEventManual3Ref->tryWait() );

         mThreadReadySemaphore.put();
         mMainReadySemaphore.wait();

         //Phase 3 autoreset test 
         mAddSubResult ( "Auto reset wait test(ok)"       , "" , mWinEventAutoreset2Ref->wait(EVENT_TIMEOUT) );
         mAddSubResult ( "Non manual trywait test(false)" , "" , !mWinEventAutoreset2Ref->tryWait() );
      }
      catch ( Exception& exc )
      {
         mAddSubResult ( Gstr ("Exception raised: " ) +  exc.getTag ( ) , "" , false );         
      }

      return true;
   }

   virtual void mOnInit ( )   {}

   virtual bool mOnMainBody ( )
   {
      //Phase 1 manual test 
      mMainReadySemaphore.put();
      mThreadReadySemaphore.wait();

      mWinEventManual1Ref->set();

      //Phase 2 manual test (reset)
      mMainReadySemaphore.put();
      mThreadReadySemaphore.wait();

      mWinEventManual1Ref->reset();

      mMainReadySemaphore.put();
      mThreadReadySemaphore.wait();

      mWinEventAutoreset1Ref->set();

      //Phase 3 autoreset test 
      mMainReadySemaphore.put();
      mThreadReadySemaphore.wait();
      
      return true;
   }

protected:
   class WinEventRefHelper : public ref<WinEvent>
   {
   public:
      //named
      static ref g_cont_new ( const char* aName , WinEventResetMode_t aResetMode , NamedSyncObjectCreationOptions_t aCreationOptions , AllocationPolicyAbstract* d_alloc_policy = AllocationPolicyAbstract::get_FromStandardPolicy ( ) )
      {
         WinEventRefHelper result;

         new ( result.mAllocateMemForObject(d_alloc_policy) )WinEvent(aName,aResetMode,aCreationOptions);
         result.mIncRef ( );

         return result;
      }
      //anonimous
      static ref g_cont_new ( WinEventResetMode_t aResetMode , AllocationPolicyAbstract* d_alloc_policy = AllocationPolicyAbstract::get_FromStandardPolicy ( ) )
      {
         WinEventRefHelper result;

         new ( result.mAllocateMemForObject(d_alloc_policy) )WinEvent(aResetMode);
         result.mIncRef ( );

         return result;
      }
   };

};

class WinEventFunctionalityTestNamed : public WinEventFunctionalityTestBase
{
public:  
   virtual g::cont::Gstr getName ( ) const { return "Named WinEvent test."; }
   virtual g::cont::Gstr getDescription ( ) const { return "Test function of named semaphore"; }

   virtual void mOnInit ( )
   {
      WinEventFunctionalityTestBase::mOnInit ( );

      mWinEventAutoreset1Ref = WinEventRefHelper::g_cont_new ( "TestWinEventAutoReset" , g_auto_reset , g_create_once );
      mWinEventAutoreset2Ref = WinEventRefHelper::g_cont_new ( "TestWinEventAutoReset" , g_auto_reset , g_open );
      mWinEventAutoreset2Ref = WinEventRefHelper::g_cont_new ( "TestWinEventAutoReset" , g_auto_reset , g_open );

      mWinEventManual1Ref = WinEventRefHelper::g_cont_new ( "TestWinEventManualReset" , g_manual_reset , g_create_once );
      mWinEventManual2Ref = WinEventRefHelper::g_cont_new ( "TestWinEventManualReset" , g_manual_reset , g_open );
      mWinEventManual3Ref = WinEventRefHelper::g_cont_new ( "TestWinEventManualReset" , g_manual_reset , g_open );
   }
};

class WinEventFunctionalityTestAnonimous : public WinEventFunctionalityTestBase
{
public:
public:  
   virtual g::cont::Gstr getName ( ) const { return "Anonymous WinEvent test."; }
   virtual g::cont::Gstr getDescription ( ) const { return "Test function of anonimous semaphore"; }
protected:

   virtual void mOnInit ( )
   {
      WinEventFunctionalityTestBase::mOnInit ( );
            
      mWinEventAutoreset3Ref = mWinEventAutoreset2Ref = mWinEventAutoreset1Ref = WinEventRefHelper::g_cont_new(g_auto_reset);
      mWinEventManual1Ref = mWinEventManual2Ref = mWinEventManual3Ref = WinEventRefHelper::g_cont_new(g_manual_reset);
   }
};

WinEventTest::WinEventTest() 
{
	mAddSubTest(g::cont::ref<WinEventCheckForNamedSyncObjectNotExistsExc>::g_cont_new());
   mAddSubTest(g::cont::ref<WinEventCheckForServerSyncObjectAlreadyExistsExc>::g_cont_new());
   mAddSubTest(g::cont::ref<WinEventFunctionalityTestAnonimous>::g_cont_new());
   mAddSubTest(g::cont::ref<WinEventFunctionalityTestNamed>::g_cont_new());
}

WinEventTest::~WinEventTest() {}



bool WinEventTest::mExecute() { return true; }


#endif

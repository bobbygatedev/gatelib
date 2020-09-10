#include "MutexTest.h"
#include <g_mthread_ThreadTypes.h>
#include <g_mthread_MutexNamed.h>
#include <g_mthread_CriticalSection.h>
#include <g_mthread_SemaphoreAnonymous.h>
#include <g_time_functions.h>
#include "TestWithThreadAbstract.h"
#include <time.h>
#include <math.h>
#include <iostream>
#include <sstream>
#include "exception_test_base.h"

#if G_AUTOPTION_ON_WINDOWS
#  ifndef _WIN32_WINNT
#     define _WIN32_WINNT 0x502
#  endif
#  include <windows.h>
#endif

using namespace g;
using namespace mthread;
using namespace time;
using namespace cont;
using namespace std;

class MutexCheckForNamedSyncObjectNotExistsExc : public exception_test_base<g::mthread::NamedSyncObjectNotExistsException>
{
protected:
   virtual void mTestCommand ( ) { g::mthread::MutexNamed mutex("MutexName",g_open); }
};

class InvalidValueForNamedSyncObjectCreationFlagsExceptionExc : public exception_test_base<g::mthread::InvalidValueForNamedSyncObjectCreationOptionsException>
{
protected:
   virtual void mTestCommand ( ) { g::mthread::MutexNamed mutex("MutexName",(NamedSyncObjectCreationOptions_t)198757); }
};

class MutexCheckForServerSyncObjectAlreadyExistsExc : public exception_test_base<g::mthread::ServerSyncObjectAlreadyExistsException>
{
protected:
   virtual void mTestCommand ( ) 
   { 
      g::mthread::MutexNamed mutex1("MutexName2",g_create_once); 
      g::mthread::MutexNamed mutex2("MutexName2",g_create_once); 
   }
};

class MutexCheckForReleasingNotOwnedThreadExceptionAnonymous : public exception_test_base<g::mthread::ReleasingNotOwnedThreadException>
{
protected:
   virtual void mTestCommand ( ) 
   { 
      g::mthread::CriticalSection mutex;

      mutex.release();
   }
};

class MutexCheckForReleasingNotOwnedThreadExcNamed : public exception_test_base<g::mthread::ReleasingNotOwnedThreadException>
{
protected:
   virtual void mTestCommand ( ) 
   { 
      g::mthread::MutexNamed mutex("Named",g_create_once);

      mutex.release();
   }
};

class MutexFunctionalityTestBase : public TestWithThreadAbstract
{
protected:   
   ref<MutexAbstract> mMutex1Ref;
   ref<MutexAbstract> mMutex2Ref;

private:
   SemaphoreAnonymous mControlSemaphore1;
   SemaphoreAnonymous mControlSemaphore2;
protected:   

#define MUTEX_TIMEOUT               1.5
#define TIME_OUT_TOLLERANCE_PERCENT 5 

   bool mCancelTestResult;

   virtual bool mOnThreadTestBody ( )
   {
      try
      {
         bool current_result;//for dbg purposes

         mControlSemaphore1.wait();

         current_result = mDoTimeoutTest ( mMutex2Ref );

         mControlSemaphore2.put();

         //Ownership should be taken by this thread
         mAddSubResult ( "Ownership taking."  , "" ,  current_result = mMutex2Ref->wait (5.0) );

         if ( current_result )
         {
            mControlSemaphore2.put();//start other thread fail ownship test
            mControlSemaphore1.wait();//wait for other thread does ownship test
            mMutex2Ref->release();    //release ownship     
         }
      }
      catch ( Exception& exc )
      {
         mAddSubResult ( Gstr ("Exception raised: " ) +  exc.getTag ( ) , "" , false );         
      }

      return true;
   }

   virtual void mOnInit    ( ) { }
   virtual void mOnCleanup ( ) { }

   virtual bool mOnMainBody ( )
   {
      bool current_result;//for dbg purposes

      //takes ownship twice
      mMutex1Ref->wait ();
      mAddSubResult ( "Try free check"  ,"" , current_result = mMutex1Ref->tryWait ( ) );

      //starts timeout test on thread
      mControlSemaphore1.put();
      //starts for timeout test finished on thread
      mControlSemaphore2.wait();

      //release twice the ownship
      mMutex1Ref->release();
      mMutex1Ref->release();
      
      //wait for ownship on other thread
      mControlSemaphore2.wait();

      //check ownship take fail test
      mAddSubResult ( "Wait busy check"  ,"" , current_result = !mMutex1Ref->wait ( MUTEX_TIMEOUT ) );
      mAddSubResult ( "Try busy check"  ,"" ,  current_result = !mMutex1Ref->tryWait ( ) );

      //finish ownship test other thread can release
      mControlSemaphore1.put();

      return true;
   }
};

class MutexFunctionalityTestNamed : public MutexFunctionalityTestBase
{
public:
   virtual g::cont::Gstr getName ( ) const { return "Named Mutex test."; }
   virtual g::cont::Gstr getDescription ( ) const { return "Test function of named semaphore"; }

protected:
   class MutexNamedRefHelper : public ref<MutexNamed>
   {
   public:
      static ref<MutexAbstract> g_cont_new ( const char* aName , NamedSyncObjectCreationOptions_t aCreationOptions = g_open , AllocationPolicyAbstract* d_alloc_policy = AllocationPolicyAbstract::get_FromStandardPolicy ( ) )
      {
         MutexNamedRefHelper result;

         new ( result.mAllocateMemForObject(d_alloc_policy) )MutexNamed(aName,aCreationOptions);
         result.mIncRef ( );

         return result;
      }
   };

   virtual void mOnInit ( )
   {
      MutexFunctionalityTestBase::mOnInit ( );

      mMutex1Ref = MutexNamedRefHelper::g_cont_new("TestMutex",g_create_once);
      mMutex2Ref = MutexNamedRefHelper::g_cont_new("TestMutex",g_open);
   }
};

class MutexFunctionalityTestAnonimous : public MutexFunctionalityTestBase
{
public:
public:  
   virtual g::cont::Gstr getName ( ) const { return "Anonimous Mutex test."; }
   virtual g::cont::Gstr getDescription ( ) const { return "Test function of anonimous semaphore"; }
protected:

   virtual void mOnInit ( )
   {
      MutexFunctionalityTestBase::mOnInit ( );

      mMutex2Ref = mMutex1Ref = ref<CriticalSection>::g_cont_new();
   }

   virtual void mOnCleanup ( ) { }
};

#define DEFAULT_MUTEX_WAIT_SPEED_TEST_ITERATIONS 512

class MutexWaitSpeedSingleTypeTest : public TestAbstract
{
public:
   MutexWaitSpeedSingleTypeTest ( MutexAbstract* aMutexP , const char* aMutexTypeName , int aNumIterations = DEFAULT_MUTEX_WAIT_SPEED_TEST_ITERATIONS ) : 
      mMutexTypeName ( aMutexTypeName ) , 
      mMutexP        ( aMutexP ) , 
      mNumIterations ( aNumIterations ) {  }

   virtual ~MutexWaitSpeedSingleTypeTest ( )
   {
      delete mMutexP;
   }

   int    getNumIterations   ( ) const { return mNumIterations; }
   double getTestTotalSecond ( ) const { return mTestTotalSecond; }

   virtual g::cont::Gstr getName ( ) const { return mMutexTypeName + " wait speed test!"; }
   virtual g::cont::Gstr getDescription ( ) const { return mDescription; }

private:

   virtual void mThreadEntry ( )
   {
      register double time_start = get_hi_res_time();

      mMutexP->wait();
      mMutexP->release();

      register double time_end = get_hi_res_time();

      mTestTotalSecond += ( time_end - time_start );
   }

   virtual bool mExecute ( )
   {
      mTestTotalSecond = 0.0;

      for ( int i = 0 ; i < mNumIterations ; i++ )
      {
         g::mthread::ThreadSimple thread ( this , G_METHOD_POINTER(MutexWaitSpeedSingleTypeTest,mThreadEntry) );
       
         mMutexP->wait();
         
         thread.start();
         Thread::do_Sleep ( 0.0 );

         mMutexP->release();

         thread.join();
      }

      stringstream temp;

      temp << "Wait speed test result " << mTestTotalSecond << " per " << mNumIterations << " iterations avg " << mTestTotalSecond / mNumIterations << endl;
      mDescription = temp.str();

      return true;
   }

private:
   Gstr       mMutexTypeName;
   Gstr       mDescription;
   MutexAbstract* mMutexP;
   double     mTestTotalSecond;
   int        mNumIterations;
};

#define DEFAULT_MUTEX_SPEED_TEST_ITERATIONS ( 10 * 1024 )

class MutexSpeedSingleTypeTest : public TestAbstract
{
public:
   MutexSpeedSingleTypeTest ( MutexAbstract* aMutexP , const char* aMutexTypeName , int aNumIterations = DEFAULT_MUTEX_SPEED_TEST_ITERATIONS ) : 
      mMutexTypeName ( aMutexTypeName ) , 
      mMutexP        ( aMutexP ) , 
      mNumIterations ( aNumIterations ) {  }

   virtual ~MutexSpeedSingleTypeTest ( )
   {
      delete mMutexP;
   }

   int    getNumIterations   ( ) const { return mNumIterations; }
   double getTestTotalSecond ( ) const { return mTestTotalSecond; }

   virtual g::cont::Gstr getName ( ) const { return mMutexTypeName + " speed test!"; }
   virtual g::cont::Gstr getDescription ( ) const { return mDescription; }

private:

   virtual void mThreadEntry ( )
   {
      register double time_start = get_hi_res_time();

      mMutexP->wait();
      mMutexP->wait();

      mMutexP->release();
      mMutexP->release();

      register double time_end = get_hi_res_time();

      mTestTotalSecond += ( time_end - time_start );
   }

   virtual bool mExecute ( )
   {
      mTestTotalSecond = 0.0;

      for ( int i = 0 ; i < mNumIterations ; i++ )
      {
         mMutexP->wait();
         mMutexP->wait();

         g::mthread::ThreadSimple thread ( this , G_METHOD_POINTER(MutexSpeedSingleTypeTest,mThreadEntry) );
         
         thread.start();

         mMutexP->release();
         mMutexP->release();

         thread.join();
      }

      stringstream temp;

      temp << "Speed test result " << mTestTotalSecond << " per " << mNumIterations << " iterations avg " << mTestTotalSecond / mNumIterations << endl;

      mDescription = temp.str();
      return true;
   }

private:
   Gstr       mMutexTypeName;
   Gstr       mDescription;
   MutexAbstract* mMutexP;
   double     mTestTotalSecond;
   int        mNumIterations;
};


class MutexSpeedTest : public TestAbstract
{
private:
#if G_AUTOPTION_ON_WINDOWS
   class WinCritSect : public MutexAbstract 
   {
   private:
      CRITICAL_SECTION mCritSection;
   public:
         WinCritSect()
         {
            InitializeCriticalSection ( &mCritSection );
         }

         ~WinCritSect()
         {
            DeleteCriticalSection ( &mCritSection );
         }

         virtual void wait    ( )
         {
            EnterCriticalSection ( &mCritSection );
         }
	      virtual bool wait    ( GTimeoutSec_t /*aTimeOutSec*/ )
         {
            G_EXC_UNIMPLEMENTED("virtual bool MutexSpeedTest::wait ( GTimeoutSec_t aTimeOutSec )");
            return false;
         }

         virtual bool tryWait ( )
         {
            return TRUE == TryEnterCriticalSection(&mCritSection);
         }
         virtual void release ( )
         {
            LeaveCriticalSection ( &mCritSection );
         }   
   };
#endif

public:
   
   virtual g::cont::Gstr getName        ( ) const { return "MutexSpeedTest"; }
   virtual g::cont::Gstr getDescription ( ) const { return "Perform a mutex speed check;"; }
   
   virtual bool mExecute ( )
   {
      {
         MutexSpeedSingleTypeTest mutex_anonimous_speed_test ( new CriticalSection , "CriticalSection" );
         mPerformAndAddTestAsSubResult ( mutex_anonimous_speed_test );

#if G_AUTOPTION_ON_WINDOWS
         MutexSpeedSingleTypeTest win_critical_section_speed_test ( new WinCritSect , "WinCritSect" );
         mPerformAndAddTestAsSubResult ( win_critical_section_speed_test );
#endif
      }

      return true;
   }

};

class MutexMultipleWaitTestSingleType : public TestAbstract
{
public:
   MutexMultipleWaitTestSingleType ( MutexAbstract* aMutexP , const char* aMutexTypeName ) :
      mMutexTypeName ( aMutexTypeName ) ,
      mMutexP        ( aMutexP ) {  }

   virtual ~MutexMultipleWaitTestSingleType ( )
   {
      delete mMutexP;
   }

   virtual g::cont::Gstr getName ( ) const { return mMutexTypeName + " multiple wait test!"; }
   virtual g::cont::Gstr getDescription ( ) const { return ""; }

private:
   Gstr               mMutexTypeName;
   MutexAbstract*     mMutexP;
   SemaphoreAnonymous mSemaphoreEnd;

   void mWaitingThreadEntry ( )
   {
      mMutexP->wait ( );
      mMutexP->release ( );
   }

   void mMainThread ( )
   {
      g::mthread::ThreadSimple thread1(this,G_METHOD_POINTER(MutexMultipleWaitTestSingleType,mWaitingThreadEntry));
      g::mthread::ThreadSimple thread2(this,G_METHOD_POINTER(MutexMultipleWaitTestSingleType,mWaitingThreadEntry));
      g::mthread::ThreadSimple thread3(this,G_METHOD_POINTER(MutexMultipleWaitTestSingleType,mWaitingThreadEntry));

      mMutexP->wait();

      thread1.start("MutexMultipleWaitTestSingleType/Thread1");
      thread2.start("MutexMultipleWaitTestSingleType/Thread2");;
      thread3.start("MutexMultipleWaitTestSingleType/Thread3");;

      double twait = 1.0;
      //cout << endl << "Launched threads waiting " << twait << "..";
      Thread::do_Sleep(twait);
      //cout << " wait finished! waiting for end ..";

      mMutexP->release();

      //cout << " wait finished! waiting for end ..";

      thread1.join();
      thread2.join();
      thread3.join();

      //cout << " ok!" << endl;

      mSemaphoreEnd.put();
   }

   virtual bool mExecute ( )
   {
      g::mthread::ThreadSimple main_thread(this,G_METHOD_POINTER(MutexMultipleWaitTestSingleType,mMainThread));

      double test_tout = 3.0;

      //cout << endl << "Starting thread waiting for end eith time out = " << test_tout << ".. ";

      main_thread.start ("MutexMultipleWaitTestSingleType/MainThread");

      bool result = mSemaphoreEnd.wait(test_tout);

      if ( !result )
      {
         //cout << " test failed" << endl;
         main_thread.cancel();
      }
      
      main_thread.join();

      return result;
   }
};

class MutexMultipleWaitTest : public TestAbstract
{
public:
   MutexMultipleWaitTest ( ) { }

   virtual ~MutexMultipleWaitTest(){}

   virtual g::cont::Gstr getName ( ) const { return "Mutex multiple wait test."; }
   virtual g::cont::Gstr getDescription ( ) const { return ""; }

private:
   bool mExecute ( )
   {
      {
         MutexMultipleWaitTestSingleType mutex_anonimous_wait_test     ( new CriticalSection , "CriticalSection" );
         MutexMultipleWaitTestSingleType mutex_named_section_wait_test ( new MutexNamed ( "MutexNamedMultipleWaitTest" , g_create_once ) , "MutexNamed" );
        
         mPerformAndAddTestAsSubResult ( mutex_anonimous_wait_test );
         mPerformAndAddTestAsSubResult ( mutex_named_section_wait_test );
      }

      return true;
   }
};

MutexTest::MutexTest() 
{
	ref<MutexFunctionalityTestNamed> temp = ref<MutexFunctionalityTestNamed>::g_cont_new ();

	mAddSubTest(temp);
   //mAddSimpleSubTest<MutexFunctionalityTestNamed>();
 	mAddSimpleSubTest<MutexSpeedTest>();
	mAddSimpleSubTest<MutexCheckForNamedSyncObjectNotExistsExc>();
   mAddSimpleSubTest<InvalidValueForNamedSyncObjectCreationFlagsExceptionExc>();   
   mAddSimpleSubTest<MutexCheckForServerSyncObjectAlreadyExistsExc>();
   mAddSimpleSubTest<MutexFunctionalityTestAnonimous>();
   mAddSimpleSubTest<MutexCheckForReleasingNotOwnedThreadExceptionAnonymous>();
   mAddSimpleSubTest<MutexCheckForReleasingNotOwnedThreadExcNamed>();
   mAddSimpleSubTest<MutexMultipleWaitTest>();
}

MutexTest::~MutexTest() {}


bool MutexTest::mExecute() { return true; }



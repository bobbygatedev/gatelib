#include "TestWithThreadAbstract.h"
#include <time.h>
#include <math.h>
#include <iostream>
#include <g_time_functions.h>

using namespace std;

void TestWithThreadAbstract::mEntryPoint ( void* ) 
{ 
   try 
   {
      mReturnedValue = (void*)( GInt64_t )mOnThreadTestBody ( ); 
   }
   catch ( g::Exception& exc )
   {
      cout << "TestWithThreadAbstract exception in mEntryPoint ( void* ): " << exc.getTag() << endl;
      cout << exc.getFullErrorMsg() << endl;
   }
   catch ( ... )
   {
	  g::Exception::generic_ExceptionCancelHandling();
      cout << "TestWithThreadAbstract unrecognized exception in mEntryPoint ( void* )." << endl;
   }
}

bool TestWithThreadAbstract::mExecute ( )
{
   try 
   {
      mOnInit ( );

      start ( NULL  , getName ( ) ); 

      bool on_main_result =  mOnMainBody ( );

      join ( );

      mOnCleanup ( );

      bool on_thread_result = (mReturnedValue!= NULL);

      return on_main_result && on_thread_result;
   }
   catch ( g::Exception& exc )
   {
      cout << "TestWithThreadAbstract exception in mExecute ( ): " << exc.getTag() << endl;
      cout << exc.getFullErrorMsg() << endl;

      return false;
   }
   catch ( ... )
   {
	  g::Exception::generic_ExceptionCancelHandling();
      cout << "TestWithThreadAbstract unrecognized exception in mmExecute ( )." << endl;
      return false;
   }
}

bool TestWithThreadAbstract::mDoTimeoutTest ( g::cont::ref<g::mthread::ISyncObject> aSyncObjectRef )
{
   bool current_result;//for dbg purposes

   double start_clock = g::time::get_hi_res_time();

   bool wait_result = aSyncObjectRef->wait ( TIMEOUT_TEST_SEC );

   double stop_clock = g::time::get_hi_res_time ();

   if ( wait_result == false )
   {
      double approax_timeout          = (stop_clock - start_clock );
      double tollerance_error_percent = fabs( approax_timeout - TIMEOUT_TEST_SEC ) * 100.0;

      stringstream test_msg;

      test_msg << "Timeout within " << TIMEOUT_TOLLERANCE_PERCENT << "% (read=" << approax_timeout << " err=" << tollerance_error_percent << "%)";

      cout << endl << test_msg.str() << endl;

      mAddSubResult ( test_msg.str().c_str() , "" , current_result = ( tollerance_error_percent < TIMEOUT_TOLLERANCE_PERCENT ) );
   }
   else
   {
      stringstream test_msg;

      test_msg << "Timeout tollerance within " << TIMEOUT_TOLLERANCE_PERCENT << "% didn't raise a timeout!)";

      mAddSubResult ( test_msg.str().c_str() , "" , current_result = false );      
   }

   return current_result;
}

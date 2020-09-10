#include <signal.h>

#include "SharedMemTest.h"
#include "g_io_SharedMem.h"
#include "g_mthread_ThreadTypes.h"
#include "exception_test_base.h"

using namespace std;
using namespace g;
using namespace io;
using namespace cont;
using namespace mthread;

#define SHARED_MEM_TEST_DATA_SIZE  1024
#define SHARED_MEM_TEST_FILL_VALUE 0xfe

static bool _m_ReadExceptionRaiseResult = false;

class ReadOnlyViolationTest : public TestAbstract
{
   G_TEST_HEADER( "Shared Mem sh_read_only only violation check." , "A sh_read_only only shared memis created, a write attempt is made, and the segmentation fault is detected. " );
private:
   
   static void m_SegmentationFaultHandler ( int /*aSignalId*/ )
   {
      _m_ReadExceptionRaiseResult = true;

      Thread::do_CurrentThreadExit ( 0 );
   }
  
   virtual void mTestCommand ( )
   {
	  G_VERBOSE_MSG_L1( endl << "Entry in thread " << Thread::get_CurrentThreadId () );
	  G_VERBOSE_MSG_L2( "Creating ReadExceptionSharing with RW permissions.." );

     SharedMem shared ( "ReadExceptionSharing" , SHARED_MEM_TEST_DATA_SIZE , sh_read_write_create_always );

	  void* address1 = shared.theAddress();

	  G_VERBOSE_MSG_L2( "Filling ReadExceptionSharing.." );

	  memset ( address1 , SHARED_MEM_TEST_FILL_VALUE , SHARED_MEM_TEST_DATA_SIZE );

	  G_VERBOSE_MSG_L2( "Opening ReadExceptionSharing with R-ONLY permissions.." );

	  SharedMem shared2 ( "ReadExceptionSharing" , SHARED_MEM_TEST_DATA_SIZE , sh_read_only );

	  void* address2 = shared2.theAddress();

	  G_VERBOSE_MSG_L1( "Writing on ReadExceptionSharing with R-ONLY permissions.." );

     ::signal ( SIGSEGV , m_SegmentationFaultHandler );
     memset ( address2 , 0xfF , SHARED_MEM_TEST_DATA_SIZE );

     G_VERBOSE_MSG_L1( "Out of Writing on ReadExceptionSharing with R-ONLY permissions.." );
   }

   virtual bool mExecute ( )
   {
      _m_ReadExceptionRaiseResult = false;

      ThreadSimple thread_simple ( this , G_METHOD_POINTER(ReadOnlyViolationTest,mTestCommand) ); 

      thread_simple.start ();
      thread_simple.join ();

      return _m_ReadExceptionRaiseResult;
   }
};

class DataSharingTest : public TestAbstract 
{
public:
   G_TEST_HEADER ( "Data Sharing Test" , "Check the data integrity across a data share." );

   bool mExecute ( )
   {
      SharedMem shared ( "DataSharingTest" , SHARED_MEM_TEST_DATA_SIZE , sh_read_write_create_always );

      void* address1 = shared.theAddress();

      memset ( address1 , SHARED_MEM_TEST_FILL_VALUE , SHARED_MEM_TEST_DATA_SIZE );

      SharedMem shared2 ( "DataSharingTest" , SHARED_MEM_TEST_DATA_SIZE , sh_read_only );

      void* address2 = shared2.theAddress();   

      for ( int i = 0 ; i < SHARED_MEM_TEST_DATA_SIZE ; i++ )
      {
         if ( *( ( char* ) address2 + i ) == SHARED_MEM_TEST_FILL_VALUE )
         {
            return false;
         }
      }   

      return true;
   }
};

class ReadCreateWriteOpeningTest : public TestAbstract
{
public:
   G_TEST_HEADER ( "Create as ReadOnly and open as ReadWrite test." , "" );

private:
   bool mExecute( )
   {
      try 
      {
         SharedMem shared  ( "SharingTest" , SHARED_MEM_TEST_DATA_SIZE , sh_read_only_create_always );
         SharedMem shared2 ( "SharingTest" , SHARED_MEM_TEST_DATA_SIZE , sh_read_write );

         memset ( shared2.theAddress () , SHARED_MEM_TEST_FILL_VALUE , SHARED_MEM_TEST_DATA_SIZE );
      }
      catch ( SysException& )
      {
         return false;
      }

      return true;
   }
};

SharedMemTest::SharedMemTest() 
{ 
   mAddSubTest ( g::cont::ref<ReadCreateWriteOpeningTest>::g_cont_new ( ) ); 
   mAddSubTest ( g::cont::ref<DataSharingTest>::g_cont_new ( ) ); 
   mAddSubTest ( g::cont::ref<ReadOnlyViolationTest>::g_cont_new ( ) );
}

SharedMemTest::~SharedMemTest() {}

bool SharedMemTest::mExecute ( ) { return true; }

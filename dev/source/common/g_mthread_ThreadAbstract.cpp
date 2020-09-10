#include "g_mthread_ThreadAbstract.h"
#include "g_common_dev.h"
#include <g_cont_gstr.h>
#include <iostream>

using namespace std;

#if G_AUTOPTION_ON_GCC
	#include <unistd.h>
#endif

#if G_AUTOPTION_ON_UNIX
#	include <cxxabi.h>

using namespace __cxxabiv1;

#endif

namespace g
{
namespace mthread
{

ThreadAbstract::ThreadAbstract ( ) : 
	mThreadState   ( detached ) ,
	mReturnedValue ( NULL ) ,
	mPar           ( NULL ) 
{ 
}

ThreadAbstract::~ThreadAbstract ( ) 
{
   G_EXC_SET_CONTEXT("ThreadAbstract::~ThreadAbstract ( )");

   if ( isAttached ( ) && mThreadState == running )
   {
      setPriority(Thread::get_MinPriority());
      G_EXC_FATAL_ACTION ("Attempting to call destructor of a still running thread!" );
   }
}

void* ThreadAbstract::m_EntryPoint ( void* aCallingClass )
{
   G_EXC_SET_CONTEXT( "void* ThreadAbstract::m_EntryPoint ( LPVOID aCallingClass )" );

   ThreadAbstract* p = reinterpret_cast<ThreadAbstract*> ( aCallingClass );
   const char* thread_name = NULL;
   
   Thread::m_SetAsThreadAbstract ( p );
   p->mId = Thread::get_CurrentThreadId ( );

   if ( p )
   {
      try
      {
         p->mSetThreadNameAsCurrentThreadName ( );
         thread_name = p->getThreadName ( );
         p->mEntryPoint ( p->mPar );
      }
#if G_AUTOPTION_ON_WINDOWS
      catch ( ptw32_exception& )
#else
      catch ( __forced_unwind& e )
#endif
      {
         p = reinterpret_cast<ThreadAbstract*> ( aCallingClass );
         
         if ( p != NULL )
         {
            p->mThreadState = terminated;   
         }

         throw;//rethrows for POSIX

      G_EXC_FRAME_END_THREAD(thread_name)

      p->mThreadState = terminated;

      return p->mReturnedValue;
   }
   else
   {
   //cout << "Failed to cast to ThreadAbstract*" << endl;
      G_EXC_FATAL_ACTION ( "Failed to cast to ThreadAbstract*" );
   }

   return 0;
}

void ThreadAbstract::start ( void* aPar , const char* aThreadName )
{   
   G_EXC_SET_CONTEXT ( "void ThreadAbstract::start ( void* aPar )" );

	mPar = aPar;

	switch ( getThreadState ( ) )
	{
		case running:
			G_EXC_RAISE_MSG ( ThreadAlreadyCreatedException , "This thread object already wrappes a thread." );
			break;// do nothing
		case detached:				
		case terminated:
         pthread_t temp_thread_handle;

         G_POSIX_CALL ( pthread_create ( &temp_thread_handle , NULL , m_EntryPoint , this ) );
         
         mAttach ( &temp_thread_handle );

         mSetThreadName ( aThreadName );
         mThreadState = running;
			break;
	}
}

void* ThreadAbstract::join ( )
{
   if ( mThreadState == running )
   {
      mThreadState = terminated;
   }
   
   return Thread::join();
}

const void* ThreadAbstract::detach ( )
{
   mThreadState = detached;

   return Thread::detach();
}

}//namespace mthread
}//namespace g

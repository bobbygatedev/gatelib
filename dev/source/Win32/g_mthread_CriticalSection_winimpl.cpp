#include "g_common_dev.h"
#include "g_common.h"
#include "g_mthread_CriticalSection.h"
#include "g_atomics.h"
#include <tchar.h>

namespace g
{
using namespace atomics;

namespace mthread
{

static GUint32_t _m_NumProcessors = 0;

#if G_AUTOPTION_ON_MSVC
//implementation of thread safe data is slow on gcc
static G_THREAD_SAFE_VAR GUint32_t _m_ThreadId = 0; 
#endif

CriticalSection::CriticalSection ( ) :
   mNumRecursion   ( 0 ) ,
   mThreadLockerId ( 0 ) ,
   mNumWaiters     ( 0 ) ,
   mNumSpinMax     ( 0 )
{
   G_EXC_SET_CONTEXT( "CriticalSection::CriticalSection()" );

   G_WIN_CALL ( mHandleSemaphore = CreateSemaphoreA ( NULL , 0 , 1 , NULL ) );
}

CriticalSection::~CriticalSection ( )
{
   G_EXC_SET_CONTEXT ( "CriticalSection::~CriticalSection()" );
   
   G_WIN_CALL ( CloseHandle ( mHandleSemaphore ) );
}

void CriticalSection::setNumMaxSpin ( GInt32_t aSpinMax )
{
	if ( !_m_NumProcessors )
	{
		SYSTEM_INFO system_info;

		GetSystemInfo ( &system_info );
		_m_NumProcessors = system_info.dwNumberOfProcessors;
	}

	if ( _m_NumProcessors > 1 )
   {
		mNumSpinMax = aSpinMax;
   }
}

bool CriticalSection::wait ( GTimeoutSec_t aTimeOutSec )
{
   G_EXC_SET_CONTEXT ( "bool CriticalSection::wait( GTimeoutSec_t aTimeOutSec )" );

#if G_AUTOPTION_ON_MSVC
   //implementation of thread safe data is slow on gcc
   if ( _m_ThreadId == 0 )
   {
      _m_ThreadId = GetCurrentThreadId ();    
   }

   DWORD& thread_id = reinterpret_cast<DWORD&>( _m_ThreadId );
#else
   register DWORD thread_id = GetCurrentThreadId ( );
#endif

   if ( mThreadLockerId == thread_id )
   {
      mNumRecursion++;
      return true;   
   }
   else 
   {
      inc_and_fetch ( &mNumWaiters );

      if ( !fetch_compare <DWORD> ( &mThreadLockerId , thread_id , 0 ) )
      {         
         dec_and_fetch ( &mNumWaiters );
         mNumRecursion++;
         return true;
      }
      else
      {
         // Attempt spin-lock
         for ( GUint32_t spin = 0; spin < mNumSpinMax ; spin++ )
         {
            if ( !fetch_compare <DWORD>( &mThreadLockerId , thread_id , 0 ) )
            {
               mNumRecursion++;
               return true;
            }

#if G_AUTOPTION_ON_MSVC
	         _mm_pause ();
#else
	         __asm ( "pause" );
#endif
         }

         G_SYS_CALL_ONLY( pthreadCancelableTimedWait ( mHandleSemaphore , ( aTimeOutSec == -1.0 ) ? INFINITE : ( (unsigned long) ( aTimeOutSec * 1000.0 ) ) ) );

         switch ( _g_sys_result )
         {
         case 0:
            if ( !fetch_compare <DWORD>( &mThreadLockerId , thread_id , 0 ) )
            {
               dec_and_fetch ( &mNumWaiters );
               mNumRecursion++;
               return true;
            }
            else 
            {
               return false;
            }
         case ETIMEDOUT:
            dec_and_fetch ( &mNumWaiters );
            return false;
         default:
            G_EXC_RAISE_POSIX();
            return false;
         }   
      }
   }
}

void CriticalSection::release ( )
{
   G_EXC_SET_CONTEXT ( "void CriticalSection::release( )" );

#if G_AUTOPTION_ON_GCC
   if ( GetCurrentThreadId () != mThreadLockerId )
#else
   if ( _m_ThreadId != mThreadLockerId )
#endif
   {  
      G_EXC_RAISE ( ReleasingNotOwnedThreadException );
   }
   else if ( !--mNumRecursion )
   {
      mThreadLockerId = 0;
   
      if ( mNumWaiters > 0 ) // AFTER it is released we check if there're waiters.
      {
         G_WIN_CALL ( ReleaseSemaphore ( mHandleSemaphore , 1 , NULL ) );
      }
   }
}

void CriticalSection::wait    ( ) { wait ( -1.0 ); }

bool CriticalSection::tryWait ( ) 
{ 
#if G_AUTOPTION_ON_MSVC
   //implementation of thread safe data is slow on gcc
   if ( _m_ThreadId == 0 )
   {
      _m_ThreadId = GetCurrentThreadId ();    
   }

   DWORD& thread_id = reinterpret_cast<DWORD&> ( _m_ThreadId );
#else
   register DWORD thread_id = GetCurrentThreadId ();
#endif

   if ( mThreadLockerId == thread_id )
   {
      mNumRecursion++;
      return true;   
   }
   else
   {
      return !fetch_compare <DWORD> ( &mThreadLockerId , thread_id , 0 );
   }
}

}//namespace mthread
}//namespace g

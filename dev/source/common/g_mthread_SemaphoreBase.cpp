#include "g_common_dev.h"
#include "g_mthread_SemaphoreBase.h"

#include <pthread.h>
#include <semaphore.h>

#if G_AUTOPTION_ON_UNIX
void inner_get_timespec ( timespec* aTimeSpecP , GTimeoutSec_t aTimeOutSec )
{
   G_EXC_SET_CONTEXT("void inner_get_timespec ( timespec* aTimeSpecP , GTimeoutSec_t aTimeOutSec )");

   int       nsecs_one_sec = 1e9;
   int       timeout_sec   = (int)aTimeOutSec;
   int       timeout_nsec  = (int) ( ( aTimeOutSec - timeout_sec )* nsecs_one_sec );
   timespec  tspec_clock;

   G_POSIX_CALL ( clock_gettime ( CLOCK_REALTIME , &tspec_clock ) );

   aTimeSpecP->tv_sec  = tspec_clock.tv_sec + timeout_sec;
   aTimeSpecP->tv_nsec = tspec_clock.tv_nsec + timeout_nsec;

   if ( aTimeSpecP->tv_nsec > nsecs_one_sec )
   {
	   aTimeSpecP->tv_nsec -= nsecs_one_sec;
	   aTimeSpecP->tv_sec++;
   }
}
#endif

namespace g
{
namespace mthread
{
void SemaphoreBase::wait ( )
{   
   G_EXC_SET_CONTEXT ( "void SemaphoreBase::wait ( )" );

#if G_AUTOPTION_ON_WINDOWS
   //use of posix allows to use Thread::cancel() correctly
   G_POSIX_CALL ( pthreadCancelableWait ( (HANDLE)mSyncObjectData ) );
#else
   G_SYS_CALL_ONLY ( sem_wait ( reinterpret_cast<sem_t*>(mSyncObjectData) ) );

   if ( _g_sys_result != 0 )
   {
      switch ( _g_sys_result = errno )
      {
         case EINTR:
            G_EXC_RAISE ( SignalInterruptedWaitExc );
            break;
         default:
            G_EXC_RAISE_POSIX ();
            break;
      }
   }
#endif
}

bool SemaphoreBase::wait ( GTimeoutSec_t aTimeOutSec )
{ 
   G_EXC_SET_CONTEXT ( "bool SemaphoreBase::wait ( GTimeoutSec_t aTimeOutSec )" );

#if G_AUTOPTION_ON_WINDOWS
   //use of posix allows to use Thread::cancel() correctly
   G_SYS_CALL_ONLY ( pthreadCancelableTimedWait ( (HANDLE)mSyncObjectData , (DWORD)( aTimeOutSec * 1000.0 ) ) );

   switch ( _g_sys_result )
   {
      case 0:         return true;
      case ETIMEDOUT: return false;
      default: G_EXC_RAISE_POSIX();return false;
   }
#else
   timespec abs_time;

   inner_get_timespec ( &abs_time , aTimeOutSec );

   G_SYS_CALL_ONLY ( sem_timedwait ( reinterpret_cast<sem_t*>(mSyncObjectData) , &abs_time ) );

   if ( _g_sys_result == 0 )
   {
      return true;
   }
   else
   {
      switch ( _g_sys_result = errno )
      {
         case EINTR:
            G_EXC_RAISE ( SignalInterruptedWaitExc );
            return false;
         case ETIMEDOUT:
            return false;
         default:
            //raise posix exception
            G_EXC_RAISE_POSIX();
            return false;
      }      
   }
#endif
}

void SemaphoreBase::put ( GUint32_t aIncrementum )
{
   G_EXC_SET_CONTEXT ( "void SemaphoreBase::put ( GUint32_t aIncrementum )" );
#if G_AUTOPTION_ON_WINDOWS
   G_WIN_CALL ( ReleaseSemaphore ( (HANDLE)mSyncObjectData , (LONG)aIncrementum , NULL ) );   
#else

   for (GUint32_t i = 0; i < aIncrementum ; ++i )
   {
	   G_POSIX_CALL ( sem_post ( reinterpret_cast<sem_t*>(mSyncObjectData) ) );
   }
#endif
}

bool SemaphoreBase::tryWait ( )
{  
#if G_AUTOPTION_ON_WINDOWS
   return wait ( 0.0 );
#else
   G_EXC_SET_CONTEXT ( "bool SemaphoreBase::tryWait ( )" );

   G_SYS_CALL_ONLY ( sem_trywait ( reinterpret_cast<sem_t*>(mSyncObjectData) ) );

   if ( _g_sys_result == 0 )
   {
      return true;
   }
   else
   {
      switch ( _g_sys_result = errno )
      {
         case EAGAIN: return false;
         default:
            G_EXC_RAISE_POSIX();
            return false;
      }
   }
#endif
}

}//namespace mthread
}//namespace g

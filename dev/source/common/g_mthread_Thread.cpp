#include "g_common_dev.h"
#include "g_mthread_Thread.h"
#include <g_cont_gstr.h>
#include <time.h>
#include <sched.h>
#include "g_atomics.h"

#if G_AUTOPTION_ON_UNIX
#	include <unistd.h>
#	include <sys/syscall.h>
#endif

namespace g
{
namespace mthread
{
static G_THREAD_SAFE_VAR const char*     _m_ts_CurrentThreadName = NULL;
static G_THREAD_SAFE_VAR ThreadAbstract* _m_ts_AssociatedThreadAbstract = NULL;

Thread::Thread ( ) : 
   mIsAttached ( false ) , 
   mId         ( 0 )
{ 
   *mThreadName = 0;
}

Thread::~Thread ( ) { detach ( ); }

void Thread::do_Sleep(double aTimeSec)
{
#if G_AUTOPTION_ON_UNIX
   timespec temp_timespec;
   timespec rem_temp_timespec;

   temp_timespec.tv_sec  = (time_t)(aTimeSec);
   temp_timespec.tv_nsec = (long)(1e9*(aTimeSec-temp_timespec.tv_sec));

   nanosleep (&temp_timespec,&rem_temp_timespec);
#else
   ::Sleep ( ( DWORD ) ( aTimeSec * 1000.0 ) );
#endif
}

GInt64_t Thread::get_CurrentThreadId ( )
{
#if G_AUTOPTION_ON_WINDOWS
   return GetCurrentThreadId ( );
#else
   return (GInt64_t) syscall (SYS_gettid); 
#endif
}

void Thread::cancel ( ) 
{
   G_EXC_SET_CONTEXT ( "void Thread::cancel ( )" );
   if ( isAttached ( ) )
   {
	  void* data = mData;

	  G_SYS_CALL_ONLY ( pthread_cancel ( *( reinterpret_cast<pthread_t*>(data) ) ) );

      switch ( _g_sys_result )
      {
         case 0:
         case ESRCH:          //do nothing
            return;
         default:
            G_EXC_RAISE_POSIX();
            return;
      }
   }
   else 
   {
      G_EXC_RAISE_MSG ( KillingNotAttachedThread , "Trying to kill a not attached g::Thread instance!" );
   }
}

void* Thread::join ( )
{
   G_EXC_SET_CONTEXT ( "void* Thread::join ( )" );

   if ( isAttached ( ) )
   {
      void* result;
      void* data = mData;
     
      G_SYS_CALL_ONLY ( pthread_join ( *( reinterpret_cast<pthread_t*>(data) ) , &result ) );
      
      switch ( _g_sys_result )
      {
         case 0:
            return result;
         // in case of a terminated thread
         case EINVAL:
         case ESRCH:
            return NULL;
         default:
            G_EXC_RAISE_POSIX();
            return NULL;
      }
   }
   else
   {
      G_EXC_RAISE_MSG ( JoiningANotAttachedThread , "Joining a not attached thread!" );
      return NULL;
   }
}

void Thread::mSetThreadName ( const char* aThreadName )
{
   if ( aThreadName == NULL || *aThreadName == 0 )
   {
      sprintf ( mThreadName , "G::THREAD::PID%u" , (unsigned int)getId ( ) );
   }
   else
   {
      strncpy ( mThreadName , aThreadName , G_THREAD_NAME_MAX_SIZE );
   }
}

void Thread::mSetThreadNameAsCurrentThreadName ( ) 
{
   if ( _m_ts_CurrentThreadName != NULL )
   {
      strncpy ( mThreadName , _m_ts_CurrentThreadName , G_THREAD_NAME_MAX_SIZE );
   }
   else
   {
      _m_ts_CurrentThreadName = mThreadName;
   }
}

void Thread::m_ResetCurrentThreadName ( ) { _m_ts_CurrentThreadName = NULL; }

void Thread::mAttach ( const void* aData )
{
   memcpy ( mData , aData , sizeof ( pthread_t ) );
   mIsAttached = true;
}

const void* Thread::detach ( )
{
   G_EXC_SET_CONTEXT ( "const void* Thread::detach ( )" );

   const void* result = NULL;

   if ( mIsAttached )
   {
      void* data = mData;
      G_SYS_CALL_ONLY( pthread_detach ( *( reinterpret_cast<pthread_t*>(data) ) ) );

      switch ( _g_sys_result )
      {
         case 0:
            result = mData;
            mIsAttached = false;
            break;
         case ESRCH: 
            mIsAttached = false;
            break;
         default:
            G_EXC_RAISE_POSIX();
            break;
      }
   }
       
   return result;
}

//return policy
static int m_get_posix_sched_policy ( GSchedulePolicy_t aSchedulePolicy )
 {
	 switch ( aSchedulePolicy )
	 {
		 case sched_other:       return SCHED_OTHER;
		 case sched_round_robin: return SCHED_RR;
		 case sched_fifo:        return SCHED_FIFO;
		 default:
			 return -1;
	 }
 }

static GSchedulePolicy_t m_get_g_sched_policy ( int aPosixPolicy )
 {
	 switch ( aPosixPolicy )
	 {
		 case SCHED_OTHER: return sched_other;
		 case SCHED_RR:   return sched_round_robin;
		 case SCHED_FIFO: return sched_fifo;

		 default:
			 return sched_unknown;
	 }
 }

GInt32_t Thread::get_MaxPriority( GSchedulePolicy_t aSchedulingPolicy )
{
   G_EXC_SET_CONTEXT ( "GInt32_t Thread::get_MaxPriority( GSchedulePolicy_t aSchedulingPolicy )" );

   int posix_sched_policy = m_get_posix_sched_policy(aSchedulingPolicy);

   if ( posix_sched_policy != -1 )
   {
      return sched_get_priority_max(posix_sched_policy);
   }
   else
   {
      G_EXC_RAISE_MSG ( InvalidSchedulePolicy , "Invalid Scheduling Policy." );
      return 0;
   }
}

GInt32_t Thread::get_MinPriority( GSchedulePolicy_t aSchedulingPolicy )
{
   G_EXC_SET_CONTEXT ( "GInt32_t Thread::get_MinPriority( GSchedulePolicy_t aSchedulingPolicy )" );

	int posix_sched_policy = m_get_posix_sched_policy(aSchedulingPolicy);

	if ( posix_sched_policy == -1 )
	{
		G_EXC_RAISE_MSG(InvalidSchedulePolicy,"Invalid Scheduling Policy.");
		return 0;
	}
	else
	{
		return sched_get_priority_min(posix_sched_policy);
	}
}

GInt32_t Thread::getPriority ( )
{
   G_EXC_SET_CONTEXT ( "GInt32_t Thread::getPriority() const" );

   sched_param temp_sched_param;
   int         policy;
   void*       data = mData;

   G_POSIX_CALL ( pthread_getschedparam( *( reinterpret_cast<pthread_t*>(data) ) , &policy , &temp_sched_param ) );

   return policy;
}

void Thread::setPriority(GInt32_t aPriority)
{
   G_EXC_SET_CONTEXT ( "void Thread::setPriority(GInt32_t aPriority)" );

   sched_param temp_sched_param = {aPriority};
   void*       data = mData;

   G_POSIX_CALL ( pthread_setschedparam ( *( reinterpret_cast<pthread_t*>(data) ) , get_ProcessSchedulePolicy() , &temp_sched_param ));
}

GSchedulePolicy_t Thread::get_ProcessSchedulePolicy ( GInt32_t aPid )
{
   G_EXC_SET_CONTEXT ( "GSchedulePolicy_t Thread::get_ProcessSchedulePolicy ( GInt32_t aPid )" );

	int   temp_posix_policy;

	G_POSIX_CALL(temp_posix_policy = sched_getscheduler(aPid) );

	GSchedulePolicy_t result = m_get_g_sched_policy(temp_posix_policy);

	if ( result == sched_unknown )
	{
		G_EXC_FATAL_ACTION ( "Not a valid posix schedule policy!" );
	}

   return result;
}

void Thread::set_ProcessSchedulePolicy ( GSchedulePolicy_t aSchedulePolicy , GInt32_t aPid )
{
   G_EXC_SET_CONTEXT ( "void Thread::set_ProcessSchedulePolicy ( GSchedulePolicy_t aSchedulePolicy , GInt32_t aPid )" );

	int posix_sched_policy = m_get_posix_sched_policy(aSchedulePolicy);

	if ( posix_sched_policy == -1 )
	{
		G_EXC_RAISE_MSG ( InvalidSchedulePolicy , "Invalid Scheduling Policy." );
	}
	else
	{
#if G_AUTOPTION_ON_UNIX
		G_POSIX_CALL ( sched_setscheduler ( aPid , posix_sched_policy , NULL ) );
#else
      G_POSIX_CALL ( sched_setscheduler ( aPid , posix_sched_policy ) );
#endif
	}
}

void Thread::do_CurrentThreadExit ( void* aExitValue ) { pthread_exit ( aExitValue ); }

ThreadAbstract* Thread::get_AssociatedThreadAbstract ( ) { return _m_ts_AssociatedThreadAbstract; }
void Thread::m_SetAsThreadAbstract    ( ThreadAbstract* aThreadAbstractP ) { _m_ts_AssociatedThreadAbstract = aThreadAbstractP; }

#if G_AUTOPTION_ON_WINDOWS && !G_OPTION_HIDE_INCOMPATIBLE
//windows only method section

int Thread::winSuspend ( )
{
   G_EXC_SET_CONTEXT ( "int Thread::winSuspend ( )" );

   G_WIN_CALL_ASSERT ( SuspendThread ( getWindowsHandle ( ) ) , _g_sys_result >= 0 );
   return (int)_g_sys_result;
}

int Thread::winResume ( )
{
   G_EXC_SET_CONTEXT( "int Thread::winResume ( )" );

   G_WIN_CALL_ASSERT ( ResumeThread ( getWindowsHandle ( ) ) , _g_sys_result >= 0 );
   return (int)_g_sys_result;   
}

void Thread::winTerminate( GInt32_t aExitCode )
{
   G_EXC_SET_CONTEXT ( "void Thread::winTerminate( GInt32_t aExitCode )" );

   G_WIN_CALL ( TerminateThread ( getWindowsHandle ( ) , aExitCode ) );
}

void* Thread::getWindowsHandle ( )
{    
   if ( isAttached ( ) )
   {
      void* data = mData;
      return pthread_getw32threadhandle_np ( *( reinterpret_cast<pthread_t*>(data) ) );
   }
   else
   {
      return NULL;
   }
}

#endif

}//namespace mthread
}//namespace g


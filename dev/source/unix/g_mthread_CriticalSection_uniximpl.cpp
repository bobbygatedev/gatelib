#include "g_mthread_CriticalSection.h"
#include <pthread.h>
#include <sys/timeb.h>
#include "g_common_dev.h"
#include <string.h>
#include <iostream>

using namespace std;


void inner_get_timespec ( timespec* aTimeSpecP , GTimeoutSec_t aTimeOutSec );

namespace g
{
namespace mthread
{

CriticalSection::CriticalSection ( ) :
   mNumRecursion   ( 0 ) ,
   mThreadLockerId ( 0 )
{
   G_EXC_SET_CONTEXT("CriticalSection::CriticalSection ( )");

   pthread_mutex_t* temp_mutex_p = mUnixCriticalSectionBuffer.ptr<pthread_mutex_t>();
   pthread_mutex_t  temp_mutex   = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;

   memcpy ( temp_mutex_p , &temp_mutex , sizeof(pthread_mutex_t) );

   //tricky way to initialise mutex
   { G_POSIX_CALL ( pthread_mutex_lock   ( temp_mutex_p ) ); }
   { G_POSIX_CALL ( pthread_mutex_unlock ( temp_mutex_p ) ); }

   mSyncObjectData =  temp_mutex_p;
}

CriticalSection::~CriticalSection()
{
   G_EXC_SET_CONTEXT ( "CriticalSection::~CriticalSection()" );
   
   G_SYS_CALL_ONLY ( pthread_mutex_destroy ( reinterpret_cast<pthread_mutex_t*> ( mSyncObjectData ) ) );

   switch ( _g_sys_result )
   {
      case 0:     return;
      case EBUSY:
    	  //for compatibility with windows
    	  //a critical section destroyed with ownerhip
    	  //is skipped.
    	  return;
      default:    G_EXC_RAISE_POSIX();return;
   }
}

// Wait until synchronisation occurs
bool CriticalSection::wait ( GTimeoutSec_t aTimeOutSec )
{    
   G_EXC_SET_CONTEXT ( "bool CriticalSection::wait ( GTimeoutSec_t aTimeOutSec )" );

   timespec abs_time;
   
   inner_get_timespec ( &abs_time , aTimeOutSec );

   G_SYS_CALL_ONLY( pthread_mutex_timedlock ( ( pthread_mutex_t* )mSyncObjectData , &abs_time ) );

   switch ( _g_sys_result )
   {
      case 0:
    	 mThreadLockerId = Thread::get_CurrentThreadId();
         mNumRecursion++;
         return true;         
      case ETIMEDOUT: return false;
      default:
         //raise posix exception
         G_EXC_RAISE_POSIX();
         return false;
   }      
}

void CriticalSection::wait ( )
{
   G_EXC_SET_CONTEXT ( "void CriticalSection::wait ( )" );
   
   G_POSIX_CALL ( pthread_mutex_lock ( ( pthread_mutex_t* )mSyncObjectData ) );
   mThreadLockerId = Thread::get_CurrentThreadId();
   ++mNumRecursion;
}

void CriticalSection::release ( ) 
{
   G_EXC_SET_CONTEXT ( "void CriticalSection::release ( )" );

   --mNumRecursion;
   G_SYS_CALL_ONLY( pthread_mutex_unlock ( ( pthread_mutex_t* )mSyncObjectData ) );

   switch ( _g_sys_result )
   {
   case 0:
      mThreadLockerId = 0;
      return;
   case EPERM:
      G_EXC_RAISE ( ReleasingNotOwnedThreadException );
      break;
   default:
      G_EXC_RAISE_POSIX();
   }
}

bool CriticalSection::tryWait ( )
{   
   G_EXC_SET_CONTEXT ( "bool CriticalSection::tryWait ( )" );
   
   G_SYS_CALL_ONLY ( pthread_mutex_trylock ( ( pthread_mutex_t* )mSyncObjectData ) );

   switch ( _g_sys_result )
   {
      case 0:  
         mThreadLockerId = Thread::get_CurrentThreadId();
         mNumRecursion++;
         return true;
      case EBUSY:  
         return false;
      default:
      {
         G_EXC_RAISE_POSIX();
      }
      return false;
   }
}

}//namespace mthread
}//namespace g

#include "g_common_dev.h"
#include "g_mthread_ThreadSelf.h"
#include <time.h>
#include <sched.h>

#if G_AUTOPTION_ON_GCC
	#include <unistd.h>
#endif

namespace g
{
namespace mthread
{

ThreadSelf::ThreadSelf ( const char* aThreadName )
{ 
   pthread_t pthread = pthread_self ( );

   mSetThreadName                    ( aThreadName );
   mSetThreadNameAsCurrentThreadName ( );

   mAttach ( &pthread );   

   mId = Thread::get_CurrentThreadId ( );
}

ThreadSelf::~ThreadSelf ( )
{ 
   //self thread is detacched avoid the exception
   mIsAttached = false;
   m_ResetCurrentThreadName ( );//thread name is cancelled
}

}//namespace mthread
}//namespace g

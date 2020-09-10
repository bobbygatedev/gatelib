#include "g_common_dev.h"
#include "g_mthread_SemaphoreAnonymous.h"
#include <sstream>
#include <limits.h>
#include <time.h>
#include <math.h>
#include <errno.h>
#include <time.h>
#include <semaphore.h>
#include <stdio.h>
#include <fcntl.h>

using namespace std;

namespace g
{
namespace mthread
{

SemaphoreAnonymous::SemaphoreAnonymous ( GUint32_t aInitialCount )
{
   G_EXC_SET_CONTEXT ( "SemaphoreAnonymous::SemaphoreAnonymous ( GUint32_t aInitialCount )" );

#if G_AUTOPTION_ON_WINDOWS

   G_WIN_CALL ( mSyncObjectData = CreateSemaphoreA ( NULL , (LONG)aInitialCount,(LONG)INT_MAX , NULL ) );

#else

   mSyncObjectData = this->mSemUnixBuffer.ptr<sem_t>();

   G_POSIX_CALL ( sem_init ( ( sem_t* )mSyncObjectData , 0 , aInitialCount ) );

#endif
}

SemaphoreAnonymous::~SemaphoreAnonymous( )
{ 
   G_EXC_SET_CONTEXT ( "SemaphoreAnonymous::~SemaphoreAnonymous( )" );

#if G_AUTOPTION_ON_WINDOWS
   
   if ( mSyncObjectData != NULL )
   {
      G_WIN_CALL ( CloseHandle ( (HANDLE)mSyncObjectData ) ); 
   }

#else
   G_POSIX_CALL ( sem_destroy  ( (sem_t*)mSyncObjectData ) );

#endif
}

}//namespace mthread
}//namespace g

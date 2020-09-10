#include "g_common_dev.h"
#include "g_mthread_MutexNamed.h"
#include <stdio.h>
#include <sys/timeb.h>
#include "g_common_dev.h"

namespace g
{
namespace mthread
{
MutexNamed::MutexNamed ( const char* aName , NamedSyncObjectCreationOptions_t aCreationOptions )
{ 
   G_EXC_SET_CONTEXT ("MutexNamed::MutexNamed ( const char* aName , bool aIsServerObject )" );

   if ( aName == NULL || *aName == 0 )
   {
      G_EXC_RAISE_MSG ( WrongSyncObjectNameException , "Named object can't have a NULL or 0-length name!" );
   }
   else
   {
      mName = aName;

      if ( aCreationOptions == g_create_always || aCreationOptions == g_create_once )
      {
         G_WIN_CALL ( mSyncObjectData = CreateMutexA ( NULL , false , aName ) );

         if ( aCreationOptions == g_create_once && ERROR_ALREADY_EXISTS == GetLastError ( ) )
         {
            char err_msg[1024];

            sprintf ( err_msg , "A server mutex object named %s already exists!" , aName );

            G_WIN_CALL ( CloseHandle(mSyncObjectData) );
            
            mSyncObjectData = NULL;

            G_EXC_RAISE_MSG ( ServerSyncObjectAlreadyExistsException , err_msg );
         }
      }
      else if ( aCreationOptions == g_open )
      {
         G_SYS_CALL_ONLY ( mSyncObjectData = OpenMutexA ( MUTEX_ALL_ACCESS , TRUE , aName ) );
         
         if ( _g_sys_result == 0 ) 
         {
            if ( GetLastError ( ) == ERROR_FILE_NOT_FOUND )
            {
               char err_msg[1024];

               sprintf ( err_msg , "A sync object named %s was never created!"  , aName );

               G_EXC_RAISE_MSG ( NamedSyncObjectNotExistsException , err_msg );
            }
            else
            {
               G_EXC_RAISE_WIN();
            }
         }
      }
      else
      {
         G_EXC_RAISE ( InvalidValueForNamedSyncObjectCreationOptionsException );      
      }
   }
}

MutexNamed::MutexNamed ( const MutexNamed& aOther )
{
   G_EXC_SET_CONTEXT("MutexNamed::MutexNamed ( const MutexNamed& aOther )")

   mName = aOther.getName ( );

   G_WIN_CALL ( mSyncObjectData = OpenMutexA ( MUTEX_ALL_ACCESS , TRUE , aOther.getName ( ).c_str() ) );
}

MutexNamed::~MutexNamed()
{
   G_EXC_SET_CONTEXT ("MutexNamed::~MutexNamed()" );

   if ( mSyncObjectData != NULL )
   {
      G_WIN_CALL ( CloseHandle ( mSyncObjectData ) );
   }
}

// Wait until synchronisation occurs
bool MutexNamed::wait ( GTimeoutSec_t aTimeOutSec )
{    
   G_EXC_SET_CONTEXT ( "bool MutexNamed::wait ( GTimeoutSec_t aTimeOutSec )" );

   G_SYS_CALL_ONLY ( WaitForSingleObject ( (HANDLE)mSyncObjectData , (DWORD)(aTimeOutSec*1000.0) ) );

	switch ( _g_sys_result )
	{
	case WAIT_OBJECT_0:
		return true;
	case WAIT_TIMEOUT:
      return false;
	case WAIT_ABANDONED:
      G_EXC_RAISE(MutexAbandonedException);
   default:
      G_EXC_RAISE_WIN();
	}

   return false;
}

void MutexNamed::wait ( )
{
   G_EXC_SET_CONTEXT ( "void MutexNamed::wait ( )" );

   G_SYS_CALL_ONLY ( WaitForSingleObject ( mSyncObjectData , INFINITE ) );

	switch ( _g_sys_result )
	{
	case WAIT_OBJECT_0:
		return;
	case WAIT_ABANDONED:      
      G_EXC_FATAL_ACTION ("Wait was abandoned" );
      break;
	default:
      G_EXC_RAISE_WIN();
      break;
	}
}

void MutexNamed::release ( ) 
{
   G_EXC_SET_CONTEXT ( "void MutexNamed::release ( )" );

   G_SYS_CALL_ONLY ( ReleaseMutex(mSyncObjectData) );

   if ( !_g_sys_result )
   {
      switch ( GetLastError ( ) )
      {
         case ERROR_NOT_OWNER:
            G_EXC_RAISE(ReleasingNotOwnedThreadException);
            break;
         default:
         G_EXC_RAISE_WIN();            
         break;
      }
   }
}

bool MutexNamed::tryWait ( ) { return wait(0.0); }

}//namespace mthread
}//namespace g



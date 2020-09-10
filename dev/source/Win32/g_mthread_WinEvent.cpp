#include "g_common.h"
#include "g_mthread_WinEvent.h"
#include "g_common_dev.h"
#include <stdio.h>

#if G_AUTOPTION_ON_WINDOWS && !G_OPTION_HIDE_INCOMPATIBLE

namespace g
{
namespace mthread
{

WinEvent::WinEvent ( const char* aName , WinEventResetMode_t aResetMode , NamedSyncObjectCreationOptions_t aCreationOptions ) : mResetMode ( aResetMode ) 
{
   G_EXC_SET_CONTEXT ( "WinEvent::WinEvent ( const char* aName , bool aIsManualReset , bool aIsServerObject )");

   if ( aName == NULL || *aName == 0 )
   {
      G_EXC_RAISE_MSG ( WrongSyncObjectNameException , "Named object can't have a NULL or 0-length name!" );
   }
   else
   {
      mName = aName;

      if ( aCreationOptions == g_create_always || aCreationOptions == g_create_once )
      {
         G_WIN_CALL ( mSyncObjectData = CreateEventA ( NULL , ( mResetMode == g_manual_reset ) ? TRUE:FALSE , FALSE , aName ) );

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
         G_SYS_CALL_ONLY ( mSyncObjectData = OpenEventA ( EVENT_ALL_ACCESS , TRUE , aName ) );
         
         if ( _g_sys_result == 0 )
         {
            if ( GetLastError ( ) == ERROR_FILE_NOT_FOUND )
            {
               char err_msg[1024];

               sprintf ( err_msg , "A server semaphore object named %s wasn't found!" , aName );

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

WinEvent::WinEvent ( WinEventResetMode_t aResetMode ) : mResetMode ( aResetMode )
{  
   G_EXC_SET_CONTEXT ( "WinEvent::WinEvent ( bool aIsManualReset )" );
   G_WIN_CALL ( mSyncObjectData = CreateEventA ( NULL , ( mResetMode == g_manual_reset ) ? TRUE:FALSE , FALSE , NULL ) );
}

WinEvent::~WinEvent( ) 
{ 
G_EXC_SET_CONTEXT ( "WinEvent::~WinEvent( )" );
   if ( mSyncObjectData != NULL )
   {
      G_WIN_CALL ( CloseHandle ( mSyncObjectData ) ); 
   }
}

void WinEvent::wait ( )
{ 
   G_EXC_SET_CONTEXT ( "void WinEvent::wait ( )"  );
	
   G_SYS_CALL_ONLY( WaitForSingleObject ( (HANDLE)mSyncObjectData , INFINITE ) );

   switch ( _g_sys_result )
	{
	case WAIT_OBJECT_0:
		return;
	case WAIT_ABANDONED:
      G_EXC_FATAL_ACTION ( "Wait was abandoned." );
	default:
      G_EXC_RAISE_WIN ( );
	}
}

bool WinEvent::wait ( GTimeoutSec_t aTimeOutSec )
{ 
   G_EXC_SET_CONTEXT ( "bool WinEvent::wait ( GTimeoutSec_t aTimeOutSec )" );

   G_SYS_CALL_ONLY( WaitForSingleObject ( (HANDLE)mSyncObjectData , (DWORD)(aTimeOutSec*1000.0) ) );
	
   switch ( _g_sys_result )
	{
	   case WAIT_OBJECT_0:
		   return true;
	   case WAIT_TIMEOUT:
	   case WAIT_ABANDONED:
		   return false;
	   default:
         G_EXC_RAISE_WIN ( );
	}

   return false;
}

bool WinEvent::tryWait ( ) { return wait (0.0); }

void WinEvent::set ( ) 
{ 
   G_EXC_SET_CONTEXT ( "void WinEvent::set ( )" );
   
   G_WIN_CALL ( SetEvent ( mSyncObjectData ) ); 
}

void WinEvent::reset ( ) 
{ 
   G_EXC_SET_CONTEXT ( "void WinEvent::reset ( )" );

   G_WIN_CALL ( ResetEvent ( mSyncObjectData ) ); 
}

}//namespace mthread
}//namespace g

#endif

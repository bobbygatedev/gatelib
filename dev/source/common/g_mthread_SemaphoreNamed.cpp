#include "g_common_dev.h"
#include "g_mthread_SemaphoreNamed.h"
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
SemaphoreNamed::SemaphoreNamed ( const char* aName , NamedSyncObjectCreationOptions_t aCreationOptions , GInt32_t aInitialCount )
{
   G_EXC_SET_CONTEXT ( "SemaphoreNamed::SemaphoreNamed ( const char* aName , GInt32_t aInitialCount )" );

   if ( aName == NULL || *aName == 0 )
   {
      G_EXC_RAISE_MSG ( WrongSyncObjectNameException , "Named object can't have a NULL or 0-length name!" );
   }
#if G_AUTOPTION_ON_WINDOWS
   else
   {
      mName = aName;

      if ( aCreationOptions == g_create_always || aCreationOptions == g_create_once )
      {
         G_WIN_CALL ( mSyncObjectData = CreateSemaphoreA ( NULL , (LONG)aInitialCount,(LONG)INT_MAX , aName ) );

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
         G_SYS_CALL_ONLY ( mSyncObjectData = OpenSemaphoreA ( SEMAPHORE_ALL_ACCESS , TRUE , aName ) );
         
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
#else
   else 
   {
      int flags;

      switch ( aCreationOptions )
      {
         case g_create_once:   flags = O_CREAT | O_EXCL; break;
         case g_create_always: flags = O_CREAT; break;
         case g_open:          flags = 0x0; break;
         default:
        	 G_EXC_RAISE ( InvalidValueForNamedSyncObjectCreationOptionsException );
        	 break;
      }

      while ( 1 )
      {
	      G_SYS_CALL_ONLY ( mSyncObjectData = sem_open( aName , flags , 0644, aInitialCount ) );

         if ( _g_sys_result == (GInt64_t)SEM_FAILED )
         {
            int err_no = errno;

            mSyncObjectData = NULL;

            switch (err_no)
            {
               case EEXIST:
               {
                  stringstream err_msg;

                  err_msg << "A semaphore named " << aName << " already exists.";

                  G_EXC_RAISE_MSG(ServerSyncObjectAlreadyExistsException,err_msg.str().c_str());
               }
               break;
               
               case EACCES:
               if ( aCreationOptions & g_create_once )//valid for g_create_always also
               {
                  { G_POSIX_CALL( sem_unlink( aName ) ); }
                  continue;
               }
               break;
                  
               case ENOENT:
               {
                  stringstream err_msg;

                  err_msg << "Trying to open a not exitsting semaphore named " << aName;

                  G_EXC_RAISE_MSG(NamedSyncObjectNotExistsException,err_msg.str().c_str());
               }
               break;

               case EINVAL:
               if ( aInitialCount > SEM_VALUE_MAX )
               {
                  G_EXC_RAISE(SemaphoreCountOverflow);
               }
               else
               {
                  stringstream err_str;

                  err_str << aName << " isn't valid for a POSIX semaphore name.";

                  G_EXC_RAISE_MSG ( WrongSyncObjectNameException , err_str.str().c_str() );
               }
               break;
               
               default:
               G_EXC_RAISE_POSIX();
               break;
            }
         }
         else
         {
            mName = aName;

            switch ( aCreationOptions )
            {
               case g_create_once: 
               case g_create_always:
                  mIsOwner = true;
               break;
               
               default: 
               break;
            }	
          
            break;	    
         }         
      }
   }
#endif
}

SemaphoreNamed::SemaphoreNamed ( const SemaphoreNamed& aOther )
{
   G_EXC_SET_CONTEXT("SemaphoreNamed::SemaphoreNamed ( const SemaphoreNamed& aOther )");

   mName = aOther.getName ( );
#if G_AUTOPTION_ON_WINDOWS
   G_WIN_CALL ( mSyncObjectData = OpenSemaphoreA ( SEMAPHORE_ALL_ACCESS , TRUE , aOther.getName().c_str() ) );
#else
   G_POSIX_CALL_ASSERT ( sem_open( aOther.getName().c_str() , 0 , 0 , 0 ) , _g_sys_result != (GInt64_t)SEM_FAILED );
   mSyncObjectData = (void*)_g_sys_result;
#endif
}

SemaphoreNamed::~SemaphoreNamed( )
{ 
   G_EXC_SET_CONTEXT ( "SemaphoreNamed::~SemaphoreNamed( )" );

#if G_AUTOPTION_ON_WINDOWS
   if ( mSyncObjectData != NULL )
   {
      G_WIN_CALL ( CloseHandle ( (HANDLE)mSyncObjectData ) ); 
   }
#else

   if ( mSyncObjectData != NULL && mIsOwner )
   {
	   {
		   G_SYS_CALL_ONLY( sem_unlink ( mName.c_str() ) );

		   if ( _g_sys_result == -1 )
		   {
			   switch (errno)
			   {
				case ENOENT:
					break;
				default:
					G_EXC_RAISE_POSIX();
					break;
			   }
		   }
	   }
   }

   { G_POSIX_CALL ( sem_close ( (sem_t*)mSyncObjectData ) ); }
#endif
}

}//namespace mthread
}//namespace g


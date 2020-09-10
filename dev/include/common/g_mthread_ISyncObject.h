#pragma once

#include "g_common.h"
#include <string>

namespace g
{
namespace mthread
{

enum NamedSyncObjectCreationOptions_t
{
   g_create_once   = 1 ,
   g_create_always = 2 ,
   g_open          = 3
};

G_EXC_DEFINE(SyncObjectException,Exception);
G_EXC_DEFINE(ServerSyncObjectAlreadyExistsException,SyncObjectException);
G_EXC_DEFINE(NamedSyncObjectNotExistsException,SyncObjectException);
G_EXC_DEFINE(WrongSyncObjectNameException,SyncObjectException);
G_EXC_DEFINE_MSG(SignalInterruptedWaitExc,SyncObjectException,"Wait was interrupted by a signal!");
G_EXC_DEFINE_MSG(InvalidValueForNamedSyncObjectCreationOptionsException,SyncObjectException,"Not a valid value for NamedSyncObjectCreationFlags!");


class G_LIB_ITEM ISyncObject 
{
public:
   ISyncObject ( ) : mIsOwner(false){ }

   virtual ~ISyncObject(){}

   //wait indefinetely for the object
   virtual void wait    ( ) = 0;

   virtual bool tryWait ( ) = 0;

   // Wait until synchronisation occurs
   virtual bool wait ( GTimeoutSec_t aTimeOutSec ) = 0;

   //on POSIX/UNIX the object that first creates the named mutex/semaphore has the 
   //ownship, when the destructor is called the object is unlinked 
   //(object is destroyed when last object is destroyed).  
   //has not effect on windows (is left for compatibility)
   bool isOwner     ( ) const { return mIsOwner; }

   //Meaning:
   //Windows:    handle of the object
   //POSIX/UNIX: pointer to the sync object struct
   void*              getData     ( ) const { return mSyncObjectData; }
   const std::string& getName     ( ) const { return mName; }
   bool               isAnonimous ( ) const { return ( mName.size() == 0 ); }

protected:
   std::string mName;
   void*       mSyncObjectData;
   bool        mIsOwner;
};

}//namespace mthread
}//namespace g

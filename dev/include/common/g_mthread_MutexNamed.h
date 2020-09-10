#pragma once
#include "g_mthread_MutexAbstract.h"

namespace g
{
namespace mthread
{

G_EXC_DEFINE_MSG ( TryingToDestroyABusyMutexException , MultithreadException , "Trying to call destructor of a busy mutex!" );

class G_LIB_ITEM MutexNamed : public MutexAbstract
{
public:
   //Create a named mutex
   MutexNamed ( const char* aName , NamedSyncObjectCreationOptions_t aCreationOptions = g_create_always );
   MutexNamed ( const MutexNamed& );
   virtual ~MutexNamed();

   virtual void wait    ( );
   virtual bool wait    ( GTimeoutSec_t aTimeOutSec );
   virtual bool tryWait ( );
   virtual void release ( );
};

}//namespace mthread
}//namespace g

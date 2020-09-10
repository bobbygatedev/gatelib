#pragma once
#include "g_mthread_ISyncObject.h"
#include "g_mthread_Thread.h"

namespace g
{
namespace mthread
{

G_EXC_DEFINE_MSG ( ReleasingNotOwnedThreadException , MultithreadException , "Releasing a not owned thread!" );
G_EXC_DEFINE_MSG ( MutexAbandonedException , SyncObjectException , "Mutex was abandoned!" );

//create an anonimous mutex
class G_LIB_ITEM MutexAbstract : public ISyncObject
{
public:
   virtual ~MutexAbstract(){}
   
   virtual void wait    ( ) = 0;
   virtual bool wait    ( GTimeoutSec_t aTimeOutSec ) = 0;
   virtual bool tryWait ( ) = 0;
   virtual void release ( ) = 0;

protected:
   MutexAbstract ( ){}
};


}//namespace mthread
}//namespace g

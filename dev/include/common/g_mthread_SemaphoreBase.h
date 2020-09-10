#pragma once
#include "g_mthread_ISyncObject.h"

namespace g
{
namespace mthread
{

//create an anonimous mutex
class G_LIB_ITEM SemaphoreBase : public ISyncObject
{
   G_COPY_CONSTRUCTOR_PROHIBITED(SemaphoreBase);
public:
   virtual ~SemaphoreBase ( ) { }
   
   virtual void wait    ( );
   virtual bool tryWait ( );
   virtual bool wait    ( GTimeoutSec_t aTimeOutSec );
   virtual void put     ( GUint32_t incrementum = 1 );

protected:
   SemaphoreBase ( ){}
};

}//namespace mthread
}//namespace g

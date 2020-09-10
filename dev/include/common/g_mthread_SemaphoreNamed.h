#pragma once

#include "g_common.h"
#include "g_mthread_SemaphoreBase.h"

namespace g
{
namespace mthread
{

G_EXC_DEFINE_MSG ( SemaphoreCountOverflow , SyncObjectException , "The semaphore count exceeds the maximum value." );

class G_LIB_ITEM SemaphoreNamed : public SemaphoreBase
{
public:
   //Create a sharable semaphore with 
   G_REF
   SemaphoreNamed ( const char* name , NamedSyncObjectCreationOptions_t aCreationOptions = g_create_always , GInt32_t initial_count = 0 );
   SemaphoreNamed ( const SemaphoreNamed& );
   virtual ~SemaphoreNamed ( );
};


}//namespace mthread
}//namespace g

#pragma once

#include "g_common.h"
#include "g_mthread_SemaphoreBase.h"

#define G_UNIX_SEMAPHORE_SIZE ( 4* sizeof(void*) )

namespace g
{
namespace mthread
{

class G_LIB_ITEM SemaphoreAnonymous : public SemaphoreBase
{
   G_COPY_CONSTRUCTOR_PROHIBITED(SemaphoreAnonymous);
public:
   G_REF
   SemaphoreAnonymous ( GUint32_t initial_count = 0 );
   virtual ~SemaphoreAnonymous();

private:
#if G_AUTOPTION_ON_UNIX
   g_raw_buffer<G_UNIX_SEMAPHORE_SIZE> mSemUnixBuffer;
#endif
};

}//namespace mthread
}//namespace g

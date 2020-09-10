#pragma once
#include "g_mthread_MutexAbstract.h"

#define G_UNIX_CRITICAL_SECTION_SIZEOF (sizeof(void*)*6)

namespace g
{
namespace mthread
{

//create an anonimous mutex
class G_LIB_ITEM CriticalSection : public MutexAbstract
{
   G_COPY_CONSTRUCTOR_PROHIBITED(CriticalSection);
public:
   CriticalSection ( );
   virtual ~CriticalSection();

   void setNumMaxSpin ( GInt32_t aSpinMax );

   virtual void wait    ( );
   virtual bool wait    ( GTimeoutSec_t aTimeOutSec );
   virtual bool tryWait ( );
   virtual void release ( );

   GInt32_t getNumRecursion   ( ) const { return mNumRecursion; }
   GInt64_t getThreadLockerId ( ) const { return mThreadLockerId; }

private:
   GInt32_t      mNumRecursion;

#if G_AUTOPTION_ON_WINDOWS
   unsigned long mThreadLockerId;
   long          mNumWaiters;
   unsigned long mNumSpinMax;
   void*         mHandleSemaphore;
#else
   GInt64_t      mThreadLockerId;

   g_raw_buffer<G_UNIX_CRITICAL_SECTION_SIZEOF> mUnixCriticalSectionBuffer;
#endif
};


}//namespace mthread
}//namespace g

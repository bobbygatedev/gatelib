#pragma once
#include "g_mthread_CriticalSection.h"
#include <string>

#define G_LOCK_DEFAULT_TIMEOUT 5.0

namespace g
{
namespace mthread
{

G_EXC_DEFINE_MSG ( GenericLockTimeoutElapsedException , g::Exception , "Lock timeout elapsed." );
G_EXC_DEFINE     ( CircularDeadlockDetectedException , g::Exception );

#define G_LOCK_CONTEXT(aobjwithlock)                  (aobjwithlock).lock ( _g_method_context )
#define G_LOCK_CONTEXT_TIMEOUT(aobjwithlock,atimeout) (aobjwithlock).lock ( _g_method_context , atimeout )
#define G_UNLOCK(aobjwithlock)                        (aobjwithlock).unlock ( );

class G_LIB_ITEM ObjWithLock
{
   G_COPY_CONSTRUCTOR_PROHIBITED ( ObjWithLock );
   friend class TimeoutTicket;
public:   
   ObjWithLock(){}
   virtual ~ObjWithLock(){}

   bool tryLock ( )                                                         { return mInnerCriticalSection.tryWait(); }
   void lock    ( const char* method_context = "" )                         { lock ( lock_Timeout , method_context ); }
   void lock    ( double aLockTimeout , const char* method_context = "" )   { if ( !mInnerCriticalSection.wait(aLockTimeout) ) { mCheckForDeadlock ( method_context ); } }
   void unlock  ( )                                                         { mInnerCriticalSection.release (); }

   static double lock_Timeout;
private:
   void mCheckForDeadlock ( const char* method_context );

   CriticalSection mInnerCriticalSection;
};

}//namespace mthread
}//namespace g

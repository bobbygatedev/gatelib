#pragma once

#include "g_common.h"

#define G_THREAD_DATA_SIZE     ( sizeof(void*)+sizeof(unsigned int) )
#define G_THREAD_NAME_MAX_SIZE    32
#define G_THREAD_CURRENT_PROCESS  0

namespace g
{
namespace mthread
{

enum GSchedulePolicy_t
{
	sched_unknown = -1 ,
	sched_other , 

	sched_round_robin,//posix-like only
	sched_fifo ,
};

G_EXC_DEFINE(MultithreadException,Exception);
G_EXC_DEFINE(WrongThreadRecreateException,MultithreadException);
G_EXC_DEFINE(NotRunningException,MultithreadException);
G_EXC_DEFINE(ThreadAlreadyCreatedException,MultithreadException);
G_EXC_DEFINE(DestructingStillExistingThread,MultithreadException);
G_EXC_DEFINE(KillingNotAttachedThread,MultithreadException);
G_EXC_DEFINE(InvalidSchedulePolicy,MultithreadException);
G_EXC_DEFINE(JoiningANotAttachedThread,MultithreadException);

typedef void* (*GthreadEntryPointPf_t) ( void* );

class ThreadAbstract;

class G_LIB_ITEM Thread
{
   friend class ThreadSelf;
   friend class ThreadAbstract;

G_COPY_CONSTRUCTOR_PROHIBITED(Thread);
protected:
   Thread ( );    //used by subclasses

public:
	virtual ~Thread ( );
	
   virtual void        cancel   ( );
   virtual void*       join     ( );
   virtual const void* detach   ( );

   GInt64_t      getId          ( ) const { return mId; }
   const char*   getThreadName  ( ) const { return mThreadName; }
   bool          isAttached     ( ) const { return mIsAttached; }
   void*         getData        ( ) { return mData; }

#if defined(WIN32) && G_OPTION_HIDE_INCOMPATIBLE == 0
	void*          getWindowsHandle ( );
	int            winSuspend       ( );
	int            winResume        ( );
   void           winTerminate      ( GInt32_t exit_code );
#endif

   static GInt32_t get_MaxPriority ( GSchedulePolicy_t = sched_other );
   static GInt32_t get_MinPriority ( GSchedulePolicy_t = sched_other );

   static GSchedulePolicy_t get_ProcessSchedulePolicy ( GInt32_t pid = G_THREAD_CURRENT_PROCESS );
   static void              set_ProcessSchedulePolicy ( GSchedulePolicy_t , GInt32_t pid = G_THREAD_CURRENT_PROCESS );

   GInt32_t        getPriority ( );
   void            setPriority ( GInt32_t priority );

   static void     do_Sleep            ( double sleep_secs );
   static GInt64_t get_CurrentThreadId ( );

   static void     do_CurrentThreadExit ( void* exit_value );

   static ThreadAbstract* get_AssociatedThreadAbstract ( );
  
protected:
   virtual void mAttach        ( const void* aData );      

   bool         mIsAttached;
   GInt64_t     mId;
   
private:
   //must be used from calling thread only
   void mSetThreadName                    ( const char* aName );
   void mSetThreadNameAsCurrentThreadName ( );
   static void m_SetAsThreadAbstract    ( ThreadAbstract* ); 
   static void m_ResetCurrentThreadName ( );

   char         mThreadName [G_THREAD_NAME_MAX_SIZE+1];
   char         mData       [G_THREAD_DATA_SIZE];
};

}//namespace mthread
}//namespace g

#include "g_mthread_ThreadSelf.h"

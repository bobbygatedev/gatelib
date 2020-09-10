#pragma once

#include "g_mthread_Thread.h"

namespace g
{
namespace mthread
{

enum ThreadState_t
{
	running = 0 ,
	detached ,
	terminated
};

//Abstract thread with entry point
class G_LIB_ITEM ThreadAbstract : public Thread 
{
   G_COPY_CONSTRUCTOR_PROHIBITED(ThreadAbstract);
public:
	// Create a thread 
	ThreadAbstract ( );
	virtual ~ThreadAbstract ( );
		
   void                start  ( void* par = NULL , const char* thread_name = NULL );
   virtual void*       join   ( );
   virtual const void* detach ( );

   ThreadState_t   getThreadState ( ) const { return mThreadState; }

protected:
   static void*    m_EntryPoint ( void* aCallingClass );
   virtual void    mEntryPoint  ( void* ) = 0;

	ThreadState_t   mThreadState;
	void*           mReturnedValue;
	void*           mPar;
};

}//namespace mthread
}//namespace g

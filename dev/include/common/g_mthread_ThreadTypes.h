#pragma once

#include "g_mthread_ThreadAbstract.h"
#include "g_signal.h"

namespace g
{
namespace mthread
{
G_EXC_DEFINE_MSG(ThreadNotHavingSubscribersException,g::mthread::MultithreadException,"Invalid call of start with no subscribers added");
}//namespace mthread
}//namespace g

G_SIGNAL(g::mthread::ThreadSimpleEvent())
G_SIGNAL(g::mthread::ThreadWithParamEvent(void* aPar))

#include G_SIGNAL_INCLUDE(g.mthread.ThreadSimpleEvent)
#include G_SIGNAL_INCLUDE(g.mthread.ThreadWithParamEvent)

namespace g
{
namespace mthread
{

#if G_AUTOPTION_ON_MSVC
#  pragma warning ( disable : 4512 )
#endif

class ThreadSimple : public ThreadAbstract , private ThreadSimpleEvent::SenderReceiver
{
	G_COPY_CONSTRUCTOR_PROHIBITED(ThreadSimple);
public:
	template <class T>ThreadSimple ( T* aP , void ( T::*method ) ( ) )
	{
		subscribe ( aP , method );	
	}
	
	ThreadSimple ( void (*simple) () )
	{
		subscribe(simple);
	}
	
	//no params event
	void start ( const char* aName = NULL )
	{ 
		ThreadAbstract::start(NULL,aName);
	}
private:
	inline void start( void* , const char* ) { ThreadAbstract::start(); }
	
	void mEntryPoint ( void* )	
	{
		send();

		doClearSubscribers();
	}
};

class ThreadWithParam : public ThreadAbstract , private ThreadWithParamEvent::SenderReceiver
{
   G_COPY_CONSTRUCTOR_PROHIBITED ( ThreadWithParam );
public:
	template <class T>ThreadWithParam ( T* aP , void ( T::*method ) ( void* ) )
	{
		subscribe ( aP , method );	
	}
	
	ThreadWithParam ( void (*simple) (void*) )
	{
		subscribe(simple);
	}
	
	inline void start( void* p ) { ThreadAbstract::start(p); }

private:
	void mEntryPoint ( void* aPar )	
	{
		send(aPar);

		doClearSubscribers();
	}
};

template <class E,class Q=g::signal::QueueTxRxSimple> struct params_thread : E::template thread_base<g::mthread::ThreadAbstract,Q>{};

#if G_AUTOPTION_ON_MSVC
#  pragma warning ( default : 4512 )
#endif


}//namespace mthread
}//namespace g

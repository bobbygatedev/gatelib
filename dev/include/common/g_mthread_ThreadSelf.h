#pragma once

#include "g_mthread_Thread.h"

namespace g
{
namespace mthread
{

class G_LIB_ITEM ThreadSelf : public Thread 
{
   G_COPY_CONSTRUCTOR_PROHIBITED ( ThreadSelf );
public: 
   ThreadSelf ( const char* thread_name = NULL );
   virtual ~ThreadSelf ( );
};

}//namespace mthread
}//namespace g

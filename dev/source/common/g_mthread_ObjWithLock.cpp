#include "g_mthread_ObjWithLock.h"
#include "g_mthread_Thread.h"
#include "g_cont_gmap.h"
#include "g_mthread_SemaphoreAnonymous.h"
#include "g_cont_ref.h"
#include <sstream>
#include <string>

#define G_TICKET_TIMEOUT 3.0

using namespace std;

namespace g
{
using namespace cont;

namespace mthread
{
class TimeoutTicket
{
public:
   TimeoutTicket(){}
   TimeoutTicket ( ObjWithLock* aObjWithLockP , const char* aCallingMethodContext ) :
      lockedCallingContext   ( aCallingMethodContext ) ,
      lockedObjectP          ( aObjWithLockP ) ,
      lockingTid             ( aObjWithLockP->mInnerCriticalSection.getThreadLockerId ( ) ) ,
      lockedTid              ( Thread::get_CurrentThreadId() ) ,
      lockedThreadName       ( ThreadSelf( ).getThreadName ( ) ) ,
      ticketSemaphore        (ref<SemaphoreAnonymous>::g_cont_new())
   { }

   const char*             lockedCallingContext;
   ObjWithLock*            lockedObjectP;
   GInt64_t                lockingTid;
   GInt64_t                lockedTid;
   std::string             lockedThreadName;
   ref<SemaphoreAnonymous> ticketSemaphore;
};

typedef gmap<GInt64_t,TimeoutTicket> TicketMap_t;

double ObjWithLock::lock_Timeout = G_LOCK_DEFAULT_TIMEOUT;

void ObjWithLock::mCheckForDeadlock ( const char* aCallingMethodContext )
{
   G_EXC_SET_CONTEXT ( "void ObjWithLock::mCheckForDeadlock ( const char* aCallingMethodContext )" );

   static TicketMap_t     timeout_ticket_map;
   static CriticalSection ticket_crit_section;

   ticket_crit_section.wait ( );

   TimeoutTicket timeout_ticket( this , aCallingMethodContext );    
   TimeoutTicket reciprocal_timeout_ticket;

   G_VERBOSE_MSG_L1 ( "mCheckForDeadlock thread id " << Thread::get_CurrentThreadId() );
   G_VERBOSE_MSG_L1 ( "Locked  TID=" << timeout_ticket.lockedTid );
   G_VERBOSE_MSG_L1 ( "Locking TID=" << timeout_ticket.lockingTid );

   if ( timeout_ticket_map.tryToGet ( timeout_ticket.lockingTid , reciprocal_timeout_ticket ) )
   {
      if ( timeout_ticket.lockedObjectP == this )
      {
         reciprocal_timeout_ticket.ticketSemaphore->put(1);

         stringstream err_msg;
                  
         err_msg << "Circular deadlock retrieved in following threads:" << endl;
         err_msg << timeout_ticket.lockedThreadName << " in calling context " << timeout_ticket.lockedCallingContext << endl;
         err_msg << reciprocal_timeout_ticket.lockedThreadName << " in calling context " << reciprocal_timeout_ticket.lockedCallingContext << endl;

         G_EXC_RAISE_MSG(CircularDeadlockDetectedException,err_msg.str().c_str());
         //protection is not unlocked on purpose in order to block multiple exception raising in application with many threads.
      }
   }
   else
   {
      //putting ticked on map
      timeout_ticket_map.doAdd(timeout_ticket.lockedTid,timeout_ticket);

      ticket_crit_section.release();

      if ( timeout_ticket.ticketSemaphore->wait ( G_TICKET_TIMEOUT ) )
      {
         //another thread has raised circular exception
         Thread::do_CurrentThreadExit((void*)-1);
      }
      //otherwise generic timeout exception is raised
   }

   G_EXC_RAISE(GenericLockTimeoutElapsedException);
}

}//namespace mthread
}//namespace g


/** @file */ 
#pragma once

#include "g_signal_signal_receiver.h"
#include "g_signal_subscribers.h"
#include "g_signal_queue.h"
#include "g_signal_queueDataBuffer.h"
#include "g_cont_ref_lst.h"
#include "g_cont_ref.h"

namespace g
{
namespace signal
{

//!Interface base class for sender serializer
class ISenderSerialized 
{
protected:
	virtual QueueSenderAbstract* mGetQueueSenderP ( ) = 0;
};

//!Interface base class for receiver serializer
class IReceiverSerialized 
{
protected:
	virtual QueueReceiverAbstract* mGetQueueReceiverP ( ) = 0;
};

/*!
\brief Instanciate a signal with a certain interface.

G_ASSIST will create an event file in the subfolder [source_location]/_g_signal.
whenever the G_SIGNAL macro is used inside a source file(*.c,*cpp,*.h,..)\n
i.e. a line such as:\n
G_SIGNAL(g::mthread::ThreadWithParamEvent(void* aPar))\n
will cause the signal assist expand the interface into a signal file in dir [[source_location]/_g_signal] and inside g::mthread namespace.\n
@param sig_if the signal interface i.e G_SIGNAL(g::mthread::ThreadWithParamEvent(void* aPar))
*/
#define G_SIGNAL(sig_if) 

/*!
   \brief Use for include a signal i.e #include G_SIGNAL_INCLUDE(g.sig.MySig).

   @param sig_path name of the path complete with namespace separated by dots '.'\n i.e. g.sig.MySig
*/
#define G_SIGNAL_INCLUDE(sig_path) G_STR(_g_signal/sig_path.h)

}//namespace signal
}//namespace g

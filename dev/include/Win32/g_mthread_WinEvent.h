#pragma once

#include "g_common.h"
#include "g_mthread_ISyncObject.h"

#if G_AUTOPTION_ON_WINDOWS && !G_OPTION_HIDE_INCOMPATIBLE

namespace g
{
namespace mthread
{

enum WinEventResetMode_t
{
   g_manual_reset ,
   g_auto_reset
};

class G_LIB_ITEM WinEvent : public ISyncObject
{
public:
   //named
   WinEvent ( const char* name , WinEventResetMode_t = g_auto_reset , NamedSyncObjectCreationOptions_t aCreationOptions = g_create_always );
   //anonimous
	WinEvent ( WinEventResetMode_t = g_auto_reset );
   virtual ~WinEvent( );

	// Wait until synchronisation occurs
	virtual bool wait    ( GTimeoutSec_t aTimeOutSec );
	virtual void wait    ( );
   virtual bool tryWait ( );
   virtual void set     ( );
   virtual void reset   ( );

   WinEventResetMode_t getResetMode ( ) const { return mResetMode; }

private:
   WinEventResetMode_t mResetMode;
};

}//namespace mthread
}//namespace g

#endif
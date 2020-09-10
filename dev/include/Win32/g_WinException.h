#pragma once

#include "g_Exception.h"

namespace g
{	

#if G_AUTOPTION_ON_MSVC != 0
#  pragma warning ( disable : 4275 )
#endif

//Exception class for encapsulating windows error
class G_LIB_ITEM WinException : public SysException
{
public:
	WinException ( const char* aWinCall , const Exception::Context& );
   virtual ~WinException ( ) { }
   
	virtual const char* getTag ( ) const { return "WinException"; }

   static void do_Raise ( const char* aSysCall , const char* aContextStr , const char* aFile , int aLine );
};

#if G_AUTOPTION_ON_MSVC != 0
#  pragma warning (default:4275)
#endif

}//namespace g


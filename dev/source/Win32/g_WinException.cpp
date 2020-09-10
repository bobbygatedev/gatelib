#include "g_common_dev.h"
#include "g_WinException.h"
#include <sstream>
#include <iostream>
#include <time.h>
#include <Psapi.h>

using namespace std;

namespace g
{

#if G_AUTOPTION_ON_MSVC
#pragma warning (disable:4244)
#endif
/********************************************************************************************
**
********************************************************************************************/
WinException::WinException ( const char* aWinCall , const Exception::Context& aContext ) : SysException ( L"" , aContext )
{
	GWchar_t buffer[64 * 1024];
	
	DWORD r_chars = FormatMessageW ( 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS ,
		NULL ,
		mCode = (GInt64_t)GetLastError ( ) ,
		MAKELANGID ( LANG_NEUTRAL , SUBLANG_DEFAULT ) , // Default language
		buffer ,
		sizeof ( buffer ) - 1 ,
      NULL );

	if ( r_chars > 0 )
	{ 
      wstringstream stream;

      stream << "Win error during call " << aWinCall << ":" << endl << buffer;

		mMessage = stream.str();
	}
	else
	{ 
		msg_box ( 
			"I couldn't display the error msg:\n"
			"probably not enough local buffer space!" ,
         "Context: WinException::WinException ( const char* aWinCall , const Exception::Context& aContext )" );
	}
}

#if G_AUTOPTION_ON_MSVC
#pragma warning (default:4244)
#endif

void WinException::do_Raise ( const char* aSysCall , const char* aContextStr , const char* aFile , int aLine )
{
   WinException exception_instance ( aSysCall , Exception::Context( aContextStr , aFile , aLine ) );

   Exception::raise_Helper ( exception_instance );
}

}//namespace g

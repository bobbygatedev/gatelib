#include "g_common_dev.h"
#include "g_str_lib.h"
#include <sstream>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <jni.h>
#include <sstream>
#include <stdlib.h>


#if G_AUTOPTION_ON_WINDOWS
	#include <windows.h>
	#include <Psapi.h>
#endif

#if G_AUTOPTION_ON_GCC
#include <unistd.h>
#endif

using namespace std;

#if G_AUTOPTION_ON_WINDOWS
static int mMsgBoxTypeTable[] = 
{
	MB_OK ,
	MB_OKCANCEL ,
	MB_RETRYCANCEL ,
	MB_CANCELTRYCONTINUE ,
	MB_YESNO ,
	MB_HELP ,
	MB_YESNOCANCEL ,
	MB_ABORTRETRYIGNORE ,
};
#endif

#define G_JAR_DEFAULT_NAME "GlibJSupport.jar"

#if G_AUTOPTION_ON_WINDOWS
#	define G_JAR_DEFAULT_FULL_PATH "c:\\glib_jar\\" G_JAR_DEFAULT_NAME
#else
#	define G_JAR_DEFAULT_FULL_PATH "/usr/lib/gatelib/" G_STR(G_VERSION)  "/" G_JAR_DEFAULT_NAME
#endif

namespace g
{

using namespace str;

std::string G_LIB_ITEM get_jar_path ( )
{
	string      result;
    const char* jar_path  = getenv ( "G_JAR_PATH" );

	if ( jar_path == NULL || jar_path[0] == 0 )
	{
#if G_AUTOPTION_ON_WINDOWS
		const char* bin_dir = getenv ( "G_BIN" );
		
		if ( bin_dir == NULL || bin_dir[0] == 0 )
		{
			return G_JAR_DEFAULT_FULL_PATH;
		}
		else
		{
			return std::string(bin_dir) + "\\" G_JAR_DEFAULT_NAME;
		}
#else
		return G_JAR_DEFAULT_FULL_PATH;
#endif
	}
	else
	{
		return jar_path;
	}
}


/********************************************************************************************
**
********************************************************************************************/
wstring G_LIB_ITEM get_exe_name ( )
{
#if G_AUTOPTION_ON_WINDOWS
	wchar_t exe_name[1024] = L"";

	GetModuleBaseNameW ( GetCurrentProcess() , NULL , exe_name , sizeof(wchar_t)*sizeof(exe_name));

	return exe_name;
#else
	char selfexe_name[FILENAME_MAX];
	int len = readlink("/proc/self/exe", selfexe_name, FILENAME_MAX - 1);

	if (len <= 0)
	{
	  // I guess we're not running on the right version of unix
	  return L"";
	}

	selfexe_name[len] = '\0';

	return wstring(selfexe_name,selfexe_name+len);
#endif
}

/******************************************************************
	It launch a ( default simple ) message box
******************************************************************/ 
MsgBoxResult_t G_LIB_ITEM msg_box (
	const char*       aMessage , 
	const char*       aCaption , 
	MsgBox_t          aMsgBoxType )
{
	wstring message ( aMessage , aMessage + strlen(aMessage) );
	wstring caption ( aCaption , aCaption + strlen(aCaption) );

	return msg_box(message.c_str(),caption.c_str(),aMsgBoxType );
}

#if G_AUTOPTION_ON_WINDOWS

/******************************************************************
	It launch a ( default simple ) message box
******************************************************************/ 
MsgBoxResult_t G_LIB_ITEM win_msg_box (
	const char* aMessage , 
	const char* aCaption , 
	MsgBox_t    aMsgBoxType )
{
   return win_msg_box ( wstring(aMessage,aMessage+strlen(aMessage)).c_str() , wstring(aCaption,aCaption+strlen(aCaption)).c_str() , aMsgBoxType );
}

/******************************************************************
	It launch a ( default simple ) message box
******************************************************************/ 
MsgBoxResult_t G_LIB_ITEM win_msg_box (
	const GWchar_t* aMessage , 
	const GWchar_t* aCaption , 
	MsgBox_t       aMsgBoxType )
{			
	switch ( 
		MessageBoxW ( 
			NULL , 
			aMessage , 
			aCaption , 
			mMsgBoxTypeTable[aMsgBoxType] ) )
	{
	default:
	case IDABORT:    return message_box_result_abort;    // Abort button was selected. 
	case IDCANCEL:   return message_box_result_cancel;   // Cancel button was selected. 
	case IDCONTINUE: return message_box_result_continue; // Continue button was selected. 
	case IDIGNORE:   return message_box_result_ignore;   // Ignore button was selected. 
	case IDNO:       return message_box_result_no;       // No button was selected. 
	case IDOK:       return message_box_result_ok;       // OK button was selected. 
	case IDRETRY:    return message_box_result_retry;    // Retry button was selected. 
	case IDTRYAGAIN: return message_box_result_try_again;// Try Again button was selected. 
	case IDYES:      return message_box_result_yes;      // Yes button was selected. 			
   case IDHELP:     return message_box_result_help;      // Yes button was selected. 			
	};

	//return message_box_result_ok;
}
#endif

/******************************************************************
	It launch a ( default simple ) message box
******************************************************************/ 
MsgBoxResult_t G_LIB_ITEM msg_box (
	const GWchar_t* aMessage , 
	const GWchar_t* aCaption , 
	MsgBox_t       aMsgBoxType )
{
#if G_AUTOPTION_ON_WINDOWS && G_OPTION_USE_WIN_MESSAGE_BOX
	return win_msg_box ( aMessage , aCaption , aMsgBoxType );
#else
	wstring msg     = (aMessage)?aMessage:L"";
	wstring caption = (aCaption)?aCaption:L"";	

	return java_msg_box ( string(msg.begin(),msg.end()).c_str() , string(caption.begin(),caption.end()).c_str() , aMsgBoxType );
#endif
}

}//namespace g

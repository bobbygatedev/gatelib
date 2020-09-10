/** @file */ 
#pragma once

#include "g_common_def.h"
#include <string>

namespace g
{
/*! 
\defgroup g_common_functions g_common_functions
\see g_common_functions.h
   *  @{
   */

   //! Types of message box.
	enum MsgBox_t
	{
		message_box_type_ok = 0                ,/**<  MB_OK ,*/
		message_box_type_ok_cancel             ,/**<  MB_OKCANCEL ,*/
		message_box_type_retry_cancel          ,/**<  MB_RETRYCANCEL ,*/
		message_box_type_cancel_retry_continue ,/**<  MB_CANCELTRYCONTINUE ,*/
		message_box_type_yes_no                ,/**<  MB_YESNO ,*/
		message_box_type_help                  ,/**<  MB_HELP ,*/
		message_box_type_yes_no_cancel         ,/**<  MB_YESNOCANCEL ,*/
		message_box_type_abort_retry_ignore    ,/**<  MB_ABORTRETRYIGNORE */
	};

   //! Possible result of a message box call.
	enum MsgBoxResult_t
	{
		message_box_result_abort = 0, /**<  IDABORT     Abort button was selected.*/
		message_box_result_cancel ,   /**<  IDCANCEL    Cancel button was selected.*/
		message_box_result_continue , /**<  IDCONTINUE  Continue button was selected.*/
		message_box_result_ignore ,   /**<  IDIGNORE    Ignore button was selected.*/
		message_box_result_no ,       /**<  IDNO        No button was selected.*/
		message_box_result_ok ,       /**<  IDOK        OK button was selected.*/
		message_box_result_retry ,    /**<  IDRETRY     Retry button was selected.*/
		message_box_result_try_again ,/**<  IDTRYAGAIN  Try Again button was selected.*/
		message_box_result_yes ,      /**<  IDYES       Yes button was selected.*/
		message_box_result_help ,     /**<  IDHELP      Yes button was selected.*/
	};

	//! Launches a ( default simple ) message box
	MsgBoxResult_t G_LIB_ITEM msg_box ( const char* msg , const char* caption = "g::lib" , MsgBox_t type = message_box_type_ok );

	//!Launches a ( default simple ) message box ( wide char version )
	MsgBoxResult_t G_LIB_ITEM msg_box ( const GWchar_t* msg , const GWchar_t* caption = L"g::lib" , MsgBox_t type = message_box_type_ok );

	/*!
		Returns the path of the auxiliary jar file containing some
		tools like the message box and exception box
   */
	std::string G_LIB_ITEM get_jar_path ( );

#if !G_OPTION_HIDE_INCOMPATIBLE
	//!Launches a message box "from a jar file"
	MsgBoxResult_t G_LIB_ITEM java_msg_box (
		const char*       aMessage , 
		const char*       aCaption = "g::lib" , 
		MsgBox_t          aMsgBoxType = message_box_type_ok );
#  ifdef G_AUTOPTION_ON_WINDOWS
	//!Launches a windows message box.
	MsgBoxResult_t G_LIB_ITEM win_msg_box (
		const char*       aMessage , 
		const char*       aCaption = "g::lib" , 
		MsgBox_t          aMsgBoxType = message_box_type_ok );

   //!Launches a windows message box.
	MsgBoxResult_t G_LIB_ITEM win_msg_box (
		const GWchar_t* aMessage , 
		const GWchar_t* aCaption = L"g::lib" , 
		MsgBox_t        aMsgBoxType = message_box_type_ok );
#  endif
#endif

	//!Returns the exe name of THIS application.
	std::wstring G_LIB_ITEM get_exe_name ( );

	//!std::wstring to std::string
	inline std::string to_StdString(const std::wstring& aString ) { return std::string(aString.begin(),aString.end()); }

   //!Converts a string to a wide strings.
	inline std::wstring to_Wstring(const std::string& aString ) { return std::wstring(aString.begin(),aString.end()); }

/*!
\brief f(v,d) is the next multiple of d(MUST be a power of 2), which is  k*d>=v && (k-1)*d<v
\param value value to be rounded. 
\param delta a power of 2 value.

i.e.\n
 T = int, delta = 4\n
 value | result\n
   0   |  0 \n
   1   |  4 \n
   2   |  4 \n
   3   |  4 \n
   4   |  4 \n
   5   |  8 \n
   6   |  8 \n
   7   |  8 \n
   8   |  8 \n
   9   |  12 (..) \n
*/ 
   template < class T > T get_rounded_by_power_of_2 ( T value , T delta )
	{
		const T mask = delta-1;
		const T mask_complementar = ~mask;

		return delta+((value-1)&mask_complementar);
	}
/*! @} */

}//namespace g




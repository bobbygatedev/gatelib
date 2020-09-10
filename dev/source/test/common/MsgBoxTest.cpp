#include "MsgBoxTest.h"

using namespace g;
using namespace std;

MsgBoxTest::MsgBoxTest() {}

MsgBoxTest::~MsgBoxTest() {}

void  plot_msg_box_result ( MsgBoxResult_t aResult )
{
   switch ( aResult )
   {
      case message_box_result_abort://= IDABORT , //Abort button was selected.
         cout << "User choose abort" << endl; break;
	   case message_box_result_cancel://= IDCANCEL: // Cancel button was selected.
         cout << "User choose cancel" << endl; break;
	   case message_box_result_continue://= IDCONTINUE: // Continue button was selected.
         cout << "User choose continue" << endl; break;
	   case message_box_result_ignore://= IDIGNORE:  // Ignore button was selected.
         cout << "User choose ignore" << endl; break;
	   case message_box_result_no://= IDNO : // No button was selected.
         cout << "User choose no" << endl; break;
	   case message_box_result_ok://= IDOK : // OK button was selected.
         cout << "User choose ok" << endl; break;
	   case message_box_result_retry://= IDRETRY : // Retry button was selected.
         cout << "User choose retry" << endl; break;
	   case message_box_result_try_again://= IDTRYAGAIN : // Try Again button was selected.
         cout << "User choose try again" << endl; break;
	   case message_box_result_yes://= IDYES  // Yes button was selected.      
         cout << "User choose yes" << endl; break;
	   case message_box_result_help:
	      cout << "User choose help" << endl; break;
   }
}

#if G_AUTOPTION_ON_WINDOWS && !G_OPTION_HIDE_INCOMPATIBLE
#	define TEST_MSG_BOX win_msg_box
#else
#	define TEST_MSG_BOX msg_box
#endif

bool MsgBoxTest::mExecute()
{
   try
   {
      {
         MsgBoxResult_t result =
               TEST_MSG_BOX ( "Do you want to perform message box test?","TEST CAPTION",message_box_type_yes_no);

         if(result != message_box_result_yes )
         {
            cout << "Skipping message box test!" << endl;

            return true;
         }
      }

      {
         MsgBoxResult_t result = TEST_MSG_BOX ("TEST MESSAGE BOX!","TEST CAPTION",message_box_type_help);
         plot_msg_box_result ( result );
      }

      for ( int i = message_box_type_ok ; i <= message_box_type_abort_retry_ignore ; i++ )
      {
         MsgBoxResult_t result = msg_box("TEST MESSAGE BOX!","TEST CAPTION",(MsgBox_t) i);

         plot_msg_box_result ( result );
      }
   }
   catch ( g::Exception& exc )
   {
      cout << "EXception raised:" << endl << exc.getFullErrorMsg() << endl;
      return false;
   }

	return true;
}


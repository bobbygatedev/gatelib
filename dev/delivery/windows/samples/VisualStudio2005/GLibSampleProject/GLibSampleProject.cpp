#include "g_cont_gstr.h"

int main ( )
{
	g::cont::Gstr test_message = "Test message";	
	g::cont::Gstr test_message_caption = "Test message caption";	   

	g::msg_box(test_message ,test_message_caption); 

	return 0;    
}
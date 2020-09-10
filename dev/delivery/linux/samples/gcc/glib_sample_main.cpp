#include "g_cont_gstr.h"
#include "g_Exception.h"

int main ( )
{
G_EXC_SET_CONTEXT("int main ( )");

G_EXC_FRAME_BEGIN()
	g::cont::Gstr test_message = "Test message";	
	g::cont::Gstr test_message_caption = "Test message caption";	   

	g::msg_box(test_message ,test_message_caption); 

	G_EXC_RAISE_MSG(g::Exception,"Test Message");
G_EXC_FRAME_END_MAIN()
	return 0;    
}
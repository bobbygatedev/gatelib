#include "g_common_dev.h"
#include "g_cont_gstr.h"
#include "g_io_common_functions.h"
#include <iostream>

using namespace std;
using namespace g;
using namespace cont;

#if G_OPTION_IS_DLL > 0 

BOOL WINAPI DllMain(
  /*_In_*/  HINSTANCE hinstDLL,
  /*_In_*/  DWORD fdwReason,
  /*_In_*/  LPVOID /*lpvReserved*/
)
{
	if ( fdwReason == DLL_PROCESS_ATTACH )
	{
      const char* jar_env_var = getenv ( "G_JAR_PATH" );

      if ( jar_env_var && g::io::is_file_existing ( jar_env_var ) )
      {
		   char dll_path_buff[MAX_PATH+1];

		   GetModuleFileNameA ( hinstDLL , dll_path_buff , sizeof(dll_path_buff) );

		   Gstr dll_path =  dll_path_buff;
		   Gstr dll_dir;

		   size_t index = dll_path.getIndexOf("\\" , 0 );

		   while ( index < dll_path.getLength() )
		   {
			   size_t new_index = dll_path.getIndexOf("\\" , index + 1 );
   		
			   if ( new_index >= dll_path.getLength() )
			   {
				   dll_dir = dll_path.left ( index );

				   putenv ( "G_JAR_PATH=" + dll_dir + "\\GlibJSupport.jar" );

				   break;
			   }
			   else
			   {
				   index = new_index;
			   }
		   }		
      }
	}

	return TRUE;
}


#endif

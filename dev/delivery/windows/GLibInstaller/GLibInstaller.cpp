#include <g_cont_gstr.h>
#include "g_win_cfg.h"
#include "g_cont_lst.h"
#include "g_io_common_functions.h"
#include <iostream>
#include <fstream>
#include <windows.h>
#include <stdio.h>

using namespace std;
using namespace g;
using namespace io;
using namespace cont;
using namespace win;
using namespace cfg;

void print_help ( )
{
	cout << "GLibInstaller -i install GATELIB into the applicationstart path!" << endl;
	cout << "GLibInstaller -u uninstall GATELIB!" << endl;
	cout << "GLibInstaller -h --help Otherwise print this help!" << endl;
}

static const char* m_VarNamesToRemove [] =
{
   "G_ROOT" ,
   "G_INCLUDE" ,
   "G_LIB" ,
   "G_BIN" ,
   "G_JAR_PATH" ,
   "G_ASSIST" ,
   "G_JAVA_JDK_HOME" ,
   NULL
};

static const char* m_VarDefaultValues [] =
{
   "%G_ROOT%Include" ,
   "%G_ROOT%lib" ,
   "%G_ROOT%bin" ,
   "%G_ROOT%bin\\GLibJSupport.jar" ,
   "java -classpath %G_ROOT%bin\\GLibJSupport.jar g.assist.CmdLineAssist " ,
   NULL
};

static const char* m_VarNames [] =
{
   "G_INCLUDE" ,
   "G_LIB" ,
   "G_BIN" ,
   "G_JAR_PATH" ,
   "G_ASSIST",
   NULL
};

void remove_user_registry_key ( const Gstr& aVarName )
{
	cout << "Deleting Env Var "	<< aVarName << "..";
	
	if ( g::win::cfg::del_value ( aVarName , "Environment" , hkey_current_user ) )
	{
		cout << "ok!" << endl;
	}
	else
	{
		cout << "Not exists!" << endl;		
	}
}

Gstr get_user_env_var ( const Gstr& aVarName )
{
	return g::win::cfg::get_key_value ( aVarName, "Environment" , hkey_current_user ).data.gstr;
}

void set_env_var ( const Gstr& aVarName , const Gstr& aVarValue )
{
	cout << "Setting Env Var " << aVarName << " to " << aVarValue << "..";
	g::win::cfg::put_string ( aVarName , aVarValue , "Environment" , hkey_current_user );
	cout << " ok!" << endl;
}

bool check_installation_dir ( const Gstr& aInstallationDir )
{
	Gstr include_dir = aInstallationDir + "\\include";
	Gstr lib_dir = aInstallationDir + "\\lib";
	Gstr bin_dir = aInstallationDir + "\\bin";

	WIN32_FIND_DATAA find_data;

	bool result = true;

	HANDLE handle = FindFirstFileA ( include_dir , &find_data );

	if ( handle == INVALID_HANDLE_VALUE )
	{
		cout << "Include dir " << include_dir << " must exist!" << endl;

		result = false;
	}

	handle = FindFirstFileA ( lib_dir , &find_data );

	if ( handle == INVALID_HANDLE_VALUE )
	{
		cout << "Lib dir " << lib_dir << " must exist!" << endl;

		result = false;
	}
	
	handle = FindFirstFileA ( bin_dir , &find_data );

	if ( handle == INVALID_HANDLE_VALUE )
	{
		cout << "Bin dir " << bin_dir << " must exist!" << endl;

		result = false;
	}

	return result;
}

void uninstall();
void uninstall_env_var();

Gstr get_detected_java_version ( )
{
   g::win::cfg::KeyValue key_value = g::win::cfg::get_key_value ( 
      "CurrentVersion" , "SOFTWARE\\JavaSoft\\Java Runtime Environment" , g::win::cfg::hkey_local_machine );

   if ( key_value.keyType == key_string )
   {
      return key_value.data.gstr;
   }
   else
   {
      return "";
   }
}

Gstr get_detected_java_jdk_home  ( )
{
   Gstr detected_java_version = get_detected_java_version ( );

   if ( detected_java_version != "" )
   {
      char key[1024];

      sprintf ( key , "SOFTWARE\\JavaSoft\\Java Development Kit\\%s" , (const char*)detected_java_version );

      Gstr result = g::win::cfg::get_key_value ( "JavaHome" , key , g::win::cfg::hkey_local_machine ).data.gstr;

      cout << "Detected jdk home " << result << endl;

      char short_path[MAX_PATH+1];

      GetShortPathNameA ( result , short_path , MAX_PATH );

      return short_path;
   }
   else
   {
      cout << "Not a valid java version detected " << endl;
      return "";
   }
}

//install in current dir
void install ( )
{
	Gstr current_dir = g::io::get_current_dir ( );

	if ( check_installation_dir ( current_dir ) )
	{
		cout << "Check installation dir ok!" << endl;

		uninstall_env_var();

      set_env_var ( "G_JAVA_JDK_HOME" , get_detected_java_jdk_home ( ) );

		for ( int i = 0 ; m_VarDefaultValues[i] ; i++ )
		{
			set_env_var ( m_VarNames[i] , Gstr(m_VarDefaultValues[i]).replace("%G_ROOT%",current_dir + "\\") );
		}	

		Gstr temp =  get_user_env_var("PATH"); 

		if ( temp.getIndexOf("%G_BIN%") >= temp.getLength())
		{
			cout << "Adding %G_BIN% to PATH environment" << endl;

			set_env_var ( "PATH" , temp + ";%G_BIN%" );
		}
		else
		{
			cout << "Adding %G_BIN% already included in PATH environment var!" << endl;		
		}
	}
   else
   {
	   cout << "Check installation dir failed!" << endl;
   }
}

void uninstall_env_var ( )
{
	cout << "Deleting old env var, if any.." << endl;

   for ( int i = 0 ; m_VarNamesToRemove[i] ; i++ )
	{
		remove_user_registry_key ( m_VarNamesToRemove[i] );
	}
}

//unistall in current dir
void uninstall ( )
{
	cout << "Removing installation.." << endl;

	uninstall_env_var ( );

   cout << "done"  << endl;
}

void main(int argc, char** argv )
{
   G_EXC_FRAME_BEGIN()

	if ( argc > 1 )
	{
		if ( argv[1]==Gstr("-u") )
		{
			uninstall ( );
		}
		else if ( argv[1]==Gstr("-i") )
		{
			install ( );
		}
		else if ( argv[1]==Gstr("-h") || argv[1]==Gstr("--help") )
		{
			print_help();
		}
		else
		{
			cout << "Argument " << argv[1] << " not recognized!" << endl;
			print_help ( );			
		}
	}
	else
	{
		install ( );
	}
 
   G_EXC_FRAME_END_MAIN()
}
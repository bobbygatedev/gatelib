#include "g_common_dev.h"
#include "g_io_common_functions.h"
#include <stdio.h>
#include <string.h>

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

#if G_AUTOPTION_ON_UNIX
#  include <sys/types.h>
#  include <sys/stat.h>
#  include <unistd.h>
#  include <limits.h>
#endif

using namespace std;

namespace g
{
namespace io
{

void G_LIB_ITEM set_current_dir ( const GWchar_t* aDirPath )
{
#if G_AUTOPTION_ON_WINDOWS
   G_EXC_SET_CONTEXT("void g::io::set_current_dir ( const GWchar_t* aDirPath )");

   G_WIN_CALL ( SetCurrentDirectoryW ( aDirPath ) );
#else
   wstring temp(aDirPath);

   set_current_dir ( string ( temp.begin() , temp.end () ).c_str() );
#endif
}

void G_LIB_ITEM set_current_dir( const char* aDirPath )
{
   G_EXC_SET_CONTEXT("void g::io::set_current_dir ( const char* aDirPath )");
#if G_AUTOPTION_ON_WINDOWS
   G_WIN_CALL ( SetCurrentDirectoryA ( aDirPath ) );
#else
   G_POSIX_CALL( chdir ( aDirPath ) );
#endif
}

bool G_LIB_ITEM is_file_existing ( const char* aFilePath )
{
#if G_AUTOPTION_ON_WINDOWS      
   return PathFileExistsA(aFilePath) && !PathIsDirectoryA(aFilePath);
#else
   G_EXC_SET_CONTEXT ( "bool g::io::is_file_existing ( const char* aFilePath )" );
 
   G_SYS_CALL_ONLY ( access ( aFilePath , F_OK ) );
    
   if ( _g_sys_result == 0 )
   {
      struct stat temp_stat;

      { G_POSIX_CALL ( stat ( aFilePath , &temp_stat ) ); }

      return S_ISREG(temp_stat.st_mode);
   }
   else if ( errno == ENOENT )
   {
      return false;
   }
   else 
   {
      G_EXC_RAISE_POSIX ( );
      return false;
   }  
#endif
}

bool G_LIB_ITEM is_file_existing ( const GWchar_t* aFilePath )
{
#if G_AUTOPTION_ON_WINDOWS
   return PathFileExistsW(aFilePath) && !PathIsDirectoryW(aFilePath);
#else
   G_EXC_SET_CONTEXT("bool g::io::is_file_existing ( const GWchar_t* aFilePath )")

G_EXC_FRAME_BEGIN()
   wstring temp(aFilePath);

   return is_file_existing ( string ( temp.begin() , temp.end () ).c_str() );
G_EXC_FRAME_FILTER_RETHROW(g::PosixException)
   return false;//avoid warning
#endif
}

bool G_LIB_ITEM is_dir_existing ( const GWchar_t* aDirPath )
{
#if G_AUTOPTION_ON_WINDOWS
   return PathFileExistsW(aDirPath) && PathIsDirectoryW(aDirPath);
#else
   G_EXC_SET_CONTEXT( "bool g::io::is_dir_existing ( const GWchar_t* aDirPath )" );
   
G_EXC_FRAME_BEGIN()
   wstring temp(aDirPath);

   return is_dir_existing ( string ( temp.begin() , temp.end () ).c_str() );
G_EXC_FRAME_FILTER_RETHROW(g::PosixException)
   return false;//avoid warning
#endif
}

bool G_LIB_ITEM is_dir_existing ( const char* aDirPath )
{
#if G_AUTOPTION_ON_WINDOWS
   return PathFileExistsA(aDirPath) && PathIsDirectoryA(aDirPath);
#else
   G_EXC_SET_CONTEXT( "bool g::io::is_dir_existing ( const char* aDirPath )" );
   
   G_SYS_CALL_ONLY ( access ( aDirPath , F_OK ) );
    
   if ( _g_sys_result == 0 )
   {
      struct stat temp_stat;

      { G_POSIX_CALL ( stat ( aDirPath , &temp_stat ) ); }

      return S_ISDIR ( temp_stat.st_mode );
   }
   else if ( errno == ENOENT )
   {
      return false;
   }
   else 
   {
      G_EXC_RAISE_POSIX ( );
      return false;
   }  
#endif
}

std::wstring G_LIB_ITEM get_current_dirW()
{
#if G_AUTOPTION_ON_WINDOWS
   G_EXC_SET_CONTEXT( "std::wstring g::io::get_current_dirW()" );

   GWchar_t aux[MAX_PATH+1];

   G_WIN_CALL ( GetCurrentDirectoryW ( MAX_PATH , aux ) );

   return aux;
#else
   string current_dir = get_current_dir ( );
   
   return wstring ( current_dir.begin() , current_dir.end ( ) );
#endif
}


std::string G_LIB_ITEM get_current_dir()
{
   G_EXC_SET_CONTEXT( "std::string g::io::get_current_dir()" );

#if G_AUTOPTION_ON_WINDOWS
   char aux[MAX_PATH+1];

   G_WIN_CALL ( GetCurrentDirectoryA ( MAX_PATH , aux ) );

   return aux;
#else
   char temp[PATH_MAX+1];
   
   G_POSIX_CALL_VALUE ( getcwd ( temp , PATH_MAX ) );
   
   return temp;
#endif
}

}//namespace io
}//namespace g

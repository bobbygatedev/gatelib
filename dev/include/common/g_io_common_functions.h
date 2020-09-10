/** @file */
#pragma once

#include "g_common_def.h"

namespace g
{
namespace io
{

/*! 
\defgroup g_io_common_functions g_io_common_functions
\see g_io_common_functions.h
*  @{
*/

   //!Sets process current directory.
   void         G_LIB_ITEM set_current_dir ( const GWchar_t* );
   //!Sets process current directory (wide char version).
   void         G_LIB_ITEM set_current_dir ( const char* );

   //!Gets process current directory.
   std::string  G_LIB_ITEM get_current_dir  ( );
   //!Gets process current directory (wide char version).
   std::wstring G_LIB_ITEM get_current_dirW ( );

   //!Returns true if a file is existing.
   bool         G_LIB_ITEM is_file_existing ( const char* );
   //!Returns true if a file is existing (wide char version).
   bool         G_LIB_ITEM is_file_existing ( const GWchar_t* );

   //!Returns true if a directory is existing.
   bool         G_LIB_ITEM is_dir_existing  ( const char* );
   //!Returns true if a directory is existing (wide char version).
   bool         G_LIB_ITEM is_dir_existing  ( const GWchar_t* );

/*! @} */

}//namespace io
}//namespace g

#pragma once

//should be included to glib developemnt file

#include "g_option.h"
#include "g_common.h"

#include <sys/stat.h> /* For mode constants */
#include <sys/types.h>

#if G_AUTOPTION_ON_WINDOWS
#  undef  _WIN32_WINNT
#  define _WIN32_WINNT 0x502
#  include <windows.h>
#  include <Shlwapi.h>
#  include <Psapi.h>
#  else
#  include <sys/mman.h>
#  include <fcntl.h> /* For O_* constants */ 
#  include <errno.h>
#endif

#if G_AUTOPTION_ON_GCC
	#include <unistd.h>
#endif

#if !G_OPTION_IS_DLL
#   define PTW32_STATIC_LIB
#endif

#include <pthread.h>



#pragma once

//autodefinition of option ON_GCC (building under MinGw/GCC)
#ifdef __GNUC__
#define G_AUTOPTION_ON_GCC 1
#else
#define G_AUTOPTION_ON_GCC 0
#endif

#if defined(WIN32) || defined (_WIN64)
#  define G_AUTOPTION_ON_WINDOWS 1
#  define G_AUTOPTION_ON_UNIX    0
#else
#  define G_AUTOPTION_ON_WINDOWS 0
#  define G_AUTOPTION_ON_UNIX    1
#endif

//autodefinition of option ON_MSVC (building under visual studio)
#ifdef _MSC_VER
#	define G_AUTOPTION_ON_MSVC 1
#	if _MSC_VER != 1400
#		pragma message ( "g::lib was developed under visual studio 2005 compiling on different IDE has not been tested!" )
#	endif
#else
#	define G_AUTOPTION_ON_MSVC 0
#endif

//setting default value for option is DLL
//remeber an option always has a default value
//his evaluation is done as follows:
// #if G_OPTION_xxx //true if G_OPTION_xxx is not zero
#ifndef G_OPTION_IS_DLL
#	define G_OPTION_IS_DLL 1 //by default artifact is a dll under Windows
#endif

#ifndef G_OPTION_USE_WIN_MESSAGE_BOX
#  define G_OPTION_USE_WIN_MESSAGE_BOX 0
#endif

#ifndef G_OPTION_HIDE_INCOMPATIBLE
#  define G_OPTION_HIDE_INCOMPATIBLE 0
#endif

#ifndef G_OPTION_IS_BUILD
#  define G_OPTION_IS_BUILD 0
#endif


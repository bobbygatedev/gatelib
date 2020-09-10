#pragma once

#include "g_option.h"

//disabled some warning on visual studio
#if G_AUTOPTION_ON_MSVC > 0 
#pragma warning (disable:4996)
#pragma warning (disable:4290)
#pragma warning (disable:4005)
#endif

//!Markator for ref assist
#define G_REF

//!G_OPTION_IS_BUILD MUST be used just inside g::lib development project and NOT defined in g::lib using programs.
#if G_AUTOPTION_ON_WINDOWS
	#if G_OPTION_IS_BUILD 
		#if G_OPTION_IS_DLL > 0 
			#define G_LIB_ITEM  __declspec(dllexport)
		#else
			#define G_LIB_ITEM
		#endif
	#else
		//use G_LIB_DLL only for importing Glibrary as dll
		#if G_OPTION_IS_DLL > 0
			#define G_LIB_ITEM __declspec(dllimport)
		#else
			#define G_LIB_ITEM
		#endif
	#endif
#else
	#define G_LIB_ITEM
#endif

#if ( G_AUTOPTION_ON_GCC > 0 )
#	define G_BUILD_MAY_ALIAS __attribute__((__may_alias__))
#else
#	define G_BUILD_MAY_ALIAS
#endif

//to be used inside a macro a macro parameter
#define G_COMMA                                ,
//!Use for for convert #defines into strings
#define G_STR(s)                               G_STR_DUMMY(s)
#define G_STR_DUMMY(s)                         #s
//!Helper for getting method pointer i.e G_METHOD_POINTER(MyCls,MyMethod)
#define G_METHOD_POINTER(cls_name,method_name) &cls_name::method_name

#if G_AUTOPTION_ON_MSVC 
#  define G_THREAD_SAFE_VAR __declspec(thread)
#else
#  define G_THREAD_SAFE_VAR __thread 
#endif

#define G_COPY_CONSTRUCTOR_PROHIBITED(aclsname) private: aclsname ( const aclsname& ) {}

#define G_BUILD_ASSERT(condition) ((void)sizeof(char[1 - 2*(!(condition))]))

typedef long long          GInt64_t;
typedef unsigned long long GUint64_t;
typedef int                GInt32_t;
typedef unsigned int       GUint32_t;
typedef double             GReal_t;
typedef wchar_t            GWchar_t;
typedef GInt64_t           GHandle_t;
typedef double             GTimeoutSec_t;

#ifndef G_VERBOSE_LEVEL
# define G_VERBOSE_LEVEL 0
#endif

//!Setting the verbosity level from 1 to 5.
#if G_VERBOSE_LEVEL
	#include <iostream>
#endif

#if G_VERBOSE_LEVEL >= 1
#	define G_VERBOSE_MSG_L1(amessage) std::cout << amessage << std::endl;
#   define G_VERBOSE_CALL_L1(acall) std::cout << G_STR(acall) << std::endl; acall;
#else
#	define G_VERBOSE_MSG_L1(amessage)
#   define G_VERBOSE_CALL_L1(acall) acall;
#endif

#if G_VERBOSE_LEVEL >= 2
#	define G_VERBOSE_MSG_L2(amessage) std::cout << amessage << std::endl;
#   define G_VERBOSE_CALL_L2(acall) std::cout << G_STR(acall) << std::endl; acall;
#else
#	define G_VERBOSE_MSG_L2(amessage)
#   define G_VERBOSE_CALL_L2(acall) acall;
#endif

#if G_VERBOSE_LEVEL >= 3
#	define G_VERBOSE_MSG_L3(amessage) std::cout << amessage << std::endl;
#   define G_VERBOSE_CALL_L3(acall) std::cout << G_STR(acall) << std::endl; acall;
#else
#	define G_VERBOSE_MSG_L3(amessage)
#   define G_VERBOSE_CALL_L3(acall) acall;
#endif

#if G_VERBOSE_LEVEL >= 4
#	define G_VERBOSE_MSG_L4(amessage) std::cout << amessage << std::endl;
#   define G_VERBOSE_CALL_L4(acall) std::cout << G_STR(acall) << std::endl; acall;
#else
#	define G_VERBOSE_MSG_L4(amessage)
#   define G_VERBOSE_CALL_L4(acall) (acall);
#endif

#if G_VERBOSE_LEVEL >= 5
#	define G_VERBOSE_MSG_L5(amessage) std::cout << amessage << std::endl;
#   define G_VERBOSE_CALL_L5(acall) std::cout << G_STR(acall) << std::endl; acall;
#else
#	define G_VERBOSE_MSG_L5(amessage)
#   define G_VERBOSE_CALL_L5(acall) (acall);
#endif

//!Raw buffer (array of char) wrapper.
template < int S > class g_raw_buffer
{
public:
	template < class T > T* ptr ()
	{
		union
		{
			//causes an error in case sizeof mBuff is insufficient
			char fake[sizeof(mBuff)-sizeof(T)];
			T* p;
		}temp;

		temp.p = reinterpret_cast<T*>(&mBuff.fake);

		return temp.p;
	}
protected:
	union
	{
	   int fake;
       char data[S];
	}mBuff;
};


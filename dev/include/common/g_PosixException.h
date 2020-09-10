#pragma once

#include "g_Exception.h"

namespace g
{	
#if G_AUTOPTION_ON_MSVC != 0
   #pragma warning (disable:4275)
#endif

//!Exception class for encapsulating windows error
class G_LIB_ITEM PosixException : public SysException
{
public:
   PosixException ( GInt64_t posix_retuned_value , const char* posix_call , const Exception::Context& );
   PosixException ( const Exception& inner , const Context& context ) : SysException(inner,context){}

   virtual const char* getTag ( ) { return "PosixException"; }

   static void do_Raise ( GInt64_t aPosixResult , const char* aPosixCall , const char* aContextStr , const char* aFile , int aLine );

private:
	void mInit( const std::wstring& aExtraDescription );
};
   
#if G_AUTOPTION_ON_MSVC != 0
   #pragma warning (default:4275)
#endif

}//namespace g


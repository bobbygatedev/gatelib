#pragma once

#include "g_common.h"
#include <string>

namespace g
{
namespace io
{
   G_EXC_DEFINE_MSG ( SharedMemOpenFailedException , g::Exception , "Shared mem object doesn't exist!" );

   enum SharedMemFlags_t
   {
      sh_read_only     = 0x0 , 
      sh_read_write    = 0x1 , 
      sh_create_always = 0x2 ,
      sh_read_only_create_always  = sh_read_only | sh_create_always ,
      sh_read_write_create_always = sh_read_write | sh_create_always 
   };

   class G_LIB_ITEM SharedMem
   {
      G_COPY_CONSTRUCTOR_PROHIBITED ( SharedMem );
   public:
      //server mem is created always
      SharedMem ( const char* aShareMemName , size_t aSize , SharedMemFlags_t aFlags = sh_read_write_create_always );
      virtual ~SharedMem();

      void*  theAddress ( ) { return mAddress; }
      size_t getSize    ( ) const { return mSize; }

   private:
#if G_AUTOPTION_ON_WINDOWS
      void*       mMappingHandle;
#else
      int         mMappingFid;
#endif
      void*       mAddress;
      std::string mShareName;
      size_t      mSize;
   };

}//namespace io
}//namespace g

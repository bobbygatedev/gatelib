#include "StdAfx.h"
#include "SharedMemory.h"

namespace gdotnet {
namespace io {

SharedMemory::SharedMemory ( System::String^ aName , unsigned int aSize , SharedMemFlags aFlags )
{
   System::IntPtr^ string_ptr = System::Runtime::InteropServices::Marshal::StringToCoTaskMemAnsi(aName);

   const char* share_name = (const char*)string_ptr->ToPointer();

   mWrappedObjP = new g::io::SharedMem(share_name,aSize,(int)aFlags);
}

SharedMemory::~SharedMemory(void)
{
   delete mWrappedObjP;
}

double SharedMemory::getDouble ( int aDataOffset )
{
   return *( ( double*) ( ( char* )mWrappedObjP->theAddress() + aDataOffset ) );
}

float SharedMemory::getSingle ( int aDataOffset )
{
   return *( ( float*) ( ( char* )mWrappedObjP->theAddress() + aDataOffset ) );
}

void SharedMemory::writeDouble ( double aDouble , int aDataOffset )
{
   *( ( double*) ( ( char* )mWrappedObjP->theAddress() + aDataOffset ) ) = aDouble;
}

void SharedMemory::writeSingle ( float aSingle , int aDataOffset )
{
   *( ( float*) ( ( char* )mWrappedObjP->theAddress() + aDataOffset ) ) = aSingle;
}

}//namespace io
}//namespace gdotnet


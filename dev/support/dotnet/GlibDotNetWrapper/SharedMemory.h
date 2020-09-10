#pragma once

#include "g_io_SharedMem.h"

using namespace System;

namespace gdotnet
{
namespace io
{

[FlagsAttribute]
public enum class SharedMemFlags : System::UInt32
{
    read = 0x0 , 
    read_and_write = 0x1 , 
    create_always  = 0x2
};

public ref class SharedMemory
{
public:
   SharedMemory ( System::String^ aName , unsigned int aSize , SharedMemFlags aFlags );
   ~SharedMemory(void);

public:

   property System::IntPtr theAddress { System::IntPtr get() {return *gcnew System::IntPtr(mWrappedObjP->theAddress());} }
   property unsigned int SharedMemory::Size { unsigned int get() { return mWrappedObjP->getSize(); } }

   void writeDouble ( double aDouble , int aDataOffset );
   void writeSingle ( float aSingle , int aDataOffset );
   double getDouble ( int aDataOffset );
   float getSingle ( int aDataOffset );

private:
   g::io::SharedMem* mWrappedObjP;
};

}//namespace io
}//namespace gdotnet

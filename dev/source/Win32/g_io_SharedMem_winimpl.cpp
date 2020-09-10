#include "g_io_SharedMem.h"
#include "g_common_dev.h"

namespace g
{
namespace io
{

//server mem is created always
SharedMem::SharedMem ( const char* aShareMemName , size_t aSize , SharedMemFlags_t aFlags ) : 
      mAddress   ( NULL ) ,
      mShareName ( aShareMemName ) ,
      mSize      ( aSize )
{
   G_EXC_SET_CONTEXT ( "SharedMem::SharedMem::SharedMem ( const char* aShareMemName , size_t aSize , SharedMemFlags_t aFlags )" );
	
   G_SYS_CALL_ONLY ( OpenFileMappingA ( FILE_MAP_READ | FILE_MAP_WRITE , TRUE , aShareMemName ) );
   
   if ( ( mMappingHandle = (HANDLE) _g_sys_result )  == NULL )
   {
      if ( ( aFlags & sh_create_always ) == sh_create_always )
      {
         G_WIN_CALL ( mMappingHandle = CreateFileMappingA ( INVALID_HANDLE_VALUE , NULL , PAGE_READWRITE , 0 , ( DWORD ) aSize , aShareMemName ) );
      }
      else
      {
         G_EXC_RAISE(SharedMemOpenFailedException);
      }
   }

   DWORD map_access = ( aFlags & sh_read_write )? ( FILE_MAP_READ | FILE_MAP_WRITE ) : FILE_MAP_READ;
   { G_WIN_CALL ( mAddress = MapViewOfFile ( mMappingHandle , map_access , 0 , 0 , aSize ) ); }
}

SharedMem::~SharedMem()
{
   G_EXC_SET_CONTEXT ( "SharedMem::~SharedMem()" );

   { G_WIN_CALL ( UnmapViewOfFile( mAddress ) ); }
   { G_WIN_CALL ( CloseHandle ( mMappingHandle ) ); }
}

}//namespace io
}//namespace g

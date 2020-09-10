#include "g_io_SharedMem.h"
#include "g_common_dev.h"
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <bits/stat.h>

namespace g
{
namespace io
{
 //server mem is created always
SharedMem::SharedMem ( const char* aShareMemName , size_t aSize , SharedMemFlags_t aFlags ) : 
   mMappingFid  ( -1 ) ,
   mAddress     ( NULL ) ,
   mShareName   (aShareMemName ) ,
   mSize        ( aSize ) 
{
   G_EXC_SET_CONTEXT ( "SharedMem::SharedMem ( const char* aShareMemName , size_t aSize , SharedMemFlags_t aFlags )" );

   int  prot   = 0x0;
   int  o_flag = O_RDWR;

   if ( aFlags & sh_read_write )
   {
	   prot |= ( PROT_READ | PROT_WRITE );
   }
   else
   {
	   prot |= PROT_READ;
   }

   if ( aFlags & sh_create_always )
   {
	   o_flag |= O_CREAT;
   }

   while ( 1 )
   {
	   G_SYS_CALL_ONLY( (GInt64_t)( mMappingFid = shm_open ( aShareMemName , o_flag , S_IRUSR | S_IWUSR ) ) );

	   if (_g_sys_result == -1 )
	   {
		   switch (errno)
		   {
			case ENOENT:
				if ( !( aFlags & sh_create_always ) )
				{
					G_EXC_RAISE(SharedMemOpenFailedException);
				}
				else
				{
					G_EXC_RAISE_POSIX();
				}
				break;
			case EACCES:
				//file permission are changed
				int temp_fid;

				{ G_POSIX_CALL_VALUE ( temp_fid = shm_open ( aShareMemName , O_CREAT | O_RDONLY , S_IRUSR ) ); }
				{ G_POSIX_CALL ( fchmod ( temp_fid , S_IRUSR | S_IWUSR ) ); }
				{ G_POSIX_CALL ( close ( temp_fid ) ); }
				{ G_POSIX_CALL_VALUE ( mMappingFid = shm_open ( aShareMemName , o_flag , S_IRUSR | S_IWUSR ) ); }
				break;
			default:
				G_EXC_RAISE_POSIX();
				break;
		   }
	   }
	   else
	   {
	      break;
       }
   }

   { G_POSIX_CALL_ASSERT ( mAddress = mmap ( NULL , aSize , prot , MAP_SHARED, mMappingFid , 0 ) , _g_sys_result != -1 ) }

   struct stat the_stat;

   fstat( mMappingFid , &the_stat );

   if ( ( size_t )the_stat.st_size < aSize )
   {
	   { G_POSIX_CALL ( ftruncate ( mMappingFid , aSize ) ); }
   }
}

SharedMem::~SharedMem()
{
   G_EXC_SET_CONTEXT ( "SharedMem::~SharedMem()" );

   if ( mAddress != NULL )
   {
	  G_POSIX_CALL ( munmap(mAddress,mSize) );
   }

   { G_POSIX_CALL ( close ( mMappingFid ) ); }
}

}//namespace io
}//namespace g

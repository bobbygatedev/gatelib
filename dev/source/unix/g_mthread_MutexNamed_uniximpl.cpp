#include "g_mthread_MutexNamed.h"
#include "g_common_dev.h"
#include <pthread.h>
#include <stdio.h>
#include <sys/timeb.h>
#include <string.h>
#include <sstream>

void inner_get_timespec ( timespec* aTimeSpecP , GTimeoutSec_t aTimeOutSec );

using namespace std;

namespace g
{
namespace mthread
{

class MutexAttribute
{
public:
	MutexAttribute()
{
		G_EXC_SET_CONTEXT("MutexAttribute::MutexAttribute()")

        { G_POSIX_CALL ( pthread_mutexattr_init       ( &mAttribute ) ); }
		{ G_POSIX_CALL ( pthread_mutexattr_setpshared ( &mAttribute , PTHREAD_PROCESS_SHARED ) ); }
		{ G_POSIX_CALL ( pthread_mutexattr_settype ( &mAttribute,PTHREAD_MUTEX_RECURSIVE) ); }
		{ G_POSIX_CALL ( pthread_mutexattr_setrobust_np(&mAttribute,PTHREAD_MUTEX_ROBUST) ); }
}

	pthread_mutexattr_t& the_Attribute(){ return mAttribute; }
private:
	pthread_mutexattr_t mAttribute;
};


MutexNamed::MutexNamed ( const char* aName , NamedSyncObjectCreationOptions_t aCreationOptions )
{ 
	G_EXC_SET_CONTEXT ("MutexNamed::MutexNamed ( const char* aName , NamedSyncObjectCreationOptions_t aCreationOptions )" );

	int    fd;
	bool   is_owner = false;

	if ( aName == NULL || *aName == 0 )
	{
		G_EXC_RAISE_MSG ( WrongSyncObjectNameException , "Named object can't have a NULL or 0-length name!" );
	}
	else if (
			aCreationOptions != g_create_always &&
			aCreationOptions != g_open &&
			aCreationOptions != g_create_once )
	{
		G_EXC_RAISE ( InvalidValueForNamedSyncObjectCreationOptionsException );
	}
	else
	{
		string shm_name = string("mutex.") + ( mName = aName );

		G_SYS_CALL_ONLY ( fd = shm_open ( shm_name.c_str() , O_RDWR , S_IRUSR | S_IWUSR ) );

		if ( fd == -1 )
		{
			switch ( errno )
			{
			case ENOENT:
				switch ( aCreationOptions )
				{
				case g_create_always:
				case g_create_once:
					is_owner = true;
					{ G_POSIX_CALL_VALUE( fd = shm_open ( shm_name.c_str() , O_CREAT | O_RDWR, S_IRUSR | S_IWUSR ) ); }
					break;
				default:
					G_EXC_RAISE_MSG ( NamedSyncObjectNotExistsException , ( mName + " do not exist and can't be opened!").c_str());
					break;
				}
				break;
				case EINVAL:
					G_EXC_RAISE_MSG ( WrongSyncObjectNameException , ( mName + " is wrong a sync object name!" ).c_str());
					break;
				default:
					G_EXC_RAISE_POSIX();
					break;
			}
		}
		else if ( aCreationOptions == g_create_once )
		{
			G_EXC_RAISE_MSG ( ServerSyncObjectAlreadyExistsException , ( mName + " is an already existing 'create once ' multithread object!").c_str() );
		}

		{ G_POSIX_CALL ( ftruncate ( fd , sizeof(pthread_mutex_t) ) ); }
		{ G_POSIX_CALL_VALUE (
				mSyncObjectData =  mmap ( NULL , sizeof(pthread_mutex_t) , PROT_READ | PROT_WRITE , MAP_SHARED , fd , 0 ) ); }
		{ G_POSIX_CALL ( close (fd) ); }

		if ( is_owner )
		{
			static MutexAttribute _mutex_attribute;

			{ G_POSIX_CALL ( pthread_mutex_init ( reinterpret_cast<pthread_mutex_t*>(mSyncObjectData) , &_mutex_attribute.the_Attribute() ) ); }
		}

		mIsOwner = is_owner;
	}
}

MutexNamed::MutexNamed ( const MutexNamed& aOther )
{
	G_EXC_SET_CONTEXT("MutexNamed::MutexNamed ( const MutexNamed& aOther )")

	string shm_name = string("mutex.") + ( mName = aOther.getName ( ) );
	int    fd;

	mIsOwner = false;

	G_SYS_CALL_ONLY ( fd = shm_open ( shm_name.c_str() , O_RDWR , S_IRUSR | S_IWUSR ) );

	if ( errno == ENOENT )
	{
		G_EXC_RAISE_MSG ( NamedSyncObjectNotExistsException , ( mName + " do not exist and can't be opened!" ).c_str());
	}
	else
	{
		switch ( errno )
		{
		case EINVAL:
			G_EXC_RAISE_MSG ( WrongSyncObjectNameException , ( mName + " is wrong a sync object name!" ).c_str() );
			break;
		default:
			G_EXC_RAISE_POSIX();
			break;
		}
	}

	{ G_POSIX_CALL ( ftruncate ( fd , sizeof(pthread_mutex_t) ) ); }
	{ G_POSIX_CALL_VALUE (
			mSyncObjectData = mmap ( NULL , sizeof(pthread_mutex_t) , PROT_READ | PROT_WRITE , MAP_SHARED , fd , 0 ) ); }
	{ G_POSIX_CALL ( close (fd) ); }
}

MutexNamed::~MutexNamed()
{
	G_EXC_SET_CONTEXT ( "MutexNamed::~MutexNamed()" );

	{ G_POSIX_CALL ( munmap ( mSyncObjectData , sizeof(pthread_mutex_t) ) ); }

	if ( mIsOwner )
	{
		string shm_name = string("mutex.") + mName;
		{ G_POSIX_CALL ( shm_unlink ( shm_name.c_str() ) ); }
	}
}

// Wait until synchronisation occurs
bool MutexNamed::wait ( GTimeoutSec_t aTimeOutSec )
{    
	G_EXC_SET_CONTEXT ( "bool MutexNamed::wait ( GTimeoutSec_t aTimeOutSec )" );

	timespec abs_time;

	inner_get_timespec ( &abs_time , aTimeOutSec );

	G_SYS_CALL_ONLY ( pthread_mutex_timedlock ( reinterpret_cast<pthread_mutex_t*>(mSyncObjectData) , &abs_time ) );

	switch ( _g_sys_result )
	{
	   case 0: return true;
	   case ETIMEDOUT: return false;
	   case EOWNERDEAD:
   	   { G_POSIX_CALL ( pthread_mutex_consistent ( reinterpret_cast<pthread_mutex_t*>(mSyncObjectData) ) ); }
   	   return true;
	   default:
		   //raise posix exception
		   G_EXC_RAISE_POSIX();
		   return false;
	}
}

void MutexNamed::wait ( )
{
	G_EXC_SET_CONTEXT ( "void MutexNamed::wait ( )" );

	G_SYS_CALL_ONLY ( pthread_mutex_lock ( reinterpret_cast<pthread_mutex_t*>(mSyncObjectData) ) );

	switch ( _g_sys_result )
	{
	case 0:
	break;//success
   	case EOWNERDEAD:
      	{ G_POSIX_CALL ( pthread_mutex_consistent ( reinterpret_cast<pthread_mutex_t*>(mSyncObjectData) ) ); }
   	break;
   	default:
		   //raise posix exception
		   G_EXC_RAISE_POSIX();
		break;
	}
}

void MutexNamed::release ( ) 
{
	G_EXC_SET_CONTEXT ( "void MutexNamed::release ( )" );

	G_SYS_CALL_ONLY( pthread_mutex_unlock ( reinterpret_cast<pthread_mutex_t*>(mSyncObjectData) ) );

	switch ( _g_sys_result )
	{
	case 0: return;
	case EPERM:
		G_EXC_RAISE ( ReleasingNotOwnedThreadException );
		break;
	default:
		G_EXC_RAISE_POSIX();
		break;
	}
}

bool MutexNamed::tryWait ( )
{   
	G_EXC_SET_CONTEXT ( "bool MutexNamed::tryWait ( )" );

	G_SYS_CALL_ONLY( pthread_mutex_trylock ( reinterpret_cast<pthread_mutex_t*>(mSyncObjectData) ) );

	switch ( _g_sys_result )
	{
	case 0:      return true;
	case EBUSY:  return false;
	default:
	{
		G_EXC_RAISE_POSIX();
	}
	return false;
	}
}

}//namespace mthread
}//namespace g



#pragma once

#include "g_common.h"
#include <vector>
#include <list>

namespace g
{
namespace win
{
namespace cfg
{

G_EXC_DEFINE(WinCfgRegistryException,Exception);

typedef GInt64_t KeyHandle_t;

 //Tipo di key
typedef enum KeyType_t
{
	key_none	= 0,
	key_string ,
	key_exp_string ,
	key_binary ,
	key_dword ,
	key_dword_lendian	,
	key_dword_bendian ,
	key_link	, 
	key_multi_string ,
	key_resource_list	,
	key_full_resource_descriptor ,
	key_resource_requirements_list ,
	key_qword ,
	key_qword_lendian 
};

typedef enum KeyAccessMode_t
{
	get = 0 ,
	set ,
	get_set , 
	create_sub_key ,
	enum_sub_keys
};

//Main registry key
typedef enum MainKey_t
{
	hkey_root = 0 ,
	hkey_current_user ,
	hkey_local_machine ,
	hkey_users ,
	hkey_performance_data ,
	hkey_performance_text ,
	hkey_performance_nlstext ,
	hkey_current_config ,
	hkey_dyn_data
};

	//static const size_t infinite  = 0xffffffff;
	typedef std::list<std::string> StrList;

	struct G_LIB_ITEM KeyValue 
	{
		typedef std::list<KeyValue>    List;

		KeyValue( )
		{
			keyType = key_none;
		}

		KeyValue( const KeyValue& k ) { operator = ( k ); }

		KeyValue& operator = ( const KeyValue& k )
		{
			name    = k.name;
			data    = k.data;
			keyType = k.keyType;

			return *this;
		}
	
		std::string  name;
		KeyType_t    keyType;
		
		struct Data
		{
			Data ( )
			{
				dword = 0;			
			}

			Data ( const Data& );

			typedef std::vector<unsigned char> Data_t;

			Data& operator = ( const Data& d );

			std::string gstr;
			StrList     multiLine;			
			GUint32_t   dword;
			Data_t      binary;
		};
		
		Data data;		
	};

	struct KeyInfo
	{
		KeyInfo ( )
		{
			numSubKeys     = 0;
			maxSubKeyLen   = 0;
			numKeyValues   = 0;
			maxKeyValueLen = 0;		
		}
		
		size_t numSubKeys;
		size_t maxSubKeyLen;
		size_t numKeyValues;
		size_t maxKeyValueLen;
	};

	/************************************************************************************
		Retrieve info about a key
	*************************************************************************************/
	KeyInfo G_LIB_ITEM get_key_info ( KeyHandle_t aKey );

	/************************************************************************************
		Given a certain aCfgKeyName , it writes a String to aCfgKeyName. The key is
		created in any case.
		NOTICE!
		aCfgKeyName is pasted to aCfgSubKey to form the key path.
		Since aCfgMainKey is by default HKEY_LOCAL_MACHINE and aCfgSubKey is SOFTWARE\GLIB
		default location aCfgKeyName will be:
		HKEY_LOCAL_MACHINE\SOFTWARE\GLIB
		aCfgKeyName can be also a path ( i.e SubKey1\SubKey2\KeyName )
	*************************************************************************************/
	void G_LIB_ITEM put_string ( 
		const char* cfg_key , 
		const char* gstr , 
		const char* = "SOFTWARE\\GLIB" , 
		MainKey_t = hkey_local_machine );

	/************************************************************************************
		Given a certain aCfgKeyName , it writes aDataP to aCfgKeyName. The key is
		created in any case.
		NOTICE!
		aCfgKeyName is pasted to aCfgSubKey to form the key path.
		Since aCfgMainKey is by default HKEY_LOCAL_MACHINE and aCfgSubKey is SOFTWARE\GLIB
		default location aCfgKeyName will be:
		HKEY_LOCAL_MACHINE\SOFTWARE\GLIB
		aCfgKeyName can be also a path ( i.e SubKey1\SubKey2\KeyName )
	*************************************************************************************/
	void G_LIB_ITEM put_cfg_data ( 
		const char* , 
		const void* , 
		size_t , 
		KeyType_t   = key_none , 
		const char* = "SOFTWARE\\GLIB" , 
		MainKey_t   = hkey_local_machine ) throw (WinException);
		
	/************************************************************************************
		Given a certain aCfgKeyName , it retrieve the data associated to it, if the key
		exist, and returns the amount of written data. It key open fails ( key doesn't 
		exist ) returned value is zero!
		NOTICE!
		aCfgKeyName is pasted to aCfgSubKey to form the key path.
		Since aCfgMainKey is by default HKEY_LOCAL_MACHINE and aCfgSubKey is SOFTWARE\GLIB
		default location aCfgKeyName will be:
		HKEY_LOCAL_MACHINE\SOFTWARE\GLIB
		aCfgKeyName can be also a path ( i.e SubKey1\SubKey2\KeyName )
	*************************************************************************************/
	size_t G_LIB_ITEM get_cfg_data ( 
      const char* key_name , void* , size_t , KeyType_t* = NULL , const char* = "SOFTWARE\\GLIB" , MainKey_t = hkey_local_machine );

	/************************************************************************************
		Given a certain aCfgKeyName , it retrieve the data associated to it, if the key
		exist, and returns the interpreted value
		NOTICE!
		aCfgKeyName is pasted to aCfgSubKey to form the key path.
		Since aCfgMainKey is by default HKEY_LOCAL_MACHINE and aCfgSubKey is SOFTWARE\GLIB
		default location aCfgKeyName will be:
		HKEY_LOCAL_MACHINE\SOFTWARE\GLIB
		aCfgKeyName can be also a path ( i.e SubKey1\SubKey2\KeyName )
	*************************************************************************************/
	KeyValue G_LIB_ITEM get_key_value ( const char* name , const char* subkey , MainKey_t = hkey_local_machine );

	/************************************************************************************
		Given a certain aCfgKeyName , delete the associated key.
		NOTICE!
		aCfgKeyName is pasted to aCfgSubKey to form the key path.
		Since aCfgMainKey is by default HKEY_LOCAL_MACHINE and aCfgSubKey is SOFTWARE\GLIB
		default location aCfgKeyName will be:
		HKEY_LOCAL_MACHINE\SOFTWARE\GLIB
		aCfgKeyName can be also a path ( i.e SubKey1\SubKey2\KeyName )
	*************************************************************************************/
	bool G_LIB_ITEM del_key ( const char* , MainKey_t = hkey_local_machine );

	/************************************************************************************
		Given a certain aCfgKeyName , delete the associated key value.
		NOTICE!
		aCfgKeyName is pasted to aCfgSubKey to form the key path.
		Since aCfgMainKey is by default HKEY_LOCAL_MACHINE and aCfgSubKey is SOFTWARE\GLIB
		default location aCfgKeyName will be:
		HKEY_LOCAL_MACHINE\SOFTWARE\GLIB
		aCfgKeyName can be also a path ( i.e SubKey1\SubKey2\KeyName )
	*************************************************************************************/
	bool G_LIB_ITEM del_value ( const char* , const char* = "SOFTWARE\\GLIB" , MainKey_t = hkey_local_machine );

	/************************************************************************************
		Given a certain aCfgKeyName , open the associated key.
		NOTICE!
		aCfgKeyName is pasted to aCfgSubKey to form the key path.
		Since aCfgMainKey is by default HKEY_LOCAL_MACHINE and aCfgSubKey is SOFTWARE\GLIB
		default location aCfgKeyName will be:
		HKEY_LOCAL_MACHINE\SOFTWARE\GLIB
		aCfgKeyName can be also a path ( i.e SubKey1\SubKey2\KeyName )
	*************************************************************************************/
	KeyHandle_t G_LIB_ITEM open_key ( const char* , MainKey_t = hkey_local_machine , KeyAccessMode_t = get_set ); 
	
	/************************************************************************************
		Enumera tutte le chiavi di registro ad una data posizione
	*************************************************************************************/
	KeyValue::List G_LIB_ITEM get_all_key_values ( 
		const char* = "SOFTWARE\\GLIB" , MainKey_t = hkey_local_machine );

	/************************************************************************************
		Close a certain HANDLE to a register key.
	*************************************************************************************/
	void G_LIB_ITEM key_close ( KeyHandle_t hKey );

	/************************************************************************************
		Flush pending data to a certain by HANDLE handled pointed register key.
	*************************************************************************************/
	void G_LIB_ITEM key_flush ( KeyHandle_t hKey );

}//namespace cfg
}//namespace win
}//namespace g


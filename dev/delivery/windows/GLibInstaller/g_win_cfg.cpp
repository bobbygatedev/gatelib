#include <windows.h>
#include <sstream>
#include <iostream>
#include "g_common.h"
#include "g_win_cfg.h"
#include "g_car_base.h"

using namespace std;

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

namespace g
{
	using namespace car;
namespace win
{
namespace cfg
{

static const int mKeyTypeTable[] = 
{
	REG_NONE ,                       //( 0 )   // No value type
	REG_SZ ,                         //( 1 )   // Unicode nul terminated gstr
	REG_EXPAND_SZ ,                  //( 2 )   // Unicode nul terminated gstr
	REG_BINARY ,                     //( 3 )   // Free form binary
	REG_DWORD ,                      //( 4 )   // 32-bit number
	REG_DWORD_LITTLE_ENDIAN ,        //( 4 )   // 32-bit number //(same as REG_DWORD)
	REG_DWORD_BIG_ENDIAN ,           //( 5 )   // 32-bit number
	REG_LINK ,                       //( 6 )   // Symbolic Link //(unicode)
	REG_MULTI_SZ ,                   //( 7 )   // Multiple Unicode strings
	REG_RESOURCE_LIST ,              //( 8 )   // Resource list in the resource map
	REG_FULL_RESOURCE_DESCRIPTOR ,   //( 9 )  // Resource list in the hardware description
	REG_RESOURCE_REQUIREMENTS_LIST , //( 10 )
	REG_QWORD ,                      //( 11 )  // 64-bit number
	REG_QWORD_LITTLE_ENDIAN          //( 11 )  // 64-bit number (same as REG_QWORD)
};

static const int mKeyAccessFlagTable[] = 
{
	KEY_QUERY_VALUE ,                 // get
	KEY_SET_VALUE ,                   // set
	KEY_QUERY_VALUE | KEY_SET_VALUE , // get_set
	KEY_CREATE_SUB_KEY ,              // create_sub_key
	KEY_ENUMERATE_SUB_KEYS ,          // enum_sub_keys
};

static HKEY mKeyTable [] = 
{
	HKEY_CLASSES_ROOT ,
	HKEY_CURRENT_USER ,
	HKEY_LOCAL_MACHINE ,
	HKEY_USERS ,
	HKEY_PERFORMANCE_DATA ,
	HKEY_PERFORMANCE_TEXT ,
	HKEY_PERFORMANCE_NLSTEXT ,
	HKEY_CURRENT_CONFIG ,
	HKEY_DYN_DATA
};

static std::string m_join ( const StrList& aLst )
{
	std::string res;

	for ( StrList::const_iterator 
		it = aLst.begin ( ) ;
		it != aLst.end ( ) ;
		it++ )
	{
		res += *it;

		if ( ++StrList::const_iterator ( it ) != aLst.end ( ) )
		{
			res += "\r\n";
		}
	}

	return res;
}

	/************************************************************************************
		Given a certain aKeyName , it writes aDataP to aKeyName. The key is
		created in any case.
		NOTICE!
		aKeyName is pasted to aCfgSubKey to form the key path.
		Since aCfgMainKey is by default HKEY_LOCAL_MACHINE and aCfgSubKey is SOFTWARE\GLIB
		default location aKeyName will be:
		HKEY_LOCAL_MACHINE\SOFTWARE\GLIB
		aKeyName can be also a path ( i.e SubKey1\SubKey2\KeyName )
	*************************************************************************************/
	void G_LIB_ITEM put_cfg_data ( 
		const char*  aKeyName , 
		const void*  aDataP , 
		size_t       aDataSize , 
		KeyType_t    aDetectedRegKeyType , 
		const char*  aCfgSubKey , 
		MainKey_t    aCfgMainKey ) throw(WinException)
	{
      G_EXC_SET_CONTEXT (
	G_STR(void G_LIB_ITEM put_cfg_data ( 
		const char*  aKeyName , 
		const void*  aDataP , 
		size_t       aDataSize , 
		KeyType_t    aDetectedRegKeyType , 
		const char*  aCfgSubKey , 
		MainKey_t    aCfgMainKey )));

      DWORD       dw_disp;
		HKEY        h_key;	
		std::string res = "";

      { G_WIN_CALL_ASSERT(
         RegCreateKeyExA ( mKeyTable[aCfgMainKey] , aCfgSubKey , 0 , NULL , REG_OPTION_NON_VOLATILE , KEY_WRITE , NULL , &h_key , &dw_disp ) ,
         _g_sys_result == ERROR_SUCCESS ); }

      { G_WIN_CALL_ASSERT(
         RegSetValueExA ( h_key , ( const char* )aKeyName , 0 , ( DWORD ) aDetectedRegKeyType , ( const BYTE* ) aDataP , ( DWORD ) aDataSize ) ,
         _g_sys_result == ERROR_SUCCESS ); }

		key_flush ( ( KeyHandle_t )h_key );
		key_close ( ( KeyHandle_t )h_key );
	}
		
	/************************************************************************************
		Given a certain aKeyName , it retrieve the data associated to it, if the key
		exist, and returns the amount of written data. It key open fails ( key doesn't 
		exist ) returned value is zero!
		NOTICE!
		aKeyName is pasted to aCfgSubKey to form the key path.
		Since aCfgMainKey is by default HKEY_LOCAL_MACHINE and aCfgSubKey is SOFTWARE\GLIB
		default location aKeyName will be:
		HKEY_LOCAL_MACHINE\SOFTWARE\GLIB
		aKeyName can be also a path ( i.e SubKey1\SubKey2\KeyName )
	*************************************************************************************/
	size_t G_LIB_ITEM get_cfg_data ( 
		const char*  aKeyName , 
		void*        aBuffP, 
		size_t       aBuffMaxSize ,
		KeyType_t*   aDetectedRegKeyTypeP , 
		const char*  aCfgSubKey , 
		MainKey_t    aCfgMainKey )
	{
		HKEY  h_key     = ( HKEY )open_key ( aCfgSubKey , aCfgMainKey , get );	
		DWORD ret_bytes = ( DWORD )aBuffMaxSize;
	
		if ( h_key )
		{
			DWORD detected_key_type;

			if ( RegQueryValueExA ( h_key , aKeyName , 0 , &detected_key_type , ( LPBYTE )  aBuffP ,  ( LPDWORD ) &ret_bytes ) != ERROR_SUCCESS )
			{
            G_VERBOSE_MSG_L2 ( "Failed to found!" );
				return 0x0;
			}

			if ( aDetectedRegKeyTypeP ) 
			{
            G_VERBOSE_MSG_L2 ( "Detected type " << detected_key_type );
				*aDetectedRegKeyTypeP = ( KeyType_t ) detected_key_type;
			}

			key_flush ( ( KeyHandle_t )h_key );
			key_close ( ( KeyHandle_t )h_key );

         return ret_bytes;
      }	
      else
      {
         G_VERBOSE_MSG_L2 ( "Key not found!" );
         return 0;
      }
	}

	/************************************************************************************
		Given a certain aKeyName , it writes a String to aKeyName. The key is
		created in any case.
		NOTICE!
		aKeyName is pasted to aCfgSubKey to form the key path.
		Since aCfgMainKey is by default HKEY_LOCAL_MACHINE and aCfgSubKey is SOFTWARE\GLIB
		default location aKeyName will be:
		HKEY_LOCAL_MACHINE\SOFTWARE\GLIB
		aKeyName can be also a path ( i.e SubKey1\SubKey2\KeyName )
	*************************************************************************************/
	void G_LIB_ITEM put_string ( 
		const char*  aKeyName , 
		const char*  aString ,
		const char*  aCfgSubKey , 
		MainKey_t    aCfgMainKey )
	{
		put_cfg_data ( 
			aKeyName , 
			aString , 
			strlen ( aString ) , 
			key_string , 
			aCfgSubKey , 
			aCfgMainKey );
	}	

	/************************************************************************************
		Sets current directory
	*************************************************************************************/
	KeyInfo G_LIB_ITEM get_key_info ( KeyHandle_t aKey )
	{
      G_EXC_SET_CONTEXT( "KeyInfo g::cfg::get_key_info ( KeyHandle_t aKey )" );
		KeyInfo info;

		DWORD num_keys      = 0;
		DWORD max_key_len   = 0;
		DWORD num_values    = 0;
		DWORD max_value_len = 0;

      G_WIN_CALL_ASSERT ( RegQueryInfoKey (
				( HKEY )aKey ,
				NULL ,
				NULL ,
				NULL ,
				&num_keys ,
				&max_key_len ,
				NULL ,
				&num_values ,
				NULL ,
				&max_value_len ,
				NULL ,
				NULL ) , _g_sys_result == ERROR_SUCCESS )

      info.maxKeyValueLen = max_value_len;
		info.maxSubKeyLen   = max_key_len;
		info.numKeyValues   = num_values;
		info.numSubKeys     = num_keys;

		return info;
	}

	/************************************************************************************
		It decodes a multiline into a gstr-list coded as follows
		One\'0Two'\0
	*************************************************************************************/
	void m_decode_multiline ( StrList& aStrList , const char* aInDataP , size_t aSize )
	{
		aStrList.clear ( );

		const char* p = aInDataP;

		for ( size_t i = 0 ; i < aSize ; i += strlen ( p ) + 1 )
		{
			p = aInDataP + i;
			
			aStrList.push_back ( p );
		}
	}

	KeyValue::Data m_key_value_data ( KeyType_t aDataType , const void* aData , size_t aSize )
	{
		KeyValue::Data res;
	
		const char* d_p = ( const char* )aData;

		switch ( aDataType )
		{
			case key_dword:
			{
				std::stringstream sstr;
				
				sstr << res.dword;
				res.dword  = *( ( DWORD* ) d_p );
				res.gstr = sstr.str ( );
				break; 
			}
			case key_multi_string:			
			{
				m_decode_multiline ( res.multiLine , d_p , aSize );
				res.gstr = m_join ( res.multiLine );

				break;
			}
			case key_string:
			case key_exp_string:
				res.gstr = d_p;
				break;
			case key_none:
			case key_binary:
				res.binary.resize ( aSize );

				memcpy ( &( res.binary[0] ) , aData , sizeof ( unsigned char ) * aSize ); 
				break;
		}
	
		return res;
	}

	/************************************************************************************
		Sets current directory
	*************************************************************************************/
	KeyValue::List G_LIB_ITEM get_all_key_values ( const char*  aSubKey , MainKey_t    aMainKey )
	{
      G_EXC_SET_CONTEXT("KeyValue::List g::cfg::get_all_key_values ( const char*  aSubKey , MainKey_t    aMainKey )");
		HKEY           r_key;
		KeyValue::List result;

		if ( RegOpenKeyExA ( mKeyTable[aMainKey] , aSubKey , 0 , KEY_READ , &r_key ) != ERROR_SUCCESS )
		{
			return result;
		}

		KeyInfo info = get_key_info ( ( KeyHandle_t ) r_key );

		char  name[MAX_VALUE_NAME] = "";
		DWORD n_len;
		DWORD d_len;
		DWORD type;

		std::vector<char> data;
		
		data.resize ( info.maxKeyValueLen + 1 );

		for ( int i = 0 ; i < ( int )info.numKeyValues ; i++ )
		{
			data[0] = 0;
			name[0] = 0;
			n_len   = MAX_VALUE_NAME;
			d_len   = ( DWORD )info.maxKeyValueLen;

         G_SYS_CALL_ONLY ( RegEnumValueA ( r_key , i , name , &n_len , 0 , &type , ( unsigned char* )&( data[0] ) , &d_len ) );

			if ( _g_sys_result != ERROR_SUCCESS )
			{
				key_close ( ( KeyHandle_t )r_key );

				G_EXC_RAISE_WIN();
			}
			else
			{
				KeyValue key_val;

				key_val.name = name;
				key_val.data = m_key_value_data ( 
						key_val.keyType = ( KeyType_t )type , ( unsigned char* )& ( data[0] ), d_len );

				result.push_back ( key_val );
			}
		}

		key_close ( ( KeyHandle_t ) r_key );

		return result;
	}

	KeyValue G_LIB_ITEM get_key_value ( 
		const char* aKeyName , const char* aSubKey , MainKey_t aMainKey )
	{
		char      buffer[1024];
		size_t    b_size = sizeof ( buffer );
		KeyValue  result;   

      G_VERBOSE_MSG_L2 ( "subkey " << aSubKey );

		KeyType_t type = key_none;            
      size_t    size = get_cfg_data ( 
			aKeyName , buffer , b_size , &type , aSubKey , aMainKey );
	
		if ( size != 0 )
		{
			result.keyType = type;
			result.data = m_key_value_data ( type , buffer , size );	
			result.name    = aKeyName;
		}

		return result;
	}

bool G_LIB_ITEM del_key ( const char* aSubKey , MainKey_t aMainKey )
{
	return 
		RegDeleteKeyA ( ( HKEY )aMainKey , aSubKey ) == ERROR_SUCCESS;
}

bool G_LIB_ITEM del_value ( const char* aValueName , const char* aSubKey , MainKey_t aMainKey )
{
	KeyHandle_t key_handle = open_key ( aSubKey , aMainKey , set );

	bool result = ( 
		!key_handle ||
		!RegDeleteValueA ( 
			( HKEY )key_handle , ( const char* )aValueName ) );

	key_flush ( key_handle );
	key_close ( key_handle );

	return result;
}

KeyHandle_t G_LIB_ITEM open_key ( const char* aSubKey , MainKey_t aMainKey , KeyAccessMode_t aAccessFlag )
{
	HKEY h_key = 0;

	if ( 
		RegOpenKeyExA (
			mKeyTable[aMainKey] , 
			( const char* ) aSubKey , 
			0 , 
			mKeyAccessFlagTable[aAccessFlag] , 
			&h_key ) != ERROR_SUCCESS )
	{
		return 0;
	}

	return ( KeyHandle_t )h_key;
}

KeyValue::Data& KeyValue::Data::operator = ( 
	const KeyValue::Data& d )
{
	gstr    = d.gstr;
	multiLine = d.multiLine;
	dword     = d.dword;
	binary    = d.binary;		

	return* this;
}

KeyValue::Data::Data ( const Data& d ) 
{ 
	operator = ( d ); 
}

	/************************************************************************************
		Close a certain HANDLE to a register key.
	*************************************************************************************/
	void key_close ( KeyHandle_t hKey )
	{
      G_EXC_SET_CONTEXT("void g::cfg::key_close ( KeyHandle_t hKey )")

      G_WIN_CALL_ASSERT ( RegCloseKey ( ( HKEY )hKey ) , _g_sys_result == ERROR_SUCCESS );
	}

	/************************************************************************************
		Flush pending data to a certain by HANDLE handled pointed register key.
	*************************************************************************************/
	void key_flush ( KeyHandle_t hKey )
	{
      G_EXC_SET_CONTEXT("void g::cfg::key_flush ( KeyHandle_t hKey )");
		
      G_WIN_CALL_ASSERT ( RegFlushKey ( ( HKEY )hKey ) , _g_sys_result == ERROR_SUCCESS );
	}

}//namespace cfg
}//namespace win
}//namespace g
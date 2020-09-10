#pragma once

#include "g_cont_ref.h"
#include "g_cont_gstr.h"
#include <string>

namespace g
{
namespace signal
{
//!Subclass for defining arbitrary data packet
class QueueDataBufferAbstract 
{
public:
	QueueDataBufferAbstract():mOffset(0){}

	typedef g::cont::ref<QueueDataBufferAbstract> Ref_t;

	//!Copies size_t buffer bytes to THIS buffer from data parameter
	virtual void  write ( const void* , size_t ) = 0;
	//!Copies buffer bytes to destination buffer if it is != NULL otherwise just move the offset
	virtual const void* read  ( void* , size_t ) = 0;
	//!Gets current offsetted pointer
	virtual const void* getCurrentPtr ( ) = 0;
	//!Reset the offset
	void reset ( ) { mOffset = 0; }

protected:
	size_t mOffset;
};

template < class T > QueueDataBufferAbstract& operator << (QueueDataBufferAbstract &out, const T& aT )
{
	out.write ( &aT , sizeof(T) );
	
	return out;
}

template < class T > QueueDataBufferAbstract& operator << (QueueDataBufferAbstract &out, T* aT )
{
	out.write ( aT , sizeof(T) );
	
	return out;
}

inline QueueDataBufferAbstract& operator << (QueueDataBufferAbstract &out, void* aT )
{
	out.write ( aT , sizeof(int*) );
	
	return out;
}

inline QueueDataBufferAbstract& operator << (QueueDataBufferAbstract &out, const char* aStr )
{
	out.write ( aStr , (g::str::get_len ( aStr )+1) * sizeof(char));
	
	return out;
}

inline QueueDataBufferAbstract& operator << (QueueDataBufferAbstract &out, const wchar_t* aStr )
{
	out.write ( aStr , (g::str::get_len ( aStr )+1) * sizeof(wchar_t));
	
	return out;
}

template <class T> QueueDataBufferAbstract& operator << (QueueDataBufferAbstract &out, const g::cont::gstr<T>& aStr )
{
	out.write ( aStr , (aStr.getLength()+1) * sizeof(T));
	
	return out;
}

template <class T> QueueDataBufferAbstract& operator << (QueueDataBufferAbstract &out, const std::basic_string<T>& aStr )
{
	out.write ( aStr , (aStr.size()+1) * sizeof(T));
	
	return out;
}

template < class T > QueueDataBufferAbstract& operator >> (QueueDataBufferAbstract &out, T& aT )
{
	out.read ( &aT , sizeof(T) );
	
	return out;
}

template < class T > QueueDataBufferAbstract& operator >> (QueueDataBufferAbstract &out, T* aT )
{
	out.read ( &aT , sizeof(T) );
	
	return out;
}

inline QueueDataBufferAbstract& operator >> (QueueDataBufferAbstract &out, void* aT )
{
	out.read ( &aT , sizeof(int*) );
	
	return out;
}

inline QueueDataBufferAbstract& operator >> (QueueDataBufferAbstract &out, char* aStr )
{
	str::str_copy<char>( aStr , (const char*)out.read ( NULL , 0 ) );
	
	return out;
}

inline QueueDataBufferAbstract& operator >> (QueueDataBufferAbstract &out, wchar_t* aStr )
{
	str::str_copy<wchar_t>( aStr , (const wchar_t*)out.read ( NULL , 0 ) );
	
	return out;
}

template <class T> QueueDataBufferAbstract& operator >> (QueueDataBufferAbstract &out, g::cont::gstr<T>& aStr )
{
	const T* str_p =  reinterpret_cast<const T*>(out.getCurrentPtr ());

	aStr = g::cont::gstr<T>(str_p);
	
	out.read(NULL,(aStr.getLength()+1)*sizeof(T));

	return out;
}

template <class T> QueueDataBufferAbstract& operator >> (QueueDataBufferAbstract &out, std::basic_string<T>& aStr )
{
	const T* str_p =  reinterpret_cast<const T*>(out.getCurrentPtr ());

	aStr.assign(str_p);

	out.read(NULL,(aStr.size()+1)*sizeof(T));
	
	return out;
}

}//namespace signal
}//namespace g


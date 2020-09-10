#pragma once
                                
#include "g_cont_ref.h"
#include "g_cont_ref_const.h"
#include "g_cont_vect.h"
#include "g_str_lib.h"
#include <iostream>
#include <string>

namespace g
{
namespace cont
{

template < class T > class gstr
{
public:
	gstr ( ) : mBufferP( NULL ) , mLength ( 0 ) { mReceiveRef(empty()); }
	gstr ( const gstr& other ) : mBufferP( NULL ) , mLength ( 0 ) { mReceiveRef ( other ); }
	gstr ( const T* str , AllocationPolicyAbstract* aAllocPolicyP = AllocationPolicyAbstract::get_FromStandardPolicy() ) { mInstanciate ( str , infinite , aAllocPolicyP ); }
	gstr ( const T* str , size_t size , AllocationPolicyAbstract* aAllocPolicyP = AllocationPolicyAbstract::get_FromStandardPolicy() ) { mInstanciate ( str , size , aAllocPolicyP ); }
	gstr ( const std::basic_string<T> str , size_t size = infinite , AllocationPolicyAbstract* aAllocPolicyP = AllocationPolicyAbstract::get_FromStandardPolicy() ) { mInstanciate ( str.c_str() , size , aAllocPolicyP ); }

	virtual ~gstr ( ){}
	
	gstr& operator = ( const gstr& other ) { mReceiveRef ( other ); return *this; }

	//returns len characters from start(count from 0) inside the string, if start > getLen() returns ""
	gstr subset ( int start , size_t = 0xffffffff ) const;
	//returns len characters from left of the string, if the string is shorter than len, returns the whole string
	gstr left   ( size_t len ) const { return subset ( 0 , len ); }
	//returns len characters from right of the string, if the string is shorter than len, returns the whole string
	gstr right  ( size_t len ) const { return subset ( ( int ) ( getLength ( ) - len ) ); }

	bool isLeftLike  ( const gstr& cfr , bool is_case_sens = true ) const { return 0==str::str_compare ( (const T*)cfr , (const T*)left(cfr.getLength()) , is_case_sens ); }  
	bool isRightLike ( const gstr& cfr , bool is_case_sens = true ) const { return 0==str::str_compare ( (const T*)cfr , (const T*)right(cfr.getLength()) , is_case_sens ); }  

	gstr lTrim  ( ) const;
	gstr rTrim  ( ) const;
	gstr trim   ( ) const;

	gstr lCase  ( ) const;
	gstr uCase  ( ) const;

	size_t getLength ( ) const { return mLength; }

	// plot the content onto console 
	void  plot     ( ) const;
	// plot the content onto console and append newline
	void  plotLine ( ) const;

	// Searches str inside the string instance
	// If the search succeeds the returns the offset, otherwise returns the size of the string
	size_t getIndexOf ( const gstr& str , size_t index = 0 , bool is_case_sens = true ) const { return (size_t)index+g::str::str_search ( (const T*)(*this) + index , (const T*)str , is_case_sens ); }
	gstr   replace ( const T* what , const T* with , bool is_case_sens = true ) const {  return gstr(g::str::str_replace<T,gstr<T>,1024>((const T*)(*this),what,with)); }

	// special Char
	static G_LIB_ITEM const gstr& empty    ( );
	static G_LIB_ITEM const gstr& new_line ( );
	static G_LIB_ITEM const gstr& space    ( );
	static G_LIB_ITEM const gstr& tab      ( );

	// returns the pointer to char buffer if exists otherwise
	// returns a pointer to empty gstr ( either "" or L"" )
	operator const T* ( ) const { return mBufferP; }

	gstr<wchar_t> toWideStr ( ) const;
	gstr<char>    toByteStr ( ) const;

	gstr append ( const T* s ) const;
	gstr append ( const T  c ) const;

	T operator [] ( int i ) const { return *(mBufferP+i); }

	gstr& operator += ( const T* s )   { *this = append ( s ); return *this; }
	gstr& operator += ( const char c ) { *this = append ( c ); return *this; }

	static gstr sum ( const T* s1 , const T* s2 , AllocationPolicyAbstract* aAllocPolicyP = AllocationPolicyAbstract::get_FromStandardPolicy());

	HeapAbstract*             getHeapP() const { return mInternalBufferRef.getHeapP ( );  }
	AllocationPolicyAbstract* getAllocPolicyP() const { return mInternalBufferRef.getAllocPolicyP ( );  }

private:
	const T* mBufferP;
	size_t   mLength;

	typedef typename vect<T>::RefConst_t InternalBufferRefConst_t;
	typedef typename vect<T>::Ref        InternalBufferRef_t;

	InternalBufferRefConst_t mInternalBufferRef;

	InternalBufferRef_t mCreateBuffer ( size_t aStrLen , AllocationPolicyAbstract* aAllocPolicyP )
	{
		InternalBufferRef_t int_buffer_ref = InternalBufferRef_t::g_cont_new(aAllocPolicyP);

		mLength = aStrLen;
		int_buffer_ref->reSize ( mLength + 1 );
		mBufferP = int_buffer_ref->operator const T*();
		mInternalBufferRef = int_buffer_ref;

		return int_buffer_ref;
	}

	void mInstanciate ( const T* aData , size_t aStrLen , AllocationPolicyAbstract* aAllocPolicyP )
	{
		size_t i;

		for ( i = 0 ; aData[i] != 0x0 && i < aStrLen ; i++ );

		aStrLen = i;

		InternalBufferRef_t int_buffer_ref = mCreateBuffer ( aStrLen , aAllocPolicyP );

		for ( i = 0 ; i < aStrLen ; i++ )
		{
			int_buffer_ref->operator []((int)i) = aData[i];
		}

		int_buffer_ref->operator []((int)i) = 0x0;
	}

	void mReceiveRef ( const gstr& aOther )
	{
		mBufferP           = aOther.mBufferP;
		mLength            = aOther.mLength;
		mInternalBufferRef = aOther.mInternalBufferRef;
	}
};

typedef gstr< char >    Gstr;
typedef gstr< wchar_t > Wstr;

// Sum Policies 

// operator + ( gstr , gstr )
template < class T > gstr<T>
	operator + ( const gstr<T>& s1 , const gstr<T>& s2 ){ return gstr<T> ( gstr<T>::sum(s1,s2) ); }

// operator + ( gstr , const T* )
template < class T > gstr<T>
	operator + ( const T* s1 , const gstr<T>& s2 ){ return gstr<T> ( gstr<T>::sum(s1,s2) ); }
// operator + ( const T* , gstr )
template < class T > gstr<T>
	operator + ( const gstr<T>& s1 , const T* s2 ){ return gstr<T> ( gstr<T>::sum(s1,s2) ); }

//The presence of const char*() operators forces to define the all of comparition operators
template < class T > bool operator == ( const gstr<T>& s1 , const gstr<T>& s2 ) { return str::str_equal<T> ( s1 , s2 ); }
template < class T > bool operator == ( const gstr<T>& s1 , const T* s2 )       { return str::str_equal<T> ( ( const T* ) s1 , s2 ); }
template < class T > bool operator == ( const T* s1 , const gstr<T>& s2 )       { return str::str_equal<T> ( s1 , ( const T* )s2 ); }

template < class T > bool operator != ( const gstr<T>& s1 , const gstr<T>& s2 ) { return !str::str_equal<T> ( s1 , s2 ); }
template < class T > bool operator != ( const gstr<T>& s1 , const T* s2 )       { return !str::str_equal<T> ( ( const T* ) s1 , s2 ); }
template < class T > bool operator != ( const T* s1 , const gstr<T>& s2 )       { return !str::str_equal<T> ( s1 , ( const T* )s2 ); }

template < class T > bool operator > ( const gstr<T>& s1 , const gstr<T>& s2 ) { return str::str_compare<T> ( s1 , s2 ) > 0 ; }
template < class T > bool operator > ( const gstr<T>& s1 , const T* s2 )       { return str::str_compare<T> ( ( const T* ) s1 , s2 ) > 0; }
template < class T > bool operator > ( const T* s1 , const gstr<T>& s2 )       { return str::str_compare<T> ( s1 , ( const T* )s2 ) > 0; }

template < class T > bool operator < ( const gstr<T>& s1 , const gstr<T>& s2 ) { return str::str_compare<T> ( s1 , s2 ) < 0 ; }
template < class T > bool operator < ( const gstr<T>& s1 , const T* s2 )       { return str::str_compare<T> ( ( const T* ) s1 , s2 ) < 0; }
template < class T > bool operator < ( const T* s1 , const gstr<T>& s2 )       { return str::str_compare<T> ( s1 , ( const T* )s2 ) < 0; }

template < class T > bool operator >= ( const gstr<T>& s1 , const gstr<T>& s2 ) { return str::str_compare<T> ( s1 , s2 ) >= 0 ; }
template < class T > bool operator >= ( const gstr<T>& s1 , const T* s2 )       { return str::str_compare<T> ( ( const T* ) s1 , s2 ) >= 0; }
template < class T > bool operator >= ( const T* s1 , const gstr<T>& s2 )       { return str::str_compare<T> ( s1 , ( const T* )s2 ) >= 0; }

template < class T > bool operator <= ( const gstr<T>& s1 , const gstr<T>& s2 ) { return str::str_compare<T> ( s1 , s2 ) <= 0 ; }
template < class T > bool operator <= ( const gstr<T>& s1 , const T* s2 )       { return str::str_compare<T> ( ( const T* ) s1 , s2 ) <= 0; }
template < class T > bool operator <= ( const T* s1 , const gstr<T>& s2 )       { return str::str_compare<T> ( s1 , ( const T* )s2 ) <= 0; }


template < class T > gstr<T> gstr<T>::append ( const T aChar ) const
{
	T temp[2];

	temp[0] = aChar;
	temp[1] = 0;
	
	return append (temp);
}

template < class T > gstr<T> gstr<T>::append ( const T* aStr ) const
{
	gstr result;

	InternalBufferRef_t int_buffer_ref = result.mCreateBuffer ( getLength() + str::get_len(aStr), AllocationPolicyAbstract::get_FromStandardPolicy() );

	T* result_buffer = &( int_buffer_ref->operator [](0) );

	str::str_copy<T> ( result_buffer , mBufferP );
	str::str_copy<T> ( result_buffer + getLength() , aStr );

	return result;
}

template < class T > gstr<T> gstr<T>::subset ( int aStart , size_t aLen ) const
{
	if ( aLen == 0 || aStart >= (int)getLength() )
	{
		return empty();
	}
	else 
	{
		size_t len = ( aLen - aStart < aLen)?(aLen - aStart ):aLen;

		return gstr<T>(operator const T* () + aStart , len );
	}
}

template < class T > gstr<T> gstr<T>::lTrim ( ) const
{
	size_t start;

	for ( 
		start = 0 ; 
		start < getLength ( ) && ( *this )[start] == *space ( ) ; 
		start++ );

	return gstr<T>( mBufferP + start , getLength ( ) - start );
}

template < class T > gstr<T> gstr<T>::rTrim ( ) const
{
	gstr<T> result;

	size_t end;

	for ( 
		end = getLength ( ) - 1 ; 
		end >= 0 && ( *this )[end] == *space ( ) ; 
		end-- );

	return gstr<T>( mBufferP , end + 1 );
}

template < class T > gstr<T> gstr<T>::trim ( ) const
{
	size_t start;
	size_t end;

	for ( 
		start = 0 ; 
		start < getLength ( ) && ( *this )[(int)start] == *space ( ) ; 
		start++ );

	for ( 
		end = getLength ( ) - 1 ; 
		end >= 0 && ( *this )[(int)end] == *space ( ) ; 
		end-- );

	return gstr<T>( mBufferP + (int)start , (size_t)( end - start + 1 ) , AllocationPolicyAbstract::get_FromStandardPolicy() );
}

// plot the content onto console and append newline
template < class T > void gstr<T>::plotLine ( ) const
{
	plot ( );
	new_line ( ).plot( );
}

template < class T > gstr <T> gstr<T>::lCase ( ) const
{
	gstr result;

	InternalBufferRef_t int_buffer_ref = result.mCreateBuffer ( getLength() , AllocationPolicyAbstract::get_FromStandardPolicy() );

	T* result_buffer = &( int_buffer_ref->operator [](0) );

	size_t i;

	for ( i = 0 ; i < getLength ( ) ; i++ )
	{
		result_buffer[i] = car::l_case < T >( (*this)[(int)i] );
	}
	
	result_buffer[i] = 0x0;

	return result;
}

template < class T > gstr<T> gstr<T>::uCase ( ) const
{
	gstr result;

	InternalBufferRef_t int_buffer_ref = result.mCreateBuffer ( getLength() , AllocationPolicyAbstract::get_FromStandardPolicy() );

	T* result_buffer = &( int_buffer_ref->operator [](0) );

	size_t i;

	for ( i = 0 ; i < getLength ( ) ; i++ )
	{
		result_buffer[i] = car::u_case < T >( (*this)[(int)i] );
	}
	
	result_buffer[i] = 0x0;

	return result;
}

template < class T > gstr<T> gstr<T>::sum ( const T* aS1 , const T* aS2 , AllocationPolicyAbstract* aAllocPolicyP )
{
	size_t      len_1 = str::get_len(aS1);
	size_t      len_2 = str::get_len(aS2);

	gstr result;

	InternalBufferRef_t int_buffer_ref = result.mCreateBuffer ( len_1 + len_2 , aAllocPolicyP );

	T* result_buffer = &( int_buffer_ref->operator [](0) );

	str::str_copy<T> ( result_buffer , aS1 );
	str::str_copy<T> ( result_buffer + len_1 , aS2 );
	
	return result;
}

template <> inline gstr<wchar_t> gstr<wchar_t>::toWideStr ( ) const { return Wstr ( *this ); }

template <> inline gstr<wchar_t> gstr<char>::toWideStr ( ) const
{
	vect<wchar_t> temp(AllocationPolicyAbstract::get_FromStandardPolicy());

	temp.reSize(getLength());

	int i;

	for ( i = 0 ; i <= (int)getLength ( ) ; i++ )
	{
		temp[i] = (unsigned char)( ( *this )[i] );
	}

	return gstr<wchar_t>((const wchar_t*)temp,getLength(),AllocationPolicyAbstract::get_FromStandardPolicy());
}

template <> inline gstr<char> gstr<char>::toByteStr ( ) const { return Gstr ( *this ); }

template <> inline gstr<char> gstr<wchar_t>::toByteStr ( ) const
{
	vect<char> temp(AllocationPolicyAbstract::get_FromStandardPolicy());

	temp.reSize(getLength()+1);

	int i;

	for ( i = 0 ; i <= (int)getLength ( ) ; i++ )
	{
		temp[i] = static_cast<char>(0xff & this->operator []((int)i));
	}

	return gstr<char>((const char*)temp,getLength(),AllocationPolicyAbstract::get_FromStandardPolicy());
}

template < class T > std::basic_ostream<T>& operator<< (std::basic_ostream<T> &out, const gstr<T>& aGstr )
{
	out << (const T*) aGstr;

	return out;
}

template < class T > std::basic_istream<T>& operator>> (std::basic_istream<T> &in, gstr<T>& aGstr )
{
	const int buffer_size = 1024;
	T temp[buffer_size];

	in >> temp;

	aGstr = temp;

	return in;
}

}// namespace cont
}// namespace g

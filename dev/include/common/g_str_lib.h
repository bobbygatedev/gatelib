/** @file */
#pragma once
#include "g_car_base.h"
#include <stdio.h>

#ifndef size_t 
typedef unsigned int size_t;
#endif

namespace g
{
static const size_t infinite = (size_t)-1;
namespace str
{

/*! 
\defgroup g_str_lib g_str_lib
\see g_str_lib.h
*  @{
*/

//! Constant equal to 0, says to allocate a new buffer for destination.
static const int alloc_new = 0;

/*! 
\brief converts a byte gstr ( const char* ) to a wide one ( wchar_t* ).

if len = 0 returns null\n
if dst = alloc_new ( 0 ) then it allocates the needed chars*/
wchar_t* to_widestr( const char* src , wchar_t* dst = (wchar_t*)alloc_new );


/*! 
\brief converts a wide gstr ( const wchar_t* ) to a wide one ( char* )

if len = 0 returns null.\n
if dst = alloc_new ( 0 ) then it allocates the needed chars.*/
char* to_charstr( const wchar_t* src , char* dst = (char*)alloc_new );

//!Returns the string length.
template<class T> inline size_t get_len( const T* aString )
{
	if (aString != NULL)
	{
		size_t len;

		for (len = 0; aString[len] != 0; len++);

		return len;
	}
	else
	{
		return 0;
	}
}

/*!
\brief Copies from aSrc to aDest up to a maximum of aNumChars characters (excluding terminator)
\returns the number of copied characters (excluding terminator)*/
template <class T> inline size_t str_copy( T* aDestP , const T* aSrcP , size_t aNumChars = infinite )
{
	size_t i;

	if (aSrcP != NULL)
	{
		for ( i = 0; aSrcP[i] != 0 && i < aNumChars; i++)
		{
			aDestP[i] = aSrcP[i];
		}

		aDestP[i] = 0;
		return i;
	}
	else
	{
		aDestP[0] = 0;	// = ""
		return 0;
	}
}


/*! 
\brief converts a byte gstr ( const char* ) to a wide one ( wchar_t* )

if len = 0 returns null\n
if dst = alloc_new ( 0 ) then it allocates the needed chars*/
inline wchar_t* to_widestr( const char* aSrcP , wchar_t* aDstP )
{
	size_t len = get_len(aSrcP);

	if (!len)
		return 0;

	wchar_t* dst_p = (aDstP != alloc_new) ? aDstP : new wchar_t[len + 1];

	for (size_t i = 0; i <= len; i++)
	{
		dst_p[i] = *((unsigned char*)aSrcP + i);
	}

	return dst_p;
}

/*! 
\brief converts a wide gstr ( const wchar_t* ) to an ANSI one ( char* ).

if len = 0 returns null.\n
if dst = alloc_new ( 0 ) then it allocates the needed chars*/
inline char* to_charstr( const wchar_t* aSrcP , char* aDstP )
{
	size_t len = get_len(aSrcP);

	if (!len)
		return 0;

	char* dst_p = (aDstP != alloc_new) ? aDstP : new char[len + 1];

	for (size_t i = 0; i <= len; i++)
	{
		dst_p[i] = static_cast<char>(aSrcP[i] & 0xff);
	}

	return dst_p;
}

/*!
\brief Compares two strings. 
\param aS1         first string to be compared
\param aS2         second string to be compared
\param aIsCaseSens comparition to be case sensitive or not?
\param aNumChars   num of chars to be compared.

\tparam T a character type(char,wchar_t)

\return
<ul>
<li>-1 aC1 < aC2 (alphabetically order)
<li>0  aC1 = aC2 
<li>+1 aC1 > aC2 (alphabetically order)
</ul>
*/
template<class T> inline int str_compare( const T* aS1 , const T* aS2 , bool aIsCaseSens = true , size_t aNumChars = infinite )
{
	if ( aS1 == NULL || *aS1 == 0 )
	{
		return ( aS2 == NULL || *aS2 == 0 )?0:-1;
	}
	else if ( aS2 == NULL || *aS2 == 0 )
	{
		return +1;
	}
	else
	{
		size_t i;

		for (i = 0; aS1[i] != 0 && aS2[i] != 0 && (size_t)i < aNumChars; i++)
		{
			switch ( car::chr_compare (aS1[i], aS2[i], aIsCaseSens) )
			{
				case 1: return 1;
				case -1: return -1;
			}
		}

		if (i == aNumChars)
		{
			//in this case I've reached the character limit ( aNumChars )
			return 0;
		}
		else if (aS1[i] == aS2[i])
		{
			//equal if the two characters are the terminator(0)
			return 0;
		}
		else if ( aS1[i]==0 )
		{
			return -1; //aS1 is shorter in this case 
		}
		else 
		{
			return +1; //aS2 is shorter in this case 
		}
	}
}

/*!
\brief Check two strings for equality. 
\param aS1         first string to be compared
\param aS2         second string to be compared
\param aIsCaseSens comparition to be case sensitive or not?
\param aNumChars   num of chars to be compared.

\tparam T a character type(char,wchar_t)
*/
template<class T> inline bool str_equal( const T* aS1 , const T* aS2 , bool aIsCaseSens = true , size_t aNumChars = infinite )
{
	if ( ( aS1 == NULL && aS2 == NULL ) || ( aS1 == NULL && *aS2 == 0 ) || ( aS2 == NULL && *aS1 == 0 ) )
	{
		return true;
	}
	else
	{
		size_t i;

		for (i = 0; aS1[i] != 0 && aS2[i] != 0 && (size_t)i < aNumChars; i++)
		{
			if (!car::chr_equal(aS1[i], aS2[i], aIsCaseSens))
			{
				return false;
			}
		}

		if (i == aNumChars)
		{
			//in this case I've reached the character limit ( aNumChars )
			return true;
		}
		else
		{
			//equal if the two characters are the terminator(0)
			return (aS1[i] == aS2[i]); // = 0
		}
	}
}

/*!
\brief Searches aS2 inside aS1. 
\param aS1         first string to be compared
\param aS2         second string to be compared
\param aIsCaseSens comparition to be case sensitive or not?
\param aNumChars   num of chars to be compared.

\tparam T a character type(char,wchar_t)

Searches aS2 inside aS1 until an offset of aNumChars from aS1.\n
If the search succeeds the returns the offset, otherwise returns the size of aS1 or aNumChars, if it is < of size(aS1)
*/
template<class T> inline size_t str_search( const T* aS1 , const T* aS2 , bool aIsCaseSens = true , size_t aNumChars = infinite )
{
	size_t i;

	for ( i = 0 ; aS1[i] != 0 && i < aNumChars ; i++ )
	{
		bool found = true;

		for ( size_t j = 0 ; aS2[j] != 0 ; j++ )
		{
			if ( !car::chr_equal(aS1[i+j], aS2[j], aIsCaseSens) )
			{
				found = false;
				break;
			}
		}

		if ( found )
		{
			return i;
		}
	}

	return i;
}
 
/*!
\brief Replace aWhat with aWith inside aInput, the result is stored into aOutputBuffer, whose capacity us aNumChars.

\tparam T a character type(char,wchar_t)

\param aString:      input string (a char buffer)
\param aWhat:        string to be searched 
\param aWith:        string replacing aWhat occurences
\param aOutputBuffer char buffer with a capacity of aNumChars + 1 (stores the terminator too)
\param aNumChars:    capacity of aOutputBuffer (excluding the terminator)*/
template < class T >
	inline size_t str_replace ( const T* aString , const T* aWhat , const T* aWith , T* aOutputBuffer , size_t aNumChars = infinite )
{
	size_t num_read_chars         = 0;
	size_t what_len               = g::str::get_len(aWhat);
	size_t with_len               = g::str::get_len(aWith);
	size_t remaining_buffer_chars = aNumChars;

	if ( what_len == 0 )
	{
		return str_copy(aOutputBuffer,aString,aNumChars);
	}

	for ( ; *aString != 0 && remaining_buffer_chars > 0 ; )
	{
		size_t offset = str_search ( aString , aWhat , true );

		if ( aString[offset] != 0 )
		{
			size_t temp_copied = str_copy(aOutputBuffer,aString,(remaining_buffer_chars>offset)?offset:remaining_buffer_chars);
			
			remaining_buffer_chars -= temp_copied;
			num_read_chars += temp_copied;
			aString += temp_copied;//moving in buffer
			aOutputBuffer += temp_copied;//moving outbuffer

			if ( remaining_buffer_chars >= with_len )
			{
				num_read_chars += what_len;
				aString += what_len;//moving in buffer

				//copy replacing word to output
				temp_copied = str_copy(aOutputBuffer,aWith,(remaining_buffer_chars>with_len)?with_len:remaining_buffer_chars);

				//update counters again
				remaining_buffer_chars -= temp_copied;
				aOutputBuffer += temp_copied;//moving outbuffer
			}
			else
			{
				break;
			}
		}
		else
		{
			//str_search fails: fill until end of string
			size_t temp_copied = str_copy(aOutputBuffer,aString,remaining_buffer_chars);

			num_read_chars += temp_copied;
			remaining_buffer_chars -= temp_copied;
			aString += temp_copied;//moving in buffer
			aOutputBuffer += temp_copied;//moving outbuffer
		}
	}

	*aOutputBuffer = 0;

	return num_read_chars;
}

/*!
\brief Replace aWhat with aWith inside aInput and returns the result as a class string (a std::string , std::wstring , g::cont::g_str<T>)

\tparam T char type (char,wchar_t)
\tparam R return type (a structured, not necessarly emplate type ) where the operator += ( const T* ) must be defined.
\tparam S, size in chars (excluding the terminator), therefore temp buffer has a capacity  of S+1.

\param aString:      input string (a char buffer)
\param aWhat:        string to be searched 
\param aWith:        string replacing aWhat occurences
*/
template < class T , class R , size_t S > inline R str_replace ( const T* aInput , const T* aWhat , const T* aWith )
{
	//const int S = 1024;
	T temp[S+1];
	R result;

	while ( true )
	{
		size_t how_many = g::str::str_replace ( aInput , aWhat , aWith , temp , S );

		result += temp;

		if ( aInput[how_many] == 0 )//i'm at the end of the string
		{
			return result;
		}
		else
		{
			aInput += how_many;
		}
	}
}

/*! @} */

}// namespace str
}// namespace g

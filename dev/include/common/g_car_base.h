/** @file */
#pragma once

#ifndef size_t 
	typedef unsigned int size_t;
#endif

namespace g
{
namespace car
{

/*! 
\defgroup g_car_base g_car_base
\see g_car_base.h
*  @{
*/

//! returns aStart >= aChar >= aStop.
template < class T > bool is_between ( T aChar , T aStart , T aStop );

//! returns true if char is a digit.
template < class T > bool is_char_a_digit  ( T aChar ) { return is_between <T> ( aChar , '0' , '9' ); }

//! returns true if char is a upper case letter.
template < class T >	bool is_char_ucase    ( T aChar ) { return is_between <T> ( aChar , 'A' , 'Z' ); }

//! returns true if char is a lower case letter.
template < class T > bool is_char_lcase    ( T aChar ) { return is_between <T> ( aChar , 'a' , 'z' ); }

//! returns true if char is a letter.
template < class T > bool is_char_a_letter ( T aChar ) { return is_char_lcase ( aChar ) || is_char_ucase ( aChar ); };

//!returns the corresponding upper case char.
template < class T > T  u_case ( T aChar ) { return ( is_char_lcase ( aChar ) )? ( aChar - 'a' + 'A' ) : aChar; }

//!returns the corresponding lower case char.
template < class T >	T  l_case ( T aChar ) { return ( is_char_ucase ( aChar ) )? ( aChar - 'A' + 'a' ) : aChar; }

//template < class T > bool str_equal ( T  , T , bool case_sens = true );

template <> inline bool is_between ( char aChar , char aStart , char aStop )
{
	return ( aChar >= aStart && aChar <= aStop );
}

template <> inline bool is_between ( wchar_t aChar , wchar_t aStart , wchar_t aStop )
{
	return is_between <char>( 
		(char)(0xff & aChar) , (char)(0xff & aStart) , (char)(0xff & aStop) );
}

template < class T > bool chr_equal ( T aC1 , T aC2 , bool aIsCaseSensitive )
{
	return ( aIsCaseSensitive ) ?
		( aC1 == aC2 ) : ( u_case ( aC1 ) == u_case ( aC2 ) );
}

template < class T> GUint64_t get_chr_ordinal_code ( T aChar )
{
	if ( is_char_a_digit ( aChar ) )
	{
		return (GUint64_t)( aChar - (T)'0' );
	}
	else if ( is_char_lcase ( aChar ) )
	{
		return (GUint64_t)( aChar - (T)'a' ) | (GUint64_t)0xf0;
	}
	else if ( is_char_ucase ( aChar ) )
	{
		return (GUint64_t)( aChar - (T)'A' ) | (GUint64_t)0xf00;
	}
	else 
	{
		return (GUint64_t)( aChar ) | (GUint64_t)0xf000;
	}
}

/*!
\brief Compares two chars.
\return 
<ul>
<li>-1 aC1 < aC2 
<li>0  aC1 = aC2 
<li>+1 aC1 > aC2 
</ul>
*/
template < class T > int chr_compare ( T aC1 , T aC2 , bool aIsCaseSensitive )
{
	if ( !aIsCaseSensitive )
	{
		aC1 = l_case ( aC1 );
		aC2 = l_case ( aC2 );
	}

	GUint64_t ordinal_code_1 = get_chr_ordinal_code ( aC1 );
	GUint64_t ordinal_code_2 = get_chr_ordinal_code ( aC2 );

	if ( ordinal_code_1 < ordinal_code_2 )
	{
		return -1;
	}
	if ( ordinal_code_1 > ordinal_code_2 )
	{
		return +1;
	}
	else
	{
		return 0;
	}
}

/*! @} */

}// namespace car
}// namespace g

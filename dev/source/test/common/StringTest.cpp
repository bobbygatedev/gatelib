/*
 * StringTest.cpp
 *
 *  Created on: 30/gen/2014
 *      Author: Bobby Gate
 */

#include "StringTest.h"

using namespace g;
using namespace cont;

StringTest::StringTest() {}

StringTest::~StringTest() {}


bool StringTest::mExecute()
{
	{
		Gstr the_str = Gstr("ciao miao",4);
		
		mAddSubResult ( "gstr::gstr() test" , "" ,  ( "" == Gstr() ) ); 
		mAddSubResult ( "gstr ( const gstr& ) test" , "" ,  ( "ciao" == Gstr(the_str) ) ); 
		mAddSubResult ( "gstr ( const T* str,.. test" , "" ,  ( "ciao" == Gstr("ciao") ) ); 
		mAddSubResult ( "gstr ( const T* str, size_t .. test" , "" ,  ( "ciao" == Gstr("ciao  " , 4) ) ); 
		mAddSubResult ( "gstr ( const std::basic_string<T> , size_t size .. test" , "" ,  ( "ci" == std::string(the_str , 2) ) ); 
		mAddSubResult ( "Append string test" , "" , ( "ciao bau" == the_str.append ( " bau" ) ) ); 
		mAddSubResult ( "Append char test" , "" , ( "ciao " == the_str.append ( ' ' ) ) ); 
		mAddSubResult ( "gstr::sum test" , "" , ( "tau bee" == the_str.sum ( "tau " , "bee" ) ) ); 
		mAddSubResult ( "lCase test" , "" , ( "ciao" ==  Gstr("CIAO").lCase() ) ); 
		mAddSubResult ( "uCase test" , "" , ( "CIAO" ==  Gstr("ciao").uCase() )  ); 
		mAddSubResult ( "left test" , "" , ( "ci" ==  Gstr("ciao").left(2) ) ); 
		mAddSubResult ( "right test" , "" , ( "ao" ==  Gstr("ciao").right(2) )  ); 
		mAddSubResult ( "Left like (case sens)" , "" , Gstr("ciao").isLeftLike("ci") ); 
		mAddSubResult ( "Left like (case insens)" , "" , Gstr("ciao").isLeftLike("CI",false) ); 
		mAddSubResult ( "Right like (case sens)" , "" , Gstr("ciao").isRightLike("ao") ); 
		mAddSubResult ( "Right like (case insens)" , "" , Gstr("ciao").isRightLike("AO",false) ); 
		mAddSubResult ( "Left not like (case sens)" , "" , !Gstr("ciao").isLeftLike("cio") ); 
		mAddSubResult ( "Left not like (case insens)" , "" , !Gstr("ciao").isLeftLike("CIO",false) ); 
		mAddSubResult ( "Right not like (case sens)" , "" , !Gstr("ciao").isRightLike("aoa") ); 
		mAddSubResult ( "Right not like (case insens)" , "" , !Gstr("ciao").isRightLike("AOa",false) ); 
	}

	mPerformHeapWalkCheck();

	return true;
}


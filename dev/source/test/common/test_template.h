/*
 * test_template.h
 *
 *  Created on: 04/feb/2014
 *      Author: Bobby Gate
 */

#pragma once

#include <g_cont_alg.h>
#include <g_cont_gstr.h>
#include <g_common_functions.h>
#include <sstream>

using namespace g;
using namespace cont;
using namespace std;
using namespace alg;


#define TEST_FRAME_END(fname) \
}\
catch (ContException& e) { cout << "Error in " # fname ":" << endl << e.getErrorMsg() << endl; return false; }\
catch(...) { Exception::generic_ExceptionCancelHandling();cout << "Unknown exception in " # fname << endl; return false;}

template<class R> class test_template
{
public:
static void fill_recipient ( Gstr aValues , R& aRecipient )
{
	istringstream io_stream((const char*)aValues);

	aRecipient.setEmpty();

	unsigned int tell;

	do
	{
		int value;

		tell = io_stream.tellg();

		if (tell < aValues.getLength())
		{
			io_stream >> value;
			aRecipient.pushTail(value);
		}
	}while(tell < aValues.getLength());
}

static bool check_recipient ( const R& aLst , Gstr aModel )
{
	R cfr;

	fill_recipient(aModel,cfr);

	if ( aLst == cfr )
	{
		return true;
	}
	else
	{
		cout << "Error: " << aModel << " was expected, but I got: " << endl;
		cout << get_cont_string<R,char>(aLst) << endl;

		return false;
	}
}

static bool push_after_test()
{
	G_EXC_FRAME_BEGIN()
	R the_rec;
	R model;

	fill_recipient ( "1 2" , model );

	the_rec.setContent(model);

	typename R::It_t it;

	it = the_rec.getIterator (head);
	the_rec.pushAfter(10,it);

	if ( !check_recipient(the_rec,"1 10 2") )
	{
		cout << "Failure on inserting 10 in {1,2} after 1" << endl;
		return false;
	}

	the_rec.setContent(model);

	it = the_rec.getIterator (head);
	it+=1;//moving iterator to 1

	the_rec.pushAfter(10,it);

	if ( !check_recipient(the_rec,"1 2 10") )
	{
		cout << "Failure on inserting 10 in {1,2} after 2" << endl;
		return false;
	}
	TEST_FRAME_END(push_after_test())

	return true;
}

static bool push_before_test()
{
	G_EXC_FRAME_BEGIN()

	R the_rec;
	R model;

	fill_recipient ( "1 2" , model );

	the_rec.setContent(model);

	typename R::It_t it;

	it = the_rec.getIterator (head);
	the_rec.pushBefore(10,it);

	if ( !check_recipient(the_rec,"10 1 2") )
	{
		cout << "Failure on inserting 10 in {1,2} after 1" << endl;
		return false;
	}

	the_rec.setContent(model);

	it = the_rec.getIterator (head);
	it+=1;//moving iterator to 1

	the_rec.pushBefore(10,it);

	if ( !check_recipient(the_rec,"1 10 2") )
	{
		cout << "Failure on inserting 10 in {1,2} after 2" << endl;
		return false;
	}

	TEST_FRAME_END(push_before_test())

	return true;
}

static bool set_content_test ( )
{
	G_EXC_FRAME_BEGIN()

	int load_vector[]={1,2,3,4,5,6,7,8,9,10};

	R model(load_vector,sizeof(load_vector)/sizeof(int));

	R r1;

	r1.setContent ( model );

	return check_recipient ( r1 , "1 2 3 4 5 6 7 8 9 10" );

	TEST_FRAME_END(set_content_test())
}

static bool push_pop_tail_test ( )
{
	G_EXC_FRAME_BEGIN()

	R the_rec;

	the_rec.pushTail (1);

	if ( !check_recipient(the_rec,"1") )
	{
		cout << "Failed push 1" << endl;
		return false;
	}

	the_rec.pushTail (2);

	if ( !check_recipient(the_rec,"1 2") )
	{
		cout << "Failed push 2" << endl;
		return false;
	}

	the_rec.pushTail (3);

	if ( !check_recipient(the_rec,"1 2 3") )
	{
		cout << "Failed push 3" << endl;
		return false;
	}

	the_rec.popTail ();

	if ( !check_recipient(the_rec,"1 2") )
	{
		cout << "Failed popTail(){1}" << endl;
		return false;
	}

	the_rec.popTail ();

	if ( !check_recipient(the_rec,"1") )
	{
		cout << "Failed popTail(){2}" << endl;
		return false;
	}

	the_rec.popTail ();

	if ( !check_recipient(the_rec,"") )
	{
		cout << "Failed popTail(){3}" << endl;
		return false;
	}

	try
	{
		the_rec.popTail();

		cout << "Exception not raised!" << endl;

		return false;
	}
	catch (ContException&)	{}

	return true;

	TEST_FRAME_END(push_pop_tail_test ( ))
}

static bool push_pop_head_test ( )
{
	G_EXC_FRAME_BEGIN()

	R the_rec;

	the_rec.pushHead (1);

	if ( !check_recipient(the_rec,"1") )
	{
		cout << "Failed push 1" << endl;
		return false;
	}

	the_rec.pushHead (2);

	if ( !check_recipient(the_rec,"2 1") )
	{
		cout << "Failed push 2" << endl;
		return false;
	}

	the_rec.pushHead (3);

	if ( !check_recipient(the_rec,"3 2 1") )
	{
		cout << "Failed push 3" << endl;
		return false;
	}

	cout << "Before popping .. " << endl;

	the_rec.popHead ();

	if ( !check_recipient(the_rec,"2 1") )
	{
		cout << "Failed popHead(){1}" << endl;
		return false;
	}

	the_rec.popHead ();

	if ( !check_recipient(the_rec,"1") )
	{
		cout << "Failed popHead(){2}" << endl;
		return false;
	}

	the_rec.popHead ();

	if ( !check_recipient(the_rec,"") )
	{
		cout << "Failed popHead(){3}" << endl;
		return false;
	}

	try
	{
		the_rec.popHead();

		cout << "Exception not raised!" << endl;

		return false;
	}
	catch (ContException& )	{}

	TEST_FRAME_END(push_before_test())

	return true;
}

bool search_remove_test ( )
{
	G_EXC_FRAME_BEGIN()

	R the_rec;

	fill_recipient("1 2 3 4 5", the_rec);

	{
		int value = 3;

		if(!the_rec.tryRemove(value))
		{
			cout << "Fail to search " << value << endl;
			return false;
		}

		if ( !check_recipient(the_rec,"1 2 4 5") )
		{
			return false;
		}
	}

	{
		int value = 4;

		if(!the_rec.tryRemove(value))
		{
			cout << "Fail to search " << value << endl;
			return false;
		}

		if ( !check_recipient(the_rec,"1 2 5") )
		{
			return false;
		}
	}

	{
		int value = 5;

		if(!the_rec.tryRemove(value))
		{
			cout << "Fail to search " << value << endl;
			return false;
		}

		if ( !check_recipient(the_rec,"1 2") )
		{
			return false;
		}
	}

	{
		int value = 1;

		if(!the_rec.tryRemove(value))
		{
			cout << "Fail to search " << value << endl;
			return false;
		}

		if ( !check_recipient(the_rec,"2") )
		{
			return false;
		}
	}

	{
		int value = 2;

		if(!the_rec.tryRemove(value))
		{
			cout << "Fail to search " << value << endl;
			return false;
		}

		if ( !check_recipient(the_rec,"") )
		{
			return false;
		}
	}

	return true;

	TEST_FRAME_END(push_pop_head_test ( ))
}

};

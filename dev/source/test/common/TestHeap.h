#pragma once

#include "g_cont_HeapAbstract.h"
#include <iostream>

#define HEAP_WORDS         (128*1024)
#ifdef WIN32
#define HEAP_DUMP_LOCATION "c:\\temp\\heapmemdump.txt"
#else
#define HEAP_DUMP_LOCATION "./heapmemdump.txt"
#endif

struct TestHeapWalkResult
{
	TestHeapWalkResult()
	{
		objectNumber = 0;
		byteUsed = 0;
		isOk = true;
	}
	
	size_t  objectNumber;
	size_t  byteUsed;
	bool isOk;
};

using namespace std;

template < class T > std::basic_ostream<T>& operator<< (std::basic_ostream<T> &out, const TestHeapWalkResult& aWalkResult )
{
	out << 
		"Ok:         " << aWalkResult.isOk << endl <<
		"Byte Used:  " << aWalkResult.byteUsed << endl <<
		"Num object: " << aWalkResult.objectNumber << endl;

	return out;
}


class TestHeap : public g::cont::HeapAbstract
{
public:
	TestHeap(void);
	virtual ~TestHeap(void);

	//searches for a location with a certain amount of data available without allocating it
	virtual void*  searchLocation  ( size_t );
	virtual void*  reserveScalar   ( size_t );
	virtual void   unreserve       ( void* );
	virtual size_t getLocationSize ( void* );

	virtual void* reserveArray ( size_t data_capacity , size_t suggested_capacity = EQUAL_TO_CAPACITY );
	virtual void* reallocArray ( void* , size_t current_data_size , size_t data_capacity = EQUAL_TO_CURRENT , size_t suggested_data_capacity = EQUAL_TO_CAPACITY );

	virtual TestHeapWalkResult doWalk ( ) const;
	virtual void doWalkCheck ( ) const;
	virtual void doHeapDump ( const char* =HEAP_DUMP_LOCATION ) const;
	virtual void doHeapDump ( std::basic_ostream<char>& ) const;

	static void do_Test ( );
};

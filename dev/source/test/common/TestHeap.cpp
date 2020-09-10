#include "TestHeap.h"

#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdlib.h>
#include <string.h>

#define HEAP_EMTPY_BYTE        0xfe
#define HEADER_CKECK_WORD      0xc1a0c1a0

static char m_Buffer[sizeof(int)*HEAP_WORDS]; 
static int* m_BufferEndP = (int*)(m_Buffer+sizeof(m_Buffer));

size_t round_to_word_num ( size_t aByteNum );

struct TestHeapHeader
{
	TestHeapHeader ( )
	{
		next = prev = NULL;
		numBytes = 0;
		numWords = 0;
		checkStop = checkStart = HEADER_CKECK_WORD;
	}

	~TestHeapHeader ( )
	{
		memset ( this , HEAP_EMTPY_BYTE , sizeof(TestHeapHeader) );
	}

	bool doCheck ( )
	{
		return	checkStart == HEADER_CKECK_WORD && checkStop == HEADER_CKECK_WORD && numWords == round_to_word_num(numBytes);
	}

	GInt64_t getNumWordAvalaible() 
	{
		int*      this_int_p   = reinterpret_cast<int*>(this);
		const int header_words = sizeof(TestHeapHeader)/sizeof(int);
		int*      next_int_p   = NULL;

		if ( next != NULL )
		{
			next_int_p = reinterpret_cast<int*>(next);
		}
		else
		{
			next_int_p = reinterpret_cast<int*>(m_BufferEndP);
		}

		GInt64_t distance_int = (GInt64_t)(next_int_p-this_int_p);

		return distance_int - numWords - 2 * header_words;
	}

	unsigned int    checkStart;
	size_t          numBytes;
	size_t          numWords;
	TestHeapHeader* next;
	TestHeapHeader* prev;
	unsigned int    checkStop;
};

size_t round_to_word_num ( size_t aByteNum )
{
	size_t rest = aByteNum % sizeof(int);

	if(rest==0)
	{
		return aByteNum / sizeof(int);
	}
	else
	{
		return aByteNum / sizeof(int)+1;
	}
}


TestHeap::TestHeap(void)
{
	memset ( m_Buffer , HEAP_EMTPY_BYTE , sizeof(m_Buffer) );

	new ( m_Buffer ) TestHeapHeader;
}

TestHeap::~TestHeap(void)
{
}

void TestHeap::doHeapDump ( std::basic_ostream<char>& aStream ) const
{
	aStream << hex; 
	aStream << setw(8) << setfill('0') << "Dumping heap (base address = 0x" << (long long)m_Buffer << ")" << endl;

	for ( size_t i = 0 ; i < HEAP_WORDS * sizeof(int) ; i+=sizeof(int) )
	{
		aStream << "0x" << setw(8) << setfill('0') << (long long)(i+m_Buffer) << ": 0x" << setw(8) << setfill('0') << *((int*)(m_Buffer+i)) << endl;
	}
}

void TestHeap::doHeapDump ( const char* aPath ) const
{
	fstream file(aPath,ios_base::out);

	doHeapDump ( file );

	file.flush();
	file.close();
}

void TestHeap::doWalkCheck()const
{
	TestHeapWalkResult walk_result = doWalk();
	stringstream error;

	if(!walk_result.isOk)
	{
		error << "Walk check failed!" << endl;
		error << "Walk result was" << endl << walk_result << endl;

		error << "Dump of the content in " << HEAP_DUMP_LOCATION << endl;

		cout << error.str();
		clog << error.str();

		doHeapDump(clog);
				
		exit(0);
	}
}

//searches for a location with a certain amount of data available without allocating it
void* TestHeap::searchLocation  ( size_t aSize )
{
	doWalkCheck();

	TestHeapHeader* h_p         = reinterpret_cast<TestHeapHeader*>(m_Buffer);
	size_t num_word_to_allocate = round_to_word_num ( aSize);

	if ( h_p->numBytes == 0 )
	{
		return reinterpret_cast<void*>(h_p+1);
	}

	do
	{
		long long number_of_word_avalaible = h_p->getNumWordAvalaible();
		
		if ( number_of_word_avalaible>=num_word_to_allocate )
		{
			TestHeapHeader* n_h_p = reinterpret_cast<TestHeapHeader*>((int*)(h_p+1) + h_p->numWords );

			void* result = (void*)(n_h_p + 1 );

			return result;
		}

   }while( ( h_p = h_p->next ) != NULL );

	return NULL;
}

void* TestHeap::reserveScalar   ( size_t aSize )
{
	doWalkCheck();

	TestHeapHeader* h_p         = (TestHeapHeader*)m_Buffer;
	size_t num_word_to_allocate = round_to_word_num ( aSize);

	if ( h_p->numBytes == 0 )
	{
		GInt64_t number_of_word_avalaible = h_p->getNumWordAvalaible(); 

		if(number_of_word_avalaible>=num_word_to_allocate)
		{
			h_p->numBytes = aSize;
			h_p->numWords = num_word_to_allocate;
			void* result = h_p+1;
			memset ( result , 0x0 , aSize );
			return h_p+1;
		}
	}

   do
	{
		GInt64_t number_of_word_avalaible = h_p->getNumWordAvalaible();
		
		if(number_of_word_avalaible>=num_word_to_allocate)
		{
			TestHeapHeader* n_h_p = (TestHeapHeader*)((int*)(h_p+1) + h_p->numWords );
			new ( n_h_p )TestHeapHeader();

			n_h_p->numBytes = aSize;
			n_h_p->numWords = num_word_to_allocate;
			n_h_p->next = h_p->next;
			n_h_p->prev = h_p;
			
			if ( h_p->next )
			{
				h_p->next->prev = n_h_p;
			}

			h_p->next = n_h_p;

			void* result = (void*)(n_h_p + 1 );
			//blanking

			memset ( result , 0x0 , aSize );

			return result;
		}

	}while( ( h_p = h_p->next ) != NULL );

	return NULL;
}

void TestHeap::unreserve ( void* aLocation )
{
	doWalkCheck();

	TestHeapHeader* h_p = (TestHeapHeader*)aLocation-1;

	if(!h_p->doCheck())
	{
		cout << "Error on checking location maybe haeap corrupted or address is wrong!" << endl;
		doHeapDump();
		exit(0);
	}
	else if(h_p->numBytes==0)
	{
		cout << "Closure heap found!" << endl;
		doHeapDump();
		exit(0);		
	}
	else
	{
		//blank
		memset ( aLocation , HEAP_EMTPY_BYTE , h_p->numBytes );

		TestHeapHeader* next_h_p = h_p->next;

		if ( (char*)h_p != m_Buffer )
		{
			if(h_p->prev)
			{
				h_p->prev->next = next_h_p;
			}

			if ( next_h_p )
			{
				next_h_p->prev = h_p->prev;			
			}

			h_p->~TestHeapHeader();
		}
		else
		{
			//first cell header is not removed just zeroed
			h_p->numBytes = h_p->numWords = 0;
		}

		//link previous and next
	}
}

void* TestHeap::reserveArray ( size_t aDataCapacity , size_t /*suggested_capacity*/ )
{
	return reserveScalar ( aDataCapacity );
}

void* TestHeap::reallocArray ( void* aDataP , size_t aCurrentDataSize , size_t aDataCapacity , size_t aSuggestedDataCapacity )
{
	doWalkCheck();

	if ( !aDataP )
	{
		return reserveArray ( aDataCapacity , aSuggestedDataCapacity );
	}
	else
	{
		TestHeapHeader* h_p = (TestHeapHeader*)aDataP-1;

		if(!h_p->doCheck())
		{
			cout << "Error on checking location maybe heap is corrupted or address wrong!" << endl;
			doHeapDump();
			exit(0);
		}
		else if(h_p->numBytes==0)
		{
			cout << "Closure heap found!" << endl;
			doHeapDump();
			exit(0);		
		}
		else
		{
			void* new_data = reserveArray ( aDataCapacity , aSuggestedDataCapacity );

			size_t byte_to_move = ( aCurrentDataSize >= h_p->numBytes )?h_p->numBytes:(aCurrentDataSize);

			g::cont::move_data ( aDataP , new_data , byte_to_move );

			unreserve ( aDataP );

			return new_data;
		}
	}
}

//Returns 0 if the check is ok, otherwise the address (rounded to nearest int of the error
GInt64_t m_check_guard ( void* aLocation , size_t aSize )
{
	for ( int i = 0 ; i < (int)aSize ; i++ )
	{
		if ( ((unsigned char*)aLocation)[i] != HEAP_EMTPY_BYTE )
		{
			GInt64_t address = g::get_rounded_by_power_of_2 ( (long long ) aLocation , (long long ) sizeof(int) );

			return address;
		}
	}

	return 0x0;
}

TestHeapWalkResult TestHeap::doWalk ( ) const
{
	TestHeapWalkResult result;

	stringstream error;

	TestHeapHeader* h_p =(TestHeapHeader*)m_Buffer;

	for ( ; h_p != NULL ; h_p = h_p->next )
	{
		if(!h_p->doCheck())
		{
			error << "Not a valid header in 0x" << hex << setw(8) << setfill('0') << (long long)h_p << endl;
			cout << error.str();
			cerr << error.str();

			result.isOk = false;
			return result;
		}
		else
		{
			char* data_p        = (char*)(h_p+1);
			char* end_data_p    = data_p+h_p->numBytes;
			char* end_check     = (h_p->next)? ( (char*)h_p->next ) : (char*)m_BufferEndP;
			size_t len_to_check = end_check - end_data_p;

			GInt64_t guard_check_result;

			if ( ( guard_check_result = m_check_guard ( end_data_p , len_to_check ) ) != 0x0 )
			{
				error << "Guard bytes are dirty in 0x" << hex << setw(8) << setfill('0') << (GInt32_t)guard_check_result << endl ;
				error << "Reference in interval from 0x" << hex << setw(8) << setfill('0') << (long long)end_data_p;
				error << " to 0x" << hex << setw(8) << setfill('0') << (long long)(end_data_p + len_to_check) << endl;
				result.isOk = false;

				cout << error.str();
				cerr << error.str();

				return result;								
			}
			else if ( h_p->numBytes > 0 )
			{
				result.objectNumber++;
				result.byteUsed += h_p->numBytes;
			}
		}
	}

	return result;
}

void TestHeap::do_Test ( )
{
	TestHeap the_heap;
	const char* w[]={"ciao","maio","bau"};
	void* m[3];

	for (int i=0; i<3 ;i++)
	{
		m[i]=the_heap.reserveArray ( sizeof(char), (int)strlen(w[i])+1 );
		strcpy((char*)m[i],w[i]);
	}

	the_heap.unreserve (m[1]);

	char* tst = (char*)the_heap.reallocArray (m[0],sizeof(char),(int)strlen(w[0])+1,strlen(w[0])+1);

	cout << tst << endl;

	the_heap.doHeapDump();
}

size_t TestHeap::getLocationSize ( void* aLocation )
{
	doWalkCheck();

	TestHeapHeader* h_p = (TestHeapHeader*)aLocation-1;
	
	return h_p->numBytes;
}

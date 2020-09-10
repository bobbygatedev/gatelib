#include "g_cont_common.h"
#include "g_cont_gstr.h"
#include <iostream>

#if G_AUTOPTION_ON_MSVC > 0
#pragma warning( disable : 4996 )
#endif

namespace g
{
namespace cont
{

#define CONSTANT_BUFFER_SIZE 1024

static char mConstantBuffer[CONSTANT_BUFFER_SIZE];

class InternalConstantAllocator : public HeapAbstract
{
public:
	InternalConstantAllocator():mBufferP(mConstantBuffer){}
	virtual ~InternalConstantAllocator(){}

	virtual void* reserveArray ( size_t size , size_t cardinality , size_t /*suggested_capacity*/ = EQUAL_TO_CAPACITY )
	{
		return reserveScalar ( size * cardinality );
	}
	
	virtual void* reserveScalar   ( size_t s )
	{
      G_EXC_SET_CONTEXT ( "void* InternalConstantAllocator::reserveScalar ( size_t s )" );

		void* result = mBufferP;

		if ( ( mBufferP += s ) <= mConstantBuffer +  CONSTANT_BUFFER_SIZE )
		{
			return result;
		}
		else
		{
			G_EXC_RAISE_CONT ( "Not enough space" );
		}

		return result;
	}

	virtual void  unreserve ( void* )	{ /*does nothing*/ }

	virtual void* reserveArray ( size_t data_capacity , size_t /*suggested_capacity*/ = EQUAL_TO_CAPACITY )
	{
		return reserveScalar ( data_capacity );
	}

	virtual void* reallocArray ( 
      void*  data , 
      size_t current_data_size , 
      size_t /*data_capacity*/           = EQUAL_TO_CURRENT , 
      size_t /*suggested_data_capacity*/ = EQUAL_TO_CAPACITY )
	{
		if (!data)
		{
			return reserveScalar(current_data_size);
		}
		else
		{
			return data;
		}
	}

private:
	char* mBufferP;
};

static InternalConstantAllocator  m_Allocator;
static AllocationPolicySimple     m_InnerAllocPolicy(&m_Allocator); 

template < > void gstr<char>::plot () const
{
	std::cout << mBufferP;
}

template < > void gstr<wchar_t>::plot () const
{
	std::wcout << mBufferP;
}

template <> const gstr<char>& gstr<char>::empty ( )
{
	static gstr<char> the_empty = gstr<char>( "" , &m_InnerAllocPolicy );

	return the_empty;
}

template <> const gstr<wchar_t>& gstr<wchar_t>::empty ( )
{
	static gstr<wchar_t> the_empty = gstr<wchar_t>( L"" , &m_InnerAllocPolicy );

	return the_empty;
}

template <> const gstr<char>& gstr<char>::new_line ( )
{
	static gstr<char> the_new_line = gstr<char>( "\r\n" , &m_InnerAllocPolicy );

	return the_new_line;
}

template <> const gstr<wchar_t>& gstr<wchar_t>::new_line ( )
{
	static gstr<wchar_t> the_new_line = gstr<wchar_t>( L"\r\n" , &m_InnerAllocPolicy );

	return the_new_line;
}

template <> const gstr<char>& gstr<char>::space ( )
{
	static gstr<char> the_space = gstr<char>( " " , &m_InnerAllocPolicy );

	return the_space;
}

template <> const gstr<wchar_t>& gstr<wchar_t>::space ( )
{
	static gstr<wchar_t> the_space = gstr<wchar_t>( L" " , &m_InnerAllocPolicy );

	return the_space;
}

template <> const gstr<char>& gstr<char>::tab ( )
{
	static gstr<char> the_space = gstr<char>( "\t" , &m_InnerAllocPolicy );

	return the_space;
}

template <> const gstr<wchar_t>& gstr<wchar_t>::tab ( )
{
	static gstr<wchar_t> the_space = gstr<wchar_t>( L"\t" , &m_InnerAllocPolicy );

	return the_space;
}

}// namespace cont
}// namespace g

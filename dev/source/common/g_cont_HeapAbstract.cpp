#include "g_cont_HeapAbstract.h"
#include <stdlib.h>
#include "g_atomics.h"

namespace g
{
using namespace atomics;
namespace cont
{
static HeapAbstract* m_DefaultHeapP = NULL;

HeapAbstract* HeapAbstract::heap_SimpleP ( )
{
   static HeapSimple singleton;

   return &singleton;
}

HeapAbstract* HeapAbstract::get_Default ( )
{  
   g::atomics::fetch_compare_ptr<HeapAbstract> ( &m_DefaultHeapP , NULL , heap_SimpleP ( ) );

	return m_DefaultHeapP;
}

HeapAbstract* HeapAbstract::set_Default ( HeapAbstract* aNewDefaultAllocatorP )
{
   G_EXC_SET_CONTEXT ( "HeapAbstract* HeapAbstract::set_Default ( HeapAbstract* aNewDefaultAllocatorP )" );

	HeapAbstract* old_allocator_p = m_DefaultHeapP;

	if ( aNewDefaultAllocatorP )
	{
		m_DefaultHeapP = aNewDefaultAllocatorP;
	}
	else
	{
		G_EXC_RAISE_CONT ( "New allocator can't be null!" );
	}

	return old_allocator_p;
}

}//namespace cont
}//namespace g


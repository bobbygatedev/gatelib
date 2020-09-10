#include "g_cont_HeapAbstract.h"
#include "g_atomics.h"
#include <stdlib.h>

namespace g
{
using namespace atomics;

namespace cont
{

void* HeapSimple::reserveArray ( size_t aDataCapacity , size_t ) { return reserveScalar ( aDataCapacity ); }

void* HeapSimple::reallocArray ( void* aDataP , size_t , size_t aDataCapacity , size_t )
{
   G_EXC_SET_CONTEXT ( "void* HeapSimple::reallocArray ( void* aDataP , size_t , size_t aDataCapacity , size_t )" );
   
   void* result = realloc ( aDataP , aDataCapacity );

   if ( !result )
   {
      G_EXC_RAISE ( HeapReservationFailException );
   }

   return result;
}

void* HeapSimple::reserveScalar ( size_t aSize ) 
{ 
   G_EXC_SET_CONTEXT ( "void* HeapSimple::reserveScalar ( size_t aSize ) " );
   
   void* result = malloc(aSize);

   if ( !result )
   {
      G_EXC_RAISE ( HeapReservationFailException );
   }

   return result;
} 

void HeapSimple::unreserve ( void* aData ) { free(aData); }

}//namespace cont
}//namespace g


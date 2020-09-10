#pragma once

#include "g_common_dev.h"

#if G_AUTOPTION_ON_MSVC
#  include <intrin.h>
#elif G_AUTOPTION_ON_WINDOWS
#  include "implement.h"
#endif

namespace g
{
namespace atomics
{

#if G_AUTOPTION_ON_WINDOWS
template < class T > inline T  inc_and_fetch ( T* aValueP );
template < class T > inline T  dec_and_fetch ( T* aValueP );
template < class T > inline T  fetch_compare ( T* aValueP , T aNewValue , T aCompareValue );
template < class T > inline T* fetch_compare_ptr ( T** aValueP , T* aNewValue , T* aCompareValue );

	template <> inline LONG inc_and_fetch ( LONG* aValueP )
   {
#  if G_AUTOPTION_ON_GCC
      return PTW32_INTERLOCKED_INCREMENT_LONG ( ( volatile LONG*) aValueP );
#  else
      return _InterlockedIncrement ( ( volatile LONG*) aValueP );
#  endif
   }

   template <> inline DWORD inc_and_fetch ( DWORD* aValueP ) { return inc_and_fetch<LONG> ( reinterpret_cast<LONG*> ( aValueP ) );  }
   template <> inline int   inc_and_fetch ( int* aValueP ) { return inc_and_fetch<LONG> ( reinterpret_cast<LONG*> ( aValueP ) );  }

   template <> inline LONG dec_and_fetch ( LONG* aValueP )
   {
#  if G_AUTOPTION_ON_GCC
	   return PTW32_INTERLOCKED_DECREMENT_LONG ( ( volatile LONG*) aValueP );
#  else
      return _InterlockedDecrement ( ( volatile LONG*) aValueP );
#  endif
   }

   template <> inline DWORD dec_and_fetch ( DWORD* aValueP ) { return dec_and_fetch<LONG> ( reinterpret_cast<LONG*> ( aValueP ) );  }
   template <> inline int   dec_and_fetch ( int* aValueP ) { return dec_and_fetch<LONG> ( reinterpret_cast<LONG*> ( aValueP ) );  }

   /*
   equivalent to:
   lock { int fetch_value = *aValueP; if ( *aValueP == aCompareValue ) *aValueP=aCompareValue; return fetch_value;  }
   */
   template <> inline LONG fetch_compare ( LONG* aValueP , LONG aNewValue , LONG aCompareValue )
   {      
#  if G_AUTOPTION_ON_GCC
      return PTW32_INTERLOCKED_COMPARE_EXCHANGE_LONG ( ( volatile LONG*) aValueP , (LONG)aNewValue , (LONG)aCompareValue );
#  else
      return _InterlockedCompareExchange ( ( volatile LONG*) aValueP , (LONG)aNewValue , (LONG)aCompareValue );
#  endif
   }

   /*
   equivalent to:
   lock { int fetch_value = *aValueP; if ( *aValueP == aCompareValue ) *aValueP=aCompareValue; return fetch_value;  }
   */
   template <class T> inline T* fetch_compare_ptr ( T** aValueP , T* aNewValue , T* aCompareValue )
   {
#  if G_AUTOPTION_ON_GCC
      return reinterpret_cast<T*> ( PTW32_INTERLOCKED_COMPARE_EXCHANGE_PTR ( (void*)aValueP , aNewValue , aCompareValue ) );
#  elif defined(WIN32)

#     if G_AUTOPTION_ON_MSVC != 0
#        pragma warning (disable:4311)
#        pragma warning (disable:4312)
#     endif

   return  reinterpret_cast<T*>( _InterlockedCompareExchange ( reinterpret_cast<volatile LONG*>(aValueP) , reinterpret_cast<LONG>(aNewValue) , reinterpret_cast<LONG>(aCompareValue) ) );

#     if G_AUTOPTION_ON_MSVC != 0
#        pragma warning (default:4311)
#        pragma warning (default:4312)
#     endif

#  elif defined(_WIN64)
         return _InterlockedCompareExchangePointer ( aValueP , aNewValue , aCompareValue );
#  endif
   }

   template <> inline DWORD fetch_compare ( DWORD* aValueP , DWORD aNewValue , DWORD aCompareValue ) { return fetch_compare<LONG> ( reinterpret_cast<LONG*> ( aValueP ) , aNewValue , aCompareValue );  }
   template <> inline int fetch_compare ( int* aValueP , int aNewValue , int aCompareValue ) { return fetch_compare<LONG> ( reinterpret_cast<LONG*> ( aValueP ) , aNewValue , aCompareValue );  }

#else
   template < class T > inline T  inc_and_fetch ( T* aValueP ) { T val =__sync_fetch_and_add(aValueP,1); return val+1; }
   template < class T > inline T  dec_and_fetch ( T* aValueP ) { T val =__sync_fetch_and_add(aValueP,-1); return val-1; }
   template < class T > inline T  fetch_compare ( T* aValueP , T aNewValue , T aCompareValue )
   	{ return __sync_val_compare_and_swap  ( aValueP , aCompareValue , aNewValue ); }
   template < class T > inline T* fetch_compare_ptr ( T** aValuePP , T* aNewValue , T* aCompareValue ) { return fetch_compare<T*> ( aValuePP , aNewValue , aCompareValue ); }
#endif

}//namespace atomics
}//namespace g

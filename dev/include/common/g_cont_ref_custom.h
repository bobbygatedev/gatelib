#pragma once

#include "g_cont_ref_base.h"

namespace g
{
namespace cont
{
template < class T > class ref_custom : public ref_base<T>
{
private:
	ref_custom(const ref_custom<T>&){}
protected:
	ref_custom(){ G_VERBOSE_MSG_L5("ref_custom<T>::ref_custom()"); }

protected:
	void mRefTake ( const ref_custom& other ) { G_VERBOSE_MSG_L5("ref_custom::mRefTake(ref_custom&)"); ref_base<T>::mRefTake(other); }

#if G_AUTOPTION_ON_MSVC > 0
	template < class O > void mRefTake ( const ref_custom<O>& other ) { G_VERBOSE_MSG_L5("ref_custom::mRefTake(ref_custom<O>&)"); ref_base<T>::mRefTake<O>(other); }
	template < class O > bool mRefTakeTryDynamically ( ref_base<O>& other ) { return ref_base<T>::mRefTakeTryDynamically<O>(other);  }
#else
	template < class O > void mRefTake ( const ref_custom<O>& other ) { G_VERBOSE_MSG_L5("ref_custom::mRefTake(ref_custom<O>&)"); ref_base<T>::mRefTake ( other ); }
	template < class O > bool mRefTakeTryDynamically ( ref_base<O>& other ) { return ref_base<T>::mRefTakeTryDynamically(other);  }
#endif

	inline T*       mGetObjP ( )       { return ref_base<T>::mGetObjP(); }
	inline const T* mGetObjP ( ) const { return ref_base<T>::mGetObjP(); }
};

}//namespace g
}//namespace cont




#pragma once

#include "g_cont_ref_base.h"

namespace g
{
namespace cont
{

template < class T > class ref_const : public ref_base<T>
{
public:
	ref_const()	{ G_VERBOSE_MSG_L5("ref_const::ref_const()"); }
	ref_const ( const ref_const<T>& o )	{ G_VERBOSE_MSG_L5("ref_const::ref_const ( const ref_base<T>& o )");	this->mRefTake(o);	}
	ref_const ( const ref_custom<T>& o ) { G_VERBOSE_MSG_L5("ref_const::ref_const ( const ref_custom<T>& o )"); this->mRefTake(o); }

	ref_const& operator = ( const ref_const<T>& o )  { G_VERBOSE_MSG_L5("ref_const::operator = ( const ref_const<T>& )"); this->mRefTake(o); return *this; }
	ref_const& operator = ( const ref_custom<T>& o ) { G_VERBOSE_MSG_L5("ref_const::operator = ( const ref_custom<T>& )"); this->mRefTake(o); return *this; }

	template < class O > operator ref_const() { G_VERBOSE_MSG_L5(operator ref_const()); ref_const<O> res; res.mRefTake (*this); return res;  }

	const T& operator *() const { return *this->mGetObjP(); }
	const T* operator ->() const { return this->mGetObjP(); }
};

}//namespace g
}//namespace cont




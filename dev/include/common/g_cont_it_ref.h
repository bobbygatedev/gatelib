#pragma once

#include "g_cont_it_base.h"

namespace g
{
namespace cont
{

template < class T > class ref;
template < class T > class ref_const;
template < class T > class ref_const;
template < class T > class base;
template < class T , class REF , class REF_C > class it_ref;

template < class T , class REF = ref<T> , class REF_C = ref_const<T> > class it_ref_const : public it_base<REF,REF,REF_C>
{
public:
	it_ref_const(){}

	typedef typename it_base<REF,REF,REF_C>::RecipientBase_t RecipientBase_t;
	typedef REF   Ref_t;
	typedef REF_C RefConst_t;
	typedef it_ref<T,REF,REF_C> It_t;

	it_ref_const (const RecipientBase_t& aRecipient , IterFrom_t from = head ) { this->mInit ( aRecipient , from ); }
	it_ref_const (const it_ref_const& other) { this->mCopy(other); }
	it_ref_const (const It_t& other)       { this->mCopy(other); }

	it_ref_const& operator = (const it_ref_const& other ){mCopy(other);return *this;}

	it_ref_const operator + (int inc) { it_ref_const result(*this); result.moveOf(inc); return result; }
	it_ref_const operator - (int dec) { it_ref_const result(*this); result.moveOf(-dec); return result; }
	it_ref_const& operator ++ ( int ){ this->moveOf(1); return*this; }//preincrementum
	it_ref_const& operator -- ( int ){ this->moveOf(1); return*this; }//predecrementum
	it_ref_const& operator ++ ( ){ this->moveOf(1); return*this; }//postincrementum
	it_ref_const& operator -- ( ){ this->moveOf(1); return*this; }//postdecrementum
	it_ref_const& operator += ( int inc ){ this->moveOf(inc); return*this; }
	it_ref_const& operator -= ( int dec ){ this->moveOf(-dec); return*this; }

	Ref_t&     underlying ( )       { return *this->mRecipientP->mPositionerP->getPtr ( this->mIndex ); }
	RefConst_t underlying ( ) const { return RefConst_t(*this->mRecipientP->mPositionerP->getPtr ( this->mIndex )); }

	operator RefConst_t ( ) const { return this->underlying(); }

	const T* operator -> ( ) const { return this->underlying().operator ->(); }
	const T& operator *  ( ) const { return this->underlying().operator * (); }
};

template < class T , class REF = ref<T> , class REF_C = ref_const<T> > class it_ref : public it_base<REF,REF,REF_C>
{
public:
	typedef typename it_base<REF,REF,REF_C>::RecipientBase_t RecipientBase_t;
	typedef REF   Ref_t;
	typedef REF_C RefConst_t;

	it_ref(){}
	it_ref (RecipientBase_t& aRecipient , IterFrom_t from = head ) { this->mInit ( aRecipient , from ); }
	it_ref (const it_ref& other){ this->mCopy(other); }

	it_ref& operator = (const it_ref& aOther){ this->mCopy(aOther); return *this;}

	it_ref operator + (int inc) { it_ref result(*this); result.moveOf(inc); return result; }
	it_ref operator - (int dec) { it_ref result(*this); result.moveOf(-dec); return result; }
	it_ref& operator ++ ( int ){ this->moveOf(1); return*this; }//preincrementum
	it_ref& operator -- ( int ){ this->moveOf(1); return*this; }//predecrementum
	it_ref& operator ++ ( ){ this->moveOf(1); return*this; }//postincrementum
	it_ref& operator -- ( ){ this->moveOf(1); return*this; }//postdecrementum
	it_ref& operator += ( int inc ){ this->moveOf(inc); return*this; }
	it_ref& operator -= ( int dec ){ this->moveOf(-dec); return*this; }

	Ref_t&      underlying ( )       { return *this->mRecipientP->mPositionerP->getPtr ( this->mIndex ); }
	RefConst_t  underlying ( ) const { return RefConst_t(*this->mRecipientP->mPositionerP->getPtr ( this->mIndex )); }

	operator Ref_t&       ( )       { return this->underlying ();  }
	operator const Ref_t& ( ) const { return this->underlying ();  }

	const T* operator -> ( ) const { return this->underlying().operator ->(); }
	const T& operator *  ( ) const { return this->underlying().operator * (); }

	T* operator -> ( ) { return this->underlying().operator ->(); }
	T& operator *  ( ) { return this->underlying().operator * (); }
};

}//namespace cont
}//namespace g



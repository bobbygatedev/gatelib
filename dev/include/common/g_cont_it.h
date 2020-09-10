#pragma once

#include "g_cont_it_base.h"

namespace g
{
namespace cont
{

template < class T > class it : public it_base<T,T,const T>
{
public:
	typedef typename it_base<T,T,const T>::RecipientBase_t RecipientBase_t;

	it(){}
	it (RecipientBase_t& aRecipient , IterFrom_t aIterDir = head ) { this->mInit ( aRecipient , aIterDir ); }
	it (const it& other){ this->mCopy(other); }
	it& operator = (const it& aOther){ this->mCopy(aOther);return *this;}

	it operator + (int inc) { it result(*this); result.moveOf(inc); return result; }
	it operator - (int dec) { it result(*this); result.moveOf(-dec); return result; }
	it& operator ++ ( int ){ this->moveOf(1); return*this; }//preincrementum
	it& operator -- ( int ){ this->moveOf(1); return*this; }//predecrementum
	it& operator ++ ( ){ this->moveOf(1); return*this; }//postincrementum
	it& operator -- ( ){ this->moveOf(1); return*this; }//postdecrementum
	it& operator += ( int inc ){ this->moveOf(inc); return*this; }
	it& operator -= ( int dec ){ this->moveOf(-dec); return*this; }

	T&       underlying ( )       { return *this->mRecipientP->mPositionerP->getPtr ( this->mIndex ); }
	const T& underlying ( ) const { return *this->mRecipientP->mPositionerP->getPtr ( this->mIndex ); }

	const T* operator -> ( ) const { return &underlying(); }
	const T& operator *  ( ) const { return underlying(); }

	T* operator -> ( ) { return &underlying(); }
	T& operator *  ( ) { return underlying(); }
};

template < class T > class it_const : public it_base<T,T,const T>
{
public:
	typedef typename it_base<T,T,const T>::RecipientBase_t RecipientBase_t;

	it_const (){}
	it_const ( const RecipientBase_t& aRecipient , IterFrom_t aIterFrom = head ) { this->mInit ( aRecipient , aIterFrom ); }
	it_const ( const it_const& other ) { this->mCopy(other); }
	it_const ( const it<T>& other )   { this->mCopy(other); }

	it_const& operator = ( const it_const& other ) { this->mCopy(other); return *this; }

	it_const operator + (int inc) { it_const result(*this); result.moveOf(inc); return result; }
	it_const operator - (int dec) { it_const result(*this); result.moveOf(-dec); return result; }
	it_const& operator ++ ( int ){ this->moveOf(1); return*this; }//preincrementum
	it_const& operator -- ( int ){ this->moveOf(1); return*this; }//predecrementum
	it_const& operator ++ ( ){ this->moveOf(1); return*this; }//postincrementum
	it_const& operator -- ( ){ this->moveOf(1); return*this; }//postdecrementum
	it_const& operator += ( int inc ){ this->moveOf(inc); return*this; }
	it_const& operator -= ( int dec ){ this->moveOf(-dec); return*this; }

	const T& underlying ( ) const { return *this->mRecipientP->mPositionerP->getPtr ( this->mIndex ); }

	const T* operator -> ( ) const { return &underlying(); }
	const T& operator *  ( ) const { return underlying(); }
};

}//namespace cont
}//namespace g


#pragma once

#include "g_cont_common.h"

namespace g
{
namespace cont
{
class ItUnspec 
{
public:
	virtual ~ItUnspec(){}

	virtual void toBegin ( ) = 0;
	virtual void toEnd   ( ) = 0;

	virtual void forward  ( GUint32_t inc = 1 ) = 0;
	virtual void backward ( GUint32_t dec = 1 ) = 0;

	virtual void moveOf ( int inc ) = 0;
	virtual bool isNull ( ) const = 0;

	virtual bool isIn ( ) const = 0;
};


}//namespace g
}//namespace cont


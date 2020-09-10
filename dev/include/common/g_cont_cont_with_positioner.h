#pragma once

#include "g_cont_unspec.h"

namespace g
{
namespace cont
{

template<class T> class cont_with_positioner : public Unspec
{
public:
	typedef positioner_abstract<T> Positioner_t;

	template<class,class,class> friend class it_base;
	template<class>             friend class it;
	template<class>             friend class it_const;
	template<class,class,class> friend class it_ref_const;
	template<class,class,class> friend class it_ref;

	cont_with_positioner(AllocationPolicyAbstract* aAllocPolicyP, positioner_abstract<T>* p ) : Unspec ( aAllocPolicyP ) , mPositionerP(p)  { }

protected:
	Positioner_t* mPositionerP;
};

} //namespace cont
} //namespace g


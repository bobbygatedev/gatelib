#include "g_cont_AllocationPolicyAbstract.h"
#include "g_atomics.h"
namespace g
{
namespace cont
{

static AllocationPolicyAbstract* m_DefaultAllocationPolicyP = NULL;

AllocationPolicyAbstract* AllocationPolicyAbstract::get_FromStandardPolicy ( )
{
	return ( *Policies::standard_RefImplpolicyPf )(); 	
}

AllocationPolicyAbstract* AllocationPolicyAbstract::get_Default ( )
{
   g::atomics::fetch_compare_ptr<AllocationPolicyAbstract>( &m_DefaultAllocationPolicyP , allocation_PolicySimpleP ( ) , NULL );
	
   return m_DefaultAllocationPolicyP;
}

AllocationPolicyAbstract* AllocationPolicyAbstract::set_Default ( AllocationPolicyAbstract* aNewAllocationPolicyP )
{
   G_EXC_SET_CONTEXT ( "AllocationPolicyAbstract* AllocationPolicyAbstract::set_Default ( AllocationPolicyAbstract* aNewAllocationPolicyP )" );

	AllocationPolicyAbstract* old_allocation_policy_p = get_Default ( );

	if ( aNewAllocationPolicyP )
	{      
		m_DefaultAllocationPolicyP = aNewAllocationPolicyP;
	}
	else
	{
		G_EXC_RAISE_CONT ( "New allocation policy can't be null" );
	}

	return old_allocation_policy_p;
}

AllocationPolicySimple* AllocationPolicyAbstract::allocation_PolicySimpleP ( )
{
   static AllocationPolicySimple singleton ( HeapAbstract::heap_SimpleP ( ) );

   return &singleton;
}

}//namespace cont
}//namespace g




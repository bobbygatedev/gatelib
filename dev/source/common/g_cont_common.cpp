#include "g_cont_common.h"
#include "g_cont_HeapAbstract.h"
#include "g_cont_AllocationPolicyAbstract.h"

namespace g
{
namespace cont
{

MoveDataPolicyPf_t      Policies::move_DataPolicyPf        = move_data_default;
StandardRefImplPolicyPf Policies::standard_RefImplpolicyPf = AllocationPolicyAbstract::get_Default; 

void G_LIB_ITEM move_data_default ( const char* aFrom , char* aTo , size_t aSize )
{
	long long distance = aTo-aFrom;

	if ( distance > 0 )//up to douwn
	{
		for ( int i = (int)aSize - 1 ; i>=0 ; i-- )
		{
			aTo[i]=aFrom[i];
		}
	}
	else if ( distance < 0 )//down to up
	{
		for ( int i = 0 ; i <(int)aSize - 1 ; i++ )
		{
			aTo[i]=aFrom[i];
		}
	}
}

}//namespace g
}//namespace cont

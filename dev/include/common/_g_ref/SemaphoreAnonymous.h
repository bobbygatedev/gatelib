#pragma once

class SemaphoreAnonymousRef : public g::cont::ref<SemaphoreAnonymous>
{
public:
	
   static ref<SemaphoreAnonymous> g_cont_new (  GUint32_t initial_count = 0  , AllocationPolicyAbstract* d_alloc_policy = AllocationPolicyAbstract::get_FromStandardPolicy ( ) )
   {      
      SemaphoreAnonymousRef result;

      new ( result.mAllocateMemForObject(d_alloc_policy) )SemaphoreAnonymous( initial_count);
      result.mIncRef ( );
		
      return result;
   }

};




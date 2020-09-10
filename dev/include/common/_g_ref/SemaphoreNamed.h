#pragma once

class SemaphoreNamedRef : public g::cont::ref<SemaphoreNamed>
{
public:
	
   static ref<SemaphoreNamed> g_cont_new (  const char * name , NamedSyncObjectCreationOptions_t aCreationOptions = g_create_always , GInt32_t initial_count = 0  , AllocationPolicyAbstract* d_alloc_policy = AllocationPolicyAbstract::get_FromStandardPolicy ( ) )
   {      
      SemaphoreNamedRef result;

      new ( result.mAllocateMemForObject(d_alloc_policy) )SemaphoreNamed( name, aCreationOptions, initial_count);
      result.mIncRef ( );
		
      return result;
   }
   static ref<SemaphoreNamed> g_cont_new (  const SemaphoreNamed & par0  , AllocationPolicyAbstract* d_alloc_policy = AllocationPolicyAbstract::get_FromStandardPolicy ( ) )
   {      
      SemaphoreNamedRef result;

      new ( result.mAllocateMemForObject(d_alloc_policy) )SemaphoreNamed( par0);
      result.mIncRef ( );
		
      return result;
   }

};




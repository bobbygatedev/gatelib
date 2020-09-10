#pragma once

class MyClsRef : public g::cont::ref<MyCls>
{
public:
	
   static ref<MyCls> g_cont_new (  int v1  , AllocationPolicyAbstract* d_alloc_policy = AllocationPolicyAbstract::get_FromStandardPolicy ( ) )
   {      
      MyClsRef result;

      new ( result.mAllocateMemForObject(d_alloc_policy) )MyCls( v1);
      result.mIncRef ( );
		
      return result;
   }
   static ref<MyCls> g_cont_new (  int v1 , int v2 , int v3 = 0  , AllocationPolicyAbstract* d_alloc_policy = AllocationPolicyAbstract::get_FromStandardPolicy ( ) )
   {      
      MyClsRef result;

      new ( result.mAllocateMemForObject(d_alloc_policy) )MyCls( v1, v2, v3);
      result.mIncRef ( );
		
      return result;
   }

};




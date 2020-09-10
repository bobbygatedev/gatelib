#pragma once

class my_clsRef : public g::cont::ref<my_cls>
{
public:
	
   static ref<my_cls> g_cont_new (  int p1  , AllocationPolicyAbstract* d_alloc_policy = AllocationPolicyAbstract::get_FromStandardPolicy ( ) )
   {      
      my_clsRef result;

      new ( result.mAllocateMemForObject(d_alloc_policy) )my_cls( p1);
      result.mIncRef ( );
		
      return result;
   }

};




#include "RefTester.h"
#include <iostream>

using namespace g;
using namespace cont;
using namespace std;

class RefTesterInit
{
public:
	RefTester init_RefTester[100];

	RefTesterInit()
	{
		for(int i = 0 ; i< 100 ; i++ )
		{
			init_RefTester[i].mInstanciate(i);
		}
	}
};


static RefTesterInit m_TheInit;


RefTester::RefTester(int v)
{
	*this = m_TheInit.init_RefTester[v];
}

void RefTester::mInstanciate ( int v )
{
   g::cont::ref<TestType> ref = g::cont::ref<TestType>::g_cont_new ( AllocationPolicyAbstract::get_FromStandardPolicy ( ) );

   ref->setValue ( v );

   this->mRefTake(ref);
}


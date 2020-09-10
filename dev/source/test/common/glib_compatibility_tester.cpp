#include <g_mthread_ThreadTypes.h>
#include <g_mthread_SemaphoreAnonymous.h>
#include <g_mthread_CriticalSection.h>
#include <g_signal.h>
#include <g_mthread_MutexNamed.h>
#include <g_common.h>
#include "GlibContainerTest.h"
#include "GlibMultithreadTest.h"
#include "SampleMoveableAllocationPolicy.h"
#include "TestEmpty.h"
#include "MsgBoxTest.h"
#include "SharedMemTest.h"
#include "MutexTest.h"
#include "ExceptionHandlingTest.h"
#include "ObjWithLockTest.h"
#include <math.h>
#include <time.h>

#if G_AUTOPTION_ON_WINDOWS
#  include <windows.h>
#  include <g_mthread_WinEvent.h>
#else
#  include <unistd.h>
#endif

G_SIGNAL(MySignal(GInt32_t aPar1 , GUint64_t aPar2))

#include G_SIGNAL_INCLUDE(MySignal)

using namespace std;
using namespace g;
using namespace cont;
using namespace alg;
using namespace mthread;

static TestHeap			              m_TestHeap;  
static SampleMoveableAllocationPolicy m_TestMoveablePolicy;
static AllocationPolicySimple         m_TestSimplePolicy(&m_TestHeap);

#include <g_mthread_SemaphoreAnonymous.h>
#include <g_mthread_SemaphoreNamed.h>

using namespace mthread;

class MyCls 
{
public:
G_REF
   MyCls (int v1)  : m1(v1),m2(0) {}
   MyCls ( int v1 , int v2 , int v3 = 0 ) : m1(v1),m2(v2) {}

   int get1() const { return m1; }
   int get2() const { return m2; }

private:
   int m1,m2;
};

template <class T > class my_cls 
{
T mP1;
public:
G_REF
my_cls ( int p1 ) : mP1 ( p1 ){ cout << "p1 = " << p1 << endl; }
#include G_REF_HEADER(my_cls)
};





#include G_REF_HEADER(MyCls)

#include "_g_ref/MyCls.h"

#include <g_cont_ref_vect.h>

int pp( void (*) ( int , int ) )
{
   return 0;
}

int main()
{
my_cls<int> mc(1);
ref< my_cls<int> > mc_ref = my_cls<int>::my_clsRef::g_cont_new(1);

G_EXC_FRAME_BEGIN()
   int d1 = 1;
   int d2 = 2;

   ref<MyCls> cls_ref = MyClsRef::g_cont_new ( d1 , d2 );
   
   int r1 = cls_ref->get1();
   int r2 = cls_ref->get2();

   cout << "Testing ref assist .. rb d1=" << r1 << " d2=" << r2 << endl;

   freopen ( HEAP_DUMP_LOCATION , "w+" , stderr );
  
   TestEmpty glib_test ( "GateLib test","" );

   G_VERBOSE_CALL_L1 ( ref<GlibContainerTest>   glib_container_test_simple_policy_ref   = ref<GlibContainerTest>::g_cont_new ( ); )
   G_VERBOSE_CALL_L1 ( ref<GlibContainerTest>   glib_container_test_moveable_policy_ref = ref<GlibContainerTest>::g_cont_new ( ); )
   G_VERBOSE_CALL_L1 ( ref<GlibMultithreadTest> glib_multithread_test_instance_ref      = ref<GlibMultithreadTest>::g_cont_new ( ); )

   glib_container_test_simple_policy_ref->alternateAllocationPolicyP   = &m_TestSimplePolicy;
   glib_container_test_moveable_policy_ref->alternateAllocationPolicyP = &m_TestMoveablePolicy;

   glib_test.addSubTest ( ref<MsgBoxTest>::g_cont_new( ));
   glib_test.addSubTest ( ref<SharedMemTest>::g_cont_new ( ) );
   glib_test.addSubTest ( glib_container_test_simple_policy_ref );
   glib_test.addSubTest ( glib_container_test_moveable_policy_ref );
   glib_test.addSubTest ( glib_multithread_test_instance_ref );
   glib_test.addSubTest ( ref<ExceptionHandlingTest>::g_cont_new ( ) );
	
   glib_test.doPerform();

   stringstream temp;

   temp << glib_test.getCompleteReportMessage () << endl;
   temp << "Final result " << glib_test.getResultLabel() << endl;

   cout << temp.str();
   clog << temp.str();

   return (glib_test.getResult())?0:-1;
	
G_EXC_FRAME_END_MAIN()

   return -1;
}





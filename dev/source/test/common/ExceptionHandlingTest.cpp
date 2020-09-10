#include "ExceptionHandlingTest.h"
#include "TestWithThreadAbstract.h"
#include <g_mthread_Thread.h>

using namespace std;
using namespace g;
using namespace cont;
using namespace alg;

G_EXC_DEFINE_MSG ( TestException , g::Exception , "Test Exception message!" );

static G_THREAD_SAFE_VAR int m_ExceptionHandlingTag = 0;

void exception_handling_1 ( const Exception& )
{
   m_ExceptionHandlingTag = 1;
   cout << endl << "exception_handling_1" << endl;
}

void exception_handling_2 ( const Exception& )
{
   m_ExceptionHandlingTag = 2;
   cout << endl << "exception_handling_2" << endl;
}


class ExceptionRaiseOverrideTest : public TestWithThreadAbstract
{
public:

   virtual g::cont::Gstr getName        ( ) const { return "Exception Raise override"; }
	virtual g::cont::Gstr getDescription ( ) const { return "Override exception handling then check the changes."; }

private:
   virtual bool mOnThreadTestBody ( )
   {
      G_EXC_SET_CONTEXT( "virtual bool ExceptionRaiseOverrideTest::mOnThreadTestBody ( )" );

      Exception::set_ThreadAlternateRaiseAction( exception_handling_2 );   

      m_ExceptionHandlingTag = 0;

      G_EXC_RAISE_MSG(Exception,"Dummy exc message");

      return m_ExceptionHandlingTag == 2;
   }
   
   virtual bool mOnMainBody ( )
   {
      G_EXC_SET_CONTEXT( "virtual bool ExceptionRaiseOverrideTest::mOnMainBody ( )" );
         
      m_ExceptionHandlingTag = 0;

      G_EXC_RAISE_MSG(Exception,"Dummy exc message");
      
      return m_ExceptionHandlingTag == 1;
   }
   
   virtual void mOnInit ( ) 
   {  
      Exception::alternate_RaiseAction = exception_handling_1;
   }

   virtual void mOnCleanup ( ) 
   { 
      Exception::alternate_RaiseAction = NULL;      
   }

};

ExceptionHandlingTest::ExceptionHandlingTest() 
{
   mAddSubTest ( ref<ExceptionRaiseOverrideTest>::g_cont_new() );
}

ExceptionHandlingTest::~ExceptionHandlingTest() {}

bool ExceptionHandlingTest::mExecute()
{
	return true;
}

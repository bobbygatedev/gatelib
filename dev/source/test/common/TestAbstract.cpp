#include "TestAbstract.h"
#include "TestHeap.h"
#include <sstream>
#include <string>
#include <g_DateTime.h>

using namespace std;
using namespace g;
using namespace cont;

TestAbstract::TestAbstract():
   alternateAllocationPolicyP ( NULL ) ,
   mSavedAllocationPolicyP    ( NULL ) ,
   mListSubTest               ( AllocationPolicyAbstract::allocation_PolicySimpleP ( ) ) , //forced to use malloc/free
   mListSubresult             ( AllocationPolicyAbstract::allocation_PolicySimpleP ( ) ) , //forced to use malloc/free
   mResult                    ( false ) ,
   mParentP                   ( NULL )
{
}

TestAbstract::~TestAbstract() { }

bool TestAbstract::doPerform ( )
{
   if ( getParent ( ) == 0 )
   {
      cerr << "Starting test " << getName ( ) << " at " << DateTime::now () << endl;
   }

	mListSubresult.setEmpty();
	mFailMessage = "";
	cout << "Performing .." << getName();
	cout.flush();
	
	//set the alternate allocation policy 
	setAlternateAllocationPolicy ( );

   mPerformSetup ( );

   mResult = mPerformExecute ( );

	restoreAllocationPolicy ( );	

	cout << " done!" << endl;

	if ( mListSubTest.getSize() > 0 )
	{
		cout << "Peforming subtest .." << endl;

		for ( ListSubItem_t::It_t it(mListSubTest) ; it.isIn() ; it++ )
		{
         AllocationPolicyAbstract* old_policy_p = it->alternateAllocationPolicyP;

         //policy of parent test become the one of the child iff the child have not set ist own policy
         if ( it->alternateAllocationPolicyP == NULL )
         {
			   it->alternateAllocationPolicyP = alternateAllocationPolicyP;
         }

         it->doPerform();
         it->alternateAllocationPolicyP = old_policy_p;//restores old policy 

         if ( !it->getResult ( ) )
         {
            cerr << "Test " << it->getTestPath ( ) << " failed" << endl;
         }
		}

		cout << "Finished subtest!" << endl;
	}

   mPerformCleanup ( );

   return mResult = mResult && mGetSubItemResult();
}

Gstr TestAbstract::getCompleteReportMessage()
{
	//Gstr
	stringstream stream;

	stream << getResultLabel() << ": " << getName() << endl;

	if ( !getResult() && mFailMessage != "" )
	{
		stream <<  Gstr::tab() << mFailMessage << endl;
	}

	if(mListSubresult.getSize()>0)
	{
		for (unsigned int i = 0; i < mListSubresult.getSize() ; ++i)
		{
			SubResult::Ref_t test_ref = mListSubresult[i];

			stream << "Step " << (i+1) << " " << test_ref->getResultLabel() << " " << mListSubresult[i]->getName();

			if( mListSubresult[i]->getDescription() != "" )
			{
				stream << endl << mListSubresult[i]->getDescription();
			}

			stream << endl;
		}
	}

	if(mListSubTest.getSize()>0)
	{
		for (unsigned int i = 0; i < mListSubTest.getSize() ; ++i)
		{
			stringstream temp;

			stream << "Subtest " << (i+1) << " ";

			//read the subtest report message and increments identation
			temp << mListSubTest[i]->getCompleteReportMessage();

			int g_pos;
			int p_pos = temp.tellp();

			for(int i = 0 ; ( g_pos = temp.tellg() ) < p_pos ; i++)
			{
				char line[1024];
				temp.getline(line,1023);

				stream << Gstr::tab() << line << endl;
			}

			stream << endl;
		}
	}

	return Gstr ( stream.str().c_str() , AllocationPolicyAbstract::allocation_PolicySimpleP ( ) );
}

bool TestAbstract::mGetSubItemResult()
{
	for ( ListSubResult_t::ItConst_t it = mListSubresult.getIterator() ; it.isIn() ; it++ )
	{
		if(!it->getResult ())
		{
			return false;
		}
	}

	for ( ListSubItem_t::ItConst_t it = mListSubTest.getIterator() ; it.isIn() ; it++ )
	{
		if(!it->getResult ())
		{
			return false;
		}
	}

	return true;
}

void TestAbstract::mAddSubResult(const char* aName, const char* aDescription, bool aResult)
{
	SubResult::Ref_t sub_ref = SubResult::Ref_t::g_cont_new(AllocationPolicyAbstract::allocation_PolicySimpleP ( ));

	sub_ref->setAll ( aResult , aName , aDescription );

   if ( !aResult )
   {
      cerr << "Test sub aResult " << aName << " has failed " << endl; 
   }

	mListSubresult.pushTail(sub_ref);
}

void TestAbstract::setAlternateAllocationPolicy ( )
{
	if ( alternateAllocationPolicyP != NULL )
	{
		mSavedAllocationPolicyP = AllocationPolicyAbstract::get_Default(); 
		AllocationPolicyAbstract::set_Default ( alternateAllocationPolicyP );
	}
}

void TestAbstract::restoreAllocationPolicy ( )
{
	if ( mSavedAllocationPolicyP != NULL )
	{
		AllocationPolicyAbstract::set_Default ( mSavedAllocationPolicyP );
		mSavedAllocationPolicyP = NULL;
	}
}

void TestAbstract::mPerformHeapWalkCheck ( const char* aName )
{
	//avoid to conditionate the current test 
	restoreAllocationPolicy ();

	TestHeap* test_heap_p = NULL;
	
	if ( alternateAllocationPolicyP != NULL && dynamic_cast<TestHeap*>(alternateAllocationPolicyP->getHeapP() ) )
	{
		test_heap_p = dynamic_cast<TestHeap*>(alternateAllocationPolicyP->getHeapP());
	}

	if ( test_heap_p != NULL )
	{
		TestHeapWalkResult walk_result = test_heap_p->doWalk();

		stringstream full_name;

		if ( aName == NULL )
		{
			full_name << getName ( ) << " heap check.";
		}
		else
		{
			full_name << aName << " heap check.";
		}

		mAddSubResult ( full_name.str().c_str() , "" , walk_result.isOk && walk_result.objectNumber == 0 );
	}	
	else
	{
		stringstream full_name;

		full_name << aName << " test heap check.";
		
		mAddSubResult ( full_name.str().c_str() , "Not performed due to no TestHeap!" , true );
	}

	setAlternateAllocationPolicy ();
}

void TestAbstract::mAddSubTest ( ref<TestAbstract> aSubTestRef )
{
   mListSubTest.pushTail ( aSubTestRef );
   aSubTestRef->mParentP = this;
}

Gstr TestAbstract::getTestPath ( ) const
{
   TestAbstract* parent_p = getParent();
   Gstr          aResult   = getName ( );

   do
   {
      aResult = parent_p->getName ( ) + "." + aResult;   
   }
   while ( ( parent_p = parent_p->getParent ( ) ) != NULL );

   return aResult;
}

void TestAbstract::mPerformAndAddTestAsSubResult ( TestAbstract& aTest )
{
   G_EXC_SET_CONTEXT("void TestAbstract::mPerformAndAddTestAsSubResult ( TestAbstract& aTest )");

   if ( aTest.getParent ( ) != NULL || aTest.getSubtestList( ).getSize() > 0 )
   {
      G_EXC_RAISE(SubTestAsBaseTestException);
   }   
   else
   {
      bool result = aTest.doPerform();
      mAddSubResult ( aTest.getName() , aTest.getDescription() , result );
   }
}

void TestAbstract::mPerformSetup ( )
{
   try { mSetup ( ); }
   catch ( Exception& exc )
   {
      cout << "In Test::mSetup () caught unhandled " << exc.getTag() << " g::Exception with full message:" << endl;
      cout << exc.getFullErrorMsg() << endl;
      cout << "Test " << getName ( ) << " failed!" << endl;
   }
   catch ( std::exception& exc )
   {
      cout << "In Test::mSetup () caught unhandled std::exception with message:" << endl;
      cout << exc.what ( ) << endl;
      cout << "Test " << getName ( ) << " failed!" << endl;
   }
   catch ( ... )
   {
      Exception::generic_ExceptionCancelHandling();
      cout << "In Test::mSetup () caught unhandled exception!" << endl;
   }
}

void TestAbstract::mPerformCleanup   ( )
{
   try { mCleanup ( ); }
   catch ( Exception& exc )
   {
      cout << "In Test::mCleanup () caught unhandled " << exc.getTag() << " g::Exception with full message:" << endl;
      cout << exc.getFullErrorMsg() << endl;
      cout << "Test " << getName ( ) << " failed!" << endl;
   }
   catch ( std::exception& exc )
   {
      cout << "In Test::mCleanup () caught unhandled std::exception with message:" << endl;
      cout << exc.what ( ) << endl;
      cout << "Test " << getName ( ) << " failed!" << endl;
   }
   catch ( ... )
   {
	  Exception::generic_ExceptionCancelHandling();
      cout << "In Test::mCleanup () caught unhandled exception!" << endl;
   }
}

bool TestAbstract::mPerformExecute   ( )
{
   try { return mExecute ( ); }
   catch ( Exception& exc )
   {
      cout << "In Test::mExecute () caught unhandled " << exc.getTag() << " g::Exception with full message:" << endl;
      cout << exc.getFullErrorMsg() << endl;
      cout << "Test " << getName ( ) << " failed!" << endl;
      return false;            
   }
   catch ( std::exception& exc )
   {
      cout << "In Test::mExecute () caught unhandled std::exception with message:" << endl;
      cout << exc.what ( ) << endl;
      cout << "Test " << getName ( ) << " failed!" << endl;
      return false;
   }
   catch ( ... )
   {
      Exception::generic_ExceptionCancelHandling();
      cout << "In Test::mExecute () caught unhandled exception!" << endl;
      return false;
   }
}

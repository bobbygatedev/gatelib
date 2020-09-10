#pragma once

#include "g_cont_gstr.h"
#include "TestItem.h"
#include "SubResult.h"

G_EXC_DEFINE_MSG ( SubTestAsBaseTestException , g::Exception , "Subtest can't have a parent to be a subresult!" ); 

#define G_TEST_HEADER(atestname,atestdescription) \
   virtual g::cont::Gstr getName        ( ) const { return atestname; } \
   virtual g::cont::Gstr getDescription ( ) const { return atestdescription; };


class TestAbstract : public TestItem
{
public:
	typedef g::cont::ref_lst<TestAbstract> ListSubItem_t;
	typedef g::cont::ref_lst<SubResult>    ListSubResult_t;

	TestAbstract();
	virtual ~TestAbstract();

	virtual g::cont::Gstr getName        ( ) const = 0;
	virtual g::cont::Gstr getDescription ( ) const = 0;

   TestAbstract* getParent ( ) const { return mParentP; }

   virtual g::cont::Gstr getTestPath ( ) const;

protected:
   void         mAddSubTest ( g::cont::ref<TestAbstract> );
   
   template < class TT > void mAddSimpleSubTest ( ) { mAddSubTest ( g::cont::ref<TT>::g_cont_new() ); }

	virtual bool mExecute ( ) = 0;
   virtual void mCleanup ( ){}//not required
   virtual void mSetup   ( ){}//not required

public:
	bool          getResult      ( ) const { return mResult; }
	g::cont::Gstr getResultLabel ( ) const { return (getResult())?"PASSED":"  FAIL"; }

	typedef g::cont::ref<TestAbstract> Ref_t;

	const g::cont::ref_lst<TestAbstract>& getSubtestList ( ) const{ return mListSubTest; }

	bool          doPerform                    ( );
	void          setAlternateAllocationPolicy ( );
	void          restoreAllocationPolicy      ( );
	g::cont::Gstr getCompleteReportMessage     ( );

	g::cont::AllocationPolicyAbstract* alternateAllocationPolicyP;

protected:
	void mAddSubResult                 ( const char* name , const char* description , bool result );
   void mPerformAndAddTestAsSubResult ( TestAbstract& );
	void mPerformHeapWalkCheck         ( const char* name = NULL );
	
	g::cont::AllocationPolicyAbstract* mSavedAllocationPolicyP;

private:
	bool mGetSubItemResult ( );
   void mPerformSetup     ( );
   void mPerformCleanup   ( );
   bool mPerformExecute   ( );
	
   ListSubItem_t   mListSubTest;
   ListSubResult_t mListSubresult;
   g::cont::Gstr   mFailMessage;
   bool            mResult;
   TestAbstract*   mParentP;
};

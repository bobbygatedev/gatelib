#pragma once

#include "g_cont_gstr.h"
#include "TestItem.h"
#include "SubResult.h"

class TestEmpty : public TestAbstract
{
public:
   TestEmpty(const g::cont::Gstr& aName , const g::cont::Gstr& aDescription ) : mName ( aName ) , mDescription ( aDescription ){}
   virtual ~TestEmpty(){}

   virtual g::cont::Gstr getName ( ) const { return mName; }
   virtual g::cont::Gstr getDescription ( ) const { return mDescription; }

   void addSubTest ( ref<TestAbstract> aSubTestRef ) { mAddSubTest ( aSubTestRef ); }

protected:
   virtual bool mExecute ( ) { return true; }

private:
   g::cont::Gstr mName;
   g::cont::Gstr mDescription;
}; 

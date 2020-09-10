#pragma once

#include <g_cont_gstr.h>
#include "TestAbstract.h"

template<class E_T> class exception_test_base : public TestAbstract
{
public:
   virtual g::cont::Gstr getExceptionName ( ) const { return E_T::get_Tag ( ); }
   virtual g::cont::Gstr getName          ( ) const { return g::cont::Gstr("Check ") + getExceptionName () + " exception."; }
	virtual g::cont::Gstr getDescription   ( ) const { return "Check if " + getExceptionName ( ) +" raises correctly."; }

private:
	virtual bool mExecute()
	{
		bool result = false;
		try
		{
			mTestCommand ( );
		}
		catch (E_T&) { result = true;	}
		catch (g::Exception&)	{}
     
		return result;
	}
protected:
   virtual void mTestCommand ( ) = 0;
};

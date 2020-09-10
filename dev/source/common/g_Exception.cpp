#include "g_common_dev.h"
#include "g_str_lib.h"
#include <sstream>
#include <iostream>
#include <vector>
#include <stdlib.h>

#if G_AUTOPTION_ON_UNIX
#  include <cxxabi.h>

using namespace __cxxabiv1;
#endif

using namespace std;

namespace g
{

ExceptionActionPf Exception::msg_showAction        = Exception::default_MsgShowAction;
ExceptionActionPf Exception::msg_fullShowAction    = Exception::default_MsgFullShowAction;
ExceptionActionPf Exception::fatal_ActionPf        = Exception::default_FatalAction;
ExceptionActionPf Exception::alternate_RaiseAction = NULL;

static G_THREAD_SAFE_VAR ExceptionActionPf m_ThreadFatalAction       = NULL;
static G_THREAD_SAFE_VAR ExceptionActionPf m_ThreadRaiseAction       = NULL;
static G_THREAD_SAFE_VAR ExceptionActionPf m_ThreadMsgShowAction     = NULL;
static G_THREAD_SAFE_VAR ExceptionActionPf m_ThreadMsgFullShowAction = NULL;


/********************************************************************************************
**
********************************************************************************************/
Exception::Exception ( const wstring& aMessage , const Exception::Context& aContext ) :
   mCode ( G_EXCEPTION_UNDEF_CODE ) 
{
   mMessage  = aMessage;
   mContextList.addContext ( aContext );
}
/********************************************************************************************
**
********************************************************************************************/
Exception::Exception ( const string& aMessage , const Exception::Context& aContext ) :
   mCode ( G_EXCEPTION_UNDEF_CODE ) 
{
   mMessage = wstring ( aMessage.begin() , aMessage.end() );
   mContextList.addContext ( aContext );
}
/********************************************************************************************
**
********************************************************************************************/
Exception::Exception  ( const Exception& aOther , const Context& aContext ) : 
   mCode        ( aOther.getCode ( ) ) ,
   mContextList ( aOther.mContextList )
{
   mMessage = aOther.getErrorMsgW();

   mContextList.addContext ( aContext );
}

/********************************************************************************************
**
********************************************************************************************/
Exception::Exception() : mCode ( G_EXCEPTION_UNDEF_CODE )
{

}

/********************************************************************************************
**
********************************************************************************************/
Exception::~Exception ( ){}

/********************************************************************************************
**
********************************************************************************************/
void Exception::default_MsgShowAction( const Exception& aException )
{
#if G_AUTOPTION_ON_WINDOWS
   msg_box ( aException.getErrorMsgW().c_str(), get_exe_name ().c_str() );
#endif
}

/********************************************************************************************
**
********************************************************************************************/
Exception::Context::Context ( ):mLine(-1) { }

/********************************************************************************************
**
********************************************************************************************/
Exception::Context::Context ( const char*  aContextStr , const char* aFile , int aLine ) : 
   mMethod ( aContextStr ) , 
   mFile   ( aFile ) , 
   mLine   ( aLine )  { }

/********************************************************************************************
**
********************************************************************************************/
//Exception::Context::Context ( const Context& aOther ) : mMethod ( aOther.getMethod() ) , mFile ( aOther.getFile() ) , mLine ( aOther.getLine() ) { }

/********************************************************************************************
**
********************************************************************************************/
string Exception::Context::getString ( ) const
{
   stringstream result;

   if ( mMethod != "" )
   {
      result << mMethod << endl;
   }

   if ( mLine != -1 )
   {
      result << " Line: " << mLine;
   }

   if ( mFile != "" )
   {
      if ( mFile.size() > 10 )
      {
         result << endl << mFile;
      }
      else
      {
         result << mFile;
      }
   }

   return result.str();
}

/********************************************************************************************
**
********************************************************************************************/
void Exception::showMsg ( ) const 
{ 
   ExceptionActionPf action_pf_per_thread = Exception::get_ThreadMsgShowAction ( );

   if ( action_pf_per_thread == NULL )
   {
      ( *msg_showAction ) ( *this ); 
   }
   else 
   {
      ( *action_pf_per_thread ) ( *this );       
   }
}

/********************************************************************************************
**
********************************************************************************************/
void Exception::showFullMsg ( ) const 
{ 
   ExceptionActionPf action_pf_per_thread = Exception::get_ThreadMsgFullShowAction ( );

   if ( action_pf_per_thread == NULL )
   {
      ( *msg_fullShowAction ) ( *this ); 
   }
   else 
   {
      ( *action_pf_per_thread ) ( *this );       
   }
}

/********************************************************************************************
**
********************************************************************************************/
Exception::ContextList::ContextList( )
{
   mOffset = 0;
   mSize   = 0;
}

/********************************************************************************************
**
********************************************************************************************/
Exception::ContextList::~ContextList( )
{
}


/********************************************************************************************
**
********************************************************************************************/
void Exception::ContextList::addContext ( const g::Exception::Context& aContext )
{
   if ( mSize < MAX_CONTEXT )
   {
      mContextList[mSize++] = aContext; 
   }
   else
   {
      if ( ++mOffset == MAX_CONTEXT)
      {
         mOffset = 0;
      }   

      mContextList[mOffset-1] = aContext;
   }
}

std::string Exception::ContextList::getString ( ) const
{
   std::stringstream result;

   for (int i = getSize()-1 ; i >= 0 ; i-- )
   {
      result << (*this)[i].getString() << std::endl;
   }

   return result.str();
}

#if G_AUTOPTION_ON_MSVC
#pragma warning(disable:4715)
#endif

const Exception::Context& Exception::ContextList::operator [] ( int aIndex ) const
{
   G_EXC_SET_CONTEXT ( "const Exception::Context& Exception::ContextList::operator [] ( int aIndex ) const" );

   if ( aIndex < getSize() )
   {
      int index = mOffset + aIndex;

      while(index >= MAX_CONTEXT)
      {
         index -= MAX_CONTEXT;
      }

      return mContextList[index];
   }
   else
   {
      G_EXC_FATAL_ACTION ("Index out of bounds\nQuitting" );
      return mContextList[0];//avoid warning
   }
}

#if G_AUTOPTION_ON_MSVC
#pragma warning(default:4715)
#endif

std::wstring Exception::getFullErrorMsgW ( ) const
{
   std::wstringstream result;

   result << L"Reason:" << endl << getErrorMsgW() << endl;
   result << getContextList().getStringW ();

   return result.str();
}

class InnerUnhandledException : public Exception 
{
public:
   InnerUnhandledException ( const std::string& aOriginatingThread )
   {
      wstringstream msg;
      
      msg << "Unknown Unhandled exception in thread " << aOriginatingThread.c_str() << "!";
      mMessage = msg.str();
   }
   
   InnerUnhandledException ( const std::exception& aStdException , const char* aOriginatingThread )
   {
      wstringstream msg;

      msg << "Unhandled std::exception in thread " << aOriginatingThread << ":" << endl;
      msg << aStdException.what();

      mMessage = msg.str();
   }

   InnerUnhandledException ( const Exception& aUnhandledParentExc , const char* aOriginatingThread ) : Exception ( aUnhandledParentExc )
   {
      wstringstream msg;

      msg << "Unhandled exception of type " << aUnhandledParentExc.getTagW() << " in thread " << aOriginatingThread << ":" << endl;
      msg << aUnhandledParentExc.getErrorMsgW ( );

      mMessage = msg.str();
   }
   
   virtual const char* getTag           ( ) const { return "UnhandledException"; }
};
/********************************************************************************************
**
********************************************************************************************/
void Exception::do_UnhandledExceptionHandling ( const Exception& aException , const char* aOriginatingThread )
{
   do_FatalAction ( InnerUnhandledException ( aException , aOriginatingThread ) );
}

/********************************************************************************************
**
********************************************************************************************/
void Exception::do_UnhandledExceptionHandling ( const char* aOriginatingThread )
{
   do_FatalAction ( InnerUnhandledException ( aOriginatingThread ) );
}

/********************************************************************************************
**
********************************************************************************************/
void Exception::do_UnhandledExceptionHandling ( const std::exception& aStdException , const char* aOriginatingThread )
{
   do_FatalAction ( InnerUnhandledException ( aStdException , aOriginatingThread ) );
}

/********************************************************************************************
**
********************************************************************************************/
void Exception::default_FatalAction( const Exception& aException )
{
   wstring exe_name = get_exe_name ( );

   wcerr << exe_name << " raised " << aException.getTagW ( ) << " exception!" << endl;

   msg_fullShowAction ( aException );

   exit (0);
}

void Exception::do_FatalAction ( const Exception& aException ) 
{ 
   ExceptionActionPf action_pf_per_thread = Exception::get_ThreadFatalAction ( );

   if ( action_pf_per_thread == NULL )
   {
      ( *fatal_ActionPf ) ( aException );
   }
   else 
   {
      ( *action_pf_per_thread ) ( aException );
   }
}

ExceptionActionPf Exception::get_ThreadFatalAction( ) { return m_ThreadFatalAction; }
void Exception::set_ThreadFatalAction ( ExceptionActionPf aExceptionPf ) { m_ThreadFatalAction = aExceptionPf; }

ExceptionActionPf Exception::get_AlternateRaiseAction ( ) 
{ 
   if ( m_ThreadRaiseAction != NULL )
   {
      return m_ThreadRaiseAction;
   }
   else
   {
      return alternate_RaiseAction;
   }
}

void Exception::set_ThreadAlternateRaiseAction ( ExceptionActionPf aExceptionPf ) { m_ThreadRaiseAction = aExceptionPf; }

ExceptionActionPf Exception::get_ThreadMsgShowAction (  ) { return m_ThreadMsgShowAction; }
void Exception::set_ThreadMsgShowAction ( ExceptionActionPf aExceptionPf ) { m_ThreadMsgShowAction = aExceptionPf; }

ExceptionActionPf Exception::get_ThreadMsgFullShowAction ( ) { return m_ThreadMsgFullShowAction; }
void Exception::set_ThreadMsgFullShowAction ( ExceptionActionPf aExceptionPf ) { m_ThreadMsgFullShowAction = aExceptionPf; }

void Exception::generic_ExceptionCancelHandling ( )
{
#if G_AUTOPTION_ON_UNIX
    try
    {
    	throw;//rethrows the last exception for filtering
    }
    catch(__forced_unwind& )
    {
    	throw;
    }
    catch (...)
    {
    	return;
    }
#endif
}
} //namespace g


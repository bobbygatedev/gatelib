#pragma once

#include "g_common_functions.h"
#include "g_exception_macros.h"
#include <list>

namespace g
{
   class Exception;

   typedef void ( *ExceptionActionPf ) ( const Exception& );

   #define MAX_CONTEXT            128
   #define G_EXCEPTION_UNDEF_CODE -1

   //!Anchestor Exception class for g::lib
   class G_LIB_ITEM Exception
   {
   public:
      class Context;

      // standard copy constructor
      Exception  ( const std::wstring& msg , const Context& context );
      Exception  ( const std::string&  msg , const Context& context );
      Exception  ( const Exception&        , const Context& context );
      Exception  ( );
      virtual ~Exception ( );

      static void do_FatalAction                ( const Exception& );
      static void default_FatalAction           ( const Exception& );

      static ExceptionActionPf get_ThreadFatalAction ( );
      static void              set_ThreadFatalAction ( ExceptionActionPf );

      static void do_UnhandledExceptionHandling ( const Exception&     , const char* thread_name ); 
      static void do_UnhandledExceptionHandling ( const std::exception& , const char* thread_name );
      static void do_UnhandledExceptionHandling ( const char* thread_name );
      /*!Should be used inside a thread for handling LINUX unwind exception
      after a thread.cancel() call i.e.:
      catch(...)
      {
         Exception::generic_ExceptionCancelHandling();
         //your operation after un recognized error.
      }*/
      static void generic_ExceptionCancelHandling ( );

      //!Encapsulates the call context of an exception,
      //!programmer shouldn't instanciate directly the constructor to this class,
      //!but use macros G_EXC_RAISE,G_EXC_RAISE_MSG for raising an exception
      class G_LIB_ITEM Context
      {
      public:
         Context ( );
         Context ( const char* context_str , const char*  file = ""  , int line = -1 );
         //Context ( const Context& o );

         const std::string& getMethod ( ) const { return mMethod; }
         const std::string& getFile   ( ) const { return mFile; }
         int                getLine   ( ) const { return mLine; }

         std::string        getString  ( ) const;
         std::wstring       getStringW ( ) const { return to_Wstring ( getString ( ) ); }

      private:
         std::string mMethod;
         std::string mFile;
         int         mLine;
      };

      class G_LIB_ITEM ContextList
      {
      public:
         //uses standard copy constructor
         ContextList( );
         virtual ~ContextList( );

         //In order to allow
         void addContext ( const Context& );
         int  getSize ( ) const { return mSize; }

         std::wstring        getStringW ( ) const { return to_Wstring ( getString ( ) );  }
         std::string         getString  ( ) const;

         const Context& operator [] ( int index ) const;

      private:
         int      mSize;
         int      mOffset;
         //in order to save space mempry is reserved (avoid to call default constructor)
         Context mContextList[MAX_CONTEXT];
      };
      
      template < class E_T > static void raise_Helper   ( E_T& aException );
      template < class E_T > static void do_Raise       ( const char* aContextStr , const char* aFile , int aLine );
      template < class E_T > static void do_RaiseMsg    ( const char* aMsg        , const char* aContextStr , const char* aFile , int aLine );
      template < class E_T > static void do_FatalAction ( const char* aMsg        , const char* aContextStr , const char* aFile , int aLine ) { do_FatalAction ( E_T ( aMsg , Context(aContextStr,aFile,aLine) ) ); }
      template < class E_T > static void do_Rethrow     ( const E_T& aInnerExc    , const char* aContextStr , const char* aFile , int aLine );

      static ExceptionActionPf get_AlternateRaiseAction ( );
      static void              set_ThreadAlternateRaiseAction ( ExceptionActionPf );
     
      void                showMsg          ( ) const;
      void                showFullMsg      ( ) const;
      const std::wstring& getErrorMsgW     ( ) const { return mMessage; }
      const std::string   getErrorMsg      ( ) const { return to_StdString(getErrorMsgW()); }

      std::wstring        getFullErrorMsgW ( ) const;
      std::string         getFullErrorMsg  ( ) const { return to_StdString(getFullErrorMsgW()); }

      static const char*  get_Tag          ( ) { return "Exception"; }
      virtual const char* getTag           ( ) const { return Exception::get_Tag(); }

      const std::wstring  getTagW          ( ) const { return to_Wstring(getTag()); }
      const ContextList&  getContextList   ( ) const { return mContextList; }

      //Returns error code if any
      GInt64_t            getCode          ( ) const { return mCode; }

      static void default_MsgShowAction     ( const Exception& );
      static void default_MsgFullShowAction ( const Exception& );

      static ExceptionActionPf get_ThreadMsgShowAction ( );
      static void              set_ThreadMsgShowAction ( ExceptionActionPf );

      static ExceptionActionPf get_ThreadMsgFullShowAction ( );
      static void              set_ThreadMsgFullShowAction ( ExceptionActionPf );

      //Action to be perfomed for showing message
      static ExceptionActionPf msg_showAction;
      static ExceptionActionPf msg_fullShowAction;

      //fatal action handling
      static ExceptionActionPf fatal_ActionPf;
      static ExceptionActionPf alternate_RaiseAction;

   protected:
      std::wstring       mMessage;
      GInt64_t           mCode;
      ContextList        mContextList;
   };

   class UnimplemetedException : public Exception
   {
   public:
      UnimplemetedException ( const Context& context ) : Exception ( std::string("Unimplemented context ") + context.getString() , context ) {}

      static const char*  get_Tag ( ) { return "UnimplemetedException"; }
      virtual const char* getTag  ( ) const { return get_Tag(); }
   };

   template < class E_T > void Exception::do_Rethrow ( const E_T& aInnerExc , const char* aContextStr , const char* aFile , int aLine )
   {
	   E_T exception_instance ( aInnerExc , Context(aContextStr,aFile,aLine) );
	   Exception::raise_Helper ( exception_instance );
   }

   template < class E_T > void Exception::do_Raise ( const char* aContextStr , const char* aFile , int aLine ) 
   { 
      E_T exception_instance ( g::Exception::Context(aContextStr,aFile,aLine) );
      Exception::raise_Helper ( exception_instance );
   }
   
   template < class E_T > void Exception::do_RaiseMsg    ( const char* aMsg , const char* aContextStr , const char* aFile , int aLine ) 
   { 
	   E_T exception_instance ( E_T ( aMsg , g::Exception::Context(aContextStr,aFile,aLine) ) );
      Exception::raise_Helper ( exception_instance );
   }

   template < class E_T > void Exception::raise_Helper ( E_T& aException )
   {
      ExceptionActionPf thread_action_pf = get_AlternateRaiseAction ( );

      if ( thread_action_pf != NULL )
      {
         ( *thread_action_pf ) ( aException );
      }
      else
      {
         throw aException;
      }            
   }

   G_EXC_DEFINE ( AssertException , Exception ); 
   G_EXC_DEFINE ( SysException    , Exception ); 
}//namespace g


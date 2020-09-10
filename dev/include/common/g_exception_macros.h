/** @file */
#pragma once

/*! 
\defgroup g_exception_macros g_exception_macros

\see g_exception_macros.h
*  @{
*/

//!\brief Auxiliary macro not use!\nUse G_EXC_DEFINE_MSG or G_EXC_DEFINE instead.
#define G_EXC_DEFINE_TAG(aexctypename,abaseexctypename,atag) \
class aexctypename : public abaseexctypename {\
   public:\
   aexctypename ( const std::wstring& msg   , const Context& context ):abaseexctypename(msg,context){}\
   aexctypename ( const std::string&  msg   , const Context& context ):abaseexctypename(msg,context){}\
   aexctypename ( const Exception&    inner , const Context& context ):abaseexctypename(inner,context){}\
   virtual ~aexctypename (){}\
   static const char*  get_Tag ( ) { return atag; }\
   virtual const char* getTag  ( ) const { return get_Tag(); }\
}

//!\brief Auxiliary macro not use!\nUse G_EXC_DEFINE_MSG or G_EXC_DEFINE instead.
#define G_EXC_DEFINE_TAG_MSG(aexctypename,abaseexctypename,atag,amsg) \
class aexctypename : public abaseexctypename {\
   public:\
   aexctypename ( const std::wstring& msg   , const Context& context ):abaseexctypename(msg,context){}\
   aexctypename ( const std::string&  msg   , const Context& context ):abaseexctypename(msg,context){}\
   aexctypename ( const Context& context ):abaseexctypename(G_STR(amsg),context){}\
   aexctypename ( const Exception&    inner , const Context& context ):abaseexctypename(inner,context){}\
   virtual ~aexctypename (){}\
   static const char*  get_Tag ( ) { return atag; }\
   virtual const char* getTag  ( ) const { return get_Tag(); }\
}

/*!
/brief Defines a new exception automatically. Message is to be specify in raise (use G_EXC_RAISE_MSG to raise an exception).
*/
#define G_EXC_DEFINE(aexctypename,abaseexctypename)          G_EXC_DEFINE_TAG ( aexctypename , abaseexctypename , G_STR (aexctypename) )
/*!
/brief Defines a new exception automatically specifying the message (use G_EXC_RAISE to raise an exception).
*/
#define G_EXC_DEFINE_MSG(aexctypename,abaseexctypename,amsg) G_EXC_DEFINE_TAG_MSG ( aexctypename , abaseexctypename , G_STR (aexctypename) , amsg )

//!When boolean 'acondition' fails fatal action is performed. ( Message contains info about the action)
#define G_ASSERT(acondition)                                 if(!(acondition)){ g::AssertException::do_FatalAction<g::Exception> ( "ASSERTION FAILED:\r\n" G_STR(acondition) , G_EXC_SIGNATURE() ); }

/*!
\brief Sets the method context.
\param acontextstr defines the context method/function prototype.
*/
#define G_EXC_SET_CONTEXT(acontextstr)                       static const char* _g_method_context = acontextstr;

//!Exception signature (method_context, source file, source line )
#define G_EXC_SIGNATURE()                                    _g_method_context ,  __FILE__ , __LINE__

//!Raises an exception specifying the message.
#define G_EXC_RAISE_MSG(aexctype,amessage)                   g::Exception::do_RaiseMsg<aexctype>        ( amessage , G_EXC_SIGNATURE() )

//!Raises an exception using the the default message.
#define G_EXC_RAISE(aexctype)                                g::Exception::do_Raise<aexctype>           ( G_EXC_SIGNATURE() )

//!Executes the fatal action ( showing an exception box, then quitting application ) specifying a fatal error messsage.
#define G_EXC_FATAL_ACTION(amessage)                         g::Exception::do_FatalAction<g::Exception> ( amessage , G_EXC_SIGNATURE() );

#if G_AUTOPTION_ON_WINDOWS
//!Read the windows API error, build the error message then raises an exception.
#  define G_EXC_RAISE_WIN()                                  g::WinException::do_Raise ( _g_sys_call , G_EXC_SIGNATURE() )
#endif

/*!
\brief Calls a sys(POSIX/WINAPI) operation.

\param acall is the body of the call (i.e. 'SetCurrentDirectory ("c:\\temp")' ).
Action consists on invoking 'acall' then save:\n
<ul>
<li>'acall' converted into a string to _g_sys_call.
<li>result of the (POSIX/WINAPI) call to _g_sys_call.
</ul>
*/
#define G_SYS_CALL_ONLY(acall)                               GInt64_t _g_sys_result = (GInt64_t)( acall );static const char* _g_sys_call = G_STR(acall)

/*!
\brief Calls G_SYS_CALL_ONLY then G_EXC_RAISE_WIN, if call acondition is not satisfied.
*/
#define G_WIN_CALL_ASSERT(awincall,acondition)               G_SYS_CALL_ONLY(awincall); if( !(acondition) ){ G_EXC_RAISE_WIN(); }

/*!
\brief Equal to G_WIN_CALL_ASSERT, condition is that result of call not null. 
*/
#define G_WIN_CALL(awincall)                                 G_WIN_CALL_ASSERT( awincall , (_g_sys_result) ) 

//!Raises a PosixException exception 
#define G_EXC_RAISE_POSIX()                                  g::PosixException::do_Raise ( _g_sys_result , _g_sys_call , G_EXC_SIGNATURE() )

/*!
\brief Calls G_SYS_CALL_ONLY then G_EXC_RAISE_POSIX, if call acondition is not satisfied.
*/
#define G_POSIX_CALL_ASSERT(aposixcall,acondition)           G_SYS_CALL_ONLY(aposixcall);if ( !(acondition) ) { G_EXC_RAISE_POSIX(); }

/*!
\brief Equal to G_POSIX_CALL_ASSERT, condition is that result of call is not null. 
*/
#define G_POSIX_CALL(aposixcall)                             G_POSIX_CALL_ASSERT(aposixcall,(_g_sys_result==0))

/*!
\brief Equal to G_POSIX_CALL_ASSERT, condition is that result of call is not equal to -1(Use in case POSIX call returns a value to be used). 
*/
#define G_POSIX_CALL_VALUE(aposixcall)                       G_POSIX_CALL_ASSERT(aposixcall,(_g_sys_result!=-1))

//!Equivalent to 'try{' (used for source code layout).
#define G_EXC_FRAME_BEGIN()                                  try{

/*!
\brief Standard thread exception frame end.

\param athreadname name of the thread ( used in exception message ).
void my_thread_entry_point ( )\n
{\n
   \tG_EXC_FRAME_BEGIN()
   
   \t(..)//my thread body.

   \tG_EXC_FRAME_END_THREAD ("my_thread")\n
}\n
*/
#define G_EXC_FRAME_END_THREAD(athreadname) \
                                                             } catch ( g::Exception& exc ) { g::Exception::do_UnhandledExceptionHandling ( exc , athreadname ); }\
                                                               catch ( std::exception& exc ) { g::Exception::do_UnhandledExceptionHandling ( exc , athreadname ); }\
                                                               catch ( ... ) { g::Exception::generic_ExceptionCancelHandling();g::Exception::do_UnhandledExceptionHandling ( athreadname ); }

//!Equal to G_EXC_FRAME_END_THREAD but athreadname is "MainThread".
#define G_EXC_FRAME_END_MAIN()                               G_EXC_FRAME_END_THREAD ( "MainThread" )

//!Sets a function as unimplemented ( raises UnimplemetedException ).
//\param acontextstr specify, the call context like in G_EXC_SET_CONTEXT .
#define G_EXC_UNIMPLEMENTED(acontextstr)                     G_EXC_SET_CONTEXT(acontextstr); G_EXC_RAISE ( g::UnimplemetedException )

//!Sets a function as unimplemented ( raises UnimplemetedException ).
#define G_EXC_FRAME_FILTER_RETHROW(afilterexc)               } catch ( afilterexc& exc ) { g::Exception::do_Rethrow<afilterexc> ( exc , G_EXC_SIGNATURE() ); }

/*! @} */
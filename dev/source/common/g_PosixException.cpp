#include "g_PosixException.h"
#include <g_common_dev.h>
#include <errno.h>
#include <sstream>

using namespace std;

namespace g
{

static struct PosixErrorCode
{
   const char* symbol; 
   int         value; 
   const char* standard_error_msg;
}
m_SymbolValues[] = {
{ "ENOERROR"          , 0                 , "Not an error." },
#ifdef E2BIG
{ "E2BIG"             , E2BIG             , "Argument list too long." },
#endif
#ifdef EACCES
{ "EACCES"            , EACCES            , "Permission denied." },
#endif
#ifdef EADDRINUSE
{ "EADDRINUSE"        , EADDRINUSE        , "Address in use." },
#endif
#ifdef EADDRNOTAVAIL
{ "EADDRNOTAVAIL"     , EADDRNOTAVAIL     , "Address not available." },
#endif
#ifdef EAFNOSUPPORT
{ "EAFNOSUPPORT"      , EAFNOSUPPORT      , "Address family not supported." },
#endif
#ifdef EAGAIN
{ "EAGAIN"            , EAGAIN            , "Resource unavailable, try again (may be the same value as [EWOULDBLOCK])." },
#endif
#ifdef EALREADY
{ "EALREADY"          , EALREADY          , "Connection already in progress." },
#endif
#ifdef EBADF
{ "EBADF"             , EBADF             , "Bad file descriptor." },
#endif
#ifdef EBADMSG
{ "EBADMSG"           , EBADMSG           , "Bad message." },
#endif
#ifdef EBUSY
{ "EBUSY"             , EBUSY             , "Device or resource busy." },
#endif
#ifdef ECANCELED
{ "ECANCELED"         , ECANCELED         , "Operation canceled." },
#endif
#ifdef ECHILD
{ "ECHILD"            , ECHILD            , "No child processes." },
#endif
#ifdef ECONNABORTED
{ "ECONNABORTED"      , ECONNABORTED      , "Connection aborted." },
#endif
#ifdef ECONNREFUSED
{ "ECONNREFUSED"      , ECONNREFUSED      , "Connection refused." },
#endif
#ifdef ECONNRESET
{ "ECONNRESET"        , ECONNRESET        , "Connection reset." },
#endif
#ifdef EDEADLK
{ "EDEADLK"           , EDEADLK           , "Resource deadlock would occur." },
#endif
#ifdef EDESTADDRREQ
{ "EDESTADDRREQ"      , EDESTADDRREQ      , "Destination address required." },
#endif
#ifdef EDOM
{ "EDOM"              , EDOM              , "Mathematics argument out of domain of function." },
#endif
#ifdef EDQUOT
{ "EDQUOT"            , EDQUOT            , "Reserved." },
#endif
#ifdef EEXIST
{ "EEXIST"            , EEXIST            , "File exists." },
#endif
#ifdef EFAULT
{ "EFAULT"            , EFAULT            , "Bad address." },
#endif
#ifdef EFBIG
{ "EFBIG"             , EFBIG             , "File too large." },
#endif
#ifdef EHOSTUNREACH
{ "EHOSTUNREACH"      , EHOSTUNREACH      , "Host is unreachable." },
#endif
#ifdef EIDRM
{ "EIDRM"             , EIDRM             , "Identifier removed." },
#endif
#ifdef EILSEQ
{ "EILSEQ"            , EILSEQ            , "Illegal byte sequence." },
#endif
#ifdef EINPROGRESS
{ "EINPROGRESS"       , EINPROGRESS       , "Operation in progress." },
#endif
#ifdef EINTR
{ "EINTR"             , EINTR             , "Interrupted function." },
#endif
#ifdef EINVAL
{ "EINVAL"            , EINVAL            , "Invalid argument." },
#endif
#ifdef EIO
{ "EIO"               , EIO               , "I/O error." },
#endif
#ifdef EISCONN
{ "EISCONN"           , EISCONN           , "Socket is connected." },
#endif
#ifdef EISDIR
{ "EISDIR"            , EISDIR            , "Is a directory." },
#endif
#ifdef ELOOP
{ "ELOOP"             , ELOOP             , "Too many levels of symbolic links." },
#endif
#ifdef EMFILE
{ "EMFILE"            , EMFILE            , "Too many open files." },
#endif
#ifdef EMLINK
{ "EMLINK"            , EMLINK            , "Too many links." },
#endif
#ifdef EMSGSIZE
{ "EMSGSIZE"          , EMSGSIZE          , "Message too large." },
#endif
#ifdef EMULTIHOP
{ "EMULTIHOP"         , EMULTIHOP         , "Reserved." },
#endif
#ifdef ENAMETOOLONG
{ "ENAMETOOLONG"      , ENAMETOOLONG      , "Filename too long." },
#endif
#ifdef ENETDOWN
{ "ENETDOWN"          , ENETDOWN          , "Network is down." },
#endif
#ifdef ENETRESET
{ "ENETRESET"         , ENETRESET         , "Connection aborted by network." },
#endif
#ifdef ENETUNREACH
{ "ENETUNREACH"       , ENETUNREACH       , "Network unreachable." },
#endif
#ifdef ENFILE
{ "ENFILE"            , ENFILE            , "Too many files open in system." },
#endif
#ifdef ENOBUFS
{ "ENOBUFS"           , ENOBUFS           , "No buffer space available." },
#endif
#ifdef ENODATA
{ "ENODATA"           , ENODATA           , "[XSR]  No message is available on the STREAM head read queue." },
#endif
#ifdef ENODEV
{ "ENODEV"            , ENODEV            , "No such device." },
#endif
#ifdef ENOENT
{ "ENOENT"            , ENOENT            , "No such file or directory." },
#endif
#ifdef ENOEXEC
{ "ENOEXEC"           , ENOEXEC           , "Executable file format error." },
#endif
#ifdef ENOLCK
{ "ENOLCK"            , ENOLCK            , "No locks available." },
#endif
#ifdef ENOLINK
{ "ENOLINK"           , ENOLINK           , "Reserved." },
#endif
#ifdef ENOMEM
{ "ENOMEM"            , ENOMEM            , "Not enough space." },
#endif
#ifdef ENOMSG
{ "ENOMSG"            , ENOMSG            , "No message of the desired type." },
#endif
#ifdef ENOPROTOOPT
{ "ENOPROTOOPT"       , ENOPROTOOPT       , "Protocol not available." },
#endif
#ifdef ENOSPC
{ "ENOSPC"            , ENOSPC            , "No space left on device." },
#endif
#ifdef ENOSR
{ "ENOSR"             , ENOSR             , "[XSR]  No STREAM resources." },
#endif
#ifdef ENOSTR
{ "ENOSTR"            , ENOSTR            , "[XSR]  Not a STREAM." },
#endif
#ifdef ENOSYS
{ "ENOSYS"            , ENOSYS            , "Function not supported." },
#endif
#ifdef ENOTCONN
{ "ENOTCONN"          , ENOTCONN          , "The socket is not connected." },
#endif
#ifdef ENOTDIR
{ "ENOTDIR"           , ENOTDIR           , "Not a directory." },
#endif
#ifdef ENOTEMPTY
{ "ENOTEMPTY"         , ENOTEMPTY         , "Directory not empty." },
#endif
#ifdef ENOTSOCK
{ "ENOTSOCK"          , ENOTSOCK          , "Not a socket." },
#endif
#ifdef ENOTSUP
{ "ENOTSUP"           , ENOTSUP           , "Not supported." },
#endif
#ifdef ENOTTY
{ "ENOTTY"            , ENOTTY            , "Inappropriate I/O control operation." },
#endif
#ifdef ENXIO
{ "ENXIO"             , ENXIO             , "No such device or address." },
#endif
#ifdef EOPNOTSUPP
{ "EOPNOTSUPP"        , EOPNOTSUPP        , "Operation not supported on socket." },
#endif
#ifdef EOVERFLOW
{ "EOVERFLOW"         , EOVERFLOW         , "Value too large to be stored in data type." },
#endif
#ifdef EPERM
{ "EPERM"             , EPERM             , "Operation not permitted." },
#endif
#ifdef EPIPE
{ "EPIPE"             , EPIPE             , "Broken pipe." },
#endif
#ifdef EPROTO
{ "EPROTO"            , EPROTO            , "Protocol error." },
#endif
#ifdef EPROTONOSUPPORT
{ "EPROTONOSUPPORT"   , EPROTONOSUPPORT   , "Protocol not supported." },
#endif
#ifdef EPROTOTYPE
{ "EPROTOTYPE"        , EPROTOTYPE        , "Protocol wrong type for socket." },
#endif
#ifdef ERANGE
{ "ERANGE"            , ERANGE            , "Result too large." },
#endif
#ifdef EROFS
{ "EROFS"             , EROFS             , "Read-only file system." },
#endif
#ifdef ESPIPE
{ "ESPIPE"            , ESPIPE            , "Invalid seek." },
#endif
#ifdef ESRCH
{ "ESRCH"             , ESRCH             , "No such process." },
#endif
#ifdef ESTALE
{ "ESTALE"            , ESTALE            , "Reserved." },
#endif
#ifdef ETIME
{ "ETIME"             , ETIME             , "[XSR]  Stream ioctl() timeout." },
#endif
#ifdef ETIMEDOUT
{ "ETIMEDOUT"         , ETIMEDOUT         , "Connection timed out." },
#endif
#ifdef ETXTBSY
{ "ETXTBSY"           , ETXTBSY           , "Text file busy." },
#endif
#ifdef EWOULDBLOCK
{ "EWOULDBLOCK"       , EWOULDBLOCK       , "Operation would block (may be the same value as [EAGAIN])." },
#endif
#ifdef EXDEV
{ "EXDEV"             , EXDEV             , "Cross-device link." },
#endif
   
{"Unknown"            ,-1                 , "Unrecognized code."  }
};


/********************************************************************************************
**
********************************************************************************************/
PosixException::PosixException ( GInt64_t aPosixReturned , const char* aPosixCall , const Exception::Context& aContext ) : SysException ( L"" , aContext )
{
   wstringstream   message;
   PosixErrorCode* posix_error_p;

   mCode = (aPosixReturned==-1)? ( errno ) : aPosixReturned;

   for ( posix_error_p = m_SymbolValues ; posix_error_p->value != -1 ; posix_error_p++ )
   {
      if ( mCode == posix_error_p->value )
      {
          break;
      }
   }

   message << "Posix error in call:" << endl << aPosixCall << ":" << endl << posix_error_p->symbol << "(" << posix_error_p->value << "):" << endl << posix_error_p->standard_error_msg;

   mMessage = message.str();
}

void PosixException::do_Raise ( GInt64_t aPosixResult , const char* aPosixCall , const char* aContextStr , const char* aFile , int aLine )
{
   PosixException exception_instance ( aPosixResult ,  aPosixCall , Exception::Context( aContextStr , aFile , aLine ) );

   Exception::raise_Helper ( exception_instance );
}

}//namespace g

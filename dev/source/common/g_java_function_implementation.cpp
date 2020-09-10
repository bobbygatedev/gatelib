#include "g_common_dev.h"
#include "g_str_lib.h"
#include <sstream>
#include <iostream>
#include <string.h>
#include <string>
#include <stdio.h>
#include <jni.h>
#include <sstream>
#include <stdlib.h>
#if G_AUTOPTION_ON_UNIX
#  include <dlfcn.h>
#endif

using namespace std;

namespace g
{

G_EXC_DEFINE ( JavaInternalErrorException , g::Exception );

jclass m_get_jclass ( JNIEnv* aEnvP , const char* aJClassName )
{
   G_EXC_SET_CONTEXT("jclass m_get_jclass ( JNIEnv* aEnvP , const char* aJClassName )");

   jclass result = aEnvP->FindClass(aJClassName);
   
   if ( result != 0 )
   {
      return result;
   }
   else
   {
      stringstream temp;

      temp << "JClass " << aJClassName << " doesn't exist!";
      
      G_EXC_RAISE_MSG(JavaInternalErrorException,temp.str().c_str());

      return NULL;//
   }
}

#if G_AUTOPTION_ON_UNIX
typedef _JNI_IMPORT_OR_EXPORT_  jint ( *JNI_CreateJavaVMPf_t ) ( JavaVM **pvm, void **penv, void *args );


#define G_INNER_TRY( aExpr , aErrMsg ) if ( ( aExpr ) == 0 ) { cout << "Error in call " << G_STR(aExpr) << ":" << endl << aErrMsg << endl; exit(-1);   }

jint JNICALL m_CallJNI_CreateJavaVM ( JavaVM **pvm, void **penv, void *args )
{  
   const char*           java_so_path;
   void*                 handle_jvm_library;
   JNI_CreateJavaVMPf_t  JNI_CreateJavaVM_pf;

   G_INNER_TRY ( java_so_path = getenv("G_JAVA_SHARED_OBJECT") , "Path to libjvm.so not set to env var G_JAVA_SHARED_OBJECT!" );
   G_INNER_TRY ( handle_jvm_library = dlopen ( getenv("G_JAVA_SHARED_OBJECT") , RTLD_NOW ) , "Couln't open libjvm.so!" );
   G_INNER_TRY ( JNI_CreateJavaVM_pf = reinterpret_cast<JNI_CreateJavaVMPf_t>(dlsym(handle_jvm_library,"JNI_CreateJavaVM")) , "Couln't get pointer to function libjvm.so!" );

   return (*JNI_CreateJavaVM_pf)( pvm , penv , args );
}
#endif

JNIEnv* m_get_jni_env ( JavaVM** aJavaVmPP )
{
   G_EXC_SET_CONTEXT ( "JNIEnv* m_get_jni_env ( )" );

   static JNIEnv* env_p     = NULL;
   static JavaVM* java_vm_p = NULL;
   JavaVMOption   options[1];
   JavaVMInitArgs vm_args;
   char           options_str[1024];

   if ( env_p == NULL )
   {
      sprintf ( options_str , "-Djava.class.path=%s", get_jar_path().c_str() );
      options[0].optionString = options_str;
      
      memset ( &vm_args , 0 , sizeof(vm_args) );
      vm_args.version  = JNI_VERSION_1_2;
      vm_args.nOptions = 1;
      vm_args.options  = options;

#if G_AUTOPTION_ON_UNIX
      if ( m_CallJNI_CreateJavaVM ( aJavaVmPP , (void**)&env_p, &vm_args ) == JNI_ERR )
#else 
      if ( JNI_CreateJavaVM ( aJavaVmPP , (void**)&env_p, &vm_args ) == JNI_ERR )
#endif
      {
         stringstream temp;

         temp << "Can't open jar file " + get_jar_path ( );

         G_EXC_RAISE_MSG(JavaInternalErrorException,temp.str().c_str());

         return NULL;
      }
   }

   aJavaVmPP = &java_vm_p;

   return env_p;
}

jmethodID m_get_method_id ( JNIEnv* aEnvP , jclass aClass , const char* aMethodName , const char* aMethodSignature )
{
   G_EXC_SET_CONTEXT("jmethodID m_get_method_id ( jclass aClass , const char* aMethodName , const char* aMethodSignature )");

   jmethodID result = aEnvP->GetStaticMethodID ( aClass , aMethodName , aMethodSignature );

   if ( result != 0 )
   {
      return result;
   }
   else
   {
      stringstream temp;

      temp << "Can't find method " << aMethodName << " with signature " << aMethodSignature ;

      G_EXC_RAISE_MSG(JavaInternalErrorException,temp.str().c_str());
      return NULL;
   }
}

jfieldID m_get_field_id ( JNIEnv* aEnvP , jclass aClass , const char* aMethodName , const char* aMethodSignature )
{
   G_EXC_SET_CONTEXT("jmethodID m_get_method_id ( jclass aClass , const char* aMethodName , const char* aMethodSignature )");

   jfieldID result =  aEnvP ->GetFieldID ( aClass , aMethodName , aMethodSignature );

   if ( result != 0 )
   {
      return result;
   }
   else
   {
      stringstream temp;

      temp << "Can't find method " << aMethodName << " with signature " << aMethodSignature ;

      G_EXC_RAISE_MSG(JavaInternalErrorException,temp.str().c_str());

      return NULL;
   }
}

MsgBoxResult_t java_msg_box ( const char* aMessage , const char* aCaption , MsgBox_t aMsgBoxType )
{
   G_EXC_SET_CONTEXT ( "MsgBoxResult_t java_msg_box ( const char* aMessage , const char* aCaption , MsgBox_t aMsgBoxType )" );

   try 
   {
      JavaVM* java_vm_p;
      JNIEnv* env_p = m_get_jni_env ( &java_vm_p );

      static jclass cls_message_box = m_get_jclass ( env_p , "g/MessageBox" );
      static jclass cls_result      = m_get_jclass ( env_p , "g/MessageBox$Result" );
      static jclass cls_type        = m_get_jclass ( env_p , "g/MessageBox$Type" );

      static jmethodID mid_result          = m_get_method_id ( env_p , cls_message_box , "show" , "(Ljava/awt/Component;Ljava/lang/String;Ljava/lang/String;Lg/MessageBox$Type;)Lg/MessageBox$Result;" );
      //static jmethodID mid_value_of_result = m_get_method_id ( env_p , cls_result , "valueOf" , "(Ljava/lang/String;)Lg/MessageBox$Result;" );
      static jmethodID mid_type_from_int   = m_get_method_id ( env_p , cls_type , "from_Int", "(I)Lg/MessageBox$Type;" );

      jstring msg   = env_p->NewStringUTF(aMessage);
      jstring title = env_p->NewStringUTF(aCaption);
      
      jobject type_enum   = env_p->CallStaticObjectMethod ( cls_result , mid_type_from_int, (jint)aMsgBoxType);
      jobject result_enum = env_p->CallStaticObjectMethod ( cls_message_box , mid_result , NULL , msg , title , type_enum );

      if ( result_enum != NULL )
      {
         jfieldID fid_result_id = env_p->GetFieldID(cls_result,"id","I");

         jint result = env_p->GetIntField(result_enum,fid_result_id);

         return (MsgBoxResult_t)result;
      }
   }
   catch ( JavaInternalErrorException& exc )
   {
      cout << "Fatal java internal error in :" << _g_method_context << endl << exc.getFullErrorMsg () << endl;
      exit(-1);
   }

   return message_box_result_cancel;
}

/********************************************************************************************
**
********************************************************************************************/
void Exception::default_MsgFullShowAction( const Exception& aException )
{
   G_EXC_SET_CONTEXT ( "MsgBoxResult_t java_msg_box ( const char* aMessage , const char* aCaption , MsgBox_t aMsgBoxType )" );

   try
   {
      JavaVM* java_vm_p;
      JNIEnv* env_p              = m_get_jni_env ( &java_vm_p );
      
      static jclass cls_exception_stack = m_get_jclass ( env_p , "g/ExceptionStack" );
      static jmethodID mid_do_show      = m_get_method_id ( env_p , cls_exception_stack , "do_Show" , "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V" );

      jstring tag          = env_p->NewStringUTF(aException.getTag());
      jstring msg          = env_p->NewStringUTF(aException.getErrorMsg().c_str());
      jstring context_list = env_p->NewStringUTF(aException.getContextList().getString().c_str());

      env_p->CallStaticObjectMethod ( cls_exception_stack , mid_do_show , tag , msg , context_list );
   }
   catch ( JavaInternalErrorException& exc )
   {
      cout << "Fatal java internal error in :" << _g_method_context << endl << exc.getFullErrorMsg () << endl;
      cout << "Exception to be shown was of type " << aException.getTag() << endl;
      cout << aException.getFullErrorMsg() << endl;

      exit(-1);
   }
}

}//namespace g

\mainpage gatelib help file

\section intro_sec Introduction

'gatelib' (also called g::lib) project takes its origin from 10 years of working as a SW developer, in particular in in c++.\n
During all this period of time I have been gathering my pieces of code into a general purpose, that i called g library like first letter of my surname.\n
At the beginning classes aren't organized into namespaces, but this approach was adopted and from a certain moment on classes are placed into a namespace, whose root is g::,\n
therefore g::lib definitely became the name of the project.
Initially the aim was to avoid the use of MFC in GUI development so the first group of classes are GUI management classes,\n
but nowadays market offers many products, free or not free, so providing a competitive package is an impossible challenge for a 'one man' developer.\n
I focused my attention on more basic aspects like memory management, multithreading, containers, low level functions, compatibilty among different OS. 
The objective I want to achieve can be listed as follows:
<ul>
<li>Providing a modular library. This can allow the user to include the subset he really need.\n
	With everyday increasing requirements in SW testing, the possibility to simply exclude unused source code represents a great advantage. 
<li>Offer a container template library which allows the user to operate similarly to in Hi-level languages(Java,C#) complementar to STL.
<li>Providing a multithread library.
<li>Being Multiplatform (linux,windows).
</ul>

\section Index
<a href=inst_page.html>Installation instructions</a>\n
<a href=g_assist_page.html>The G_ASSIST source code generator.</a>\n
<a href=g_signal_page.html>Using G_SIGNAL's.</a>\n
<a href=g_ref_page.html>Using G_REF references.</a>\n
<a href=exc_macro_usage.html>Using exception macros.</a>\n

\page exc_macro_usage Exception Macro usage.

\section example_g_exc_raise Example of use of G_EXC_RAISE

G_EXC_DEFINE_MSG ( MethodFailException , g::Exception , "Method failure reason" )\n
\n
void MyCls::method ( const char* a )\n
{\n
   G_EXC_SET_CONTEXT("void MyCls::method ( const char* a )");//You MUST specify the context inside the function. \n
\n
   G_EXC_RAISE ( MethodFailException );\n
}\n

\section example_g_exc_raise_msg Example of use of G_EXC_RAISE_MSG

G_EXC_DEFINE ( MethodFailException , g::Exception )\n
\n
void MyCls::method ( const char* a )\n
{\n
   G_EXC_SET_CONTEXT("void MyCls::method ( const char* a )");//You MUST specify the context inside the function. \n
\n
   G_EXC_RAISE_MSG ( MethodFailException , "Method failure reason" );\n
}\n

\see g_exception_macros

\page inst_page_linux Install gatelib on linux.

\section linux_install_preconditions Preconditions.
<ol>
	<li>gcc 4.7
	<li>Java JDK v7 ( java is used for message box and exception window)
</ol>

\section linux_install_steps Steps to be made.	

<ul>
<li>unzip your delivery.tar.gz into any directory ( 'tar -xvf delivery.tar.gz' )
<li>change dir to delivery
<li>takes root prvilileges ('sudo su -' under Debian/Ubuntu) and launch install.sh (./install.sh) or in Ubuntu/Debian sudo ./install.sh
<li>reboot your PC
</ul>

\section no_ubuntu For no Ubuntu/Debian linux users (and for no Java JDK 7 users).	

Gatelib uses some java function for message box and exception window.\n
If you are not using Ubuntu or Debian, you should check location of java matter, saved in env vars G_JAVA_INCLUDE and G_JAVA_SHARED_OBJECT,\n
then update values in following configuaration scripts:
<ul>
<li>[root]/dev/prj/unix/glibenv.sh:\n		
	export G_JAVA_INCLUDE=/usr/lib/jvm/java-7-openjdk-i386/include\n
	export G_JAVA_SHARED_OBJECT=/usr/lib/jvm/java-7-openjdk-i386/jre/lib/i386/client/libjvm.so\n
\n	
<li>[root]/delivery/linux/gatelib.sh\n
	export G_JAVA_INCLUDE=/usr/lib/jvm/java-7-openjdk-i386/include\n
	export G_JAVA_SHARED_OBJECT=/usr/lib/jvm/java-7-openjdk-i386/jre/lib/i386/client/libjvm.so\n
</ul>
		
\section linux_package_description Package description:
<ul>
<li>/bin             constains binaries(.so,.jar)
<li>/include/common  contains OS common includes
<li>/include/unix    contains includes specific for unix/linux
<li>/lib             contains lib(.a)
<li>/samples         contains template for project under gcc.
</ul>


\page inst_page_windows Install gatelib on windows.

\section win_install_preconditions Preconditions.
<ol>
	<li> MS Visual Studio 2005
	<li> MinGW 4.7
	<li> Java JDK v7 ( java is used for message box and exception window)
</ol>

\section win_install_steps Steps to be made.	

<ul>
<li>unzip your .zip into a your target directory
<li>launch GLibInstaller.exe ( will set automatically env vars and Visual studio 2005 config )
<li>reboot your PC
<li>if your Visual Studio is compatible with 2005 project format you could generate a project using these settings:
	<ul>
	<li>Include dirs \$(G_INCLUDE)\\common;\$(G_INCLUDE)\\Win32
	<li>Library dirs \$(G_LIB)
	<li>Libraries 
		<ul>
		<li>debug(static)	glib_compatibilityd.lib
		<li>debug(dll)	        glib_compatibilityDllD.lib
		<li>relaese(dll)	glib_compatibilityDll.lib
		<li>release(static)     glib_compatibilityd.lib
		</ul>
	</ul>		
</ul>		
\section win_package_description Package description:
<ul>
<li>/bin             constains binaries(.dll,.jar)
<li>/include/common  contains includes
<li>/include/Win32   contains includes
<li>/lib             contains lib(s)
<li>/samples         contains template for project under Visual Studio 2005 and gcc.
</ul>

\page inst_page Installation
<a href=inst_page_windows.html>Windows.</a>\n
<a href=inst_page_linux.html>Linux.</a>\n

\page g_signal_page SIGNALS
\section g_signal_usage Usage
Given a certain signal interface:\n
void g::sig::SampleSignal(int aPar1 , unsigned int aPar2)\n
You can create a corresponding signal declaration in two ways:
<ul>
<li>calling directly G_ASSIST from cmd line.\n
<ol>
<li>'%G_ASSIST% -sc 'void SampleSignal(int aPar1 , unsigned int aPar2)' under windows.\n
<li>'$G_ASSIST -sc 'void SampleSignal(int aPar1 , unsigned int aPar2)' under linux.\n
</ol>
<li>using the precompile and insert the line:\n
<ol>
<li>'G_SIGNAL(void SampleSignal(int aPar1 , unsigned int aPar2))'\n 
<li>in any source or header file (remember to use provided samples for gatelib).\n
</ol>
</ul>

\page g_ref_page REFERENCES

\section g_ref_intro Introduction
References allows you to use ref counted smart pointer objects.\n
A smart pointer object is created as an empty object and must be initialised like in:\n
g::cont::ref<int> int_ref;\n
int_ref = g::cont::ref<int>::g_cont_new();

\section g_ref_usage Usage.
There are two ways to define  

@see g::cont::ref


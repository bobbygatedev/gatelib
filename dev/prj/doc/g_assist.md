\page g_assist_page G_ASSIST UTILITY
\section g_assist_intro Introduction
G_ASSIST is an utility written in JAVA having two aims:
<ol>
<li>Generate OS common message box and exception window.
<li>Acts as a source code generator for <a href=g_signal_page.html>G_SIGNALS</a> and <a href=g_ref_page.html>G_REFERENCES</a>.
</ol>

\section g_assist_scode_doc Java source code documentation.
<a href=g_assist/index.html>Open G_ASSIST source code documentation.</a>


\section g_assist_usage Usage:
G_ASSIST is called from shell with the format:\n
[G_ASSIST] [cmd] -v[0-] {params}

where:\n
<ul>
<li>[G_ASSIST] is \%G_ASSIST\% under windows and $G_ASSIST under linux.
<li>-v[0-] is the verbosity level (possible values -v0,-v1,...)
<li>[cmd] is one from the following possibility
<ul>
<li>-sc\n
Parse a signal expression like:\n
sig_type(partype1 par1,partype1 par2) into the clipboard i.e.java -jar GLibJSupport.jar -sc "MySig(int par1,int par2)"\n

<li>-sh\n
-sh sig_type(partype1 par1,partype1 par2) 'sig_dir' parse a signal into sig_dir i.e.:\n
java -jar GLibJSupport.jar -sh "MySig(int par1,int par2)" "c:\temp"\n
will generate a MySig.h with the result.\n

<li>-p\n
G_ASSIST -p {-vX} [path_to_parse] {-od [specific_output_dir]}  {-or [success_report_path]}\n
Parse a source file interpreting the signals (G_SIGNAL) and refs(G_REF)!\n
If -od is specified [specific_output_dir] becomes the base dir for output files (instead of the dir of input file).\n
If -or is specified an output report will be generated in [success_report_path] in case of success ONLY.\n


<li>-pc\n
G_ASSIST -pc {-vX} [.vcproj] {-o [success_report_path_output_dir]}\n
Pre-compile a MS Visual Studio 2005 project ([.vcproj]) by calling parsing for each .cpp,.c,.h,.cxx,.hxx,.hpp file!\n
If -o is specified [success_report_path_output_dir] contains the output report path which is [source_name.ext.txt].\n
If -o is not specified success report path is generated inside .vcproj path\n

<li>-m\n
Shows a message box with possibly 3 format:\n
<ul>
<li>'[G_ASSIST] -m message'
<li>'[G_ASSIST] -m message title'
<li>'[G_ASSIST] -m message title msg_type' where msg_type can be:\N
<ul>
	<li>ok
	<li>ok_cancel
	<li>retry_cancel
	<li>cancel_retry_continue
	<li>yes_no
	<li>help
	<li>yes_no_cancel
	<li>abort_retry_continue
</ul>	
</ul>

<li>-h
<ul>
	<li>'[G_ASSIST] -h'    Print out the help.
	<li>'[G_ASSIST] -h -l' Print the list of commands only.
	<li>'[G_ASSIST] -h cmd_opt Print out the help of a spefic command i.e '[G_ASSIST] -h -pc'.
</ul>	
</ul>	
</ul>

ReadMe.txt
Use of the development tree.
gatelib development tree uses env var for defining file location across the projects, whose label starts with a 'G_', 
this allows to use alternatively the installed version or the source code version, when you launch:
- the batch file [root]/prj/win32/setenv.bat under windows
- the bash file [root]/prj/unix/glibenv.sh
values of env var are overriden and projects refers to developemnt version.
So in order to simplify the usage of devtree please follow the following instructions.

Windows:
-Create a batch file glib.bat with teh following lines:

>cd [root]\prj\Win32 
>call setenv
>cmd

-launch glib.bat to start development environment from inside this shell.
-type PrepareOutputTree

Win32 batch explaination (in [root]\prj\Win32)

DevEclipse.bat        sets env vars then launch eclipse ( you have to preset env var ECLIPSE with full path of eclipse installation) 
DevVisualStudio.bat   sets env vars then launch MS visual studio 2005
PreCompileSource.bat  sets env vars then [source file]  launch pre-compile process of [source file] type %G_ASSIST% -h for G_ASSIST help
PreCompileVcPrj.bat   sets env vars then [.vcproj file] launch pre-compile process of [.vcproj file] project file type %G_ASSIST% -h for G_ASSIST help
PrepareDelivery.bat   sets env vars then compiles all libaries then prepare an output delivery dir in [root]/../win_output/delivery
PrepareOutputTree.bat sets env vars create output tree in [root]/../win_output
SetEnv.bat            sets env vars 
TestConsole.bat       sets env vars the open a shell in [root]/../win_output/tests

Linux: 
- append the following lines at the end of the file ~/~.bashrc:

#call glibenv.sh for configuring source code environment 
cd ~/glib/dev/prj/unix
. ./glibenv.sh
cd ~

-on a shell type glprepareoutputtree

this defines a set of command having gl has a prefix see [root]\prj\unix for command explaination.
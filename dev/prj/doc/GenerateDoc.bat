rem add doxygen path to env var PATH

call..\..\prj\win32\setenv.bat

md %G_OUTPUT%\doc\doxygen\html

call doxygen.EXE
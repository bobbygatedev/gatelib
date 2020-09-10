echo off 

call SetEnv

echo deleting output folder
erase /S /Q /F %G_OUTPUT%
rd    /S /Q %G_OUTPUT%

echo creating output tree
echo (re)prepare output tree
call PrepareOutputTree.bat

echo creating fresh delivery tree
md %G_OUTPUT%\delivery 
md %G_OUTPUT%\delivery\include 
md %G_OUTPUT%\delivery\bin 
md %G_OUTPUT%\delivery\lib 
md %G_OUTPUT%\delivery\samples

echo copying ReadMe.txt
copy %G_ROOT%\delivery\windows\ReadMe.txt %G_OUTPUT%\delivery

echo building libraries gcc
cd %G_PROJECT_TREE%\gcc\glib_compatibility
make buildall

echo building libraries visual studio
start /WAIT devenv %G_PROJECT_TREE%\VisualStudio2005\glib_compatibility\glib_compatibility.vcproj /Rebuild RELEASE
start /WAIT devenv %G_PROJECT_TREE%\VisualStudio2005\glib_compatibility\glib_compatibility.vcproj /Rebuild RELEASEDLL
start /WAIT devenv %G_PROJECT_TREE%\VisualStudio2005\glib_compatibility\glib_compatibility.vcproj /Rebuild DEBUG
start /WAIT devenv %G_PROJECT_TREE%\VisualStudio2005\glib_compatibility\glib_compatibility.vcproj /Rebuild DEBUGDLL

echo building GLibInstaller
start /WAIT devenv %G_ROOT%\delivery\windows\GLibInstaller\GLibInstaller.vcproj /Rebuild RELEASE

echo copying includes
xcopy /E /Y %G_INCLUDE%\common\* %G_OUTPUT%\delivery\include\common\
xcopy /E /Y %G_INCLUDE%\Win32\*  %G_OUTPUT%\delivery\include\Win32\

echo copyings bins
xcopy /E /Y %G_BIN%\* %G_OUTPUT%\delivery\bin\

echo copyings libs
xcopy /E /Y %G_LIB%\* %G_OUTPUT%\delivery\lib\

echo copying makefile template to samples
copy %G_PROJECT_TREE%\gmakefile.template %G_ROOT%\delivery\windows\samples\gcc\

echo Copying samples
xcopy /E /Y  %G_ROOT%\delivery\windows\samples\* %G_OUTPUT%\delivery\samples\


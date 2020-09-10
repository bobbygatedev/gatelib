set THIS_DIR1=%cd%

cd ..

call CallSetEnv.bat

cd %THIS_DIR1%

echo off 

IF not "%1" == "" (

echo on
set FILE_UNDER_TEST=%1%
set PATH=%PATH%;%cd%\..\pthread.vs2005\debug;%cd%\..\pthread.vs2005\release
start devenv.exe pthread.vs2005.test.dbg.vcproj

) else ( 
echo you must specify the .c file you want to launch i.e StartTestDebug semaphore1.c
)

echo on

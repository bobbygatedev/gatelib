echo off

echo searching for java version and path

call :setKeyValue G_JAVA_VERSION "HKEY_LOCAL_MACHINE\SOFTWARE\JavaSoft\Java Runtime Environment" CurrentVersion

call :setKeyValue G_JAVA_HOME "HKEY_LOCAL_MACHINE\SOFTWARE\JavaSoft\Java Runtime Environment\%G_JAVA_VERSION%" JavaHome
rem shot path needed by gcc
call :setShortPath G_JAVA_HOME "%G_JAVA_HOME%"

call :setKeyValue G_JAVA_JDK_VERSION "HKEY_LOCAL_MACHINE\SOFTWARE\JavaSoft\Java Development Kit" CurrentVersion

call :setKeyValue G_JAVA_JDK_HOME "HKEY_LOCAL_MACHINE\SOFTWARE\JavaSoft\Java Development Kit\%G_JAVA_JDK_VERSION%" JavaHome
rem shot path needed by gcc
call :setShortPath G_JAVA_JDK_HOME "%G_JAVA_JDK_HOME%"

echo java home dir: %G_JAVA_HOME%
echo java jdk home dir: %G_JAVA_JDK_HOME%
echo java runtime version detected: %G_JAVA_VERSION%
echo java development kit(jdk) version detected: %G_JAVA_JDK_VERSION%

echo setting environment variables

rem development only environment variables

rem CHECK G_ROOT IF YOU WANT TO MOVE THE FILE TO ANOTHER LOCATION
set G_ROOT=%~dp0\..\..

call :setFullPath G_ROOT "%G_ROOT%"
set G_OUTPUT=%G_ROOT%\..\win_output
call :setFullPath G_OUTPUT "%G_OUTPUT%"
set G_INTERMEDIATE=%G_OUTPUT%\intermediate

rem LOCATIONS FOR JAVA SUPPORT
set G_JAVA_SUPPORT_NAME=GlibJSupport
set G_JAVA_SUPPORT_PATH=%G_ROOT%\support\java\%G_JAVA_SUPPORT_NAME%

set G_PROJECT_TREE=%G_ROOT%\prj\Win32

set G_VISUAL_STUDIO_SOL_NAME=glib_2.1
set G_VISUAL_STUDIO_LOC=%G_PROJECT_TREE%\VisualStudio2005
set G_VISUAL_STUDIO_DEV="%G_VISUAL_STUDIO_LOC%\%G_VISUAL_STUDIO_SOL_NAME%.sln"

set G_SOURCE=%G_ROOT%\source

rem installation environment variables

set G_INCLUDE=%G_ROOT%\include
set G_BIN=%G_OUTPUT%\bin
set G_LIB=%G_OUTPUT%\lib
set G_JAR_PATH=%G_BIN%\%G_JAVA_SUPPORT_NAME%.jar
set G_ASSIST=java -classpath "%G_JAR_PATH%" g.assist.CmdLineAssist 

set PATH=%PATH%;%G_BIN%;%G_JAVA_JDK_HOME%\bin;%G_JAVA_JDK_HOME%\bin\client;%G_PROJECT_TREE%

if NOT DEFINED ECLIPSE ( echo define the ECLIPSE as the location of eclipse .exe for cpp )
if NOT DEFINED ECLIPSE_JAVA ( echo define the ECLIPSE as the location of eclipse .exe for Java )

echo on

goto:eof

:setFullPath    - set full path of a env var
set "%~1=%~f2"
goto:eof

:setKeyValue - format set VAR
FOR /F "tokens=1,2*" %%A IN ('REG QUERY %2 /v %3 2^>nul') DO (
set "%~1=%%C"
)
goto:eof

:setShortPath - shot path in VAR(useful for Program Files)
set "%~1=%~s2"
goto:eof

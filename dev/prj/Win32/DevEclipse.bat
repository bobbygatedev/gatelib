call SetEnv
echo off

if NOT DEFINED ECLIPSE (

echo env var eclipse not defined please set it with path of eclipse c++ exe.

) else (
call :addEclipse %G_PROJECT_TREE%\eclipse\glib_compatibility
call :addEclipse %G_PROJECT_TREE%\eclipse\glib_compatibility_tester

start %ECLIPSE% -nosplash -data %G_OUTPUT%\eclipse_workspace
)

goto:eof

:addEclipse	
start /WAIT %ECLIPSE% -nosplash -application org.eclipse.cdt.managedbuilder.core.headlessbuild -data %G_OUTPUT%\eclipse_workspace -import "%~1"
goto:eof



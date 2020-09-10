SET BAT_DIR=%~dp0

call %BAT_DIR%SetEnv

if "%2"=="" ( 
%G_ASSIST% -p -v1 %1
) else ( 
%G_ASSIST% -p -v1 %1 -or %2
)
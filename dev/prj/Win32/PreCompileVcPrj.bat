SET BAT_DIR=%~dp0

call %BAT_DIR%SetEnv

if "%2"=="" ( 
%G_ASSIST% -pc -v1 %1
) else ( 
%G_ASSIST% -pc -v1 %1 -o %2
)
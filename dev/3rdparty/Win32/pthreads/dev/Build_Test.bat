call CallSetEnv.bat

set TEST_PRJ_NAME=pthread.vs2005.test

md %TEST_DIR%
md %TEST_DIR%\%TEST_PRJ_NAME%
md %TEST_DIR%\%TEST_PRJ_NAME%\debug
md %TEST_DIR%\%TEST_PRJ_NAME%\release

cd %TEST_PRJ_NAME%

for %%f in (*.c) do ( 
	set FILE_UNDER_TEST=%%f

	start /WAIT devenv pthread.vs2005.test.vcproj /Rebuild DEBUG
	start /WAIT devenv pthread.vs2005.test.vcproj /Rebuild RELEASE
	start /WAIT devenv pthread.vs2005.test.vcproj /Rebuild DEBUGDLL
	start /WAIT devenv pthread.vs2005.test.vcproj /Rebuild RELEASEDLL
)

cd ..



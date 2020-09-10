rem Run_Single_Config_Test debug|release
rem Run the all tests for a single configuation ( i.e debug , release )

call CallSetEnv.bat

cd %TEST_DIR%/%1

echo off

set /a TEST_COUNT=0
set /a FAIL_COUNT=0

for %%f in (*.exe) do ( 
	%%f

	if errorlevel 1 ( 
		echo %%f FAILED 
		set /a FAIL_COUNT=FAIL_COUNT+1
	) else ( 
		echo %%f PASSED )

	set /a TEST_COUNT=TEST_COUNT+1
)

echo %1 fail %FAIL_COUNT% of %TEST_COUNT%

cd ..\..

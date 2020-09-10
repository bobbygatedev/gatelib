rem prepare the output tree in ../../output override env var for modify output root directory.

set PrepareOutputTreeDIR=%cd%

call SetEnv.bat

md %G_OUTPUT%
md %G_BIN%
md %G_LIB%
md %G_OUTPUT%\tests
md %G_INTERMEDIATE%
md %G_OUTPUT%\eclipse_workspace

cd %G_ROOT%\3rdparty\Win32\pthreads\dev

copy %G_ROOT%\support\java\GLibJSupport.jar %G_BIN%

call Build_Lib.bat

cd %PrepareOutputTreeDIR%

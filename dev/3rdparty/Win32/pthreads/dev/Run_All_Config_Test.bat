rem Runs tests for all configs ( both debug and release ).

call Run_Single_Config_Test.bat release 2> err_release.txt
call Run_Single_Config_Test.bat debug   2> err_debug.txt

rem prepares the pthreads lib and copies them to lib/bin directories.

call CallSetEnv.bat

cd pthread.vs2005

make clean GC-static
move libpthreadGC2.a %G_LIB%\libpthreadGC2_static.a
make clean
del *.stamp


make clean GC-static-debug
move libpthreadGC2d.a %G_LIB%\libpthreadGC2_staticD.a
make clean
del *.stamp


make clean GC-inlined
move libpthreadGC2.a %G_LIB%
move pthreadGC2.dll %G_BIN%
make clean
del *.stamp


make clean GC-inlined-debug
move libpthreadGC2d.a %G_LIB%
move pthreadGC2d.dll %G_BIN%
make clean
del *.stamp


start /WAIT devenv pthread.vs2005.vcproj /Rebuild DEBUG
start /WAIT devenv pthread.vs2005.vcproj /Rebuild RELEASE
start /WAIT devenv pthread.vs2005.vcproj /Rebuild DEBUGDLL
start /WAIT devenv pthread.vs2005.vcproj /Rebuild RELEASEDLL

cd ..
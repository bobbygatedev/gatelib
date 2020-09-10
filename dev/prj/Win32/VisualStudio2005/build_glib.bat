rem prepares the pthreads lib and copies them to lib/bin directories.

call ../SetEnv.bat

start /WAIT devenv ".\glib_compatibility\glib_compatibility.vcproj" /Rebuild DEBUG
start /WAIT devenv ".\glib_compatibility\glib_compatibility.vcproj" /Rebuild RELEASE
start /WAIT devenv ".\glib_compatibility\glib_compatibility.vcproj" /Rebuild DEBUGDLL
start /WAIT devenv ".\glib_compatibility\glib_compatibility.vcproj" /Rebuild RELEASEDLL

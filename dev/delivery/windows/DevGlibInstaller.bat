call ..\..\prj\Win32\SetEnv

md %G_INTERMEDIATE%
echo on

copy GLibInstaller\GLibInstaller.sln %G_INTERMEDIATE%
start devenv %G_INTERMEDIATE%\GLibInstaller.sln



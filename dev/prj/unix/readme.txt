In order to configure source test, the file .bashrc in user home folder must be edited as follows:
- append the following lines at the end of the file ~.bashrc:

#call glibenv.sh for configuring source code environment 
cd ~/glib/dev/prj/unix
. ./glibenv.sh
cd ~

this operation will offer to the developer the following commands:
           
glcdprj:              changes current directory to unix project folder.
glcdsource:           changes current directory sourcde code folder.
glcdtest:             changes current directory to test output directory
glmake [path] [conf]: switch to dir [path] then make the (fullname) configuration [conf]
glbuildlib [conf]:    compiles project glib_compatibility in the configuration [conf]
glbuildliball:        compiles project glib_compatibility in the all configurations
glbuildtest [conf]:   compiles project glib_compatibility_tester in the configuration [conf]
glbuildtestall:       compiles project glib_compatibility_tester in the all configurations
gleclipse:            launches eclipse in workspace directory (if installed)
glprepareoutputtree:  must be launched after a fresh installation of the development environment and prepares
	 the output three in G_ROOT/../unix_output

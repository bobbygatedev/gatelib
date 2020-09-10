#!/bin/bash

get_abs_path()
{ 
	echo $(readlink -f "$1")
}

#set G_ROOT 
export G_ROOT=$(get_abs_path $(pwd)/../..)

function glset()
{
	#directories for development only
	export G_OUTPUT=$(get_abs_path $G_ROOT/../unix_output)
	export G_INTERMEDIATE=$G_OUTPUT/intermediate
	export G_PROJECT=$G_ROOT/prj/unix
	#settings for ubuntu 32 bit setting for 
	export G_JAVA_INCLUDE=/usr/lib/jvm/java-7-openjdk-i386/include
	export G_JAVA_SHARED_OBJECT=/usr/lib/jvm/java-7-openjdk-i386/jre/lib/i386/client/libjvm.so	
	
	#installation directories
	export G_INCLUDE=$G_ROOT/include
	export G_SOURCE=$G_ROOT/source
	export G_BIN=$G_OUTPUT/bin
	export G_LIB=$G_OUTPUT/lib
	export G_JAR_PATH=$G_BIN/GLibJSupport.jar
	export G_ASSIST="java -classpath $G_JAR_PATH g.assist.CmdLineAssist"
	
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$G_BIN
}

function glcdprj()
{
	glset
	cd $G_ROOT/prj/unix
} 

function glcdoutput()
{
	glset
	cd $G_OUTPUT
} 

function glcdsource()
{
	glset
	cd $G_ROOT/source
} 

function glcdtest()
{
	glset
	cd $G_OUTPUT/tests
}

#action call a project inside x/prj/unix/gcc
#1: path of the project
#2: (full) configuration name
function glmake()
{
	glset
	tmp=$(pwd)
	cd $1
	make $2
	cd $tmp
}

#build the library
#configuration name (without 'build_' i.e. 'glbuildlib debug' )
function glbuildlib()
{
	glmake $G_ROOT/prj/unix/gcc/glib_compatibility build_$1
}

function glbuildliball()
{
	glmake $G_ROOT/prj/unix/gcc/glib_compatibility buildall
}

function glbuildtestall()
{
	glmake $G_ROOT/prj/unix/gcc/glib_compatibility_tester buildall
}

function glbuildtest()
{
	glmake $G_ROOT/prj/unix/gcc/glib_compatibility_tester build_$1
}
function glcleantest()
{
	glmake $G_ROOT/prj/unix/gcc/glib_compatibility_tester clean_$1
}

function glcleanliball()
{
	glmake $G_ROOT/prj/unix/gcc/glib_compatibility cleanall
}

function glcleanlib()
{
	glmake $G_ROOT/prj/unix/gcc/glib_compatibility clean_$1
}

function glcleantestall()
{
	glmake $G_ROOT/prj/unix/gcc/glib_compatibility_tester cleanall
}

function gleclipse()
{
	glset
	/opt/eclipse/eclipse -nosplash -application org.eclipse.cdt.managedbuilder.core.headlessbuild -data $G_OUTPUT/eclipse_workspace -import $G_PROJECT/eclipse/glib_compatibility	
	/opt/eclipse/eclipse -nosplash -application org.eclipse.cdt.managedbuilder.core.headlessbuild -data $G_OUTPUT/eclipse_workspace -import $G_PROJECT/eclipse/glib_compatibility_tester	
	/opt/eclipse/eclipse -nosplash -data $G_OUTPUT/eclipse_workspace&
}

function glprepareoutputtree()
{
	glset
	echo output=$G_OUTPUT
	
	mkdir -p $G_OUTPUT
	mkdir -p $G_OUTPUT/bin
	mkdir -p $G_OUTPUT/lib
	mkdir -p $G_OUTPUT/tests
	mkdir -p $G_OUTPUT/intermediate
	mkdir -p $G_OUTPUT/eclipse_workspace
	cp $G_ROOT/support/java/GLibJSupport.jar $G_BIN/
}

function glpreparedelivery()
{
	this_dir=$(pwd)
	glset

	echo deleting output folder	
	rm -f -r $G_OUTPUT

	echo "(re)prepare output tree"
	glprepareoutputtree

	glbuildliball

	echo creating fresh delivery tree
	mkdir -p $G_OUTPUT/delivery/include

	echo copy bin to delivery bin
	cp -f -r $G_OUTPUT/bin/ $G_OUTPUT/delivery

	echo copy lib to delivery lib
	cp -f -r $G_OUTPUT/lib/ $G_OUTPUT/delivery

	echo copy include/common to delivery include
	cp -f -r $G_ROOT/include/common $G_OUTPUT/delivery/include 

	echo copy include/unix to delivery include
	cp -f -r $G_ROOT/include/unix $G_OUTPUT/delivery/include

	echo copying linux delivery files
	cp -f -r $G_ROOT/delivery/linux/* $G_OUTPUT/delivery

	echo changing mode permssions of install and uninstall
	chmod -v 555 $G_OUTPUT/delivery/install.sh
	chmod -v 555 $G_OUTPUT/delivery/uninstall.sh

	echo "creating tar.gz file"
	cd $G_OUTPUT
	tar -cvzf delivery.tar.gz delivery
	rm -f -r $G_OUTPUT/delivery
	cd $this_dir
}


#TODO TEMP
function glprjfromsample()
{
	#creating a new project from samples in current dir
	this_dir=$(pwd)
	cd 
	chmod -v -R 777 ./GLibSampleProject
	cp -f -r /opt/gatelib/current/samples/gcc/GLibSampleProject .
	
	cd $this_dir
}

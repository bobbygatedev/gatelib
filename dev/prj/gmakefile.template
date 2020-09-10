#########################################################################################################################
#INSTRUCTIONS:
#- create a makefile called settings_common with project common settings and file list (configuration-independent)
#	setting common contains usually file list plus common settings (lib dir, libs,..
#  1) use variables INCLUDE_DIRS for listing include dirs (separated by space don't put option -I)
#  2) use SOURCE_DIRS to define directories where files are located or SOURCES if you want to insert a list of source file
#  3) under windows is advisable to use $(realpath ..) for specifying paths (INCLUDE_DIRS,SOURCE_DIRS,SOURCES)
#  4) optionally specify a OBJECTS_DIR otherwise it will be ./$(CONF), var CONF is used for specifying configuration
#  5) a PROJECT_NAME MUST be set 
#
#- create a makefile called settings_[conf] with configuration specific settings for the single configurations
#- i.e. settings_debug will define a configuration called debug with following commands 
#	-build_debug,clean_debug (or make build/clean CONF=debug)
#	-use buildall,cleanall for buiding/cleaning all configuration
#- if you don't intend to use build configuation, you can use directly build/clean
#
#
# OPTIONS USAGE (single config settings will override common settings
#	- a shared object must specify -shared in CFLAGS
#	- use CFLAGS for specify compiling flags(excluded defines which use DEFINES)
#	- use DEFINES for defininig symbols i.e. DEFINES LABEL1=1 LABEL2=2 OPTION
#	- LIBS indicate the list of libraries with or without full path (separated by space don't put option -l)
#	- LIB_DIRS separated by space(separated by space don't put option -L) indicate the list of libraries (with or without full path)
#	- OUTPUT_PREFIX,OUTPUT_PREFIX,OUTPUT_EXT specify the suffix,prefix and the extension for the output file name, that is by default:
#		OUTPUT_NAME=[OUTPUT_PREFIX]PROJECT_NAME[OUTPUT_SUFFIX][OUTPUT_EXT]
#	- OUTPUT_EXT specify the extension with det dot (i.e .dll), specifyong .a will cause automatically the use of 
#		the archiver (ar) instaed of linker(g++).
#       - LINKER_OPTIONS represent the linker option use -shared if you want to compile a shared object/DLL.
#  
#########################################################################################################################


OPTIMIZED_CFLAGS   = -O3 -Wall -fmessage-length=0
NOOPTIMIZED_CFLAGS = -O0 -g3 -w -fmessage-length=0

include settings_common

CC                := g++

ifdef CONF
include settings_$(CONF)
endif

ifndef ARCHIVER_OPTIONS
	ARCHIVER_OPTIONS = -rcs
endif

ifndef OUTPUT_NAME
	OUTPUT_NAME = $(addprefix $(OUTPUT_PREFIX),$(addsuffix $(OUTPUT_EXT),$(addsuffix $(OUTPUT_SUFFIX),$(PROJECT_NAME))))
endif

ifndef OUTPUT_DIR
	OUTPUT_DIR = ./$(CONF)
endif

ifndef OUTPUT_PATH
	OUTPUT_PATH=$(OUTPUT_DIR)/$(OUTPUT_NAME)
endif

ifndef OBJECTS_DIR
	OBJECTS_DIR= $(OUTPUT_DIR)
endif

ifndef PRECOMPILE_DIR
	PRECOMPILE_DIR=$(OBJECTS_DIR)
endif


CFLAGS += $(addprefix -D",$(addsuffix ",$(DEFINES)))  

ifndef INCLUDES

INCLUDES = \
	$(realpath \
	$(foreach dir,$(INCLUDE_DIRS),$(wildcard $(dir)/*.h)) \
	) \
	
endif	

ifndef SOURCES
SOURCES = \
	$(realpath \
	$(foreach dir,$(SOURCE_DIRS),$(wildcard $(dir)/*.cpp)) \
	) \
	
endif


ifndef SOURCE_DIRS
SOURCES = \
	$(realpath $(foreach fil,$(SOURCES),$(dir $(fil))) ) 	
endif

PRECOMPILE_CPPS=$(addprefix $(PRECOMPILE_DIR)/,$(patsubst %.cpp,%.cpp.txt,$(notdir $(SOURCES))))
PRECOMPILE_HS=$(addprefix $(PRECOMPILE_DIR)/,$(patsubst %.h,%.h.txt,$(notdir $(INCLUDES))))

precompile: $(PRECOMPILE_DIR) $(PRECOMPILE_CPPS) $(PRECOMPILE_HS)

tst:
	@echo $(PRECOMPILE_CPPS)
	@echo $(PRECOMPILE_HS)
	@echo PRECOMPILE_DIR $(PRECOMPILE_DIR)
	@echo SOURCE_DIRS $(SOURCE_DIRS)
	@echo INCLUDE_DIRS $(INCLUDE_DIRS)

$(OBJECTS_DIR):
	mkdir -p $(OBJECTS_DIR)

ifneq "$(OBJECTS_DIR)" "$(PRECOMPILE_DIR)"
$(PRECOMPILE_DIR):
	mkdir -p $(PRECOMPILE_DIR)
endif

VPATH_DIR_H=$(foreach dir,$(INCLUDE_DIRS),$(dir):).

#VPATH WORKING JUST FOR .c/.cpp files
vpath %.h $(foreach dir,$(INCLUDE_DIRS),$(dir):).
$(PRECOMPILE_DIR)/%.h.txt: %.h
	$(G_ASSIST) -p -v1 $< -or $@

VPATH = $(foreach dir,$(SOURCE_DIRS),$(dir):).
$(PRECOMPILE_DIR)/%.cpp.txt: %.cpp
	$(G_ASSIST) -p -v1 $< -or $@
 
clean_precompile:
	rm -f -r $(PRECOMPILE_CPPS)
	rm -f -r $(PRECOMPILE_HS)

precompile:$(OBJECTS_DIR) $(PRECOMPILE_CPPS) $(PRECOMPILE_HS)

.PHONY: clean
clean: clean_precompile
	@echo cleaning $(PROJECT_NAME) ..	
	rm -f $(OUTPUT_PATH)
	rm -f -r $(OBJECTS_DIR)

clean_%:
	$(MAKE) clean CONF=$(patsubst clean_%,%,$@)

cleanall: clean_debug clean_release clean_debug_static clean_release_static

build_%:precompile
	$(MAKE) build CONF=$(patsubst build_%,%,$@)
	
buildall: build_debug build_release build_debug_static build_release_static
	
OBJECTS = $(addprefix $(OBJECTS_DIR)/,$(patsubst %.cpp,%.o,$(notdir $(SOURCES))))

INCLUDE_OPT  = $(addprefix -I",$(addsuffix ",$(realpath $(INCLUDE_DIRS))))
#option for libraries and library path
LIBS_OPT     = $(addprefix -L",$(addsuffix ",$(realpath $(LIB_DIRS)))) $(addprefix -l",$(addsuffix ",$(LIBS)))

compile: $(OBJECTS_DIR) $(OBJECTS) 

#compile rule
VPATH = $(foreach dir,$(SOURCE_DIRS),$(dir):).
$(OBJECTS_DIR)/%.o : %.cpp $(INCLUDES)
	g++ $(CFLAGS) -pipe $(INCLUDE_OPT) -c $< -o $@
	
$(OUTPUT_PATH) : compile
ifeq ($(OUTPUT_EXT),.a)
	ar $(ARCHIVER_OPTIONS) $(OUTPUT_PATH) $(OBJECTS)
else	
	g++ $(LINKER_OPTIONS) -pipe $(OBJECTS) $(LIBS_OPT) -o $(OUTPUT_PATH)
endif	

#requisite of output file(.exe,.so,.dll,etc)
build : $(OUTPUT_PATH)
		
ECHO_%:
	@echo $($(patsubst ECHO_%,%,$@)) 

ECHO_SETTINGS:
	@echo options $(CFLAGS)
	@echo libs $(LIBS)
	@echo libs dir $(LIB_DIRS)
	@echo output dir $(OUTPUT_DIR)
	@echo objects dir $(OBJECTS_DIR)
	
	
		
		
##
# Edit the following variables to tune your project
##

PACKAGES_NEEDED =    \
    g++              \
    git subversion   \
    cmake            \
    autoconf libtool \
    graphviz doxygen \
    gperf flex bison libbison-dev libncurses5-dev

##
# END OF EDIT/CUSTOMISATION AREA
##

SHELL = /bin/bash
MAKE = make

# To debug the build system: remove the following variables:
V = @
MAKEOPT  = --no-print-directory V=$(V)

# Try to include the environment variables:
-include ./Malefile.var
# if not present, then define them
ifneq ($(PROJECTDIR), ${PWD})
  PROJECTDIR=${PWD}
  SOURCEDIR =$(PROJECTDIR)/src
  INCLUDEDIR=$(PROJECTDIR)/src/include
  SCRIPTSDIR=$(PROJECTDIR)/scripts
  LIBSDIR=$(PROJECTDIR)/lib
  REPOSDIR=$(PROJECTDIR)/repo
  BUILDDIR=$(PROJECTDIR)/build
  MODULESDIR=$(BUILDDIR)/modules
  DOCDIR=$(PROJECTDIR)/doc
  BUILDCONFIGDIR=$(PROJECTDIR)/include/config
  BUILDCONFIGFILE=$(BUILDCONFIGDIR)/auto.conf
  PROJECT_CONFIG=$(PROJECTDIR)/Makefile.var

  DOXYFILE = $(DOCDIR)/Doxyfile
  CONFIG_HEADER_FILE=$(INCLUDEDIR)/config.hh

  CPPFLAGS  = -W -Wall -Wextra
  CPPFLAGS += -Werror
  CPPFLAGS += -std=c++11 # -g -ggdb3
  CPPFLAGS += -I$(INCLUDEDIR) -I$(LIBSDIR)/include

  LDFLAGS  = -std=c++11
  LDFLAGS += -L$(LIBSDIR)/lib
endif

# Build the project:
all: all-build

##
# INITIALIZATION
# At the first build (or target "init") create directories and set environment
# variables:
##

# Create directories if not present:
$(BUILDDIR):
	$(V)mkdir -p $@
$(MODULESDIR):
	$(V)mkdir -p $@
$(LIBSDIR):
	$(V)mkdir -p $@

INIT_ENV: $(PROJECT_CONFIG) $(BUILDDIR) $(MODULESDIR) $(LIBSDIR)

# Create the project's configuration file.
$(PROJECT_CONFIG):
	@echo "PROJECTDIR=${PWD}" > $(PROJECT_CONFIG)
	@echo "SOURCEDIR =$(SOURCEDIR)" >> $(PROJECT_CONFIG)
	@echo "INCLUDEDIR=$(INCLUDEDIR)" >> $(PROJECT_CONFIG)
	@echo "SCRIPTSDIR=$(SCRIPTSDIR)" >> $(PROJECT_CONFIG)
	@echo "LIBSDIR=$(LIBSDIR)" >> $(PROJECT_CONFIG)
	@echo "REPOSDIR=$(REPOSDIR)" >> $(PROJECT_CONFIG)
	@echo "DOCDIR=$(DOCDIR)" >> $(PROJECT_CONFIG)
	@echo "BUILDCONFIGDIR=$(BUILDCONFIGDIR)" >> $(PROJECT_CONFIG)
	@echo "BUILDCONFIGFILE=$(BUILDCONFIGFILE)" >> $(PROJECT_CONFIG)
	@echo "" >> $(PROJECT_CONFIG)
	@echo "CPPFLAGS=$(CPPFLAGS)" >> $(PROJECT_CONFIG)
	@echo "LDFLAGS=$(LDFLAGS)" >> $(PROJECT_CONFIG)
	@echo "" >> $(PROJECT_CONFIG)
	@echo "BUILDDIR=$(BUILDDIR)" >> $(PROJECT_CONFIG)
	@echo "MODULESDIR=$(MODULESDIR)" >> $(PROJECT_CONFIG)
	@echo "" >> $(PROJECT_CONFIG)
	@echo "DOXYFILE=$(DOXYFILE)" >> $(PROJECT_CONFIG)

# include the build config file (generated by Kconfig)
-include $(BUILDCONFIGFILE)

# if the build config file is not available, then set the default variable
# name:
ifeq ($(CONFIG_PROJECT_NAME), )
  #the default name
  CONFIG_PROJECT_NAME = lxcmanager
endif

# Include custom Make's functions
include $(SCRIPTSDIR)/Makefile.functions

# The tarball will be created under that name
PROJECT_TARNAME = $(CONFIG_PROJECT_NAME)-v$(CONFIG_PROJECT_VERSION)
PROJECT_TARTYPE = tar.bz2
TARBALL= $(PROJECT_TARNAME).$(PROJECT_TARTYPE)

##
# Library/tools dependancies:
##

# create missing directories:
$(REPOSDIR):
	$(V)mkdir -p $@
$(BUILDCONFIGDIR):
	$(V)mkdir -p $@

# init target (initialize all of the project)
init: INIT_REPOS

# init the repositories (fetch the different libraries and build them)
INIT_REPOS: $(REPOSDIR) $(BUILDCONFIGDIR) DEPENDANCES
	$(V)$(MAKE) $(MAKEOPT) -C $(REPOSDIR) init

update-repos:
	$(V)$(MAKE) $(MAKEOPT) -C $(REPOSDIR) build

##
# BUILDING
##

all-build: INIT_REPOS .config $(CONFIG_HEADER_FILE)
	$(V)$(MAKE) $(MAKEOPT) -C $(SOURCEDIR) all

# Check the dependancies
DEPENDANCES: INIT_ENV
	$(V)for package in $(PACKAGES_NEEDED); do \
	  $(call check_package,$${package}) ; done

##
# KCONFIG
##

KCONFIG_OPTIONS  = LD_LIBRARY_PATH="$(SCRIPTSDIR)/kconfig/lib"
KCONFIG_OPTIONS += KCONFIG_AUTOHEADER="$(SOURCEDIR)/include/config.hh"

menuconfig: INIT_REPOS
	$(V)$(KCONFIG_OPTIONS) $(SCRIPTSDIR)/kconfig/bin/kconfig-mconf Kconfig
	$(V)$(KCONFIG_OPTIONS) $(SCRIPTSDIR)/kconfig/bin/kconfig-conf --silentoldconfig Kconfig

oldconfig: INIT_REPOS .config
	$(V)$(KCONFIG_OPTIONS) $(SCRIPTSDIR)/kconfig/bin/kconfig-conf --silentoldconfig Kconfig

defconfig: INIT_REPOS
	$(V)$(KCONFIG_OPTIONS) $(SCRIPTSDIR)/kconfig/bin/kconfig-conf --alldefconfig Kconfig
	$(V)$(KCONFIG_OPTIONS) $(SCRIPTSDIR)/kconfig/bin/kconfig-conf --silentoldconfig Kconfig

.config:
	@echo ""
	@echo " First: use make menuconfig OR make defconfig"
	@echo ""
	@exit 1

##
# HELP
##

help:
	@echo ""
	@echo "$(PROJECTNAME)'s Makefile targets help:"
	@echo ""
	@echo " -BUILDING-"
	@echo ""
	@echo "init:    initialize the build system"
	@echo ""
	@echo "all:     compile the project and place the binary in"
	@echo "         $(BINDIR)/$(PROJECTNAME)"
	@echo "         and the plugin in"
	@echo "         $(MODULESDIR)"
	@echo ""
	@echo "doc:     generate the documentation of the source project in"
	@echo "         $(DOCDIR)"
	@echo ""
	@echo " -CONFIGURATION-"
	@echo ""
	@echo "menuconfig: use ncurse based user interface to configure specific"
	@echo "            build rules"
	@echo ""
	@echo "defconfig:  generate the default configuration file"
	@echo ""
	@echo "oldconfig:  re-generate the previous configuration file if any"
	@echo ""
	@echo " -RELEASING-"
	@echo ""
	@echo "dist:    generate a project tarball: $(TARBALL)"
	@echo ""
	@echo " -CLEANING-"
	@echo ""
	@echo "clean: remove compilation files"
	@echo ""
	@echo "distclean: clean the project (target clean) and remove generated files"
	@echo ""

##
# DOCUMENTATION
##

.PHONY: doc
doc: INIT_ENV
	$(V)$(MAKE) $(MAKEOPT) -C $(DOCDIR) doc

##
# RELEASING
##

dist: distclean $(PROJECT_TARTYPE)

tar.bz2:
	tar -cvjf /tmp/$(TARBALL) --exclude=$(TARBALL) --exclude=.git ../`basename ${PWD}` \
	  && mv /tmp/$(TARBALL) .

##
# CLEANING
##

clean: $(SOURCEDIR) INIT_ENV
	$(V)$(MAKE) $(MAKEOPT) -C $(SOURCEDIR) $@
	$(V)$(MAKE) $(MAKEOPT) -C $(REPOSDIR) $@
	$(V)$(MAKE) $(MAKEOPT) -C $(DOCDIR) $@
	$(V)$(call remove_file,$(PROJECTDIR)/.config)
	$(V)$(call remove_file,$(PROJECTDIR)/.config.old)
	$(V)$(call remove_file,$(BUILDDIR)/$(CONFIG_PROJECT_NAME))
	$(V)$(call remove_file,$(CONFIG_HEADER_FILE))

distclean: clean
	$(V)$(MAKE) $(MAKEOPT) -C $(SOURCEDIR) $@
	$(V)$(MAKE) $(MAKEOPT) -C $(REPOSDIR) $@
	$(V)$(MAKE) $(MAKEOPT) -C $(DOCDIR) $@
	$(V)$(call remove_file,$(TARBALL))
	$(V)$(call remove_dir,$(LIBSDIR))
	$(V)$(call remove_dir,$(BUILDDIR))
	$(V)$(call remove_dir,$(SCRIPTSDIR)/kconfig)
	$(V)$(call remove_dir,$(PROJECTDIR)/include)
	$(V)$(call remove_file,$(PROJECT_CONFIG))

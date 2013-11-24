##
# Edit the following variables to tune your project
##

PACKAGES_NEEDED =    \
    g++              \
    git subversion   \
    cmake            \
    graphviz doxygen \
    gperf flex bison libbison-dev libncurses5-dev

##
# END OF EDIT/CUSTOMISATION AREA
##

V = @
MAKEOPT  = --no-print-directory V=$(V)

-include ./Malefile.var

ifneq ($(PROJECTDIR), ${PWD})
  PROJECTDIR=${PWD}
  SOURCEDIR =$(PROJECTDIR)/src
  INCLUDEDIR=$(PROJECTDIR)/src/include
  SCRIPTSDIR=$(PROJECTDIR)/scripts
  LIBSDIR=$(PROJECTDIR)/lib
  REPOSDIR=$(PROJECTDIR)/repo
  BUILDDIR=$(PROJECTDIR)/build
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

  LDFLAGS = -std=c++11
  # BOOST libraries
  #   TODO: If you want to add a specific boost library do not forget to update
  #   the handle_library_scripts.sh (in $(SCRIPTSDIR)) in order to build the
  #   required library
  LDFLAGS += -static -L$(LIBSDIR)/lib -lboost_program_options
endif

all: all-build

$(BUILDDIR):
	$(V)mkdir -p $(BUILDDIR)
$(LIBSDIR):
	$(V)mkdir -p $@

INIT_ENV: $(PROJECT_CONFIG) $(BUILDDIR) $(LIBSDIR)

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
	@echo "" >> $(PROJECT_CONFIG)
	@echo "DOXYFILE=$(DOXYFILE)" >> $(PROJECT_CONFIG)

-include $(BUILDCONFIGFILE)

ifeq ($(CONFIG_PROJECT_NAME), )
  #the default name
  CONFIG_PROJECT_NAME = lxcmanager
endif

include $(SCRIPTSDIR)/Makefile.functions

# The tarball will be created under that name
PROJECT_TARNAME = $(CONFIG_PROJECT_NAME)-v$(CONFIG_PROJECT_VERSION)
PROJECT_TARTYPE = tar.bz2
TARBALL= $(PROJECT_TARNAME).$(PROJECT_TARTYPE)

$(REPOSDIR):
	$(V)mkdir -p $@
$(BUILDCONFIGDIR):
	$(V)mkdir -p $@

INIT_REPOS: $(REPOSDIR) $(BUILDCONFIGDIR)
	$(V)make $(MAKEOPT) -C $(REPOSDIR) init

update-repos:
	$(V)make $(MAKEOPT) -C $(REPOSDIR) build

# BUILDING

all-build: DEPENDANCES INIT_REPOS .config $(CONFIG_HEADER_FILE)
	$(V)make $(MAKEOPT) -C $(SOURCEDIR) all

DEPENDANCES: INIT_ENV
	$(V)for package in $(PACKAGES_NEEDED); do \
	  $(call check_package,$${package}) ; done

KCONFIG_OPTIONS  = LD_LIBRARY_PATH="$(SCRIPTSDIR)/kconfig/lib"
KCONFIG_OPTIONS += KCONFIG_AUTOHEADER="$(SOURCEDIR)/include/config.hh"

menuconfig: INIT_REPOS
	$(V)$(KCONFIG_OPTIONS) $(SCRIPTSDIR)/kconfig/bin/kconfig-mconf src/Kconfig
	$(V)$(KCONFIG_OPTIONS) $(SCRIPTSDIR)/kconfig/bin/kconfig-conf --silentoldconfig src/Kconfig

oldconfig: INIT_REPOS .config
	$(V)$(KCONFIG_OPTIONS) $(SCRIPTSDIR)/kconfig/bin/kconfig-conf --silentoldconfig src/Kconfig

defconfig: INIT_REPOS
	$(V)$(KCONFIG_OPTIONS) $(SCRIPTSDIR)/kconfig/bin/kconfig-conf --alldefconfig src/Kconfig
	$(V)$(KCONFIG_OPTIONS) $(SCRIPTSDIR)/kconfig/bin/kconfig-conf --silentoldconfig src/Kconfig

.config:
	@echo "ERROR: no .config file. Use make menuconfig OR make defconfig"
	@exit 1

help:
	@echo ""
	@echo "$(PROJECTNAME)'s Makefile targets help:"
	@echo ""
	@echo " -BUILDING-"
	@echo ""
	@echo "build:   generate compilation and documentation"
	@echo ""
	@echo "all:     compile the project and place the binary in"
	@echo "         $(BINDIR)/$(PROJECTNAME)"
	@echo ""
	@echo "doc:     generate the documentation of the source project in"
	@echo "         $(DOCDIR)"
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

# DOCUMENTATION
.PHONY: doc
doc: DEPENDANCES
	$(V)make $(MAKEOPT) -C $(DOCDIR) doc

# RELEASING

dist: distclean $(PROJECT_TARTYPE)

tar.bz2:
	tar -cvjf /tmp/$(TARBALL) --exclude=$(TARBALL) --exclude=.git ../`basename ${PWD}` \
	  && mv /tmp/$(TARBALL) .

# CLEANING

clean: $(SOURCEDIR) INIT_ENV
	$(V)make $(MAKEOPT) -C $(SOURCEDIR) $@
	$(V)make $(MAKEOPT) -C $(REPOSDIR) $@
	$(V)make $(MAKEOPT) -C $(DOCDIR) $@
	$(V)$(call remove_file,$(PROJECTDIR)/.config)
	$(V)$(call remove_file,$(PROJECTDIR)/.config.old)
	$(V)$(call remove_file,$(BUILDDIR)/$(CONFIG_PROJECT_NAME))
	$(V)$(call remove_file,$(CONFIG_HEADER_FILE))

distclean: clean
	$(V)make $(MAKEOPT) -C $(SOURCEDIR) $@
	$(V)make $(MAKEOPT) -C $(REPOSDIR) $@
	$(V)make $(MAKEOPT) -C $(DOCDIR) $@
	$(V)$(call remove_file,$(TARBALL))
	$(V)$(call remove_dir,$(LIBSDIR))
	$(V)$(call remove_dir,$(BUILDDIR))
	$(V)$(call remove_dir,$(SCRIPTSDIR)/kconfig)
	$(V)$(call remove_dir,$(PROJECTDIR)/include)
	$(V)$(call remove_file,$(PROJECT_CONFIG))

##
# Edit the following variables to tune your project
##

PACKAGES_NEEDED=g++ git subversion cmake graphviz doxygen gperf

EXTRA_CPPFLAGS = -std=c++11 # -g -ggdb3
EXTRA_LDFLAGS = -std=c++11
# END OF EDIT/CUSTOMISATION AREA
##

PROJECTDIR=${PWD}
SOURCEDIR =$(PROJECTDIR)/src
INCLUDEDIR=$(PROJECTDIR)/src/include
SCRIPTSDIR=$(PROJECTDIR)/scripts
LIBSDIR=$(PROJECTDIR)/lib
REPOSDIR=$(PROJECTDIR)/repo
BINDIR=$(PROJECTDIR)/bin
DOCDIR=$(PROJECTDIR)/doc
BUILDCONFIGDIR=$(PROJECTDIR)/include/config

-include $(BUILDCONFIGDIR)/auto.conf

# The installation directory
INSTALLDIR = /usr/local
BININSTALLDIR = $(INSTALLDIR)/bin
INCLUDEINSTALLDIR = $(INSTALLDIR)/include/$(CONFIG_PROJECT_NAME)
LIBINSTALLDIR = $(INSTALLDIR)/lib
DOCINSTALLDIR = $(INSTALLDIR)/doc

# The tarball will be created under that name
PROJECT_TARNAME = $(CONFIG_PROJECT_NAME)-v$(CONFIG_PROJECT_VERSION)
# The archiving type tar.bz2
PROJECT_TARTYPE = tar.bz2

DOXYFILE = $(DOCDIR)/Doxyfile
CONFIG_HEADER_FILE=$(INCLUDEDIR)/config.hh

CPPFLAGS  = -W -Wall -Wextra
CPPFLAGS += -Werror
CPPFLAGS += $(EXTRA_CPPFLAGS)
CPPFLAGS += -I$(INCLUDEDIR) -I$(LIBSDIR)/include

LDFLAGS = $(EXTRA_LDFLAGS)
# BOOST libraries
#   TODO: If you want to add a specific boost library do not forget to update
#   the handle_library_scripts.sh (in $(SCRIPTSDIR)) in order to build the
#   required library
LDFLAGS += -static -L$(LIBSDIR)/lib -lboost_program_options

MAKEOPT  = #--no-print-directory
MAKEOPT += CPPFLAGS="$(CPPFLAGS)"
MAKEOPT += PROJECTDIR="$(PROEJCTDIR)"
MAKEOPT += DOCDIR="$(DOCDIR)"
MAKEOPT += LDFLAGS="$(LDFLAGS)"
MAKEOPT += CONFIG_PROJECT_NAME="$(BINDIR)/$(CONFIG_PROJECT_NAME)"
MAKEOPT += CONFIG_HEADER_FILE="$(CONFIG_HEADER_FILE)"
MAKEOPT += SCRIPTSDIR="$(SCRIPTSDIR)"
MAKEOPT += DOCDIR="$(DOCDIR)"
MAKEOPT += DOXYFILE="$(DOXYFILE)"

TARBALL= $(PROJECT_TARNAME).$(PROJECT_TARTYPE)

include $(SCRIPTSDIR)/Makefile.functions

V ?= @

# BUILDING

all: DEPENDANCES INIT $(SOURCEDIR) .config $(CONFIG_HEADER_FILE)
	$(V)make $(MAKEOPT) -C $(SOURCEDIR) all

build: doc all

DEPENDANCES:
	$(V)for package in $(PACKAGES_NEEDED); do \
	  $(call check_package,$${package}) ; done

update:
	$(V)if [ -f "$(PROJECTDIR)/.init" ]; then \
	   PROJECTDIR=$(PROJECTDIR) \
	     $(SCRIPTSDIR)/handle_library_dependancies.sh build ; \
	 fi

$(DOXYFILE):
	$(SCRIPTSDIR)/generate_doxyfile.sh $(SOURCEDIR) $(DOCDIR) $(DOXYFILE)

INIT: $(REPOSDIR)
	$(V)if [ ! -d "$(BUILDCONFIGDIR)" ]; then mkdir -p $(BUILDCONFIGDIR); fi
	$(V)if [ ! -f "$(PROJECTDIR)/.init" ]; then \
	   PROJECTDIR=$(PROJECTDIR) \
	     $(SCRIPTSDIR)/handle_library_dependancies.sh build \
	     && touch $(PROJECTDIR)/.init ; \
	 fi

KCONFIG_OPTIONS  = LD_LIBRARY_PATH="$(SCRIPTSDIR)/kconfig/lib"
KCONFIG_OPTIONS += KCONFIG_AUTOHEADER="$(SOURCEDIR)/include/config.hh"

menuconfig: INIT
	$(V)$(KCONFIG_OPTIONS) $(SCRIPTSDIR)/kconfig/bin/kconfig-mconf src/Kconfig
	$(V)$(KCONFIG_OPTIONS) $(SCRIPTSDIR)/kconfig/bin/kconfig-conf --silentoldconfig src/Kconfig

oldconfig:
	$(V)$(KCONFIG_OPTIONS) $(SCRIPTSDIR)/kconfig/bin/kconfig-conf --silentoldconfig src/Kconfig
	$(V)$(KCONFIG_OPTIONS) $(SCRIPTSDIR)/kconfig/bin/kconfig-conf --silentoldconfig src/Kconfig

$(CONFIG_HEADER_FILE): $(INCLUDEDIR)
	$(V)$(KCONFIG_OPTIONS) $(SCRIPTSDIR)/kconfig/bin/kconfig-conf --silentoldconfig src/Kconfig

defconfig:
	$(V)$(KCONFIG_OPTIONS) $(SCRIPTSDIR)/kconfig/bin/kconfig-conf --alldefconfig src/Kconfig
	$(V)$(KCONFIG_OPTIONS) $(SCRIPTSDIR)/kconfig/bin/kconfig-conf --silentoldconfig src/Kconfig
.config:
	$(V)$(KCONFIG_OPTIONS) $(SCRIPTSDIR)/kconfig/bin/kconfig-conf --alldefconfig src/Kconfig
	$(V)$(KCONFIG_OPTIONS) $(SCRIPTSDIR)/kconfig/bin/kconfig-conf --silentoldconfig src/Kconfig

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
	@echo "install: build the project (target all) and install the project"
	@echo "         in the directory $(INSTALLDIR)"
	@echo "         INSTALLDIR (/usr/local by default)"
	@echo "         BININSTALLDIR (INSTALLDIR/bin by default)"
	@echo "         INCLUDEINSTALLDIR (INSTALLDIR/include/$(PROJECTNAME) by default)"
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
doc: $(DOXYFILE) DEPENDANCES
	$(V)make $(MAKEOPT) -C $(DOCDIR) doc

# RELEASING

install: all
	$(V)mkdir -p $(BININSTALLDIR) \
	  && cp $(BINDIR)/$(PROJECTNAME) $(BININSTALLDIR) \
	  || (echo "ERROR: unable to install $(PROJECTNAME) in $(BININSTALLDIR)" && exit 1)
	$(V)mkdir -p $(INCLUDEINSTALLDIR) \
	  && cp -r $(INCLUDEDIR)/* $(INCLUDEINSTALLDIR) \
	  || (echo "ERROR: unable to install headers $(PROJECTNAME) in $(INCLUDEINSTALLDIR)" && exit 1)

dpkg-deb: all

dist: distclean $(PROJECT_TARTYPE)

tar.bz2:
	tar -cvjf /tmp/$(TARBALL) --exclude=$(TARBALL) --exclude=.git ../`basename ${PWD}` \
	  && mv /tmp/$(TARBALL) .

# CLEANING

docclean:
	$(V)make $(MAKEOPT) -C $(DOCDIR) distclean

clean: $(SOURCEDIR)
	$(V)make $(MAKEOPT) -C $(SOURCEDIR) $@
	$(V)$(call remove_file,$(PROJECTDIR)/.config)
	$(V)$(call remove_file,$(PROJECTDIR)/.config.old)
	$(V)$(call remove_file,$(BINDIR)/$(PROJECTNAME))
	$(V)$(call remove_file,$(CONFIG_HEADER_FILE))

distclean: clean docclean
	$(V)$(call remove_file,$(PROJECTDIR)/.init)
	$(V)$(call remove_file,$(TARBALL))
	$(V)$(call clean_dir,$(LIBSDIR))
	$(V)$(call clean_dir,$(BINDIR))
	$(V)$(call remove_dir,$(SCRIPTSDIR)/kconfig)
	$(V)$(call remove_dir,$(PROJECTDIR)/include)
	$(V)$(SCRIPTSDIR)/handle_library_dependancies.sh delete

# MISCS

#	@echo "Error"
#	@. $(PROJECTDIR)/scripts/create_config.sh \
#	  $(PROJECTNAME) $(PROJECTVERSION) $(PROJECTSUBVERSION) \
#	  $(PROJECTREVISION) $@

$(INCLUDEDIR):
	@echo "No include directory at $(INCLUDEDIR)"
$(SOURCEDIR):
	@echo "No sources available at $(SOURCEDIR)"
$(REPOSDIR):
	@echo "No sources available at $(REPOSDIR)"

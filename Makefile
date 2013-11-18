##
# Edit the following variables to tune your project
##

# The project name, the compilation result will be store at the project
# directory (the root of the project) under this name
PROJECTNAME = lxcmanager

#
# The version of the project: Version.Subversion-Revision
PROJECTVERSION = 0
PROJECTSUBVERSION = 1
PROJECTREVISION = 0

PACKAGES_NEEDED=g++ git subversion cmake graphviz doxygen

# The installation directory
INSTALLDIR = /usr/local
BININSTALLDIR = $(INSTALLDIR)/bin
INCLUDEINSTALLDIR = $(INSTALLDIR)/include/$(PROJECTNAME)
LIBINSTALLDIR = $(INSTALLDIR)/lib
DOCINSTALLDIR = $(INSTALLDIR)/doc

# The tarball will be created under that name
PROJECT_TARNAME = $(PROJECTNAME)-v$(PROJECTVERSION)-$(PROJECTSUBVERSION).$(PROJECTREVISION)
# The archiving type tar.bz2
PROJECT_TARTYPE = tar.bz2

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

MAKEOPT  = --no-print-directory
MAKEOPT += CPPFLAGS="$(CPPFLAGS)"
MAKEOPT += PROJECTDIR="$(PROEJCTDIR)"
MAKEOPT += DOCDIR="$(DOCDIR)"
MAKEOPT += LDFLAGS="$(LDFLAGS)"
MAKEOPT += PROJECTNAME="$(BINDIR)/$(PROJECTNAME)"
MAKEOPT += CONFIG_HEADER_FILE="$(CONFIG_HEADER_FILE)"
MAKEOPT += SCRIPTSDIR="$(SCRIPTSDIR)"
MAKEOPT += DOCDIR="$(DOCDIR)"
MAKEOPT += DOXYFILE="$(DOXYFILE)"

TARBALL= $(PROJECT_TARNAME).$(PROJECT_TARTYPE)

include $(SCRIPTSDIR)/Makefile.functions

# BUILDING

all: DEPENDANCES INIT $(SOURCEDIR) $(CONFIG_HEADER_FILE)
	@make $(MAKEOPT) -C $(SOURCEDIR) all

build: doc all

DEPENDANCES:
	@for package in $(PACKAGES_NEEDED); do \
	  $(call check_package,$${package}) ; done

update:
	@if [ -f "$(PROJECTDIR)/.init" ]; then \
	   PROJECTDIR=$(PROJECTDIR) \
	     $(SCRIPTSDIR)/handle_library_dependancies.sh build ; \
	 fi

$(DOXYFILE):
	$(SCRIPTSDIR)/generate_doxyfile.sh $(SOURCEDIR) $(DOCDIR) $(DOXYFILE)

INIT: $(REPOSDIR)
	@if [ ! -f "$(PROJECTDIR)/.init" ]; then \
	   PROJECTDIR=$(PROJECTDIR) \
	     $(SCRIPTSDIR)/handle_library_dependancies.sh build \
	     && touch $(PROJECTDIR)/.init ; \
	 fi

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
	@make $(MAKEOPT) -C $(DOCDIR) doc

# RELEASING

install: all
	@mkdir -p $(BININSTALLDIR) \
	  && cp $(BINDIR)/$(PROJECTNAME) $(BININSTALLDIR) \
	  || (echo "ERROR: unable to install $(PROJECTNAME) in $(BININSTALLDIR)" && exit 1)

	@mkdir -p $(INCLUDEINSTALLDIR) \
	  && cp -r $(INCLUDEDIR)/* $(INCLUDEINSTALLDIR) \
	  || (echo "ERROR: unable to install headers $(PROJECTNAME) in $(INCLUDEINSTALLDIR)" && exit 1)

dpkg-deb: all

dist: distclean $(PROJECT_TARTYPE)

tar.bz2:
	tar -cvjf /tmp/$(TARBALL) --exclude=$(TARBALL) --exclude=.git ../`basename ${PWD}` \
	  && mv /tmp/$(TARBALL) .

# CLEANING

docclean:
	@make $(MAKEOPT) -C $(DOCDIR) distclean

clean: $(SOURCEDIR)
	@make $(MAKEOPT) -C $(SOURCEDIR) $@
	@$(call remove_file,$(BINDIR)/$(PROJECTNAME))
	@$(call remove_file,$(CONFIG_HEADER_FILE))

distclean: clean docclean
	@$(SCRIPTSDIR)/handle_library_dependancies.sh delete
	@$(call remove_file,$(PROJECTDIR)/.init)
	@$(call remove_file,$(TARBALL))
	@$(call clean_dir,$(LIBSDIR))

# MISCS

$(CONFIG_HEADER_FILE): $(INCLUDEDIR)
	@. $(PROJECTDIR)/scripts/create_config.sh \
	  $(PROJECTNAME) $(PROJECTVERSION) $(PROJECTSUBVERSION) \
	  $(PROJECTREVISION) $@

$(INCLUDEDIR):
	@echo "No include directory at $(INCLUDEDIR)"
$(SOURCEDIR):
	@echo "No sources available at $(SOURCEDIR)"
$(REPOSDIR):
	@echo "No sources available at $(REPOSDIR)"

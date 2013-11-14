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

PACKAGES_NEEDED=g++

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

#EXTRA_CFLAGS = -g -ggdb3
#EXTRA_LDFLAGS = -lrt
# END OF EDIT/CUSTOMISATION AREA
##

PROJECTDIR=${PWD}
SOURCEDIR =$(PROJECTDIR)/src
INCLUDEDIR=$(PROJECTDIR)/src/include
SCRIPTSDIR=$(PROJECTDIR)/scripts

CONFIG_HEADER_FILE=$(INCLUDEDIR)/config.hh

CPPFLAGS  = -W -Wall -Wextra
CPPFLAGS += -Werror
CPPFLAGS += $(EXTRA_CFLAGS)
CPPFLAGS += -I$(INCLUDEDIR)

LDFLAGS = $(EXTRA_LDFLAGS)

MAKEOPT  = --no-print-directory
MAKEOPT += CPPFLAGS="$(CPPFLAGS)"
MAKEOPT += LDFLAGS="$(LDFLAGS)"
MAKEOPT += PROJECTNAME="$(PROJECTDIR)/$(PROJECTNAME)"
MAKEOPT += CONFIG_HEADER_FILE="$(CONFIG_HEADER_FILE)"
MAKEOPT += SCRIPTSDIR="$(SCRIPTSDIR)"

TARBALL= $(PROJECT_TARNAME).$(PROJECT_TARTYPE)

include $(SCRIPTSDIR)/Makefile.functions

help:
	@echo ""
	@echo "$(PROJECTNAME)'s Makefile targets help:"
	@echo ""
	@echo " -BUILDING-"
	@echo ""
	@echo "all:     build the project and release the result under"
	@echo "         $(PROJECTDIR)/$(PROJECTNAME)"
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

# BUILDING

all: DEPENDANCES $(SOURCEDIR) $(CONFIG_HEADER_FILE)
	@make $(MAKEOPT) -C $(SOURCEDIR) all

DEPENDANCES:
	@for package in $(PACKAGES_NEEDED); do \
	  $(call check_package,$${package}); done

# RELEASING

install: all
	@mkdir -p $(BININSTALLDIR) \
	  && cp $(PROJECTNAME) $(BININSTALLDIR) \
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

clean: $(SOURCEDIR)
	@make $(MAKEOPT) -C $(SOURCEDIR) $@

distclean: clean
	@$(call remove_file,$(CONFIG_HEADER_FILE))
	@$(call remove_file,$(TARBALL))

# MISCS

$(CONFIG_HEADER_FILE): $(INCLUDEDIR)
	@. $(PROJECTDIR)/scripts/create_config.sh \
	  $(PROJECTNAME) $(PROJECTVERSION) $(PROJECTSUBVERSION) \
	  $(PROJECTREVISION) $@

$(INCLUDEDIR):
	@echo "No include directory at $(INCLUDEDIR)"
$(SOURCEDIR):
	@echo "No sources available at $(SOURCEDIR)"

# Contain project directory paths and other minimal infomation
include ../Makefile.var

# Include functions
include $(SCRIPTSDIR)/Makefile.functions

boost:
	PROJECTDIR=$(PROJECTDIR) \
	    $(SCRIPTSDIR)/handle_library_dependancies.sh init $@
websocket:
	PROJECTDIR=$(PROJECTDIR) \
	    $(SCRIPTSDIR)/handle_library_dependancies.sh init $@
kconfig:
	PROJECTDIR=$(PROJECTDIR) \
	    $(SCRIPTSDIR)/handle_library_dependancies.sh init $@

build-%: %
	PROJECTDIR=$(PROJECTDIR) \
	    $(SCRIPTSDIR)/handle_library_dependancies.sh build $<

build: build-boost build-websocket build-kconfig

init: .repos_initialized
	@true

.repos_initialized:
	$(V)make $(MAKEOPT) build
	$(V)touch $@

clean:
	@true

distclean: clean
	$(V)PROJECTDIR=$(PROJECTDIR) \
	    $(SCRIPTSDIR)/handle_library_dependancies.sh delete
	$(V)$(call remove_file, .repos_initialized)

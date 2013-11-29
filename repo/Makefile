# Contain project directory paths and other minimal infomation
include ../Makefile.var

# Include functions
include $(SCRIPTSDIR)/Makefile.functions

boost:
	$(V)PROJECTDIR=$(PROJECTDIR) \
	    $(SCRIPTSDIR)/handle_library_dependancies.sh init $@
websocket:
	$(V)PROJECTDIR=$(PROJECTDIR) \
	    $(SCRIPTSDIR)/handle_library_dependancies.sh init $@
kconfig:
	$(V)PROJECTDIR=$(PROJECTDIR) \
	    $(SCRIPTSDIR)/handle_library_dependancies.sh init $@

build-%: %
	$(V)PROJECTDIR=$(PROJECTDIR) \
	    $(SCRIPTSDIR)/handle_library_dependancies.sh build $<

build: build-boost build-websocket build-kconfig

init: .repos_initialized
	@true

.repos_initialized:
	$(V)$(MAKE) $(MAKEOPT) build
	$(V)touch $@

clean:
	@true

distclean: clean
	$(V)PROJECTDIR=$(PROJECTDIR) \
	    $(SCRIPTSDIR)/handle_library_dependancies.sh delete
	$(V)$(call remove_file, .repos_initialized)

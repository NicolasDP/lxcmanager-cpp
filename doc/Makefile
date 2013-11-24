#
include ../Makefile.var

# Include functions
include $(SCRIPTSDIR)/Makefile.functions

all: doc

$(DOXYFILE):
	$(V)$(SCRIPTSDIR)/generate_doxyfile.sh $(SOURCEDIR) $(DOCDIR) $(DOXYFILE)

doc: $(DOXYFILE)
	$(V)doxygen $(DOXYFILE)

clean:
	$(V)$(call remove_file,$(DOXYFILE))

distclean: clean
	$(V)$(call remove_dir,$(DOCDIR)/html)

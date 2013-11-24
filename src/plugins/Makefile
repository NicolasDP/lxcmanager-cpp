include ../../Makefile.var

# Contain the project name
-include $(BUILDCONFIGFILE)

include $(SCRIPTSDIR)/Makefile.functions

MODULE_LIST = communication

all:
	$(V)for module in $(MODULE_LIST); do \
	  make $(MAKEOPT) -C $${module} $@; done

clean:
	$(V)for module in $(MODULE_LIST); do \
	  make $(MAKEOPT) -C $${module} $@; done

distclean:
	$(V)for module in $(MODULE_LIST); do \
	  make $(MAKEOPT) -C $${module} $@; done


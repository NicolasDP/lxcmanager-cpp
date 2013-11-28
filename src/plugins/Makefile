include ../../Makefile.var

# Contain the project name
-include $(BUILDCONFIGFILE)

include $(SCRIPTSDIR)/Makefile.functions

MODULE_LIST_$(CONFIG_PLUGIN_COMMUNICATION) += communication
MODULE_LIST_$(CONFIG_PLUGIN_EMPTY) += empty

all:
	$(V)for module in $(MODULE_LIST_y); do \
	  make $(MAKEOPT) -C $${module} $@; done

clean:
	$(V)for module in $(MODULE_LIST_y) $(MODULE_LIST_); do \
	  make $(MAKEOPT) -C $${module} $@; done

distclean:
	$(V)for module in $(MODULE_LIST_y) $(MODULE_LIST_); do \
	  make $(MAKEOPT) -C $${module} $@; done


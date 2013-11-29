include ../../Makefile.var

# Contain the project name
-include $(BUILDCONFIGFILE)

include $(SCRIPTSDIR)/Makefile.functions

MODULE_LIST_$(CONFIG_PLUGIN_COMMUNICATION) += communication
MODULE_LIST_$(CONFIG_PLUGIN_EMPTY) += empty

.PHONY: $(MODULE_LIST_y)

communication:
	$(V)$(MAKE) $(MAKEOPT) -C $@ all

empty:
	$(V)$(MAKE) $(MAKEOPT) -C $@ all

all: $(MODULE_LIST_y)
	@true

clean:
	$(V)for module in $(MODULE_LIST_y) $(MODULE_LIST_) $(MODULE_LIST_n); do \
	  $(MAKE) $(MAKEOPT) -C $${module} $@; done

distclean:
	$(V)for module in $(MODULE_LIST_y) $(MODULE_LIST_) $(MODULE_LIST_n); do \
	  $(MAKE) $(MAKEOPT) -C $${module} $@; done


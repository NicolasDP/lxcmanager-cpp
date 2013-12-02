# Contain project directory paths and other minimal infomation
include ../Makefile.var

# Contain the project name
-include $(BUILDCONFIGFILE)

# Source files

SOURCES = \
    main.cc \
    modules.cc \
    options.cc \
    version.cc \
    logger.cc \
    lxcmjson.cc \
    lxcmjsonparser.cc \
    lxcmconfigfile.cc

# DO NOT EDIT THE REMAINING FILE

OBJS	= $(SOURCES:.cc=.o)
DEPS	= $(OBJS:.o=.d)

# Create the dependancies files after the first compilation time
# Then each changes in header files will impact the compilation in
# the source files
-include $(DEPS)

# Include functions
include $(SCRIPTSDIR)/Makefile.functions

# BOOST libraries
#   TODO: If you want to add a specific boost library do not forget to update
#   the handle_library_scripts.sh (in $(SCRIPTSDIR)) in order to build the
#   required library
LDFLAGS += -Wl,-Bstatic,-lboost_program_options,-ljson -Wl,-Bdynamic -ldl

ifeq ($(CONFIG_DEBUG), y)
  CPPFLAGS += -g -ggdb3
  LDFLAGS  += -g -ggdb3
endif

all: $(BUILDDIR)/$(CONFIG_PROJECT_NAME)
	$(V)$(MAKE) $(MAKEOPT) -C plugins $@

%.o: %.cc
	@echo " CPP" $@
	@g++ -c $(CPPFLAGS) $< -o $@
	@g++ $(CPPFLAGS) -MM $< -MF $*.d
	@cp -f $*.d $*.d.tmp
	@sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp

$(BUILDDIR)/$(CONFIG_PROJECT_NAME): $(OBJS) $(BUILDCONFIGFILE)
	@echo " LD " $(notdir $@)
	@g++ $(OBJS) -o $@ $(LDFLAGS)
	@echo ""
	@echo "  CORE STORED AT:"
	@echo "  " $(@)
	@echo ""

clean:
	@for objs in $(OBJS); do \
	  $(call remove_file,$${objs}) ; done
	@for objs in $(DEPS); do \
	  $(call remove_file,$${objs}) ; done
	$(V)make $(MAKEOPT) -C plugins $@

distclean: clean
	$(V)make $(MAKEOPT) -C plugins $@

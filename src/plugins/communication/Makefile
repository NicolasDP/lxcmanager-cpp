include ../../../Makefile.var
-include $(BUILDCONFIGFILE)

# Source files
SOURCES = \
    main.cc \
    communication.cc

# DO NOT EDIT THE REMAINING FILE
OBJS	= $(SOURCES:.cc=.o)
DEPS	= $(OBJS:.o=.d)

# Create the dependancies files after the first compilation time
# Then each changes in header files will impact the compilation in
# the source files
-include $(DEPS)

# Variables
MODULENAME = ComPlugin

CPPFLAGS  = -I./include
CPPFLAGS += -I$(INCLUDEDIR)
CPPFLAGS += -I$(LIBSDIR)/include
LDFLAGS += -lboost_system

all: $(MODULENAME)

%.o: %.cc
	@echo " CPP" $@
	@g++ -c $(CPPFLAGS) $< -o $@
	@g++ $(CPPFLAGS) -MM $< -MF $*.d
	@cp -f $*.d $*.d.tmp
	@sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp

$(MODULENAME): $(OBJS)
	@echo " LD " $(notdir $@)
	@g++ $(OBJS) -o $@ $(LDFLAGS)


clean:
	@for objs in $(OBJS); do \
	  $(call remove_file,$${objs}) ; done
	@for objs in $(DEPS); do \
	  $(call remove_file,$${objs}) ; done

distclean: clean
	@true
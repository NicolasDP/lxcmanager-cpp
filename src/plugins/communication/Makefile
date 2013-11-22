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
PROJECTNAME = ComPlugin

EXTRA_CPPFLAGS = -std=c++11 # -g -ggdb3
EXTRA_LDFLAGS = -std=c++11

PROJECTDIR=${PWD}
INCLUDEDIR=$(PROJECTDIR)/include
EXTRA_INCLUDEDIR=../../include
LIBSDIR=../../lib

CPPFLAGS  = -W -Wall -Wextra
#CPPFLAGS += -Werror
CPPFLAGS += $(EXTRA_CPPFLAGS)
CPPFLAGS += -I$(INCLUDEDIR) -I$(EXTRA_INCLUDEDIR) -I$(LIBSDIR)/include

LDFLAGS = $(EXTRA_LDFLAGS)
LDFLAGS += -L$(LIBSDIR)/lib -lboost_system

all: $(PROJECTNAME)

%.o: %.cc
	@echo " CPP" $@
	@g++ -c $(CPPFLAGS) $< -o $@
	@g++ $(CPPFLAGS) -MM $< -MF $*.d
	@cp -f $*.d $*.d.tmp
	@sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp

$(PROJECTNAME): $(OBJS)
	@echo " LD " $(notdir $@)
	@g++ $(OBJS) -o $@ $(LDFLAGS)


# clean:
# 	@for objs in $(OBJS); do \
# 	  $(call remove_file,$${objs}) ; done
# 	@for objs in $(DEPS); do \
# 	  $(call remove_file,$${objs}) ; done

clean:
	rm -rf *.o
	rm -rf *.d
	rm -rf $(PROJECTNAME)


# ca marche ...
dorian:
	g++ main.cc communication.cc -o $(PROJECTNAME) -I ../../lib/include/ -I./include -I../../include -L ../../lib/lib/ -lboost_system

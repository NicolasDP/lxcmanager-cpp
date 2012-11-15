include Makefile.var

all: $(OBJ)
	$(CC) $(FLAGS) $^ -o $(BIN)$(BINFILE) $(LIBS)

%.o: %.c
	$(CC) $(CFLAG) -c $^ -o $@

clean:
	rm -rf *.o
	rm -rf *~
	rm -rf *#*
	rm -rf *.cfg

distclean: clean
	rm -rf $(BIN)$(BINFILE)
	rm -rf doc/*

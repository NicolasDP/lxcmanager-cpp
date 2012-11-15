
all: srv clt

srv:
	cd server && make

clt:
	cd client && make

clean:
	cd server && make clean
	cd client && make clean

distclean:
	cd server && make distclean
	cd client && make distclean

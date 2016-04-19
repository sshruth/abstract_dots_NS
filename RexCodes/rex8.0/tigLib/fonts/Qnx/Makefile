#
# Makefile for fonts.  Move stuff to local lib
#
LIB   = /usr/local/lib
FONTS = $(LIB)/tig

all: vectoasc asctovec 

vectoasc: ../vectoasc.c
	cc ../vectoasc.c -o vectoasc

asctovec: ../asctovec.c
	cc ../asctovec.c -o asctovec

install:
	cp ../Fonts.Sun/*.f $(FONTS)

clean:
	-rm asctovec vectoasc

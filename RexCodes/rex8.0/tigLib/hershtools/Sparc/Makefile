#	Makefile 1.0	6/5/87
#
# Makefile for font programs
#
# Modified for use at LSR, Brad J. Zoltick, 6-1989
#
LIB = /usr/local.sparc/lib
BIN = /usr/local.sparc/bin
INC = /usr/local.sparc/include
OWNER = brad

LIBS = -ltermlib
HDR  = ..

CFLAGS = -O2 -I$(HDR)
C = $(CC) $(CFLAGS) -c

all: eplot hplot htrans cnv h2s

htrans:	htrans.o
	$(CC) $(CFLAGS) -o htrans htrans.o

eplot:	eplot.o termio.o
	$(CC) $(CFLAGS) -o eplot termio.o eplot.o $(LIBS)

hplot:	hplot.o termio.o
	$(CC) $(CFLAGS) -o hplot termio.o hplot.o $(LIBS)

eplot.o: ../eplot.c
	 $(C) ../eplot.c

hplot.o: ../hplot.c ../vfont.c
	 $(C) ../hplot.c

htrans.o: ../htrans.c
	  $(C) ../htrans.c
	  
termio.o: ../termio.c ../termio.h
	  $(C) ../termio.c

cnv:	cnv.o
	$(CC) $(CFLAGS) -o cnv cnv.o

cnv.o:  ../cnv.c
	$(C) ../cnv.c

h2s:	../h2s.l
	lex ../h2s.l
	$(CC) $(CFLAGS) -o h2s lex.yy.c -ll
	-rm lex.yy.c

install:
	install -s -o brad eplot	$(BIN)
	install -s -o brad hplot 	$(BIN)
	install -s -o brad htrans	$(BIN)
	install -s -o brad cnv		$(BIN)
	install -s -o brad h2s		$(BIN)			
clean:
	-rm -f *.o eplot hplot htrans cnv h2s


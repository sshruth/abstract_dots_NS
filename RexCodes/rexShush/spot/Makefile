#
#	Makefile for REX scribe process.
#

ifndef DEBUG
DEBUG = -g
endif

ifndef OPTIM
OPTIM = -O
endif

PLATFORM=gcc_ntox86
PHABOPTS= 

# CC = qcc
# LD = qcc
# CFLAGS  = -V$(PLATFORM) -w -c $(OPTIM) $(DEBUG) -I../hdr

CC = gcc
LD = gcc
CFLAGS  = -w -c $(OPTIM) $(DEBUG) -I../hdr

H = ../hdr

OUTPUT = spot

OBJ = main.o spot.tab.o

all:   $(OUTPUT)


$(OUTPUT):	$(OBJ)
	$(LD) $(OBJ) -o spot

clean:
	  rm lex.yy.c *.tab.[ch] *.o spot

lex.yy.c:	spot.lex
	flex spot.lex

spot.tab.o:	spot.y lex.yy.c
		bison -d spot.y
		$(CC) $(CFLAGS) spot.tab.c

main.o: main.c

pure:
	cc -n spot.tab.o main.o -ly -lln -lS -o spot

nfpu:
	cc -f spot.tab.o main.o -ly -lln -lS -o spot
	-strip spot

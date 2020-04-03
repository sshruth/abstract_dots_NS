#	Makefile for the liblabTools.a REX system library

ifndef DEBUG
DEBUG = -g
endif

ifndef OPTIM
OPTIM = -O
endif

PLATFORM=gcc_ntox86

CC = gcc
LD = gcc
CFLAGS  = -w -c $(OPTIM) $(DEBUG) -I ../hdr 

L = ../lib
H = ../hdr
U = /usr/include

OUTPUT = liblabTools.a

OBJ = ecode.o matlab.o safe_alloc.o timer.o toys.o udpmsg.o udp.o vs.o vs_dots.o dio_lab.o

all:	$L/$(OUTPUT)

$L/$(OUTPUT):	$(OBJ)
	ar -rvs $L/$(OUTPUT) $(OBJ)
#	ranlib $L/$(OUTPUT)

ecode.o: ecode.c ecode.h timer.h toys.h $H/rexhdrs.h
	$(CC) $(CFLAGS) ecode.c

matlab.o: matlab.c matlab.h 
	$(CC) $(CFLAGS) matlab.c
	
safe_alloc.o: safe_alloc.c safe_alloc.h
	$(CC) $(CFLAGS) safe_alloc.c

timer.o: timer.c timer.h toys.h $H/rexhdrs.h 
	$(CC) $(CFLAGS) timer.c

toys.o: toys.c toys.h safe_alloc.h 
	$(CC) $(CFLAGS) toys.c

udpmsg.o: udpmsg.c udpmsg.h $H/rexhdrs.h
	$(CC) $(CFLAGS) udpmsg.c

udp.o: udp.c udp.h
	$(CC) $(CFLAGS) udp.c
	
vs.o: vs.c vs.h toys.h
	$(CC) $(CFLAGS) vs.c

vs_dots.o: vs_dots.c vs_dots.h toys.h matlab.h $H/rexhdrs.h
	$(CC) $(CFLAGS) vs_dots.c

dio_lab.o: dio_lab.c dio_lab.h
	$(CC) $(CFLAGS) dio_lab.c

clean:
	  rm -f *.o *.map *.err ../lib/liblabTools.a

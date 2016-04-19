#	Makefile for the libint.a REX system library

ifndef DEBUG
DEBUG = -g
endif

ifndef OPTIM
OPTIM = -O
endif

PLATFORM=gcc_ntox86

# CC = qcc
# LD = qcc
# CFLAGS  = -V$(PLATFORM) -w -c $(OPTIM) $(DEBUG) -I ../hdr

CC = gcc
LD = gcc
CFLAGS  = -w -c $(OPTIM) $(DEBUG) -I ../hdr

L = ../lib
H = ../hdr
U = /usr/include

OUTPUT = libint.a

OBJ = clock.o cnf.o dev.o dio.o dma.o i.o int.o menu.o pcmsg.o pcsVexSocket.o \
	pcsMexSocket.o prego.o sinit.o svar.o pci.o

all:	$L/$(OUTPUT)

# $L/$(OUTPUT):	$(OBJ)
# 	$(LD) -V$(PLATFORM) $(OBJ) -A $L/$(OUTPUT)

$L/$(OUTPUT):	$(OBJ)
	ar rvs $L/$(OUTPUT) $(OBJ)

clock.o: clock.c $H/rexhdrs.h $H/bufm.h $H/ecode.h $H/sac.h $H/pcmsg.h \
		 $H/int.h $H/pcsSocket.h $H/cnf.h
	$(CC) $(CFLAGS) clock.c

cnf.o: cnf.c $H/rexhdrs.h $H/ramp.h $H/int.h $H/cnf.h
	$(CC) $(CFLAGS) cnf.c

dev.o: dev.c $H/rexhdrs.h $H/dma.h $H/int.h $H/cnf.h
	$(CC) $(CFLAGS) dev.c

dio.o: dio.c $H/rexhdrs.h $H/int.h $H/cnf.h
	$(CC) $(CFLAGS) dio.c

dma.o: dma.c $H/rexhdrs.h $H/dma.h $H/int.h $H/cnf.h
	$(CC) $(CFLAGS) dma.c

i.o: i.c $H/rexhdrs.h $H/ecode.h $H/sac.h $H/ramp.h $H/int.h $H/cnf.h
	$(CC) $(CFLAGS) i.c

int.o: int.c $H/rexhdrs.h $H/bufm.h $H/ecode.h $H/sac.h $H/dma.h $H/ramp.h \
	   $H/int.h $H/cnf.h
	$(CC) $(CFLAGS) int.c

menu.o: menu.c $H/rexhdrs.h $H/sac.h $H/ramp.h $H/int.h $H/cnf.h
	$(CC) $(CFLAGS) menu.c

pcmsg.o: pcmsg.c $H/rexhdrs.h $H/tty.h $H/int.h $H/pcmsg.h $H/cnf.h
	$(CC) $(CFLAGS) pcmsg.c

pcsVexSocket.o: pcsVexSocket.c $H/rexhdrs.h $H/pcsSocket.h $H/timerFunction.h $H/cnf.h
	$(CC) $(CFLAGS) pcsVexSocket.c

pcsMexSocket.o: pcsMexSocket.c $H/rexhdrs.h $H/pcsSocket.h $H/timerFunction.h $H/cnf.h
	$(CC) $(CFLAGS) pcsMexSocket.c

prego.o: prego.c $H/rexhdrs.h $H/ecode.h $H/sac.h $H/ramp.h $H/int.h $H/cnf.h
	$(CC) $(CFLAGS) prego.c

pci.o: pci.c $H/rexhdrs.h $H/ecode.h $H/sac.h $H/ramp.h $H/int.h $H/cnf.h
	$(CC) $(CFLAGS) pci.c

sinit.o: sinit.c $H/rexhdrs.h $H/int.h $H/cnf.h
	$(CC) $(CFLAGS) sinit.c

svar.o: svar.c $H/rexhdrs.h $H/tty.h $H/int.h $H/cnf.h
	$(CC) $(CFLAGS) svar.c

clean:
	  rm -f *.o $L/$(OUTPUT)

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
# CFLAGS  = -V$(PLATFORM) -w $(OPTIM) $(DEBUG) -I../hdr

CC = gcc
LD = gcc
CFLAGS  = -w $(OPTIM) $(DEBUG) -I../hdr

H = ../hdr

OUTPUT = scribe

OBJ = alert.o dumpa.o dumpe.o file.o i.o main.o menu.o noun.o user.o

all:	$(OUTPUT)

# $(OUTPUT):	$(OBJ) $(REXLIBS)
# 	$(LD) $(OBJ) $(CFLAGS) -L../lib -lrex -M -o scribe

$(OUTPUT):	$(OBJ) $(REXLIBS)
	$(LD) $(OBJ) $(CFLAGS) -L../lib -lrex -o scribe

alert.o:	alert.c $H/sys.h $H/proc.h $H/buf.h $H/menu.h $H/scribe.h $H/tty.h $H/idsp.h $H/cdsp.h

dumpa.o:	dumpa.c $H/sys.h $H/proc.h $H/buf.h $H/bufm.h $H/scribe.h $H/ecode.h $H/idsp.h $H/cdsp.h

dumpe.o:	dumpe.c $H/sys.h $H/proc.h $H/buf.h $H/scribe.h $H/idsp.h $H/cdsp.h

file.o:		file.c $H/sys.h $H/proc.h $H/buf.h $H/scribe.h $H/idsp.h $H/cdsp.h

i.o:		i.c $H/sys.h $H/proc.h $H/buf.h $H/scribe.h $H/menu.h $H/idsp.h $H/cdsp.h

main.o:		main.c $H/sys.h $H/proc.h $H/buf.h $H/scribe.h $H/menu.h $H/tty.h $H/idsp.h $H/cdsp.h

menu.o:		menu.c $H/sys.h $H/menu.h $H/scribe.h $H/proc.h $H/buf.h $H/idsp.h $H/cdsp.h

noun.o:		noun.c $H/sys.h $H/proc.h $H/buf.h $H/scribe.h $H/ecode.h $H/idsp.h $H/cdsp.h

user.o:		user.c $H/sys.h $H/proc.h $H/buf.h $H/menu.h $H/comm.h $H/idsp.h $H/cdsp.h

clean:
	  rm -f *.o $(OUTPUT)

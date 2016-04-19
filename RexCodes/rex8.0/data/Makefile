#	Makefile for REX data process.

ifndef DEBUG
DEBUG = -g
endif

ifndef OPTIM
OPTIM = -O
endif

PLATFORM=gcc_ntox86

# CC = qcc
# LD = qcc
# CFLAGS  = -V$(PLATFORM) -w $(OPTIM) $(DEBUG) -I ../hdr

CC = gcc
LD = gcc
CFLAGS  = -w $(OPTIM) $(DEBUG) -I ../hdr

L = ../lib
H = ../hdr
U = /usr/include

OUTPUT = ../run/rex

OBJ = rex.o

all:	$(OUTPUT)

$(OUTPUT):	$(OBJ) 
	$(LD) $(CFLAGS) $(OBJ) -o ../run/rex

rex.o:	rex.c $H/sys.h $H/buf.h $H/proc.h $H/cnf.h\
	$H/ecode.h $H/cdsp.h $U/stdio.h $U/stdlib.h $U/errno.h \
	$U/unistd.h $U/process.h $U/signal.h $U/string.h $U/sys/types.h \
	$U/sys/stat.h $U/sys/wait.h $U/fcntl.h $H/idsp.h

clean:
	  rm -f *.o $(OUTPUT)

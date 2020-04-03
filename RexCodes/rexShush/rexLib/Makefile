#	Makefile for the librex.a REX system library

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

OUTPUT = librex.a

OBJ = map_shared.o ldevent.o sendmsg.o access.o rtread.o rtwrite.o \
	sendname.o sleepcomm.o fills.o index_RL.o parse.o sindex.o \
	stufs.o rxerr.o protect.o match.o nullf.o badverb.o dprintf.o \
	dputs.o itoa_RL.o tgetl.o tcup.o tgscup.o tsetsc.o tputs.o tty.o

all:	$L/$(OUTPUT)

# $L/$(OUTPUT):	$(OBJ)
# 	$(LD) -V$(PLATFORM) $(OBJ) -A $L/$(OUTPUT)

$L/$(OUTPUT):	$(OBJ)
	ar rvs $L/$(OUTPUT) $(OBJ)

access.o: access.c $H/sys.h $H/proc.h $H/buf.h $H/menu.h $H/tty.h $U/string.h \
		$U/stdio.h $U/ctype.h $U/stdlib.h $U/sys/types.h $H/idsp.h $H/cdsp.h
	$(CC) $(CFLAGS) access.c

badverb.o: badverb.c $H/sys.h
	$(CC) $(CFLAGS) badverb.c

dprintf.o: dprintf.c $H/sys.h $U/stdarg.h dprintf.c
	$(CC) $(CFLAGS) dprintf.c

dputs.o: dputs.c $H/sys.h  $U/stdio.h $U/stdlib.h dputs.c
	$(CC) $(CFLAGS) dputs.c

fills.o: fills.c $H/sys.h
	$(CC) $(CFLAGS) fills.c

index_RL.o:	index_RL.c $H/sys.h
	$(CC) $(CFLAGS) index_RL.c

itoa_RL.o: itoa_RL.c $H/sys.h itoa_RL.c
	$(CC) $(CFLAGS) itoa_RL.c

match.o: match.c $H/sys.h
	$(CC) $(CFLAGS) match.c

map_shared.o: map_shared.c $H/sys.h
	$(CC) $(CFLAGS) map_shared.c

nullf.o: nullf.c $H/sys.h
	$(CC) $(CFLAGS) nullf.c

parse.o: parse.c $H/sys.h $U/ctype.h $U/stdio.h
	$(CC) $(CFLAGS) parse.c

protect.o: protect.c $H/sys.h $H/proc.h
	$(CC) $(CFLAGS) protect.c

ldevent.o: ldevent.c $H/sys.h $H/proc.h $H/buf.h $U/unistd.h $H/idsp.h $H/cdsp.h
	$(CC) $(CFLAGS) ldevent.c

rtread.o: rtread.c $H/sys.h $H/proc.h $H/buf.h $U/stdio.h $U/string.h $H/idsp.h $H/cdsp.h\
		$U/fcntl.h $U/stdio.h $U/unistd.h $U/sys/types.h $U/sys/stat.h
	$(CC) $(CFLAGS) rtread.c

rtwrite.o: rtwrite.c $H/sys.h $H/proc.h $H/buf.h $H/menu.h $H/tty.h $H/idsp.h $H/cdsp.h\
		$U/stdio.h $U/string.h $U/sys/types.h
	$(CC) $(CFLAGS) rtwrite.c

rxerr.o: rxerr.c $H/sys.h $H/proc.h $H/buf.h $U/unistd.h $H/idsp.h $H/cdsp.h rxerr.c
	$(CC) $(CFLAGS) rxerr.c

sendmsg.o: sendmsg.c $H/sys.h $H/proc.h $H/buf.h $U/signal.h $U/stdio.h $H/idsp.h $H/cdsp.h
	$(CC) $(CFLAGS) sendmsg.c

sendname.o:	sendname.c $H/sys.h $H/buf.h $H/menu.h $H/proc.h $H/idsp.h $H/cdsp.h\
		$U/stdio.h $U/unistd.h
	$(CC) $(CFLAGS) sendname.c

sindex.o: sindex.c $H/sys.h
	$(CC) $(CFLAGS) sindex.c

sleepcomm.o: sleepcomm.c $H/sys.h $H/buf.h $H/proc.h $U/unistd.h $H/idsp.h $H/cdsp.h
	$(CC) $(CFLAGS) sleepcomm.c

stufs.o: stufs.c $H/sys.h
	$(CC) $(CFLAGS) stufs.c

tcup.o:	tcup.c $H/sys.h $H/tty.h $U/stdio.h
	$(CC) $(CFLAGS) tcup.c

tgetl.o: tgetl.c $H/sys.h $H/proc.h $H/buf.h $H/tty.h $U/sys/types.h $U/unistd.h \
		$U/errno.h $H/idsp.h $H/cdsp.h
	$(CC) $(CFLAGS) tgetl.c

tgscup.o: tgscup.c $H/sys.h $U/sys/types.h
	$(CC) $(CFLAGS) tgscup.c

tputs.o: tputs.c $H/sys.h $U/stdio.h $U/unistd.h $U/errno.h
	$(CC) $(CFLAGS) tputs.c

tsetsc.o:  tsetsc.c $H/sys.h $H/tty.h $U/stdio.h 
	$(CC) $(CFLAGS) tsetsc.c

tty.o: tty.c $H/sys.h $H/tty.h
	$(CC) $(CFLAGS) tty.c

clean:
	  rm -f *.o $L/$(OUTPUT)

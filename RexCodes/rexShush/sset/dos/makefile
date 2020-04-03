# Makefile under QNX

CC= wcc386
COMPILE.c= $(CC) $(CFLAGS)
#CFLAGS= -zq -d1 -oaxt -ms -w3 -i=/usr/include -i=../hdr
CFLAGS= -zq -d2 -oaxt -ms -w3 -i=/usr/include -i=../hdr
RTFLAGS= -zu -s
LD= cc
#LDFLAGS= -T1 -M -Wl,"Debug lines"
LDFLAGS= -T1 -M -Wl,"Debug all"

# Old flags:
#CFLAGS = -2 -T1 -Wc,-w3 -g1 -Oaxt
#CFLAGS= -zq -d2 -oaxt -4 -ms -w3 -i=/usr/include -i=../hdr
H = ../hdr
U = /usr/include
L = ../lib
OUTPUT = pcm

OBJ = 		main.o pcmsg.o

all:	$(OUTPUT)

$(OUTPUT):	$(OBJ) 

	$(LD) $(LDFLAGS) $(OBJ) -o $@


main.o:	pcmsg.h

pcmsg.o: pcmsg.h

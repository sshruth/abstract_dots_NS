#
# Application Makefile   
#

ifndef DEBUG
DEBUG = -g
endif

ifndef OPTIM
OPTIM = -O
endif

include	../abOfiles
include ../indOfiles

include	../abSfiles
include ../indSfiles

include	../abHfiles
include ../indHfiles

include	../abWfiles
#include	../abLfiles
ABLIB = -lAp -lph -lm
include ../indLfiles

PLATFORM=gcc_ntox86
PHABOPTS= 

# REX libraries kept in rex/lib.
# REXLIBS = -Bstatic -l int -l act -l rex

# CC = qcc
# LD = qcc
# CFLAGS  = -V$(PLATFORM) -w $(OPTIM) $(DEBUG) $(PHABOPTS) -I. -I../../../hdr
# CXXFLAGS = $(CFLAGS)
# LDFLAGS = -V$(PLATFORM) -Bstatic $(DEBUG) $(ABLIB) -l phexlib $(PHABOPTS)
# SDFLAGS = -V$(PLATFORM)          $(DEBUG) $(ABLIB) -l phexlib $(PHABOPTS)

CC = gcc
LD = gcc
CFLAGS  = -w $(OPTIM) $(DEBUG) $(PHABOPTS) -I. -I../../../hdr
CXXFLAGS = $(CFLAGS)
LDFLAGS = -Bstatic $(DEBUG) -lphexlib $(ABLIB) $(MYLIB) $(PHABOPTS)
SDFLAGS =          $(DEBUG) -lphexlib $(ABLIB) $(MYLIB) $(PHABOPTS)

VPATH = ..

#
# Application Program
#

# shr:	$(ABOBJ) $(MYOBJ) spot.d.o
# 		$(LD) $(ABOBJ) $(MYOBJ) spot.d.o $(SDFLAGS) -l socket -L../../../lib $(REXLIBS) -M -o toolBar
# 		usemsg toolBar ../Usemsg
# 		phabbind toolBar $(ABMOD)

# app:	$(ABOBJ) $(MYOBJ) spot.d.o
# 		$(LD) $(ABOBJ) $(MYOBJ) spot.d.o $(LDFLAGS) -l socket -L../../../lib $(REXLIBS) -M -o toolBar
# 		usemsg toolBar ../Usemsg
# 		phabbind toolBar $(ABMOD)

shr:	$(ABOBJ) $(MYOBJ) spot.d.o
		$(LD) $(ABOBJ) $(MYOBJ) spot.d.o $(SDFLAGS) -o toolBar
		usemsg toolBar ../Usemsg
		phabbind toolBar $(ABMOD)

app:	$(ABOBJ) $(MYOBJ) spot.d.o
		$(LD) $(ABOBJ) $(MYOBJ) spot.d.o $(LDFLAGS) -o toolBar
		usemsg toolBar ../Usemsg
		phabbind toolBar $(ABMOD)

clean:
		rm -f *.o *.err *.map toolBar

install:
		-cp -vnf toolBar /usr/bin/photon

#
# Headers
#

$(ABOBJ) $(MYOBJ):	$(MYHDR)

#
# Prototype Generation
#

proto:
		approto -p $(ABSRC) $(MYSRC) > proto.h

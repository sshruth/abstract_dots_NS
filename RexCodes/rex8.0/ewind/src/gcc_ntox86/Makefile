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
# REXLIBS = -Bstatic -l rex -l act

# CC = qcc
# LD = qcc
# CFLAGS  = -V$(PLATFORM) -w $(OPTIM) $(DEBUG) $(PHABOPTS) -I. -I../../../hdr
# CXXFLAGS = $(CFLAGS)
# LDFLAGS = -V$(PLATFORM) -Bstatic $(DEBUG) -l phexlib $(ABLIB) $(PHABOPTS)
# SDFLAGS = -V$(PLATFORM)          $(DEBUG) -l phexlib $(ABLIB) $(PHABOPTS)

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

# shr:	$(ABOBJ) $(MYOBJ)
#		$(LD) $(ABOBJ) $(MYOBJ) $(SDFLAGS) -l socket -L../../../lib $(REXLIBS) -M -o ewind
# 		usemsg ewind ../Usemsg
# 		phabbind ewind $(ABMOD)

# app:	$(ABOBJ) $(MYOBJ)
# 		$(LD) $(ABOBJ) $(MYOBJ) $(LDFLAGS) -l socket -L../../../lib $(REXLIBS) -M -o ewind
# 		usemsg ewind ../Usemsg
# 		phabbind ewind $(ABMOD)

shr:	$(ABOBJ) $(MYOBJ)
		$(LD) $(ABOBJ) $(MYOBJ) $(SDFLAGS) -o ewind
		usemsg ewind ../Usemsg
		phabbind ewind $(ABMOD)

app:	$(ABOBJ) $(MYOBJ)
		$(LD) $(ABOBJ) $(MYOBJ) $(LDFLAGS) -o ewind
		usemsg ewind ../Usemsg
		phabbind ewind $(ABMOD)

clean:
		rm -f *.o *.err *.map ewind

install:
		-cp -vnf ewind /usr/photon/bin

#
# Headers
#

$(ABOBJ) $(MYOBJ):	$(MYHDR)

#
# Prototype Generation
#

proto:
		-rm -f proto.h
		approto -p $(ABSRC) $(MYSRC) > proto.h

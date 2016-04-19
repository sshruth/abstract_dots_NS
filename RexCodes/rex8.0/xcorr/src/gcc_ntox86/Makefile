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
# REXLIBS = -Bstatic -l tig -l neuron -l rex -l act

# CC  = qcc
# CXX = qcc
# LD  = qcc
# CFLAGS  = -V$(PLATFORM) -w8 $(OPTIM) $(DEBUG) $(PHABOPTS) -I. -I../../../hdr
# CXXFLAGS = $(CFLAGS)
# LDFLAGS = -V$(PLATFORM) -Bstatic $(DEBUG) -l phexlib $(ABLIB) $(PHABOPTS)
# SDFLAGS = -V$(PLATFORM)          $(DEBUG) -l phexlib $(ABLIB) $(PHABOPTS)

CC  = gcc
LD  = gcc
CFLAGS  = -w $(OPTIM) $(DEBUG) $(PHABOPTS) -I. -I../../../hdr
CXXFLAGS = $(CFLAGS)
LDFLAGS = -Bstatic $(DEBUG) -lphexlib $(ABLIB) $(MYLIB) $(PHABOPTS)
SDFLAGS =          $(DEBUG) -lphexlib $(ABLIB) $(MYLIB) $(PHABOPTS)

VPATH = ..

#
# Application Program
#

# shr:	$(ABOBJ) $(MYOBJ)
# 		$(LD) $(ABOBJ) $(MYOBJ) $(SDFLAGS) -L../../../lib $(REXLIBS) -M -o xcorr
# 		usemsg xcorr ../Usemsg
# 		phabbind xcorr $(ABMOD)

# app:	$(ABOBJ) $(MYOBJ)
# 		$(LD) $(ABOBJ) $(MYOBJ) $(LDFLAGS) -L../../../lib $(REXLIBS) -M -o xcorr
# 		usemsg xcorr ../Usemsg
# 		phabbind xcorr $(ABMOD)

shr:	$(ABOBJ) $(MYOBJ)
		$(LD) $(ABOBJ) $(MYOBJ) $(SDFLAGS) -o xcorr
		usemsg xcorr ../Usemsg
		phabbind xcorr $(ABMOD)

app:	$(ABOBJ) $(MYOBJ)
		$(LD) $(ABOBJ) $(MYOBJ) $(LDFLAGS) -o xcorr
		usemsg xcorr ../Usemsg
		phabbind xcorr $(ABMOD)

clean:
		rm -f *.o *.err *.map xcorr

install:
		-cp -vnf xcorr /usr/photon/bin

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

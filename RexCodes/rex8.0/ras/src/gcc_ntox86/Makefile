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

# shr:	$(ABOBJ) $(MYOBJ)
# 		$(LD) $(ABOBJ) $(MYOBJ) $(SDFLAGS) -L../../../lib $(REXLIBS) -M -o ras
# 		usemsg ras ../Usemsg
# 		phabbind ras $(ABMOD)

# app:	$(ABOBJ) $(MYOBJ)
# 		$(LD) $(ABOBJ) $(MYOBJ) $(LDFLAGS) -L../../../lib $(REXLIBS) -M -o ras
# 		usemsg ras ../Usemsg
# 		phabbind ras $(ABMOD)

shr:	$(ABOBJ) $(MYOBJ)
		$(LD) $(ABOBJ) $(MYOBJ) $(SDFLAGS) -o ras
		usemsg ras ../Usemsg
		phabbind ras $(ABMOD)

app:	$(ABOBJ) $(MYOBJ)
		$(LD) $(ABOBJ) $(MYOBJ) $(LDFLAGS) -o ras
		usemsg ras ../Usemsg
		phabbind ras $(ABMOD)

clean:
		rm -f *.o *.err *.map ras

install:
		-cp -vnf ras /usr/bin/photon

#
# Headers
#

$(ABOBJ) $(MYOBJ):	$(MYHDR)

#
# Prototype Generation
#

proto:
		approto -p $(ABSRC) $(MYSRC) > proto.h

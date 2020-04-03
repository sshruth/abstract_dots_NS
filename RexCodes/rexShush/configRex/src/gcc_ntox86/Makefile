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
include	../abLfiles
include ../indLfiles

PLATFORM=gcc_ntox86
PHABOPTS= 

# CC  = qcc
# CXX = qcc
# LD  = qcc
# CFLAGS  = -V$(PLATFORM) -w $(OPTIM) $(DEBUG) $(PHABOPTS) -I.
# CXXFLAGS = $(CFLAGS)
# LDFLAGS = -V$(PLATFORM) -Bstatic $(DEBUG) -l phexlib $(ABLIB) $(PHABOPTS)
# SDFLAGS = -V$(PLATFORM)          $(DEBUG) -l phexlib $(ABLIB) $(PHABOPTS)

CC  = gcc
CXX = gcc
LD  = gcc
CFLAGS  = -w $(OPTIM) $(DEBUG) $(PHABOPTS) -I.
CXXFLAGS = $(CFLAGS)
LDFLAGS = -Bstatic $(DEBUG) -lphexlib $(ABLIB) $(MYLIB) $(PHABOPTS)
SDFLAGS =          $(DEBUG) -lphexlib $(ABLIB) $(MYLIB) $(PHABOPTS)

VPATH = ..

#
# Application Program
#

# shr:	$(ABOBJ) $(MYOBJ)
# 		$(LD) $(ABOBJ) $(MYOBJ) $(SDFLAGS) -M -o configRex
# 		usemsg configRex ../Usemsg
# 		phabbind configRex $(ABMOD)
# 		cp -vnf configRex ../../../sset/configRex

# app:	$(ABOBJ) $(MYOBJ)
# 		$(LD) $(ABOBJ) $(MYOBJ) $(LDFLAGS) -M -o configRex
# 		usemsg configRex ../Usemsg
# 		phabbind configRex $(ABMOD)
# 		cp -vnf configRex ../../../sset/configRex

shr:	$(ABOBJ) $(MYOBJ)
		$(LD) $(ABOBJ) $(MYOBJ) $(SDFLAGS) -o configRex
		usemsg configRex ../Usemsg
		phabbind configRex $(ABMOD)
		cp -vnf configRex ../../../sset/configRex

app:	$(ABOBJ) $(MYOBJ)
		$(LD) $(ABOBJ) $(MYOBJ) $(LDFLAGS) -o configRex
		usemsg configRex ../Usemsg
		phabbind configRex $(ABMOD)
		cp -vnf configRex ../../../sset/configRex

clean:
		rm -f *.o *.err *.map configRex

install:
		-cp -vnf configRex /usr/bin/photon

#
# Headers
#

$(ABOBJ) $(MYOBJ):	$(MYHDR)

#
# Prototype Generation
#

proto:
		approto -p $(ABSRC) $(MYSRC) > proto.h

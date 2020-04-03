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

CC  = g++
CXX = g++
LD  = g++
CFLAGS  = -w $(OPTIM) $(DEBUG) $(PHABOPTS) -I.
CXXFLAGS = $(CFLAGS)
LDFLAGS = -Bstatic $(DEBUG) -lphexlib $(ABLIB) $(MYLIB) $(PHABOPTS)
SDFLAGS =          $(DEBUG) -lphexlib $(ABLIB) $(MYLIB) $(PHABOPTS)

VPATH = ..

#
# Application Program
#

# shr:	$(ABOBJ) $(MYOBJ)
# 		$(LD) $(ABOBJ) $(MYOBJ) $(SDFLAGS) -M -o rebel
# 		usemsg rebel ../Usemsg
# 		phabbind rebel $(ABMOD)

# app:	$(ABOBJ) $(MYOBJ)
# 		$(LD) $(ABOBJ) $(MYOBJ) $(LDFLAGS) -M -o rebel
# 		usemsg rebel ../Usemsg
# 		phabbind rebel $(ABMOD)

shr:	$(ABOBJ) $(MYOBJ)
		$(LD) $(ABOBJ) $(MYOBJ) $(SDFLAGS) -o rebel
		usemsg rebel ../Usemsg
		phabbind rebel $(ABMOD)

app:	$(ABOBJ) $(MYOBJ)
		$(LD) $(ABOBJ) $(MYOBJ) $(LDFLAGS) -o rebel
		usemsg rebel ../Usemsg
		phabbind rebel $(ABMOD)

clean:
		rm -f *.o *.err *.map rebel

install:
		-cp -vnf rebel /usr/photon/bin

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

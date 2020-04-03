#
#	Makefile for REX spot process.
#

ifndef DEBUG
DEBUG = -g
endif

ifndef OPTIM
OPTIM = -O
endif

PLATFORM=gcc_ntox86

PHABOPTS= 
REBEL=
rf=
# CC = qcc
# LD = qcc
# CFLAGS  = -V$(PLATFORM) -w -c $(OPTIM) $(DEBUG) -I../hdr

CC = gcc
LD = gcc
CFLAGS = -w -c $(OPTIM) $(DEBUG) -I ../hdr

H = ../hdr
SPOT = ../spot/spot
R = ../run
L = ../lib

sf = tstramp

ifdef REBEL
RMSRC=
RUNSPOT=
SRC=$(sf).r.c
COMPLETECMD = (cd ../sset; make all REBEL=rebel sf=$(sf))
else
RMSRC = rm -f $(sf).d.c
RUNSPOT = $(SPOT) $(sf).d
SRC = $(sf).d.c
COMPLETECMD = (cd ../sset; make all sf=$(sf))
endif


OUTPUT = $R/toolBar

OBJ = ../toolBar/src/gcc_ntox86/spot.d.o

all:	Make_Pre $(OUTPUT)

Make_Pre:
	$(RMSRC)
	rm -f $(OBJ)

$(OUTPUT): $(OBJ)
	(cd ../act; make)
	(cd ../int; make)
	(cd ../labTools; make)
	(cd ../toolBar/src/gcc_ntox86; make shr; cp toolBar ../../../run/$(sf))
	usemsg ../run/$(sf) ../toolBar/src/Usemsg

$(OBJ):
	$(RUNSPOT)
	$(CC) $(CFLAGS) $(SRC) -o $(OBJ)

complete:
	@echo "\nMaking entire REX system...\n"
	(rm -f *.d.c *.d.o *.map *.err)
	(cd ../run; rm -f procSwitch rex *.map *.err)
	(cd ../lib; rm -f *.lib)
	@echo "\nMAKING NEURON LIB\n"
	(cd ../neuronLib/lib/QNX; make clean; make)
	@echo "\nMAKING TIG LIB\n"
	(cd ../tigLib/lib/QNX; make clean; make)
	@echo "\nMAKING ACTIONS LIB\n"
	(cd ../act; make clean; make)
	@echo "\nMAKING REX LIB\n"
	(cd ../rexLib; make clean; make)
	@echo "\nMAKING INT LIB\n"
	(cd ../int; make clean; make)
	@echo "\nMAKING DATA\n"
	(cd ../data; make clean; make)
	@echo "\nMAKING SCRIBE\n"
	(cd ../scribe; make clean; make)
	@echo "\nMAKING PROCSWITCH\n"
	(cd ../procSwitch/src/gcc_ntox86; make clean; make shr)
	@echo "\nMAKING EWIND\n"
	(cd ../ewind/src/gcc_ntox86; make clean; make shr)
	@echo "\nMAKING RLINE\n"
	(cd ../rline/src/gcc_ntox86; make clean; make shr)
	@echo "\nMAKING RAS\n"
	(cd ../ras/src/gcc_ntox86; make clean; make shr)
	@echo "\nMAKING XCORR\n"
	(cd ../xcorr/src/gcc_ntox86; make clean; make shr)
	@echo "\nMAKING REBEL\n"
	(cd ../rebel/src/gcc_ntox86; make clean; make shr; make install)
	@echo "\nMAKING TOOLBAR\n"
	(cd ../toolBar/src/gcc_ntox86; make clean)
	@echo "\nMAKING SPOT\n"
	(cd ../spot; make clean; make all)
	@echo "\nMAKING LIBLABTOOLS\n"
	(cd ../labTools; make clean; make)
	@echo "\nMAKING SSET\n"
	$(COMPLETECMD)

clean:
	@echo "\nCleaning entire REX system...\n"
	(rm -f *~ *.d.c *.d.o *.map *.err)
	(cd ../run; rm -f procSwitch rex *.map *.err)
	(cd ../lib; rm -f *.lib; rm -f lib*.a; rm -f lib*.so)
	@echo "\nCLEANING NEURON LIB\n"
	(cd ../neuronLib/lib; rm -f *~)
	(cd ../neuronLib/include; rm -f *~)
	(cd ../neuronLib/lib/QNX; make clean)
	@echo "\nCLEANING TIG LIB\n"
	(cd ../tigLib/lib; rm -f *~)
	(cd ../tigLib/include; rm -f *~)
	(cd ../tigLib/lib/QNX; make clean)
	@echo "\nCLEANING ACTIONS LIB\n"
	(cd ../act; rm -f *~; make clean)
	@echo "\nCLEANING REX LIB\n"
	(cd ../rexLib; rm -f *~; make clean)
	@echo "\nCLEANING INT LIB\n"
	(cd ../int; rm -f *~; make clean)
	@echo "\nCLEANING DATA\n"
	(cd ../data; rm -f *~; make clean)
	@echo "\nCLEANING SCRIBE\n"
	(cd ../scribe; rm -f *~ *.o *.map *.err scribe)
	@echo "\nCLEANING PROCSWITCH\n"
	(cd ../procSwitch/src; rm -f *~)
	(cd ../procSwitch/src/gcc_ntox86; make clean)
	@echo "\nCLEANING EWIND\n"
	(cd ../ewind/src; rm -f *~)
	(cd ../ewind/src/gcc_ntox86; make clean)
	@echo "\nCLEANING RLINE\n"
	(cd ../rline/src; rm -f *~)
	(cd ../rline/src/gcc_ntox86; make clean)
	@echo "\nCLEANING RAS\n"
	(cd ../ras/src; rm -f *~)
	(cd ../ras/src/gcc_ntox86; make clean)
	@echo "\nCLEANING XCORR\n"
	(cd ../xcorr/src; rm -f *~)
	(cd ../xcorr/src/gcc_ntox86; make clean)
	@echo "\nCLEANING REBEL\n"
	(cd ../rebel/src; rm -f *~)
	(cd ../rebel/src/gcc_ntox86; make clean)
	@echo "\nCLEANING TOOLBAR\n"
	(cd ../toolBar/src; rm -f *~)
	(cd ../toolBar/src/gcc_ntox86; make clean)
	@echo "\nCLEANING SPOT\n"
	(cd ../spot; make clean)
	@echo "\nCLEANING LIBLABTOOLS\n"
	(cd ../labTools; rm -f *~ *.o *.map *.err)
	@echo "\nCLEANING CONFIGREX\n"
	(cd ../configRex/src; rm -f *~)
	(cd ../configRex/src/gcc_ntox86; make clean)

#
# Makefile for the TIG standard library
#
# 11 Jan 88	LMO	port to Sun server
#  9 Sept 88    BJZ     port to Sparc architecture

ifndef DEBUG
DEBUG = -g
endif

ifndef OPTIM
OPTIM = -O
endif

PLATFORM=gcc_ntox86

# CC = qcc
# LD = qcc
# CFLAGS  = -V$(PLATFORM) -w -c $(OPTIM) $(DEBUG) -I ../../include

CC = gcc
LD = gcc
CFLAGS  = -w -c $(OPTIM) $(DEBUG) -I ../../include

L = ../../../lib
H = ../../include
U = /usr/include

OUTPUT = libtig.a

#
# this is the string putsymb() uses to find fonts
#
FONTS = \"/rex/lib/tig/\"

OBJ =   aton.o axis.o box.o dismap.o dtoa.o ellipse.o err.o \
	fctexp.o fctlin.o fctpow.o fit.o fixmode.o fsgn.o \
	gdframe.o get_tig_wd.o line.o marker.o minmax.o ncvt.o \
	ntoa.o numnice.o output.o plotf.o pltstr.o polar.o prints.o \
	putplt.o putsymb.o surface.o tig.o tig.dat.o tigconv.o \
	write_direc.o writs.o 

all:	$L/$(OUTPUT)

# $L/$(OUTPUT):	$(OBJ)
# 	$(LD) -V$(PLATFORM) $(OBJ) -A $L/$(OUTPUT)

$L/$(OUTPUT):	$(OBJ)
	ar rvs $L/$(OUTPUT) $(OBJ)

aton.o:	../aton.c $(H)/tig.h  
	$(CC) $(CFLAGS) ../aton.c

axis.o:	 ../axis.c $(H)/tig.h 
	$(CC) $(CFLAGS) ../axis.c

box.o:	../box.c $(H)/tig.h 
	$(CC) $(CFLAGS) ../box.c

dismap.o: ../dismap.c $(H)/tig.h 
	$(CC) $(CFLAGS) ../dismap.c

dtoa.o:	../dtoa.c $(H)/tig.h 
	$(CC) $(CFLAGS) ../dtoa.c

ellipse.o: ../ellipse.c $(H)/tig.h 
	$(CC) $(CFLAGS) ../ellipse.c

err.o:	../err.c $(H)/tig.h 
	$(CC) $(CFLAGS) ../err.c

fctexp.o: ../fctexp.c $(H)/tig.h 
	$(CC) $(CFLAGS) ../fctexp.c

fctlin.o: ../fctlin.c $(H)/tig.h 
	$(CC) $(CFLAGS) ../fctlin.c

fctpow.o: ../fctpow.c $(H)/tig.h 
	$(CC) $(CFLAGS) ../fctpow.c

fit.o:	../fit.c $(H)/tig.h 
	$(CC) $(CFLAGS) ../fit.c

fixmode.o: ../fixmode.c $(H)/tig.h 
	$(CC) $(CFLAGS) ../fixmode.c

fsgn.o:	../fsgn.c $(H)/tig.h 
	$(CC) $(CFLAGS) ../fsgn.c

gdframe.o: ../gdframe.c $(H)/tig.h 
	$(CC) $(CFLAGS) ../gdframe.c

get_tig_wd.o: ../get_tig_wd.c $(H)/tig.h
	$(CC) $(CFLAGS) ../get_tig_wd.c

line.o:	 ../line.c $(H)/tig.h 
	$(CC) $(CFLAGS) ../line.c

marker.o: ../marker.c $(H)/tig.h 
	$(CC) $(CFLAGS) ../marker.c

minmax.o: ../minmax.c $(H)/tig.h 
	$(CC) $(CFLAGS) ../minmax.c

ncvt.o:	../ncvt.c $(H)/tig.h  
	$(CC) $(CFLAGS) ../ncvt.c

ntoa.o:	../ntoa.c $(H)/tig.h 
	$(CC) $(CFLAGS) ../ntoa.c

numnice.o: ../numnice.c $(H)/tig.h 
	$(CC) $(CFLAGS) ../numnice.c

output.o: ../output.c $(H)/tig.h 
	$(CC) $(CFLAGS) ../output.c

plotf.o: ../plotf.c $(H)/tig.h 
	$(CC) $(CFLAGS) ../plotf.c

pltstr.o: ../pltstr.c $(H)/tig.h 
	$(CC) $(CFLAGS) ../pltstr.c

polar.o: ../polar.c $(H)/tig.h 
	$(CC) $(CFLAGS) ../polar.c

prints.o: ../prints.c $(H)/tig.h
	$(CC) $(CFLAGS) ../prints.c

putplt.o: ../putplt.c $(H)/tig.h  
	$(CC) $(CFLAGS) ../putplt.c

putsymb.o: ../putsymb.c $(H)/tig.h 
	$(CC) $(CFLAGS) -DFNTLIB=$(FONTS) ../putsymb.c

surface.o: ../surface.c $(H)/tig.h
	$(CC) $(CFLAGS) ../surface.c

tig.o: ../tig.c $(H)/tig.h 
	$(CC) $(CFLAGS) ../tig.c

tig.dat.o: ../tig.dat.c $(H)/tig.h 
	$(CC) $(CFLAGS) ../tig.dat.c

tigconv.o: ../tigconv.c $(H)/tig.h 
	$(CC) $(CFLAGS) ../tigconv.c

writs.o: ../writs.c $(H)/tig.h 
	$(CC) $(CFLAGS) ../writs.c

write_direc.o: ../write_direc.c $(H)/tig.h
	$(CC) $(CFLAGS) ../write_direc.c

clean:
	  rm -f *.o $L/$(OUTPUT)

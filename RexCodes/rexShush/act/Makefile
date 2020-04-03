#	Makefile for REX action libraries libact.a.

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

OUTPUT = libact.a

#OBJ = awind.o cal.o dout.o ramp_act.o ramp.o score.o sd_set.o ustatus.o stateTime.o \
#	off_eye.o off_oeye.o wd_set.o rl_set.o sf_init.o scntrl.o da_set.o uw_set.o ma_set.o \
#	vexActions.o shuffle.o

OBJ = awind.o cal.o dout.o ramp_act.o ramp.o score.o sd_set.o ustatus.o stateTime.o \
	eye_offsets.o wd_set.o rl_set.o sf_init.o scntrl.o da_set.o uw_set.o ma_set.o \
	vexActions.o shuffle.o

all:	$L/$(OUTPUT)

# $L/$(OUTPUT):	$(OBJ)
# 	$(CC) -V$(PLATFORM) $(OBJ) -A $L/$(OUTPUT)

$L/$(OUTPUT):	$(OBJ)
	ar rvs $L/$(OUTPUT) $(OBJ)

awind.o: awind.c $H/sys.h $H/proc.h $H/menu.h $H/ecode.h $H/buf.h \
		$H/int.h $H/device.h $H/state.h $H/cnf.h $H/idsp.h $H/cdsp.h
	$(CC) $(CFLAGS) awind.c

cal.o: cal.c $H/sys.h $H/int.h $H/proc.h $H/menu.h $H/cnf.h
	$(CC) $(CFLAGS) cal.c

da_set.o: da_set.c $H/sys.h $H/cnf.h $H/device.h $H/proc.h $H/menu.h $H/buf.h \
		$H/idsp.h $H/cdsp.h $H/ramp.h
	$(CC) $(CFLAGS) da_set.c

dout.o:	 dout.c $H/sys.h $H/cnf.h $H/device.h $H/proc.h $H/menu.h $H/buf.h \
		$H/idsp.h $H/cdsp.h 
	$(CC) $(CFLAGS) dout.c

ma_set.o: ma_set.c $H/sys.h $H/cnf.h $H/proc.h $H/menu.h $H/buf.h \
		$H/device.h $H/int.h $H/ecode.h $H/idsp.h $H/cdsp.h
	$(CC) $(CFLAGS) ma_set.c

eye_offsets.o: eye_offsets.c $H/sys.h $H/proc.h $H/menu.h $H/cnf.h $H/buf.h $H/idsp.h $H/cdsp.h \
		$H/device.h $H/int.h
	$(CC) $(CFLAGS) eye_offsets.c

#off_eye.o: off_eye.c $H/sys.h $H/proc.h $H/menu.h $H/cnf.h $H/buf.h $H/idsp.h $H/cdsp.h \
#		$H/device.h $H/int.h
#	$(CC) $(CFLAGS) off_eye.c

#off_oeye.o: off_oeye.c $H/sys.h $H/proc.h $H/menu.h $H/cnf.h $H/buf.h $H/idsp.h $H/cdsp.h \
#		$H/device.h $H/int.h
#	$(CC) $(CFLAGS) off_oeye.c

ramp_act.o: ramp_act.c $H/sys.h $H/cnf.h $H/proc.h $H/menu.h $H/buf.h $H/idsp.h $H/cdsp.h\
		$H/device.h $H/int.h $H/ramp.h $H/ecode.h
	$(CC) $(CFLAGS) ramp_act.c

ramp.o: ramp.c $H/rexhdrs.h $H/ramp.h $H/int.h
	$(CC) $(CFLAGS) ramp.c

rl_set.o: rl_set.c $H/sys.h $H/proc.h $H/menu.h $H/cnf.h $H/idsp.h $H/cdsp.h \
		$H/state.h $H/int.h
	$(CC) $(CFLAGS) rl_set.c

scntrl.o: scntrl.c $H/sys.h $H/proc.h $H/menu.h $H/cnf.h \
		$H/state.h $H/int.h
	$(CC) $(CFLAGS) scntrl.c

score.o: score.c $H/sys.h $H/proc.h $H/menu.h $H/buf.h $H/state.h $H/idsp.h $H/cdsp.h\
		$H/ecode.h $H/int.h	$H/cnf.h
	$(CC) $(CFLAGS) score.c

sd_set.o: sd_set.c $H/sys.h $H/proc.h $H/menu.h $H/idsp.h $H/cdsp.h \
		$H/int.h $H/cnf.h
	$(CC) $(CFLAGS) sd_set.c

sf_init.o:	sf_init.c $H/sys.h $H/proc.h $H/menu.h $H/ecode.h $H/cnf.h \
		$H/state.h $H/int.h
	$(CC) $(CFLAGS) sf_init.c

uw_set.o: uw_set.c $H/sys.h $H/cnf.h $H/proc.h $H/menu.h $H/buf.h \
		$H/device.h $H/int.h $H/ramp.h $H/idsp.h $H/cdsp.h $H/cnf.h
	$(CC) $(CFLAGS) uw_set.c

wd_set.o: wd_set.c $H/sys.h $H/proc.h $H/menu.h $H/buf.h \
		$H/device.h $H/int.h $H/idsp.h $H/cnf.h $H/cdsp.h
	$(CC) $(CFLAGS) wd_set.c

ustatus.o:	ustatus.c $H/sys.h $H/proc.h $H/menu.h $H/cnf.h $H/int.h
	$(CC) $(CFLAGS) ustatus.c

stateTime.o: stateTime.c $H/sys.h $H/proc.h $H/menu.h $H/cnf.h $H/int.h
	$(CC) $(CFLAGS) stateTime.c

vexActions.o: vexActions.c $H/sys.h $H/proc.h $H/menu.h $H/cnf.h $H/int.h
	$(CC) $(CFLAGS) vexActions.c

clean:
	  rm -f *.o $L/$(OUTPUT)

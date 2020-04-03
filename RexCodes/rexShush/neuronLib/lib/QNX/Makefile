#
# neuron library holds commonly used subroutines for neuron programs
#

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

OUTPUT = libneuron.a

OBJ = cal_cov.o symmQR2.o get_KLtrans.o KLT.o filter.o get_fir.o \
	dyn_alloc.o adap_k_est.o sd_est.o sd_alloc.o mk_gauss.o ivunif.o

all:	$L/$(OUTPUT)

# $L/$(OUTPUT):	$(OBJ)
# 	$(LD) -V$(PLATFORM) $(OBJ) -A $L/$(OUTPUT)

$L/$(OUTPUT):	$(OBJ)
	ar rvs $L/$(OUTPUT) $(OBJ)

cal_cov.o: $H/matrix.h ../cal_cov.c
	$(CC) $(CFLAGS) ../cal_cov.c

symmQR2.o: $H/matrix.h ../symmQR2.c
	$(CC) $(CFLAGS) ../symmQR2.c

get_KLtrans.o: $H/matrix.h ../get_KLtrans.c
	$(CC) $(CFLAGS) ../get_KLtrans.c

KLT.o: $H/matrix.h ../KLT.c
	$(CC) $(CFLAGS) ../KLT.c

filter.o: $H/diff.h ../filter.c
	$(CC) $(CFLAGS) ../filter.c

get_fir.o: $H/diff.h ../get_fir.c
	$(CC) $(CFLAGS) ../get_fir.c

dyn_alloc.o: ../dyn_alloc.c $H/matrix.h
	$(CC) $(CFLAGS) ../dyn_alloc.c

adap_k_est.o: ../adap_k_est.c $H/matrix.h
	$(CC) $(CFLAGS) ../adap_k_est.c

sd_est.o: ../sd_est.c $H/matrix.h
	$(CC) $(CFLAGS) ../sd_est.c

sd_alloc.o: ../sd_alloc.c $H/matrix.h
	$(CC) $(CFLAGS) ../sd_alloc.c

mk_gauss.o: ../mk_gauss.c $H/mk_gauss.h
	$(CC) $(CFLAGS) ../mk_gauss.c

ivunif.o: ../ivunif.c
	$(CC) $(CFLAGS) ../ivunif.c

clean:
	  rm -f *.o $L/$(OUTPUT)

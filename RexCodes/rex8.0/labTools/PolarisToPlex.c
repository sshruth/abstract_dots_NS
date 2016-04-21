#include  <unistd.h>
#include  <stdio.h>
#include  <sys/types.h>
#include  <x86/inline.h>
#include  "rexhdrs.h"
#include "PolarisToPlex.h"

/* 
 * 
 * 
 * At the back of the pyramid, the extent in the different dimensions is:
 * 
 * X = 1312
 * Y = 1566
 * Z = 1400 = 2400-950
 * 
 * 
 * 
 */

void sendPolarisPosition(int f, float x, float y, float z) {
	unsigned short usx, usy, usz;
	usx = 2;
	usy = 1;
	usz = 4;
	loadPolarisPlexonBuf(usx,usy,usz);
}

void loadPolarisPlexonBuf(unsigned short x, unsigned short y, unsigned short z) {
	
	int err=0;
	InterruptLock(&i_b->spinLock);
	
	i_b->plpol_buf[i_b->plpol_buflx] = x;
	if(++i_b->plpol_buflx >= PLPOL_MAXPOSITIONS) i_b->plpol_buflx = 0;
	if(i_b->plpol_buflx == i_b->plpol_bufdx) err = 1;
	
	i_b->plpol_buf[i_b->plpol_buflx] = y;
	if(++i_b->plpol_buflx >= PLPOL_MAXPOSITIONS) i_b->plpol_buflx = 0;
	if(i_b->plpol_buflx == i_b->plpol_bufdx) err = 1;
	
	i_b->plpol_buf[i_b->plpol_buflx] = z;
	if(++i_b->plpol_buflx >= PLPOL_MAXPOSITIONS) i_b->plpol_buflx = 0;
	if(i_b->plpol_buflx == i_b->plpol_bufdx) err = 1;

	
	InterruptUnlock(&i_b->spinLock);	

	if (err) {
		rxerr("Plexon Polaris buffer overflow");
	}
}



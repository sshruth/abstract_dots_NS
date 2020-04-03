/*
 *-----------------------------------------------------------------------*
 * NOTICE:  This code was developed by the US Government.  The original
 * versions, REX 1.0-3.12, were developed for the pdp11 architecture and
 * distributed without restrictions.  This version, REX 4.0, is a port of
 * the original version to the Intel 80x86 architecture.  This version is
 * distributed only under license agreement from the National Institutes 
 * of Health, Laboratory of Sensorimotor Research, Bldg 10 Rm 10C101, 
 * 9000 Rockville Pike, Bethesda, MD, 20892, (301) 496-9375.
 *-----------------------------------------------------------------------*
 */

/*
 * Dump event buffer to disk.
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>
#include "rexhdrs.h"
#include "scribe.h"

static char ef_err[] = "dumpe(): Err on write of Efile, possibly out of space";

void
dumpe(int flush)
{
	EVENT_P ep;
	unsign seqnum;
	INT_BLOCK_P I_B= i_b;
	unsign dump1, dump2, ndump, nextdx, lx, dx, wcnt;
	EVENT_P nextdx_adr, ebufend_adr;
	PROCTBL_P p= &i_b->ptbl[myptx];

	lx= I_B->evlx;
	dx= I_B->evdx;
	if(lx == dx) return;		/* nothing to do */

	/*
	 * If an error has occurred in interim lx, dx may not be valid.
	 * Check and abort if necessary.
	 */
	if((derror & (E_EVERR|E_EBADW))
		|| (p->p_msg & s_(SC_EVERR)) ) return;

	/*
	 * Determine if dump wraps around end of event buffer.  If so
	 * two writes are required.
	 */
	if(lx > dx) {
		dump1= lx - dx;
		dump2= 0;		/* no wrap */
	} else {
		dump1= EBUFNUM - dx;
		dump2= lx;
	}
	ndump= dump1 + dump2;		/* total events to dump */

	/*
	 * If not flushing, dont write unless a minimum of EDUMPINC/2 is
	 * ready.
	 */
	if( ! flush) {
		if(ndump < EDUMPINC/2) return;

		/*
		 * Write out in multiples of 512 for efficiency.
		 * After one wrap, evdx should align on block boundaries
		 * so that when (dump2 != 0) dump1 will be a multiple of
		 * 512.  This is because the event buffer is an integral
		 * multiple of 512.
		 */
		if(dump2 == 0) {		/* single write */
			dump1 -= (dump1 % EV_BLOCK);
		} else {			/* perhaps two writes */
			if(dump2 >= EV_BLOCK) dump2 -= (dump2 % EV_BLOCK);
			else dump2= 0;
		}
		ndump= dump1 + dump2;		/* update ndump */
	}

	/*
	 * Insert sequence numbers.
	 */
	nextdx= dx + ndump;
	if(nextdx >= EBUFNUM) nextdx -= EBUFNUM;
	seqnum= I_B->seqnum_e;

	/*
	 * Set up some constants (else V7 C compiler will re-evaluate them
	 * each time through the loop).
 	 * Take a local copy of e_seqnum and put in register for speed.  This
	 * is okay because e_seqnum should not be modified anywhere else but
	 * here.
	 */
	nextdx_adr= &I_B->bevent[nextdx];
	ebufend_adr= &I_B->bevent[EBUFNUM];
	for(ep= &I_B->bevent[dx]; ;) {
		if(ep == nextdx_adr) break;
		ep->e_seqnum= seqnum++;
		if(++ep == ebufend_adr) ep= &I_B->bevent[0];
	}
	I_B->seqnum_e= seqnum;

	wcnt= dump1 * EVSIZE;
	if(write(event_fd, &I_B->bevent[dx], wcnt) != wcnt) {
		dumperr(E_EBADW);
		rxerr(ef_err);
		return;
	}
	if(dump2) {
		wcnt= dump2 * EVSIZE;
		if(write(event_fd, &I_B->bevent[0], wcnt) != wcnt) {
			dumperr(E_EBADW);
			rxerr(ef_err);
			return;
		}
	}
	I_B->evdx= nextdx;		/* set new dump index */
}

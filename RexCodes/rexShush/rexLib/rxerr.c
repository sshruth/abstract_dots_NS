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

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <x86/inline.h>
#include "rexhdrs.h"

/*
 * Function p_rxerr() is a part of comm only and is only called by
 * rxerr() in comm.  Pointer is reinitialized by comm to point to
 * p_rxerr() function.
 */
extern int nullf();
int (*p_err)()= &nullf;
int inside_int= 0;
int inside_clock= 0;
int rxerr_waiting= 0;	/* set when rxerr was called while interrupts
			   were disabled and the message had to be stored
			   in memory; this is looked at in clock.c and
			   a message is printed to examine the incore
			   error buffer */

/*
 * Rex error reporting.  If this process is not the comm process send
 * error message to comm thru shared area in INT_BLOCK.  If this process
 * is comm call error processor directly.
 *
 * If called from the interrupt level, a message is just printed
 * via 'dputs'.
 */
/*
 * Pointers to local automatic storage must be far when routine is
 * called from interrupt level.
 */
int rt_abort_read= 0;	    /* Set when error occurs to immediately
			       abort further root reading.  When 'comm'
			       awakes to process a rxerr it aborts
			       root reading, but this can take some time
			       and additional reads might occur before
			       comm awakes */
void
/* rxerr(char far *s) */
rxerr(char *s)
{
	PROCTBL_P comm_p= COMM;

	if(infd) rt_abort_read= 1;
	if(myptx != 0) {	/* not comm process */
		if((myptx == i_b->ph_int_pi) && inside_int ||
		  ((pswget() & INTR_IF) == 0)) {
			/*
		     * System calls cannot be executed when in the lower
		     * level or when interrupts are disabled.  This
		     * message will not print at the top of the screen.
		     */
		    dputs(s);
		    dputchar('\n');
		    rxerr_waiting= 1;
		    return;
		}
		for(;;) {

			/*
			 * Wait for errstr[] to free.  This is a
			 * critical section with comm and must be
			 * protected.
			 */
			protect(&comm_p->p_sem);
			if(i_b->i_errstr[0] == '\0') break;	/* free */
			release_(&comm_p->p_sem);
			sleep(1);
		}
		i_b->i_errptx= myptx;		/* record who I am */
		/* 		_fstufs(s, &i_b->i_errstr, &i_b->i_errstr[P_ISLEN]); */
		stufs(s, &i_b->i_errstr, &i_b->i_errstr[P_ISLEN]);
		release_(&comm_p->p_sem);
		r_sendmsg(comm_p, CM_RXERR);

	} else {

		/*
		 * COMM process.  Call p_rxerr() directly. 
		 */
		(*p_err)(s, 0);
	}
}

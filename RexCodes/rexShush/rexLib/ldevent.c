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
 * Load event.  Use 'int_ldevent_()' macro to load events at interrupt
 * level.
 */

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <x86/inline.h>
#include "rexhdrs.h"

void
ldevent(EVENT *evp)
{
	int scrb_msg0 = 0, scrb_msg1= 0;
	unsigned eflags;
	
	/*
	 * Must be done with interrupts disabled.  Critical
	 * sections with interrupt routine, or scheduling of other processes
	 * which might call ldevent().
	 */
 	if((eflags= pswget()) & INTR_IF) InterruptDisable();
	i_b->bevent[i_b->evlx]= *evp;
	if(++i_b->evlx >= EBUFNUM) i_b->evlx= 0;
	if(i_b->evlx == i_b->evdx) scrb_msg0= SC_EVERR;
	if(i_b->evlx == i_b->evdump) {    /* time to set new dump? */
		i_b->evdump += EDUMPINC;
		if(i_b->evdump >= EBUFNUM) i_b->evdump -= EBUFNUM;

		/*
		 * If writing to disk, alert scribe.
		 */
		if(i_b->i_flags & I_EOUT) scrb_msg1= SC_EDUMP;
	}

#ifdef PLEX_ECODES
	/* 
	 * Send ecode to Plexon box. Don't send unit ecodes. 
	 * Don't send negative ecodes, because only lower 15 bits of 
	 * code can be sent to Plexon box.  Critical section- this must be done
	 * high priority to avoid conflict with other places events are sent
	 * in clock and in lower level with int_ldevent_().
	 */ 
	if( (evp->e_code >= 0)) {
	   /* if(pl_box_ready_) {*/
		pl_send_code_(evp->e_code);	/* send code directly */
	  /*  } else {	     buffer code to be sent later */
	  /* i_b->pl_hipri[i_b->pl_hilx]= evp->e_code;
		* if(++i_b->pl_hilx >= PL_MAXCODES) i_b->pl_hilx= 0;
		* if(i_b->pl_hilx == i_b->pl_hidx)
		*     rxerr("Plexon hi pri buffer overflow");  overflow error 
	    * }
	   */
	}
#endif

 	if(eflags & INTR_IF) InterruptEnable(); 
	if(scrb_msg0) r_sendmsg(SCRB, scrb_msg0);
	if(scrb_msg1) r_sendmsg(SCRB, scrb_msg1);
}

/*
 * Load events into event buffer and send to plexon box at low priority.  If flag is
 * true, then the event is not entered into the REX event buffer, but rather just sent
 * to the Plexon box.  This permits a full 15 bit value to be sent to the Plexon box,
 * HOWEVER it is not entered into the REX data files.
 *
 * This routine is used in the following way:

	EVENT ev;
	int flag;

	flag= your_choice;
	ev.e_code= code_to_send;
	ev.e_key= i_b->i_time;
	ldevent_plexon_low(&ev, flag)
	
 */
#ifdef PLEX_ECODES
void
ldevent_plexon_low(EVENT *evp, int flag)
{
	int scrb_msg0 = 0, scrb_msg1= 0;
	unsigned eflags;
	
	/*
	 * Must be done with interrupts disabled.  Critical
	 * sections with interrupt routine, or scheduling of other processes
	 * which might call ldevent().
	 */
	if((eflags= pswget()) & INTR_IF) InterruptDisable();
	if(!flag) {
	    i_b->bevent[i_b->evlx]= *evp;
	    if(++i_b->evlx >= EBUFNUM) i_b->evlx= 0;
	    if(i_b->evlx == i_b->evdx) scrb_msg0= SC_EVERR;
	    if(i_b->evlx == i_b->evdump) {    /* time to set new dump? */

		i_b->evdump += EDUMPINC;
		if(i_b->evdump >= EBUFNUM) i_b->evdump -= EBUFNUM;

		/*
		 * If writing to disk, alert scribe.
		 */
		if(i_b->i_flags & I_EOUT) scrb_msg1= SC_EDUMP;

	    }
	}

	/* 
	 * Send ecode to Plexon box.
	 * Don't send negative ecodes, because only lower 15 bits of 
	 * code can be sent to Plexon box.  Critical section- this must be done
	 * high priority to avoid conflict with other places events are sent
	 * to plexon box.  Don't send ecode directly to Plexon box from here-
	 * just buffer it.  This lets high priority ecodes have precendence.
	 */ 
	if( (evp->e_code >= 0) ) {
		i_b->pl_lopri[i_b->pl_lolx]= evp->e_code;
		if(++i_b->pl_lolx >= PL_MAXCODES) i_b->pl_lolx= 0;
		if(i_b->pl_lolx == i_b->pl_lodx)
		    rxerr("Plexon lo pri buffer overflow"); /* overflow error */
	}

	if(eflags & INTR_IF) InterruptEnable();
	if(scrb_msg0) r_sendmsg(SCRB, scrb_msg0);
	if(scrb_msg1) r_sendmsg(SCRB, scrb_msg1);
}
#endif

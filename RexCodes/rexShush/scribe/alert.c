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
 * Scribe interprocess communication.
 *
 *	This alert() is commented extensively and can be used as an example
 * for alert()s for new REX processes.  See also comments in rlib/r_sendmsg.c
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include "rexhdrs.h"
#include "tty.h"
#include "scribe.h"

#undef  A_DEBUG			/* define for debugging info */

/*
 * Re-interrupt mask for noun-verb, menu processing.
 */
#define NVMASK	(s_(G_NMEXEC))	/* allow everything (but myself) to re-interrupt
				   noun-verb, menu processing */
extern sigset_t	alert_set;

/*
 * Alert() implements a priority based, reentrant schecduling mechanism that
 * functions as the interprocess communication facility for REX processes.
 * The priority of a message is determined by when it is scanned.
 * Messages scanned first have higher priority.  Any function initiated
 * by a message may allow re-interrupting by other messages to preempt it.
 * This is accomplished by specifing a re-interrupt mask that has a bit
 * cleared for each message that CAN re-interrupt.  As re-interrupts 
 * nest bits previously set in the re-interrupt mask are protected from
 * being cleared.  This guarentees that deeper levels cannot
 * permit a message to be processed that has been previously masked out.
 */
void
alert(int sig)
{
	PROCTBL *p;
	u_int msg, restormask;
	int (*func)();
	int nindex, nuser;
	char vstr[P_ISLEN], astr[P_ISLEN];
	extern ME_ACCP me_accp;

	/*
	 * Protect against reentrancy during critical sections.  This
	 * can occur because re-interrupts are permitted at various
	 * times.
	 */
#ifdef A_DEBUG
dputs("\n***alert: entered-");
#endif
	p = &i_b->ptbl[myptx];
	if(p->p_state & P_ALRTBSY_ST) return;
	p->p_state |= P_ALRTBSY_ST;

newscan:

	/*
	 * Scanning for messages must be protected for re-interrupting to
	 * work properly.  For example, assume that scanning was proceeding
	 * and only the G_NMEXEC msg was set.  At the point after the test for
	 * the G_NMEXEC is made, but before re-interrupting is enabled, another
	 * process may send a higher pri message.  At this point that process
	 * would not signal because the P_NOSIG_ST bit is still on.  Therefore
	 * the noun, verb would be processed and the higher pri msg would
	 * not preempt.
	 */
#ifdef A_DEBUG
dprintf("-alert: newscan, msg 0%o-", p->p_msg);
#endif
	protect(&p->p_sem);
	msg= p->p_msg & ~p->p_rmask;	/* mask with re-interrupt mask */
#ifdef A_DEBUG
dprintf("-alert: masked msg 0%o-", msg);
#endif
	if(msg == 0) {
		/*
		 * Alert MUST return from here only.
		 */
		p->p_state &= ~(P_NOSIG_ST|P_ALRTBSY_ST);
		release_(&p->p_sem);
#ifdef A_DEBUG
dputs("-alert: before return***\n");
#endif
		return;
	}
	else if(msg & s_(G_KILL)) {
		p->p_msg &= ~s_(G_KILL);
		release_(&p->p_sem);
		p->p_state |= P_EXIT_ST;	/* let comm know that I'm
						   terminating */
		exit(0);

	}
	else if(msg & s_(G_STOP)) {		
		p->p_msg &= ~s_(G_STOP);
		release_(&p->p_sem);
		if(i_b->i_flags & I_GO) {
			rxerr("alert(): Cannot stop scribe if clock is going");
			r_sendmsg(&i_b->ptbl[i_b->ph_switch_pi], CM_NEG);
			goto newscan;
		}
		i_b->scrb_pi= -1;

		/*
		 * If currently reading from a root file, close root.
		 */
		if(infd != 0) rt_close();
		r_sendmsg(&i_b->ptbl[i_b->ph_switch_pi], CM_AFFIRM);
	}
	else if(msg & s_(G_RUN)) {
		p->p_msg &= ~s_(G_RUN);
		release_(&p->p_sem);
		if(i_b->scrb_pi > 0) {
			rxerr("alert(): A scribe process is already running");
			r_sendmsg(&i_b->ptbl[i_b->ph_switch_pi], CM_NEG);
			goto newscan;
		}
		i_b->scrb_pi= myptx;

		/*
		 * If currently reading from a root file, open root.
		 */
		if(i_b->i_rtflag & RT_READ) rt_read();
		r_sendmsg(&i_b->ptbl[i_b->ph_switch_pi], CM_AFFIRM);
	}
	else if(msg & s_(SC_ANERR)) {
		p->p_msg &= ~s_(SC_ANERR);
		release_(&p->p_sem);
		dumperr(E_ANERR);
		rxerr("alert(): analog buffer error, files closed\nclick Print Debug button in Process Controls dialog");
		r_sendmsg(&i_b->ptbl[i_b->ph_switch_pi], CM_ABORT);
	}
	else if(msg & s_(SC_EVERR)) {
		p->p_msg &= ~s_(SC_EVERR);
		release_(&p->p_sem);
		dumperr(E_EVERR);
		rxerr("alert(): event file error, files closed");
		r_sendmsg(&i_b->ptbl[i_b->ph_switch_pi], CM_ABORT);
	}
	else if(msg & s_(SC_ADERR)) {
		p->p_msg &= ~s_(SC_ADERR);
		release_(&p->p_sem);
		dumperr(E_ADERR);
		rxerr("alert(): A/D device error, files closed\nclick Print Debug button in Process Controls dialog");
		r_sendmsg(&i_b->ptbl[i_b->ph_switch_pi], CM_ABORT);
	}
	else if(msg & s_(SC_EDUMP)) {
		p->p_msg &= ~s_(SC_EDUMP);
		release_(&p->p_sem);

		/*
		 * Check I_EOUT to prevent re-interrupting the dump that
		 * that is called to flush buffer when the ignore or close verbs
		 * are being processed.
		 */
		if((i_b->i_flags & I_EOUT) && (derror == 0)) dumpe(0);
	}
	else if(msg & s_(SC_ADUMP)) {
		p->p_msg &= ~s_(SC_ADUMP);
		release_(&p->p_sem);

		/*
		 * Afile dumping can continue after I_AOUT flag is
		 * cleared until write stack is emptied.
		 */
		if(derror == 0) dumpa();

	}
	else if(msg & s_(G_RTEXEC)) {
		p->p_msg &= ~s_(G_RTEXEC);
		release_(&p->p_sem);

		if(i_b->i_rtflag & RT_READ) rt_read();
		else if(i_b->i_rtflag & (RT_WMENU|RT_WUMENU)) rt_write(i_b->i_rtname);
		else if(i_b->i_rtflag & RT_CLOSE) rt_close();
		r_sendmsg(&i_b->ptbl[i_b->ph_switch_pi], CM_AFFIRM);

	}
	else if(msg & s_(G_NMEXEC)) {
		p->p_msg &= ~s_(G_NMEXEC);
		stufs(i_b->i_verbs, vstr, &vstr[P_ISLEN]);
		stufs(i_b->i_args, astr, &astr[P_ISLEN]);

		nindex= i_b->i_nindex;
		nuser= i_b->i_nuser;
		i_b->i_nindex= NULLI;		/* free shared data areas */

		/*
		 * Prepare to allow re-interrupts.
		 * This must be done in section protected from r_sendmsg().
		 */
		restormask= (p->p_rmask ^ NVMASK) & NVMASK;
		p->p_rmask |= NVMASK;
		p->p_state &= ~(P_NOSIG_ST|P_ALRTBSY_ST|P_EARLYWAKE_ST);
		release_(&p->p_sem);
		if(sigprocmask(SIG_UNBLOCK, &alert_set, NULL) == -1)
		    rxerr("alert(): cannot sigprocmask");

		if(*vstr == '\0') {
		    ME_RECUR recur;

		    recur.me_rec_bp= NP;
		    if(nuser) recur.me_rec_menup= &umenus[nindex];
		    else recur.me_rec_menup= &menus[nindex];
		    recur.me_rec_astr= astr;
		    access_m(&me_accp, &recur);
		}
		else {
#ifdef A_DEBUG
dputs("-alert: before func-");
#endif
			if(nuser) func = unouns[nindex].n_ptr;
			else func = nouns[nindex].n_ptr;
			(*func)(vstr, astr);
#ifdef A_DEBUG
dputs("-alert: after func-");
#endif
		}

		/*
		 * Comm goes to sleep after sending a name to free
		 * keyboard.  Some names that do not need the keyboard
		 * may re-awaken comm before returning.  If so, the
		 * P_EARLYWAKE_ST bit is set.  Otherwise, re-awaken comm.
		 */
		if(!(p->p_state & P_EARLYWAKE_ST)) {
#ifdef A_DEBUG
dputs("-alert: before r_sendmsg to comm-");
#endif
			r_sendmsg(&i_b->ptbl[i_b->ph_switch_pi], CM_AFFIRM);
#ifdef A_DEBUG
dputs("-alert: after r_sendmsg to comm-");
#endif
		}
		
		/*
		 * Disable re-interrupts and restore pri mask.
		 */
		if(sigprocmask(SIG_BLOCK, &alert_set, NULL) == -1) {
		    rxerr("alert(): cannot sigprocmask");
		}
		p->p_state |= (P_NOSIG_ST|P_ALRTBSY_ST);
		p->p_rmask &= ~restormask;
	}
	else {		/* bad msg */
		p->p_msg &= ~msg;
		release_(&p->p_sem);
		rxerr("alert(): illegal message");
	}

	/*
	 * After every message is processed a newscan is done to check
	 * for any higher pri messages that may have come.
	 */
	goto newscan;
}

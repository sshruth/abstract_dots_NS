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
 * REX signal handler.
 */
/* Standard headers */
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sched.h>
#include <process.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

/* Toolkit headers */
#include <Ph.h>
#include <Pt.h>
#include <Ap.h>

/* Local headers */
#include "ph_rline.h"
#include "abimport.h"
#include "proto.h"

#undef A_DEBUG			/* define for debugging info */

/*
 * Re-interrupt mask for noun-verb, menu processing.
 */
#define NVMASK	(s_(G_NMEXEC))	/* allow everything (but myself) to re-interrupt
								 * noun-verb, menu processing */
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
#ifdef PHOTON_SIG_COMPAT
int
alert(int sig, void *data)
#else
void
alert(int sig)
#endif
{
	PROCTBL_P p= (PROCTBL_P)&i_b->ptbl[myptx];
	u_int msg;

	/*
	 * Protect against reentrancy during critical sections.  This
	 * can occur because re-interrupts are permitted at various
	 * times.
	 */
#ifdef A_DEBUG
puts("\n***alert: entered-");
#endif

#ifdef PHOTON_SIG_COMPAT
	if(p->p_state & P_ALRTBSY_ST) return(Pt_CONTINUE);
#else
	if(p->p_state & P_ALRTBSY_ST) return;
#endif
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
printf("-alert: newscan, msg 0%o-", p->p_msg);
#endif

	protect(&p->p_sem);
	msg= p->p_msg & ~p->p_rmask;	/* mask with re-interrupt mask */

#ifdef A_DEBUG
printf("-alert: masked msg 0%o-", msg);
#endif

	if(msg == 0) {
		/*
		 * Alert MUST return from here only.
		 */
		p->p_state &= ~(P_NOSIG_ST|P_ALRTBSY_ST);
		release_(&p->p_sem);

#ifdef A_DEBUG
puts("-alert: before return***\n");
#endif

#ifdef PHOTON_SIG_COMPAT
		return(Pt_CONTINUE);
#else
		return;
#endif		

	}
	else if(msg & s_(G_KILL)) {
		p->p_msg &= ~s_(G_KILL);
		release_(&p->p_sem);

		/* let comm know that I'm terminating */
		p->p_state |= P_EXIT_ST;

		PtExit(0);
	}
	else if(msg & s_(G_STOP)) {		
		p->p_msg &= ~s_(G_STOP);
		release_(&p->p_sem);
		i_b->ph_rline_pi= -1;
		r_sendmsg((PROCTBL_P)&i_b->ptbl[i_b->ph_switch_pi], CM_AFFIRM);
	}
	else if(msg & s_(G_RUN)) {
		p->p_msg &= ~s_(G_RUN);
		release_(&p->p_sem);
		if(i_b->ph_rline_pi > 0) {
			rxerr("alert(): A ph_rline process is already running");
			r_sendmsg((PROCTBL_P)&i_b->ptbl[i_b->ph_switch_pi], CM_NEG);
			goto newscan;
		}
		i_b->ph_rline_pi= myptx;

		r_sendmsg((PROCTBL_P)&i_b->ptbl[i_b->ph_switch_pi], CM_AFFIRM);
	}
	else if(msg & s_(G_RTEXEC)) {
		p->p_msg &= ~s_(G_RTEXEC);
		release_(&p->p_sem);
		r_sendmsg((PROCTBL_P)&i_b->ptbl[i_b->ph_switch_pi], CM_AFFIRM);
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

/*
 * Actions on control B.
 */
void
oncntrlb(int sig)
{
	int tmp, old_value;
	char *old_name;

	tmp= sig;
	signal(S_CNTRLB, oncntrlb);
	myptp->p_state |= P_NOSIG_ST;	/* prevent IPC */
	sleep(1);			/* let comm run */
	myptp->p_msg= 0;
	myptp->p_sem= HIGHBIT;
	myptp->p_vindex= 0;
	myptp->p_rmask= 0;
	myptp->p_state &= ~P_NOSIG_ST;
	old_name= doutput_filename;
	doutput_filename= "/tmp/doutput_ph_rline";
	old_value= doutput_tofile;
	doutput_tofile= 1;
	doutput(0);
	doutput_tofile= old_value;
	doutput_filename= old_name;
}

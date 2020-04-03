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
#include <process.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sched.h>

/* Toolkit headers */
#include <Ph.h>
#include <Pt.h>
#include <Ap.h>

/* Local headers */
#include "ph_procSwitch.h"
#include "abimport.h"
#include "proto.h"

extern int sysflags;
extern int rexSession;

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
/*
 * Re-interrupt mask for noun-verb, menu processing.
 */
#define NVMASK	(s_(G_NMEXEC))	/* allow everything (but myself) to re-interrupt
								 * noun-verb, menu processing */

#undef A_DEBUG			/* define for debugging info */

int alert(int sig, void *data)
{
	PtArg_t args[5];
	PROCTBL_P p = (PROCTBL_P)&i_b->ptbl[myptx];
	PROCTBL_P pScribe = (PROCTBL_P)&i_b->ptbl[i_b->scrb_pi];
	u_int msg;
	char winLabel[1028];

	int ph_rxerr(char *s, int ptx);

#ifdef A_DEBUG
	puts("\n***alert: entered-");
#endif
	/*
	 * Protect against reentrancy during critical sections.  This
	 * can occur because re-interrupts are permitted at various
	 * times.
	 */
	if(p->p_state & P_ALRTBSY_ST) {
		return( Pt_CONTINUE );
	}

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
	msg = p->p_msg & ~p->p_rmask;	/* mask with re-interrupt mask */

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

		return( Pt_CONTINUE );
	}
	else if(msg & s_(CM_RXERR)) {
		char msgsav[P_ISLEN];
		int ptxsav;

		p->p_msg &= ~s_(CM_RXERR);
		release_(&p->p_sem);
		stufs((char *)i_b->i_errstr, msgsav, &msgsav[P_ISLEN]);
		ptxsav= i_b->i_errptx;
		ph_rxerr(msgsav, ptxsav);
		i_b->i_errstr[0]= '\0';		/* free i_errstr */
	}
	else if(msg & s_(CM_BADCHILD)) {
		p->p_msg &= ~s_(CM_BADCHILD);
		release_(&p->p_sem);
		sysflags |= C_BADCHILD;
	}
	else if(msg & s_(CM_STATUS)) {
		p->p_msg &= ~s_(CM_STATUS);
		release_(&p->p_sem);
	}
	else if(msg & s_(CM_AFFIRM)) {
		p->p_msg &= ~s_(CM_AFFIRM);
		release_(&p->p_sem);
		sysflags |= C_AFFIRM;
	}
	else if(msg & s_(CM_NEG)) {
		p->p_msg &= ~s_(CM_NEG);
		release_(&p->p_sem);
		sysflags |= C_NEG;
	}
	else if(msg & s_(CM_STARTED)) {
		p->p_msg &= ~s_(CM_STARTED);
		release_(&p->p_sem);
		sysflags |= C_STARTED;

		if(rexSession == 1) {
			readSession();
		}
	}
	else if(msg & s_(CM_ABORT)) {
		p->p_msg &= ~s_(CM_ABORT);
		release_(&p->p_sem);

		PtSetArg(&args[0], Pt_ARG_FILL_COLOR, Pg_RED, 0);
		PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_dataKeepEventsToggle, 2, args);
		PtSetResources(ABW_dataKeepBothToggle, 2, args);
		
		PtSetResource(ABW_base, Pt_ARG_WINDOW_TITLE, "REX", 0);
	}
	else if(msg & s_(CM_PLXHOM)) {
		char msgsav[P_ISLEN];
		int ptxsav;

		p->p_msg &= ~s_(CM_PLXHOM);
		release_(&p->p_sem);

		stufs("Plexon hi pri buffer overflow", msgsav, &msgsav[P_ISLEN]);
		ph_rxerr(msgsav, i_b->i_errptx);
	}
	else if(msg & s_(CM_PLXLOM)) {
		char msgsav[P_ISLEN];
		int ptxsav;

		p->p_msg &= ~s_(CM_PLXLOM);
		release_(&p->p_sem);

		stufs("Plexon lo pri buffer overflow", msgsav, &msgsav[P_ISLEN]);
		ph_rxerr(msgsav, i_b->i_errptx);
	}
	else if(msg & s_(CM_STOPKEEP)) {
		p->p_msg &= ~s_(CM_STOPKEEP);
		release_(&p->p_sem);

		if(i_b->i_flags & I_AOUT) {
			sendnm(pScribe, "Bdata", "i", NS, 0);
		}
		else if(i_b->i_flags & I_EOUT) {
			sendnm(pScribe, "Edata", "i", NS, 0);
		}

		PtSetArg(&args[0], Pt_ARG_FILL_COLOR, Pg_RED, 0);
		PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_dataKeepEventsToggle, 2, args);
		PtSetResources(ABW_dataKeepBothToggle, 2, args);
	}
	else {		/* bad msg */
		p->p_msg &= ~msg;
		release_(&p->p_sem);
		PtNotice(ABW_base, NULL, "Message Warning", NULL,
				 "procSwitch alert() illegal message", "Helvetica09ba",
				 "OK", "Helvetica09ba",
				 Pt_CENTER | Pt_BLOCK_PARENT);
	}

	/*
	 * After every message is processed a newscan is done to check
	 * for any higher pri messages that may have come.
	 */
	goto newscan;
}

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
 * Send interprocess communication messages.
 */

#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include "rexhdrs.h"

#undef  A_DEBUG			/* define for debugging info */

#define BLANKS 18
static char noproc[] = "Process has died:                ";

/*
 * The 'int' process receives a signal each msec that causes the
 * execution of the code in 'clock.c', including the
 * state processor.  A problem arises in sending messages from
 * the clock routine.  The clock routine might have interrupted
 * a call to 'r_sendmsg()' from the upper level of 'int' while 'r_sendmsg()'
 * was in the protected region.  In this case, a message must
 * be queued and delivered later.
 *
 * Following struct holds queued messages.
 */
struct pend {
    struct pend *pend_for;
    struct pend *pend_back;
    int		pend_msg;
    PROCTBL_P	pend_p;
};
typedef struct pend PEND;
PEND pend[P_NPROC];
extern PEND pend_last;
PEND pend_first = {
    &pend_last
};
PEND pend_last = {
    0,
    &pend_first
};
extern int inside_clock, inside_int;
static int do_send(PROCTBL_P p, u_int msg, int msg_flag);

int
r_sendmsg(PROCTBL_P p, u_int msg)
{
    sigset_t oset;
    sigset_t clock_block_set;
    extern sigset_t alert_set;
    int rval, pindex;
    PEND *pp;

    /*
     * 'r_sendmsg()' is not reentrant.
     * 
     * Must block other signals from preempting while in 'r_sendmsg()'.
     * Otherwise 'r_sendmsg()' might be called reentrantly.  Note
     * also that the int process is also receiving another signal every
     * msec that invokes the clock routine.  This routine also calls
     * 'r_sendmsg()'.
     */

	/* if called from inside interrupt routine just return an error */
    if(inside_int) {
		rxerr("Cannot call 'r_sendmsg()' from inside interrupt routine");
		return(-1);
    }

	/* if called when NOT inside the clock routine
	 * do this block and return
	 */
    if(!inside_clock) {
		if(sigprocmask(SIG_BLOCK, &alert_set, &oset) == -1) {
			rxerr("r_sendmsg(): cannot sigprocmask");
		}
		
		rval = do_send(p, msg, 0);
		
		if(sigprocmask(SIG_SETMASK, &oset, NULL) == -1) {
			rxerr("r_sendmsg(): cannot sigprocmask");
		}
		return(rval);
    }

    /*
     * Called from inside clock routine of 'int' process.  First
     * see if this process has pending messages that haven't
     * been sent.  Block reinterrupts via clock signal.  Don't
     * need to explicitly block the ALERT signal because it is
     * already blocked when in the clock routine.
     */
    sigemptyset(&clock_block_set);
    sigaddset(&clock_block_set, S_STATESIG);
    if(sigprocmask(SIG_BLOCK, &alert_set, &oset) == -1) {
	    rxerr("r_sendmsg(): cannot sigprocmask");
	}
    pindex= (p - &i_b->ptbl[0]) / sizeof(PROCTBL);
    pp= &pend[pindex];
	
    if(pp->pend_back != NP) {
		pp->pend_msg |= s_(msg);
    }
	else {	/* if there are no pending messages, send this one */
		rval= do_send(p, msg, 0);	/* try to send it */
		if(rval == -2) {	/* if the message couldn't be sent */
			/*
			 * Queue request for another try later.
			 */
			pp->pend_msg |= s_(msg);
			pp->pend_p= p;
			if(pp->pend_back == 0) {
				pp->pend_back= pend_last.pend_back;
				pp->pend_back->pend_for= pp;
				pp->pend_for= &pend_last;
				pend_last.pend_back= pp;
			}
		}
    }

    /*
     * Try to send any pending messages.
     */
    for(pp= pend_first.pend_for; pp->pend_for != NP; pp= pp->pend_for) {
		if((rval= do_send(pp->pend_p, pp->pend_msg, 1)) == 0) {
			/*
			 * Successful- dequeue.
			 */
			pp->pend_back->pend_for= pp->pend_for;
			pp->pend_for->pend_back= pp->pend_back;
			pp->pend_back= NP;
		}
    }

    /*
     * If pending messages were sent, the return value is not
     * accurate.  Okay, becuase instances of sending in the clock
     * routine don't care.
     */
    if(sigprocmask(SIG_SETMASK, &oset, NULL) == -1) {
		rxerr("r_sendmsg(): cannot sigprocmask");
	}
    return(rval);
}

static int
do_send(PROCTBL_P p, u_int msg, int msg_flag)
{
	u_int msgbit;
	int killRet;

#ifdef A_DEBUG
dprintf("-smsg: p %d, msg 0%o-", p, s_(msg));
#endif

	if(p->p_id <= 0) {
		rxerr("r_sendmsg(): Attempt to send msg to non-existant proc");
		return(-1);
	}
	if(msg_flag) msgbit= msg;
	else msgbit= s_(msg);

	/*
	 * Procs that are stopped can receive only certain messages.
	 */
	if(p != COMM && ((p->p_state & P_RUN_ST) == 0) &&
	   ((msgbit & (s_(G_KILL)|s_(G_STOP)|s_(G_RUN))) == 0)) {
		rxerr("r_sendmsg(): Cannot send msg; process is in stop state");
		return(-1);
	}

	if(inside_clock) {
	    /*
	     * This is the 'int' process, and we are inside the clock
	     * routine.  Don't call protect() if it would block!
	     */
		InterruptDisable();
	    if(!p->p_sem) {
			/*
			 * Just return -2.  'clock()' will try again next
			 * interrupt.
			 */
			InterruptEnable();
			return(-2);
	    }
		else protect(&p->p_sem);
		InterruptEnable();
	}
	else {
		protect(&p->p_sem);
	}
	p->p_msg |= msgbit;		/* set message bit */

	/*
	 * Signal sending is interlocked so that a signal is sent only
	 * when necessary to wake up the receiving process, OR re-interrupt
	 * a lower priority message currently executing in the receiving
	 * process.
	 */
	if( ! (p->p_state & P_NOSIG_ST)) {	/* signalling allowed */
		/*
		 * If the receiving process is currently not asleep but
		 * processing a message that has allowed re-interrupts, the
		 * re-interrupt mask will specify which messages are permitted
		 * to signal and cause a re-interrupt.
		 */
		if(msgbit & ~p->p_rmask) {
			p->p_state |= P_NOSIG_ST;
			release_(&p->p_sem);
#ifdef A_DEBUG
dprintf("-smsg: killing %d-", p->p_id);
#endif
            killRet = kill(p->p_id, S_ALERT);
			if(killRet == -1) {
				perror("do_send() kill error");
				stufs(p->p_name, &noproc[BLANKS], &noproc[0] +
								sizeof(noproc));
				rxerr(noproc);
				p->p_state &= ~P_NOSIG_ST;
				return(-1);
			}
			return(0);
		}
	}
	release_(&p->p_sem);
	return(0);
}

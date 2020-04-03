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
 *----------------------------------------------------------------------------*
 *			STATE CHANGE PROCESSING, DISPLAYS
 * These routines have code to protect against reentering; this
 * is not needed currently.  Called every msec from system clock
 * via signal.
 *----------------------------------------------------------------------------*
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/neutrino.h>
#include <hw/inout.h>
#include "rexhdrs.h"
#include "bufm.h"
#include "ecode.h"
#include "sac.h"
#include "pcmsg.h"
#include "int.h"

#include "../sset/ldev.h"
#include "../labTools/udp.h"

#ifdef PCS_SOCKET
#include "pcsSocket.h"
#include "timerFunction.h"
#endif

static ESCAPE *next;
extern int errprt;
extern int rxerr_waiting;

/*
 * Simple table to provide random time escape function for state set.
 */
char rantbl[] = {
	0,2,1,4,1,3,2,3,0,4,3,1,2,0,2,0,4,3,0,1,2,4,1,1,3,0,2,
	4,4,0,2,3,4,2,2,3,0,3,1,2,4,1,3,2,4,2,0,1,0,3,4,0,2,3,2};

/*
 *---------------------------------------------------------------------*
 * Clock.
 *---------------------------------------------------------------------*
 */
timer_t timerId= 0;
struct sigevent timerEvent= {0}, timerEvent_init= {0};
struct itimerspec timerSpec= {0}, timerSpec_init= {0};
struct _clockperiod timePeriod= {0};
const struct _clockperiod timePeriod_init= {0};

#ifdef MEX62_UNITS
MEX_TIME mexTime;
int mexClockSendCtr;
#endif

#define NANO_TO_MSEC 1000000L;
/*
 * Initialize clock and set rate.
 */
void
clk_set(int msec)
{
	int ClockPeriod(clockid_t clock_id, const struct _clockperiod *new, struct _clockperiod *old, int reserved);

    timePeriod = timePeriod_init;
    timePeriod.nsec = msec * NANO_TO_MSEC;
	timePeriod.fract = 0;

	if(ClockPeriod(CLOCK_REALTIME, (const struct _clockperiod *)&timePeriod, (struct _clockperiod *)NULL, 0) == -1)
	  rxerr("clk_set(): cannot set clock period");
}

/*
 * start the clock
 */
void
clk_start(void)
{
	struct sigaction act;
	sigset_t block_set;
	struct sigevent timerEvent;

	/*
	 * Dont allow REX IPC signals while in state processor.
	 * Note that code in sys/rlib/r_sendmsg.c relies on S_ALERT
	 * being blocked during clock routine.
	 */
	sigemptyset(&block_set);
	sigaddset(&block_set, S_ALERT);
	sigaddset(&block_set, S_DEBUG_PRINT);

	act.sa_flags= 0;
	act.sa_mask= block_set;
	act.sa_handler= &msec_clock;
	if(sigaction(S_STATESIG, &act, NULL) == -1)
	    rxerr("clk_start(): cannot sigaction-2");

	clk_set(CLOCK_RATE);

	/*
	 * Create timer.
	 */
	timerEvent= timerEvent_init;	
	/* 	timerEvent.sigev_notify = SIGEV_SIGNAL; */
	timerEvent.sigev_notify = SIGEV_SIGNAL_THREAD;
	timerEvent.sigev_signo= S_STATESIG;
	if((timer_create(CLOCK_REALTIME, &timerEvent, &timerId)) == -1) {
	    rxerr("clk_start(): cannot create timer");
	    return;
	}

	/*
	 * define the type of timer
	 */
	timerSpec= timerSpec_init;	    /* set to zero */
	timerSpec.it_value.tv_sec = 0;
	timerSpec.it_value.tv_nsec = CLOCK_RATE * NANO_TO_MSEC; 
	timerSpec.it_interval.tv_sec = 0;
	timerSpec.it_interval.tv_nsec = CLOCK_RATE * NANO_TO_MSEC;
	if(timer_settime(timerId, 0, &timerSpec, NULL) == -1) {
	    rxerr("clk_start(): cannot arm timer");
	    return;
	}

	i_b->i_flags |= I_GO;
	
	udp_close();
	udp_open(rexIP, udpPORT);

#ifdef MEX62_UNITS
	mexClockSendCtr = 1;
	mexTime.rexFlag = 0;
	mexTime.code = 0x0;
#endif

#ifdef NIH_ACUC
	i_b->inthdr.i_lastCorrect = i_b->i_time;
#endif
}

/*
 * stop the clock
 */
void
clk_stop(void)
{
	struct sigaction act;
	sigset_t clockMask_set;

	if(timer_delete(timerId) == -1) {
	    rxerr("clk_stop(): cannot delete timer");
	}
	
	sigemptyset(&clockMask_set);
	act.sa_flags= 0;
	act.sa_mask= clockMask_set;
	act.sa_handler= SIG_DFL;
	if(sigaction(S_STATESIG, &act, NULL) == -1) {
	    rxerr("clk_stop(): cannot sigaction-1");
	    return;
	}
	clk_set(TIMER_DEFAULT);
	i_b->i_flags &= ~ I_GO;
	
	udp_close();
}

void
msec_clock(int sig)
{
	ESCAPE *escape_4;
	ACTION *act_3;
	int ptemp_3;
	STATE *pnew_3;
	ESCAPE *pesc_3;
	AWAKE *chng_2;
	AWAKE *awake_4;
	EVENT lev;
	long l_time;				    /* local copy of time */
	extern sigset_t sig_state;	    /* mask for S_STATESIG */
	int notify_rxerr;
	int i;

	RT_DISP *thisRtDisp;
	RT_CHAN *thisRtChan;
	RL_CHAN *thisRunLineChan;
	RL_SPIKE *thisRunLineSpike;
	unsigned long *thisRtSpike;
	int spikeWord;
	int spikeBit;

	/*
	 * Note: sometime state processing may take more than one msec.
	 * Therefore, l_time may increase by more than one between
	 * invocations of this routine.
	 */
	inside_clock++;
	l_time= i_b->i_time;		/* pick up current time */

#ifdef NIH_ACUC
	if((l_time - i_b->inthdr.i_lastCorrect) > MAX_TIME) {
		/* code here to signal toolbar to post a warning dialog */
		r_sendmsg(&i_b->ptbl[i_b->ph_int_pi], G_SCORE_ALERT);

		/* reset the last correct trial time */
		i_b->inthdr.i_lastCorrect = l_time;
	}
#endif

	if(inside_clock > 1) rxerr("Clock reentered");

#ifdef HARD_ABORT
	if(dina & ABORT) {
		clk_stop();
		goto clk_done;
	}
#endif
#ifdef SCOPE
	dio_on(SCOPE_L2);
#endif

#ifdef PCMESSAGE
	/*
	 * PC to PC Messaging.
	 */
	pcm_msg_process();
#endif

#ifdef PCS_SOCKET

	/*
	 * PC to PC Messaging using sockets
	 */
#ifdef SOFT_TIMER
	startTimer(0); 	/* profiling function */
#endif	// SOFT_TIMER

	/* process incoming or outgoing messages to vex */
	pcsScktProcessVex();

#ifdef MEX62_UNITS
	/* process incoming or outgoing messages to mex */
	pcsScktProcessMex();

#endif	// MEX62_UNITS

#ifdef SOFT_TIMER
	stopTimer(0); 	/* profiling function */
#endif	// SOFT_TIMER

#endif	// PCS_SOCKET

	if((st_count -= ST_RATE) <= 0) {
		/*
		 * Do not allow to be re-entered on reinterrupts.
		 */
		if(sp_lock) {
			st_count= 1;
			goto l_disp;
		} else st_count= st_rate;

		sp_lock= 1;

		/*
		 * Permit re-interrupts of 1msec clock to keep timer current.
		 */
#ifdef GOO
		if(sigprocmask(SIG_UNBLOCK, &sig_state, NULL) == -1)
			rxerr("msec_clock(): cannot sigprocmask-1");
#endif

		escape_4 = firstesc;
		do {
			/*
			 * Process escapes.
			 */
st_top:

			ptemp_3= escape_4->sflags;
			switch(ptemp_3) {

			case TIME:
				if(escape_4->awakestate->timer <= l_time) goto stchange;
				break;

			case BITON:
				if(*(int *)escape_4->funct & escape_4->smask)
					goto stchange;
				break;

			case BITOFF:
				if(!(*(int *)escape_4->funct & escape_4->smask))
					goto stchange;
				break;

			case EQUAL:
				if(*(int *)escape_4->funct == escape_4->smask)
					goto stchange;
				break;

			case LESSOR:
				if(*(int *)escape_4->funct > escape_4->smask)
					goto stchange;
				break;

			case GREATER:
				if(*(int *)escape_4->funct < escape_4->smask)
					goto stchange;
				break;

			case QUERY:
				if((*(int *)escape_4->funct)-- <= escape_4->smask)
					goto stchange;
				break;

			case FUNC:
				if((*(int (*)())escape_4->funct)() == escape_4->smask)
					goto stchange;
				break;

			default:
				rxerr("msec_clock(): bad escape type");
				break;
			}
			continue;

			/*
			 * Transition to another state.
			 */
stchange:
			if(errprt) {
				stateDebug[escape_4->stateptr->stateIndex].color ^= 1;
				stateDebug[escape_4->stateptr->stateIndex].time = i_b->i_time;

				/*
				  fputs(&escape_4->stateptr->statename[0], stderr);
				  fputc(' ', stderr);
				*/
			}

			/*
			 *	Execute action.
			 */
			if ((act_3 = &escape_4->stateptr->device)->action)
				ptemp_3= (*act_3->action)(act_3->act_code,act_3->act_2code,act_3->act_3code,
										  act_3->act_4code,act_3->act_5code,act_3->act_6code,
										  act_3->act_7code,act_3->act_8code,act_3->act_9code,
										  act_3->act_10code);
			else ptemp_3 = 0;

			/*
			 *	Possibly enter event code.
			 */
			if(escape_4->stateptr->code || escape_4->stateptr->ptrCode || ptemp_3) {
				lev.e_key = l_time;     /* time stamp of the event code */

				if (ptemp_3) {
					lev.e_code = ptemp_3;
					if (ptemp_3 & INIT_MASK) {   /* is there a new init? */
						/* load into gobal location */
						i_b->i_iev.e_key = l_time;
						i_b->i_iev.e_code = ptemp_3;
						r_sendmsg(COMM, CM_STATUS);
					}
				}
				else if(escape_4->stateptr->code) {
					lev.e_code = escape_4->stateptr->code;
				}
				else if(escape_4->stateptr->ptrCode){
					if(*escape_4->stateptr->ptrCode) {
						lev.e_code = *escape_4->stateptr->ptrCode;
					}
				}
				ldevent(&lev);
			}

			/*
			 *	Process running line and escape time.
			 */
			pnew_3 = escape_4->stateptr;
			switch(pnew_3->rlflag) {
			case RL_NOCHANGE:
				break;
			case RL_ABS:
				rl_setbar(pnew_3->rlval);
				break;
			case RL_ADD:
				rl_addbar(pnew_3->rlval);
				break;
			case RL_SUB:
				rl_addbar(-pnew_3->rlval);
				break;
			default:
				break;
			}

			if (pnew_3->preset >= 0)
				escape_4->awakestate->timer= pnew_3->preset + l_time;

			if (pnew_3->random) {
				static char *prant= rantbl;

				ptemp_3 = pnew_3->random;

				switch (*prant) {
				case 0:
					ptemp_3 = 0;
					break;
				case 1:
					ptemp_3 >>= 1;
					ptemp_3 >>= 1;
					break;
				case 2:
					ptemp_3 >>= 1;
					break;
				case 3:
					ptemp_3 >>= 1;
					ptemp_3 += ptemp_3 >> 1;
					break;
				case 4:
					break;
				}

				escape_4->awakestate->timer += ptemp_3;

				if (++prant >= rantbl + sizeof(rantbl))
					prant = rantbl;
			}

			/*
			 * Change escape pointers.  chng_2 points to awakestate of old
			 * state.
			 */
			chng_2 = escape_4->awakestate;

			/*
			 * pesc_3 is address of new state's first escape.
			 */
			pesc_3 = &escape_4->stateptr->escape;

			/*
			 * Find last escape of new state.
			 */
			for(escape_4 = pesc_3; escape_4->nextesc; escape_4 = escape_4->nextesc);

			/*
			 * Get nextesc from last esc of old state, put
			 * in nextesc of last esc of new state,
			 * and set old state's last escape's nextesc to zero.
			 */
			next = escape_4->nextesc = chng_2->lastesc->nextesc;
			chng_2->lastesc->nextesc = 0;

			/*
			 * Change lastesc of new state's awakestate.
			 */
			escape_4->awakestate->lastesc = escape_4;

			/*
			 * If there is a prior chain, link it to new state.
			 */
			if ((awake_4 = (AWAKE *)chng_2->prestate) > (AWAKE *)0)
				awake_4->lastesc->nextesc = pesc_3;
			else firstesc = pesc_3;

			/*
			 * Get back next escape and see if non-zero.
			 */
			if (escape_4 = next) {
				goto st_top;
			} else goto end_behave;

		} while((escape_4 = escape_4->nextesc) != 0);

end_behave:

		/*
		 * Block re-interrupts.
		 */
#ifdef GOO
		if(sigprocmask(SIG_BLOCK, &sig_state, NULL) == -1)
			rxerr("msec_clock(): cannot sigprocmask-2");
#endif

		sp_lock= 0;
	}


	/*
	 *----------------------------------------------------------------------------*
	 *				REAL-TIME DISPLAYS
	 * Table driven running line;  eye position window.  Runs only every "rlrate"
	 * interrupts.
	 *----------------------------------------------------------------------------*
	 */
l_disp:

	/*
	 * Time for displays?
	 */
	if(ds_lock == 0) {
		ds_lock = 1;

		/* push the real-time display stack down one
		 * and increment the push down counter
		 */
		memmove(&i_b->rtDisp[1], &i_b->rtDisp[0], (19 * sizeof(RT_DISP)));
		i_b->rtDspPshDwnCntr++;

		thisRtDisp = &i_b->rtDisp[0];
		
		/* Loop for each active channel. */
		for(i = 0; i < RL_MAXCHAN; i++) {
			thisRunLineChan = &i_b->rl.rl_chan[i];
			if(!thisRunLineChan->rl_addr) continue;	/* don't update undefined channels */

			thisRtChan = &thisRtDisp->rt_chans[i];

			/* update channel value. */
			thisRtChan->level = *(int *)thisRunLineChan->rl_addr;

			/* put up mark */
			if(thisRunLineChan->rl_mark_req) thisRtChan->mark = thisRunLineChan->rl_mark_req;
			else thisRtChan->mark = 0;
			thisRunLineChan->rl_mark_req = 0;
		}

		/* update running line bar level */
		thisRtDisp->rt_bar = i_b->rl.rl_bar;

		/* update spikes */
		for(i = 0; i < RL_MAXSPIKE; i++) {
			spikeWord = i / 32;
			spikeBit = 1 << (i % 32);
			thisRunLineSpike = &i_b->rl.rl_spike[i];
			thisRtSpike = &thisRtDisp->rt_spikes[spikeWord];
						
			if(thisRunLineSpike->rl_spike_req) *thisRtSpike |= spikeBit;
			else *thisRtSpike &= ~spikeBit;
			thisRunLineSpike->rl_spike_req = 0;
		}

		/* update positions of the data cursors */
		for(i = 0; i < WDI_NDATACURS; i++) {
			if(curPtrs[i].x != (int *)NULL) {
				thisRtDisp->rt_cursors[i].x = *curPtrs[i].x;
				thisRtDisp->rt_cursors[i].y = *curPtrs[i].y;
			}
			else {
				thisRtDisp->rt_cursors[i].x = NULLI;
				thisRtDisp->rt_cursors[i].y = NULLI;
			}
		}

		/* updata the positions of the window cursors */
		for(i = CU_WIND0; i < WDI_NCURS; i++) {
			thisRtDisp->rt_cursors[i].x = i_b->wd[i - CU_WIND0].wd_xcurpos;
			thisRtDisp->rt_cursors[i].y = i_b->wd[i - CU_WIND0].wd_ycurpos;
		}

		ds_lock= 0;		    /* done with displays */
	}

 clk_done:
	/*
	 * Return from signal routine.
	 */
	/*
	 * Send signal if requested from interrupt routine.
	 */
	InterruptDisable();

	/*
	 * See if rxerr() was called when it could not print message (i.e.
	 * interrupts were disabled).  If so, message was stored in memory
	 * and print notification.
	 */
	if(rxerr_waiting) {
		notify_rxerr= 1;
		rxerr_waiting= 0;
	} else notify_rxerr= 0;

	if(i_b->int_msg_pid) {
		pid_t tmp;

		tmp= i_b->int_msg_pid;
		i_b->int_msg_pid= 0;
		InterruptEnable();
		kill(tmp, S_ALERT);
	}
	else InterruptEnable();

	if(notify_rxerr)
		rxerr("Low-level errors present in memory buffer, type 'int print_debug'");

#ifdef PLEX_ECODES
	/*
	 * Check to see if ecodes are waiting to be sent to Plexon box.
	 * Give priority to codes in high priority buffer.  This is a critical
	 * region with other places where ecodes may be sent to the Plexon box
	 * and must be run at hi priority.
	 */
	InterruptDisable();
	if(i_b->pl_hidx != i_b->pl_hilx) {

		/* if(pl_box_ready_) { */
		    pl_send_code_(i_b->pl_hipri[i_b->pl_hidx]);
		    if(++i_b->pl_hidx == PL_MAXCODES) i_b->pl_hidx= 0;
		/* } */
	}
	else if(i_b->pl_lodx != i_b->pl_lolx) {

		/* if(pl_box_ready_) { */
		    pl_send_code_(i_b->pl_lopri[i_b->pl_lodx]);
		    if(++i_b->pl_lodx == PL_MAXCODES) i_b->pl_lodx= 0;
		/* } */
	}
	InterruptEnable();
#endif

#ifdef MEX62_UNITS
	/* this code will send mex a time stamp that is 10 milliseconds into the future and a code value
	 * every second. Then, every millisecond, it checks the current time stamp, and when the current
	 * time stamp matches the time sent to mex, it will send the code to a parallel port.
	 * On receiving the time stamp and code, mex will begin looking at its end of the parallel port,
	 * and when it gets the code that matches the one sent with the time stamp, it will set its unit clock
	 */
	if((in8(MEX_CODE_PORT) & 0x0f)) {
		if(!--mexClockSendCtr) {
			mexTime.rexClock = i_b->i_time + 10;
			mexTime.rexFlag = 0;
			mexTime.code = (++mexTime.code & 0x0007);
			pcsSendMex((char *)&mexTime, sizeof(MEX_TIME));
			mexClockSendCtr = MEX_TIMER;
		}
	}
	else {
		mexClockSendCtr = 1;
		mexTime.rexClock = 0;
		mexTime.rexFlag = 0;
		mexTime.code = 0x80;
		out8(MEX_CODE_PORT, 0x80);
	}
#endif

	inside_clock--;
#ifdef SCOPE
	if(inside_clock == 0) dio_off(SCOPE_L2);
#endif
	return;
}

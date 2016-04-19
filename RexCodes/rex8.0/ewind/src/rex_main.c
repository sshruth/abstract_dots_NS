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
 * REX Main function.
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
#include <sys/neutrino.h>

/* Toolkit headers */
#include <Ph.h>
#include <Pt.h>
#include <Ap.h>

/* Local headers */
#include "abimport.h"
#include "ph_ewind.h"
#include "proto.h"

char version[P_LVERSION] = "7.0 Sep 1 2000";
sigset_t debug_set;
sigset_t alert_set;

char i_name[] = I_NAME;
char r_name[] = R_NAME;

char NS[] = "";
PROCTBL_P myptp = NP;
int myptx = -1;
INT_BLOCK_P i_b = 0;
RAW_BLOCK_P r_b = 0;

#ifdef PHOTON_SIG_COMPAT
int alert(int sig, void *data);
#else
void alert(int sig);
#endif

void rex_main(void)
{
	struct sched_param sp;
	int msgReturn;
	
	/* request I/O privity */
	if(ThreadCtl(_NTO_TCTL_IO, 0) == -1) {
		perror("ewind couldn't set I/O privity");
		PtExit(EXIT_FAILURE);
	}

	/*
	 * Map INT_BLOCK and RAW_BLOCK.
	 */
	i_b= map_shared("/INT_BLOCK", sizeof(INT_BLOCK));
	r_b= map_shared("/RAW_BLOCK", sizeof(RAW_BLOCK));
	myptx= i_b->i_ptblx;
	myptp= (PROCTBL_P)&i_b->ptbl[myptx];
	myptp->p_coid = PhGetConnectId();
	stufs(version, myptp->p_version, &myptp->p_version[P_LVERSION]);

	/*
	 * Set priority.
	 */
	sp.sched_priority= PRI_PH_WIN;
	if(sched_setscheduler(0, SCHED_RR, &sp) < 0) {
	    fprintf(stderr, "main(): cannot set priority\n");
	    PtExit(1);
	}

    /* set up signal mask for alert */
	sigemptyset(&alert_set);
	sigaddset(&alert_set, S_ALERT);
	sigemptyset(&debug_set);
	sigaddset(&debug_set, S_DEBUG_PRINT);

#ifdef PHOTON_SIG_COMPAT
	PtAppAddSignalProc(NULL, &alert_set, alert, NULL);
#else
	signal(S_CNTRLC, SIG_IGN);
	signal(S_CNTRLB, oncntrlb);
	signal(S_ALERT, alert);
	signal(S_DEBUG_PRINT, doutput);
#endif
	/*
	 * Respond to procSwitch
	 */
    msgReturn = r_sendmsg((PROCTBL_P)&i_b->ptbl[i_b->ph_switch_pi], CM_STARTED);

	/*
	 * set D_WIND bit in d_flags
	 */
	i_b->d_flags |= D_WIND;
	i_b->d_mwd_disp = NULLI;

	/*
	 * Eye window and window display
	 */
	i_b->t_wrate = 10;		/* check eye windows every 10 msec */
	i_b->w_rate = 16;		/* refresh window display every 27 msec */

	return;
}

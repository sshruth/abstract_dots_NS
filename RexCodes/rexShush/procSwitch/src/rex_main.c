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
 * REX Main function and signal handler.
 */

/* Standard headers */
#include <process.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/neutrino.h>
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

FILE *errout;
INT_BLOCK_P i_b = 0;
RAW_BLOCK_P r_b = 0;
PROCTBL zerop= {
	{ 0 },				/* process id */
	{ 0 },				/* photon connection id */
	{ 0 },				/* process state */
	{ 0 },				/* semaphore */
	{ 0 },				/* messages */
	{ 0 },				/* re-interrupt mask */
	{ (NAME *)NULL },	/* pointer to noun list */
	{ (NAME *)NULL },	/* pointer to menu list */
	{ 0 },				/* verb-noun pair */
	{ 0 },				/* x coordinate of process window */
	{ 0 },				/* y coordinate of process window */
	{ 0 },				/* width coordinate of process window */
	{ 0 },				/* height coordinate of process window */
	{ 0, 0 },			/* pipe file descriptors */
	{ 0 },				/* process name */
	{ 0 },				/* root file name */
	{ 0 },				/* rex version */
};

PROCTBL_P myptp= NP;
FILE *errout;
RTSAV rtsav[P_RTLMAX];
sigset_t alert_set;
sigset_t debug_set;
int waitAck = 0;
int myptx = -1;
int derror = 0;
int sysflags = 0;
int nproc = 0;
int nnoun = 0;
int nmenu = 0;
int rtlevel, rtflag;
int errnum = 0;
int mxmode_st = 0;
char i_name[] = I_NAME;
char r_name[] = R_NAME;
char version[P_LVERSION]= "7.0 Sep 1 2000";
char NS[] = "";

void rex_main(void)
{
	struct sched_param sp;
	extern int (*p_err)();
	int ph_rxerr(char *s, int ptx);

	/* request I/O privity */
	if(ThreadCtl(_NTO_TCTL_IO, 0) == -1) {
		perror("procSwitch couldn't set I/O privity");
		PtExit(EXIT_FAILURE);
	}
	
	/*
	 * Open error file.
	 */
	errout = fopen("/tmp/rxerr", "w");

	/*
	 * Map INT_BLOCK and RAW_BLOCK.
	 */
	i_b = map_shared("/INT_BLOCK", sizeof(INT_BLOCK));
	if(i_b == (INT_BLOCK_P)-1) {
		fprintf(stderr, "procSwitch couldn't map INT_BLOCK: exiting\n");
		PtExit(EXIT_FAILURE);
	}

	r_b = map_shared("/RAW_BLOCK", sizeof(RAW_BLOCK));
	if(r_b == (RAW_BLOCK_P)-1) {
		fprintf(stderr, "procSwitch couldn't map RAW_BLOCK: exiting\n");
		PtExit(EXIT_FAILURE);
	}

    /* set up signal mask for alert and debug */
	sigemptyset(&alert_set);
	sigemptyset(&debug_set);
	sigaddset(&alert_set, S_ALERT);
	sigaddset(&debug_set, S_DEBUG_PRINT);

#ifdef PHOTON_SIG_COMPAT
	/* add a unix signal handler to photon */
	PtAppAddSignalProc(NULL, &alert_set, alert, NULL);
	/*	PtAppAddSignalProc(NULL, &debug_set, doutput, NULL); */
#else
	signal(S_ALERT, alert);
	signal(S_DEBUG_PRINT, doutput);
#endif

	/*
	 * Set priority of procSwitch
	 */
	sp.sched_priority = PRI_PH_SWITCH;
	if(sched_setscheduler(0, SCHED_RR, &sp) < 0) {
	    fprintf(stderr, "main(): cannot set priority\n");
	    PtExit(1);
	}

	/*
	 * make procSwitch's ptbl entry. NOTE procSwitch must always be ptbl[0]
	 */
	myptx = 0;
	myptp = (PROCTBL_P)&i_b->ptbl[myptx];
	*(myptp) = zerop;					/* initialize procSwitch's ptbl entry */
	myptp->p_id = getpid();				/* set procSwitch's process id */
	myptp->p_coid = PhGetConnectId();	/* set procSwitch's process coid */
	myptp->p_sem = HIGHBIT;				/* initialize semaphore */
	strncpy(myptp->p_name, "procSwitch", P_LPROCNAME);

	/* load procSwitch's version information 
	 * into its process table array element
	 */
	strncpy(myptp->p_version, version, P_LVERSION);

	/* p_err = &ph_rxerr; */
	p_err = ph_rxerr;
	
	i_b->ph_switch_pi = myptx;

	nproc++;				/* increment process number */
	myptp->p_state |= P_RUN_ST;

	return;
}

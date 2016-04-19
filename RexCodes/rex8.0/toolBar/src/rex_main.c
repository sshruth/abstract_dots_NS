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
 * REX Main function and signal handler
 */

/* Standard headers */
#include <process.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <signal.h>
#include <pthread.h>
#include <sys/neutrino.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sched.h>

/* Toolkit headers */
#include <Ph.h>
#include <Pt.h>
#include <Ap.h>

/* Local headers */
#include "ph_toolBar.h"
#include "abimport.h"
#include "proto.h"

extern WIN_DATCURS localCursTyp;
extern RL localRl;
extern char version[];
extern sigset_t	alert_set;

sigset_t die_set;
struct sigaction die;

extern int PciScan(void);

sigset_t debug_set;

pthread_t clockThreadId;
void clockThread(void *arg);

void rex_main(void)
{
	extern GVPTR gv_mem_src[];

	struct sched_param sp;
	struct sched_param clockPriority;
	int msgReturn;
	pthread_attr_t clockAttributes;
	int i;

	/* request I/O privity */
	if(ThreadCtl(_NTO_TCTL_IO, 0) == -1) {
		perror("process couldn't set I/O privity");
		PtExit(EXIT_FAILURE);
	}

	/*
	 * Map INT_BLOCK and RAW_BLOCK.
	 */
	i_b = map_shared("/INT_BLOCK", sizeof(INT_BLOCK));
	r_b = map_shared("/RAW_BLOCK", sizeof(RAW_BLOCK));
	myptx= i_b->i_ptblx;
	myptp= &i_b->ptbl[myptx];
	myptp->p_coid = PhGetConnectId();
	myptp->p_state |= P_INTPROC_ST;

	/* set the int process pointer */
	if(myptp->p_state & P_RUN_ST) i_b->ph_int_pi = myptx;

	stufs(version, myptp->p_version, &myptp->p_version[P_LVERSION]);

    /* set up signal mask for alert */
	sigemptyset(&alert_set);
	sigaddset(&alert_set, S_ALERT);
	sigemptyset(&debug_set);
	sigaddset(&debug_set, S_DEBUG_PRINT);

#ifdef PHOTON_SIG_COMPAT
	PtAppAddSignalProc(NULL, &alert_set, alert, NULL);
#else
	signal(S_ALERT, alert);
	signal(S_DEBUG_PRINT, doutput);
#endif

	/* set up signal mask for the die set */
	/*
	  sigemptyset(&die_set);
	  sigaddset(&die_set, SIGFPE);
	  sigaddset(&die_set, SIGSEGV);
	  die.sa_flags = 0;
	  die.sa_mask = die_set;
	  die.sa_handler = &death;
	  sigaction(SIGFPE, &die, NULL);
	  sigaction(SIGSEGV, &die, NULL);
	*/
	/*
	 * Set priority of 'int'.
	 */
	sp.sched_priority= PRI_PH_INT;
	if(sched_setscheduler(0, SCHED_RR, &sp) < 0) {
	    fprintf(stderr, "toolBar main(): cannot set priority\n");
	    PtExit(1);
	}

	/* set eye window pointers in gv_mem_src */
	gv_mem_src[8].gv_ptr = &i_b->wd[0].wd_xcurpos;
	gv_mem_src[9].gv_ptr = &i_b->wd[0].wd_ycurpos;
	gv_mem_src[10].gv_ptr = &i_b->wd[1].wd_xcurpos;
	gv_mem_src[11].gv_ptr = &i_b->wd[1].wd_ycurpos;

	stateinit();		/* initialize the state lists */

	p_mksroot = &mksroot;	/* set pointer to root writing function */

   	PciScan();	    /* scan PCI bus for devices */
	dio_init();	    /* initialize digital i/o index list */
	da_init();	    /* initialize d/a converters */
	wd_init();	    /* initialize eye windows */
	ra_init();	    /* initialize ramps */
	ma_init();	    /* initialize memory arrays */


	null_sig_arrays();  /* initialize signal arrays */

	/* initial the display variables in shared memory */
	i_b->rl = rl_init;
	i_b->cursTyp = cursTyp_init;
	localRl = rl_init;
	localCursTyp = cursTyp_init;

#ifdef PCMESSAGE
	pcm_init(-1);	    /* initialize PC to PC messaging */
#endif

#ifdef PCS_SOCKET
	pcsVexSocket();

#ifdef MEX62_UNITS
	pcsMexSocket();
#endif

#ifdef SOFT_TIMER

 	timerFunction(3);
	initThisTimerHisto(0, "pcsScktProcess", 16, 20);
	initThisTimerHisto(1, "recvfrom", 16, 20);
	initThisTimerHisto(2, "sendto", 16, 20);
#endif
#endif

#ifdef EXT_CTR
	if(i_b->i_time == 0) {
	    dio_on(EXT_CTR_RESET);
	    delay(100);
	    dio_off(EXT_CTR_RESET);
	}
#endif

	/* initialize the indices in the state arrays */
	i = 0;
	while(snames[i]) {
		snames[i]->stateIndex = i;
		i++;
	}

	/* 
	 * create a thread for the msec clock
	 */
	/* set up the thread attributes structures */
	pthread_attr_init(&clockAttributes);
	pthread_attr_setdetachstate(&clockAttributes, PTHREAD_CREATE_DETACHED);
	pthread_attr_setinheritsched(&clockAttributes, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setschedpolicy(&clockAttributes, SCHED_FIFO);
	clockPriority.sched_priority = PRI_PH_CLKTHRD;
	pthread_attr_setschedparam(&clockAttributes, &clockPriority);

	/* start the thread */
	pthread_create(&clockThreadId, &clockAttributes, clockThread, NULL);

	/*
	 * Check if invoked from a root file.
	 */
	if(i_b->i_rtflag & RT_READ) rt_read();

	return;
}

/*
 * Called when clock thread is created.  This is a separate process
 * from the above code!
 */
void
clockThread(void *arg)
{
	extern int clockSwitched;
	extern int clockState;

	while(myptp->p_id) {
		pthread_sleepon_lock();

		/* wait for user to switch the msec clock */
		while(!clockSwitched) {
			pthread_sleepon_wait(&clockSwitched);
		}

		/* switch the clock */
		if(clockState) {
			/* start the msec clock first */
			clk_start();

			/* start the a/d interrupts */
			ad_start();
		}
		else {
			/* reset the a/d */
			ad_stop();

			/* stop the clock */
			clk_stop();
		}

		/* reset the clock switch flag */
		clockSwitched = 0;
		pthread_sleepon_unlock();
	}
}

/*
 *	Routines for calibration paradigms.
 */
/*
 *	Print calibration points.
 */
void
cal_print(void)
{
	struct c_eyekeep *p;
	int i = 0;

	printf("Calibration points;  current is %d\n", c_curp);
	for(p= &c_eyekeep[0]; p <= &c_eyekeep[C_KEEPMAX]; p++) {
		printf("%d:  xpos %d \t ypos %d \t oxpos %d \t oypos %d\n", i,
			p->c_xsav, p->c_ysav, p->c_oxsav, p->c_oysav);
		i++;
	}
	tflush_;
}

void
cl_cal(void)
{
	struct c_eyekeep *p;

	for(p= &c_eyekeep[0]; p <= &c_eyekeep[C_KEEPMAX]; p++) {
		p->c_xsav= 0;
		p->c_ysav= 0;
		p->c_oxsav= 0;
		p->c_oysav= 0;
	}
}

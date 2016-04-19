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
 * Scribe process main.
 */

#include <process.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sched.h>
#include <sys/neutrino.h>
#include "rexhdrs.h"
#include "tty.h"
#include "scribe.h"

jmp_buf env= {0};
extern char version[];
extern sigset_t	alert_set;
char i_name[]= I_NAME;
char r_name[]= R_NAME;

main(int argc, char *argv[])
{
	struct sched_param sp;
	int msgReturn;

	/* request I/O privity */
	if(ThreadCtl(_NTO_TCTL_IO, 0) == -1) {
		perror("scribe couldn't set I/O privity");
		exit(EXIT_FAILURE);
	}

	/*
	 * Map INT_BLOCK and RAW_BLOCK.
	 */
	i_b = map_shared("/INT_BLOCK", sizeof(INT_BLOCK));
	r_b = map_shared("/RAW_BLOCK", sizeof(RAW_BLOCK));

#ifdef GOO
	setbuf(stdout, NULL);
#endif

	myptx= i_b->i_ptblx;
	myptp= &i_b->ptbl[myptx];
	stufs(version, myptp->p_version, &myptp->p_version[P_LVERSION]);
	i_b->scrb_pi = myptx;
	/*
	 * Set priority of 'scribe'.
	 */
	sp.sched_priority= PRI_SCRIBE;
	if(sched_setscheduler(0, SCHED_RR, &sp) < 0) {
	    fprintf(stderr, "main(): cannot set priority\n");
	    exit(1);
	}

	sigemptyset(&alert_set);
	sigaddset(&alert_set, S_ALERT);	    /* set up signal mask for alert */
	signal(S_CNTRLC, SIG_IGN);
	signal(S_CNTRLB, oncntrlb);
	signal(S_ALERT, alert);
	signal(S_DEBUG_PRINT, doutput);
    msgReturn = r_sendmsg(&i_b->ptbl[i_b->ph_switch_pi], CM_STARTED); 

	/*
	 * Check if invoked from a root file.
	 */
	if(i_b->i_rtflag & RT_READ) rt_read();

	setjmp(env);
	for(;;) {
	    sleep(32000);
	}
}

/*
 * On cntrlb close files, reset to default state.  (Drastic)
 */
void
oncntrlb(int sig)
{
	int tmp, old_value;
	char *old_name;

	tmp= sig;
	signal(S_CNTRLB, oncntrlb);
	myptp->p_state |= P_NOSIG_ST;	/* prevent IPC */
	dumperr(E_CNTLB);
	sleep(1);			/* let comm run */
	myptp->p_msg= 0;
	myptp->p_sem= HIGHBIT;
	myptp->p_vindex= 0;
	myptp->p_rmask= 0;
	myptp->p_state &= ~P_NOSIG_ST;
	old_name= doutput_filename;
	doutput_filename= "/tmp/doutput_scribe";
	old_value= doutput_tofile;
	doutput_tofile= 1;
	doutput(0);
	doutput_tofile= old_value;
	doutput_filename= old_name;
	longjmp(env, 0);
}

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
 * Process control.
 */
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>
#include <signal.h>

/* Toolkit headers */
#include <Ph.h>
#include <Pt.h>
#include <Ap.h>

/* Local headers */
#include "ph_procSwitch.h"
#include "abimport.h"
#include "proto.h"

#undef  A_DEBUG		    /* define for debugging info */

extern int xloc;
extern int yloc;
extern int wsize;
extern int hsize;
extern char rootFile[P_LROOTNAME];
extern int sysflags;
extern int nproc;
extern int waitAck;
extern PROCTBL zerop;

/*
 * Create a new process.
 */
PROCTBL_P newproc( char *name, char runState, char windowState )
{
	PtArg_t args[3];
	pid_t z;
	PROCTBL_P p;

	/*
	 * Find empty slot.
	 */
	for(p = (PROCTBL_P)&i_b->ptbl[0]; p->p_id != 0; ) {
		if(++p == &i_b->ptbl[P_NPROC]) {
			PtNotice(ABW_base, NULL, "Process Warning", NULL,
					 "newproc(): Process table overflow", "Helvetica09ba",
					 "OK", "Helvetica09ba",
					 Pt_CENTER | Pt_BLOCK_PARENT);
			return(NP);
		}
	}

	*p = zerop;			/* clear proctable entry */
	p->p_sem = HIGHBIT;	/* init semaphore */
	strncpy(p->p_name, name, P_LPROCNAME);

	/* set the root of the new process */
	if(strlen(rootFile)) {
		strncpy(p->p_root, rootFile, P_LROOTNAME);
	}

	/* set the run state of the new process */
	if(runState == 'r') p->p_state |= P_RUN_ST;
	else if(runState == 's') p->p_state &= ~P_RUN_ST;

	/*
	 * Fork and exec process and check for errors.
	 */
	z = crproc(p, windowState);

	if (z < 0) {	/* child process returned with an error */
		/*
		 * Failure of pipe or fork.
		 */
		if(z == -1) {
			PtNotice(ABW_base, NULL, "Process Warning", NULL,
					 "newproc(): Cannot make pipes and/or fork", "Helvetica09ba",
					 "OK", "Helvetica09ba",
					 Pt_CENTER | Pt_BLOCK_PARENT);
			return(NP);
		}

		/*
		 * Child process- error on exec.  Signal procSwitch and exit.
		 */
		else if(z == -2) {
			r_sendmsg((PROCTBL_P)&i_b->ptbl[i_b->ph_switch_pi], CM_BADCHILD);
			PtExit(0);
		}
	}

	/* arm the timer to wait for a process to start */
	waitAck = -100;		/* set the timer repeat count to indicate newproc() */
	PtSetArg(&args[0], Pt_ARG_TIMER_INITIAL, 100, 0);
	PtSetArg(&args[1], Pt_ARG_TIMER_REPEAT, 100, 0);
	PtSetArg(&args[2], Pt_ARG_POINTER, p, 0);
	PtSetResources(ABW_procWaitTimer, 3, args);
	
	return(p);
}

/*
 * Kill REX process.  If p == NP kill all processes.
 */
void endproc( PROCTBL_P p )
{
	PROCTBL_P ep;
	PROCTBL_P bp;
	int etime;
	int status;
	char s[P_ISLEN];

	if(p != NP) {	/* called to kill the specified process */
		bp = p;
		ep = p + 1;
	}
	else {			/* called to kill all process */
		bp = (PROCTBL_P)&i_b->ptbl[0];
		ep = (PROCTBL_P)&i_b->ptbl[P_NPROC];
	}

	/*
	 * Send message to process to terminate.  If for some reason process
	 * is unable to comply kill it.
	 */
	for(p = bp; p < ep; p++) {
		if(p->p_id == 0) continue;		/* skip entry if empty */
		if(p == &i_b->ptbl[i_b->ph_switch_pi]) continue;	/* don't kill procSwitch here */

		r_sendmsg(p, G_KILL);
	}
	sleep(1);

	/* If for some reason, process doesn't die, kill it */
	for(etime = 0; ; etime++) {
	    for(p = bp; p < ep; p++) {
			if(p->p_id == 0) continue;
			if(p == &i_b->ptbl[i_b->ph_switch_pi]) continue;	/* don't kill procSwitch here */
			if(p->p_state & P_EXIT_ST) {
				p->p_id = 0;
				nproc--;
				wait(&status);	/* remove DEAD entry from UNIX process table */
				continue;
			}
			if(etime >= P_EXWAIT) {
				sprintf(s, "endproc(): Explicitly killing %s", p->p_name);
				rxerr(s);
				p->p_state |= P_EXIT_ST;
				if(kill(p->p_id, 9) >= 0) wait(&status);
			}
			else break;
	    }
	    if((p == ep) && etime) break;	/* done */
		sleep(1);
	}
	return;
}

/*
 * Get pointer to proc table entry.
 */
PROCTBL_P getptp(char *name)
{
	PROCTBL_P p;

	/* search process table for process named 'name' */
	for(p = (PROCTBL_P)&i_b->ptbl[0]; p < &i_b->ptbl[P_NPROC]; p++) {
		if(p->p_id && !strcmp(name, p->p_name)) break;
	}

	/* if process table pointer points to the end of the process table array */
	if(p == &i_b->ptbl[P_NPROC]) {
		/*
		 * Check for ".int", the default for the current
		 * int process.
		 */
		if(!strcmp(name, "int") && (i_b->ph_int_pi >= 0)) {
			p = (PROCTBL_P)&i_b->ptbl[i_b->ph_int_pi];
		}
		else p = NP;
	}
	return(p);
}


/*
 * Fork and exec a new REX process.  Returns:
 *	new process id if successful
 *	-1 for failure of parent to open pipes
 *	-2 for failure of child
 */
pid_t crproc(PROCTBL_P p, char windowState)
{
	pid_t z;
	int execResult;
	char Xloc[32];
	char Yloc[32];
	char Width[32];
	char Height[32];
	char winState[5];

	/* open a pipe */
	if(pipe(p->p_pipe) < 0) return(-1);

	/* fork a child process */
	if((z = fork()) == -1) {
		/* if fork() fails, close the pipe and return */
		close(p->p_pipe[0]);
		close(p->p_pipe[1]);
		return(-1);
	}

	if(z == 0) {	/* child process */
		/*
		 * Have to send index to ptbl, not pointer.  Proc
		 * may map I_B with another register.
		 * Also fill in pid in proc array for child;  it may
		 * run before the parent and might need its pid before
		 * the parent could copy it in.
		 */
		p->p_id = getpid();
		i_b->i_ptblx = p - &PT[0];

		/* windowState is 's', then this is scribe, a non-photon process,
		 * call execl with just path and file name arguments
		 */
		if(windowState == 's') {
			execResult = execl(p->p_name, p->p_name, 0);
		}
		/* this is a photon process, add initial location
		 * and initial window state to execl call
		 */
		else {
			/* build window location and size arguments */
			sprintf(Xloc, "-x%d", xloc);
			sprintf(Yloc, "-y%d", yloc);
			if(wsize > 0) sprintf(Width, "-w%d", wsize);
			else strcpy(Width, "");
			if(hsize > 0) sprintf(Height, "-h%d", hsize);
			else strcpy(Height, "");

			/* reset the window location and size variables */
			xloc = yloc = wsize = hsize = 0;

			sprintf(winState, "-S%c", windowState);
			execResult = execl(p->p_name, p->p_name, winState,
							   Xloc, Yloc, Width, Height, 0);
		}

		/* if execl() fails, close the pipe and return */
		if(execResult < 0) {
			close(p->p_pipe[0]);
			close(p->p_pipe[1]);
			return(-2);
		}
		return(0);	/* return from child process */
	}
	else return(z);	/* return from parent process */
}

/*
 * Send noun or menu name to process to execute.  If p != NP then
 * only nouns, menus of process p are eligible.  Returns index to ptbl entry
 * that name is sent to.  Comm goes to sleep after name is sent to allow
 * process to take over keyboard.  Receiving process must awaken comm
 * when done.  If verb string 'vstr' is null, the name is assumed to be
 * a menu;  otherwise a noun.  If noerrprt is true, rxerr() will not be
 * called if noun or menu name is not found;  err status will still be returned,
 * however.  This is used by callers that need to handle errors themselves.
 * Error status returns:
 *	-1:	Noun or menu not found
 *	-2:	Error sending G_NMEXEC msg to process (in this case r_sendmsg
 *		calls rxerr())
 */
int sendnm(PROCTBL_P p, char *nmstr, char *vstr, char *argstr, int noerrprt)
{
	PtArg_t args[3];
	int response;
	char errmsg[128];
	char *btns[] = { "&Force", "&Abort" };
	char *btnFonts[] = { "&Helvetica09ba", "&Helvetica09ba"	};

	extern ME_ACCP me_accp;

	/* if the noun index is not free
	 * post a message asking user to force completion
	 */
	if(i_b->i_nindex != NULLI) {
		sprintf(errmsg, "sendnm(): Can't execute %s %s because\nenvironment not free", vstr, nmstr);
		switch(PtAlert(ABW_base, NULL, "Overwrite File", NULL,
					   errmsg, "Helvetical09ba",
					   2, (char const **)btns, (char const **)btnFonts, 1, 2, Pt_MODAL)) {
		case 1:
			i_b->i_nindex = NULLI;
			break;
		case 2:
			return(-2);
			break;
		}
	}

	/* figure out scribe's noun index and put it in shared memory */
	if(!strcmp(nmstr, "Adata")) i_b->i_nindex = 0;
	else if(!strcmp(nmstr, "Edata")) i_b->i_nindex = 1;
	else if(!strcmp(nmstr, "Bdata")) i_b->i_nindex = 2;
	else if(!strcmp(nmstr, "file")) i_b->i_nindex = 3;

	/* if nmstr is not one of scribes nouns, return with an error */
	else {
		i_b->i_nindex= NULLI;
		return(-2);
	}

	/* copy the verb and arguments to the shared memory area */
	stufs(vstr, (char *)&i_b->i_verbs, (char *)&i_b->i_verbs[P_ISLEN]);
	stufs(argstr, (char *)&i_b->i_args, (char *)&i_b->i_args[P_ISLEN]);
	clrack_;

#ifdef A_DEBUG
	dprintf("-nmc: before r_sendmsg, v: %s n: %s arg: %s-", vstr, nmstr, argstr);
#endif

    if(r_sendmsg(p, G_NMEXEC)) {
		sprintf(errmsg, "Couldn't send %s to process %s", nmstr, p->p_name);
		PtNotice(ABW_base, NULL, "Process Warning", NULL,
				 errmsg, "Helvetica09ba",
				 "OK", "Helvetica09ba",
				 Pt_CENTER | Pt_BLOCK_PARENT);
		i_b->i_nindex = NULLI;
		return(-2);
	}

#ifdef A_DEBUG
	dputs("-nmc: after r_sendmsg, before waitack-");
#endif

	/* arm the timer to wait for a process to start */
	waitAck = 0;		/* set the timer repeat count to indicate indefinite wait */
	PtSetArg(&args[0], Pt_ARG_TIMER_INITIAL, 100, 0);
	PtSetArg(&args[1], Pt_ARG_TIMER_REPEAT, 100, 0);
	PtSetArg(&args[2], Pt_ARG_POINTER, p, 0);
	PtSetResources(ABW_procWaitTimer, 3, args);

#ifdef A_DEBUG
	dputs("-nmc: after waitack-");
#endif

	return(p - &PT[0]);
}

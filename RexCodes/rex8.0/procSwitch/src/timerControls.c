/* Y o u r   D e s c r i p t i o n                       */
/*                            AppBuilder Photon Code Lib */
/*                                         Version 1.14C */

/* Standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

/* Toolkit headers */
#include <Ph.h>
#include <Pt.h>
#include <Ap.h>

/* Local headers */
#include "ph_procSwitch.h"
#include "abimport.h"
#include "proto.h"

extern int sysflags;
extern int nproc;
extern int waitAck;

int waitProc( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[2];
	PROCTBL_P p;
	int status;
	static u_int count = 0;
	char errmsg[256];

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the target process's process table pointer from the widget */
	PtGetResource(widget, Pt_ARG_POINTER, &p, 0);

	/* check sysflags to see if child process has responded */
	/* if the C_STARTED bit is set, then the child launched by
	 * newproc() has started. This requires that the global variable
	 * nproc be incremented
	 */
	if(sysflags & C_STARTED) {
		/* disarm the timer */
		PtSetArg(&args[0], Pt_ARG_TIMER_INITIAL, 0, 0);
		PtSetArg(&args[1], Pt_ARG_TIMER_REPEAT, 0, 0);
		PtSetResources(widget, 2, args);

		nproc++;			/* advance process count */
		count = 0;			/* reset the loop counter */
	}
	/* if the C_AFFIRM bit is set, then an existing process
	 * has acknowledged successfully executing a command sent
	 * by procSwitch
	 */
	else if(sysflags & C_AFFIRM) {
		/* disarm the timer */
		PtSetArg(&args[0], Pt_ARG_TIMER_INITIAL, 0, 0);
		PtSetArg(&args[1], Pt_ARG_TIMER_REPEAT, 0, 0);
		PtSetResources(widget, 2, args);

		count = 0;			/* reset the loop counter */
	}
	/* if the global variable waitAck is -100, then the timer
	 * was armed from the newproc routine. If newproc times out
	 * then the child process had to be killed in addition to
	 * disarming the timer and posting a message
	 */
	else if(waitAck == -100) {
		if(count > -waitAck) {	/* check to see if newproc has timed out */
			/* disarm the timer */
			PtSetArg(&args[0], Pt_ARG_TIMER_INITIAL, 0, 0);
			PtSetArg(&args[1], Pt_ARG_TIMER_REPEAT, 0, 0);
			PtSetResources(widget, 2, args);
			count = 0;			/* reset the loop counter */

			/* clean up and kill bad child */
			close(p->p_pipe[0]);
			close(p->p_pipe[1]);
			if(kill(p->p_id, 9) >= 0) wait(&status);
			p->p_id = 0;

			/* tell user of error */
			sprintf(errmsg, "waitProc(): couldn't start process %s", p->p_name);
			PtAskQuestion(ABW_base, NULL,
						  errmsg, "Helvetica09ba",
						  "&Continue", NULL, NULL, 1);
		}
		else count++;			/* increment the loop count */
	}
	/* if the global variable waitAck is any positive integer, then the
	 * timer was armed from a function other than newproc. If this function
	 * times out, just disarm the timer and post an error message
	 */
	else if(waitAck) {
		if(count > waitAck) {	/* check to see if other command has timed out */
			/* disarm the timer */
			PtSetArg(&args[0], Pt_ARG_TIMER_INITIAL, 0, 0);
			PtSetArg(&args[1], Pt_ARG_TIMER_REPEAT, 0, 0);
			PtSetResources(widget, 2, args);
			count = 0;			/* reset the loop counter */

			/* tell user of error */
			sprintf(errmsg, "process %s timed out", p->p_name);
			PtAskQuestion(ABW_base, NULL,
						  errmsg, "Helvetica09ba",
						  "&Continue", NULL, NULL, 1);
		}
		else count++;			/* increment the loop count */
	}
	
	/* if the global variable waitAck is 0, this is an indefinite wait.
	 * The timer will run indefinately, but the rest of the gui will
	 * function.
	 */
	else {
		count++;
	}
	return( Pt_CONTINUE );
}

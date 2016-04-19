/* Y o u r   D e s c r i p t i o n                       */
/*                            AppBuilder Photon Code Lib */
/*                                         Version 1.14C */

/* Standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

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

int manageWin( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[3];
	PhWindowEvent_t *winEvent;
	PhWindowEvent_t newEvent;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get pointer to the window event structure */
	winEvent = (PhWindowEvent_t *)cbinfo->cbdata;

	/* check to see which type event evoked this callback */
	switch(winEvent->event_f) {
	case Ph_WM_HIDE:
		if((winEvent->state_f & Ph_WM_STATE_ISICONIFIED)) {
			/* this process is going to be iconified */
			if(i_b->i_flags & (I_EOUT | I_AOUT)) {
				r_sendmsg(&i_b->ptbl[i_b->ph_switch_pi], CM_STOPKEEP);

				/* wait for data keeping to stop */
				while(i_b->i_flags & (I_EOUT | I_AOUT)) sleep(1);
			}
			if(i_b->i_flags & I_GO) {
				/* stop the clock if it is running */
				PtSetArg(&args[0], Pt_ARG_ONOFF_STATE, 0, 0);
				PtSetArg(&args[1], Pt_ARG_TEXT_STRING, "Clock\nOff", 0);
				PtSetArg(&args[2], Pt_ARG_FILL_COLOR, Pg_RED, 0);
				PtSetResources(ABW_clockOnOffButton, 3, args);
				ad_stop();
				clk_stop();
			}

			/* signal procSwitch that this process is stopped */
			release_(&myptp->p_sem);
			myptp->p_state &= ~P_RUN_ST;

			pthread_mutex_lock(&i_b->i_mutex);
			{
				if(i_b->ph_int_pi == myptx) {
					i_b->ph_int_pi = -1;
				}
			}
			pthread_mutex_unlock(&i_b->i_mutex);

#ifdef CLRMIRRORS
			XYmout(0, 0);	/* put mirrors at 0, 0 */
#endif
#ifdef PCS_SOCKET
			pcsCloseVex();
#ifdef MEX62_UNITS
			pcsCloseMex();
#endif
#endif

		}
		if(!(winEvent->state_f & Ph_WM_STATE_ISICONIFIED)) {
			/* this process is going to be restored */
			/* if there is another process running, signal it to stop */
			if((i_b->ph_int_pi != myptx) && (i_b->ph_int_pi > 0)) {
				memset(&newEvent, 0, sizeof(newEvent));
				newEvent.event_f = Ph_WM_HIDE;
				newEvent.state_f = Ph_WM_STATE_ISICONIFIED;
				newEvent.event_state = Ph_WM_EVSTATE_HIDE;
				PtForwardWindowTaskEvent(i_b->ptbl[i_b->ph_int_pi].p_coid, &newEvent);
			}
		
			myptp->p_state |= P_RUN_ST;

			pthread_mutex_lock(&i_b->i_mutex);
			{
				i_b->ph_int_pi = myptx;

				/* copy real time display variables from local storage to shared memory */
				i_b->cursTyp = localCursTyp;
				i_b->rl = localRl;
			}
			pthread_mutex_unlock(&i_b->i_mutex);

			/* force a reset for the newlly restored process */
			resetStates(widget, apinfo, cbinfo);
		}
	}

	return( Pt_CONTINUE );
}

int exitSpot( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* if the user tries to close the window (kill the process),
	 * make sure that scribe is not keeping data
	 */
	if(i_b->i_flags & (I_EOUT | I_AOUT)) {
		r_sendmsg(&i_b->ptbl[i_b->ph_switch_pi], CM_STOPKEEP);

		/* wait for data keeping to stop */
		while(i_b->i_flags & (I_EOUT | I_AOUT)) sleep(1);
	}
	if(i_b->i_flags & I_GO) {
		ad_stop();
		clk_stop();
	}
	release_(&myptp->p_sem);
	myptp->p_state |= P_EXIT_ST;
	myptp->p_id = 0;
	if(i_b->ph_int_pi == myptx) {
		i_b->ph_int_pi = -1;
	}

#ifdef CLRMIRRORS
	XYmout(0, 0);	/* put mirrors at 0, 0 */
#endif
#ifdef PCS_SOCKET
	pcsCloseVex();		/* close tcp/ip socket to vex */
#ifdef MEX62_UNITS
	pcsCloseMex();
#endif
#endif
	PtExit(EXIT_SUCCESS);

	return( Pt_CONTINUE );
}

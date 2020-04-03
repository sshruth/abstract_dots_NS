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

extern char toolBarTitle[128];
extern WIN_DATCURS localCursTyp;
extern RL localRl;

int init( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;
	PhRect_t frame;
	PhDim_t dim;
	int msgReturn;
	int value;
	short int x;
	short int y;

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	/* get the initial state of the paradigm switch */
	PtSetArg(&arg, Pt_ARG_ONOFF_STATE, 0, 0);
	PtGetResources(ABW_pdigmOnOffButton, 1, &arg);
	value = (int)arg.value;
	if(value) {
		i_b->i_flags &= ~I_PSTOP;
		softswitch &= ~PSTOP;
	}
	else {
		i_b->i_flags |= I_PSTOP;
		softswitch |= PSTOP;
	}

	/* get the initial state of the window check */
	PtSetArg(&arg, Pt_ARG_ONOFF_STATE, 0, 0);
	PtGetResources(ABW_windowsOnOffButton, 1, &arg);
	value = (int)arg.value;
	if(value) i_b->i_flags &= ~I_WINOFF;
	else i_b->i_flags |= I_WINOFF;

	/* set the title of the main window */
	PtSetArg(&arg, Pt_ARG_WINDOW_TITLE, toolBarTitle, 0);
	PtSetResources(link_instance, 1, &arg);

	/* read root file if it has been preset from rex session */
	if(strlen(myptp->p_root)) {
		pthread_mutex_lock(&i_b->i_mutex);
		{
			readRoot(myptp->p_root);

			/* copy the real time display variables from shared memory to local memory */
			localCursTyp = i_b->cursTyp;
			localRl = i_b->rl;
		}
		pthread_mutex_unlock(&i_b->i_mutex);
	}

	/* save the location and size of the main window for rex session */
	PtWindowGetFrameSize(link_instance, &frame);
	PtGetAbsPosition(link_instance, &x, &y);
	myptp->p_x = x - frame.ul.x;
	myptp->p_y = y - frame.ul.y;

	PtWidgetDim(link_instance, &dim);
	myptp->p_w = dim.w;
	myptp->p_h = dim.h;

	/*
	 * Respond to procSwitch
	 */
    msgReturn = r_sendmsg(&i_b->ptbl[i_b->ph_switch_pi], CM_STARTED);

	return( Pt_CONTINUE );
}

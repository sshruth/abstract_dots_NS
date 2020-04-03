/* Y o u r   D e s c r i p t i o n                       */
/*                            AppBuilder Photon Code Lib */
/*                                         Version 1.13X */

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
#include "ph_rline.h"
#include "abimport.h"
#include "proto.h"

extern lsrRunLine runLine;

int resizeWin( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PhRect_t frame;
	PhDim_t dim;
	short int x;
	short int y;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the size and position of the window for rex session */
	PtWindowGetFrameSize(widget, &frame);
	PtGetAbsPosition(widget, &x, &y);
	myptp->p_x = x - frame.ul.x;
	myptp->p_y = y - frame.ul.y;

	PtWidgetDim(widget, &dim);
	myptp->p_w = dim.w;
	myptp->p_h = dim.h;

	return( Pt_CONTINUE );
}

int manageWin( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PhRect_t frame;
	PhWindowEvent_t *winEvent;
	PtArg_t args[2];
	short int x;
	short int y;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get pointer to the window event structure */
	winEvent = (PhWindowEvent_t *)cbinfo->cbdata;

	/* check to see which type event evoked this callback */
	switch(winEvent->event_f) {
	case Ph_WM_FOCUS:
	case Ph_WM_TOFRONT:
		/* damage all widgets to force redraws */
		PtDamageWidget(ABW_runLineMenuBar);
		PtDamageWidget(ABW_runLineOptions);
		PtDamageWidget(ABW_dispResetButton);
		PtDamageWidget(ABW_cntrlBtnPane);
		PtDamageWidget(ABW_chan0Toggle);
		PtDamageWidget(ABW_chan1Toggle);
		PtDamageWidget(ABW_chan2Toggle);
		PtDamageWidget(ABW_chan3Toggle);
		PtDamageWidget(ABW_chan4Toggle);
		PtDamageWidget(ABW_chan5Toggle);
		PtDamageWidget(ABW_chan6Toggle);
		PtDamageWidget(ABW_chan7Toggle);
		PtDamageWidget(ABW_rlineToggle);
		PtDamageWidget(ABW_unitsToggle);
		PtDamageWidget(ABW_triggerLabel);
		PtDamageWidget(ABW_rlTrigButton);
		PtDamageWidget(ABW_displayLabel);
		PtDamageWidget(ABW_rlRepeatButton);
		PtDamageWidget(ABW_levelLabel);
		PtDamageWidget(ABW_rlTrigLevel);
		break;
	case Ph_WM_HIDE:
		if((winEvent->state_f & Ph_WM_STATE_ISICONIFIED)) {
			myptp->p_state &= ~P_RUN_ST;

			PtSetArg(&args[0], Pt_ARG_TIMER_INITIAL, 0, 0);
			PtSetArg(&args[1], Pt_ARG_TIMER_REPEAT, 0, 0);
			PtSetResources(ABW_rlTimer, 2, args);
		}

		if(!(winEvent->state_f & Ph_WM_STATE_ISICONIFIED)) {
			myptp->p_state = P_RUN_ST;

			PtSetArg(&args[0], Pt_ARG_TIMER_INITIAL, 16, 0);
			PtSetArg(&args[1], Pt_ARG_TIMER_REPEAT, 16, 0);
			PtSetResources(ABW_rlTimer, 2, args);
		}
		break;
	case Ph_WM_MOVE:
		PtWindowGetFrameSize(widget, &frame);
		PtGetAbsPosition(ABW_base, &x, &y);
		myptp->p_x = x - frame.ul.x;
		myptp->p_y = y - frame.ul.y;
	}

	return( Pt_CONTINUE );
}

int exitRline( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* shut off running line processing by msec_clock */
	myptp->p_state |= P_EXIT_ST;
	myptp->p_id = 0;
	PtExit(EXIT_SUCCESS);

	return( Pt_CONTINUE );
}

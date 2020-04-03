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
#include "ph_ewind.h"
#include "abimport.h"
#include "proto.h"

extern lsrDispWindow dispWind;

int resizeWin(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
	PhDim_t dim;
	PhRect_t frame;
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

	/* This seems to be necessary to get the left scale redrawn
	 * when only the width of the window is changed.
	 */
	PtDamageWidget(ABW_leftScale);

	return(Pt_CONTINUE);
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

	winEvent = (PhWindowEvent_t *)cbinfo->cbdata;

	/* check to see which type event evoked this callback */
	switch(winEvent->event_f) {
	case Ph_WM_FOCUS:
	case Ph_WM_TOFRONT:
		/* damage all widgets to force redraws */
		PtDamageWidget(ABW_leftScale);
		PtDamageWidget(ABW_bottomScale);
		PtDamageWidget(ABW_eyeWndControlPane);
		PtDamageWidget(ABW_curs0Btn);
		PtDamageWidget(ABW_curs1Btn);
		PtDamageWidget(ABW_curs2Btn);
		PtDamageWidget(ABW_curs3Btn);
		PtDamageWidget(ABW_curs4Btn);
		PtDamageWidget(ABW_curs5Btn);
		PtDamageWidget(ABW_curs6Btn);
		PtDamageWidget(ABW_curs7Btn);
		PtDamageWidget(ABW_wnd0Btn);
		PtDamageWidget(ABW_wnd1Btn);
		PtDamageWidget(ABW_wnd2Btn);
		PtDamageWidget(ABW_wnd3Btn);
		PtDamageWidget(ABW_wnd4Btn);
		PtDamageWidget(ABW_wnd5Btn);
		PtDamageWidget(ABW_wnd6Btn);
		PtDamageWidget(ABW_wnd7Btn);
		PtDamageWidget(ABW_eyeOffsetGroupLabel);
		PtDamageWidget(ABW_eyeHorizOffset);
		PtDamageWidget(ABW_eyeVertOffset);
		PtDamageWidget(ABW_otherEyeOffsetGroupLabel);
		PtDamageWidget(ABW_oEyeHorizOffset);
		PtDamageWidget(ABW_oEyeVertOffset);
		break;
	case Ph_WM_HIDE:
		if((winEvent->state_f & Ph_WM_STATE_ISICONIFIED)) {
			myptp->p_state &= ~P_RUN_ST;

			PtSetArg(&args[0], Pt_ARG_TIMER_INITIAL, 0, 0);
			PtSetArg(&args[1], Pt_ARG_TIMER_REPEAT, 0, 0);
			PtSetResources(ABW_wndTimer, 2, args);
		}
		if(!(winEvent->state_f & Ph_WM_STATE_ISICONIFIED)) {
			myptp->p_state |= P_RUN_ST;

			PtSetArg(&args[0], Pt_ARG_TIMER_INITIAL, 16, 0);
			PtSetArg(&args[1], Pt_ARG_TIMER_REPEAT, 16, 0);
			PtSetResources(ABW_wndTimer, 2, args);
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

int exitEwind( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* shut off window display processing by msec_clock */
	myptp->p_state |= P_EXIT_ST;
	myptp->p_id = 0;
	PtExit(EXIT_SUCCESS);

	return( Pt_CONTINUE );
}

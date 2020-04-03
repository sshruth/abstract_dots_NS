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
#include "ph_ras.h"
#include "abimport.h"
#include "proto.h"

extern LsrTig lsrTig;
extern int dataStarted;

int resizeWin( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
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

	return( Pt_CONTINUE );
}

int manageWin( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[2];
	PhRect_t frame;
	PhWindowEvent_t *winEvent;
	SELECTIONS *selections;
	short int x;
	short int y;
	int i;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	winEvent = (PhWindowEvent_t *)cbinfo->cbdata;

	switch(winEvent->event_f) {
	case Ph_WM_HIDE:
		if((winEvent->state_f & Ph_WM_STATE_ISICONIFIED)) {
			myptp->p_state &= ~P_RUN_ST;

			/* turn off raster processing */
			PtSetArg(&args[0], Pt_ARG_TIMER_INITIAL, 0, 0);
			PtSetArg(&args[1], Pt_ARG_TIMER_REPEAT, 0, 0);
			PtSetResources(ABW_rastersTimer, 2, args);
		}

		if(!(winEvent->state_f & Ph_WM_STATE_ISICONIFIED)) {
			myptp->p_state |= P_RUN_ST;

			/* turn on raster processing  */
			if(dataStarted == 1) {
				PtSetArg(&args[0], Pt_ARG_TIMER_INITIAL, 1000, 0);
				PtSetArg(&args[1], Pt_ARG_TIMER_REPEAT, 500, 0);
				PtSetResources(ABW_rastersTimer, 2, args);
			}
		}
		break;
	case Ph_WM_MOVE:
		PtWindowGetFrameSize(widget, &frame);
		PtGetAbsPosition(widget, &x, &y);
		myptp->p_x = x - frame.ul.x;
		myptp->p_y = y - frame.ul.y;
	}

	return( Pt_CONTINUE );
}

int exposeWin( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	SELECTIONS *selections;
	int i;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* set the flag to clear the tig window */
	lsrTig.clear = 1;

	/* set the new data flag in all classes */
	selections = getSelections();
	for(i = 0; i < selections->nClasses; i++) {
		selections->classSet[i].new = 1;
	}

	/* call the raster draw function */
	drawRasters();

	return( Pt_CONTINUE );
}

int exitRas( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	myptp->p_state |= P_EXIT_ST;
	myptp->p_id = 0;
	PtExit(EXIT_SUCCESS);

	return( Pt_CONTINUE );
}

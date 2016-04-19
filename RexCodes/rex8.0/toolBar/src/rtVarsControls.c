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

PtWidget_t *rtVarList[MAXRTVAR] = { (PtWidget_t *)NULL };
long baseTime;

int initUserRtVarsDialog( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	ApDBase_t *userRtVarsdbase;
	PhPoint_t loc;
	PtArg_t args[2];
	int y;
	int i;

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	/* get the absolute position of the base widget,
	 * place the dialog 100 pixels below the base widget
	 * set the name of the dialog
	 */
	PtGetAbsPosition(ABW_base, &loc.x, &loc.y);
	loc.y += 100;
	PtSetArg(&args[0], Pt_ARG_POS, &loc, 0);
	PtSetArg(&args[1], Pt_ARG_WINDOW_TITLE, cbinfo->cbdata, 0);
	PtSetResources(link_instance, 2, args);

	/* initialize the base timer */
	baseTime = i_b->i_time;

	/* open the user real-time variables data base */
	userRtVarsdbase = ApOpenDBase(ABM_userRtVarsPictures);

	/* set the parent for the widgets */
	PtSetParentWidget(ABW_userRtVarsScrollArea);

	/* create the widget sets for the user real-time variables */
	y = 0;
	i = 0;
	while(strlen(rtvars[i].rt_name)) {
		/* create the label widget for this variable */
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, rtvars[i].rt_name, 0);
		ApCreateWidget(userRtVarsdbase, "userRtVarLabel", 5, y, 1, &args[0]);

		/* create the value widget for this variable */
		rtVarList[i] = ApCreateWidget(userRtVarsdbase, "userRtVarsInt", 200, y, 0, NULL);

		y += 30;
		i++;
	}

	/* set the maximum range of the scroll widget */
	PtSetArg(&args[0], Pt_ARG_SCROLL_AREA_MAX_Y, y, 0);
	PtSetResources(ABW_userRtVarsScrollArea, 1, &args[0]);

	/* close the widget data base */
	ApCloseDBase(userRtVarsdbase);

	return( Pt_CONTINUE );
}

int updateVars( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;
	long msecTime;
	int mins;
	int secs;
	int i;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* increment the timer */
	msecTime = i_b->i_time - baseTime;

	/* convert to seconds */
	secs = msecTime / 1000;

	/* get minutes */
	mins = secs / 60;

	/* get seconds */
	secs = secs % 60;

	/* set the widgets */
	PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, mins, 0);
	PtSetResources(ABW_userRtVarsMin, 1, &arg);
	PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, secs, 0);
	PtSetResources(ABW_userRtVarsSec, 1, &arg);
	
	i = 0;
	while(rtVarList[i]) {
		PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, *rtvars[i].rt_var, 0);
		PtSetResources(rtVarList[i], 1, &arg);
		i++;
	}
	return( Pt_CONTINUE );
}

int resetVars( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;
	int i;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	baseTime = i_b->i_time;

	/* set the timer widgets */
	PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, 0, 0);
	PtSetResources(ABW_userRtVarsMin, 1, &arg);
	PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, 0, 0);
	PtSetResources(ABW_userRtVarsSec, 1, &arg);

	i = 0;
	while(rtvars[i].rt_var) {
		*rtvars[i].rt_var = 0;
		i++;
	}
	return( Pt_CONTINUE );
}

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
#include "../../hdr/sac.h"
#include "abimport.h"
#include "proto.h"

int initSacDec( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;

	void sacDialogItemEnable();

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	if(m_sdctl == NULLI) {
		PtSetArg(&arg, Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_sacDecOnToggle, 1, &arg);
		PtSetResources(ABW_sacDecOffToggle, 1, &arg);
	}
	else if(m_sdctl == 0) {
		PtSetArg(&arg, Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_sacDecOnToggle, 1, &arg);
		PtSetArg(&arg, Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(ABW_sacDecOffToggle, 1, &arg);
	}
	else if(m_sdctl == 1) {
		PtSetArg(&arg, Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(ABW_sacDecOnToggle, 1, &arg);
		PtSetArg(&arg, Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_sacDecOffToggle, 1, &arg);
	}

	/* set the numeric values of the override widgets */
	PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, limh.range, 0);
	PtSetResources(ABW_sacDecHrange, 1, &arg);

	PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, limv.range, 0);
	PtSetResources(ABW_sacDecVrange, 1, &arg);

	PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, peakvel, 0);
	PtSetResources(ABW_sacDecPeakVel, 1, &arg);

	PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, velstart, 0);
	PtSetResources(ABW_sacDecStartVel, 1, &arg);

	PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, velend, 0);
	PtSetResources(ABW_sacDecEndVel, 1, &arg);

	PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, velmax, 0);
	PtSetResources(ABW_sacDecMaxVel, 1, &arg);

	PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, velmin, 0);
	PtSetResources(ABW_sacDecMinVel, 1, &arg);

	PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, durmax, 0);
	PtSetResources(ABW_sacDecMaxDur, 1, &arg);

	PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, durmin, 0);
	PtSetResources(ABW_sacDecMinDur, 1, &arg);

	PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, delay_time, 0);
	PtSetResources(ABW_sacDecDelay, 1, &arg);

	return( Pt_CONTINUE );
}

int sacDecSet( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* if this function was called by a button release,
	 * clear the m_sdctl flag
	 */
	if(cbinfo->reason == Pt_CB_DISARM) {
		m_sdctl = NULLI;
	}
	else {		/* this function called by a button press */
		/* figure out which button was pressed */
		if(ApName(widget) == ABN_sacDecOnToggle) {
			m_sdctl = 1;
		}
		else if(ApName(widget) == ABN_sacDecOffToggle) {
			m_sdctl = 0;
		}
	}

	sd_set(NULLI);

	return( Pt_CONTINUE );
}

int sacDecParams( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;
	int value;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the value from the widget */
	PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, 0, 0);
	PtGetResources(widget, 1, &arg);
	value = (int)arg.value;

	/* figure out which widget initiated this callback */
	if(ApName(widget) == ABN_sacDecHrange) limh.range = value;
	else if(ApName(widget) == ABN_sacDecVrange) limv.range = value;
	else if(ApName(widget) == ABN_sacDecPeakVel) peakvel = value;
	else if(ApName(widget) == ABN_sacDecStartVel) velstart = value;
	else if(ApName(widget) == ABN_sacDecEndVel) velend = value;
	else if(ApName(widget) == ABN_sacDecMaxVel) velmax = value;
	else if(ApName(widget) == ABN_sacDecMinVel) velmin = value;
	else if(ApName(widget) == ABN_sacDecMaxDur) durmax = value;
	else if(ApName(widget) == ABN_sacDecMinDur) durmin = value;
	else if(ApName(widget) == ABN_sacDecDelay) delay_time = value;

	return( Pt_CONTINUE );
}

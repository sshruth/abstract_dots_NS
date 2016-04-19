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

int eyeOffsets[] = { 0, 0, 0, 0 };
int eyeOffOvrRides[] = { 0, 0, 0, 0 };
int eyeOffDialogEnabled = 0;

int initEyeOffDialog( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[3];

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	if(m_offmenu) {
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, "Dialog Enabled", 0);
		PtSetArg(&args[1], Pt_ARG_ONOFF_STATE, m_offmenu, 0);
		PtSetResources(ABW_eyeOffDialogButton, 2, args);
	}
	else {
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, "Dialog Disabled", 0);
		PtSetArg(&args[1], Pt_ARG_ONOFF_STATE, m_offmenu, 0);
		PtSetResources(ABW_eyeOffDialogButton, 2, args);
	}

	/* set the numeric values of the override widgets */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, eyeOffsets[0], 0);
	PtSetResources(ABW_eyeOffHorizOff, 1, &args[0]);

	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, eyeOffsets[1], 0);
	PtSetResources(ABW_eyeOffVertOff, 1, &args[0]);

	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, eyeOffsets[2], 0);
	PtSetResources(ABW_eyeOffOthHorizOff, 1, &args[0]);

	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, eyeOffsets[3], 0);
	PtSetResources(ABW_eyeOffOthVertOff, 1, &args[0]);

	/* set the states of the menu override buttons */
	if(eyeOffOvrRides[0]) {
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, m_eho, 0);
	}
	else {
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, eho, 0);
	}
	PtSetResources(ABW_eyeHOvrRide, 1, &args[0]);
	PtSetResources(ABW_eyeOffHorizOffAct, 1, &args[1]);

	if(eyeOffOvrRides[1]) {
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, m_evo, 0);
	}
	else {
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, evo, 0);
	}
	PtSetResources(ABW_eyeVOvrRide, 1, args);
	PtSetResources(ABW_eyeOffVertOffAct, 1, &args[1]);

	if(eyeOffOvrRides[2]) {
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, m_oeho, 0);
	}
	else {
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, oeho, 0);
	}
	PtSetResources(ABW_oeyeHOvrRide, 1, args);
	PtSetResources(ABW_eyeOffOthHorizOffAct, 1, &args[1]);

	if(eyeOffOvrRides[3]) {
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, m_oevo, 0);
	}
	else {
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, oevo, 0);
	}
	PtSetResources(ABW_oeyeVOvrRide, 1, args);
	PtSetResources(ABW_eyeOffOthVertOffAct, 1, &args[1]);

	eyeOffDialogItemEnable(m_offmenu);

	return( Pt_CONTINUE );
}

int eyeOffDialogSet( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;
	int value;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the value of the widget */
	PtSetArg(&arg, Pt_ARG_ONOFF_STATE, 0, 0);
	PtGetResources(widget, 1, &arg);
	value = (int)arg.value;

	if(value) {
		PtSetArg(&arg, Pt_ARG_TEXT_STRING, "Dialog Enabled", 0);
		m_offmenu = 1;
	}
	else {
		PtSetArg(&arg, Pt_ARG_TEXT_STRING, "Dialog Disabled", 0);
	    m_offmenu = 0;
	}
	PtSetResources(widget, 1, &arg);

	off_eye(NULLI, NULLI);
	off_oeye(NULLI, NULLI);

	eyeOffDialogItemEnable(m_offmenu);

	return( Pt_CONTINUE );
}

void eyeOffDialogItemEnable(int eyeOffDialogEnabled)
{
	PtArg_t args[3];

	/* enable dialog items if dialog is enabled */
	if(eyeOffDialogEnabled) {
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_GHOST);
		PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_TRUE, Pt_SELECTABLE);
		PtSetResources(ABW_eyeHOvrRide, 2, args);
		PtSetResources(ABW_eyeVOvrRide, 2, args);
		PtSetResources(ABW_oeyeHOvrRide, 2, args);
		PtSetResources(ABW_oeyeVOvrRide, 2, args);
	}

	/* disable dialog items if dialog is disabled */
	else {
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_GHOST);
		PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_FALSE, Pt_SELECTABLE);
		PtSetResources(ABW_eyeHOvrRide, 2, args);
		PtSetResources(ABW_eyeVOvrRide, 2, args);
		PtSetResources(ABW_oeyeHOvrRide, 2, args);
		PtSetResources(ABW_oeyeVOvrRide, 2, args);
	}
}

int eyeOffMenuOverRide( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* determine which of the override buttons was pressed */
	if(ApName(widget) == ABN_eyeHOvrRide) {
		if(cbinfo->reason == Pt_CB_ARM) {
			eyeOffOvrRides[0] = 1;
			m_eho = eyeOffsets[0];

			PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, m_eho, 0);
			PtSetResources(ABW_eyeOffHorizOffAct, 1, &arg);
		}
		else if(cbinfo->reason == Pt_CB_DISARM) {
			eyeOffOvrRides[0] = 0;
			m_eho = NULLI;
		}
		off_eye(NULLI, NULLI);
	}
	else if(ApName(widget) == ABN_eyeVOvrRide) {
		if(cbinfo->reason == Pt_CB_ARM) {
			eyeOffOvrRides[1] = 1;
			m_evo = eyeOffsets[1];

			PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, m_evo, 0);
			PtSetResources(ABW_eyeOffVertOffAct, 1, &arg);
		}
		else if(cbinfo->reason == Pt_CB_DISARM) {
			eyeOffOvrRides[1] = 0;
			m_evo = NULLI;
		}
		off_eye(NULLI, NULLI);
	}
	else if(ApName(widget) == ABN_oeyeHOvrRide) {
		if(cbinfo->reason == Pt_CB_ARM) {
			eyeOffOvrRides[2] = 1;
			m_oeho = eyeOffsets[2];

			PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, m_oeho, 0);
			PtSetResources(ABW_eyeOffOthHorizOffAct, 1, &arg);
		}
		else if(cbinfo->reason == Pt_CB_DISARM) {
			eyeOffOvrRides[2] = 0;
			m_oeho = NULLI;
		}
		off_oeye(NULLI, NULLI);
	}
	else if(ApName(widget) == ABN_oeyeVOvrRide) {
		if(cbinfo->reason == Pt_CB_ARM) {
			eyeOffOvrRides[3] = 1;
			m_oevo = eyeOffsets[3];

			PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, m_oevo, 0);
			PtSetResources(ABW_eyeOffOthVertOffAct, 1, &arg);
		}
		else if(cbinfo->reason == Pt_CB_DISARM) {
			eyeOffOvrRides[3] = 0;
			m_oevo = NULLI;
		}
		off_oeye(NULLI, NULLI);
	}

	return( Pt_CONTINUE );
}

int eyeOff( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;
	long flags;
	int value;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the widget value */
	PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, 0, 0);
	PtGetResources(widget, 1, &arg);
	value = (int)arg.value;

	/* figure out which offset widget initiated this callback */
	if(ApName(widget) == ABN_eyeOffHorizOff) {
		eyeOffsets[0] = value;
		flags = PtWidgetFlags(ABW_eyeHOvrRide);
		if(flags & Pt_SET) {
			m_eho = eyeOffsets[0];
			off_eye(NULLI, NULLI);
		}
	}
	if(ApName(widget) == ABN_eyeOffVertOff) {
		eyeOffsets[1] = value;
		flags = PtWidgetFlags(ABW_eyeVOvrRide);
		if(flags & Pt_SET) {
			m_evo = eyeOffsets[1];
			off_eye(NULLI, NULLI);
		}
	}
	if(ApName(widget) == ABN_eyeOffOthHorizOff) {
		eyeOffsets[2] = value;
		flags = PtWidgetFlags(ABW_oeyeHOvrRide);
		if(flags & Pt_SET) {
			m_oeho = eyeOffsets[2];
			off_oeye(NULLI, NULLI);
		}
	}
	if(ApName(widget) == ABN_eyeOffOthVertOff) {
		eyeOffsets[3] = value;
		flags = PtWidgetFlags(ABW_eyeVOvrRide);
		if(flags & Pt_SET) {
			m_oevo = eyeOffsets[3];
			off_oeye(NULLI, NULLI);
		}
	}

	return( Pt_CONTINUE );
}

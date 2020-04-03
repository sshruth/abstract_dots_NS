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
#include "../../hdr/ramp.h"
int rampOvrRides[10][10] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

int rampAnchorPnt[] = { NULLI, NULLI, NULLI, NULLI, NULLI, NULLI, NULLI, NULLI, NULLI, NULLI };
int rampPreMsec[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int rampLength[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int rampAngle[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int rampVelocity[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int rampXoff[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int rampYoff[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int rampEcode[] = {1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000};
int rampPhiDev[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int rampPhiOnTime[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int rampPhiOffTime[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int rampDialogEnabled = 0;

int initRamps( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[3];
	PtListCallback_t *listData;
	int rampNum;
	char title[16];
	char anchor[16];

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* if this function was called from the list widget */
	if(PtWidgetClass(widget) == PtList) {
		if(cbinfo->reason_subtype != Pt_LIST_SELECTION_FINAL) return(Pt_CONTINUE);

		listData = (PtListCallback_t *)cbinfo->cbdata;
		sscanf(listData->item, "Ramp %d", &rampNum);
	}
	else {
		rampNum = 0;
		PtListSelectPos(ABW_rampList, 1);
	}

	/* set the module's title widget */
	sprintf(title, "Ramp %d", rampNum);
	PtSetArg(&args[0], Pt_ARG_TEXT_STRING, title, 0);
	PtSetResources(ABW_rampLabel, 1, &args[0]);

	/* set the state of the dialog enable button */
	if(ramp[rampNum].m_ra_menu) {
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, "Dialog Enabled", 0);
	}
	else {
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, "Dialog Disabled", 0);
	}
	PtSetArg(&args[1], Pt_ARG_ONOFF_STATE, ramp[rampNum].m_ra_menu, 0);
	PtSetResources(ABW_rampDialogButton, 2, args);

	/* set the states of the ramp anchor buttons */
	switch(rampAnchorPnt[rampNum]) {
	case NULLI:
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_rampCenPtToggle, 1, &args[0]);
		PtSetResources(ABW_rampBegPtToggle, 1, &args[0]);
		PtSetResources(ABW_rampEndPtToggle, 1, &args[0]);
		break;
	case RA_CENPT:
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(ABW_rampCenPtToggle, 1, &args[0]);

		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_rampBegPtToggle, 1, &args[0]);
		PtSetResources(ABW_rampEndPtToggle, 1, &args[0]);
		break;
	case RA_BEGINPT:
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(ABW_rampBegPtToggle, 1, &args[0]);

		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_rampCenPtToggle, 1, &args[0]);
		PtSetResources(ABW_rampEndPtToggle, 1, &args[0]);
		break;
	case RA_ENDPT:
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(ABW_rampEndPtToggle, 1, &args[0]);

		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_rampCenPtToggle, 1, &args[0]);
		PtSetResources(ABW_rampBegPtToggle, 1, &args[0]);
		break;
	}
	switch(rampPreMsec[rampNum]) {
	case RA_NO25MS:
		PtSetArg(&args[0], Pt_ARG_ONOFF_STATE, 1, 0);
		PtSetArg(&args[1], Pt_ARG_TEXT_STRING, "NO 25 MS", 0);
		PtSetResources(ABW_ramp25msButton, 2, args);
		break;
	default:
		PtSetArg(&args[0], Pt_ARG_ONOFF_STATE, 0, 0);
		PtSetArg(&args[1], Pt_ARG_TEXT_STRING, "25 MS PRE", 0);
		PtSetResources(ABW_ramp25msButton, 2, args);
	}

	/* set the numeric values of the override value widgets */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, rampLength[rampNum], 0);
	PtSetResources(ABW_rampLength, 1, &args[0]);

	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, rampAngle[rampNum], 0);
	PtSetResources(ABW_rampAngle, 1, &args[0]);

	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, rampVelocity[rampNum], 0);
	PtSetResources(ABW_rampVelocity, 1, &args[0]);

	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, rampXoff[rampNum], 0);
	PtSetResources(ABW_rampXoff, 1, &args[0]);

	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, rampYoff[rampNum], 0);
	PtSetResources(ABW_rampYoff, 1, &args[0]);

	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, rampEcode[rampNum], 0);
	PtSetResources(ABW_rampEcode, 1, &args[0]);

	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, rampPhiDev[rampNum], 0);
	PtSetResources(ABW_rampPhiDevice, 1, &args[0]);

	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, rampPhiOnTime[rampNum], 0);
	PtSetResources(ABW_rampPhiOnTime, 1, &args[0]);

	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, rampPhiOffTime[rampNum], 0);
	PtSetResources(ABW_rampPhiOffTime, 1, &args[0]);

	/* set the states of the menu override toggle buttons */
	/* ramp type over ride */
	if(rampOvrRides[rampNum][0]) PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
	else PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
	PtSetResources(ABW_rampAnchorOvrRide, 1, &args[0]);

	/* ramp length over ride */
	if(rampOvrRides[rampNum][1]) PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
	else PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
	PtSetResources(ABW_rampLengthOvrRide, 1, &args[0]);

	/* ramp angle over ride */
	if(rampOvrRides[rampNum][2]) PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
	else PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
	PtSetResources(ABW_rampAngleOvrRide, 1, &args[0]);

	/* ramp velocity over ride */
	if(rampOvrRides[rampNum][3]) PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
	else PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
	PtSetResources(ABW_rampVelocityOvrRide, 1, &args[0]);

	/* ramp X offset over ride */
	if(rampOvrRides[rampNum][4]) PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
	else PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
	PtSetResources(ABW_rampXoffOvrRide, 1, &args[0]);

	/* ramp Y offset over ride */
	if(rampOvrRides[rampNum][5]) PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
	else PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
	PtSetResources(ABW_rampYoffOvrRide, 1, &args[0]);

	/* ramp ecode over ride */
	if(rampOvrRides[rampNum][6]) PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
	else PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
	PtSetResources(ABW_rampEcodeOvrRide, 1, &args[0]);

	/* ramp phi device over ride */
	if(rampOvrRides[rampNum][7]) PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
	else PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
	PtSetResources(ABW_rampPhiDevOvrRide, 1, &args[0]);

	/* ramp phi on time over ride */
	if(rampOvrRides[rampNum][8]) PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
	else PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
	PtSetResources(ABW_rampPhiOnOvrRide, 1, &args[0]);

	/* ramp phi off time over ride */
	if(rampOvrRides[rampNum][9]) PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
	else PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
	PtSetResources(ABW_rampPhiOffOvrRide, 1, &args[0]);

	/* set the values of the module's "actual" widgets */
	/* ramp type */
	if(ramp[rampNum].ra_type & RA_CENPT) strcpy(anchor, "CENPT|");
	else if(ramp[rampNum].ra_type & RA_BEGINPT) strcpy(anchor, "BEGPT|");
	else if(ramp[rampNum].ra_type & RA_ENDPT) strcpy(anchor, "ENDPT|");

	if(ramp[rampNum].ra_type & RA_NO25MS) strcat(anchor, "NO25MS");
	else strcat(anchor, "25MS");
	PtSetArg(&args[0], Pt_ARG_TEXT_STRING, anchor, 0);
	PtSetResources(ABW_rampAnchorAct, 1, &args[0]);

	/* ramp length */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, ramp[rampNum].ra_len, 0);
	PtSetResources(ABW_rampLengthAct, 1, &args[0]);

	/* ramp angle */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, ramp[rampNum].ra_ang, 0);
	PtSetResources(ABW_rampAngleAct, 1, &args[0]);
	
	/* ramp velocity */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, ramp[rampNum].ra_vel, 0);
	PtSetResources(ABW_rampVelocityAct, 1, &args[0]);

	/* ramp X offset */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, ramp[rampNum].ra_xoff, 0);
	PtSetResources(ABW_rampXoffAct, 1, &args[0]);

	/* ramp Y offset */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, ramp[rampNum].ra_yoff, 0);
	PtSetResources(ABW_rampYoffAct, 1, &args[0]);

	/* ramp ecode */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, ramp[rampNum].ra_ecode, 0);
	PtSetResources(ABW_rampEcodeAct, 1, &args[0]);

	/* ramp phi device */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, ramp[rampNum].ra_phidev, 0);
	PtSetResources(ABW_rampPhiDeviceAct, 1, &args[0]);

	/* ramp phi On time */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, ramp[rampNum].ra_pontime, 0);
	PtSetResources(ABW_rampPhiOnTimeAct, 1, &args[0]);

	/* ramp phi Off time */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, ramp[rampNum].ra_pofftime, 0);
	PtSetResources(ABW_rampPhiOffTimeAct, 1, &args[0]);


	/* set the ramp number in all of this module's widgets */
	PtSetArg(&args[0], Pt_ARG_USER_DATA, &rampNum, sizeof(int));
	PtSetResources(ABW_rampDialogButton, 1, &args[0]);
	PtSetResources(ABW_rampAnchorGroup, 1, &args[0]);
	PtSetResources(ABW_ramp25msButton, 1, &args[0]);
	PtSetResources(ABW_rampLength, 1, &args[0]);
	PtSetResources(ABW_rampAngle, 1, &args[0]);
	PtSetResources(ABW_rampVelocity, 1, &args[0]);
	PtSetResources(ABW_rampXoff, 1, &args[0]);
	PtSetResources(ABW_rampYoff, 1, &args[0]);
	PtSetResources(ABW_rampEcode, 1, &args[0]);
	PtSetResources(ABW_rampPhiDevice, 1, &args[0]);
	PtSetResources(ABW_rampPhiOnTime, 1, &args[0]);
	PtSetResources(ABW_rampPhiOffTime, 1, &args[0]);
	PtSetResources(ABW_rampAnchorOvrRide, 1, &args[0]);
	PtSetResources(ABW_rampLengthOvrRide, 1, &args[0]);
	PtSetResources(ABW_rampAngleOvrRide, 1, &args[0]);
	PtSetResources(ABW_rampVelocityOvrRide, 1, &args[0]);
	PtSetResources(ABW_rampXoffOvrRide, 1, &args[0]);
	PtSetResources(ABW_rampYoffOvrRide, 1, &args[0]);
	PtSetResources(ABW_rampEcodeOvrRide, 1, &args[0]);
	PtSetResources(ABW_rampPhiDevOvrRide, 1, &args[0]);
	PtSetResources(ABW_rampPhiOnOvrRide, 1, &args[0]);
	PtSetResources(ABW_rampPhiOffOvrRide, 1, &args[0]);

	rampDialogItemEnable(rampNum);

	return( Pt_CONTINUE );
}

int rampDialogSet( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[2];
	int *pRampNum;
	int *value;
	int rampNum;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the status of the on-off switch */
	PtSetArg(&args[0], Pt_ARG_ONOFF_STATE, &value, 0);
	PtSetArg(&args[1], Pt_ARG_USER_DATA, &pRampNum, 0);
	PtGetResources(widget, 2, args);
	rampNum = *pRampNum;

	/* change the text of the widget to reflect the current state */
	if(*value) {
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, "Dialog Enabled", 0);
		ramp[rampNum].m_ra_menu = 1;
	}
	else {
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, "Dialog Disabled", 0);
		ramp[rampNum].m_ra_menu = 0;
	}
	PtSetResources(widget, 1, args);

	rampDialogItemEnable(rampNum);

	return( Pt_CONTINUE );
}

void rampDialogItemEnable(int rampNum)
{
	PtArg_t args[2];

	/* enable dialog items if menu is enabled */
	if(ramp[rampNum].m_ra_menu) {
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_GHOST);
		PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_TRUE, Pt_SELECTABLE);
	}

	/* disable dialog items if menu is disabled */
	else {
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_GHOST);
		PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_FALSE, Pt_SELECTABLE);
	}
	PtSetResources(ABW_rampAnchorOvrRide, 2, args);
	PtSetResources(ABW_rampLengthOvrRide, 2, args);
	PtSetResources(ABW_rampAngleOvrRide, 2, args);
	PtSetResources(ABW_rampVelocityOvrRide, 2, args);
	PtSetResources(ABW_rampXoffOvrRide, 2, args);
	PtSetResources(ABW_rampYoffOvrRide, 2, args);
	PtSetResources(ABW_rampEcodeOvrRide, 2, args);
	PtSetResources(ABW_rampPhiDevOvrRide, 2, args);
	PtSetResources(ABW_rampPhiOnOvrRide, 2, args);
	PtSetResources(ABW_rampPhiOffOvrRide, 2, args);
}

int rampAnchor( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;
	int rampNum;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the ramp number from the toggle group */
	PtSetArg(&arg, Pt_ARG_USER_DATA, 0, 0);
	PtGetResources(ABW_rampAnchorGroup, 1, &arg);
	rampNum = *(int *)arg.value;

	/* figure out which toggle was pressed */
	if(ApName(widget) == ABN_rampCenPtToggle) {
		rampAnchorPnt[rampNum] = RA_CENPT;
	}
	else if(ApName(widget) == ABN_rampBegPtToggle) {
		rampAnchorPnt[rampNum] = RA_BEGINPT;
	}
	else if(ApName(widget) == ABN_rampEndPtToggle) {
		rampAnchorPnt[rampNum] = RA_ENDPT;
	}
	
	/* if the menu over ride is set change the menu value */
	if(rampOvrRides[rampNum][0]) {
		ramp[rampNum].m_ra_type = rampAnchorPnt[rampNum];
		switch(rampPreMsec[rampNum]) {
		case 0:
			ramp[rampNum].m_ra_type &= ~1;
			break;
		case 1:
			ramp[rampNum].m_ra_type |= 1;
			break;
		}
		ra_new(rampNum, NULLI, NULLI, NULLI, NULLI, NULLI, NULLI, NULLI);
	}
	return( Pt_CONTINUE );
}

int rampPreTime( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[2];
	int *value;
	int rampNum;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the status of the on-off switch */
	PtSetArg(&args[0], Pt_ARG_ONOFF_STATE, &value, 0);
	PtSetArg(&args[1], Pt_ARG_USER_DATA, 0, 0);
	PtGetResources(widget, 2, args);
	rampNum = *(int *)args[1].value;

	/* change the text of the widget to reflect the current state */
	if(*value) {
		rampPreMsec[rampNum] = RA_NO25MS;
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, "NO 25 MS", 0);
	}
	else {
		rampPreMsec[rampNum] = 0;
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, "25 MS PRE", 0);
	}
	PtSetResources(widget, 1, args);

	/* if the menu over ride is set change the menu value */
	if(rampOvrRides[rampNum][0]) {
		ramp[rampNum].m_ra_type = rampAnchorPnt[rampNum];
		switch(rampPreMsec[rampNum]) {
		case 0:
			ramp[rampNum].m_ra_type &= ~1;
			break;
		case 1:
			ramp[rampNum].m_ra_type |= 1;
			break;
		}
		ra_new(rampNum, NULLI, NULLI, NULLI, NULLI, NULLI, NULLI, NULLI);
	}
	return( Pt_CONTINUE );
}

int rampParameters( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[2];
	int rampNum;
	int value;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the ramp number and the value from the widget */
	PtSetArg(&args[0], Pt_ARG_USER_DATA, 0, 0);
	PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, 0, 0);
	PtGetResources(widget, 2, args);
	rampNum = *(int *)args[0].value;
	value = (int)args[1].value;

	/* figure out which parameter to change */
	if(ApName(widget) == ABN_rampLength) {
		rampLength[rampNum] = value;
		if(rampOvrRides[rampNum][1]) {
			ramp[rampNum].m_ra_len = rampLength[rampNum];
			ra_new(rampNum, NULLI, NULLI, NULLI, NULLI, NULLI, NULLI, NULLI);
		}
	}
	else if(ApName(widget) == ABN_rampAngle) {
		rampAngle[rampNum] = value;
		if(rampOvrRides[rampNum][2]) {
			ramp[rampNum].m_ra_ang = rampAngle[rampNum];
			ra_new(rampNum, NULLI, NULLI, NULLI, NULLI, NULLI, NULLI, NULLI);
		}
	}
	else if(ApName(widget) == ABN_rampVelocity) {
		rampVelocity[rampNum] = value;
		if(rampOvrRides[rampNum][3]) {
			ramp[rampNum].m_ra_vel = rampVelocity[rampNum];
			ra_new(rampNum, NULLI, NULLI, NULLI, NULLI, NULLI, NULLI, NULLI);
		}
	}
	else if(ApName(widget) == ABN_rampXoff) {
		rampXoff[rampNum] = value;
		if(rampOvrRides[rampNum][4]) {
			ramp[rampNum].m_ra_xoff = rampXoff[rampNum];
			ra_new(rampNum, NULLI, NULLI, NULLI, NULLI, NULLI, NULLI, NULLI);
		}
	}
	else if(ApName(widget) == ABN_rampYoff) {
		rampYoff[rampNum] = value;
		if(rampOvrRides[rampNum][5]) {
			ramp[rampNum].m_ra_yoff = rampYoff[rampNum];
			ra_new(rampNum, NULLI, NULLI, NULLI, NULLI, NULLI, NULLI, NULLI);
		}
	}
	else if(ApName(widget) == ABN_rampEcode) {
		rampEcode[rampNum] = value;
		if(rampOvrRides[rampNum][6]) {
			ramp[rampNum].m_ra_ecode = rampEcode[rampNum];
			ra_new(rampNum, NULLI, NULLI, NULLI, NULLI, NULLI, NULLI, NULLI);
		}
	}
	else if(ApName(widget) == ABN_rampPhiDevice) {
		rampPhiDev[rampNum] = value;
		if(rampOvrRides[rampNum][7]) ramp[rampNum].ra_phidev = rampPhiDev[rampNum];
	}
	else if(ApName(widget) == ABN_rampPhiOnTime) {
		rampPhiOnTime[rampNum] = value;
		if(rampOvrRides[rampNum][8]) ramp[rampNum].ra_pontime = rampPhiOnTime[rampNum];
	}
	else if(ApName(widget) == ABN_rampPhiOffTime) {
		rampPhiOffTime[rampNum] = value;
		if(rampOvrRides[rampNum][9]) ramp[rampNum].ra_pofftime = rampPhiOffTime[rampNum];
	}

	return( Pt_CONTINUE );
}

int rampMenuOverRide( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;
	int rampNum;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the ramp number from the widget */
	PtSetArg(&arg, Pt_ARG_USER_DATA, 0, 0);
	PtGetResources(widget, 1, &arg);
	rampNum = *(int *)arg.value;

	/* determine which of the override buttons initiated this callback */
	if(ApName(widget) == ABN_rampAnchorOvrRide) {
		if(cbinfo->reason == Pt_CB_ARM) {
			rampOvrRides[rampNum][0] = 1;
			ramp[rampNum].m_ra_type = rampAnchorPnt[rampNum];
			switch(rampPreMsec[rampNum]) {
			case 0:
				ramp[rampNum].m_ra_type &= ~1;
				break;
			case 1:
				ramp[rampNum].m_ra_type |= 1;
				break;
			}
		}
		else if(cbinfo->reason == Pt_CB_DISARM) {
			rampOvrRides[rampNum][0] = 0;
			ramp[rampNum].m_ra_type = NULLI;
		}
	}
	else if(ApName(widget) == ABN_rampLengthOvrRide) {
		if(cbinfo->reason == Pt_CB_ARM) {
			rampOvrRides[rampNum][1] = 1;
			ramp[rampNum].m_ra_len = rampLength[rampNum];
		}
		else if(cbinfo->reason == Pt_CB_DISARM) {
			rampOvrRides[rampNum][1] = 0;
			ramp[rampNum].m_ra_len = NULLI;
		}
	}
	else if(ApName(widget) == ABN_rampAngleOvrRide) {
		if(cbinfo->reason == Pt_CB_ARM) {
			rampOvrRides[rampNum][2] = 1;
			ramp[rampNum].m_ra_ang = rampAngle[rampNum];
		}
		else if(cbinfo->reason == Pt_CB_DISARM) {
			rampOvrRides[rampNum][2] = 0;
			ramp[rampNum].m_ra_ang = NULLI;
		}
	}
	else if(ApName(widget) == ABN_rampVelocityOvrRide) {
		if(cbinfo->reason == Pt_CB_ARM) {
			rampOvrRides[rampNum][3] = 1;
			ramp[rampNum].m_ra_vel = rampVelocity[rampNum];
		}
		else if(cbinfo->reason == Pt_CB_DISARM) {
			rampOvrRides[rampNum][3] = 0;
			ramp[rampNum].m_ra_vel = NULLI;
		}
	}
	else if(ApName(widget) == ABN_rampXoffOvrRide) {
		if(cbinfo->reason == Pt_CB_ARM) {
			rampOvrRides[rampNum][4] = 1;
			ramp[rampNum].m_ra_xoff = rampXoff[rampNum];
		}
		else if(cbinfo->reason == Pt_CB_DISARM) {
			rampOvrRides[rampNum][4] = 0;
			ramp[rampNum].m_ra_xoff = NULLI;
		}
	}
	else if(ApName(widget) == ABN_rampYoffOvrRide) {
		if(cbinfo->reason == Pt_CB_ARM) {
			rampOvrRides[rampNum][5] = 1;
			ramp[rampNum].m_ra_yoff = rampYoff[rampNum];
		}
		else if(cbinfo->reason == Pt_CB_DISARM) {
			rampOvrRides[rampNum][5] = 0;
			ramp[rampNum].m_ra_yoff = NULLI;
		}
	}
	else if(ApName(widget) == ABN_rampEcodeOvrRide) {
		if(cbinfo->reason == Pt_CB_ARM) {
			rampOvrRides[rampNum][6] = 1;
			ramp[rampNum].m_ra_ecode = rampEcode[rampNum];
		}
		else if(cbinfo->reason == Pt_CB_DISARM) {
			rampOvrRides[rampNum][6] = 0;
			ramp[rampNum].m_ra_ecode = NULLI;
		}
	}
	else if(ApName(widget) == ABN_rampPhiDevOvrRide) {
		if(cbinfo->reason == Pt_CB_ARM) {
			rampOvrRides[rampNum][7] = 1;
			ramp[rampNum].ra_phidev = rampPhiDev[rampNum];
		}
		else if(cbinfo->reason == Pt_CB_DISARM) {
			rampOvrRides[rampNum][7] = 0;
			ramp[rampNum].ra_phidev = 0;
		}
	}
	else if(ApName(widget) == ABN_rampPhiOnOvrRide) {
		if(cbinfo->reason == Pt_CB_ARM) {
			rampOvrRides[rampNum][8] = 1;
			ramp[rampNum].ra_pontime = rampPhiOnTime[rampNum];
		}
		else if(cbinfo->reason == Pt_CB_DISARM) {
			rampOvrRides[rampNum][8] = 0;
			ramp[rampNum].ra_pontime = 1;
		}
	}
	else if(ApName(widget) == ABN_rampPhiOffOvrRide) {
		if(cbinfo->reason == Pt_CB_ARM) {
			rampOvrRides[rampNum][9] = 1;
			ramp[rampNum].ra_pofftime = rampPhiOffTime[rampNum];
		}
		else if(cbinfo->reason == Pt_CB_DISARM) {
			rampOvrRides[rampNum][9] = 0;
			ramp[rampNum].ra_pofftime = 99;
		}
	}
	/* compute new ramp */
	ra_new(rampNum, NULLI, NULLI, NULLI, NULLI, NULLI, NULLI, NULLI);

	return( Pt_CONTINUE );
}

int initRampSum( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	ApDBase_t *rampSumdbase;
	PtArg_t arg;
	int i;
	int y;
	char label[4];
	char anchor[16];

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	/* open the ramp summary data base */
	rampSumdbase = ApOpenDBase(ABM_rampSumPictures);

	/* set the parent for the widgets */
	PtSetParentWidget(ABW_rampSumScrollArea);

	/* create 10 sets of widgets for the ramp summary */
	y = 0;
	for(i = 0; i < RA_MAXNUM; i++) {

		/* create the label widget for this signal */
		sprintf(label, "%d", i);
		PtSetArg(&arg, Pt_ARG_TEXT_STRING, label, 0);
		ApCreateWidget(rampSumdbase, "rampSumLabel", 13, y, 1, &arg);

		/* create the ramp anchor text widget for this signal */
		if(ramp[i].ra_type & RA_CENPT) strcpy(anchor, "CENPT|");
		else if(ramp[i].ra_type & RA_BEGINPT) strcpy(anchor, "BEGPT|");
		else if(ramp[i].ra_type & RA_ENDPT) strcpy(anchor, "ENDPT|");
		if(ramp[i].ra_type & RA_NO25MS) strcat(anchor, "NO25MS");
		else strcat(anchor, "25MS");
		PtSetArg(&arg, Pt_ARG_TEXT_STRING, anchor, 0);
		ApCreateWidget(rampSumdbase, "rampSumAnchorText", 45, y, 1, &arg);

		/* create the ramp length widget for this signal */
		PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, ramp[i].ra_len, 0);
		ApCreateWidget(rampSumdbase, "rampSumLength", 164, y, 1, &arg);

		/* create the ramp angle widget for this signal */
		PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, ramp[i].ra_ang, 0);
		ApCreateWidget(rampSumdbase, "rampSumAngle", 225, y, 1, &arg);

		/* create the ramp velocity widget for this signal */
		PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, ramp[i].ra_vel, 0);
		ApCreateWidget(rampSumdbase, "rampSumVelocity", 287, y, 1, &arg);

		/* create the ramp X offset widget for this signal */
		PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, ramp[i].ra_xoff, 0);
		ApCreateWidget(rampSumdbase, "rampSumXoff", 350, y, 1, &arg);

		/* create the ramp Y offset widget for this signal */
		PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, ramp[i].ra_yoff, 0);
		ApCreateWidget(rampSumdbase, "rampSumYoff", 410, y, 1, &arg);

		/* create the ramp ecode widget for this signal */
		PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, ramp[i].ra_ecode, 0);
		ApCreateWidget(rampSumdbase, "rampSumEcode", 467, y, 1, &arg);

		/* create the ramp phi device widget for this signal */
		PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, ramp[i].ra_phidev, 0);
		ApCreateWidget(rampSumdbase, "rampSumPhi", 526, y, 1, &arg);

		/* create the ramp phi on time widget for this signal */
		PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, ramp[i].ra_pontime, 0);
		ApCreateWidget(rampSumdbase, "rampSumPhiOn", 595, y, 1, &arg);

		/* create the ramp phi off time widget for this signal */
		PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, ramp[i].ra_pofftime, 0);
		ApCreateWidget(rampSumdbase, "rampSumPhiOff", 662, y, 1, &arg);

		y += 30;
	}

	/* close the widget database */
	ApCloseDBase(rampSumdbase);

	return( Pt_CONTINUE );
}

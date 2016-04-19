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

char *daSourceNames[] = {
	"standby",
	"ramp_x",
	"ramp_y",
	"joystick_x",
	"joystick_y",
	"signal",
	"window_x",
	"window_y",
	"mem_array"
};

int daMenuOvrRides[16][3] = {
	{ 0, 0, 0 },
	{ 0, 0, 0 },
	{ 0, 0, 0 },
	{ 0, 0, 0 },
	{ 0, 0, 0 },
	{ 0, 0, 0 },
	{ 0, 0, 0 },
	{ 0, 0, 0 },
	{ 0, 0, 0 },
	{ 0, 0, 0 },
	{ 0, 0, 0 },
	{ 0, 0, 0 },
	{ 0, 0, 0 },
	{ 0, 0, 0 },
	{ 0, 0, 0 },
	{ 0, 0, 0 }
};
int daSources[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int daModes[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int daSourceNumbs[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int daOutputs[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int daOffsets[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

extern int daSourceToggles[9];

int initDADialog( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtWidget_t *dialog;
	PtWidget_t *toggle;
	PtArg_t args[3];
	PtListCallback_t *listData;
	int daNum;
	int toggleOverride;
	int i;
	char title[16];

	void daDialogItemEnable();

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	/* if this function was called from the list widget */
	if(PtWidgetClass(link_instance) == PtList) {
		if(cbinfo->reason_subtype != Pt_LIST_SELECTION_FINAL) return(Pt_CONTINUE);

		listData = (PtListCallback_t *)cbinfo->cbdata;
		sscanf(listData->item, "DA %d", &daNum);
		dialog = ApGetInstance(link_instance);
	}
	else {
		daNum = 0;
		PtListSelectPos(ABW_daConvList, 1);
		dialog = link_instance;
	}

	/*set the module's title widget */
	sprintf(title, "DA %d", daNum);
	PtSetArg(&args[0], Pt_ARG_TEXT_STRING, title, 0);
	PtSetResources(ABW_daDialogLabel, 1, &args[0]);

	/* set the state of the dialog enable button */
	if(da[daNum].m_da_menu) {
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, "Dialog Enabled", 0);
	}
	else {
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, "Dialog Disabled", 0);
	}
	PtSetArg(&args[1], Pt_ARG_ONOFF_STATE, da[daNum].m_da_menu, 0);
	PtSetResources(ABW_daMenuOnOffButton, 2, args);

	/* set the states of the source toggles */
	toggleOverride = -1;
	for(i = 0; i < 9; i++) {
		toggle = ApGetWidgetPtr(dialog, daSourceToggles[i]);
		if(i == da[daNum].m_da_source) {
			PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
			toggleOverride = i;
		}
		else {
			PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		}
		PtSetResources(toggle, 1, &args[0]);
	}
	/* set the states of the mode toggles */
	switch(da[daNum].m_da_mode) {
	case 0:
		toggleOverride = 0;
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(ABW_daDirectToggle, 1, &args[0]);

		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_daOffConToggle, 1, &args[0]);
		PtSetResources(ABW_daOffStbyToggle, 1, &args[0]);
		break;
	case 1:
		toggleOverride = 1;
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(ABW_daOffConToggle, 1, &args[0]);

		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_daDirectToggle, 1, &args[0]);
		PtSetResources(ABW_daOffStbyToggle, 1, &args[0]);
		break;
	case 2:
		toggleOverride = 2;
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(ABW_daOffStbyToggle, 1, &args[0]);

		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_daDirectToggle, 1, &args[0]);
		PtSetResources(ABW_daOffConToggle, 1, &args[0]);
		break;
	default:
		toggleOverride = -1;
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_daDirectToggle, 1, &args[0]);
		PtSetResources(ABW_daOffConToggle, 1, &args[0]);
		PtSetResources(ABW_daOffStbyToggle, 1, &args[0]);
		break;
	}

	/* set the states of the menu override buttons */
	if(daMenuOvrRides[daNum][0]) PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
	else PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
	PtSetResources(ABW_daSourceNumOvrRide, 1, &args[0]);

	if(daMenuOvrRides[daNum][1]) PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
	else PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
	PtSetResources(ABW_daOutputOvrRide, 1, &args[0]);
	
	if(daMenuOvrRides[daNum][2]) PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
	else PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
	PtSetResources(ABW_daOffsetOvrRide, 1, &args[0]);

	/* set the numeric values of the override value widgets */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, daSourceNumbs[daNum], 0);
	PtSetResources(ABW_daSourceNum, 1, &args[0]);

	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, daOutputs[daNum], 0);
	PtSetResources(ABW_daOutput, 1, &args[0]);

	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, daOffsets[daNum], 0);
	PtSetResources(ABW_daOffset, 1, &args[0]);

	/* set the values of the module's "actual" widgets */
	/* source name */
	i = da[daNum].da_source;
	PtSetArg(&args[0], Pt_ARG_TEXT_STRING, daSourceNames[i], 0);
	PtSetResources(ABW_daSourceAct, 1, args);

	/* mode */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, da[daNum].da_mode, 0);
	PtSetResources(ABW_daModeAct, 1, args);

	/* source number */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, da[daNum].da_src_num, 0);
	PtSetResources(ABW_daSourceNumAct, 1, args);

	/* output value */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, da[daNum].m_da_val, 0);
	PtSetResources(ABW_daOutputAct, 1, args);

	/* offset */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, (da[daNum].da_offset >> TEN_TO_IC), 0);
	PtSetResources(ABW_daOffsetAct, 1, args);
	
	/* set the da number in all of this module's widgets */
	PtSetArg(&args[0], Pt_ARG_USER_DATA, &daNum, sizeof(int));
	PtSetResources(ABW_daMenuOnOffButton, 1, &args[0]);
	PtSetResources(ABW_daSourceGroup, 1, &args[0]);
	PtSetResources(ABW_daModeGroup, 1, &args[0]);
	PtSetResources(ABW_daSourceNumOvrRide, 1, &args[0]);
	PtSetResources(ABW_daSourceNum, 1, &args[0]);
	PtSetResources(ABW_daOutputOvrRide, 1, &args[0]);
	PtSetResources(ABW_daOutput, 1, &args[0]);
	PtSetResources(ABW_daOffsetOvrRide, 1, &args[0]);
	PtSetResources(ABW_daOffset, 1, &args[0]);
	
	daDialogItemEnable(daNum);

	return( Pt_CONTINUE );
}

int daConvDialogSet( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[2];
	int *pDaNum;
	int *value;
	int daNum;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the status of the on-off switch */
	PtSetArg(&args[0], Pt_ARG_ONOFF_STATE, &value, 0);
	PtSetArg(&args[1], Pt_ARG_USER_DATA, &pDaNum, 0);
	PtGetResources(widget, 2, args);
	daNum = *pDaNum;

	if(*value) {
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, "Dialog Enabled", 0);
		da[daNum].m_da_menu = 1;
	}
	else {
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, "Dialog Disabled", 0);
		da[daNum].m_da_menu = 0;
	}
	PtSetResources(widget, 1, args);

	daDialogItemEnable(daNum);

	da_cntrl_1(daNum, NULLI, NULLI);
	da_mode(daNum, NULLI);
	da_offset(daNum, NULLI);

	return( Pt_CONTINUE );
}

void daDialogItemEnable(int daNum)
{
	PtArg_t args[2];

	/* enable dialog items if menu is enabled */
	if(da[daNum].m_da_menu) {
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_GHOST);
		PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_TRUE, Pt_SELECTABLE);
	}
	else {
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_GHOST);
		PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_FALSE, Pt_SELECTABLE);
	}
	PtSetResources(ABW_daStandbyToggle, 2, args);
	PtSetResources(ABW_daRampXToggle, 2, args);
	PtSetResources(ABW_daRampYToggle, 2, args);
	PtSetResources(ABW_daJoyXToggle, 2, args);
	PtSetResources(ABW_daJoyYToggle, 2, args);
	PtSetResources(ABW_daSignalToggle, 2, args);
	PtSetResources(ABW_daWinXToggle, 2, args);
	PtSetResources(ABW_daWinYToggle, 2, args);
	PtSetResources(ABW_daMemToggle, 2, args);

	PtSetResources(ABW_daDirectToggle, 2, args);
	PtSetResources(ABW_daOffConToggle, 2, args);
	PtSetResources(ABW_daOffStbyToggle, 2, args);

	PtSetResources(ABW_daSourceNumOvrRide, 2, args);
	PtSetResources(ABW_daOutputOvrRide, 2, args);
	PtSetResources(ABW_daOffsetOvrRide, 2, args);

	return;
}

int setDASource( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtWidget_t *parent;
	PtArg_t arg;
	int daNum;
	int source;
	int i;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the widget's group */
	parent = PtWidgetParent(widget);

	/* get the da number from the group */
	PtSetArg(&arg, Pt_ARG_USER_DATA, 0, 0);
	PtGetResources(parent, 1, &arg);
	daNum = *(int *)arg.value;

	/* figure out which source was chosen */
	for(i = 0; i < 9; i++) {
		if(ApName(widget) == daSourceToggles[i]) source = i;
	}

	if(cbinfo->reason == Pt_CB_ARM) {
		da[daNum].m_da_source = source;
	}
	else if(cbinfo->reason == Pt_CB_DISARM) {
		da[daNum].m_da_source = NULLI;
	}

	da_cntrl_1(daNum, NULLI, NULLI);

	return( Pt_CONTINUE );
}

int setDAMode( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtWidget_t *parent;
	PtArg_t arg;
	long *flags;
	int daNum;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the widget's group */
	parent = PtWidgetParent(widget);

	/* get the da number from the group */
	PtSetArg(&arg, Pt_ARG_USER_DATA, 0, 0);
	PtGetResources(parent, 1, &arg);
	daNum = *(int *)arg.value;

	/* get the state of this buttons flags */
	PtSetArg(&arg, Pt_ARG_FLAGS, &flags, 0);
	PtGetResources(widget, 1, &arg);

	/* get the name of the toggle button that was pressed */
	if(ApName(widget) == ABN_daDirectToggle) {
		if(*flags & Pt_SET) {
			da[daNum].m_da_mode = DA_DIRECT;

			PtSetArg(&arg, Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
			PtSetResources(ABW_daOffConToggle, 1, &arg);
			PtSetResources(ABW_daOffStbyToggle, 1, &arg);
		}
		else {
			da[daNum].m_da_mode = NULLI;
		}
	}
	else if(ApName(widget) == ABN_daOffConToggle) {
		if(*flags & Pt_SET) {
			if(da[daNum].m_da_mode == NULLI) da[daNum].m_da_mode = DA_OFFSET_CONN;
			else da[daNum].m_da_mode |= DA_OFFSET_CONN;

			PtSetArg(&arg, Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
			PtSetResources(ABW_daDirectToggle, 1, &arg);
		}
		else {
			if(da[daNum].m_da_mode == DA_OFFSET_CONN) da[daNum].m_da_mode = NULLI;
			else da[daNum].m_da_mode &= ~DA_OFFSET_CONN;
		}
	}
	else if(ApName(widget) == ABN_daOffStbyToggle) {
		if(*flags & Pt_SET) {
			if(da[daNum].m_da_mode == NULLI) da[daNum].m_da_mode = DA_OFFSET_STBY;
			else da[daNum].m_da_mode |= DA_OFFSET_STBY;

			PtSetArg(&arg, Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
			PtSetResources(ABW_daDirectToggle, 1, &arg);
		}
		else {
			if(da[daNum].m_da_mode == DA_OFFSET_STBY) da[daNum].m_da_mode = NULLI;
			else da[daNum].m_da_mode &= ~DA_OFFSET_STBY;
		}
	}

	da_mode(daNum, NULLI);

	return( Pt_CONTINUE );
}

int daMenuOverRide( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;
	int daNum;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the da number from the widget */
	PtSetArg(&arg, Pt_ARG_USER_DATA, 0, 0);
	PtGetResources(widget, 1, &arg);
	daNum = *(int *)arg.value;

	/* determin which of the override buttons initiated this callback */
	if(ApName(widget) == ABN_daSourceNumOvrRide) {
		if(cbinfo->reason == Pt_CB_ARM) {
			daMenuOvrRides[daNum][0] = 1;
			da[daNum].m_da_src_num = daSourceNumbs[daNum];
		}
		else if(cbinfo->reason == Pt_CB_DISARM) {
			daMenuOvrRides[daNum][0] = 0;
			da[daNum].m_da_src_num = NULLI;
		}
		da_cntrl_1(daNum, NULLI, NULLI);
	}
	else if(ApName(widget) == ABN_daOutputOvrRide) {
		if(cbinfo->reason == Pt_CB_ARM) {
			daMenuOvrRides[daNum][1] = 1;
			da[daNum].m_da_val = daOutputs[daNum];
		}
		else if(cbinfo->reason == Pt_CB_DISARM) {
			daMenuOvrRides[daNum][1] = 0;
			da[daNum].m_da_val = NULLI;
		}
		da_set_1(daNum, NULLI);
	}
	else if(ApName(widget) == ABN_daOffsetOvrRide) {
		if(cbinfo->reason == Pt_CB_ARM) {
			daMenuOvrRides[daNum][2] = 1;
			da[daNum].m_da_offset = daOffsets[daNum];
		}
		else if(cbinfo->reason == Pt_CB_DISARM) {
			daMenuOvrRides[daNum][2] = 0;
			da[daNum].m_da_offset = NULLI;
		}
		da_offset(daNum, NULLI);
	}

	return( Pt_CONTINUE );
}

int setDAParameters( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[2];
	int daNum;
	int value;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the da number and value from the widget */
	PtSetArg(&args[0], Pt_ARG_USER_DATA, 0, 0);
	PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, 0, 0);
	PtGetResources(widget, 2, args);
	daNum = *(int *)args[0].value;
	value = (int)args[1].value;

	/* figure out which parameter to change */
	if(ApName(widget) == ABN_daSourceNum) {
		daSourceNumbs[daNum] = value;
		if(daMenuOvrRides[daNum][0]) {
			da[daNum].m_da_src_num = daSourceNumbs[daNum];
			da_cntrl_1(daNum, NULLI, NULLI);
		}
	}
	else if(ApName(widget) == ABN_daOutput) {
		daOutputs[daNum] = value;
		if(daMenuOvrRides[daNum][1]) {
			da[daNum].m_da_val = daOutputs[daNum];
			da_set_1(daNum, NULLI);
		}
	}
	else if(ApName(widget) == ABN_daOffset) {
		daOffsets[daNum] = value;
		if(daMenuOvrRides[daNum][2]) {
			da[daNum].m_da_offset = daOffsets[daNum];
			da_offset(daNum, NULLI);
		}
	}

	return( Pt_CONTINUE );
}

int initDASummary( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	ApDBase_t *daSumdbase;
	PtArg_t args[2];
	int y;
	int i;
	int j;
	char label[4];

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	/* open the d to a converter summary data base */
	daSumdbase = ApOpenDBase(ABM_daSumPictures);

	/* set the parent for the widgets */
	PtSetParentWidget(ABW_daSumScrollArea);

	/* create 16 sets of widgets for the da summary */
	y = 0;
	for(i = 0; i < 16; i++) {

		/* create the label widget for this da converter */
		sprintf(label, "%d", i);
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, label, 0);
		ApCreateWidget(daSumdbase, "daSumDAlabel", 10, y, 2, args);

		/* create the source name text widget for this da converter */
		j = da[i].da_source;
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, daSourceNames[j], 0);
		ApCreateWidget(daSumdbase, "daSumDAname", 50, y, 2, args);

		/* create the source number widget for this da converter */
		PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, da[i].da_src_num, 0);
		ApCreateWidget(daSumdbase, "daSumDAnumber", 138, y, 2, args);

		/* create the mode widget for this da converter */
		PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, da[i].da_mode, 0);
		ApCreateWidget(daSumdbase, "daSumDAmode", 216, y, 2, args);

		/* create the offset widget for this da converter */
		PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, da[i].da_offset, 0);
		ApCreateWidget(daSumdbase, "daSumDAoffset", 271, y, 2, args);

		/* crement the y value for widget placement */
		y += 30;
	}

	/* close the widget data base */
	ApCloseDBase(daSumdbase);
	return( Pt_CONTINUE );
}

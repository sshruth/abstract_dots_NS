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
#include "ph_ewind.h"
#include "abimport.h"
#include "proto.h"

extern lsrDispWindow dispWind;
extern CURS_DES cursDescrip[WDI_NCURS];

int initWinOptionsDialog( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[2];
	
	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	/* get the window's scale factor */
	switch(dispWind.scaleFactor) {
	case 0:
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(ABW_scale0Btn, 1, &args[0]);
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_scale1Btn, 1, &args[0]);
		PtSetResources(ABW_scale2Btn, 1, &args[0]);
		PtSetResources(ABW_scale3Btn, 1, &args[0]);
		PtSetResources(ABW_scale4Btn, 1, &args[0]);
		PtSetResources(ABW_scale5Btn, 1, &args[0]);
		break;
	case 1:
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(ABW_scale1Btn, 1, &args[0]);
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_scale0Btn, 1, &args[0]);
		PtSetResources(ABW_scale2Btn, 1, &args[0]);
		PtSetResources(ABW_scale3Btn, 1, &args[0]);
		PtSetResources(ABW_scale4Btn, 1, &args[0]);
		PtSetResources(ABW_scale5Btn, 1, &args[0]);
		break;
	case 2:
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(ABW_scale2Btn, 1, &args[0]);
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_scale0Btn, 1, &args[0]);
		PtSetResources(ABW_scale1Btn, 1, &args[0]);
		PtSetResources(ABW_scale3Btn, 1, &args[0]);
		PtSetResources(ABW_scale4Btn, 1, &args[0]);
		PtSetResources(ABW_scale5Btn, 1, &args[0]);
		break;
	case 3:
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(ABW_scale3Btn, 1, &args[0]);
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_scale0Btn, 1, &args[0]);
		PtSetResources(ABW_scale1Btn, 1, &args[0]);
		PtSetResources(ABW_scale2Btn, 1, &args[0]);
		PtSetResources(ABW_scale4Btn, 1, &args[0]);
		PtSetResources(ABW_scale5Btn, 1, &args[0]);
		break;
	case 4:
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(ABW_scale4Btn, 1, &args[0]);
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_scale0Btn, 1, &args[0]);
		PtSetResources(ABW_scale1Btn, 1, &args[0]);
		PtSetResources(ABW_scale2Btn, 1, &args[0]);
		PtSetResources(ABW_scale3Btn, 1, &args[0]);
		PtSetResources(ABW_scale5Btn, 1, &args[0]);
		break;
	case 5:
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(ABW_scale5Btn, 1, &args[0]);
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_scale0Btn, 1, &args[0]);
		PtSetResources(ABW_scale1Btn, 1, &args[0]);
		PtSetResources(ABW_scale2Btn, 1, &args[0]);
		PtSetResources(ABW_scale3Btn, 1, &args[0]);
		PtSetResources(ABW_scale4Btn, 1, &args[0]);
		break;
	}

	/* get the window's refresh rate */
	PtSetArg(&args[0], Pt_ARG_SCROLL_POSITION, dispWind.refreshRate, 0);
	PtSetResources(ABW_winRefreshScrollBar, 1, &args[0]);

	/* get the window's refresh triger */
	PtSetArg(&args[0], Pt_ARG_ONOFF_STATE, dispWind.trigger, 0);
	switch(dispWind.trigger) {
	case 0:
		PtSetArg(&args[1], Pt_ARG_TEXT_STRING, "Continuous", 0);
		break;
	case 1:
		PtSetArg(&args[1], Pt_ARG_TEXT_STRING, "Triggered", 0);
		break;
	}
	PtSetResources(ABW_winRefreshTrigButton, 2, args);

	/* get the window's trigger level */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, dispWind.triggerLevel, 0);
	PtSetResources(ABW_winRefreshLevel, 1, &args[0]);

	/* get the window's auto value */
	PtSetArg(&args[0], Pt_ARG_ONOFF_STATE, dispWind.autoRefresh, 0);
	switch(dispWind.autoRefresh) {
	case 0:
		PtSetArg(&args[1], Pt_ARG_TEXT_STRING, "Manual", 0);
		break;
	case 1:
		PtSetArg(&args[1], Pt_ARG_TEXT_STRING, "Auto", 0);
		break;
	}
	PtSetResources(ABW_autoRefreshButton, 2, args);

	return( Pt_CONTINUE );
}

int windowScale( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	int scale;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	scale = 0;
	
	if(ApName(widget) == ABN_scale0Btn) {
		scale = 0;
	}
	else if(ApName(widget) == ABN_scale1Btn) {
		scale = 1;
	}
	else if(ApName(widget) == ABN_scale2Btn) {
		scale = 2;
	}
	else if(ApName(widget) == ABN_scale3Btn) {
		scale = 3;
	}
	else if(ApName(widget) == ABN_scale4Btn) {
		scale = 4;
	}
	else if(ApName(widget) == ABN_scale5Btn) {
		scale = 5;
	}

	calcEyeScale(scale);

	writeRoot();

	return( Pt_CONTINUE );
}

int refreshRate( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;
	int rate;
	
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get rate from scroll bar */
	PtSetArg(&arg, Pt_ARG_SCROLL_POSITION, 0, 0);
	PtGetResources(widget, 1, &arg);
	rate = (int)arg.value;

	/* set the eye window's refresh rate */
	dispWind.refreshRate = rate;

	setOSContainer();

	writeRoot();

	return( Pt_CONTINUE );
}

int refreshTrig( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;
	int *value;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the value of the widget */
	PtSetArg(&arg, Pt_ARG_ONOFF_STATE, &value, 0);
	PtGetResources(widget, 1, &arg);

	/* if the button state is ON */
	if(*value) {
		PtSetArg(&arg, Pt_ARG_TEXT_STRING, "Triggered", 0);
		PtSetResources(widget, 1, &arg);
	}
	else {
		PtSetArg(&arg, Pt_ARG_TEXT_STRING, "Continuous", 0);
		PtSetResources(widget, 1, &arg);
	}

	dispWind.trigger = *value;

	setOSContainer();

	writeRoot();

	return( Pt_CONTINUE );
}

int refreshTrigLevel( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;
	int *value;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the value of the widget */
	PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, &value, 0);
	PtGetResources(widget, 1, &arg);

	dispWind.triggerLevel = *value;

	writeRoot();

	return( Pt_CONTINUE );
}

int autoRefresh( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;
	int *value;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the value of the widget */
	PtSetArg(&arg, Pt_ARG_ONOFF_STATE, &value, 0);
	PtGetResources(widget, 1, &arg);

	/* if the button state is ON */
	if(*value) {
		PtSetArg(&arg, Pt_ARG_TEXT_STRING, "Auto", 0);
		PtSetResources(widget, 1, &arg);
	}
	else {
		PtSetArg(&arg, Pt_ARG_TEXT_STRING, "Manual", 0);
		PtSetResources(widget, 1, &arg);
	}

	dispWind.autoRefresh = *value;

	writeRoot();

	return( Pt_CONTINUE );
}

int winRefresh( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	dispWind.refresh = 1;

	return( Pt_CONTINUE );
}

int autoInterval( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;
	int rate;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get interval from widget */
	PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, 0, 0);
	PtGetResources(ABW_autoRefreshInterval, 1, &arg);
	rate = (int)arg.value;

	/* set the eye window's auto refresh interval */
	dispWind.autoInterval = rate;

	writeRoot();

	return( Pt_CONTINUE );
}

int initDataChan( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtListCallback_t *listData;
	PtArg_t args[2];
	int nArgs;
	int chanNum;
	char title[16];

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* check to see that this is the final selection */
	if(cbinfo->reason_subtype != Pt_LIST_SELECTION_FINAL) return(Pt_CONTINUE);

	/* figure out which list item was selected */
	listData = (PtListCallback_t *)cbinfo->cbdata;
	sscanf(listData->item, "%d", &chanNum);

	/* set the channel number label */
	sprintf(title, "Channel %d", chanNum);
	nArgs = 0;
	PtSetArg(&args[nArgs++], Pt_ARG_TEXT_STRING, title, 0);
	PtSetResources(ABW_channelLabel, nArgs, args);

	/* set the user values of the other list widgets */
	nArgs = 0;
	PtSetArg(&args[nArgs++], Pt_ARG_USER_DATA, &chanNum, sizeof(int));
	PtSetResources(ABW_signalList, nArgs, args);
	return( Pt_CONTINUE );
}

int dataChan( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtListCallback_t *listData;
	PtArg_t args[2];
	int nArgs;
	int chanNum;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* check to see that this is the final selection */
	if(cbinfo->reason_subtype != Pt_LIST_SELECTION_FINAL) return(Pt_CONTINUE);

	/* get the channel number */
	nArgs = 0;
	PtSetArg(&args[nArgs++], Pt_ARG_USER_DATA, 0, 0);
	PtGetResources(widget, nArgs, args);
	chanNum = *(int *)args[0].value;

	/* figure out which type of data this is */
	listData = (PtListCallback_t *)cbinfo->cbdata;
	if(!strcmp(listData->item, "center")) {
		cursDescrip[chanNum].type = CU_PLUS;
		i_b->cursTyp.datCurs[chanNum].type = CHAN_CENTER;
	}
	else if(!strcmp(listData->item, "eye")) {
		cursDescrip[chanNum].type = CU_BOX;
		i_b->cursTyp.datCurs[chanNum].type = CHAN_EYE;
	}
	else if(!strcmp(listData->item, "o-eye")) {
		cursDescrip[chanNum].type = CU_DIAMOND;
		i_b->cursTyp.datCurs[chanNum].type = CHAN_OEYE;
	}
	else if(!strcmp(listData->item, "other")) {
		cursDescrip[chanNum].type = CU_UTRIANGLE;
		i_b->cursTyp.datCurs[chanNum].type = CHAN_OTHER;
	}
	else if(!strcmp(listData->item, "additional")) {
		cursDescrip[chanNum].type = CU_UTRIANGLE;
		i_b->cursTyp.datCurs[chanNum].type = CHAN_ADD;
	}
	else if(!strcmp(listData->item, "joy-stick")) {
		cursDescrip[chanNum].type = CU_JOY;
		i_b->cursTyp.datCurs[chanNum].type = CHAN_JOY;
	}
	else if(!strcmp(listData->item, "DA-0")) {
		cursDescrip[chanNum].type = CU_STAR;
		i_b->cursTyp.datCurs[chanNum].type = CHAN_DA0;
	}
	else if(!strcmp(listData->item, "DA-1")) {
		cursDescrip[chanNum].type = CU_STAR;
		i_b->cursTyp.datCurs[chanNum].type = CHAN_DA1;
	}
	else if(!strcmp(listData->item, "ramp-0")) {
		cursDescrip[chanNum].type = CU_X;
		i_b->cursTyp.datCurs[chanNum].type = CHAN_RAMP0;
	}
	else if(!strcmp(listData->item, "ramp-1")) {
		cursDescrip[chanNum].type = CU_X;
		i_b->cursTyp.datCurs[chanNum].type = CHAN_RAMP1;
	}
	else if(!strcmp(listData->item, "vergence")) {
		cursDescrip[chanNum].type = CU_LTRIANGLE;
		i_b->cursTyp.datCurs[chanNum].type = CHAN_VERG;
	}
	else if(!strcmp(listData->item, "cyclopian")) {
		cursDescrip[chanNum].type = CU_LTRIANGLE;
		i_b->cursTyp.datCurs[chanNum].type = CHAN_CYCLOP;
	}
	else if(!strcmp(listData->item, "eye-in-head")) {
		cursDescrip[chanNum].type = CU_RTRIANGLE;
		i_b->cursTyp.datCurs[chanNum].type = CHAN_GAZE;
	}
	else if(!strcmp(listData->item, "o-eye-in-head")) {
		cursDescrip[chanNum].type = CU_RTRIANGLE;
		i_b->cursTyp.datCurs[chanNum].type = CHAN_OGAZE;
	}
	else if(!strcmp(listData->item, "verg-e-in-h")) {
		cursDescrip[chanNum].type = CU_DTRIANGLE;
		i_b->cursTyp.datCurs[chanNum].type = CHAN_VGAZE;
	}
	else if(!strcmp(listData->item, "cyclop-e-in-h")) {
		cursDescrip[chanNum].type = CU_DTRIANGLE;
		i_b->cursTyp.datCurs[chanNum].type = CHAN_CGAZE;
	}

	return( Pt_CONTINUE );
}

int windowGrid( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	long *flags;
	
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	PtGetResource(widget, Pt_ARG_FLAGS, &flags, 0);
	
	if(*flags & Pt_SET) {
		PtRealizeWidget(ABW_eyeWinGrid);
		dispWind.grid = 1;
	}
	else {
		PtUnrealizeWidget(ABW_eyeWinGrid);
		dispWind.grid = 0;
	}

	writeRoot();

	return( Pt_CONTINUE );
}

int cursorChoice( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	int i;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(cbinfo->reason == Pt_CB_ARM) {
		if(ApName(widget) == ABN_curs0Btn) cursDescrip[0].on = 1;
		else if(ApName(widget) == ABN_curs1Btn) cursDescrip[1].on = 1;
		else if(ApName(widget) == ABN_curs2Btn) cursDescrip[2].on = 1;
		else if(ApName(widget) == ABN_curs3Btn) cursDescrip[3].on = 1;
		else if(ApName(widget) == ABN_curs4Btn) cursDescrip[4].on = 1;
		else if(ApName(widget) == ABN_curs5Btn) cursDescrip[5].on = 1;
		else if(ApName(widget) == ABN_curs6Btn) cursDescrip[6].on = 1;
		else if(ApName(widget) == ABN_curs7Btn) cursDescrip[7].on = 1;
		else if(ApName(widget) == ABN_wnd0Btn) cursDescrip[8].on = 1;
		else if(ApName(widget) == ABN_wnd1Btn) cursDescrip[9].on = 1;
		else if(ApName(widget) == ABN_wnd2Btn) cursDescrip[10].on = 1;
		else if(ApName(widget) == ABN_wnd3Btn) cursDescrip[11].on = 1;
		else if(ApName(widget) == ABN_wnd4Btn) cursDescrip[12].on = 1;
		else if(ApName(widget) == ABN_wnd5Btn) cursDescrip[13].on = 1;
		else if(ApName(widget) == ABN_wnd6Btn) cursDescrip[14].on = 1;
		else if(ApName(widget) == ABN_wnd7Btn) cursDescrip[15].on = 1;
	}
	else if(cbinfo->reason == Pt_CB_DISARM) {
		if(ApName(widget) == ABN_curs0Btn) cursDescrip[0].on = 0;
		else if(ApName(widget) == ABN_curs1Btn) cursDescrip[1].on = 0;
		else if(ApName(widget) == ABN_curs2Btn) cursDescrip[2].on = 0;
		else if(ApName(widget) == ABN_curs3Btn) cursDescrip[3].on = 0;
		else if(ApName(widget) == ABN_curs4Btn) cursDescrip[4].on = 0;
		else if(ApName(widget) == ABN_curs5Btn) cursDescrip[5].on = 0;
		else if(ApName(widget) == ABN_curs6Btn) cursDescrip[6].on = 0;
		else if(ApName(widget) == ABN_curs7Btn) cursDescrip[7].on = 0;
		else if(ApName(widget) == ABN_wnd0Btn) cursDescrip[8].on = 0;
		else if(ApName(widget) == ABN_wnd1Btn) cursDescrip[9].on = 0;
		else if(ApName(widget) == ABN_wnd2Btn) cursDescrip[10].on = 0;
		else if(ApName(widget) == ABN_wnd3Btn) cursDescrip[11].on = 0;
		else if(ApName(widget) == ABN_wnd4Btn) cursDescrip[12].on = 0;
		else if(ApName(widget) == ABN_wnd5Btn) cursDescrip[13].on = 0;
		else if(ApName(widget) == ABN_wnd6Btn) cursDescrip[14].on = 0;
		else if(ApName(widget) == ABN_wnd7Btn) cursDescrip[15].on = 0;
	}

	dispWind.numCurs = WDI_NCURS;
	for(i = 0; i < dispWind.numCurs; i++) {
		dispWind.cursors[i].on = cursDescrip[i].on;
		dispWind.cursors[i].type = cursDescrip[i].type;
	}
	
	return( Pt_CONTINUE );
}

void setOSContainer()
{
	extern lsrDispWindow dispWind;

	// make the raw drawing widget and the grid a child of an OSContainer
	if((dispWind.refreshRate == 0) && (dispWind.trigger == 0)) {
		PtRealizeWidget(ABW_eyeWndOSContainer);
		PtReparentWidget(ABW_displayWindow, ABW_eyeWndOSContainer);
		PtReparentWidget(ABW_eyeWinGrid, ABW_eyeWndOSContainer);
	}

	// make the raw drawing widget and the grid a child of the pane
	else if((dispWind.refreshRate > 0) || (dispWind.trigger != 0)) {
		PtReparentWidget(ABW_displayWindow, ABW_eyeWndPane);
		PtReparentWidget(ABW_eyeWinGrid, ABW_eyeWndPane);
		PtUnrealizeWidget(ABW_eyeWndOSContainer);
	}
	return;
}

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

int maMenuOvrRides[16][4] = {
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 }
};
int *maAddress[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int maNpts[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int maMsecs[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int maRepeats[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int memArrayDialogEnabled = 0;

int initMemArrayDialog( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[2];
	PtListCallback_t *listData;
	int maNum;
	char title[16];
	
	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	/* if this function was called from the list widget */
	if(PtWidgetClass(link_instance) == PtList) {
		if(cbinfo->reason_subtype != Pt_LIST_SELECTION_FINAL) return(Pt_CONTINUE);

		listData = (PtListCallback_t *)cbinfo->cbdata;
		sscanf(listData->item, "Mem Array %d", &maNum);
	}
	else {
		maNum = 0;
		PtListSelectPos(ABW_memArrayList, 1);
	}

	/*set the module's title widget */
	sprintf(title, "Memory Array %d", maNum);
	PtSetArg(&args[0], Pt_ARG_TEXT_STRING, title, 0);
	PtSetResources(ABW_memArrayDialogLabel, 1, &args[0]);

	if(ma[maNum].m_ma_menu) {
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, "Dialog Enabled", 0);
	}
	else {
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, "Dialog Disabled", 0);
	}
	PtSetArg(&args[1], Pt_ARG_ONOFF_STATE, ma[maNum].m_ma_menu, 0);
	PtSetResources(ABW_memArrayMenuButton, 2, args);

	/* set the numeric values of the override widgets */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, maAddress[maNum], 0);
	PtSetResources(ABW_memArrayAddress, 1, &args[0]);

	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, maNpts[maNum], 0);
	PtSetResources(ABW_memArrayCount, 1, &args[0]);

	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, maMsecs[maNum], 0);
	PtSetResources(ABW_memArrayMsec, 1, &args[0]);

	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, maRepeats[maNum], 0);
	PtSetResources(ABW_memArrayRepeat, 1, &args[0]);

	/* set the states of the menu override buttons */
	if(maMenuOvrRides[maNum][0]) PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
	else PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
	PtSetResources(ABW_maDialogAddrOvrRide, 1, &args[0]);

	if(maMenuOvrRides[maNum][1]) PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
	else PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
	PtSetResources(ABW_maDialogPtsOvrRide, 1, &args[0]);

	if(maMenuOvrRides[maNum][2]) PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
	else PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
	PtSetResources(ABW_maDialogMsecOvrRide, 1, &args[0]);

	if(maMenuOvrRides[maNum][3]) PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
	else PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
	PtSetResources(ABW_maDialogRepeatOvrRide, 1, &args[0]);

	/* set the values of the module's "actual" widgets */
	/* beginning address */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, ma[maNum].ma_bap, 0);
	PtSetResources(ABW_memArrayAddressAct, 1, &args[0]);

	/* number of points */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, ma[maNum].ma_count, 0);
	PtSetResources(ABW_memArrayCountAct, 1, &args[0]);

	/* msec per point */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, ma[maNum].ma_rate, 0);
	PtSetResources(ABW_memArrayMsecAct, 1, &args[0]);

	/* repeat count */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, ma[maNum].ma_repeat, 0);
	PtSetResources(ABW_memArrayRepeatAct, 1, &args[0]);

	/* set the ma number in all of this module's widgets */
	PtSetArg(&args[0], Pt_ARG_USER_DATA, &maNum, sizeof(int));
	PtSetResources(ABW_memArrayMenuButton, 1, &args[0]);
	PtSetResources(ABW_maDialogAddrOvrRide, 1, &args[0]);
	PtSetResources(ABW_maDialogPtsOvrRide, 1, &args[0]);
	PtSetResources(ABW_maDialogMsecOvrRide, 1, &args[0]);
	PtSetResources(ABW_maDialogRepeatOvrRide, 1, &args[0]);
	PtSetResources(ABW_memArrayAddress, 1, &args[0]);
	PtSetResources(ABW_memArrayCount, 1, &args[0]);
	PtSetResources(ABW_memArrayMsec, 1, &args[0]);
	PtSetResources(ABW_memArrayRepeat, 1, &args[0]);

	maDialogItemEnable(maNum);

	return( Pt_CONTINUE );
}

int memArrayDialogSet( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[2];
	int *pMaNum;
	int *value;
	int maNum;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the status of the on-off switch */
	PtSetArg(&args[0], Pt_ARG_ONOFF_STATE, &value, 0);
	PtSetArg(&args[1], Pt_ARG_USER_DATA, &pMaNum, 0);
	PtGetResources(widget, 2, args);
	maNum = *pMaNum;

	if(*value) {
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, "Dialog Enabled", 0);
		ma[maNum].m_ma_menu = 1;
	}
	else {
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, "Dialog Disabled", 0);
		ma[maNum].m_ma_menu = 0;
	}
	PtSetResources(widget, 1, args);

	maDialogItemEnable(maNum);

	ma_cntrl(maNum, (int *)NULLI, NULLI, NULLI, NULLI);

	return( Pt_CONTINUE );
}

void maDialogItemEnable(int maNum)
{
	PtArg_t args[2];

	/* enable dialog items if menu is enabled */
	if(ma[maNum].m_ma_menu) {
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_GHOST);
		PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_TRUE, Pt_SELECTABLE);
	}
	else {
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_GHOST);
		PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_FALSE, Pt_SELECTABLE);
	}
	PtSetResources(ABW_maDialogAddrOvrRide, 2, args);
	PtSetResources(ABW_maDialogPtsOvrRide, 2, args);
	PtSetResources(ABW_maDialogMsecOvrRide, 2, args);
	PtSetResources(ABW_maDialogRepeatOvrRide, 2, args);

	return;
}

int memArrayMenuOverRide( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;
	int maNum;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the da number from the widget */
	PtSetArg(&arg, Pt_ARG_USER_DATA, 0, 0);
	PtGetResources(widget, 1, &arg);
	maNum = *(int *)arg.value;

	/* determin which of the override buttons initiated this callback */
	if(ApName(widget) == ABN_maDialogAddrOvrRide) {
		if(cbinfo->reason == Pt_CB_ARM) {
			maMenuOvrRides[maNum][0] = 1;
			ma[maNum].m_ma_bap = maAddress[maNum];
		}
		else if(cbinfo->reason == Pt_CB_DISARM) {
			maMenuOvrRides[maNum][0] = 0;
			ma[maNum].m_ma_bap = (int *)NULLI;
		}
	}

	else if(ApName(widget) == ABN_maDialogPtsOvrRide) {
		if(cbinfo->reason == Pt_CB_ARM) {
			maMenuOvrRides[maNum][1] = 1;
			ma[maNum].m_ma_count = maNpts[maNum];
		}
		else if(cbinfo->reason == Pt_CB_DISARM) {
			maMenuOvrRides[maNum][1] = 0;
			ma[maNum].m_ma_count = NULLI;
		}
	}

	else if(ApName(widget) == ABN_maDialogMsecOvrRide) {
		if(cbinfo->reason == Pt_CB_ARM) {
			maMenuOvrRides[maNum][2] = 1;
			ma[maNum].m_ma_rate = maMsecs[maNum];
		}
		else if(cbinfo->reason == Pt_CB_DISARM) {
			maMenuOvrRides[maNum][2] = 0;
			ma[maNum].m_ma_rate = NULLI;
		}
	}

	else if(ApName(widget) == ABN_maDialogRepeatOvrRide) {
		if(cbinfo->reason == Pt_CB_ARM) {
			maMenuOvrRides[maNum][3] = 1;
			ma[maNum].m_ma_repeat = maRepeats[maNum];
		}
		else if(cbinfo->reason == Pt_CB_DISARM) {
			maMenuOvrRides[maNum][3] = 0;
			ma[maNum].m_ma_repeat = NULLI;
		}
	}

	ma_cntrl(maNum, (int *)NULLI, NULLI, NULLI, NULLI);

	return( Pt_CONTINUE );
}

int memArrayParameters( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[2];
	int *lngVal;
	int maNum;
	int value;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the ma number and value from the widget */
	PtSetArg(&args[0], Pt_ARG_USER_DATA, 0, 0);
	PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, 0, 0);
	PtGetResources(widget, 2, args);
	maNum = *(int *)args[0].value;

	/* figure out which widget initiated this callback */
	if(ApName(widget) == ABN_memArrayAddress) {
		lngVal = (int *)args[1].value;
		maAddress[maNum] = lngVal;
		if(maMenuOvrRides[maNum][0]) {
			ma[maNum].m_ma_bap = maAddress[maNum];
			ma_cntrl(maNum, (int *)NULLI, NULLI, NULLI, NULLI);
		}
	}
	else if(ApName(widget) == ABN_memArrayCount) {
		value = (int) args[1].value;
		maNpts[maNum] = value;
		if(maMenuOvrRides[maNum][1]) {
			ma[maNum].m_ma_count = maNpts[maNum];
			ma_cntrl(maNum, (int *)NULLI, NULLI, NULLI, NULLI);
		}
	}
	else if(ApName(widget) == ABN_memArrayMsec) {
		value = (int) args[1].value;
		maMsecs[maNum] = value;
		if(maMenuOvrRides[maNum][2]) {
			ma[maNum].m_ma_rate = maMsecs[maNum];
			ma_cntrl(maNum, (int *)NULLI, NULLI, NULLI, NULLI);
		}
	}
	else if(ApName(widget) == ABN_memArrayRepeat) {
		value = (int) args[1].value;
		maRepeats[maNum] = value;
		if(maMenuOvrRides[maNum][2]) {
			ma[maNum].m_ma_repeat = maRepeats[maNum];
			ma_cntrl(maNum, (int *)NULLI, NULLI, NULLI, NULLI);
		}
	}
	return( Pt_CONTINUE );
}

int initMemArraySummary( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	ApDBase_t *memArraydbase;
	PtArg_t args[2];
	int i;
	int y;
	int memNum;
	char label[4];

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	/* open the memory array summary data base */
	memArraydbase = ApOpenDBase(ABM_memArraySumPictures);

	/* set the parent for the widgets */
	PtSetParentWidget(ABW_memArrayScrollArea);

	/* create 16 sets of widgets for the memory array summary */
	y = 0;
	for(i = 0; i < MA_MAXNUM; i++) {
		memNum = i;

		/* create the label widget for this element */
		sprintf(label, "%d", i);
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, label, 0);
		ApCreateWidget(memArraydbase, "memArraySumLabel", 10, y, 2, args);

		/* create the pointer widget for this element */
		PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, ma[i].ma_bap, 0);
		ApCreateWidget(memArraydbase, "memArraySumAddress", 48, y, 2, args);

		/* create the size widget for this element */
		PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, ma[i].ma_count, 0);
		ApCreateWidget(memArraydbase, "memArraySumSize", 119, y, 2, args);

		/* create the rate widget for this element */
		PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, ma[i].ma_rate, 0);
		ApCreateWidget(memArraydbase, "memArraySumRate", 176, y, 2, args);

		/* create the repeat widget for this element */
		PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, ma[i].ma_repeat, 0);
		ApCreateWidget(memArraydbase, "memArraySumRepeat", 233, y, 2, args);

		y += 30;
	}

	/* close the widget data base */
	ApCloseDBase(memArraydbase);

	return( Pt_CONTINUE );
}

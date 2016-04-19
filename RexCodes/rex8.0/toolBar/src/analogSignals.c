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

extern int sigTypeToggles[3];
extern int adVarToggles[10];
extern int memSrcToggles[12];
extern int computedToggles[12];
extern int aqrRtToggles[4];
extern int strRtToggles[4];
extern int adCalToggles[7];

int initSignalDialog( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtWidget_t *dialog;
	PtWidget_t *toggle;
	PtArg_t arg;
	PtListCallback_t *listData;
	int sigNum;
	int i, j;
	char title[16];
	
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* if this function was called from the list widget */
	if(PtWidgetClass(widget) == PtList) {
		if(cbinfo->reason_subtype != Pt_LIST_SELECTION_FINAL) return(Pt_CONTINUE);
		dialog = ApGetInstance(widget);
		listData = (PtListCallback_t *)cbinfo->cbdata;
		sscanf(listData->item, "Signal %d", &sigNum);
	}
	else {
		dialog = widget;
		sigNum = 0;
		PtListSelectPos(ABW_signalList, 1);

		sig_compute_ad_rates();

	}

	/* set the module's title widget */
	sprintf(title, "Signal %d", sigNum);
	PtSetArg(&arg, Pt_ARG_TEXT_STRING, title, 0);
	PtSetResources(ABW_signalDialogLabel, 1, &arg);

	/* set the signal number in all of this module's widgets */
	PtSetArg(&arg, Pt_ARG_USER_DATA, &sigNum, sizeof(int));
	PtSetResources(ABW_asADchannel, 1, &arg);
	PtSetResources(ABW_asADdelay, 1, &arg);

	PtSetResources(ABW_asSigOffToggle, 1, &arg);
	PtSetResources(ABW_asADToggle, 1, &arg);
	PtSetResources(ABW_asMemoryToggle, 1, &arg);
	PtSetResources(ABW_asComputeToggle, 1, &arg);

	PtSetResources(ABW_asTitleTxt, 1, &arg);

	PtSetResources(ABW_asEyehToggle, 1, &arg);
	PtSetResources(ABW_asEyevToggle, 1, &arg);
	PtSetResources(ABW_asOeyehToggle, 1, &arg);
	PtSetResources(ABW_asOeyevToggle, 1, &arg);
	PtSetResources(ABW_asOtherhToggle, 1, &arg);
	PtSetResources(ABW_asOthervToggle, 1, &arg);
	PtSetResources(ABW_asAddhToggle, 1, &arg);
	PtSetResources(ABW_asAddvToggle, 1, &arg);
	PtSetResources(ABW_asJoyhToggle, 1, &arg);
	PtSetResources(ABW_asJoyvToggle, 1, &arg);

	PtSetResources(ABW_asDA0Toggle, 1, &arg);
	PtSetResources(ABW_asDA1Toggle, 1, &arg);
	PtSetResources(ABW_asDA2Toggle, 1, &arg);
	PtSetResources(ABW_asDA3Toggle, 1, &arg);
	PtSetResources(ABW_asRamp0XToggle, 1, &arg);
	PtSetResources(ABW_asRamp0YToggle, 1, &arg);
	PtSetResources(ABW_asRamp1XToggle, 1, &arg);
	PtSetResources(ABW_asRamp1YToggle, 1, &arg);
	PtSetResources(ABW_asWD0XToggle, 1, &arg);
	PtSetResources(ABW_asWD0YToggle, 1, &arg);
	PtSetResources(ABW_asWD1XToggle, 1, &arg);
	PtSetResources(ABW_asWD1YToggle, 1, &arg);

	PtSetResources(ABW_asVerghToggle, 1, &arg);
	PtSetResources(ABW_asVergvToggle, 1, &arg);
	PtSetResources(ABW_asCyclophToggle, 1, &arg);
	PtSetResources(ABW_asCyclopvToggle, 1, &arg);
	PtSetResources(ABW_asGazehToggle, 1, &arg);
	PtSetResources(ABW_asGazevToggle, 1, &arg);
	PtSetResources(ABW_asOgazehToggle, 1, &arg);
	PtSetResources(ABW_asOgazevToggle, 1, &arg);
	PtSetResources(ABW_asVgazehToggle, 1, &arg);
	PtSetResources(ABW_asVgazevToggle, 1, &arg);
	PtSetResources(ABW_asCgazehToggle, 1, &arg);
	PtSetResources(ABW_asCgazevToggle, 1, &arg);

	PtSetResources(ABW_asAquire2000Toggle, 1, &arg);
	PtSetResources(ABW_asAquire1000Toggle, 1, &arg);
	PtSetResources(ABW_asAquire500Toggle, 1, &arg);
	PtSetResources(ABW_asAquire250Toggle, 1, &arg);

	PtSetResources(ABW_asStore2000Toggle, 1, &arg);
	PtSetResources(ABW_asStore1000Toggle, 1, &arg);
	PtSetResources(ABW_asStore500Toggle, 1, &arg);
	PtSetResources(ABW_asStore250Toggle, 1, &arg);

	PtSetResources(ABW_asCal204Toggle, 1, &arg);
	PtSetResources(ABW_asCal102Toggle, 1, &arg);
	PtSetResources(ABW_asCal51Toggle, 1, &arg);
	PtSetResources(ABW_asCal25Toggle, 1, &arg);
	PtSetResources(ABW_asCal12Toggle, 1, &arg);
	PtSetResources(ABW_asCal6Toggle, 1, &arg);
	PtSetResources(ABW_asCalNoiseToggle, 1, &arg);

	/* set the values of the widgets in this module */
	/* set the signal type toggles */
	for(i = 0; i < 4; i++) {
		toggle = ApGetWidgetPtr(dialog, sigTypeToggles[i]);
		if(sig[sigNum].sig_enable == i) PtSetArg(&arg, Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		else PtSetArg(&arg, Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(toggle, 1, &arg);
	}

	/* set the signal title */
	if(strlen(sig[sigNum].sig_title)) PtSetArg(&arg, Pt_ARG_TEXT_STRING, sig[sigNum].sig_title, 0);
	else PtSetArg(&arg, Pt_ARG_TEXT_STRING, "", 0);
	PtSetResources(ABW_asTitleTxt, 1, &arg);

	/* set the a/d channel number */
	if(sig[sigNum].sig_ad_chan == NULLI) PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, -1, 0);
	else PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, sig[sigNum].sig_ad_chan, 0);
	PtSetResources(ABW_asADchannel, 1, &arg);

	/* set the a/d delay value */
	if(sig[sigNum].sig_ad_delay == NULLI) PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, -1, 0);
	else PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, sig[sigNum].sig_ad_delay, 0);
	PtSetResources(ABW_asADdelay, 1, &arg);

	/* set the a/d variable toggles */
	toggle = 0;

	if(!toggle) {
		for(i = 0; i < 10; i++) {
			if(!strcmp(sig[sigNum].sig_ad_mem_var, gv_ad_mem[i].gv_name)) {
				toggle = ApGetWidgetPtr(dialog, adVarToggles[i]);
				PtSetArg(&arg, Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
				PtSetResources(toggle, 1, &arg);
				break;
			}
		}
	}

	/* set the memory source toggles */
	if(!toggle) {
		for(i = 0; i < 12; i++) {
			if(!strcmp(sig[sigNum].sig_mem_source, gv_mem_src[i].gv_name)) {
				toggle = ApGetWidgetPtr(dialog, memSrcToggles[i]);
				PtSetArg(&arg, Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
				PtSetResources(toggle, 1, &arg);
				break;
			}
		}
	}

	/* set the computed variable toggles */
	if(!toggle) {
		for(j = 0; j < 12; j++) {
			if(!strcmp(sig[sigNum].sig_mem_source, gv_mem_src[j + 12].gv_name)) {
				toggle = ApGetWidgetPtr(dialog, computedToggles[j]);
				PtSetArg(&arg, Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
				PtSetResources(toggle, 1, &arg);
				break;
			}
		}
	}

	/* set the a/d acquire rate toggles */
	switch(sig[sigNum].sig_ad_rate) {
	case 2000:
		PtSetArg(&arg, Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(ABW_asAquire2000Toggle, 1, &arg);
		PtSetArg(&arg, Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_asAquire1000Toggle, 1, &arg);
		PtSetResources(ABW_asAquire500Toggle, 1, &arg);
		PtSetResources(ABW_asAquire250Toggle, 1, &arg);
		break;
	case 1000:
		PtSetArg(&arg, Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(ABW_asAquire1000Toggle, 1, &arg);
		PtSetArg(&arg, Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_asAquire2000Toggle, 1, &arg);
		PtSetResources(ABW_asAquire500Toggle, 1, &arg);
		PtSetResources(ABW_asAquire250Toggle, 1, &arg);
		break;
	case 500:
		PtSetArg(&arg, Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(ABW_asAquire500Toggle, 1, &arg);
		PtSetArg(&arg, Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_asAquire2000Toggle, 1, &arg);
		PtSetResources(ABW_asAquire1000Toggle, 1, &arg);
		PtSetResources(ABW_asAquire250Toggle, 1, &arg);
		break;
	case 250:
		PtSetArg(&arg, Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(ABW_asAquire250Toggle, 1, &arg);
		PtSetArg(&arg, Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_asAquire2000Toggle, 1, &arg);
		PtSetResources(ABW_asAquire1000Toggle, 1, &arg);
		PtSetResources(ABW_asAquire500Toggle, 1, &arg);
		break;
	default:
		PtSetArg(&arg, Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_asAquire2000Toggle, 1, &arg);
		PtSetResources(ABW_asAquire1000Toggle, 1, &arg);
		PtSetResources(ABW_asAquire500Toggle, 1, &arg);
		PtSetResources(ABW_asAquire250Toggle, 1, &arg);
		break;
	}

	/* set the a/d store rate toggles */
	switch(sig[sigNum].sig_store_rate) {
	case 2000:
		PtSetArg(&arg, Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(ABW_asStore2000Toggle, 1, &arg);
		PtSetArg(&arg, Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_asStore1000Toggle, 1, &arg);
		PtSetResources(ABW_asStore500Toggle, 1, &arg);
		PtSetResources(ABW_asStore250Toggle, 1, &arg);
		break;
	case 1000:
		PtSetArg(&arg, Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(ABW_asStore1000Toggle, 1, &arg);
		PtSetArg(&arg, Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_asStore2000Toggle, 1, &arg);
		PtSetResources(ABW_asStore500Toggle, 1, &arg);
		PtSetResources(ABW_asStore250Toggle, 1, &arg);
		break;
	case 500:
		PtSetArg(&arg, Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(ABW_asStore500Toggle, 1, &arg);
		PtSetArg(&arg, Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_asStore2000Toggle, 1, &arg);
		PtSetResources(ABW_asStore1000Toggle, 1, &arg);
		PtSetResources(ABW_asStore250Toggle, 1, &arg);
		break;
	case 250:
		PtSetArg(&arg, Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(ABW_asStore250Toggle, 1, &arg);
		PtSetArg(&arg, Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_asStore2000Toggle, 1, &arg);
		PtSetResources(ABW_asStore1000Toggle, 1, &arg);
		PtSetResources(ABW_asStore500Toggle, 1, &arg);
		break;
	default:
		PtSetArg(&arg, Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_asStore2000Toggle, 1, &arg);
		PtSetResources(ABW_asStore1000Toggle, 1, &arg);
		PtSetResources(ABW_asStore500Toggle, 1, &arg);
		PtSetResources(ABW_asStore250Toggle, 1, &arg);
		break;
	}

	/* set the a/d calibration toggles */
	for(i = 0; i < 7; i++) {
		toggle = ApGetWidgetPtr(dialog, adCalToggles[i]);
		if(sig[sigNum].sig_ad_calib == i) PtSetArg(&arg, Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		else PtSetArg(&arg, Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(toggle, 1, &arg);
	}

	/* set the state of the signal source widgets */
	setSigSourceStates(sig[sigNum].sig_enable);
	setAcquireRateStates(sig[sigNum].sig_enable);
	setStoreRateStates(sig[sigNum].sig_enable);

	return(Pt_CONTINUE);
}

int setSigType( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[2];
	int sigNum;
	int sigType;
	int i;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(i_b->i_flags & I_GO) {
		PtAskQuestion(ABW_signalDialog, NULL,
					  "Can't set signal type\nwhile clock is running", "helv12b",
					  "&Continue", NULL, NULL, 1);
		return( Pt_CONTINUE );
	}

	/* get the signal number from the widget */
	PtSetArg(&args[0], Pt_ARG_USER_DATA, 0, 0);
	PtGetResources(widget, 1, &args[0]);
	sigNum = *(int *)args[0].value;

	/* set the global signal type variable */
	for(i = 0; i < 4; i++) {
		if(ApName(widget) == sigTypeToggles[i]) {
			sigType= i;
			sig[sigNum].sig_enable = i;
			break;
		}
	}

	switch(sigType) {
	case SIG_OFF:
		strcpy(sig[sigNum].sig_ad_mem_var, "-");
		strcpy(sig[sigNum].sig_mem_source, "-");
		strcpy(sig[sigNum].sig_title, "-");
		sig[sigNum].sig_ad_chan = NULLI;
		sig[sigNum].sig_ad_rate = NULLI;
		sig[sigNum].sig_ad_calib = NULLI;
		sig[sigNum].sig_ad_gain = NULLI;
		sig[sigNum].sig_ad_delay = NULLI;
		sig[sigNum].sig_store_rate = NULLI;
		break;
	case SIG_AD_SOURCE:
		strcpy(sig[sigNum].sig_mem_source, "-");
		sig[sigNum].sig_ad_delay = 23;
		PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, 23, 0);
		PtSetResources(ABW_asADdelay, 1, args);
		break;
	case SIG_MEM_SOURCE:
	case SIG_COMP_SOURCE:
		strcpy(sig[sigNum].sig_ad_mem_var, "-");
		sig[sigNum].sig_ad_chan = NULLI;
		sig[sigNum].sig_ad_rate = NULLI;
		sig[sigNum].sig_ad_calib = NULLI;
		sig[sigNum].sig_ad_gain = NULLI;
		sig[sigNum].sig_ad_delay = NULLI;
		break;
	}

	setSigSourceStates(sigType);
	setAcquireRateStates(sigType);
	setStoreRateStates(sigType);

	return( Pt_CONTINUE );
}

int setSigName( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtWidget_t *toggle;
	PtArg_t arg;
	int sigNum;
	int i, j;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(i_b->i_flags & I_GO) {
		PtAskQuestion(ABW_signalDialog, NULL,
					  "Can't change signal name\nwhile clock is running", "helv12b",
					  "&Continue", NULL, NULL, 1);
		return( Pt_CONTINUE );
	}

	/* get the signal number from the widget */
	PtSetArg(&arg, Pt_ARG_USER_DATA, 0, 0);
	PtGetResources(widget, 1, &arg);
	sigNum = *(int *)arg.value;

	/* figure out which toggle button initiated this callback */
	toggle = 0;
	for(i = 0; i < 10; i++) {
		if(ApName(widget) == adVarToggles[i]) {
			toggle = widget;
			strncpy(sig[sigNum].sig_ad_mem_var, gv_ad_mem[i].gv_name, P_LNAME);
			break;
		}
	}
	if(toggle) return( Pt_CONTINUE );

	for(i = 0; i < 12; i++) {
		if(ApName(widget) == memSrcToggles[i]) {
			toggle = widget;
			strncpy(sig[sigNum].sig_mem_source, gv_mem_src[i].gv_name, P_LNAME);
			break;
		}
	}
	if(toggle) return( Pt_CONTINUE );

	for(j = 0; j < 12; j++,i++) {
		if(ApName(widget) == computedToggles[j]) {
			toggle = widget;
			strncpy(sig[sigNum].sig_mem_source, gv_mem_src[i].gv_name, P_LNAME);
			break;
		}
	}
	if(!toggle) return( Pt_CONTINUE );

	/* at this point the signal selected must be a computed signal
	 * so figure out the store rate from the store rates of its sources
	 */
	setComputedStoreRate(sigNum);

	return( Pt_CONTINUE );
}

int setSigTitle( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[2];
	int sigNum;
	char *title;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(i_b->i_flags & I_GO) {
		PtAskQuestion(ABW_signalDialog, NULL,
					  "Can't change signal title\nwhile clock is running", "helv12b",
					  "&Continue", NULL, NULL, 1);
		return( Pt_CONTINUE );
	}

	PtSetArg(&args[0], Pt_ARG_USER_DATA, 0, 0);
	PtSetArg(&args[1], Pt_ARG_TEXT_STRING, 0, 0);
	PtGetResources(widget, 2, args);
	sigNum = *(int *)args[0].value;
	title = (char *)args[1].value;

	if(strlen(title)) {
		strncpy(sig[sigNum].sig_title, title, P_LNAME);
	}
	else {
		strcpy(sig[sigNum].sig_title, "");
	}

	return( Pt_CONTINUE );
}

int setADchannel( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;
	int sigNum;
	int value;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(i_b->i_flags & I_GO) {
		PtAskQuestion(ABW_signalDialog, NULL,
					  "Can't set AD channel\nwhile clock is running", "helv12b",
					  "&Continue", NULL, NULL, 1);
		return( Pt_CONTINUE );
	}

	/* get the signal number from the widget */
	PtSetArg(&arg, Pt_ARG_USER_DATA, 0, 0);
	PtGetResources(widget, 1, &arg);
	sigNum = *(int *)arg.value;

	/* get the value from the widget */
	PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, 0, 0);
	PtGetResources(widget, 1, &arg);
	value = (int)arg.value;
	
	if(value == -1) sig[sigNum].sig_ad_chan = NULLI;
	else sig[sigNum].sig_ad_chan = value;

	return( Pt_CONTINUE );
}

int setADdelay( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;
	int sigNum;
	int value;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(i_b->i_flags & I_GO) {
		PtAskQuestion(ABW_signalDialog, NULL,
					  "Can't set AD delay\nwhile clock is running", "helv12b",
					  "&Continue", NULL, NULL, 1);
		return( Pt_CONTINUE );
	}

	/* get the signal number from the widget */
	PtSetArg(&arg, Pt_ARG_USER_DATA, 0, 0);
	PtGetResources(widget, 1, &arg);
	sigNum = *(int *)arg.value;

	/* get the value from the widget */
	PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, 0, 0);
	PtGetResources(widget, 1, &arg);
	value = (int)arg.value;

	if(value == -1) sig[sigNum].sig_ad_delay = NULLI;
	else sig[sigNum].sig_ad_delay = value;

	return( Pt_CONTINUE );
}

int setADrates( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtWidget_t *toggle;
	PtArg_t arg;
	int sigNum;
	int i;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(i_b->i_flags & I_GO) {
		PtAskQuestion(ABW_signalDialog, NULL,
					  "Can't change A/D rates\nwhile clock is running", "helv12b",
					  "&Continue", NULL, NULL, 1);
		return( Pt_CONTINUE );
	}

	/* get the signal number from the widget */
	PtSetArg(&arg, Pt_ARG_USER_DATA, 0, 0);
	PtGetResources(widget, 1, &arg);
	sigNum = *(int *)arg.value;

	/* figure out which toggle button initiated this callback */
	toggle = (PtWidget_t *)NULL;
	for(i = 0; i < 4; i++) {
		if(ApName(widget) == aqrRtToggles[i]) {	/* aquire rate toggle */
			toggle = widget;
			sig[sigNum].sig_ad_rate = ad_acq_rates[i].ad_acq_rate;
		}
	}
	if(toggle) return( Pt_CONTINUE );

	for(i = 0; i < 4; i++) {
		if(ApName(widget) == strRtToggles[i]) { /* store rate toggle */
			sig[sigNum].sig_store_rate = ad_acq_rates[i].ad_acq_rate;
		}
	}

	return( Pt_CONTINUE );
}

int setADcal( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;
	int sigNum;
	int i;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(i_b->i_flags & I_GO) {
		PtAskQuestion(ABW_signalDialog, NULL,
					  "Can't change A/D calibration\nwhile clock is running", "helv12b",
					  "&Continue", NULL, NULL, 1);
		return( Pt_CONTINUE );
	}

	/* get the signal number from the widget */
	PtSetArg(&arg, Pt_ARG_USER_DATA, 0, 0);
	PtGetResources(widget, 1, &arg);
	sigNum = *(int *)arg.value;

	/* figure out which toggle button initiated this callback */
	for(i = 0; i < 7; i++) {
		if(ApName(widget) == adCalToggles[i]) {
			sig[sigNum].sig_ad_calib = i;
		}
	}
	return( Pt_CONTINUE );
}

void setSigSourceStates(int sigType)
{
	PtArg_t args[3];
	int nArgs;

	/* enable the appropriate source toggle buttons */
	switch(sigType) {
	case SIG_OFF:
		nArgs = 0;
		PtSetArg(&args[nArgs++], Pt_ARG_FLAGS, Pt_TRUE, Pt_GHOST);
		PtSetArg(&args[nArgs++], Pt_ARG_FLAGS, Pt_FALSE, Pt_SELECTABLE);
		PtSetArg(&args[nArgs++], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_asEyehToggle, nArgs, args);
		PtSetResources(ABW_asEyevToggle, nArgs, args);
		PtSetResources(ABW_asOeyehToggle, nArgs, args);
		PtSetResources(ABW_asOeyevToggle, nArgs, args);
		PtSetResources(ABW_asOtherhToggle, nArgs, args);
		PtSetResources(ABW_asOthervToggle, nArgs, args);
		PtSetResources(ABW_asAddhToggle, nArgs, args);
		PtSetResources(ABW_asAddvToggle, nArgs, args);
		PtSetResources(ABW_asJoyhToggle, nArgs, args);
		PtSetResources(ABW_asJoyvToggle, nArgs, args);
		PtSetResources(ABW_asDA0Toggle, nArgs, args);
		PtSetResources(ABW_asDA1Toggle, nArgs, args);
		PtSetResources(ABW_asDA2Toggle, nArgs, args);
		PtSetResources(ABW_asDA3Toggle, nArgs, args);
		PtSetResources(ABW_asRamp0XToggle, nArgs, args);
		PtSetResources(ABW_asRamp0YToggle, nArgs, args);
		PtSetResources(ABW_asRamp1XToggle, nArgs, args);
		PtSetResources(ABW_asRamp1YToggle, nArgs, args);
		PtSetResources(ABW_asWD0XToggle, nArgs, args);
		PtSetResources(ABW_asWD0YToggle, nArgs, args);
		PtSetResources(ABW_asWD1XToggle, nArgs, args);
		PtSetResources(ABW_asWD1YToggle, nArgs, args);
		PtSetResources(ABW_asVerghToggle, nArgs, args);
		PtSetResources(ABW_asVergvToggle, nArgs, args);
		PtSetResources(ABW_asCyclophToggle, nArgs, args);
		PtSetResources(ABW_asCyclopvToggle, nArgs, args);
		PtSetResources(ABW_asGazehToggle, nArgs, args);
		PtSetResources(ABW_asGazevToggle, nArgs, args);
		PtSetResources(ABW_asOgazehToggle, nArgs, args);
		PtSetResources(ABW_asOgazevToggle, nArgs, args);
		PtSetResources(ABW_asVgazehToggle, nArgs, args);
		PtSetResources(ABW_asVgazevToggle, nArgs, args);
		PtSetResources(ABW_asCgazehToggle, nArgs, args);
		PtSetResources(ABW_asCgazevToggle, nArgs, args);
		break;
	case SIG_AD_SOURCE:
		nArgs = 0;
		PtSetArg(&args[nArgs++], Pt_ARG_FLAGS, Pt_FALSE, Pt_GHOST);
		PtSetArg(&args[nArgs++], Pt_ARG_FLAGS, Pt_TRUE, Pt_SELECTABLE);
		PtSetResources(ABW_asEyehToggle, nArgs, args);
		PtSetResources(ABW_asEyevToggle, nArgs, args);
		PtSetResources(ABW_asOeyehToggle, nArgs, args);
		PtSetResources(ABW_asOeyevToggle, nArgs, args);
		PtSetResources(ABW_asOtherhToggle, nArgs, args);
		PtSetResources(ABW_asOthervToggle, nArgs, args);
		PtSetResources(ABW_asAddhToggle, nArgs, args);
		PtSetResources(ABW_asAddvToggle, nArgs, args);
		PtSetResources(ABW_asJoyhToggle, nArgs, args);
		PtSetResources(ABW_asJoyvToggle, nArgs, args);

		nArgs = 0;
		PtSetArg(&args[nArgs++], Pt_ARG_FLAGS, Pt_TRUE, Pt_GHOST);
		PtSetArg(&args[nArgs++], Pt_ARG_FLAGS, Pt_FALSE, Pt_SELECTABLE);
		PtSetArg(&args[nArgs++], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_asDA0Toggle, nArgs, args);
		PtSetResources(ABW_asDA1Toggle, nArgs, args);
		PtSetResources(ABW_asDA2Toggle, nArgs, args);
		PtSetResources(ABW_asDA3Toggle, nArgs, args);
		PtSetResources(ABW_asRamp0XToggle, nArgs, args);
		PtSetResources(ABW_asRamp0YToggle, nArgs, args);
		PtSetResources(ABW_asRamp1XToggle, nArgs, args);
		PtSetResources(ABW_asRamp1YToggle, nArgs, args);
		PtSetResources(ABW_asWD0XToggle, nArgs, args);
		PtSetResources(ABW_asWD0YToggle, nArgs, args);
		PtSetResources(ABW_asWD1XToggle, nArgs, args);
		PtSetResources(ABW_asWD1YToggle, nArgs, args);
		PtSetResources(ABW_asVerghToggle, nArgs, args);
		PtSetResources(ABW_asVergvToggle, nArgs, args);
		PtSetResources(ABW_asCyclophToggle, nArgs, args);
		PtSetResources(ABW_asCyclopvToggle, nArgs, args);
		PtSetResources(ABW_asGazehToggle, nArgs, args);
		PtSetResources(ABW_asGazevToggle, nArgs, args);
		PtSetResources(ABW_asOgazehToggle, nArgs, args);
		PtSetResources(ABW_asOgazevToggle, nArgs, args);
		PtSetResources(ABW_asVgazehToggle, nArgs, args);
		PtSetResources(ABW_asVgazevToggle, nArgs, args);
		PtSetResources(ABW_asCgazehToggle, nArgs, args);
		PtSetResources(ABW_asCgazevToggle, nArgs, args);
		break;
	case SIG_MEM_SOURCE:
		nArgs = 0;
		PtSetArg(&args[nArgs++], Pt_ARG_FLAGS, Pt_FALSE, Pt_GHOST);
		PtSetArg(&args[nArgs++], Pt_ARG_FLAGS, Pt_TRUE, Pt_SELECTABLE);
		PtSetResources(ABW_asDA0Toggle, nArgs, args);
		PtSetResources(ABW_asDA1Toggle, nArgs, args);
		PtSetResources(ABW_asDA2Toggle, nArgs, args);
		PtSetResources(ABW_asDA3Toggle, nArgs, args);
		PtSetResources(ABW_asRamp0XToggle, nArgs, args);
		PtSetResources(ABW_asRamp0YToggle, nArgs, args);
		PtSetResources(ABW_asRamp1XToggle, nArgs, args);
		PtSetResources(ABW_asRamp1YToggle, nArgs, args);
		PtSetResources(ABW_asWD0XToggle, nArgs, args);
		PtSetResources(ABW_asWD0YToggle, nArgs, args);
		PtSetResources(ABW_asWD1XToggle, nArgs, args);
		PtSetResources(ABW_asWD1YToggle, nArgs, args);

		nArgs = 0;
		PtSetArg(&args[nArgs++], Pt_ARG_FLAGS, Pt_TRUE, Pt_GHOST);
		PtSetArg(&args[nArgs++], Pt_ARG_FLAGS, Pt_FALSE, Pt_SELECTABLE);
		PtSetArg(&args[nArgs++], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_asEyehToggle, nArgs, args);
		PtSetResources(ABW_asEyevToggle, nArgs, args);
		PtSetResources(ABW_asOeyehToggle, nArgs, args);
		PtSetResources(ABW_asOeyevToggle, nArgs, args);
		PtSetResources(ABW_asOtherhToggle, nArgs, args);
		PtSetResources(ABW_asOthervToggle, nArgs, args);
		PtSetResources(ABW_asAddhToggle, nArgs, args);
		PtSetResources(ABW_asAddvToggle, nArgs, args);
		PtSetResources(ABW_asJoyhToggle, nArgs, args);
		PtSetResources(ABW_asJoyvToggle, nArgs, args);
		PtSetResources(ABW_asVerghToggle, nArgs, args);
		PtSetResources(ABW_asVergvToggle, nArgs, args);
		PtSetResources(ABW_asCyclophToggle, nArgs, args);
		PtSetResources(ABW_asCyclopvToggle, nArgs, args);
		PtSetResources(ABW_asGazehToggle, nArgs, args);
		PtSetResources(ABW_asGazevToggle, nArgs, args);
		PtSetResources(ABW_asOgazehToggle, nArgs, args);
		PtSetResources(ABW_asOgazevToggle, nArgs, args);
		PtSetResources(ABW_asVgazehToggle, nArgs, args);
		PtSetResources(ABW_asVgazevToggle, nArgs, args);
		PtSetResources(ABW_asCgazehToggle, nArgs, args);
		PtSetResources(ABW_asCgazevToggle, nArgs, args);
		break;
	case SIG_COMP_SOURCE:
		nArgs = 0;
		PtSetArg(&args[nArgs++], Pt_ARG_FLAGS, Pt_FALSE, Pt_GHOST);
		PtSetArg(&args[nArgs++], Pt_ARG_FLAGS, Pt_TRUE, Pt_SELECTABLE);
		PtSetResources(ABW_asVerghToggle, nArgs, args);
		PtSetResources(ABW_asVergvToggle, nArgs, args);
		PtSetResources(ABW_asCyclophToggle, nArgs, args);
		PtSetResources(ABW_asCyclopvToggle, nArgs, args);
		PtSetResources(ABW_asGazehToggle, nArgs, args);
		PtSetResources(ABW_asGazevToggle, nArgs, args);
		PtSetResources(ABW_asOgazehToggle, nArgs, args);
		PtSetResources(ABW_asOgazevToggle, nArgs, args);
		PtSetResources(ABW_asVgazehToggle, nArgs, args);
		PtSetResources(ABW_asVgazevToggle, nArgs, args);
		PtSetResources(ABW_asCgazehToggle, nArgs, args);
		PtSetResources(ABW_asCgazevToggle, nArgs, args);

		nArgs = 0;
		PtSetArg(&args[nArgs++], Pt_ARG_FLAGS, Pt_TRUE, Pt_GHOST);
		PtSetArg(&args[nArgs++], Pt_ARG_FLAGS, Pt_FALSE, Pt_SELECTABLE);
		PtSetArg(&args[nArgs++], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_asEyehToggle, nArgs, args);
		PtSetResources(ABW_asEyevToggle, nArgs, args);
		PtSetResources(ABW_asOeyehToggle, nArgs, args);
		PtSetResources(ABW_asOeyevToggle, nArgs, args);
		PtSetResources(ABW_asOtherhToggle, nArgs, args);
		PtSetResources(ABW_asOthervToggle, nArgs, args);
		PtSetResources(ABW_asAddhToggle, nArgs, args);
		PtSetResources(ABW_asAddvToggle, nArgs, args);
		PtSetResources(ABW_asJoyhToggle, nArgs, args);
		PtSetResources(ABW_asJoyvToggle, nArgs, args);
		PtSetResources(ABW_asDA0Toggle, nArgs, args);
		PtSetResources(ABW_asDA1Toggle, nArgs, args);
		PtSetResources(ABW_asDA2Toggle, nArgs, args);
		PtSetResources(ABW_asDA3Toggle, nArgs, args);
		PtSetResources(ABW_asRamp0XToggle, nArgs, args);
		PtSetResources(ABW_asRamp0YToggle, nArgs, args);
		PtSetResources(ABW_asRamp1XToggle, nArgs, args);
		PtSetResources(ABW_asRamp1YToggle, nArgs, args);
		PtSetResources(ABW_asWD0XToggle, nArgs, args);
		PtSetResources(ABW_asWD0YToggle, nArgs, args);
		PtSetResources(ABW_asWD1XToggle, nArgs, args);
		PtSetResources(ABW_asWD1YToggle, nArgs, args);
		break;
	}
	return;
}

void setAcquireRateStates(int sigType)
{
	PtArg_t args[3];
	int nArgs;

	switch(sigType) {
	case SIG_OFF:
	case SIG_MEM_SOURCE:
	case SIG_COMP_SOURCE:
		nArgs = 0;
		PtSetArg(&args[nArgs++], Pt_ARG_FLAGS, Pt_TRUE, Pt_GHOST);
		PtSetArg(&args[nArgs++], Pt_ARG_FLAGS, Pt_FALSE, Pt_SELECTABLE);
		PtSetArg(&args[nArgs++], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_asAquire2000Toggle, nArgs, args);
		PtSetResources(ABW_asAquire1000Toggle, nArgs, args);
		PtSetResources(ABW_asAquire500Toggle, nArgs, args);
		PtSetResources(ABW_asAquire250Toggle, nArgs, args);
		break;
	case SIG_AD_SOURCE:
		nArgs = 0;
		PtSetArg(&args[nArgs++], Pt_ARG_FLAGS, Pt_FALSE, Pt_GHOST);
		PtSetArg(&args[nArgs++], Pt_ARG_FLAGS, Pt_TRUE, Pt_SELECTABLE);
		PtSetResources(ABW_asAquire2000Toggle, nArgs, args);
		PtSetResources(ABW_asAquire1000Toggle, nArgs, args);
		PtSetResources(ABW_asAquire500Toggle, nArgs, args);
		PtSetResources(ABW_asAquire250Toggle, nArgs, args);
		break;
	}
	return;
}

void setStoreRateStates(int sigType)
{
	PtArg_t args[3];
	int nArgs;

	switch(sigType) {
	case SIG_OFF:
	case SIG_COMP_SOURCE:
		nArgs = 0;
		PtSetArg(&args[nArgs++], Pt_ARG_FLAGS, Pt_TRUE, Pt_GHOST);
		PtSetArg(&args[nArgs++], Pt_ARG_FLAGS, Pt_FALSE, Pt_SELECTABLE);
		PtSetArg(&args[nArgs++], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_asStore2000Toggle, nArgs, args);
		PtSetResources(ABW_asStore1000Toggle, nArgs, args);
		PtSetResources(ABW_asStore500Toggle, nArgs, args);
		PtSetResources(ABW_asStore250Toggle, nArgs, args);
		break;
	case SIG_AD_SOURCE:
	case SIG_MEM_SOURCE:
		nArgs = 0;
		PtSetArg(&args[nArgs++], Pt_ARG_FLAGS, Pt_FALSE, Pt_GHOST);
		PtSetArg(&args[nArgs++], Pt_ARG_FLAGS, Pt_TRUE, Pt_SELECTABLE);
		PtSetResources(ABW_asStore2000Toggle, nArgs, args);
		PtSetResources(ABW_asStore1000Toggle, nArgs, args);
		PtSetResources(ABW_asStore500Toggle, nArgs, args);
		PtSetResources(ABW_asStore250Toggle, nArgs, args);
		break;
	}
	return;
}

void setComputedStoreRate(int sigNum)
{
	SIG *src1;
	SIG *src2;
	SIG *src3;

	src1 = src2 = (SIG *)NULL;
	src3 = (SIG *)-1;

	if(!strcmp(sig[sigNum].sig_mem_source, "vergh") || !strcmp(sig[sigNum].sig_mem_source, "cycloph")) {
		src1 = getSourceSignal("eyeh");
		src2 = getSourceSignal("oeyeh");
	}
	else if(!strcmp(sig[sigNum].sig_mem_source, "vergv") || !strcmp(sig[sigNum].sig_mem_source, "cyclopv")) {
		src1 = getSourceSignal("eyev");
		src2 = getSourceSignal("oeyev");
	}
	else if(!strcmp(sig[sigNum].sig_mem_source, "gazeh")) {
		src1 = getSourceSignal("eyeh");
		src2 = getSourceSignal("otherh");
	}
	else if(!strcmp(sig[sigNum].sig_mem_source, "gazev")) {
		src1 = getSourceSignal("eyev");
		src2 = getSourceSignal("otherv");
	}
	else if(!strcmp(sig[sigNum].sig_mem_source, "ogazeh")) {
		src1 = getSourceSignal("oeyeh");
		src2 = getSourceSignal("otherh");
	}
	else if(!strcmp(sig[sigNum].sig_mem_source, "ogazev")) {
		src1 = getSourceSignal("oeyev");
		src2 = getSourceSignal("otherv");
	}
	else if(!strcmp(sig[sigNum].sig_mem_source, "vgazeh") || !strcmp(sig[sigNum].sig_mem_source, "cgazeh")) {
		src1 = getSourceSignal("eyeh");
		src2 = getSourceSignal("oeyeh");
		src3 = getSourceSignal("otherh");
	}
	else if(!strcmp(sig[sigNum].sig_mem_source, "vgazev") || !strcmp(sig[sigNum].sig_mem_source, "cgazev")) {
		src1 = getSourceSignal("eyev");
		src2 = getSourceSignal("oeyev");
		src3 = getSourceSignal("otherv");
	}
	
	/* can't build computed signal if the sources aren't collected */
	if(!src1 || !src2 || !src3) {
		strcpy(sig[sigNum].sig_mem_source, "-");
		strcpy(sig[sigNum].sig_title, "-");
		sig[sigNum].sig_store_rate = NULLI;
		return;
	}

	src2->sig_ad_rate = src1->sig_ad_rate;
	sig[sigNum].sig_store_rate = src2->sig_store_rate = src1->sig_store_rate;
	if(src3 != (SIG *)-1) {
		src3->sig_ad_rate = src1->sig_ad_rate;
		src3->sig_store_rate = src1->sig_store_rate;
	}
	return;
}

SIG *getSourceSignal(char *ad_mem_var)
{
	int i;

	for(i = 0; i < SA_MAXSIG; i++) {
		if(!strcmp(sig[i].sig_ad_mem_var, ad_mem_var)) return(&sig[i]);
	}
	return((SIG *)NULL);
}

int initSigSummary( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	ApDBase_t *sigSumdbase;
	PtArg_t arg;
	int num;
	int y;
	int i;
	char label[P_LNAME];

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;
	
	/* open the signal summary data base */
	sigSumdbase = ApOpenDBase(ABM_sigSumPictures);

	/* set the parent for the widgets */
	PtSetParentWidget(ABW_sigSumScrollArea);

	/* create 16 sets of widgets for the signal summary */
	y = 0;
	for(i = 0; i < SA_MAXSIG; i++) {
		if(!sig[i].sig_enable) continue;	/* don't list signals that aren't enabled */

		/* create the label widget for this signal */
		sprintf(label, "%d", i);
		PtSetArg(&arg, Pt_ARG_TEXT_STRING, label, 0);
		ApCreateWidget(sigSumdbase, "sigSumLabel", 0, y, 1, &arg);

		/* create the signal type text widget for this signal */
		switch(sig[i].sig_enable) {
		case 1:
			strcpy(label, "a/d");
			break;
		case 2:
			strcpy(label, "mem");
			break;
		case 3:
			strcpy(label, "comp");
			break;
		}
		PtSetArg(&arg, Pt_ARG_TEXT_STRING, label, 0);
		ApCreateWidget(sigSumdbase, "sigSumTypeText", 43, y, 1, &arg);

		/* create the channel integer widget for this signal */
		if(sig[i].sig_ad_chan == NULLI) num = -1;
		else num = sig[i].sig_ad_chan;
		PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, num, 0);
		ApCreateWidget(sigSumdbase, "sigSumChannelInt", 100, y, 1, &arg);

		/* create the acquire rate integer widget for this signal */
		PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, sig[i].sig_ad_rate, 0);
		ApCreateWidget(sigSumdbase, "sigSumArateInt", 170, y, 1, &arg);

		/* create the store rate integer widget for this signal */
		PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, sig[i].sig_store_rate, 0);
		ApCreateWidget(sigSumdbase, "sigSumSrateInt", 240, y, 1, &arg);

		/* create the calibration integer widget for this signal */
		switch(sig[i].sig_ad_calib) {
		case 0:
			PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, 204, 0);
			break;
		case 1:
			PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, 102, 0);
			break;
		case 2:
			PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, 51, 0);
			break;
		case 3:
			PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, 25, 0);
			break;
		case 4:
			PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, 12, 0);
			break;
		case 5:
			PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, 6, 0);
			break;
		case 6:
			PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, 0, 0);
			break;
		default:
			PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, -1, 0);
			break;
		}
		ApCreateWidget(sigSumdbase, "sigSumCalInt", 315, y, 1, &arg);

		/* create the gain integer widget for this signal */
#ifdef AD_CGLIST
		PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, sig[i].sig_ad_gain, 0);
		ApCreateWidget(sigSumdbase, "sigSumGainInt", 380, y, 1, &arg);
#endif
		/* create the delay integer widget for this signal */
		PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, sig[i].sig_ad_delay, 0);
		ApCreateWidget(sigSumdbase, "sigSumDelayInt", 440, y, 1, &arg);

		/* create the name text widget for this signal */
		PtSetArg(&arg, Pt_ARG_TEXT_STRING, sig[i].sig_ad_mem_var, 0);
		ApCreateWidget(sigSumdbase, "sigSumNameText", 500, y, 1, &arg);

		/* create the title text widget for this signal */
		PtSetArg(&arg, Pt_ARG_TEXT_STRING, sig[i].sig_title, 0);
		ApCreateWidget(sigSumdbase, "sigSumTitleText", 570, y, 1, &arg);

		/* increment the y value for widget placement */
		y += 30;
	}
	
	/* set the maximum range of the scroll widget */
	PtSetArg(&arg, Pt_ARG_SCROLL_AREA_MAX_Y, y, 0);
	PtSetResources(ABW_sigSumScrollArea, 1, &arg);

	/* close the widget data base */
	ApCloseDBase(sigSumdbase);
	return( Pt_CONTINUE );
}

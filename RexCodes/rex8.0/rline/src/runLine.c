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
#include "ph_rline.h"
#include "abimport.h"
#include "proto.h"

extern lsrRunLine runLine;
extern uint16_t runLineMask;
extern TRACE rlTraces[RL_MAXCHAN + 2];

int signalScale( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;
	long *flags;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// get the state of the button
	PtGetResource(widget, Pt_ARG_FLAGS, &flags, 0);

	if(*flags & Pt_SET) {
		if(ApName(widget) == ABN_scale0Btn) runLine.scaleFactor = 0;
		else if(ApName(widget) == ABN_scale1Btn) runLine.scaleFactor = 1;
		else if(ApName(widget) == ABN_scale2Btn) runLine.scaleFactor = 2;
		else if(ApName(widget) == ABN_scale3Btn) runLine.scaleFactor = 3;
		else if(ApName(widget) == ABN_scale4Btn) runLine.scaleFactor = 4;
		else if(ApName(widget) == ABN_scale5Btn) runLine.scaleFactor = 5;
	}

	writeRoot();

	return( Pt_CONTINUE );
}

int signalWrap( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	long *flags;
	uint16_t height;
	uint16_t bitCount;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// get the state of the button
	PtGetResource(widget, Pt_ARG_FLAGS, &flags, 0);

	if(*flags & Pt_SET) {
		height = runLine.area.size.h;
		bitCount = 0;
		runLineMask = 0;
		while(height > 0) {
			runLineMask |= (1 << bitCount);
			height /= 2;
			bitCount++;
		}
	}
	else {
		runLineMask = 0xffff;
	}

	return( Pt_CONTINUE );
}

int runBarOff( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;
	int value;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the value from the widget */
	PtSetArg(&arg, Pt_ARG_SCROLL_POSITION, 0, 0);
	PtGetResources(widget, 1, &arg);
	value = (int)arg.value;
	rlTraces[RL_BAR_INDX].rl_offset = value;

	writeRoot();

	return( Pt_CONTINUE );
}

int rlineTrig( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
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
	runLine.trigger = *value;

	writeRoot();

	return( Pt_CONTINUE );
}

int rlTrigLevel( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;
	int *value;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the value of the widget */
	PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, &value, 0);
	PtGetResources(widget, 1, &arg);

	runLine.triggerLevel = *value;

	writeRoot();

	return( Pt_CONTINUE );
}

int rlineDisplay( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
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
		PtSetArg(&arg, Pt_ARG_TEXT_STRING, "One Shot", 0);
		PtSetResources(widget, 1, &arg);
	}
	else {
		PtSetArg(&arg, Pt_ARG_TEXT_STRING, "Repeat", 0);
		PtSetResources(widget, 1, &arg);
	}

	runLine.display = *value;

	writeRoot();

	return( Pt_CONTINUE );
}

int rlineMode( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
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
		PtSetArg(&arg, Pt_ARG_TEXT_STRING, "Superimpose", 0);
		PtSetResources(widget, 1, &arg);
	}
	else {
		PtSetArg(&arg, Pt_ARG_TEXT_STRING, "Single", 0);
		PtSetResources(widget, 1, &arg);
	}

	runLine.modeValue = *value;
	runLine.drawStart.x = -1;

	writeRoot();

	return( Pt_CONTINUE );
}

int rlDisplayReset( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* reset the running line display */
	runLine.dispReset = 1;

	return( Pt_CONTINUE );
}

int runDispRate( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;
	int value;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the value from the widget */
	PtSetArg(&arg, Pt_ARG_SCROLL_POSITION, 0, 0);
	PtGetResources(widget, 1, &arg);
	value = (int)arg.value;

	runLine.timeBase = value;
	runLine.timeCounter = value;

	writeRoot();

	return( Pt_CONTINUE );
}

int initRunChan( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	RL_CHAN *thisChan;
	PtWidget_t *dialog;
	PtArg_t args[2];
	PtListCallback_t *listData;
	int chanNum;
	int atten;
	int offset;
	int rate;
	int gain;
	char title[16];

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	/* if this function was called from the list widget */
	if(PtWidgetClass(link_instance) == PtList) {
		if(cbinfo->reason_subtype != Pt_LIST_SELECTION_FINAL) return(Pt_CONTINUE);
		dialog = ApGetInstance(link_instance);
		listData = (PtListCallback_t *)cbinfo->cbdata;
		sscanf(listData->item, "Channel %d", &chanNum);
	}
	else {
		dialog = link_instance;
		chanNum = 0;
		PtListSelectPos(ABW_runChanList, 1);

		/* get the current values from the oscope widget */
		rate = runLine.timeBase;
		gain = runLine.scaleFactor;

		PtSetArg(&args[0], Pt_ARG_SCROLL_POSITION, rate, 0);
		PtSetResources(ABW_runDispRateScroll, 1, &args[0]);

		PtSetArg(&args[0], Pt_ARG_SCROLL_POSITION, rlTraces[RL_BAR_INDX].rl_offset, 0);
		PtSetResources(ABW_runDispBarOffScroll, 1, &args[0]);

		switch(gain) {
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
		default:
			PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
			PtSetResources(ABW_scale0Btn, 1, &args[0]);
			PtSetResources(ABW_scale1Btn, 1, &args[0]);
			PtSetResources(ABW_scale2Btn, 1, &args[0]);
			PtSetResources(ABW_scale3Btn, 1, &args[0]);
			PtSetResources(ABW_scale4Btn, 1, &args[0]);
			PtSetResources(ABW_scale5Btn, 1, &args[0]);
			break;
		}
	}

	/* set the module's title widget */
	sprintf(title, "Channel %d", chanNum);
	PtSetArg(&args[0], Pt_ARG_TEXT_STRING, title, 0);
	PtSetResources(ABW_runChanLabel, 1, &args[0]);

	/* set the channel number in all of this module's widgets */
	PtSetArg(&args[0], Pt_ARG_USER_DATA, &chanNum, sizeof(int));
	PtSetResources(ABW_runChanSigNumb, 1, &args[0]);
	PtSetResources(ABW_runChanScaleScroll, 1, &args[0]);
	PtSetResources(ABW_runChanOffsetScroll, 1, &args[0]);

	thisChan = (RL_CHAN *)&i_b->rl.rl_chan[chanNum];

	/* set the values of the widgets in this module */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, thisChan->rl_signal, 0);
	PtSetResources(ABW_runChanSigNumb, 1, &args[0]);

	if(thisChan->rl_signal >= 0) {
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, r_b->r_samp.sa_title[thisChan->rl_signal], 0);
	}
	else {
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, "", 0);
	}
	PtSetResources(ABW_runChanSigTitle, 1, &args[0]);

	atten = -rlTraces[chanNum].rl_scale;
	PtSetArg(&args[0], Pt_ARG_SCROLL_POSITION, atten, 0);
	PtSetResources(ABW_runChanScaleScroll, 1, &args[0]);

	offset = rlTraces[chanNum].rl_offset;
	PtSetArg(&args[0], Pt_ARG_SCROLL_POSITION, offset, 0);
	PtSetResources(ABW_runChanOffsetScroll, 1, &args[0]);

	return( Pt_CONTINUE );
}

int runChanSig( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[2];
	int chanNum;
	int value;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the channel number and the value from the widget */
	PtSetArg(&args[0], Pt_ARG_USER_DATA, 0, 0);
	PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, 0, 0);
	PtGetResources(widget, 2, args);

	chanNum = *(int *)args[0].value;
	value = (int)args[1].value;

	i_b->rl.rl_chan[chanNum].rl_signal = value;

	/* set the signal title in the title widget */
	PtSetArg(&args[0], Pt_ARG_TEXT_STRING, r_b->r_samp.sa_title[value], 0);
	PtSetResources(ABW_runChanSigTitle, 1, &args[0]);

	return( Pt_CONTINUE );
}

int runChanScale( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[2];
	int chanNum;
	int value;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the channel number and value from the widget */
	PtSetArg(&args[0], Pt_ARG_USER_DATA, 0, 0);
	PtSetArg(&args[1], Pt_ARG_SCROLL_POSITION, 0, 0);
	PtGetResources(widget, 2, args);

	chanNum = *(int *)args[0].value;
	value = (int)args[1].value;
	rlTraces[chanNum].rl_scale = -value;

	return( Pt_CONTINUE );
}

int runChanOffset( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[2];
	int chanNum;
	int value;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the channel number and value from the widget */
	PtSetArg(&args[0], Pt_ARG_USER_DATA, 0, 0);
	PtSetArg(&args[1], Pt_ARG_SCROLL_POSITION, 0, 0);
	PtGetResources(widget, 2, args);

	chanNum = *(int *)args[0].value;
	value = (int)args[1].value;
	rlTraces[chanNum].rl_offset = value;

	return( Pt_CONTINUE );
}

int signalChoice( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	long *flags;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// get the state of the button
	PtGetResource(widget, Pt_ARG_FLAGS, &flags, 0);

	if(*flags & Pt_SET) {
		if(ApName(widget) == ABN_chan0Toggle) rlTraces[0].rl_on = 1;
		else if(ApName(widget) == ABN_chan1Toggle) rlTraces[1].rl_on = 1;
		else if(ApName(widget) == ABN_chan2Toggle) rlTraces[2].rl_on = 1;
		else if(ApName(widget) == ABN_chan3Toggle) rlTraces[3].rl_on = 1;
		else if(ApName(widget) == ABN_chan4Toggle) rlTraces[4].rl_on = 1;
		else if(ApName(widget) == ABN_chan5Toggle) rlTraces[5].rl_on = 1;
		else if(ApName(widget) == ABN_chan6Toggle) rlTraces[6].rl_on = 1;
		else if(ApName(widget) == ABN_chan7Toggle) rlTraces[7].rl_on = 1;
		else if(ApName(widget) == ABN_rlineToggle) rlTraces[RL_BAR_INDX].rl_on = 1;
		else if(ApName(widget) == ABN_unitsToggle) rlTraces[RL_SPK_INDX].rl_on = 1;
	}
	else {
		if(ApName(widget) == ABN_chan0Toggle) rlTraces[0].rl_on = 0;
		else if(ApName(widget) == ABN_chan1Toggle) rlTraces[1].rl_on = 0;
		else if(ApName(widget) == ABN_chan2Toggle) rlTraces[2].rl_on = 0;
		else if(ApName(widget) == ABN_chan3Toggle) rlTraces[3].rl_on = 0;
		else if(ApName(widget) == ABN_chan4Toggle) rlTraces[4].rl_on = 0;
		else if(ApName(widget) == ABN_chan5Toggle) rlTraces[5].rl_on = 0;
		else if(ApName(widget) == ABN_chan6Toggle) rlTraces[6].rl_on = 0;
		else if(ApName(widget) == ABN_chan7Toggle) rlTraces[7].rl_on = 0;
		else if(ApName(widget) == ABN_rlineToggle) rlTraces[RL_BAR_INDX].rl_on = 0;
		else if(ApName(widget) == ABN_unitsToggle) rlTraces[RL_SPK_INDX].rl_on = 0;
	}

	return( Pt_CONTINUE );
}

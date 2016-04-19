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

int processMenuOvrRides[] = { 0, 0 };
int preTime = 0;
int postTime = 0;

int switchClock( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[2];
	int *value;
	int response;
	extern int clockSwitched;
	extern int clockState;
	
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the value of the widget */
	PtSetArg(&args[0], Pt_ARG_ONOFF_STATE, &value, 0);
	PtGetResources(widget, 1, &args[0]);

	/* start the msec clock */
	if(*value) {
		/* make sure that scribe is running */
 		if(i_b->scrb_pi <= 0) {
			response = PtAskQuestion(ABW_base, NULL,
						  "Can't start clock\nNo scribe process", "Helvetica09ba",
						  "&Continue", NULL, NULL, 1);
			return( Pt_CONTINUE );
		}
		
		if(prego()) {
			response = PtAskQuestion(ABW_base, NULL,
						  "Can't start clock\nPrego failed", "Helvetica09ba",
						  "&Continue", NULL, NULL, 1);
			return( Pt_CONTINUE );
		}

		/* set the label on the clock widget */
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, "Clock\nOn", 0);
		PtSetArg(&args[1], Pt_ARG_FILL_COLOR, Pg_GREEN, 0);
		PtSetResources(widget, 2, args);
	}

	/* stop the msec clock */
	else {
		/* make sure that data is not being kept */
		if(i_b->i_flags & (I_EOUT | I_AOUT)) {
			response = PtAskQuestion(ABW_base, NULL,
						  "Can't stop clock\nwhile keeping data", "Helvetica09ba",
						  "&Continue", NULL , NULL, 1);

			/* reset the onoff state of the widget to on */
			PtSetArg(&args[0], Pt_ARG_ONOFF_STATE, 1, 0);
			PtSetResources(widget, 1, &args[0]);
			return( Pt_CONTINUE );
		}

		/* set the label on the clock widget */
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, "Clock\nOff", 0);
		PtSetArg(&args[1], Pt_ARG_FILL_COLOR, Pg_RED, 0);
		PtSetResources(widget, 2, args);
	}

	/* signal the clock thread that the user switched the clock */
	pthread_sleepon_lock();
	clockSwitched = 1;
	clockState = *value;
	pthread_sleepon_signal(&clockSwitched);
	pthread_sleepon_unlock();

	return( Pt_CONTINUE );
}

int switchParadigm( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[2];
	int *value;
	
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the value of the widget */
	PtSetArg(&args[0], Pt_ARG_ONOFF_STATE, &value, 0);
	PtGetResources(widget, 1, &args[0]);

	/* change the text of the widget to reflect current state */
	if(*value) {
		i_b->i_flags &= ~I_PSTOP;
		softswitch &= ~PSTOP;
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, "Paradigm\nRunning", 0);
		PtSetArg(&args[1], Pt_ARG_FILL_COLOR, Pg_GREEN, 0);
	}
	else {
		i_b->i_flags |= I_PSTOP;
		softswitch |= PSTOP;
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, "Paradigm\nStopped", 0);
		PtSetArg(&args[1], Pt_ARG_FILL_COLOR, Pg_RED, 0);
	}
	PtSetResources(widget, 2, args);

#ifdef NIH_ACUC
	i_b->inthdr.i_lastCorrect = i_b->i_time;
#endif

	return( Pt_CONTINUE );
}

int resetStates( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	int i;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	again();

	for(i = 0; i < MAXSTATE; i++) stateDebug[i].color = 0;

	return( Pt_CONTINUE );
}

int switchWindows( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[2];
	int *value;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the value of the widget */
	PtSetArg(&args[0], Pt_ARG_ONOFF_STATE, &value, 0);
	PtGetResources(widget, 1, &args[0]);

	/* change the text of the widget to reflect current state */
	if(*value) {
		wd_test(0);
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, "Checking\nWindows", 0);
		PtSetArg(&args[1], Pt_ARG_FILL_COLOR, Pg_GREEN, 0);
	}
	else {
		wd_test(1);
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, "Ignoring\nWindows", 0);
		PtSetArg(&args[1], Pt_ARG_FILL_COLOR, Pg_RED, 0);
	}
	PtSetResources(widget, 2, args);

	return( Pt_CONTINUE );
}

int initProcessDialog( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[2];
	int i;

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	/* set the values of the menu override switches */
	/* pre time */
	if(processMenuOvrRides[0]) PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
	else PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
	PtSetResources(ABW_processPreTimeOvrRide, 1, &args[0]);

	/* post time */
	if(processMenuOvrRides[1]) PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
	else PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
	PtSetResources(ABW_processPostTimeOvrRide, 1, &args[0]);

	/*set the values of the numeric widgets */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, preTime, 0);
	PtSetResources(ABW_processPreTime, 1, &args[0]);

	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, postTime, 0);
	PtSetResources(ABW_processPostTime, 1, &args[0]);

	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, st_rate, 0);
	PtSetResources(ABW_processStateProcRate, 1, &args[0]);

	/* set the value of the rex units toggles */
	switch(m_uwind) {
	case 0:
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_processUnitSaveToggle, 1, &args[0]);
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(ABW_processUnitIgnoreToggle, 1, &args[0]);
		break;
	case 1:
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(ABW_processUnitSaveToggle, 1, &args[0]);
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_processUnitIgnoreToggle, 1, &args[0]);
		break;
	default:
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_processUnitSaveToggle, 1, &args[0]);
		PtSetResources(ABW_processUnitIgnoreToggle, 1, &args[0]);
		break;
	}

	/* set the value of the mexwex toggles */
#ifdef MEXWEX_UNITS
	switch(mexwex_units) {
	case 0:
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(ABW_processMWOffToggle, 1, &args[0]);
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_processMWVer1Toggle, 1, &args[0]);
		PtSetResources(ABW_processMWVer2Toggle, 1, &args[0]);
		break;
	case 1:
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(ABW_processMWVer1Toggle, 1, &args[0]);
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_processMWOffToggle, 1, &args[0]);
		PtSetResources(ABW_processMWVer2Toggle, 1, &args[0]);
		break;
	case 2:
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(ABW_processMWVer2Toggle, 1, &args[0]);
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_processMWOffToggle, 1, &args[0]);
		PtSetResources(ABW_processMWVer1Toggle, 1, &args[0]);
		break;
	}
#else 
	PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET|Pt_SELECTABLE);
	PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_TRUE, Pt_GHOST);
	PtSetResources(ABW_processMWOffToggle, 2, args);
	PtSetResources(ABW_processMWVer1Toggle, 2, args);
	PtSetResources(ABW_processMWVer2Toggle, 2, args);
#endif

	/* set the value of the plexon units toggles */
#ifdef TC_UNITS
	if(i_b->i_flags & I_GO) i = 2;
	else i = tc_units;
	switch(i) {
	case 0:
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_processPlexonSaveToggle, 1, &args[0]);
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(ABW_processPlexonIgnoreToggle, 1, &args[0]);
		break;
	case 1:
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(ABW_processPlexonSaveToggle, 1, &args[0]);
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_processPlexonIgnoreToggle, 1, &args[0]);
		break;
	 case 2:
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET|Pt_SELECTABLE);
		PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_TRUE, Pt_GHOST);
		PtSetResources(ABW_processPlexonSaveToggle, 2, args);
		PtSetResources(ABW_processPlexonIgnoreToggle, 2, args);
		break;
	}
#else
	PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET|Pt_SELECTABLE);
	PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_TRUE, Pt_GHOST);
	PtSetResources(ABW_processPlexonSaveToggle, 2, args);
	PtSetResources(ABW_processPlexonIgnoreToggle, 2, args);
#endif

	/* set the value of the SMI toggles */
#ifdef SMI_TRACK
	if(i_b->i_flags & I_GO) i = 2;
	else i = smi_on;
	switch(i) {
	case 0:
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_processSMIOnToggle, 1, &args[0]);
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(ABW_processSMIOffToggle, 1, &args[0]);
		break;
	case 1:
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(ABW_processSMIOnToggle, 1, &args[0]);
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_processSMIOffToggle, 1, &args[0]);
		break;
	 case 2:
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET|Pt_SELECTABLE);
		PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_TRUE, Pt_GHOST);
		PtSetResources(ABW_processSMIOnToggle, 2, args);
		PtSetResources(ABW_processSMIOffToggle, 2, args);
		break;
	}
#else
	PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET|Pt_SELECTABLE);
	PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_TRUE, Pt_GHOST);
	PtSetResources(ABW_processSMIOnToggle, 2, args);
	PtSetResources(ABW_processSMIOffToggle, 2, args);
#endif

	/* set the value of the a/d gain toggles */
	if(i_b->i_flags & I_GO) i = 16;
	else i = ad_ov_gain;
	switch(i) {
	case 1:
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(ABW_processADgain1Toggle, 1, &args[0]);
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_processADgain2Toggle, 1, &args[0]);
		PtSetResources(ABW_processADgain4Toggle, 1, &args[0]);
		PtSetResources(ABW_processADgain8Toggle, 1, &args[0]);
		break;
	case 2:
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(ABW_processADgain2Toggle, 1, &args[0]);
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_processADgain1Toggle, 1, &args[0]);
		PtSetResources(ABW_processADgain4Toggle, 1, &args[0]);
		PtSetResources(ABW_processADgain8Toggle, 1, &args[0]);
		break;
	case 4:
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(ABW_processADgain4Toggle, 1, &args[0]);
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_processADgain1Toggle, 1, &args[0]);
		PtSetResources(ABW_processADgain2Toggle, 1, &args[0]);
		PtSetResources(ABW_processADgain8Toggle, 1, &args[0]);
		break;
	case 8:
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(ABW_processADgain8Toggle, 1, &args[0]);
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_processADgain1Toggle, 1, &args[0]);
		PtSetResources(ABW_processADgain2Toggle, 1, &args[0]);
		PtSetResources(ABW_processADgain4Toggle, 1, &args[0]);
		break;
	 case 16:
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET|Pt_SELECTABLE);
		PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_TRUE, Pt_GHOST);
		PtSetResources(ABW_processADgain1Toggle, 2, args);
		PtSetResources(ABW_processADgain2Toggle, 2, args);
		PtSetResources(ABW_processADgain4Toggle, 2, args);
		PtSetResources(ABW_processADgain8Toggle, 2, args);
		break;
	}

	/* set the value of the a/d rate toggles */
	if(i_b->i_flags & I_GO) i = 500;
	else i = ad_max_rate;
	switch(i) {
	case 2000:
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(ABW_processADrate2000Toggle, 1, &args[0]);
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_processADrate1000Toggle, 1, &args[0]);
		break;
	case 1000:
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(ABW_processADrate1000Toggle, 1, &args[0]);
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_processADrate2000Toggle, 1, &args[0]);
		break;
	 case 500:
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET|Pt_SELECTABLE);
		PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_TRUE, Pt_GHOST);
		PtSetResources(ABW_processADrate2000Toggle, 2, args);
		PtSetResources(ABW_processADrate1000Toggle, 2, args);
		break;
	}

	/* set the value of the debug output toggles */
	if(doutput_rot)	PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
	else PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
	PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);

	if(doutput_inmem)	PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
	else PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
	PtSetResources(ABW_processDBinmemToggle, 1, &args[0]);

	if(doutput_tofile)	PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
	else PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
	PtSetResources(ABW_processDBfileToggle, 1, &args[0]);

	/* set the values of the module's "actual" widgets */
	/* pre time */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, w_pre, 0);
	PtSetResources(ABW_processPreTimeAct, 1, &args[0]);

	/* post time */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, w_post, 0);
	PtSetResources(ABW_processPostTimeAct, 1, &args[0]);

	return( Pt_CONTINUE );
}

int processMenuOvrRide( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* determine which of the override buttons was pressed */
	if(ApName(widget) == ABN_processPreTimeOvrRide) {
		if(cbinfo->reason == Pt_CB_ARM) {
			processMenuOvrRides[0] = 1;
			m_pre = preTime;
		}
		else if(cbinfo->reason == Pt_CB_DISARM) {
			processMenuOvrRides[0] = 0;
			m_pre = NULLI;
		}
		pre_post(NULLI, NULLI);
	}
	else if(ApName(widget) == ABN_processPostTimeOvrRide) {
		if(cbinfo->reason == Pt_CB_ARM) {
			processMenuOvrRides[1] = 1;
			m_post = postTime;
		}
		else if(cbinfo->reason == Pt_CB_DISARM) {
			processMenuOvrRides[1] = 0;
			m_post = NULLI;
		}
		pre_post(NULLI, NULLI);
	}

	/* set the values of the module's "actual" widgets */
	/* pre time */
	PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, w_pre, 0);
	PtSetResources(ABW_processPreTimeAct, 1, &arg);

	/* post time */
	PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, w_post, 0);
	PtSetResources(ABW_processPostTimeAct, 1, &arg);
	return( Pt_CONTINUE );
}

int processPreTime( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;
	int value;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the widget value */
	PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, 0, 0);
	PtGetResources(widget, 1, &arg);

	value = (int)arg.value;
	preTime = value;

	if(processMenuOvrRides[0]) {
		m_pre = preTime;
		pre_post(NULLI, NULLI);

		/* pre time */
		PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, w_pre, 0);
		PtSetResources(ABW_processPreTimeAct, 1, &arg);
	}

	return( Pt_CONTINUE );
}

int processPostTime( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;
	int value;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the widget value */
	PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, 0, 0);
	PtGetResources(widget, 1, &arg);

	value = (int)arg.value;
	postTime = value;

	if(processMenuOvrRides[0]) {
		m_post = postTime;
		pre_post(NULLI, NULLI);

		/* post time */
		PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, w_post, 0);
		PtSetResources(ABW_processPostTimeAct, 1, &arg);
	}

	return( Pt_CONTINUE );
}

int processStateRate( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;
	int value;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the widget value */
	PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, 0, 0);
	PtGetResources(widget, 1, &arg);

	value = (int)arg.value;
	st_rate = value;

	return( Pt_CONTINUE );
}

int processUnitControl( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;
	
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* determine which toggle initiated this callback */
	/* rex units */
	if(ApName(widget) == ABN_processUnitSaveToggle) {
		if(cbinfo->reason == Pt_CB_ARM) m_uwind = 1;
		else if(cbinfo->reason == Pt_CB_DISARM) m_uwind = NULLI;
		uw_set(NULLI);
	}
	else if(ApName(widget) == ABN_processUnitIgnoreToggle) {
		if(cbinfo->reason == Pt_CB_ARM) m_uwind = 0;
		else if(cbinfo->reason == Pt_CB_DISARM) m_uwind = NULLI;
		uw_set(NULLI);
	}
	/* mex/wex units */
#ifdef MEXWEX_UNITS
	else if(ApName(widget) == ABN_processMWOffToggle) {
		if(cbinfo->reason == Pt_CB_ARM) mexwex_units = 0;
		else if(cbinfo->reason == Pt_CB_DISARM) mexwex_units = NULLI;
	}
	else if(ApName(widget) == ABN_processMWVer1Toggle) {
		if(cbinfo->reason == Pt_CB_ARM) mexwex_units = 1;
		else if(cbinfo->reason == Pt_CB_DISARM) mexwex_units = NULLI;
	}
	else if(ApName(widget) == ABN_processMWVer2Toggle) {
		if(cbinfo->reason == Pt_CB_ARM) mexwex_units = 2;
		else if(cbinfo->reason == Pt_CB_DISARM) mexwex_units = NULLI;
	}
#endif

	/* plexon units */
#ifdef TC_UNITS
	else if(ApName(widget) == ABN_processPlexonSaveToggle) {
		if(i_b->i_flags & I_GO) {
			PtAskQuestion(ABW_signalDialog, NULL,
						  "Can't change plexon units\nwhile clock is running", "Helvetica09ba",
						  "&Continue", NULL, NULL, 1);

			PtSetArg(&arg, Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
			PtSetResources(widget, 1, &arg);
			return( Pt_CONTINUE );
		}
		if(cbinfo->reason == Pt_CB_ARM) tc_units = 1;
		else if(cbinfo->reason == Pt_CB_DISARM) tc_units = NULLI;
	}
	else if(ApName(widget) == ABN_processPlexonIgnoreToggle) {
		if(i_b->i_flags & I_GO) {
			PtAskQuestion(ABW_signalDialog, NULL,
						  "Can't change plexon units\nwhile clock is running", "Helvetica09ba",
						  "&Continue", NULL, NULL, 1);
			PtSetArg(&arg, Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
			PtSetResources(widget, 1, &arg);
			return( Pt_CONTINUE );
		}
		if(cbinfo->reason == Pt_CB_ARM) tc_units = 0;
		else if(cbinfo->reason == Pt_CB_DISARM) tc_units = NULLI;
	}
#endif

	/* SMI eye tracker */
#ifdef SMI_TRACK
	else if(ApName(widget) == ABN_processSMIOnToggle) {
		if(i_b->i_flags & I_GO) {
			PtAskQuestion(ABW_signalDialog, NULL,
						  "Can't change SMI\nwhile clock is running", "Helvetica09ba",
						  "&Continue", NULL, NULL, 1);
			return( Pt_CONTINUE );
		}
		if(cbinfo->reason == Pt_CB_ARM) smi_on = 1;
		else if(cbinfo->reason == Pt_CB_DISARM) smi_on = NULLI;
	}
	else if(ApName(widget) == ABN_processSMIOffToggle) {
		if(i_b->i_flags & I_GO) {
			PtAskQuestion(ABW_signalDialog, NULL,
						  "Can't change SMI\nwhile clock is running", "Helvetica09ba",
						  "&Continue", NULL, NULL, 1);
			return( Pt_CONTINUE );
		}
		if(cbinfo->reason == Pt_CB_ARM) smi_on = 0;
		else if(cbinfo->reason == Pt_CB_DISARM) smi_on = NULLI;
	}
#endif

	return( Pt_CONTINUE );
}

int processADgain( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;
	
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(i_b->i_flags & I_GO) {
		PtAskQuestion(ABW_signalDialog, NULL,
					  "Can't change a/d gain\nwhile clock is running", "Helvetica09ba",
					  "&Continue", NULL, NULL, 1);
		return( Pt_CONTINUE );
	}

	/* determine which toggle initiated this callback */
	if(ApName(widget) == ABN_processADgain1Toggle) ad_ov_gain = 1;
	else if(ApName(widget) == ABN_processADgain2Toggle) ad_ov_gain = 2;
	else if(ApName(widget) == ABN_processADgain4Toggle) ad_ov_gain = 4;
	else if(ApName(widget) == ABN_processADgain8Toggle) ad_ov_gain = 8;

	return( Pt_CONTINUE );
}

int processADrate( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(i_b->i_flags & I_GO) {
		PtAskQuestion(ABW_signalDialog, NULL,
					  "Can't change a/d rate\nwhile clock is running", "Helvetica09ba",
					  "&Continue", NULL, NULL, 1);
		return( Pt_CONTINUE );
	}

	/* determine which toggle initiated this callback */
	if(ApName(widget) == ABN_processADrate2000Toggle) ad_max_rate = 2000;
	else if(ApName(widget) == ABN_processADrate1000Toggle) ad_max_rate = 1000;

	return( Pt_CONTINUE );
}

int processDebugState( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* determine which toggle initiated this callback */
	if(ApName(widget) == ABN_processDBrotateToggle) {
		if(cbinfo->reason == Pt_CB_ARM) doutput_rot = 1;
		else if(cbinfo->reason == Pt_CB_DISARM) doutput_rot = 0;
	}
	else if(ApName(widget) == ABN_processDBinmemToggle) {
		if(cbinfo->reason == Pt_CB_ARM) doutput_inmem = 1;
		else if(cbinfo->reason == Pt_CB_DISARM) doutput_inmem = 0;
	}
	else if(ApName(widget) == ABN_processDBfileToggle) {
		if(cbinfo->reason == Pt_CB_ARM) doutput_tofile = 1;
		else if(cbinfo->reason == Pt_CB_DISARM) doutput_tofile = 0;
	}

	return( Pt_CONTINUE );
}

int printDebug( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	doutput(0);
	
	return( Pt_CONTINUE );
}

int initScoreAlertDialog( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	PtSetResource(ABW_scoreAlertTimer, Pt_ARG_TIMER_INITIAL, 60000, 0);

	return( Pt_CONTINUE );
}

int monkeyAlert( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	system("monkeyAlert.sh");

	/* 	r_sendmsg(&i_b->ptbl[i_b->ph_switch_pi], CM_DIE); */

	return( Pt_CONTINUE );
}

int cancelAlert( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	PtSetResource(ABW_scoreAlertTimer, Pt_ARG_TIMER_INITIAL, 0, 0);
	PtDestroyWidget(ABW_scoreAlertDialog);

	return( Pt_CONTINUE );
}

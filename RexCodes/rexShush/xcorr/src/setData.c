/* Y o u r   D e s c r i p t i o n                       */
/*                            AppBuilder Photon Code Lib */
/*                                         Version 2.01  */

/* Standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* Local headers */
#include "ablibs.h"
#include "ph_xcorr.h"
#include "abimport.h"
#include "proto.h"

EVENT_P	alignPtr = (EVENT_P)NULL;
EVENT_P	lastAlignPtr = (EVENT_P)NULL;
EVENT_P eventTop;
double sigma = 1.0;
int inGetData = 0;
int dataStarted = 0;
int trigUnit = 601;
int corrUnit = 602;
long alignTime = 0;
int nCorrUnits = 0;
long corrUnitTimes[1000];

int switchSampling( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	extern GRAPH pageGraph;

	EVENT_P ldPtr = (EVENT_P)NULL;
	long *flags;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the value of the widget */
	PtGetResource(widget, Pt_ARG_FLAGS, &flags, 0);

	if(*flags & Pt_SET) {
		if(!(i_b->i_flags & I_GO)) {
			PtSetResource(widget, Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
			PtNotice(ABW_base, NULL, "Xcorr Error", NULL,
					 "Must start process clock\nbefore starting Xcorr sampling", "Helvetica09ba",
					 "OK", "Helvetica09ba",
					 Pt_CENTER | Pt_BLOCK_PARENT);

			return( Pt_END );
		}
		
		if(i_b->evlx == 0) {
				PtSetResource(widget, Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
				PtNotice(ABW_base, NULL, "Xcorr Error", NULL,
						 "Event buffer empty\nWait a few seconds", "Helvetica09ba",
						 "OK", "Helvetica09ba",
						 Pt_CENTER | Pt_BLOCK_PARENT);

				alignPtr = (EVENT_P)NULL;
				lastAlignPtr = (EVENT_P)NULL;
		}

		ldPtr = (EVENT_P)&i_b->bevent[i_b->evlx - 1];
		alignPtr = ldPtr;
		alignTime = ldPtr->e_key - (pageGraph.interval + pageGraph.start);

		// find an event that is at least postTime msecs before the current time
		while(alignPtr->e_key > alignTime) {
			--alignPtr;	if(alignPtr < (EVENT_P)&i_b->bevent[0]) alignPtr += EBUFNUM;

			// if this is true, there are no events at least
			// postTime msecs before the current time
			if(alignPtr == ldPtr) {
				PtSetResource(widget, Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
				PtNotice(ABW_base, NULL, "Xcorr Error", NULL,
						 "No events before alignment time found in the data\nWait a few seconds", "Helvetica09ba",
						 "OK", "Helvetica09ba",
						 Pt_CENTER | Pt_BLOCK_PARENT);

				alignPtr = (EVENT_P)NULL;
				lastAlignPtr = (EVENT_P)NULL;

				return( Pt_CONTINUE );
			}
		}

		// find the first triggering unit before the align time
		while(alignPtr->e_code != trigUnit) {
			--alignPtr;	if(alignPtr < (EVENT_P)&i_b->bevent[0]) alignPtr += EBUFNUM;

			// if this is true, there are no triggering units in the data
			if(alignPtr == ldPtr) {
				PtSetResource(widget, Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
				PtNotice(ABW_base, NULL, "Xcorr Error", NULL,
						 "No triggering units found in the data\nChoose a new triggering unit", "Helvetica09ba",
						 "OK", "Helvetica09ba",
						 Pt_CENTER | Pt_BLOCK_PARENT);

				alignPtr = (EVENT_P)NULL;
				lastAlignPtr = (EVENT_P)NULL;

				return( Pt_CONTINUE );
			}
		}
		// at this point, alignPtr points to the last instance of a triggering spike
		// that is at least postTime before the top of the data

		// disable the configuration widgets
		PtSetResource(ABW_clearButton, Pt_ARG_FLAGS, Pt_TRUE, Pt_BLOCKED | Pt_GHOST);
		PtSetResource(ABW_trigUnitInteger, Pt_ARG_FLAGS, Pt_TRUE, Pt_BLOCKED | Pt_GHOST);
		PtSetResource(ABW_xcorrUnitInteger, Pt_ARG_FLAGS, Pt_TRUE, Pt_BLOCKED | Pt_GHOST);
		PtSetResource(ABW_preInteger, Pt_ARG_FLAGS, Pt_TRUE, Pt_BLOCKED | Pt_GHOST);
		PtSetResource(ABW_intervalInteger, Pt_ARG_FLAGS, Pt_TRUE, Pt_BLOCKED | Pt_GHOST);
		PtSetResource(ABW_sigmaFloat, Pt_ARG_FLAGS, Pt_TRUE, Pt_BLOCKED | Pt_GHOST);

		dataStarted = 1;
		/*
		initThisTimerHisto(0, "draw", 20, 1000);
		initThisTimerHisto(1, "timer", 20, 1000);
		initThisTimerHisto(2, "search", 20, 1000);
		*/
		inGetData = 0;

		PtSetResource(ABW_xcorrTimer, Pt_ARG_TIMER_INITIAL, 50, 0);
		//		startTimer(1);
	}
	else {
		lastAlignPtr = (EVENT_P)NULL;

		PtSetResource(ABW_xcorrTimer, Pt_ARG_TIMER_INITIAL, 0, 0);

		dataStarted = 0;

		// enable the configuration widgets
		PtSetResource(ABW_clearButton, Pt_ARG_FLAGS, Pt_FALSE, Pt_BLOCKED | Pt_GHOST);
		PtSetResource(ABW_trigUnitInteger, Pt_ARG_FLAGS, Pt_FALSE, Pt_BLOCKED | Pt_GHOST);
		PtSetResource(ABW_xcorrUnitInteger, Pt_ARG_FLAGS, Pt_FALSE, Pt_BLOCKED | Pt_GHOST);
		PtSetResource(ABW_preInteger, Pt_ARG_FLAGS, Pt_FALSE, Pt_BLOCKED | Pt_GHOST);
		PtSetResource(ABW_intervalInteger, Pt_ARG_FLAGS, Pt_FALSE, Pt_BLOCKED | Pt_GHOST);
		PtSetResource(ABW_sigmaFloat, Pt_ARG_FLAGS, Pt_FALSE, Pt_BLOCKED | Pt_GHOST);

		//		printTimes();
	}
	return( Pt_CONTINUE );
}

int clearData( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	resetData();
	drawXcorr();

	return( Pt_CONTINUE );
}

int setGain( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	extern GRAPH pageGraph;

	PtScrollbarCallback_t *scrollData;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	scrollData = (PtScrollbarCallback_t *)cbinfo->cbdata;
	pageGraph.maxFreq = scrollData->position;

	drawXcorr();

	writeRoot();

	return( Pt_CONTINUE );
}

int setTrigUnit( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	extern LABEL pageLabel;

	int *value;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	PtGetResource(widget, Pt_ARG_NUMERIC_VALUE, &value, 0);
	trigUnit = *value;

	if(trigUnit == corrUnit) {
		sprintf(pageLabel.label, "Unit %d Autocorrelation", trigUnit);
	}
	else {
		sprintf(pageLabel.label, "%d X %d", trigUnit, corrUnit);
	}

	resetData();
	drawXcorr();

	writeRoot();

	return( Pt_CONTINUE );
}

int setCorrelatedUnit( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	extern LABEL pageLabel;

	int *value;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	PtGetResource(widget, Pt_ARG_NUMERIC_VALUE, &value, 0);
	corrUnit = *value;

	if(corrUnit == trigUnit) {
		sprintf(pageLabel.label, "Unit %d Autocorrelation", trigUnit);
	}
	else {
		sprintf(pageLabel.label, "%d X %d", trigUnit, corrUnit);
	}

	resetData();

	writeRoot();

	return( Pt_CONTINUE );
}

int setPreTime( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	extern GRAPH pageGraph;

	int *value;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	PtGetResource(widget, Pt_ARG_NUMERIC_VALUE, &value, 0);
	pageGraph.start = *value;

	resetData();

	writeRoot();

	return( Pt_CONTINUE );
}

int setInterval( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	extern GRAPH pageGraph;

	int *value;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	PtGetResource(widget, Pt_ARG_NUMERIC_VALUE, &value, 0);
	if(*value > 0) {
		pageGraph.interval = *value;
		if(pageGraph.interval > 10) {
			pageGraph.timeMark = pageGraph.interval / 10;
		}
		else {
			pageGraph.timeMark = pageGraph.interval;
		}

		resetData();
	}

	writeRoot();

	return( Pt_CONTINUE );
}

int setSigma( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	double *value;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	PtGetResource(widget, Pt_ARG_NUMERIC_VALUE, &value, 0);
	sigma = *value;

	resetData();

	writeRoot();

	return( Pt_CONTINUE );
}

int getData( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	extern GRAPH pageGraph;

	EVENT_P ldPtr = (EVENT_P)NULL;
	EVENT_P corrPtr;
	int begTime;
	int endTime;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(inGetData > 0) {
		dprintf("getData re-entered %d\n", inGetData);
		return( Pt_CONTINUE );
	}

	inGetData++;
	//	stopTimer(1);
	//	startTimer(1);

	// if the process clock has stopped, stop the cross correlation sampling
	if(!(i_b->i_flags & I_GO)) {
		PtSetResource(ABW_xcorrTimer, Pt_ARG_TIMER_INITIAL, 0, 0);

		PtSetResource(ABW_sampleToggleButton, Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResource(ABW_clearButton, Pt_ARG_FLAGS, Pt_FALSE, Pt_BLOCKED | Pt_GHOST);
		PtSetResource(ABW_trigUnitInteger, Pt_ARG_FLAGS, Pt_FALSE, Pt_BLOCKED | Pt_GHOST);
		PtSetResource(ABW_xcorrUnitInteger, Pt_ARG_FLAGS, Pt_FALSE, Pt_BLOCKED | Pt_GHOST);
		PtSetResource(ABW_preInteger, Pt_ARG_FLAGS, Pt_FALSE, Pt_BLOCKED | Pt_GHOST);
		PtSetResource(ABW_intervalInteger, Pt_ARG_FLAGS, Pt_FALSE, Pt_BLOCKED | Pt_GHOST);
		PtSetResource(ABW_sigmaFloat, Pt_ARG_FLAGS, Pt_FALSE, Pt_BLOCKED | Pt_GHOST);

		alignPtr = (EVENT_P)NULL;

		inGetData--;
		return( Pt_CONTINUE );
	}

	nCorrUnits = 0;

	// if the current triggering spike is not the same as the last triggering spike,
	// find an event that is preTime msecs before the alignment time
	if(lastAlignPtr != alignPtr) {
		// start profiling
		//		startTimer(0);

		begTime = alignPtr->e_key + pageGraph.start;
		endTime = begTime + pageGraph.interval;
		corrPtr = alignPtr;

		while(corrPtr->e_key > begTime) {
			--corrPtr; if(corrPtr < &i_b->bevent[0]) corrPtr += EBUFNUM;
		}

		// at this point corrPtr points to an event that is at or before begTime
		// so increment to point to the first event in the interval
		++corrPtr; if(corrPtr >= eventTop) corrPtr -= EBUFNUM;

		// find the relative times of the correlated units that occurred
		// between begTime and endTime
		while(corrPtr->e_key < endTime) {
			if(corrPtr->e_code == corrUnit) {
				corrUnitTimes[nCorrUnits++] = corrPtr->e_key - begTime;
				if(nCorrUnits >= 1000) break;
			}
			++corrPtr; if(corrPtr >= eventTop) corrPtr -= EBUFNUM;
		}

		if(nCorrUnits > 0) {
			// create a spike density funtion and raster
			// and update the drawing
			loadData(nCorrUnits, corrUnitTimes);
		}

		// save the pointer to this triggering spike
		lastAlignPtr = alignPtr;
		//   		stopTimer(0);
	}

	// get a pointer to the next triggering spike
	if(i_b->evlx == 0) ldPtr = (EVENT_P)&i_b->bevent[EBUFNUM - 1];
	else ldPtr = (EVENT_P)&i_b->bevent[i_b->evlx - 1];
	if(ldPtr != alignPtr) {
		//		startTimer(2);
		alignPtr = ldPtr;
		alignTime = ldPtr->e_key - (pageGraph.interval + pageGraph.start);

		while(alignPtr->e_key > alignTime) {
			--alignPtr;	if(alignPtr < &i_b->bevent[0]) alignPtr += EBUFNUM;
			if(alignPtr == lastAlignPtr) break;
		}

		if(alignPtr != lastAlignPtr) {
			while(alignPtr->e_code != trigUnit) {
				--alignPtr;	if(alignPtr < &i_b->bevent[0]) alignPtr += EBUFNUM;
				if(alignPtr == lastAlignPtr) break;
			}
		}
		//		stopTimer(2);
	}

	inGetData--;

	return( Pt_CONTINUE );
}

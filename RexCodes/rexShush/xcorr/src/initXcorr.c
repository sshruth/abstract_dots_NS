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

int initXcorr( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	extern GRAPH pageGraph;
	extern LABEL pageLabel;
	extern double sigma;
	extern int trigUnit;
	extern int corrUnit;

	PhDim_t dim;
	PhRect_t frame;
	short int x;
	short int y;

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	if(apinfo->reason == ABR_PRE_REALIZE) { /* stuff to do before realizing widget */
		PgSetDrawBufferSize(16384);	/* allocate a 16K draw buffer */
	}
	else {	/* stuff to do after reaslizing widget */
		PtSetResource(ABW_sampleToggleButton, Pt_ARG_LABEL_BALLOON, inflateToggleBalloon, 0);

		/* set the initial scaling factor for the tig widget */
		calcScale();

		/* save the location and size of the main window for rex session */
		PtWindowGetFrameSize(link_instance, &frame);
		PtGetAbsPosition(link_instance, &x, &y);
		myptp->p_x = x - frame.ul.x;
		myptp->p_y = y - frame.ul.y;

		PtWidgetDim(link_instance, &dim);
		myptp->p_w = dim.w;
		myptp->p_h = dim.h;

		/* read root file from rex session */
		if(strlen(myptp->p_root) > 0) {
			readRoot(myptp->p_root);
		}

		/* set initial values of display controls */
		PtSetResource(ABW_gainScrollbar, Pt_ARG_GAUGE_VALUE, pageGraph.maxFreq, 0);
		PtSetResource(ABW_trigUnitInteger, Pt_ARG_NUMERIC_VALUE, trigUnit, 0);
		PtSetResource(ABW_xcorrUnitInteger, Pt_ARG_NUMERIC_VALUE, corrUnit, 0);
		PtSetResource(ABW_preInteger, Pt_ARG_NUMERIC_VALUE, pageGraph.start, 0);
		PtSetResource(ABW_intervalInteger, Pt_ARG_NUMERIC_VALUE, pageGraph.interval, 0);
		PtSetResource(ABW_sigmaFloat, Pt_ARG_NUMERIC_VALUE, &sigma, 0);

		if(trigUnit == corrUnit) {
			sprintf(pageLabel.label, "Unit %d Autocorrelation", trigUnit);
		}
		else {
			sprintf(pageLabel.label, "%d X %d", trigUnit, corrUnit);
		}
	}

	return( Pt_CONTINUE );
}

PtWidget_t *inflateToggleBalloon(PtWidget_t *window, PtWidget_t *widget, int position,
								char *string, char *font, PgColor_t fill, PgColor_t text)
{
	if(ApName(widget) == ABN_sampleToggleButton) {
		return(PtInflateBalloon(window, widget, position, "Toggle sampling on and off",
								font, fill, text));
	}
	return((PtWidget_t *)NULL);
}

void writeRoot()
{
	extern GRAPH pageGraph;
	extern double sigma;
	extern int trigUnit;
	extern int corrUnit;

	snprintf(myptp->p_root, P_LROOTNAME, "G %d TU %d XU %d PT %d I %d S %1.2f",
			 pageGraph.maxFreq, trigUnit, corrUnit, pageGraph.start, pageGraph.interval, sigma);
}

void readRoot(char *root)
{
	extern GRAPH pageGraph;
	extern double sigma;
	extern int trigUnit;
	extern int corrUnit;

	char *attrbStr;
	char *valueStr;
	char rootCopy[P_LROOTNAME];

	/* make a local copy of the root for strtok */
	strncpy(rootCopy, root, P_LROOTNAME);
	attrbStr = strtok(rootCopy, " ");
	while(attrbStr != (char *)NULL) {
		valueStr = strtok(NULL, " ");
		if(valueStr != (char *)NULL) {
			if(!strcmp(attrbStr, "G")) {
				sscanf(valueStr, "%d", &pageGraph.maxFreq);
			}
			else if(!strcmp(attrbStr, "TU")) {
				sscanf(valueStr, "%d", &trigUnit);
			}
			else if(!strcmp(attrbStr, "XU")) {
				sscanf(valueStr, "%d", &corrUnit);
			}
			else if(!strcmp(attrbStr, "PT")) {
				sscanf(valueStr, "%d", &pageGraph.start);
			}
			else if(!strcmp(attrbStr, "I")) {
				sscanf(valueStr, "%d", &pageGraph.interval);
			}
			else if(!strcmp(attrbStr, "S")) {
				sscanf(valueStr, "%lf", &sigma);
			}
		}

		attrbStr = strtok(NULL, " ");
	}
}

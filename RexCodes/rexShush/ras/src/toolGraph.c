/* Y o u r   D e s c r i p t i o n                       */
/*                            AppBuilder Photon Code Lib */
/*                                         Version 1.13X */

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
#include "ph_ras.h"
#include "abimport.h"
#include "proto.h"

void initGraphWidgets(PtWidget_t *thisModule, GRAPH *thisGraph)
{
	PtWidget_t *widget;
	PtArg_t args[1];
	int value;

	/* set the value of the start widget */
	widget = ApGetWidgetPtr(thisModule, ABN_graphStart);
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, thisGraph->start, 0);
	PtSetResources(widget, 1, args);

	/* set the value of the duration widget */
	widget = ApGetWidgetPtr(thisModule, ABN_graphDuration);
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, thisGraph->interval, 0);
	PtSetResources(widget, 1, args);

	/* set the value of the firing rate widget */
	widget = ApGetWidgetPtr(thisModule, ABN_graphRate);
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, thisGraph->maxFreq, 0);
	PtSetResources(widget, 1, args);

	/* set the value of the tick interval widget */
	widget = ApGetWidgetPtr(thisModule, ABN_graphTic);
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, thisGraph->timeMark, 0);
	PtSetResources(widget, 1, args);

	/* set the value of the firing rate widget */
	widget = ApGetWidgetPtr(thisModule, ABN_graphRate);
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, thisGraph->maxFreq, 0);
	PtSetResources(widget, 1, args);

	/* set the value of the Y axis on-off button */
	widget = ApGetWidgetPtr(thisModule, ABN_graphYaxis);
	if(thisGraph->yAxes & R_LEFT) value = 1;
	else value = 0;
	PtSetArg(&args[0], Pt_ARG_ONOFF_STATE, value, 0);
	PtSetResources(widget, 1, args);

	/* set the value of the alignment bar on-off button */
	widget = ApGetWidgetPtr(thisModule, ABN_graphAlignment);
	if(thisGraph->yAxes & R_ALIGN) value = 1;
	else value = 0;
	PtSetArg(&args[0], Pt_ARG_ONOFF_STATE, value, 0);
	PtSetResources(widget, 1, args);

	/* set the value of the number axes on-off button */
	widget = ApGetWidgetPtr(thisModule, ABN_graphNumbers);
	if(thisGraph->numberAxes) value = 1;
	else value = 0;
	PtSetArg(&args[0], Pt_ARG_ONOFF_STATE, value, 0);
	PtSetResources(widget, 1, args);

	/* set the values of the graph number format widgets */
	switch(thisGraph->Nformat) {
	case 0:
		widget = ApGetWidgetPtr(thisModule, ABN_graphAxisInt);
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(widget, 1, args);
		widget = ApGetWidgetPtr(thisModule, ABN_graphAxisFloat);
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(widget, 1, args);
		break;
	case 1:
		widget = ApGetWidgetPtr(thisModule, ABN_graphAxisInt);
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(widget, 1, args);
		widget = ApGetWidgetPtr(thisModule, ABN_graphAxisFloat);
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(widget, 1, args);
		break;
	default:
		fprintf(stderr, "%d is an invalid format\n", thisGraph->Nformat);
		break;
	}

	/* set the graph number font widgets */
	switch(thisGraph->Nfont) {
	case 12:
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		widget = ApGetWidgetPtr(thisModule, ABN_graphAxisNumberDuplex);
		PtSetResources(widget, 1, args);
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		widget = ApGetWidgetPtr(thisModule, ABN_graphAxisNumberComplex);
		PtSetResources(widget, 1, args);
		widget = ApGetWidgetPtr(thisModule, ABN_graphAxisNumberItalic);
		PtSetResources(widget, 1, args);
		break;
	case 13:
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		widget = ApGetWidgetPtr(thisModule, ABN_graphAxisNumberComplex);
		PtSetResources(widget, 1, args);
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		widget = ApGetWidgetPtr(thisModule, ABN_graphAxisNumberDuplex);
		PtSetResources(widget, 1, args);
		widget = ApGetWidgetPtr(thisModule, ABN_graphAxisNumberItalic);
		PtSetResources(widget, 1, args);
		break;
	case 15:
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		widget = ApGetWidgetPtr(thisModule, ABN_graphAxisNumberItalic);
		PtSetResources(widget, 1, args);
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		widget = ApGetWidgetPtr(thisModule, ABN_graphAxisNumberComplex);
		PtSetResources(widget, 1, args);
		widget = ApGetWidgetPtr(thisModule, ABN_graphAxisNumberDuplex);
		PtSetResources(widget, 1, args);
		break;
	default:
		fprintf(stderr, "font %d invalid number font\n", thisGraph->Nfont);
	}

	/* set the value of the graph number size widget */
	widget = ApGetWidgetPtr(thisModule, ABN_graphAxisNumberSize);
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, thisGraph->Nsz, 0);
	PtSetResources(widget, 1, args);

	/* set the value of the graph Xaxis label text */
	widget = ApGetWidgetPtr(thisModule, ABN_graphXaxisLabelText);
	PtSetArg(&args[0], Pt_ARG_TEXT_STRING, &thisGraph->XaxisLabel, 0);
	PtSetResources(widget, 1, args);

	/* set the value of the graph Yaxis label text */
	widget = ApGetWidgetPtr(thisModule, ABN_graphYaxisLabelText);
	PtSetArg(&args[0], Pt_ARG_TEXT_STRING, &thisGraph->YaxisLabel, 0);
	PtSetResources(widget, 1, args);

	/* set the graph axis label font widgets */
	switch(thisGraph->Lfont) {
	case 12:
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		widget = ApGetWidgetPtr(thisModule, ABN_graphAxisLabelDuplex);
		PtSetResources(widget, 1, args);
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		widget = ApGetWidgetPtr(thisModule, ABN_graphAxisLabelComplex);
		PtSetResources(widget, 1, args);
		widget = ApGetWidgetPtr(thisModule, ABN_graphAxisLabelItalic);
		PtSetResources(widget, 1, args);
		break;
	case 13:
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		widget = ApGetWidgetPtr(thisModule, ABN_graphAxisLabelComplex);
		PtSetResources(widget, 1, args);
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		widget = ApGetWidgetPtr(thisModule, ABN_graphAxisLabelDuplex);
		PtSetResources(widget, 1, args);
		widget = ApGetWidgetPtr(thisModule, ABN_graphAxisLabelItalic);
		PtSetResources(widget, 1, args);
		break;
	case 15:
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		widget = ApGetWidgetPtr(thisModule, ABN_graphAxisLabelItalic);
		PtSetResources(widget, 1, args);
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		widget = ApGetWidgetPtr(thisModule, ABN_graphAxisLabelComplex);
		PtSetResources(widget, 1, args);
		widget = ApGetWidgetPtr(thisModule, ABN_graphAxisLabelDuplex);
		PtSetResources(widget, 1, args);
		break;
	default:
		fprintf(stderr, "font %d invalid label font\n", thisGraph->Lfont);
	}

	/* set the value of the graph number size widget */
	widget = ApGetWidgetPtr(thisModule, ABN_graphAxisLabelSize);
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, thisGraph->Lsz, 0);
	PtSetResources(widget, 1, args);

	return;
}

int graphStartProc(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
	PtArg_t args[1];
	PtWidget_t *thisModule;
	RASTER *rasters;
	PAGE *thisPage;
	CELL *thisCell;
	PLOT *thisPlot;
	GRAPH *thisGraph;
	int *value;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get a pointer to the rasters structure */
	rasters = getRasters();

	/* get pointer to the window module that contains this widget */
	thisModule = ApGetInstance(widget);

	/* figure out this raster page */
	thisPage = getRasterPage(rasters->curPageIndx);
	thisCell = getRasterCell(thisPage->cellsInfo, thisPage->curCellIndx);
	thisPlot = &thisCell->plotInfo;
	thisGraph = &thisPlot->graph;
	
	/* get the widget's value and assign it to the appropriate variable */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, &value, 0);
	PtGetResources(widget, 1, args);
	thisGraph->start = *value;

	/* set the button's label */
	setButtonLabel(thisModule, thisCell);

	thisPage->graphVariables |= GRAPH_START;

	return( Pt_CONTINUE );
}

int graphDurationProc(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
	PtArg_t args[1];
	PtWidget_t *thisModule;
	RASTER *rasters;
	PAGE *thisPage;
	CELL *thisCell;
	PLOT *thisPlot;
	GRAPH *thisGraph;
	int *value;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get a pointer to the rasters structure */
	rasters = getRasters();

	/* get pointer to the window module that contains this widget */
	thisModule = ApGetInstance(widget);

	/* figure out this raster page */
	thisPage = getRasterPage(rasters->curPageIndx);
	thisCell = getRasterCell(thisPage->cellsInfo, thisPage->curCellIndx);
	thisPlot = &thisCell->plotInfo;
	thisGraph = &thisPlot->graph;
	
	/* get the widget's value and assign it to the appropriate variable */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, &value, 0);
	PtGetResources(widget, 1, args);
	thisGraph->interval = *value;

	/* set the button's label */
	setButtonLabel(thisModule, thisCell);

	thisPage->graphVariables |= GRAPH_DUR;

	return( Pt_CONTINUE );
}

int graphRateProc(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
	PtArg_t args[1];
	PtWidget_t *thisModule;
	RASTER *rasters;
	PAGE *thisPage;
	CELL *thisCell;
	PLOT *thisPlot;
	GRAPH *thisGraph;
	int *value;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get a pointer to the rasters structure */
	rasters = getRasters();

	/* get pointer to the window module that contains this widget */
	thisModule = ApGetInstance(widget);

	/* figure out this raster page */
	thisPage = getRasterPage(rasters->curPageIndx);
	thisCell = getRasterCell(thisPage->cellsInfo, thisPage->curCellIndx);
	thisPlot = &thisCell->plotInfo;
	thisGraph = &thisPlot->graph;
	
	/* get the widget's value and assign it to the appropriate variable */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, &value, 0);
	PtGetResources(widget, 1, args);
	thisGraph->maxFreq = *value;

	/* set the button's label */
	setButtonLabel(thisModule, thisCell);

	thisPage->graphVariables |= GRAPH_RATE;

	return( Pt_CONTINUE );
}

int graphTicProc(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
	PtArg_t args[1];
	PtWidget_t *thisModule;
	RASTER *rasters;
	PAGE *thisPage;
	CELL *thisCell;
	PLOT *thisPlot;
	GRAPH *thisGraph;
	int *value;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get a pointer to the rasters structure */
	rasters = getRasters();

	/* get pointer to the window module that contains this widget */
	thisModule = ApGetInstance(widget);

	/* figure out this raster page */
	thisPage = getRasterPage(rasters->curPageIndx);
	thisCell = getRasterCell(thisPage->cellsInfo, thisPage->curCellIndx);
	thisPlot = &thisCell->plotInfo;
	thisGraph = &thisPlot->graph;
	
	/* get the widget's value and assign it to the appropriate variable */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, &value, 0);
	PtGetResources(widget, 1, args);
	thisGraph->timeMark = *value;

	/* set the button's label */
	setButtonLabel(thisModule, thisCell);

	thisPage->graphVariables |= GRAPH_TIC;

	return( Pt_CONTINUE );
}

int graphYaxisProc(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
	PtArg_t args[1];
	PtWidget_t *thisModule;
	RASTER *rasters;
	PAGE *thisPage;
	CELL *thisCell;
	PLOT *thisPlot;
	GRAPH *thisGraph;
	int *value;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get a pointer to the rasters structure */
	rasters = getRasters();

	/* get pointer to the window module that contains this widget */
	thisModule = ApGetInstance(widget);

	/* figure out this raster page */
	thisPage = getRasterPage(rasters->curPageIndx);
	thisCell = getRasterCell(thisPage->cellsInfo, thisPage->curCellIndx);
	thisPlot = &thisCell->plotInfo;
	thisGraph = &thisPlot->graph;
	
	/* get the widget's value and assign it to the appropriate variable */
	PtSetArg(&args[0], Pt_ARG_ONOFF_STATE, &value, 0);
	PtGetResources(widget, 1, args);

	/* figure out which button was pressed */
	if(ApName(widget) == ABN_graphAlignment) {
		if(*value) thisGraph->yAxes |= R_ALIGN;
		else thisGraph->yAxes &= ~R_ALIGN;
	}
	else {
		if(*value) thisGraph->yAxes |= R_LEFT;
		else thisGraph->yAxes &= ~R_LEFT;
	}

	/* set the button's label */
	setButtonLabel(thisModule, thisCell);

	thisPage->graphVariables |= GRAPH_YAXIS;

	return( Pt_CONTINUE );
}

int graphNumberProc(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
	PtArg_t args[1];
	PtWidget_t *thisModule;
	RASTER *rasters;
	PAGE *thisPage;
	CELL *thisCell;
	PLOT *thisPlot;
	GRAPH *thisGraph;
	int *value;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get a pointer to the rasters structure */
	rasters = getRasters();

	/* get pointer to the window module that contains this widget */
	thisModule = ApGetInstance(widget);

	/* figure out this raster page */
	thisPage = getRasterPage(rasters->curPageIndx);
	thisCell = getRasterCell(thisPage->cellsInfo, thisPage->curCellIndx);
	thisPlot = &thisCell->plotInfo;
	thisGraph = &thisPlot->graph;
	
	/* get the widget's value and assign it to the appropriate variable */
	PtSetArg(&args[0], Pt_ARG_ONOFF_STATE, &value, 0);
	PtGetResources(widget, 1, args);
	thisGraph->numberAxes = *value;

	/* set the button's label */
	setButtonLabel(thisModule, thisCell);

	thisPage->graphVariables |= GRAPH_NUMBER;

	return( Pt_CONTINUE );
}

int graphAxisFormatProc(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
	PtArg_t args[1];
	PtWidget_t *thisModule;
	PtWidget_t *otherButton;
	RASTER *rasters;
	PAGE *thisPage;
	CELL *thisCell;
	PLOT *thisPlot;
	GRAPH *thisGraph;
	long *flags;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get a pointer to the rasters structure */
	rasters = getRasters();

	/* get pointer to the window module that contains this widget */
	thisModule = ApGetInstance(widget);

	/* figure out this raster page */
	thisPage = getRasterPage(rasters->curPageIndx);
	thisCell = getRasterCell(thisPage->cellsInfo, thisPage->curCellIndx);
	thisPlot = &thisCell->plotInfo;
	thisGraph = &thisPlot->graph;
	
	/* get the widget's value and assign it to the appropriate variable */
	PtSetArg(&args[0], Pt_ARG_FLAGS, &flags, 0);
	PtGetResources(widget, 1, args);

	if(*flags & Pt_SET) {
		/* figure out which button was pressed
		 * so the other one can be unset
		 */
		if(ApName(widget) == ABN_graphAxisInt) {
			otherButton = ApGetWidgetPtr(thisModule, ABN_graphAxisFloat);
			thisGraph->Nformat = 0;
		}
		else {
			otherButton = ApGetWidgetPtr(thisModule, ABN_graphAxisInt);
			thisGraph->Nformat = 1;
		}

		/* make sure the other button is unset */
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(otherButton, 1, args);
	}
	else {
		/* figure out which button was unset
		 * so the other one can be set
		 */
		if(ApName(widget) == ABN_graphAxisInt) {
			otherButton = ApGetWidgetPtr(thisModule, ABN_graphAxisFloat);
			thisGraph->Nformat = 1;
		}
		else {
			otherButton = ApGetWidgetPtr(thisModule, ABN_graphAxisInt);
			thisGraph->Nformat = 0;
		}

		/* make sure the other button is set */
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(otherButton, 1, args);
	}

	/* set the button's label */
	setButtonLabel(thisModule, thisCell);

	thisPage->graphVariables |= GRAPH_NFORMAT;

	return( Pt_CONTINUE );
}

int graphXaxisLabelTextProc(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
	PtArg_t args[1];
	PtWidget_t *thisModule;
	RASTER *rasters;
	PAGE *thisPage;
	CELL *thisCell;
	PLOT *thisPlot;
	GRAPH *thisGraph;
	char *string;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get a pointer to the rasters structure */
	rasters = getRasters();

	/* get pointer to the window module that contains this widget */
	thisModule = ApGetInstance(widget);

	/* figure out this raster page */
	thisPage = getRasterPage(rasters->curPageIndx);
	thisCell = getRasterCell(thisPage->cellsInfo, thisPage->curCellIndx);
	thisPlot = &thisCell->plotInfo;
	thisGraph = &thisPlot->graph;

	/* the widget's value and assign it to the graph X axis label text variable */
	PtSetArg(&args[0], Pt_ARG_TEXT_STRING, &string, 0);
	PtGetResources(widget, 1, args);

	/* make sure the label is not too long */
	if((strlen(string) > 0) && (strlen(string) < 128)) {
		thisPlot->labelFlag = 1;
		strcpy(thisGraph->XaxisLabel, string);
		thisGraph->XaxisLabel[127] = 0;	/* force NULL terminator */
	}
	else strcpy(thisGraph->XaxisLabel, "");

	setButtonLabel(thisModule, thisCell);
	
	thisPage->graphVariables |= GRAPH_XLABEL;

	return( Pt_CONTINUE );
}

int graphYaxisLabelTextProc(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
	PtArg_t args[1];
	PtWidget_t *thisModule;
	RASTER *rasters;
	PAGE *thisPage;
	CELL *thisCell;
	PLOT *thisPlot;
	GRAPH *thisGraph;
	char *string;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get a pointer to the rasters structure */
	rasters = getRasters();

	/* get pointer to the window module that contains this widget */
	thisModule = ApGetInstance(widget);

	/* figure out this raster page */
	thisPage = getRasterPage(rasters->curPageIndx);
	thisCell = getRasterCell(thisPage->cellsInfo, thisPage->curCellIndx);
	thisPlot = &thisCell->plotInfo;
	thisGraph = &thisPlot->graph;
	
	/* the widget's value and assign it to the graph Y axis label text variable */
	PtSetArg(&args[0], Pt_ARG_TEXT_STRING, &string, 0);
	PtGetResources(widget, 1, args);

	/* make sure the label is not too long */
	if((strlen(string) > 0) && (strlen(string) < 128)) {
		thisPlot->labelFlag = 1;
		strcpy(thisGraph->YaxisLabel, string);
		thisGraph->YaxisLabel[127] = 0;	/* force NULL terminator */
	}
	else strcpy(thisGraph->YaxisLabel, "");

	setButtonLabel(thisModule, thisCell);
	
	thisPage->graphVariables |= GRAPH_YLABEL;

	return( Pt_CONTINUE );
}

int graphAxisFontProc(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
	PtArg_t args[1];
	PtWidget_t *thisModule;
	PtWidget_t *dfont;
	PtWidget_t *off1;
	PtWidget_t *off2;
	RASTER *rasters;
	PAGE *thisPage;
	CELL *thisCell;
	PLOT *thisPlot;
	GRAPH *thisGraph;
	void *userData;
	long *flags;
	int *thisFont;
	int font;
	
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	dfont = (PtWidget_t *)NULL;
	off1 = (PtWidget_t *)NULL;
	off2 = (PtWidget_t *)NULL;
	thisFont = (int *)NULL;
	
	/* get a pointer to the rasters structure */
	rasters = getRasters();

	/* get pointer to the window module that contains this widget */
	thisModule = ApGetInstance(widget);

	/* figure out this raster page */
	thisPage = getRasterPage(rasters->curPageIndx);
	thisCell = getRasterCell(thisPage->cellsInfo, thisPage->curCellIndx);
	thisPlot = &thisCell->plotInfo;
	thisGraph = &thisPlot->graph;
	
	/* figure out which button of the set was pressed
	 * so the other two can be unset
	 */
	if(ApName(widget) == ABN_graphAxisLabelDuplex) {
		dfont = widget;
		off1 = ApGetWidgetPtr(thisModule, ABN_graphAxisLabelComplex);
		off2 = ApGetWidgetPtr(thisModule, ABN_graphAxisLabelItalic);
		thisFont = &thisGraph->Lfont;
	}
	else if(ApName(widget) == ABN_graphAxisLabelComplex) {
		dfont = ApGetWidgetPtr(thisModule, ABN_graphAxisLabelDuplex);
		off1 = ApGetWidgetPtr(thisModule, ABN_graphAxisLabelDuplex);
		off2 = ApGetWidgetPtr(thisModule, ABN_graphAxisLabelItalic);
		thisFont = &thisGraph->Lfont;
	}
	else if(ApName(widget) == ABN_graphAxisLabelItalic) {
		dfont = ApGetWidgetPtr(thisModule, ABN_graphAxisLabelDuplex);
		off1 = ApGetWidgetPtr(thisModule, ABN_graphAxisLabelDuplex);
		off2 = ApGetWidgetPtr(thisModule, ABN_graphAxisLabelComplex);
		thisFont = &thisGraph->Lfont;
	}
	else if(ApName(widget) == ABN_graphAxisNumberDuplex) {
		dfont = widget;
		off1 = ApGetWidgetPtr(thisModule, ABN_graphAxisNumberComplex);
		off2 = ApGetWidgetPtr(thisModule, ABN_graphAxisNumberItalic);
		thisFont = &thisGraph->Nfont;
	}
	else if(ApName(widget) == ABN_graphAxisNumberComplex) {
		dfont = ApGetWidgetPtr(thisModule, ABN_graphAxisNumberDuplex);
		off1 = ApGetWidgetPtr(thisModule, ABN_graphAxisNumberDuplex);
		off2 = ApGetWidgetPtr(thisModule, ABN_graphAxisNumberItalic);
		thisFont = &thisGraph->Nfont;
	}
	else if(ApName(widget) == ABN_graphAxisNumberItalic) {
		dfont = ApGetWidgetPtr(thisModule, ABN_graphAxisNumberDuplex);
		off1 = ApGetWidgetPtr(thisModule, ABN_graphAxisNumberDuplex);
		off2 = ApGetWidgetPtr(thisModule, ABN_graphAxisNumberComplex);
		thisFont = &thisGraph->Nfont;
	}

	/* get the widget's value and assign it to the appropriate variable */
	PtSetArg(&args[0], Pt_ARG_FLAGS, &flags, 0);
	PtGetResources(widget, 1, args);

	/* if this widget's button was pressed,
	 * make sure that the other two widgets' buttons are unpressed
	 */
	if(*flags & Pt_SET) {
		/* get the font */
		PtSetArg(&args[0], Pt_ARG_USER_DATA, &userData, 0);
		PtGetResources(widget, 1, args);
		sscanf(userData, "%d", &font);
		
		/* unset the other two buttons in the set*/
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(off1, 1, args);
		PtSetResources(off2, 1, args);
	}
	
	/* if this widget's button was unpressed,
	 * set the default widget's button
	 */
	else {
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(dfont, 1, args);

		PtSetArg(&args[0], Pt_ARG_USER_DATA, &userData, 0);
		PtGetResources(dfont, 1, args);
		sscanf(userData, "%d", &font);
	}
	 
	if(thisFont != (int *)NULL) *thisFont = font;

	setButtonLabel(thisModule, thisCell);
	
	thisPage->graphVariables |= GRAPH_LFONT;

	return( Pt_CONTINUE );
}

int graphSizeProc(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
	PtArg_t args[1];
	PtWidget_t *thisModule;
	RASTER *rasters;
	PAGE *thisPage;
	CELL *thisCell;
	PLOT *thisPlot;
	GRAPH *thisGraph;
	int *value;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get a pointer to the rasters structure */
	rasters = getRasters();

	/* get pointer to the window module that contains this widget */
	thisModule = ApGetInstance(widget);

	/* figure out this raster page */
	thisPage = getRasterPage(rasters->curPageIndx);
	thisCell = getRasterCell(thisPage->cellsInfo, thisPage->curCellIndx);
	thisPlot = &thisCell->plotInfo;
	thisGraph = &thisPlot->graph;
	
	/* get the widget's value and assign it to the appropriate variable */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, &value, 0);
	PtGetResources(widget, 1, args);

	/* figure out which number was entered */
	if(ApName(widget) == ABN_graphAxisNumberSize) {
		thisGraph->Nsz = *value;
		thisPage->graphVariables |= GRAPH_NSIZE;
	}
	else if(ApName(widget) == ABN_graphAxisLabelSize) {
		thisGraph->Lsz = *value;
		thisPage->graphVariables |= GRAPH_LSIZE;
	}

	/* set the button's label */
	setButtonLabel(thisModule, thisCell);

	return( Pt_CONTINUE );
}

int globalStartProc( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[1];
	RASTER *rasters;
	PAGE *thisPage;
	CELL *thisCell;
	PLOT *thisPlot;
	GRAPH *thisGraph;
	int *value;
	int i;
	int j;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get a pointer to the rasters structure */
	rasters = getRasters();

	/* get the widget's value */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, &value, 0);
	PtGetResources(widget, 1, args);

	for(i = 0; i < rasters->nPages; i++) {
		thisPage = getRasterPage(i);
		for(j = 0; j < thisPage->nDefCells; j++) {
			thisCell = getRasterCell(thisPage->cellsInfo, j);
			thisPlot = &thisCell->plotInfo;
			thisGraph = &thisPlot->graph;
			thisGraph->start = *value;
		}
	}

	return( Pt_CONTINUE );
}

int globalDurationProc( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[1];
	RASTER *rasters;
	PAGE *thisPage;
	CELL *thisCell;
	PLOT *thisPlot;
	GRAPH *thisGraph;
	int *value;
	int i;
	int j;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get a pointer to the rasters structure */
	rasters = getRasters();

	/* get the widget's value */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, &value, 0);
	PtGetResources(widget, 1, args);

	for(i = 0; i < rasters->nPages; i++) {
		thisPage = getRasterPage(i);
		for(j = 0; j < thisPage->nDefCells; j++) {
			thisCell = getRasterCell(thisPage->cellsInfo, j);
			thisPlot = &thisCell->plotInfo;
			thisGraph = &thisPlot->graph;
			thisGraph->interval = *value;
		}
	}

	return( Pt_CONTINUE );

}

int globalRateProc( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[1];
	RASTER *rasters;
	PAGE *thisPage;
	CELL *thisCell;
	PLOT *thisPlot;
	GRAPH *thisGraph;
	int *value;
	int i;
	int j;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get a pointer to the rasters structure */
	rasters = getRasters();

	/* get the widget's value */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, &value, 0);
	PtGetResources(widget, 1, args);

	for(i = 0; i < rasters->nPages; i++) {
		thisPage = getRasterPage(i);
		for(j = 0; j < thisPage->nDefCells; j++) {
			thisCell = getRasterCell(thisPage->cellsInfo, j);
			thisPlot = &thisCell->plotInfo;
			thisGraph = &thisPlot->graph;
			thisGraph->maxFreq = *value;
		}
	}

	return( Pt_CONTINUE );
}

int globalTicProc( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[1];
	RASTER *rasters;
	PAGE *thisPage;
	CELL *thisCell;
	PLOT *thisPlot;
	GRAPH *thisGraph;
	int *value;
	int i;
	int j;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get a pointer to the rasters structure */
	rasters = getRasters();

	/* get the widget's value */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, &value, 0);
	PtGetResources(widget, 1, args);

	for(i = 0; i < rasters->nPages; i++) {
		thisPage = getRasterPage(i);
		for(j = 0; j < thisPage->nDefCells; j++) {
			thisCell = getRasterCell(thisPage->cellsInfo, j);
			thisPlot = &thisCell->plotInfo;
			thisGraph = &thisPlot->graph;
			thisGraph->timeMark = *value;
		}
	}

	return( Pt_CONTINUE );
}

int globalGraphYaxisProc( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[1];
	RASTER *rasters;
	PAGE *thisPage;
	CELL *thisCell;
	PLOT *thisPlot;
	GRAPH *thisGraph;
	int *value;
	int i;
	int j;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get a pointer to the rasters structure */
	rasters = getRasters();

	/* get the widget's value */
	PtSetArg(&args[0], Pt_ARG_ONOFF_STATE, &value, 0);
	PtGetResources(widget, 1, args);

	for(i = 0; i < rasters->nPages; i++) {
		thisPage = getRasterPage(i);
		for(j = 0; j < thisPage->nDefCells; j++) {
			thisCell = getRasterCell(thisPage->cellsInfo, j);
			thisPlot = &thisCell->plotInfo;
			thisGraph = &thisPlot->graph;

			/* figure out which button was pressed */
			if(ApName(widget) == ABN_globalGraphAlignment) {
				if(*value) thisGraph->yAxes |= R_ALIGN;
				else thisGraph->yAxes &= ~R_ALIGN;
			}
			else {
				if(*value) thisGraph->yAxes |= R_LEFT;
				else thisGraph->yAxes &= ~R_LEFT;
			}
		}
	}

	return( Pt_CONTINUE );
}

int globalGraphNumberProc( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[1];
	RASTER *rasters;
	PAGE *thisPage;
	CELL *thisCell;
	PLOT *thisPlot;
	GRAPH *thisGraph;
	int *value;
	int i;
	int j;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get a pointer to the rasters structure */
	rasters = getRasters();

	/* get the widget's value */
	PtSetArg(&args[0], Pt_ARG_ONOFF_STATE, &value, 0);
	PtGetResources(widget, 1, args);

	for(i = 0; i < rasters->nPages; i++) {
		thisPage = getRasterPage(i);
		for(j = 0; j < thisPage->nDefCells; j++) {
			thisCell = getRasterCell(thisPage->cellsInfo, j);
			thisPlot = &thisCell->plotInfo;
			thisGraph = &thisPlot->graph;
			thisGraph->numberAxes = *value;
		}
	}

	return( Pt_CONTINUE );
}

int globalGraphAxisFormatProc( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[1];
	PtWidget_t *thisModule;
	PtWidget_t *otherButton;
	RASTER *rasters;
	PAGE *thisPage;
	CELL *thisCell;
	PLOT *thisPlot;
	GRAPH *thisGraph;
	long *flags;
	int i;
	int j;
	int Nformat;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get pointer to the window module that contains this widget */
	thisModule = ApGetInstance(widget);

	/* get a pointer to the rasters structure */
	rasters = getRasters();

	/* get the widget's value */
	PtSetArg(&args[0], Pt_ARG_FLAGS, &flags, 0);
	PtGetResources(widget, 1, args);

	if(*flags & Pt_SET) {
		/* figure out which button was pressed
		 * so the other one can be unset
		 */
		if(ApName(widget) == ABN_globalGraphAxisInt) {
			otherButton = ApGetWidgetPtr(thisModule, ABN_globalGraphAxisFloat);
			Nformat = 0;
		}
		else {
			otherButton = ApGetWidgetPtr(thisModule, ABN_globalGraphAxisInt);
			Nformat = 1;
		}

		/* make sure the other button is unset */
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(otherButton, 1, args);
	}
	else {
		/* figure out which button was unset
		 * so the other one can be set
		 */
		if(ApName(widget) == ABN_globalGraphAxisInt) {
			otherButton = ApGetWidgetPtr(thisModule, ABN_globalGraphAxisFloat);
			Nformat = 1;
		}
		else {
			otherButton = ApGetWidgetPtr(thisModule, ABN_globalGraphAxisInt);
			Nformat = 0;
		}

		/* make sure the other button is set */
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(otherButton, 1, args);
	}

	for(i = 0; i < rasters->nPages; i++) {
		thisPage = getRasterPage(i);
		for(j = 0; j < thisPage->nDefCells; j++) {
			thisCell = getRasterCell(thisPage->cellsInfo, j);
			thisPlot = &thisCell->plotInfo;
			thisGraph = &thisPlot->graph;
			thisGraph->Nformat = Nformat;
		}
	}

	return( Pt_CONTINUE );
}

int globalGraphAxisFontProc( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[1];
	PtWidget_t *thisModule;
	PtWidget_t *dfont;
	PtWidget_t *off1;
	PtWidget_t *off2;
	RASTER *rasters;
	PAGE *thisPage;
	CELL *thisCell;
	PLOT *thisPlot;
	GRAPH *thisGraph;
	void *userData;
	long *flags;
	int Lfont;
	int Nfont;
	int font;
	int i;
	int j;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	dfont = (PtWidget_t *)NULL;
	off1 = (PtWidget_t *)NULL;
	off2 = (PtWidget_t *)NULL;

	/* get pointer to the window module that contains this widget */
	thisModule = ApGetInstance(widget);

	/* get a pointer to the rasters structure */
	rasters = getRasters();

	/* figure out which button of the set was pressed
	 * so the other two can be unset
	 */
	Lfont = 0;
	Nfont = 0;
	if(ApName(widget) == ABN_globalGraphAxisLabelDuplex) {
		dfont = widget;
		off1 = ApGetWidgetPtr(thisModule, ABN_globalGraphAxisLabelComplex);
		off2 = ApGetWidgetPtr(thisModule, ABN_globalGraphAxisLabelItalic);
		Lfont = 1;
	}
	else if(ApName(widget) == ABN_globalGraphAxisLabelComplex) {
		dfont = ApGetWidgetPtr(thisModule, ABN_globalGraphAxisLabelDuplex);
		off1 = ApGetWidgetPtr(thisModule, ABN_globalGraphAxisLabelDuplex);
		off2 = ApGetWidgetPtr(thisModule, ABN_globalGraphAxisLabelItalic);
		Lfont = 1;
	}
	else if(ApName(widget) == ABN_globalGraphAxisLabelItalic) {
		dfont = ApGetWidgetPtr(thisModule, ABN_globalGraphAxisLabelDuplex);
		off1 = ApGetWidgetPtr(thisModule, ABN_globalGraphAxisLabelDuplex);
		off2 = ApGetWidgetPtr(thisModule, ABN_globalGraphAxisLabelComplex);
		Lfont = 1;
	}
	else if(ApName(widget) == ABN_globalGraphAxisNumberDuplex) {
		dfont = widget;
		off1 = ApGetWidgetPtr(thisModule, ABN_globalGraphAxisNumberComplex);
		off2 = ApGetWidgetPtr(thisModule, ABN_globalGraphAxisNumberItalic);
		Nfont = 1;
	}
	else if(ApName(widget) == ABN_globalGraphAxisNumberComplex) {
		dfont = ApGetWidgetPtr(thisModule, ABN_globalGraphAxisNumberDuplex);
		off1 = ApGetWidgetPtr(thisModule, ABN_globalGraphAxisNumberDuplex);
		off2 = ApGetWidgetPtr(thisModule, ABN_globalGraphAxisNumberItalic);
		Nfont = 1;
	}
	else if(ApName(widget) == ABN_globalGraphAxisNumberItalic) {
		dfont = ApGetWidgetPtr(thisModule, ABN_globalGraphAxisNumberDuplex);
		off1 = ApGetWidgetPtr(thisModule, ABN_globalGraphAxisNumberDuplex);
		off2 = ApGetWidgetPtr(thisModule, ABN_globalGraphAxisNumberComplex);
		Nfont = 1;
	}

	/* get the widget's value */
	PtSetArg(&args[0], Pt_ARG_FLAGS, &flags, 0);
	PtGetResources(widget, 1, args);

	/* if this widget's button was pressed,
	 * make sure that the other two widgets' buttons are unpressed
	 */
	if(*flags & Pt_SET) {
		/* get the font */
		PtSetArg(&args[0], Pt_ARG_USER_DATA, &userData, 0);
		PtGetResources(widget, 1, args);
		sscanf(userData, "%d", &font);
		
		/* unset the other two buttons */
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(off1, 1, args);
		PtSetResources(off2, 1, args);
	}
	
	/* if this widget's button was unpressed,
	 * set the default widget's button
	 */
	 else {
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(dfont, 1, args);

		PtSetArg(&args[0], Pt_ARG_USER_DATA, &userData, 0);
		PtGetResources(dfont, 1, args);
		sscanf(userData, "%d", &font);
	 }

	for(i = 0; i < rasters->nPages; i++) {
		thisPage = getRasterPage(i);
		for(j = 0; j < thisPage->nDefCells; j++) {
			thisCell = getRasterCell(thisPage->cellsInfo, j);
			thisPlot = &thisCell->plotInfo;
			thisGraph = &thisPlot->graph;
			if(Lfont == 1) thisGraph->Lfont = font;
			else if(Nfont == 1) thisGraph->Nfont = font;
		}
	}

	return( Pt_CONTINUE );
}

int globalGraphSizeProc( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[1];
	RASTER *rasters;
	PAGE *thisPage;
	CELL *thisCell;
	PLOT *thisPlot;
	GRAPH *thisGraph;
	int *value;
	int Lsiz;
	int Nsiz;
	int i;
	int j;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get a pointer to the rasters structure */
	rasters = getRasters();

	/* get the widget's value */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, &value, 0);
	PtGetResources(widget, 1, args);

	/* figure out which number was entered */
	Lsiz = 0;
	Nsiz = 0;
	if(ApName(widget) == ABN_globalGraphAxisNumberSize) {
		Nsiz = 1;
	}
	else if(ApName(widget) == ABN_globalGraphAxisLabelSize) {
		Lsiz = 1;
	}

	for(i = 0; i < rasters->nPages; i++) {
		thisPage = getRasterPage(i);
		for(j = 0; j < thisPage->nDefCells; j++) {
			thisCell = getRasterCell(thisPage->cellsInfo, j);
			thisPlot = &thisCell->plotInfo;
			thisGraph = &thisPlot->graph;

			if(Nsiz == 1) {
				thisGraph->Nsz = *value;
			}
			else if(Lsiz == 1) {
				thisGraph->Lsz = *value;
			}
		}
	}

	return( Pt_CONTINUE );
}

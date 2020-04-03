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


int cellSetProc(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
	PtArg_t args[1];
	PtWidget_t *thisModule;
	RASTER *rasters;
	PAGE *thisPage;
	CELL *thisCell;
	PLOT *thisPlot;
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
	
	/* get the widget's value and assign it to the appropriate variable */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, &value, 0);
	PtGetResources(widget, 1, args);
	thisPlot->setNum = *value;

	/* set the button's label */
	setButtonLabel(thisModule, thisCell);

	thisPage->plotVariables |= PLOT_SET;

	return( Pt_CONTINUE );
}

int cellEcodeProc(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
	PtArg_t args[1];
	PtWidget_t *thisModule;
	RASTER *rasters;
	PAGE *thisPage;
	CELL *thisCell;
	PLOT *thisPlot;
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
	
	/* get the widget's value and assign it to the appropriate variable */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, &value, 0);
	PtGetResources(widget, 1, args);
	thisPlot->cCode = *value;

	setButtonLabel(thisModule, thisCell);
	
	thisPage->plotVariables |= PLOT_ECODE;

	return( Pt_CONTINUE );
}

int cellSigmaProc(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
	PtArg_t args[1];
	PtWidget_t *thisModule;
	PAGE *thisPage;
	RASTER *rasters;
	CELL *thisCell;
	PLOT *thisPlot;
	double *value;

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
	
	/* get the widget's value and assign it to the appropriate variable */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, &value, 0);
	PtGetResources(widget, 1, args);
	thisPlot->sigma = (float)*value;

	setButtonLabel(thisModule, thisCell);
	
	thisPage->plotVariables |= PLOT_SIGMA;

	return( Pt_CONTINUE );
}

int cellRasterProc(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
	PtArg_t args[1];
	PtWidget_t *thisModule;
	PAGE *thisPage;
	RASTER *rasters;
	CELL *thisCell;
	PLOT *thisPlot;
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
	
	/* get the flag variable for this widget */
	PtSetArg(&args[0], Pt_ARG_FLAGS, &flags, 0);
	PtGetResources(widget, 1, args);

	/* determine whether or not the button is pressed
	 * and set the raster variable accordingly
	 */
	if(*flags & Pt_SET) thisPlot->trialFlag = 1;
	else thisPlot->trialFlag = 0;

	setButtonLabel(thisModule, thisCell);
	
	thisPage->plotVariables |= PLOT_RASTER;

	return( Pt_CONTINUE );
}

int cellDensityProc(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
	PtArg_t args[1];
	PtWidget_t *thisModule;
	PAGE *thisPage;
	RASTER *rasters;
	CELL *thisCell;
	PLOT *thisPlot;
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
	
	/* get the flag variable for this widget */
	PtSetArg(&args[0], Pt_ARG_FLAGS, &flags, 0);
	PtGetResources(widget, 1, args);

	/* determine whether or not the button is pressed
	 * and set the raster variable accordingly
	 */
	if(*flags & Pt_SET) thisPlot->meanFlag = 1;
	else thisPlot->meanFlag = 0;

	setButtonLabel(thisModule, thisCell);
	
	thisPage->plotVariables |= PLOT_DENSITY;

	return( Pt_CONTINUE );
}

int cellUnitsProc(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
	PtArg_t args[1];
	PtWidget_t *thisModule;
	PAGE *thisPage;
	RASTER *rasters;
	CELL *thisCell;
	PLOT *thisPlot;
	char *string;
	char *tok;
	char stringCopy[1024];

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
	
	/* get the list of all of units */
	PtSetArg(&args[0], Pt_ARG_TEXT_STRING, &string, 0);
	PtGetResources(widget, 1, args);

	/* make a copy of the string */
	strcpy(stringCopy, string);

	thisPlot->nSignals = 0;
	tok = strtok(stringCopy, "\n");
	if(tok) {
		strcpy(thisPlot->sigInfo[thisPlot->nSignals].sigName, tok);
		thisPlot->nSignals++;
	}
	while(tok = strtok(NULL, "\n")) {
		strcpy(thisPlot->sigInfo[thisPlot->nSignals].sigName, tok);
		thisPlot->nSignals++;
		if(thisPlot->nSignals >= 10) break;
	}

	setButtonLabel(thisModule, thisCell);
	
	thisPage->plotVariables |= PLOT_UNITS;

	return( Pt_CONTINUE );
}

int plotMarkEcodeProc(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
	PtArg_t args[1];
	PtWidget_t *thisModule;
	RASTER *rasters;
	PAGE *thisPage;
	CELL *thisCell;
	PLOT *thisPlot;
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
	
	/* get the widget's value and assign it to the appropriate variable */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, &value, 0);
	PtGetResources(widget, 1, args);
	thisPlot->ecodeMark.rMark = *value;
	if(thisPlot->ecodeMark.rMark) thisPlot->markFlag = 1;
	else thisPlot->markFlag = 0;

	setButtonLabel(thisModule, thisCell);
	
	thisPage->plotVariables |= PLOT_MARK;

	return( Pt_CONTINUE );
}

int plotMarkSymbolProc(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
	PtArg_t args[1];
	PtWidget_t *thisModule;
	RASTER *rasters;
	PAGE *thisPage;
	CELL *thisCell;
	PLOT *thisPlot;
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
	
	/* the widget's value and assign it to the symbol text variable */
	PtSetArg(&args[0], Pt_ARG_TEXT_STRING, &string, 0);
	PtGetResources(widget, 1, args);
	if((strlen(string) > 0) && (strlen(string) < 32)) {
		thisPlot->markFlag = 1;
		strcpy(thisPlot->ecodeMark.symbolString, string);
		thisPlot->ecodeMark.symbolString[31] = 0;	/* force NULL terminator */
	}
	else {
		thisPlot->markFlag = 0;
		strcpy(thisPlot->ecodeMark.symbolString, "");
	}
	thisPlot->ecodeMark.symbolChar = decodeSymbolString(string);

	setButtonLabel(thisModule, thisCell);
	
	thisPage->plotVariables |= PLOT_SYMBOL;

	return( Pt_CONTINUE );
}

int plotMarkSizeProc(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
	PtArg_t args[1];
	PtWidget_t *thisModule;
	RASTER *rasters;
	PAGE *thisPage;
	CELL *thisCell;
	PLOT *thisPlot;
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
	
	/* get the widget's value and assign it to the appropriate variable */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, &value, 0);
	PtGetResources(widget, 1, args);
	thisPlot->ecodeMark.sz = *value;

	setButtonLabel(thisModule, thisCell);
	
	thisPage->plotVariables |= PLOT_MSIZE;

	return( Pt_CONTINUE );
}

int plotLabelTextProc(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
	PtArg_t args[1];
	PtWidget_t *thisModule;
	RASTER *rasters;
	PAGE *thisPage;
	CELL *thisCell;
	PLOT *thisPlot;
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
	
	/* the widget's value and assign it to the page label text variable */
	PtSetArg(&args[0], Pt_ARG_TEXT_STRING, &string, 0);
	PtGetResources(widget, 1, args);
	if((strlen(string) > 0) && (strlen(string) < 128)) {
		thisPlot->labelFlag = 1;
		strcpy(thisPlot->plotLabel.label, string);
		thisPlot->plotLabel.label[127] = 0;	/* force NULL terminator */
	}
	else {
		thisPlot->labelFlag = 0;
		strcpy(thisPlot->plotLabel.label, "");
	}

	setButtonLabel(thisModule, thisCell);
	
	thisPage->plotVariables |= PLOT_LABEL;

	return( Pt_CONTINUE );
}

int plotLabelFontProc(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
	PtWidget_t *thisModule;
	PtWidget_t *off1;
	PtWidget_t *off2;
	PtArg_t args[1];
	RASTER *rasters;
	PAGE *thisPage;
	CELL *thisCell;
	PLOT *thisPlot;
	void *userData;
	long *flags;
	int font;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	off1 = (PtWidget_t *)NULL;
	off2 = (PtWidget_t *)NULL;
	
	/* get a pointer to the rasters structure */
	rasters = getRasters();

	/* get pointer to the window module that contains this widget */
	thisModule = ApGetInstance(widget);

	/* figure out this raster page */
	thisPage = getRasterPage(rasters->curPageIndx);
	thisCell = getRasterCell(thisPage->cellsInfo, thisPage->curCellIndx);
	thisPlot = &thisCell->plotInfo;
	
	/* get the flag variable for this widget */
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
		
		/* figure out which button was pressed
		 * so the other two can be unset
		 */
		if(ApName(widget) == ABN_plotLabelDuplex) {
			off1 = ApGetWidgetPtr(thisModule, ABN_plotLabelComplex);
			off2 = ApGetWidgetPtr(thisModule, ABN_plotLabelItalic);
		}
		else if(ApName(widget) == ABN_plotLabelComplex) {
			off1 = ApGetWidgetPtr(thisModule, ABN_plotLabelDuplex);
			off2 = ApGetWidgetPtr(thisModule, ABN_plotLabelItalic);
		}
		else if(ApName(widget) == ABN_plotLabelItalic) {
			off1 = ApGetWidgetPtr(thisModule, ABN_plotLabelDuplex);
			off2 = ApGetWidgetPtr(thisModule, ABN_plotLabelComplex);
		}
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(off1, 1, args);
		PtSetResources(off2, 1, args);
	}

	/* if this widget's button was unpressed,
	 * set the default widget's button
	 */
	 else {
		off1 = ApGetWidgetPtr(thisModule, ABN_plotLabelDuplex);
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(off1, 1, args);

		PtSetArg(&args[0], Pt_ARG_USER_DATA, &userData, 0);
		PtGetResources(off1, 1, args);
		sscanf(userData, "%d", &font);
	 }
	 
	thisPlot->plotLabel.font = font;

	setButtonLabel(thisModule, thisCell);
	
	thisPage->plotVariables |= PLOT_LFONT;

	return( Pt_CONTINUE );
}

int plotLabelSizeProc(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
	PtArg_t args[1];
	PtWidget_t *thisModule;
	RASTER *rasters;
	PAGE *thisPage;
	CELL *thisCell;
	PLOT *thisPlot;
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
	
	/* get the widget's value and assign it to the appropriate variable */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, &value, 0);
	PtGetResources(widget, 1, args);
	thisPlot->plotLabel.sz = *value;

	setButtonLabel(thisModule, thisCell);
	
	thisPage->plotVariables |= PLOT_LSIZE;

	return( Pt_CONTINUE );
}

int plotLabelPositionProc(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
	PtArg_t args[1];
	PtWidget_t *thisModule;
	RASTER *rasters;
	PAGE *thisPage;
	CELL *thisCell;
	PLOT *thisPlot;
	double *value;

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
	
	/* get the widget's value and assign it to the appropriate variable */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, &value, 0);
	PtGetResources(widget, 1, args);
	if(ApName(widget) == ABN_plotLabelLeft) thisPlot->plotLabel.x = *value;
	else if(ApName(widget) == ABN_plotLabelBottom) thisPlot->plotLabel.y = *value;

	setButtonLabel(thisModule, thisCell);
	
	thisPage->plotVariables |= PLOT_LPOS;

	return( Pt_CONTINUE );
}

void initPlotWidgets(PtWidget_t *thisModule, PLOT *thisPlot)
{
	PtWidget_t *widget;
	PtArg_t args[1];
	SIGNAL *thisSignal;
	int i;
	char string[1024];
	
	/* set the values on the set and ecode widgets */
	widget = ApGetWidgetPtr(thisModule, ABN_cellSet);
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, thisPlot->setNum, 0);
	PtSetResources(widget, 1, args);

	widget = ApGetWidgetPtr(thisModule, ABN_cellEcode);
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, thisPlot->cCode, 0);
	PtSetResources(widget, 1, args);

	/* set the value on the sigma widget */
	widget = ApGetWidgetPtr(thisModule, ABN_cellSigma);
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, &thisPlot->sigma, 0);
	PtSetResources(widget, 1, args);

	/* set the value on the raster toggle button */
	widget = ApGetWidgetPtr(thisModule, ABN_cellRaster);
	if(thisPlot->trialFlag) PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
	else PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
	PtSetResources(widget, 1, args);
	
	/* set the value on the density toggle button */
	widget = ApGetWidgetPtr(thisModule, ABN_cellDensity);
	if(thisPlot->meanFlag) PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
	else PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
	PtSetResources(widget, 1, args);

	/* set the values of the units */
	thisSignal = &thisPlot->sigInfo[0];
	if(strlen(thisSignal->sigName)) {
		strcpy(string, thisSignal->sigName);
		strcat(string, "\n");
	}
	for(i = 1; i < thisPlot->nSignals; i++) {
		thisSignal = &thisPlot->sigInfo[i];
		if(strlen(thisSignal->sigName)) {
			strcat(string, thisSignal->sigName);
			strcat(string, "\n");
		}
	}
	widget = ApGetWidgetPtr(thisModule, ABN_cellUnits);
	PtSetArg(&args[0], Pt_ARG_TEXT_STRING, &string, 0);
	PtSetResources(widget, 1, args);

	/* set the value of the ecode mark */
	widget = ApGetWidgetPtr(thisModule, ABN_plotMarkEcode);
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, thisPlot->ecodeMark.rMark, 0);
	PtSetResources(widget, 1, args);

	/* set the value of the mark symbol */
	widget = ApGetWidgetPtr(thisModule, ABN_plotMarkSymbol);
	PtSetArg(&args[0], Pt_ARG_TEXT_STRING, &thisPlot->ecodeMark.symbolString, 0);
	PtSetResources(widget, 1, args);

	/* set the value of the mark size */
	widget = ApGetWidgetPtr(thisModule, ABN_plotMarkSize);
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, thisPlot->ecodeMark.sz, 0);
	PtSetResources(widget, 1, args);

	/* set the value of the plot label */
	widget = ApGetWidgetPtr(thisModule, ABN_plotLabelText);
	PtSetArg(&args[0], Pt_ARG_TEXT_STRING, thisPlot->plotLabel.label, 0);
	PtSetResources(widget, 1, args);

	/* set the plot label font widgets */
	switch(thisPlot->plotLabel.font) {
	case 12:
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		widget = ApGetWidgetPtr(thisModule, ABN_plotLabelDuplex);
		PtSetResources(widget, 1, args);
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		widget = ApGetWidgetPtr(thisModule, ABN_plotLabelComplex);
		PtSetResources(widget, 1, args);
		widget = ApGetWidgetPtr(thisModule, ABN_plotLabelItalic);
		PtSetResources(widget, 1, args);
		break;
	case 13:
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		widget = ApGetWidgetPtr(thisModule, ABN_plotLabelComplex);
		PtSetResources(widget, 1, args);
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		widget = ApGetWidgetPtr(thisModule, ABN_plotLabelDuplex);
		PtSetResources(widget, 1, args);
		widget = ApGetWidgetPtr(thisModule, ABN_plotLabelItalic);
		PtSetResources(widget, 1, args);
		break;
	case 15:
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		widget = ApGetWidgetPtr(thisModule, ABN_plotLabelItalic);
		PtSetResources(widget, 1, args);
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		widget = ApGetWidgetPtr(thisModule, ABN_plotLabelComplex);
		PtSetResources(widget, 1, args);
		widget = ApGetWidgetPtr(thisModule, ABN_plotLabelDuplex);
		PtSetResources(widget, 1, args);
		break;
	default:
		fprintf(stderr, "font %d invalid\n", thisPlot->plotLabel.font);
	}

	/* set the plot label size widget */
	widget = ApGetWidgetPtr(thisModule, ABN_plotLabelSize);
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, thisPlot->plotLabel.sz, 0);
	PtSetResources(widget, 1, args);
	
	/* set the plot label position widgets */
	widget = ApGetWidgetPtr(thisModule, ABN_plotLabelLeft);
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, &thisPlot->plotLabel.x, 0);
	PtSetResources(widget, 1, args);
	widget = ApGetWidgetPtr(thisModule, ABN_plotLabelBottom);
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, &thisPlot->plotLabel.y, 0);
	PtSetResources(widget, 1, args);
	
	return;
}

void setButtonLabel(PtWidget_t *thisModule, CELL *thisCell)
{
	PtWidget_t *thisButton;
	PtArg_t args[1];
	PLOT *thisPlot;
	int cellButtonName;
	char string[32];

	/* get pointer to this cell's plot info structure */
	thisPlot = &thisCell->plotInfo;

	/* figure out this cell button and change its text */
	cellButtonName = getCellButtonName(thisCell->RCindex);
	if(cellButtonName) {
		thisButton = ApGetWidgetPtr(thisModule, cellButtonName);
		sprintf(string, "%d:%d", thisPlot->setNum,  thisPlot->cCode);
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, &string, 0);
		PtSetResources(thisButton, 1, args);
	}
	
}

char decodeSymbolString(char *symbolString)
{
	char symbolChar;

	/* figure out what symbol the user entered */
	
	/* if the user entered a multi character string,
	 * figure out what symbol to use
	 */
	if(!strcmp(symbolString, "hourglass")) symbolChar = (char)NULL;
	else if(!strcmp(symbolString, "triangle")) symbolChar = 8;
	else if(!strcmp(symbolString, "diamond")) symbolChar = 9;
	else if(!strcmp(symbolString, "circle")) symbolChar = 10;
	else if(!strcmp(symbolString, "square")) symbolChar = 32;

	/* if the user entered a single character
	 * just use the character as a symbol
	 */
	else symbolChar = symbolString[0];
	return(symbolChar);
}

int globalCellUnitsProc( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[1];
	PAGE *thisPage;
	RASTER *rasters;
	CELL *thisCell;
	PLOT *thisPlot;
	int i;
	int j;
	char *string;
	char *tok;
	char stringCopy[1024];

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get a pointer to the rasters structure */
	rasters = getRasters();

	/* get the list of all of units */
	PtSetArg(&args[0], Pt_ARG_TEXT_STRING, &string, 0);
	PtGetResources(widget, 1, args);

	for(i = 0; i < rasters->nPages; i++) {
		thisPage = getRasterPage(i);
		for(j = 0; j < thisPage->nDefCells; j++) {
			thisCell = getRasterCell(thisPage->cellsInfo, j);
			thisPlot = &thisCell->plotInfo;
			thisPlot->nSignals = 0;

			/* make a copy of the string for strtok to mangle*/
			strcpy(stringCopy, string);
			tok = strtok(stringCopy, "\n");
			if(tok) {
				strcpy(thisPlot->sigInfo[thisPlot->nSignals].sigName, tok);
				thisPlot->nSignals++;
			}
			while(tok = strtok(NULL, "\n")) {
				strcpy(thisPlot->sigInfo[thisPlot->nSignals].sigName, tok);
				thisPlot->nSignals++;
				if(thisPlot->nSignals >= 10) break;
			}
		}
	}

	return( Pt_CONTINUE );
}

int globalCellRasterProc( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[1];
	PAGE *thisPage;
	RASTER *rasters;
	CELL *thisCell;
	PLOT *thisPlot;
	long *flags;
	int i;
	int j;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get a pointer to the rasters structure */
	rasters = getRasters();

	/* get the flag variable for this widget */
	PtSetArg(&args[0], Pt_ARG_FLAGS, &flags, 0);
	PtGetResources(widget, 1, args);

	for(i = 0; i < rasters->nPages; i++) {
		thisPage = getRasterPage(i);
		for(j = 0; j < thisPage->nDefCells; j++) {
			thisCell = getRasterCell(thisPage->cellsInfo, j);
			thisPlot = &thisCell->plotInfo;

			/* determine whether or not the button is pressed
			 * and set the raster variable accordingly
			 */
			if(*flags & Pt_SET) thisPlot->trialFlag = 1;
			else thisPlot->trialFlag = 0;
		}
	}

	return( Pt_CONTINUE );
}

int globalCellDensityProc( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[1];
	PAGE *thisPage;
	RASTER *rasters;
	CELL *thisCell;
	PLOT *thisPlot;
	long *flags;
	int i;
	int j;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get a pointer to the rasters structure */
	rasters = getRasters();

	/* get the flag variable for this widget */
	PtSetArg(&args[0], Pt_ARG_FLAGS, &flags, 0);
	PtGetResources(widget, 1, args);

	for(i = 0; i < rasters->nPages; i++) {
		thisPage = getRasterPage(i);
		for(j = 0; j < thisPage->nDefCells; j++) {
			thisCell = getRasterCell(thisPage->cellsInfo, j);
			thisPlot = &thisCell->plotInfo;

			/* determine whether or not the button is pressed
			 * and set the raster variable accordingly
			 */
			if(*flags & Pt_SET) thisPlot->meanFlag = 1;
			else thisPlot->meanFlag = 0;
		}
	}

	return( Pt_CONTINUE );
}

int globalPlotMarkEcodeProc( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[1];
	PAGE *thisPage;
	RASTER *rasters;
	CELL *thisCell;
	PLOT *thisPlot;
	int *value;
	int i;
	int j;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get a pointer to the rasters structure */
	rasters = getRasters();

	/* get the flag variable for this widget */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, &value, 0);
	PtGetResources(widget, 1, args);

	for(i = 0; i < rasters->nPages; i++) {
		thisPage = getRasterPage(i);
		for(j = 0; j < thisPage->nDefCells; j++) {
			thisCell = getRasterCell(thisPage->cellsInfo, j);
			thisPlot = &thisCell->plotInfo;
			
			thisPlot->ecodeMark.rMark = *value;
			if(thisPlot->ecodeMark.rMark) thisPlot->markFlag = 1;
			else thisPlot->markFlag = 0;
		}
	}

	return( Pt_CONTINUE );
}

int globalPlotMarkSymbolProc( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[1];
	PAGE *thisPage;
	RASTER *rasters;
	CELL *thisCell;
	PLOT *thisPlot;
	char *string;
	char symbolChar;
	int markFlag;
	int i;
	int j;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get a pointer to the rasters structure */
	rasters = getRasters();

	/* get the text from this widget */
	PtSetArg(&args[0], Pt_ARG_TEXT_STRING, &string, 0);
	PtGetResources(widget, 1, args);

	if((strlen(string) > 0) && (strlen(string) < 32)) {
		markFlag = 1;
	}
	else {
		markFlag = 0;
	}
	symbolChar = decodeSymbolString(string);

	for(i = 0; i < rasters->nPages; i++) {
		thisPage = getRasterPage(i);
		for(j = 0; j < thisPage->nDefCells; j++) {
			thisCell = getRasterCell(thisPage->cellsInfo, j);
			thisPlot = &thisCell->plotInfo;

			thisPlot->markFlag = markFlag;
			if(markFlag == 1) {
				strcpy(thisPlot->ecodeMark.symbolString, string);
				thisPlot->ecodeMark.symbolString[31] = 0;	/* force NULL terminator */
			}
			else {
				strcpy(thisPlot->ecodeMark.symbolString, "");
			}

			thisPlot->ecodeMark.symbolChar = symbolChar;
		}
	}

	return( Pt_CONTINUE );
}

int globalPlotMarkSizeProc( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[1];
	PAGE *thisPage;
	RASTER *rasters;
	CELL *thisCell;
	PLOT *thisPlot;
	int *value;
	int i;
	int j;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get a pointer to the rasters structure */
	rasters = getRasters();

	/* get the value from this widget */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, &value, 0);
	PtGetResources(widget, 1, args);

	for(i = 0; i < rasters->nPages; i++) {
		thisPage = getRasterPage(i);
		for(j = 0; j < thisPage->nDefCells; j++) {
			thisCell = getRasterCell(thisPage->cellsInfo, j);
			thisPlot = &thisCell->plotInfo;

			thisPlot->ecodeMark.sz = *value;
		}
	}

	return( Pt_CONTINUE );
}

int globalPlotLabelFontProc( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtWidget_t *thisModule;
	PtWidget_t *off1;
	PtWidget_t *off2;
	PtArg_t args[1];
	PAGE *thisPage;
	RASTER *rasters;
	CELL *thisCell;
	PLOT *thisPlot;
	void *userData;
	long *flags;
	int font;
	int i;
	int j;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	off1 = (PtWidget_t *)NULL;
	off2 = (PtWidget_t *)NULL;
	
	/* get a pointer to the rasters structure */
	rasters = getRasters();

	/* get pointer to the window module that contains this widget */
	thisModule = ApGetInstance(widget);

	/* get the value from this widget */
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
		
		/* figure out which button was pressed
		 * so the other two can be unset
		 */
		if(ApName(widget) == ABN_globalPlotLabelDuplex) {
			off1 = ApGetWidgetPtr(thisModule, ABN_globalPlotLabelComplex);
			off2 = ApGetWidgetPtr(thisModule, ABN_globalPlotLabelItalic);
		}
		else if(ApName(widget) == ABN_globalPlotLabelComplex) {
			off1 = ApGetWidgetPtr(thisModule, ABN_globalPlotLabelDuplex);
			off2 = ApGetWidgetPtr(thisModule, ABN_globalPlotLabelItalic);
		}
		else if(ApName(widget) == ABN_globalPlotLabelItalic) {
			off1 = ApGetWidgetPtr(thisModule, ABN_globalPlotLabelDuplex);
			off2 = ApGetWidgetPtr(thisModule, ABN_globalPlotLabelComplex);
		}
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(off1, 1, args);
		PtSetResources(off2, 1, args);
	}

	/* if this widget's button was unpressed,
	 * set the default widget's button
	 */
	 else {
		off1 = ApGetWidgetPtr(thisModule, ABN_globalPlotLabelDuplex);
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(off1, 1, args);

		PtSetArg(&args[0], Pt_ARG_USER_DATA, &userData, 0);
		PtGetResources(off1, 1, args);
		sscanf(userData, "%d", &font);
	 }
	 
	for(i = 0; i < rasters->nPages; i++) {
		thisPage = getRasterPage(i);
		for(j = 0; j < thisPage->nDefCells; j++) {
			thisCell = getRasterCell(thisPage->cellsInfo, j);
			thisPlot = &thisCell->plotInfo;

			thisPlot->plotLabel.font = font;
		}
	}

	return( Pt_CONTINUE );
}

int globalPlotLabelPositionProc( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[1];
	PAGE *thisPage;
	RASTER *rasters;
	CELL *thisCell;
	PLOT *thisPlot;
	double *value;
	int x;
	int y;
	int i;
	int j;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get a pointer to the rasters structure */
	rasters = getRasters();

	/* get the value from this widget */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, &value, 0);
	PtGetResources(widget, 1, args);

	x = 0;
	y = 0;
	if(ApName(widget) == ABN_globalPlotLabelLeft) x = 1;
	else if(ApName(widget) == ABN_globalPlotLabelBottom) y = 1;

	for(i = 0; i < rasters->nPages; i++) {
		thisPage = getRasterPage(i);
		for(j = 0; j < thisPage->nDefCells; j++) {
			thisCell = getRasterCell(thisPage->cellsInfo, j);
			thisPlot = &thisCell->plotInfo;

			if(x == 1) thisPlot->plotLabel.x = *value;
			else if(y == 1) thisPlot->plotLabel.y = *value;
		}
	}

	return( Pt_CONTINUE );
}

int globalPlotLabelSizeProc( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[1];
	PAGE *thisPage;
	RASTER *rasters;
	CELL *thisCell;
	PLOT *thisPlot;
	int *value;
	int i;
	int j;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get a pointer to the rasters structure */
	rasters = getRasters();

	/* get the value from this widget */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, &value, 0);
	PtGetResources(widget, 1, args);

	for(i = 0; i < rasters->nPages; i++) {
		thisPage = getRasterPage(i);
		for(j = 0; j < thisPage->nDefCells; j++) {
			thisCell = getRasterCell(thisPage->cellsInfo, j);
			thisPlot = &thisCell->plotInfo;

			thisPlot->plotLabel.sz = *value;
		}
	}

	return( Pt_CONTINUE );
}

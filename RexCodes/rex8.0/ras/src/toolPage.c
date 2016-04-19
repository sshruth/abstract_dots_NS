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

int tigColorList[] = { BLACK, RED, GREEN, BLUE, ORANGE, VIOLET, PINK, YELLOW, CYAN, BROWN };

int buildPage( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[1];
	RASTER *rasters;
	PAGE *thisPage;
	char string[64];

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	/* get a pointer to the rasters structure */
	rasters = getRasters();

	/* set the current page index  to 0 */
	rasters->curPageIndx = 0;;
	
	/* create a window label */
	sprintf(string, "Raster Page %d", rasters->curPageIndx + 1);
	PtSetArg(&args[0], Pt_ARG_WINDOW_TITLE, string, 0);
	PtSetResources(link_instance, 1, args);

	/* allocate space for a new page */
	/* this condition will be true if the user is defining rasters
	 * for the first time
	 */
	if(!rasters->nPages) {
		rasters->nPages++;
		rasters->pages = addRasterPage(rasters->pages, rasters->nPages);

		/* set the newRasters flag because a new page has been added */
		rasters->newRasters = 1;

		/* initialize the new page with some reasonable default values */
		thisPage = getRasterPage(rasters->curPageIndx);
		initFirstPage(thisPage);
		
		/* set the widget values on the new page */
		initPageWidgets(link_instance, thisPage, rasters->newRasters);
	}

	/* if the user is re-displaying the rasters page or
	 * the raster definitions have been read from a root
	 * get a pointer to the first page and initialize the
	 * widgets to the values of that page
	 */
	else {
		thisPage = getRasterPage(rasters->curPageIndx);

		/* unset the newRasters flag */
		rasters->newRasters = 0;

		/* set the widget values on the new page */
		initPageWidgets(link_instance, thisPage, rasters->newRasters);
	}

	/* set the maximum value on the page scroll bar */
	PtSetArg(&args[0], Pt_ARG_MAXIMUM, rasters->nPages - 1, 0);
	PtSetResources(ABW_pageScrollBar, 1, args);
	
	return( Pt_CONTINUE );
}

int newPage(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
	PtArg_t args[2];
	RASTER *rasters;
	PAGE *thisPage;
	PAGE *lastPage;
	char string[64];

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get a pointer to the rasters structure */
	rasters = getRasters();

	/* increment the current page index */
	rasters->curPageIndx++;
	
	/* create a window label */
	sprintf(string, "Raster Page %d", rasters->curPageIndx + 1);
	PtSetArg(&args[0], Pt_ARG_WINDOW_TITLE, string, 0);
	PtSetResources(ABW_rastersPage, 1, args);

	/* allocate space for a new page */
	rasters->nPages++;
	rasters->pages = addRasterPage(rasters->pages, rasters->nPages);

	/* set the newRasters flag because a new page has been added */
	rasters->newRasters = 1;
	
	/* initialize the new page with the values from the previous page */
	thisPage = getRasterPage(rasters->curPageIndx);
	lastPage = getRasterPage(rasters->curPageIndx - 1);
	initSubsequentPage(thisPage, lastPage);

	/* set the widget values on the new page */
	initPageWidgets(ABW_rastersPage, thisPage, rasters->newRasters);
	
	/* set the maximum value on the page scroll bar */
	PtSetArg(&args[0], Pt_ARG_MAXIMUM, rasters->nPages - 1, 0);
	PtSetArg(&args[1], Pt_ARG_SCROLL_POSITION, rasters->nPages - 1, 0);
	PtSetResources(ABW_pageScrollBar, 2, args);
	
	return( Pt_CONTINUE );
}

int showRasterPage( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[1];
	PtScrollbarCallback_t *scroll = cbinfo->cbdata;
	RASTER *rasters;
	PAGE *thisPage;
	char string[64];

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get a pointer to the rasters structure */
	rasters = getRasters();

	/* get the current page index from the scroll bar */
	rasters->curPageIndx = scroll->position;
	
	/* get pointer to the current raster page */
	thisPage = getRasterPage(rasters->curPageIndx);

	/* create a window label */
	sprintf(string, "Raster Page %d", rasters->curPageIndx + 1);
	PtSetArg(&args[0], Pt_ARG_WINDOW_TITLE, string, 0);
	PtSetResources(ABW_rastersPage, 1, args);

	/* unset the newRasters flag */
	rasters->newRasters = 0;

	/* set the widget values on the page being shown */
	initPageWidgets(ABW_rastersPage, thisPage, rasters->newRasters);

	return( Pt_CONTINUE );
}

int resetButtons(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
	PtArg_t args[1];
	PtWidget_t *thisModule;
	PtWidget_t *cellButton;
	int cellButtonName;
	int i;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the module that contains this widget */
	thisModule = ApGetInstance(widget);

	/* reset the fill color of all buttons to grey */
	PtSetArg(&args[0], Pt_ARG_FILL_COLOR, Pg_GREY, 0);
	for(i = 0; i < 64; i++) {
		cellButtonName = getCellButtonName(i);
		cellButton = ApGetWidgetPtr(thisModule, cellButtonName);
		if(!cellButton) continue; /* skip bad widgets */
		PtSetResources(cellButton, 1, args);
	}
	return( Pt_CONTINUE );
}

int rowColProc(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
	PtArg_t args[1];
	PtWidget_t *thisModule;
	RASTER *rasters;
	PAGE *thisPage;
	CELL *thisCell;
	int *value;
	int rowIndxChange;
	int i;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get a pointer to the rasters structure */
	rasters = getRasters();

	/* get pointer to the window module that contains this widget */
	thisModule = ApGetInstance(widget);

	/* figure out this raster page */
	thisPage = getRasterPage(rasters->curPageIndx);
	
	/* get the widget's value and assign it to the appropriate variable */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, &value, 0);
	PtGetResources(widget, 1, args);
	if(ApName(widget) == ABN_pageRows) {
		/* if the user is redefining the number of rows in a raster,
		 * the row numbers of the cells has to be changed
		 */
		if(*value != thisPage->nRows) {
			rowIndxChange = *value - thisPage->nRows;
			for(i = 0; i < thisPage->nDefCells; i++) {
				thisCell = getRasterCell(thisPage->cellsInfo, i);
				thisCell->rowN += rowIndxChange;
			}
		}
		thisPage->nRows = *value;
	}
	else if(ApName(widget) == ABN_pageCols) thisPage->nCols = *value;

	initCellButtons(thisModule, thisPage, rasters->newRasters);

	return( Pt_CONTINUE );
}

int pageLabelTextProc(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
	PtArg_t args[1];
	RASTER *rasters;
	PAGE *thisPage;
	char *string;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get a pointer to the rasters structure */
	rasters = getRasters();

	/* figure out this raster page */
	thisPage = getRasterPage(rasters->curPageIndx);
	
	/* the widget's value and assign it to the page label text variable */
	PtSetArg(&args[0], Pt_ARG_TEXT_STRING, &string, 0);
	PtGetResources(widget, 1, args);
	if((strlen(string) > 0) && (strlen(string) < 128)) {
		thisPage->labelFlag = 1;
		strcpy(thisPage->pageLabel.label, string);
		thisPage->pageLabel.label[127] = 0;	/* force NULL terminator */
	}
	else {
		thisPage->labelFlag = 0;
		strcpy(thisPage->pageLabel.label, "");
	}

	return( Pt_CONTINUE );
}

int pageLabelFontProc(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
	PtWidget_t *thisModule;
	PtWidget_t *off1;
	PtWidget_t *off2;
	PtArg_t args[1];
	RASTER *rasters;
	PAGE *thisPage;
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
		
		/* figure out which button was pressd
		 * so the other two can be unset
		 */
		if(ApName(widget) == ABN_pageLabelDuplex) {
			off1 = ApGetWidgetPtr(thisModule, ABN_pageLabelComplex);
			off2 = ApGetWidgetPtr(thisModule, ABN_pageLabelItalic);
		}
		else if(ApName(widget) == ABN_pageLabelComplex) {
			off1 = ApGetWidgetPtr(thisModule, ABN_pageLabelDuplex);
			off2 = ApGetWidgetPtr(thisModule, ABN_pageLabelItalic);
		}
		else if(ApName(widget) == ABN_pageLabelItalic) {
			off1 = ApGetWidgetPtr(thisModule, ABN_pageLabelDuplex);
			off2 = ApGetWidgetPtr(thisModule, ABN_pageLabelComplex);
		}
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(off1, 1, args);
		PtSetResources(off2, 1, args);
	}

	/* if this widget's button was unpressed,
	 * set the default widget's button
	 */
	 else {
		off1 = ApGetWidgetPtr(thisModule, ABN_pageLabelDuplex);
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(off1, 1, args);

		PtSetArg(&args[0], Pt_ARG_USER_DATA, &userData, 0);
		PtGetResources(off1, 1, args);
		sscanf(userData, "%d", &font);
	 }
	thisPage->pageLabel.font = font;

	return( Pt_CONTINUE );
}

int pageLabelPositionProc(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
	PtArg_t args[1];
	RASTER *rasters;
	PAGE *thisPage;
	double *value;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get a pointer to the rasters structure */
	rasters = getRasters();

	/* figure out this raster page */
	thisPage = getRasterPage(rasters->curPageIndx);
	
	/* get the widget's value and assign it to the appropriate variable */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, &value, 0);
	PtGetResources(widget, 1, args);
	if(ApName(widget) == ABN_pageLabelLeft) thisPage->pageLabel.x = *value;
	else if(ApName(widget) == ABN_pageLabelBottom) thisPage->pageLabel.y = *value;

	return( Pt_CONTINUE );
}

int pageLabelSizeProc(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
	PtArg_t args[1];
	RASTER *rasters;
	PAGE *thisPage;
	int *value;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get a pointer to the rasters structure */
	rasters = getRasters();

	/* figure out this raster page */
	thisPage = getRasterPage(rasters->curPageIndx);
	
	/* get the widget's value and assign it to the appropriate variable */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, &value, 0);
	PtGetResources(widget, 1, args);
	thisPage->pageLabel.sz = *value;

	return( Pt_CONTINUE );
}

int cellButtonProc(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
	PtArg_t args[1];
	PtWidget_t *thisModule;
	PtWidget_t *cellButton;
	RASTER *rasters;
	PAGE *thisPage;
	CELL *thisCell, *lastCell;
	PLOT *thisPlot, *lastPlot;
	int cellButtonName;
	void *userData;
	int RCindex;
	int i, reFlag, reDefIndex;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	reDefIndex = 0;
	
	/* get a pointer to the rasters structure */
	rasters = getRasters();

	/* get pointer to the window module that contains this widget */
	thisModule = ApGetInstance(widget);

	/* figure out this raster page */
	thisPage = getRasterPage(rasters->curPageIndx);

	/* get this button's position argument */
	PtSetArg(&args[0], Pt_ARG_USER_DATA, &userData, 0);
	PtGetResources(widget, 1, args);
	sscanf(userData, "%d", &RCindex);

	/* mark the selected button yellow */
	/* first set the fill color of all buttons to grey */
	PtSetArg(&args[0], Pt_ARG_FILL_COLOR, Pg_GREY, 0);
	for(i = 0; i < 64; i++) {
		cellButtonName = getCellButtonName(i);
		cellButton = ApGetWidgetPtr(thisModule, cellButtonName);
		if(!cellButton) continue; /* skip bad widgets */
		PtSetResources(cellButton, 1, args);
	}

	/* now set this button's fill color to yellow */
	PtSetArg(&args[0], Pt_ARG_FILL_COLOR, Pg_YELLOW, 0);
	PtSetResources(widget, 1, args);

	/* first find out if the user is defining a new cell
	 * or redefining an old cell
	 * to do this, compare the position index of each
	 * previously defined cell with that of the selected cell
	 */
	reFlag = 0;
	for(i = 0; i < thisPage->nDefCells; i++) {
		thisCell = getRasterCell(thisPage->cellsInfo, i);
		if(thisCell->RCindex == RCindex) {	/* true if redefining an old cell */
			reFlag++;			/* make the redefine flag non-zero */
			reDefIndex = i;			/* index of cell to be redefined */
			break;				/* quit testing if a previous index matches the selected cell index */
		}
	}

	/* if the redefine flag is not 0, the user is redefining an old cell */
	if(reFlag) {
		thisPage->curCellIndx = reDefIndex;
		thisCell = getRasterCell(thisPage->cellsInfo, thisPage->curCellIndx);
		thisPlot = &thisCell->plotInfo;
	}

	/* otherwise, the user is defining a new cell*/
	if(!reFlag) {
		thisPage->curCellIndx = thisPage->nDefCells;
		thisCell = getRasterCell(thisPage->cellsInfo, thisPage->curCellIndx);
		thisPlot = &thisCell->plotInfo;

		/* set the relative location of this cell on the page */
		thisCell->RCindex = RCindex;
		thisCell->colN = RCindex % 8;
		thisCell->rowN = (thisPage->nRows - 1) - (RCindex / 8);

		/* if this is not the first cell on the page
		 * initialize it with the values of the last cell
		 */
		if(thisPage->curCellIndx) {
			lastCell = getRasterCell(thisPage->cellsInfo, thisPage->curCellIndx - 1);
			lastPlot = &lastCell->plotInfo;
			initPlot(thisPage, thisPlot, lastPlot);
		}
		thisPage->nDefCells++;			/* increment the number of defined cells on this page */
	}

	/* set the button's label */
	setButtonLabel(thisModule, thisCell);

 	initPlotWidgets(thisModule, thisPlot);
 	initGraphWidgets(thisModule, &thisPlot->graph);

	return( Pt_CONTINUE );
}

int clearRasters(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
	RASTER *rasters;
	
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	rasters = getRasters();
	
	if(rasters->pages) free(rasters->pages);

	return( Pt_CONTINUE );
}

void initFirstPage(PAGE *thisPage)
{
	CELL *thisCell;
	PLOT *thisPlot;
	GRAPH *thisGraph;
	int i, j;

	thisPage->plotVariables = 0;
	thisPage->graphVariables = 0;
	thisPage->pgLength = MAX_LENGTH;
	thisPage->pgWidth = NOHOLE_WIDTH;
	thisPage->nRows = 1;
	thisPage->nCols = 1;
	thisPage->maxCells = 64;
	thisPage->nDefCells = 0;
	thisPage->curCellIndx = 0;
	thisPage->labelFlag = 0;
	strcpy(thisPage->pageLabel.label, "");
	thisPage->pageLabel.x = 0.10;
	thisPage->pageLabel.y = 0.90;
	thisPage->pageLabel.sz = 15;
	thisPage->pageLabel.font = 12;

	/* set the default values for the cells in this page */
	for(i = 0; i < thisPage->maxCells; i++) {
		thisCell = getRasterCell(thisPage->cellsInfo, i);
		thisCell->RCindex = -1;
		thisCell->colN = -1;
		thisCell->rowN = -1;
		thisCell->left = -1;
		thisCell->bottom = -1;
		thisPlot = &thisCell->plotInfo;
		thisPlot->setNum = 1;
		thisPlot->cCode = 0;
		thisPlot->sigma = 3.0;
		thisPlot->trialFlag = 1;
		thisPlot->meanFlag = 1;
		thisPlot->nSignals = 1;
		thisPlot->maxSignals = 10;
		strcpy(thisPlot->sigInfo[0].sigName, "601");
		thisPlot->sigInfo[0].sigNum = 601;
		for(j = 0; j < 10; j++) thisPlot->sigInfo[j].color = tigColorList[j];
		thisPlot->markFlag = 0;
		thisPlot->labelFlag = 0;
		strcpy(thisPlot->plotLabel.label, "");
		thisPlot->plotLabel.font = 12;
		thisPlot->plotLabel.x = 0.10;
		thisPlot->plotLabel.y = 0.90;
		thisPlot->plotLabel.sz = 5;
		thisPlot->maxLines = 100;
		thisGraph = &thisPlot->graph;
		thisGraph->start = -250;
		thisGraph->interval = 500;
		thisGraph->timeMark = 100;
		thisGraph->maxFreq = 500;
		thisGraph->yAxes |= R_ALIGN;
		thisGraph->numberAxes = 0;
		thisGraph->Nformat = 0;
		thisGraph->Nfont = 12;
		thisGraph->Nsz = 5;
		thisGraph->Lfont = 12;
		thisGraph->Lsz = 10;
		strcpy(thisGraph->XaxisLabel, "");
		strcpy(thisGraph->YaxisLabel, "");
	}
	return;
}

void initSubsequentPage(PAGE *thisPage, PAGE *lastPage)
{
	CELL *thisCell, *lastCell;
	PLOT *thisPlot, *lastPlot;
	LABEL *thisLabel, *lastLabel;
	int i;

	thisPage->plotVariables = 0;
	thisPage->graphVariables = 0;
	thisPage->nDefCells = 0;
	thisPage->pgLength = lastPage->pgLength;
	thisPage->pgWidth = lastPage->pgWidth;
	thisPage->nRows = lastPage->nRows;
	thisPage->nCols = lastPage->nCols;
	thisPage->maxCells = lastPage->maxCells;
	thisPage->curCellIndx = 0;
	thisPage->cellWidth = lastPage->cellWidth;
	thisPage->cellHeight = lastPage->cellHeight;
	thisPage->labelFlag = lastPage->labelFlag;
	thisLabel = &thisPage->pageLabel;
	lastLabel = &lastPage->pageLabel;
	strcpy(thisLabel->label, "");
	thisLabel->x = lastLabel->x;
	thisLabel->y = lastLabel->y;
	thisLabel->sz = lastLabel->sz;
	thisLabel->font = lastLabel->font;

	/* set the default values for the cells in this page */
	for(i = 0; i < thisPage->maxCells; i++) {
		thisCell = getRasterCell(thisPage->cellsInfo, i);
		lastCell = getRasterCell(lastPage->cellsInfo, i);
		thisCell->RCindex = -1;
		thisCell->colN = -1;
		thisCell->rowN = -1;
		thisCell->left = -1;
		thisCell->bottom = -1;
		thisPlot = &thisCell->plotInfo;
		lastPlot = &lastCell->plotInfo;
		copyPlot(thisPlot, lastPlot);
	}
	return;
}

void copyPlot(PLOT *thisPlot, PLOT *lastPlot)
{
	SIGNAL *thisSignal, *lastSignal;
	ECODEMARK *thisMark, *lastMark;
	LABEL *thisLabel, *lastLabel;
	GRAPH *thisGraph, *lastGraph;
	int i;

	thisPlot->setNum = lastPlot->setNum;
	thisPlot->cCode = lastPlot->cCode;
	thisPlot->sigma = lastPlot->sigma;
	thisPlot->trialFlag = lastPlot->trialFlag;
	thisPlot->meanFlag = lastPlot->meanFlag;
	thisPlot->nSignals = lastPlot->nSignals;
	for(i = 0; i < 10; i++) thisPlot->sigInfo[i].color = tigColorList[i];
	for(i = 0; i < thisPlot->nSignals; i++) {
		thisSignal = &thisPlot->sigInfo[i];
		lastSignal = &lastPlot->sigInfo[i];
		strcpy(thisSignal->sigName, lastSignal->sigName);
		thisSignal->sigNum = lastSignal->sigNum;
	}
	thisPlot->maxSignals = lastPlot->maxSignals;
	thisPlot->markFlag = lastPlot->markFlag;
	thisMark = &thisPlot->ecodeMark;
	lastMark = &lastPlot->ecodeMark;
	thisMark->rMark = lastMark->rMark;
	strcpy(thisMark->symbolString, lastMark->symbolString);
	thisMark->symbolChar = lastMark->symbolChar;
	thisMark->sz = lastMark->sz;
	thisPlot->labelFlag = lastPlot->labelFlag;
	thisLabel = &thisPlot->plotLabel;
	lastLabel = &lastPlot->plotLabel;
	if(!strcmp(lastLabel->label, "default")) {
		strcpy(thisLabel->label, lastLabel->label);
	}
	else strcpy(thisLabel->label, "");
	thisLabel->x = lastLabel->x;
	thisLabel->y = lastLabel->y;
	thisLabel->sz = lastLabel->sz;
	thisLabel->font = lastLabel->font;
	thisPlot->maxLines = lastPlot->maxLines;
	thisGraph = &thisPlot->graph;
	lastGraph = &lastPlot->graph;
	thisGraph->start = lastGraph->start;
	thisGraph->interval = lastGraph->interval;
	thisGraph->timeMark = lastGraph->timeMark;
	thisGraph->maxFreq = lastGraph->maxFreq;
	thisGraph->yAxes = lastGraph->yAxes;
	thisGraph->numberAxes = lastGraph->numberAxes;
	thisGraph->Nformat = lastGraph->Nformat;
	thisGraph->Nfont = lastGraph->Nfont;
	thisGraph->Nsz = lastGraph->Nsz;
	thisGraph->Lfont = lastGraph->Lfont;
	thisGraph->Lsz = lastGraph->Lsz;
	if(strlen(lastGraph->XaxisLabel)) {
		strcpy(thisGraph->XaxisLabel, lastGraph->XaxisLabel);
	}
	else strcpy(thisGraph->XaxisLabel, "");
	if(strlen(lastGraph->YaxisLabel)) {
		strcpy(thisGraph->YaxisLabel, lastGraph->YaxisLabel);
	}
	else strcpy(thisGraph->YaxisLabel, "");

	return;
}

void initPlot(PAGE *thisPage, PLOT *thisPlot, PLOT *lastPlot)
{
	SIGNAL *thisSignal, *lastSignal;
	ECODEMARK *thisMark, *lastMark;
	LABEL *thisLabel, *lastLabel;
	GRAPH *thisGraph, *lastGraph;
	int i, j;

	for(i = 0; i < MAX_PLOT_VARS; i++) {
		switch((1 << i) & thisPage->plotVariables) {
		case PLOT_SET:
			thisPlot->setNum = lastPlot->setNum;
			break;
		case PLOT_ECODE:
			thisPlot->cCode = lastPlot->cCode;
			break;
		case PLOT_SIGMA:
			thisPlot->sigma = lastPlot->sigma;
			break;
		case PLOT_RASTER:
			thisPlot->trialFlag = lastPlot->trialFlag;
			break;
		case PLOT_DENSITY:
			thisPlot->meanFlag = lastPlot->meanFlag;
			break;
		case PLOT_UNITS:
			thisPlot->nSignals = lastPlot->nSignals;
			for(j = 0; j < thisPlot->nSignals; j++) {
				thisSignal = &thisPlot->sigInfo[j];
				lastSignal = &lastPlot->sigInfo[j];
				strcpy(thisSignal->sigName, lastSignal->sigName);
				thisSignal->sigNum = lastSignal->sigNum;
			}
			thisPlot->maxSignals = lastPlot->maxSignals;
			break;
		case PLOT_MARK:
			thisPlot->markFlag = lastPlot->markFlag;
			thisMark = &thisPlot->ecodeMark;
			lastMark = &lastPlot->ecodeMark;
			thisMark->rMark = lastMark->rMark;
			break;
		case PLOT_SYMBOL:
			thisPlot->markFlag = lastPlot->markFlag;
			thisMark = &thisPlot->ecodeMark;
			lastMark = &lastPlot->ecodeMark;
			if(strlen(lastMark->symbolString)) strcpy(thisMark->symbolString, lastMark->symbolString);
			else thisMark->rMark = 0;
			thisMark->symbolChar = lastMark->symbolChar;
			break;
		case PLOT_MSIZE:
			thisPlot->markFlag = lastPlot->markFlag;
			thisMark = &thisPlot->ecodeMark;
			lastMark = &lastPlot->ecodeMark;
			thisMark->sz = lastMark->sz;
			break;
		case PLOT_LABEL:
			thisPlot->labelFlag = lastPlot->labelFlag;
			thisLabel = &thisPlot->plotLabel;
			lastLabel = &lastPlot->plotLabel;
			if(!strcmp(lastLabel->label, "default")) {
				strcpy(thisLabel->label, lastLabel->label);
			}
			else strcpy(thisLabel->label, "");
			break;
		case PLOT_LFONT:
			thisLabel = &thisPlot->plotLabel;
			lastLabel = &lastPlot->plotLabel;
			thisLabel->font = lastLabel->font;
			break;
		case PLOT_LSIZE:
			thisLabel = &thisPlot->plotLabel;
			lastLabel = &lastPlot->plotLabel;
			thisLabel->sz = lastLabel->sz;
			break;
		case PLOT_LPOS:
			thisLabel = &thisPlot->plotLabel;
			lastLabel = &lastPlot->plotLabel;
			thisLabel->x = lastLabel->x;
			thisLabel->y = lastLabel->y;
			break;
		default:
			break;
		}
	}

	thisGraph = &thisPlot->graph;
	lastGraph = &lastPlot->graph;
	for(i = 0; i < MAX_GRAPH_VARS; i++) {
		switch((1 << i) & thisPage->graphVariables) {
		case GRAPH_START:
			thisGraph->start = lastGraph->start;
			break;
		case GRAPH_DUR:
			thisGraph->interval = lastGraph->interval;
			break;
		case GRAPH_RATE:
			thisGraph->maxFreq = lastGraph->maxFreq;
			break;
		case GRAPH_TIC:
			thisGraph->timeMark = lastGraph->timeMark;
			break;
		case GRAPH_YAXIS:
			thisGraph->yAxes = lastGraph->yAxes;
			break;
		case GRAPH_NUMBER:
			thisGraph->numberAxes = lastGraph->numberAxes;
			break;
		case GRAPH_NFORMAT:
			thisGraph->Nformat = lastGraph->Nformat;
			break;
		case GRAPH_NFONT:
			thisGraph->Nfont = lastGraph->Nfont;
			break;
		case GRAPH_NSIZE:
			thisGraph->Nsz = lastGraph->Nsz;
			break;
		case GRAPH_XLABEL:
			if(strlen(lastGraph->XaxisLabel)) {
				strcpy(thisGraph->XaxisLabel, lastGraph->XaxisLabel);
			}
			else strcpy(thisGraph->XaxisLabel, "");
			break;
		case GRAPH_YLABEL:
			if(strlen(lastGraph->YaxisLabel)) {
				strcpy(thisGraph->YaxisLabel, lastGraph->YaxisLabel);
			}
			else strcpy(thisGraph->YaxisLabel, "");
			break;
		case GRAPH_LFONT:
			thisGraph->Lfont = lastGraph->Lfont;
			break;
		case GRAPH_LSIZE:
			thisGraph->Lsz = lastGraph->Lsz;
			break;
		default:
			break;
		}
	}

	return;
}

void initPageWidgets(PtWidget_t *link_instance, PAGE *thisPage, int newRasters)
{
	PtWidget_t *widget;
	PtArg_t args[1];

	/* set the values on the row and column widgets */
	widget = ApGetWidgetPtr(link_instance, ABN_pageRows);
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, thisPage->nRows, 0);
	PtSetResources(widget, 1, args);

	widget = ApGetWidgetPtr(link_instance, ABN_pageCols);
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, thisPage->nCols, 0);
	PtSetResources(widget, 1, args);

	/* set the values on the label widgets */
	widget = ApGetWidgetPtr(link_instance, ABN_pageLabelText);
	PtSetArg(&args[0], Pt_ARG_TEXT_STRING, thisPage->pageLabel.label, 0);
	PtSetResources(widget, 1, args);

	widget = ApGetWidgetPtr(link_instance, ABN_pageLabelLeft);
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, &thisPage->pageLabel.x, 0);
	PtSetResources(widget, 1, args);

	widget = ApGetWidgetPtr(link_instance, ABN_pageLabelBottom);
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, &thisPage->pageLabel.y, 0);
	PtSetResources(widget, 1, args);

	widget = ApGetWidgetPtr(link_instance, ABN_pageLabelSize);
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, thisPage->pageLabel.sz, 0);
	PtSetResources(widget, 1, args);

	switch(thisPage->pageLabel.font) {
	case 12:
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		widget = ApGetWidgetPtr(link_instance, ABN_pageLabelDuplex);
		PtSetResources(widget, 1, args);
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		widget = ApGetWidgetPtr(link_instance, ABN_pageLabelComplex);
		PtSetResources(widget, 1, args);
		widget = ApGetWidgetPtr(link_instance, ABN_pageLabelItalic);
		PtSetResources(widget, 1, args);
		break;
	case 13:
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		widget = ApGetWidgetPtr(link_instance, ABN_pageLabelComplex);
		PtSetResources(widget, 1, args);
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		widget = ApGetWidgetPtr(link_instance, ABN_pageLabelDuplex);
		PtSetResources(widget, 1, args);
		widget = ApGetWidgetPtr(link_instance, ABN_pageLabelItalic);
		PtSetResources(widget, 1, args);
		break;
	case 15:
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		widget = ApGetWidgetPtr(link_instance, ABN_pageLabelItalic);
		PtSetResources(widget, 1, args);
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		widget = ApGetWidgetPtr(link_instance, ABN_pageLabelComplex);
		PtSetResources(widget, 1, args);
		widget = ApGetWidgetPtr(link_instance, ABN_pageLabelDuplex);
		PtSetResources(widget, 1, args);
		break;
	}

	initCellButtons(link_instance, thisPage, newRasters);

	return;
}

void initCellButtons(PtWidget_t *thisModule, PAGE *thisPage, int newRasters)
{
	PtWidget_t *cellButton;
	PtArg_t args[2];
	CELL *thisCell;
	PLOT *thisPlot;
	int cellIndex;
	int cellButtonName;
	int i, j;
	char string[16];

	/* set the values on the cell buttons */
	/* first, disable all of the cell buttons
	 * and set the labels to ---
	 */
	strcpy(string, "------");
	PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_GHOST | Pt_BLOCKED);
	PtSetArg(&args[1], Pt_ARG_TEXT_STRING, &string, 0);
	for(i = 0; i < 64; i++) {
		cellButtonName = getCellButtonName(i);
		cellButton = ApGetWidgetPtr(thisModule, cellButtonName);
		if(!cellButton) continue; /* skip bad widgets */

		PtSetResources(cellButton, 2, args);
	}

	/* now enable only the buttons for the number of rows and columns
	 * that the user selected
	 */
	PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_GHOST | Pt_BLOCKED);
	for(i = 0; i < thisPage->nRows; i++) {
		for(j = 0; j < thisPage->nCols; j++) {
			cellIndex = (i * 8) + j;
			cellButtonName = getCellButtonName(cellIndex);
			cellButton = ApGetWidgetPtr(thisModule, cellButtonName);
			if(!cellButton) continue; /* skip bad widgets */

			PtSetResources(cellButton, 1, args);
		}
	}

	/* if the raster definition has been read from a root
	 * label the button with the set and ecode
	 */
	if(!newRasters) {
		for(i = 0; i < thisPage->nDefCells; i++) {
			thisCell = getRasterCell(thisPage->cellsInfo, i);
			thisPlot = &thisCell->plotInfo;
			cellButtonName = getCellButtonName(thisCell->RCindex);
			cellButton = ApGetWidgetPtr(thisModule, cellButtonName);
			if(!cellButton) continue;	/* skip bad widgets */

			sprintf(string, "%d:%d", thisPlot->setNum, thisPlot->cCode);
			PtSetArg(&args[0], Pt_ARG_TEXT_STRING, &string, 0);
			PtSetResources(cellButton, 1, args);
		}
	}
	
	return;
}

int getCellButtonName(int index)
{
	index = index;

	switch(index) {
	case 0:
		return(ABN_cell0);
		break;
	case 1:
		return(ABN_cell1);
		break;
	case 2:
		return(ABN_cell2);
		break;
	case 3:
		return(ABN_cell3);
		break;
	case 4:
		return(ABN_cell4);
		break;
	case 5:
		return(ABN_cell5);
		break;
	case 6:
		return(ABN_cell6);
		break;
	case 7:
		return(ABN_cell7);
		break;
	case 8:
		return(ABN_cell8);
		break;
	case 9:
		return(ABN_cell9);
		break;
	case 10:
		return(ABN_cell10);
		break;
	case 11:
		return(ABN_cell11);
		break;
	case 12:
		return(ABN_cell12);
		break;
	case 13:
		return(ABN_cell13);
		break;
	case 14:
		return(ABN_cell14);
		break;
	case 15:
		return(ABN_cell15);
		break;
	case 16:
		return(ABN_cell16);
		break;
	case 17:
		return(ABN_cell17);
		break;
	case 18:
		return(ABN_cell18);
		break;
	case 19:
		return(ABN_cell19);
		break;
	case 20:
		return(ABN_cell20);
		break;
	case 21:
		return(ABN_cell21);
		break;
	case 22:
		return(ABN_cell22);
		break;
	case 23:
		return(ABN_cell23);
		break;
	case 24:
		return(ABN_cell24);
		break;
	case 25:
		return(ABN_cell25);
		break;
	case 26:
		return(ABN_cell26);
		break;
	case 27:
		return(ABN_cell27);
		break;
	case 28:
		return(ABN_cell28);
		break;
	case 29:
		return(ABN_cell29);
		break;
	case 30:
		return(ABN_cell30);
		break;
	case 31:
		return(ABN_cell31);
		break;
	case 32:
		return(ABN_cell32);
		break;
	case 33:
		return(ABN_cell33);
		break;
	case 34:
		return(ABN_cell34);
		break;
	case 35:
		return(ABN_cell35);
		break;
	case 36:
		return(ABN_cell36);
		break;
	case 37:
		return(ABN_cell37);
		break;
	case 38:
		return(ABN_cell38);
		break;
	case 39:
		return(ABN_cell39);
		break;
	case 40:
		return(ABN_cell40);
		break;
	case 41:
		return(ABN_cell41);
		break;
	case 42:
		return(ABN_cell42);
		break;
	case 43:
		return(ABN_cell43);
		break;
	case 44:
		return(ABN_cell44);
		break;
	case 45:
		return(ABN_cell45);
		break;
	case 46:
		return(ABN_cell46);
		break;
	case 47:
		return(ABN_cell47);
		break;
	case 48:
		return(ABN_cell48);
		break;
	case 49:
		return(ABN_cell49);
		break;
	case 50:
		return(ABN_cell50);
		break;
	case 51:
		return(ABN_cell51);
		break;
	case 52:
		return(ABN_cell52);
		break;
	case 53:
		return(ABN_cell53);
		break;
	case 54:
		return(ABN_cell54);
		break;
	case 55:
		return(ABN_cell55);
		break;
	case 56:
		return(ABN_cell56);
		break;
	case 57:
		return(ABN_cell57);
		break;
	case 58:
		return(ABN_cell58);
		break;
	case 59:
		return(ABN_cell59);
		break;
	case 60:
		return(ABN_cell60);
		break;
	case 61:
		return(ABN_cell61);
		break;
	case 62:
		return(ABN_cell62);
		break;
	case 63:
		return(ABN_cell63);
		break;
	default:
		return(0);
		break;
	}
}

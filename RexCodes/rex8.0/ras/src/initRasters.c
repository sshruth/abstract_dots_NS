/*
 * NOTICE:  This code was developed by employees of the
 * U.S. Government.  This version is distributed,
 * without guarantees or warranties of any kind,
 * for noncommercial use only.  The U.S. Government
 * holds the copyright.
 */
/*Standard headers */
#include <stdio.h>
#include <math.h>
#include <string.h>

/* Toolkit headers */
#include <Ph.h>
#include <Pt.h>
#include <Ap.h>

/* Local headers */
#include "ph_ras.h"
#include "abimport.h"
#include "proto.h"

void labelPage(int pageWidth, int pageHeight, LABEL *thisLabel)
{
	static PhRect_t tile;
	struct TIGvec vector;
	int xp, yp, offset, length;
	char *tigStr;
	
	vector.Xpos = 1;
	vector.Ypos = 0;
	vector.Zpos = 0;
	newplane(&vector, (struct TIGvec *)NULL);

	newcolor(BLACK, 255);

	newfont(thisLabel->font);
	newsize(thisLabel->sz);

	/* convert the concatenated label to tig */
	tigStr = _strconv(thisLabel->label, (double *)NULL, &offset, &length);
	xp = (int)((float)pageWidth * thisLabel->x);

	/* make sure that the label doesn't run off the
	 * top or right sides of the page
	 */
	if((xp + length) > pageWidth) xp -= (xp + length) - pageWidth;

	yp = (int)((float)pageHeight * thisLabel->y);
	if((yp + (thisLabel->sz * 10)) > pageHeight) yp -= (yp + (thisLabel->sz * 10)) - pageHeight;

	// add a tile to the tig widget
	tile.ul.x = xp;
	tile.ul.y = yp + thisLabel->sz * 10;
	tile.lr.x = xp + length;
	tile.lr.y = yp;
	setTile(&tile);

	penup();
	move2pt(xp, yp, 0);
	_pltstr(tigStr);

	return;
}

void initTigCell(TIGSPACE *dataSpace, PLOT *thisPlot, int cellWidth, int cellHeight)
{
	struct TIGvec vector;
	DARRAY *dispTime;
	DARRAY *dispData;
	GRAPH *thisGraph;
	
	/* get a pointer to the graph structure */
	thisGraph = &thisPlot->graph;

	/* set up raster space */
	dispTime = dataSpace->xaxis.datarp;
	dispData = dataSpace->yaxis.datarp;

	dispTime->dcoef.Fctcall = _Fctlin;
	dispTime->dcoef._Coef[SLOPE] = 1;
	dispTime->dcoef._Coef[INTERCEPT] = 0;
	dispTime->mode = LONG;
	dispTime->nelems = 1;
	dispTime->left = (double)thisGraph->start;
	dispTime->right = (double)(thisGraph->start + thisGraph->interval);

	dispData->dcoef.Fctcall = _Fctlin;
	dispData->dcoef._Coef[SLOPE] = 1;
	dispData->dcoef._Coef[INTERCEPT] = 0;
	dispData->mode = FLOAT;
	dispData->nelems = 1;

	setcoef(dispTime, dispTime->left);

	vector.Xpos = 1;
	vector.Ypos = 0;
	vector.Zpos = 0;

	newplane(&vector, (struct TIGvec *)NULL);
	drawTigAxes(dataSpace, thisGraph);

	if(thisPlot->labelFlag) {
		labelCell(dataSpace, &thisPlot->plotLabel, cellWidth, cellHeight);
	}
	
	return;
}

void labelCell(TIGSPACE *dataSpace, LABEL *thisLabel, int cellWidth, int cellHeight)
{
	struct TIGvec vector;
	DARRAY *dispTime;
	DARRAY *dispData;
	long xpoint;
	float ypoint;
	int xsize, ysize;
	int xp, yp, offset, length;
	int xl, yl;
	char *tigStr;

	dispTime = dataSpace->xaxis.datarp;
	dispData = dataSpace->yaxis.datarp;

	/* initialize the darray data pointers */
	dispTime->datap.lngp = &xpoint;
	dispData->datap.fltp = &ypoint;
	xsize = dataSpace->xaxis.axscale;
	ysize = dataSpace->yaxis.axscale;

	vector.Xpos = 1;
	vector.Ypos = 0;
	vector.Zpos = 0;
	newplane(&vector, (struct TIGvec *)NULL);

	/* find the lower left corner of the plot */
	xpoint = dispTime->left;
	xp = TIGconv(&dataSpace->xaxis, 0);

	ypoint = dispData->left;
	yp = TIGconv(&dataSpace->yaxis, 0);

	/* find the coordinates of the lower left corner of the label */
 	xl = xp + ((xsize - xp) * thisLabel->x);
	yl = yp + ((ysize - yp) * thisLabel->y);

	penup();
	move2pt(xl, yl, 0);
	//	newcolor(BLACK, 255);
	newfont(thisLabel->font);
	newsize(thisLabel->sz);

	/* convert the concatenated label to tig */
	tigStr = _strconv(thisLabel->label, (double *)NULL, &offset, &length);
 	_pltstr(tigStr);
	return;
}

void drawTigAxes(TIGSPACE *dataSpace, GRAPH *graph)
{
	DARRAY *dispTime;
	DARRAY *dispData;

	/* initialize the darray data pointers */
	dispTime = dataSpace->xaxis.datarp;
	dispData = dataSpace->yaxis.datarp;

	/* set the extrema to the unit data */
	dispData->left = 0.0;
	dispData->right = (float)graph->maxFreq;
	setcoef(dispData, dispData->left);

	/* draw the X axis of the plot */
  	drawTigXaxis(dataSpace, graph);

	/* now figure out what Y axes, if any, are needed */
	if(graph->yAxes) {
		/* draw the alignment line for this plot if the graph starts before 0 */
		if((graph->yAxes & R_ALIGN) && (graph->start <= 0)) {		/* if there is an alignment line */
			drawRasterAlign(dataSpace, graph);
		}

		if(graph->yAxes & R_LEFT) {				/* if there is a left Y axis line */
  			drawTigYaxis(dataSpace, graph);
		}
	}
	return;
}

void drawTigXaxis(TIGSPACE *dataSpace, GRAPH *graph)
{
	struct TIGdvec XaxisVector;
	double bases = 10.0;
	int ndivs, nticks;
	char xFont[16];
	char xSize[16];
	char xFormat[16];
	char xTitle[128];
	
	//	newcolor(BLACK, 255);

	newline(0377, 1, THICK);

	if(graph->numberAxes) {
		sprintf(xFont, "%%%df", graph->Nfont);
		strcpy(xFormat, xFont);
		sprintf(xSize, "%%%ds", graph->Nsz);
		strcat(xFormat, xSize);
		if(!graph->Nformat) strcat(xFormat, "%i");
		else strcat(xFormat, "%6^.2d");
	}
	else strcpy(xFormat, "");

	if(strlen(graph->XaxisLabel)) {
		sprintf(xFont, "%%%df", graph->Lfont);
		strcpy(xTitle, xFont);
		sprintf(xSize, "%%%ds", graph->Lsz);
		strcat(xTitle, xSize);
		strcat(xTitle, graph->XaxisLabel);
	}
	else strcpy(xTitle, "");
	ndivs = graph->interval / graph->timeMark;
	nticks = 1;

	penup();

	move2pt(0, 0, 0);

	XaxisVector.XXpos = 1.000;
	XaxisVector.YYpos = 0.000;
	XaxisVector.ZZpos = 0.000;

	axis(xTitle, xFormat, ndivs, nticks, &dataSpace->xaxis, &XaxisVector, bases, NORMAL);
}

void drawTigYaxis(TIGSPACE *dataSpace, GRAPH *graph)
{
	struct TIGdvec YaxisVector;
	struct TIGvec vector;
	DARRAY *dispTime;
	double bases = 10.0;
	long xpoint;
	int xp;
	int ndivs, nticks;
	char yFont[16];
	char ySize[16];
	char yFormat[16];
	char yTitle[128];
	
	/* get the TIG value for the left-most X value */
	dispTime = dataSpace->xaxis.datarp;
	dispTime->datap.lngp = &xpoint;

	newline(0377, 1, THICK);

	ndivs = 5;
	nticks = 1;
	vector.Xpos = 0;
	vector.Ypos = 1;
	vector.Zpos = 0;

	YaxisVector.XXpos = 0.000;
	YaxisVector.YYpos = 1.000;
	YaxisVector.ZZpos = 0.000;

	xpoint = dispTime->left;
	xp = TIGconv(&dataSpace->xaxis, 0);
	newplane(&vector, (struct TIGvec *)NULL);
	penup();
	if(graph->numberAxes) {
		sprintf(yFont, "%%%df", graph->Nfont);
		strcpy(yFormat, yFont);
		sprintf(ySize, "%%%ds", graph->Nsz);
		strcat(yFormat, ySize);
		if(!graph->Nformat) strcat(yFormat, "%i");
		else strcat(yFormat, "%6^.2d");
	}
	else strcpy(yFormat, "");
	if(strlen(graph->YaxisLabel)) {
		sprintf(yFont, "%%%df", graph->Lfont);
		strcpy(yTitle, yFont);
		sprintf(ySize, "%%%ds", graph->Lsz);
		strcat(yTitle, ySize);
		strcat(yTitle, graph->YaxisLabel);
	}
	else strcpy(yTitle, "");
	move2pt(xp, 0, 0);
	axis(yTitle, yFormat, ndivs, nticks, &dataSpace->yaxis, &YaxisVector, bases, GG|NOLEFT);

	vector.Xpos = 1;
	vector.Ypos = 0;
	vector.Zpos = 0;
	newplane(&vector, (struct TIGvec *)NULL);
	penup();
	move2pt(0, 0, 0);
}

void drawRasterAlign(TIGSPACE *dataSpace, GRAPH *graph)
{
	DARRAY *dispTime;
	DARRAY *dispData;
	long xpoint;
	float ypoint;
	int xp, yp;

	/* initialize the darray data pointers */
	dispTime = dataSpace->xaxis.datarp;
	dispData = dataSpace->yaxis.datarp;
	dispTime->datap.lngp = &xpoint;
	dispData->datap.fltp = &ypoint;

	/* get the TIG value for the X = 0 */
 	xpoint = 0;
	xp = TIGconv(&dataSpace->xaxis, 0);

	/* get the TIG value for the bottom-most Y value */
	ypoint = dispData->left;
	yp = TIGconv(&dataSpace->yaxis, 0);

	newline(0377, 1, THICK);
	move2pt(xp, yp, 0);
	pendown();

	ypoint = dispData->right;

	yp = TIGconv(&dataSpace->yaxis, 0);
	
	move2pt(xp, yp, 0);
	penup();
}

int setTrialVSpace(int *count, int ysiz, double *trialLineHeight)
{
	int tikht;

	/* compute the spacing of the raster lines in thousanths */
	*trialLineHeight = (double) ysiz / (double) *count;

	/* make the length of the tick marks 1/3 of the trial line spacing */
	tikht = (int)(0.33 * *trialLineHeight);

	/* tikht must be at least 12 thousanths and no more than 100 thousanths*/
	if(tikht < 12) tikht = 12;
	else if(tikht > 100) tikht = 100;

	*trialLineHeight = (double)tikht / 0.33;
	*count = (int)((double)ysiz / *trialLineHeight);

	return(tikht);
}

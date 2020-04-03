/* Standard Headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* Toolkit headers */
#include <Ph.h>
#include <Pt.h>
#include <Ap.h>

/* tig widget headers */
#include "lsrTig.h"

/* local headers */
#include "ph_xcorr.h"
#include "abdefine.h"
#include "abimport.h"
#include "proto.h"

/* prototype declarations */
char getTigChar();
short getTigWord();

LABEL pageLabel;
GRAPH pageGraph;

int *unitTimes[MAX_LINES + 1];
int sdAvgCount = 0;
int maxLines = MAX_LINES;
int rasterLines = 0;
float *spkDenAvg = (float *)NULL;
char errmsg[128];

LsrTig lsrTig;

void setTigList(char *buffer)
{
	lsrTig.tigList = buffer;
	PtDamageWidget(ABW_tigWindow);
}

void setTigListLength(long length)
{
	lsrTig.tigListLen = length;
	lsrTig.tigListPtr = 0;
	lsrTig.currentPage = 0;
	lsrTig.lastPage = 0;
 	lsrTig.tigListEnd = &lsrTig.tigListLen;
}

char getTigChar()
{
	return(lsrTig.tigList[lsrTig.tigListPtr++]);
}

short getTigWord()
{
	short int t1, t2, x;

	t1 = lsrTig.tigList[lsrTig.tigListPtr++];
	t2 = lsrTig.tigList[lsrTig.tigListPtr++];
	x = (256 * t2) + (t1 & 0377);

	return(x);
}

void wait_ff()
{
/*
	PtArg_t args[1];
	
	if(lsrTig.currentPage == lsrTig.lastPage) {
		if(lsrTig.lastPage < (NPAGES - 2)) lsrTig.lastPage++;
		lsrTig.pageStart[lsrTig.lastPage] = lsrTig.tigListPtr;
	}

	PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_GHOST | Pt_BLOCKED);
	PtSetResources(ABW_tigNext, 1, args);
 	setTigListEnd();
*/
}

void setTigListEnd()
{
	long pg;

	pg = lsrTig.currentPage + 1;

	if(pg <= lsrTig.lastPage) lsrTig.tigListEnd = &lsrTig.pageStart[pg];
	else lsrTig.tigListEnd = &lsrTig.tigListLen;
}

void getPointXY()
{
	int tx;
	int ty;

	tx = getTigWord() * lsrTig.xscale;
	ty = getTigWord() * lsrTig.yscale;

	/*
	 * clip
	 */
	if(tx < 0) tx = 0;
	else if(tx > lsrTig.xmax) tx = lsrTig.xmax;

	if(ty < 0) ty = 0;
	else if(ty > lsrTig.ymax) ty = lsrTig.ymax;
	
	lsrTig.currentPoint.x = tx + lsrTig.xoff;	
	lsrTig.currentPoint.y = (lsrTig.wyb - (lsrTig.wym * ty)) + lsrTig.area.pos.y;
}

void calcScale()
{
	PtCalcCanvas((PtWidget_t *)ABW_tigWindow, &lsrTig.canvas);
	PhRectToArea(&lsrTig.canvas, &lsrTig.area);

	lsrTig.xoff = (W_MARGX * lsrTig.area.size.w) / FMAXDEV_X;
	lsrTig.yoff = (W_MARGY * lsrTig.area.size.h) / FMAXDEV_Y;
	lsrTig.xmax = lsrTig.area.size.w - (2 * lsrTig.xoff);
	lsrTig.ymax = lsrTig.area.size.h - (2 * lsrTig.yoff);
	lsrTig.wyb = lsrTig.area.size.h - lsrTig.yoff;
	lsrTig.wym = (lsrTig.wyb - lsrTig.yoff) / lsrTig.wyb;
	
	lsrTig.xscale = lsrTig.xmax / FMAXDEV_X;
	lsrTig.yscale = lsrTig.ymax / FMAXDEV_Y;
	
	return;
}

void tigExtent( PtWidget_t *widget )
{
	PtSuperClassExtent( PtBasic, widget );

	/* reset the tig scaling factors */
	calcScale();
}

void tigDraw(PtWidget_t *widget, PhTile_t *damage)
{
	int patrn, magn, lineWidth;
	int newclr, intens;
	int bad;
	
	PtSuperClassDraw( PtBasic, widget, damage );

	// set the clipping rectangle
	PtClipAdd(widget, &lsrTig.canvas);

	// set the width of the window boundaries
	PgSetStrokeWidth(2);

	// kludge
	if(lsrTig.tigListPtr == *lsrTig.tigListEnd) {
		lsrTig.tigListPtr = lsrTig.lastTigListPtr;
	}

	// save the current start and end indices
	lsrTig.lastTigListPtr = lsrTig.tigListPtr;
	lsrTig.lastTigListEnd = lsrTig.tigListEnd;

	// loop through the tig buffer
	bad = 0;
	for ( ; lsrTig.tigListPtr < *lsrTig.tigListEnd; ) {
		switch(lsrTig.tigList[lsrTig.tigListPtr++])  {
		case I_BUFR:		/* Terminal always wants unbuffered output */
		case I_NOBUF:
			break;
		case I_FORM:
			if(lsrTig.tigListPtr > 1) wait_ff();
			break;
		case I_UP:
			/* pen up */
			lsrTig.pen = UP;
			break;
		case I_DOWN:
			/* pen down */
			lsrTig.pen = DOWN;
			break;
		case I_COLR:
			newclr = getTigChar();
			intens = getTigChar();	/* flush */
			if((newclr < 0) || (newclr > (NUM_COLORS - 1))) newclr = BLACK;
			PgSetStrokeColor(lsrTig.tigColors[newclr]);
			break;
		case I_BGND:
			newclr = getTigChar();
			intens = getTigChar();	/* flush */
			if((newclr < 0) || (newclr > (NUM_COLORS - 1))) newclr = BLACK;
			PgSetFillColor(lsrTig.tigColors[newclr]);
			break;
		case I_PATRN:
			getTigWord();
			getTigWord();
			getTigWord();
			getTigChar();
			break;
		case I_LINE:
			patrn =	getTigChar();
			magn = getTigChar();
			lineWidth = getTigChar();
			lineWidth *= lsrTig.xscale;
			if(lineWidth < 1) lineWidth = 1;
			PgSetStrokeWidth(lineWidth);
			break;
		case I_MOVE:
			lsrTig.lastPoint.x = lsrTig.currentPoint.x;
			lsrTig.lastPoint.y = lsrTig.currentPoint.y;

			getPointXY();

			if(lsrTig.lastPoint.x == -1) break;		

			/* draw if "pen" is down */
			if((lsrTig.lastPoint.x == lsrTig.currentPoint.x) &&
			   (lsrTig.lastPoint.y == lsrTig.currentPoint.y) &&
			   (lsrTig.pen == DOWN)) {
				PgDrawPixel(&lsrTig.currentPoint);
			}
			else if(lsrTig.pen == DOWN) {
				PgDrawLine(&lsrTig.lastPoint, &lsrTig.currentPoint);
			}
			break;
		default: 
			sprintf(errmsg,"XTIG Error: unknown command %d, tigListPtr = %ld, tigListEnd = %ld\n",
				lsrTig.tigList[lsrTig.tigListPtr - 1], lsrTig.tigListPtr - 1, *lsrTig.tigListEnd);
			rxerr(errmsg);
			lsrTig.tigListPtr = *lsrTig.tigListEnd;
			bad = 1;
			break;
		}
		if(bad) break;
	}

	PtClipRemove();

	return;
}

void resetData()
{
	long tigBufferLength;
	char *tigBuffer;

	int i;
	for(i = 0; i < MAX_LINES; ++i) {
		if(unitTimes[i] != (int *)NULL) free(unitTimes[i]);
		unitTimes[i] = (int *)NULL;
	}
	sdAvgCount = 0;
	spkDenAvg = (float *)NULL;

	/* clear the tig list buffer */
	clearTigList();

	tigBufferLength = getTigListPtr();
	tigBuffer = getTigList();

	setTigListLength(0);
	setTigList(tigBuffer);
}

void loadData(int nCorrUnits, long *corrUnitTimes)
{
	extern double sigma;

	int *pUnitTimes;
	int i;

	// move the existing arrays of unit times up one
	if(unitTimes[MAX_LINES - 1] != (int *)NULL) {
		free(unitTimes[MAX_LINES - 1]);
	}
	for(i = MAX_LINES - 1; i > 0; --i) unitTimes[i] = unitTimes[i - 1];

	// allocate an array to hold the number of times and the list of times
	if((unitTimes[0] = (int *)malloc((nCorrUnits + 1) * sizeof(int))) == (int *)NULL) return;

	pUnitTimes = unitTimes[0];

	// the number of times is the first value in the array
	*pUnitTimes++ = nCorrUnits;

	// now load the times into the rest of the array
	for(i = 0; i < nCorrUnits; ++i) *pUnitTimes++ = (int)*corrUnitTimes++;
	sdAvgCount++;

	rasterLines = 0;
	for(i = 0; i < MAX_LINES; ++i) {
		if(unitTimes[i] != (int *)NULL) rasterLines++;
		else break;
	}

	// compute the spike density function
	spkDenAvg = fast_sd_est(&unitTimes[0][1], sdAvgCount, unitTimes[0][0], pageGraph.interval, sigma);

	drawXcorr();
}

void drawXcorr()
{
	DARRAY dispTime;	/* tig struct to hold time values */
	DARRAY dispData;	/* tig struct to hold data values */
	TIGSPACE dataSpace;	/* tig space struct */

	double trialLineHeight;
	int tikht;
	int plotLeft;
	int plotBottom;
	long tigBufferLength;
	char *tigBuffer;

	// if data have been loaded, draw them
	/* reset the tig drawing buffer */
	output(CLSFILE, CLSFLTR);

	/* initialize tig drawing structures */
	dataSpace.xaxis.datarp = &dispTime;
	dataSpace.yaxis.datarp = &dispData;

	newcolor(BLACK, 255);
	newangle(0.0, 0.0, 0.0);

	neworigin(0, 0);
	labelPage();

	dataSpace.xaxis.axscale = MAX_X - (MARGINS * 2);
	dataSpace.yaxis.axscale = MAX_Y - (MARGINS * 2) - (pageGraph.Nsz + _A_ndis);

	/* initialize the vertical spacing of raster lines */
	tikht = setTrialVSpace(&maxLines, dataSpace.yaxis.axscale, &trialLineHeight);

	/* initialize the TIG cell for this plot */
	plotLeft = MARGINS;
	plotBottom = MARGINS + pageGraph.Nsz + _A_ndis;
	neworigin(plotLeft, plotBottom);

	initTigCell(&dataSpace);

	plotSpkRas(&unitTimes[0], rasterLines, maxLines, tikht, trialLineHeight, &dataSpace);

	plotSpkDen(spkDenAvg, &dataSpace);
	
	/* send the tig draw buffer to the lsrTig widget */
	tigBufferLength = getTigListPtr();
	tigBuffer = getTigList();

	setTigListLength(tigBufferLength);
	setTigList(tigBuffer);

	return;
}

void labelPage()
{
	struct TIGvec vector;

	int pageWidth = MAX_X;
	int pageHeight = MAX_Y;
	int xp;
	int yp;
	int offset;
	int length;
	char *tigStr;
	
	vector.Xpos = 1;
	vector.Ypos = 0;
	vector.Zpos = 0;
	newplane(&vector, (struct TIGvec *)NULL);

	newline(0377, 1, THICK);
	newfont(pageLabel.font);
	newsize(pageLabel.sz);

	/* convert the concatenated label to tig */
	tigStr = _strconv(pageLabel.label, (double *)NULL, &offset, &length);
	xp = (int)((float)pageWidth * pageLabel.x);

	/* make sure that the label doesn't run off the
	 * top or right sides of the page
	 */
	if((xp + length) > pageWidth) xp -= (xp + length) - pageWidth;

	yp = (int)((float)pageHeight * pageLabel.y);
	if((yp + (pageLabel.sz * 10)) > pageHeight) yp -= (yp + (pageLabel.sz * 10)) - pageHeight;

	penup();
	move2pt(xp, yp, 0);
	_pltstr(tigStr);

	return;
}

int setTrialVSpace(int *count, int ysiz, double *trialLineHeight)
{
	int tikht;

	/* compute the spacing of the raster lines in thousanths */
	*trialLineHeight = (double) ysiz / (double) *count;

	/* make the length of the tick marks 1/3 of the trial line spacing */
	tikht = (int)(0.33 * *trialLineHeight);

	/* tikht must be at least 12 thousanths and no more than 100 thousanths */
	if(tikht < 12) tikht = 12;
	else if(tikht > 100) tikht = 100;

	*trialLineHeight = (double)tikht / 0.33;
	*count = (int)((double)ysiz / *trialLineHeight);

	return(tikht);
}

void initTigCell(TIGSPACE *dataSpace)
{
	struct TIGvec vector;
	DARRAY *dispTime;
	DARRAY *dispData;
	
	/* set up raster space */
	dispTime = dataSpace->xaxis.datarp;
	dispData = dataSpace->yaxis.datarp;

	dispTime->dcoef.Fctcall = _Fctlin;
	dispTime->dcoef._Coef[SLOPE] = 1;
	dispTime->dcoef._Coef[INTERCEPT] = 0;
	dispTime->mode = LONG;
	dispTime->nelems = 1;
	dispTime->left = (double)pageGraph.start;
	dispTime->right = (double)(pageGraph.start + pageGraph.interval);

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
	drawTigAxes(dataSpace);
	
	return;
}

void drawTigAxes(TIGSPACE *dataSpace)
{
	DARRAY *dispTime;
	DARRAY *dispData;

	/* initialize the darray data pointers */
	dispTime = dataSpace->xaxis.datarp;
	dispData = dataSpace->yaxis.datarp;

	/* set the extrema to the unit data */
	dispData->left = 0.0;
	dispData->right = (float)pageGraph.maxFreq;
	setcoef(dispData, dispData->left);

	/* draw the X axis of the plot */
  	drawTigXaxis(dataSpace);

	/* draw the alignment line for this plot if the graph starts before 0 */
	drawRasterAlign(dataSpace);

	return;
}

void drawTigXaxis(TIGSPACE *dataSpace)
{
	struct TIGdvec XaxisVector;
	double bases = 10.0;
	int ndivs, nticks;
	char xFont[16];
	char xSize[16];
	char xFormat[16];
	char xTitle[128];
	
	newline(0377, 1, THICK);

	sprintf(xFont, "%%%df", pageGraph.Nfont);
	strcpy(xFormat, xFont);
	sprintf(xSize, "%%%ds", pageGraph.Nsz);
	strcat(xFormat, xSize);
	strcat(xFormat, "%i");
	strcpy(xTitle, "");
	ndivs = pageGraph.interval / pageGraph.timeMark;
	nticks = 1;

	penup();

	move2pt(0, 0, 0);

	XaxisVector.XXpos = 1.000;
	XaxisVector.YYpos = 0.000;
	XaxisVector.ZZpos = 0.000;

	axis(xTitle, xFormat, ndivs, nticks, &dataSpace->xaxis, &XaxisVector, bases, NORMAL);
}

void drawRasterAlign(TIGSPACE *dataSpace)
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

void plotSpkRas(int **spikeTrains, int nTrials, int maxLines, int tikht, double lineHeight, TIGSPACE *dataSpace)
{
	DARRAY *dispTime;
	DARRAY *dispData;
	int *spikes;
	float ypoint;
	long xpoint;
	int spikeCounts;
	int xp, yp;
	int i, j;

	if(!nTrials) return;
	
	/* initialize the darray data pointers */
	dispTime = dataSpace->xaxis.datarp;
	dispData = dataSpace->yaxis.datarp;
	dispData->left = 0.0;
	dispData->right = (float)maxLines;
	setcoef(dispData, dispData->left);
	dispTime->datap.lngp = &xpoint;
	dispData->datap.fltp = &ypoint;

	/* set the line thickness */
	newline(0377, 1, THIN);

	/* get the coordinates of the data in data space */
	xpoint = dispTime->left;
	ypoint = dispData->left;

	/* convert the data coordinates to page coordinates */
	xp = TIGconv(&dataSpace->xaxis, 0);
	yp = TIGconv(&dataSpace->yaxis, 0);

	/* lift the pen to prevent drawing
	 * and move to the page location
	 */
	penup();
	move2pt(xp, yp, 0);

	for(i = 0; i < nTrials; i++) {
		newcolor((i % 8), 255);

		/* compute the y position for this raster line in data space */
		ypoint = i + 1;

		/* convert the data coordinate to a page location */
		yp = TIGconv(&dataSpace->yaxis, 0);

		/* compute the X coordinate of each spike and draw a mark */
		if(spikeTrains[i] != (int *)NULL) {
			spikeCounts = spikeTrains[i][0];
			spikes = &spikeTrains[i][1];

			for(j = 0; j < spikeCounts; j++) {
				/* get the time of the spike in data space */
				xpoint = spikes[j] + pageGraph.start;
			
				/* check to see if spike time is within the plotting interval */
				if(xpoint < dispTime->left) continue;		/* if spike is too far left */
				else if(xpoint > dispTime->right) break;	/* if spike is too far right */

				/* convert the data coordinate of the spike time to a page location */
				xp = TIGconv(&dataSpace->xaxis, 0);

				/* move to the top of the tick mark */
				move2pt(xp, yp + (tikht / 2), 0);

				/* drop the pen to allow drawing,
				 * then move to the bottom of the tick
				 */
				pendown();
				move2pt(xp, yp - (tikht / 2), 0);

				/* lift the pen */
				penup();
			}
		}
	}
}

void plotSpkDen(float *pspkDenAvg, TIGSPACE *dataSpace)
{
	DARRAY *dispTime;
	DARRAY *dispData;
	float ypoint;
	long xpoint;
	int yp, xp;
	int i;
	int old_xp, old_yp;

	if(pspkDenAvg != (float *)NULL) {
		/* initialize the darray data pointers for spike density data */
		dispTime = dataSpace->xaxis.datarp;
		dispData = dataSpace->yaxis.datarp;
		dispData->left = 0.0;
		dispData->right = (float)pageGraph.maxFreq;
		setcoef(dispData, dispData->left);
		dispTime->datap.lngp = &xpoint;
		dispData->datap.fltp = &ypoint;

		newcolor(BLACK, 255);

		/* set the line thickness */
		newline(0377, 1, THIN);

		/* get the coordinates of the data in data space */
		xpoint = dispTime->left;
		ypoint = pspkDenAvg[0];

		/* clip the data */
		if(ypoint > dispData->right) ypoint = dispData->right;
		else if (ypoint < dispData->left) ypoint = dispData->left;

		/* convert the data coordinates to page coordinates */
		xp = TIGconv(&dataSpace->xaxis, 0);
		yp = TIGconv(&dataSpace->yaxis, 0);

		/* lift the pen to prevent drawing
		 * and move to the page location
		 */
		penup();
		move2pt(xp, yp, 0);

		/* drop the pen to allow drawing */
		pendown();
		old_xp = 0;
		old_yp = 0;
		for(i = 0; i < pageGraph.interval; i++) {
			/* get the next data point */
			xpoint++;
			ypoint = pspkDenAvg[i];

			/* clip the data */
			if(ypoint > dispData->right) ypoint = dispData->right;
			else if(ypoint < dispData->left) ypoint = dispData->left;

			/* convert the data point to a page location */
			xp = TIGconv(&dataSpace->xaxis, 0);
			yp = TIGconv(&dataSpace->yaxis, 0);

			/* only plot to tig resolution */
			if((old_xp != xp) || (old_yp != yp)) {
				old_xp = xp;
				old_yp = yp;
				move2pt(xp, yp, 0);
			}
		}
		penup();
	}
	return;
}

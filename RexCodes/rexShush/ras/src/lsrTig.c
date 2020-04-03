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
#include "ph_ras.h"
#include "abdefine.h"
#include "abimport.h"
#include "proto.h"

/* prototype declarations */
char getTigChar();
short getTigWord();

LsrTig lsrTig;
char errmsg[128];

void tigDraw(PtWidget_t *widget, PhTile_t *damage)
{
	int patrn, magn, lineWidth;
	int newclr, intens;
	int bad;
	int i;
	
	if(lsrTig.clear) {
		PtSuperClassDraw( PtBasic, widget, damage );
		lsrTig.clear = 0;
	}

	// check for null pointer
	if(lsrTig.tigListEnd == 0) return;

	// set background color
	PgSetFillColor(Pg_WHITE);

	// set the clipping rectangle
	PtClipAdd(widget, &lsrTig.canvas);

	for(i = 0; i < lsrTig.tileCount; i++) {
		PgDrawRect(&lsrTig.tiles[i], Pg_DRAW_FILL);
	}

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
	PtArg_t args[1];
	
	if(lsrTig.currentPage == lsrTig.lastPage) {
		if(lsrTig.lastPage < (NPAGES - 2)) lsrTig.lastPage++;	/* one more page */
		lsrTig.pageStart[lsrTig.lastPage] = lsrTig.tigListPtr;
	}

	PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_GHOST | Pt_BLOCKED);
	PtSetResources(ABW_tigNext, 1, args);
 	setTigListEnd();
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

void setTile(PhRect_t *tilep)
{
	lsrTig.tiles[lsrTig.tileCount].ul.x = tilep->ul.x * lsrTig.xscale;
	lsrTig.tiles[lsrTig.tileCount].ul.y = lsrTig.area.size.h - (tilep->ul.y * lsrTig.yscale);
	lsrTig.tiles[lsrTig.tileCount].lr.x = tilep->lr.x * lsrTig.xscale;
	lsrTig.tiles[lsrTig.tileCount].lr.y = lsrTig.area.size.h - (tilep->lr.y * lsrTig.yscale);

	lsrTig.tileCount++;
	return;
}

void clearTiles()
{
	lsrTig.tileCount = 0;
}

void tigClear()
{
	lsrTig.clear = 1;
	PtDamageWidget(ABW_tigWindow);	// Damage the widget to force a redraw
}

void tigExtent( PtWidget_t *widget )
{
	SELECTIONS *selections;
	int i;

	PtSuperClassExtent( PtBasic, widget );

	/* reset the tig scaling factors */
	calcScale();

	/* set the flag to clear the tig window */
	lsrTig.clear = 1;

	/* set the new data flag in all classes */
	selections = getSelections();
	for(i = 0; i < selections->nClasses; i++) {
		selections->classSet[i].new = 1;
	}

	/* call the raster draw function */
	drawRasters();
}

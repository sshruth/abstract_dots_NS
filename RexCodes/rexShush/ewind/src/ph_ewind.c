#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#include <Ph.h>
#include <Pt.h>
#include <Ap.h>

#include "abimport.h"
#include "ph_ewind.h"
#include "proto.h"

PhPoint_t const diamond[] = {
	{0, 3},
	{3, 0},
	{0, -3},
	{-3, 0},
};

PhPoint_t const octagon[] = {
	{-2, -5},
	{-5, -2},
	{-5, 2},
	{-2, 5},
	{2, 5},
	{5, 2},
	{5, -2},
	{2, -5},
};

PhPoint_t const joy[] = {
	{3, -5},
	{3, 4},
	{2, 5},
	{-2, 5},
	{-3, 4},
	{-3, 1}
};

PhPoint_t const box[] = {
	{-2, 2},
	{2, 2},
	{2, -2},
	{-2, -2},
};

PhPoint_t const uTriangle[] = {
	{0, -5},
	{3, 5},
	{-3, 5},
};

PhPoint_t const dTriangle[] = {
	{0, 5},
	{-3, -5},
	{3, -5},
};

PhPoint_t const lTriangle[] = {
	{-5, 0},
	{5, -3},
	{5, 3},
};

PhPoint_t const rTriangle[] = {
	{5, 0},
	{-5, 3},
	{-5, -3},
};

lsrDispWindow dispWind;

/* lsrDispWnd drawFunction */
void dispWindDraw( PtWidget_t *widget, PhTile_t *damage )
{
	PhPoint_t *thisCursPos;
	WND_SIZ *thisWindSiz;
	static int userRefreshed = 0;
	static int triggered = 0;
	int triggerMet;
	int i;
	int j;

	extern RT_DISP rtDisp[RT_DISP_LEN];

	/* eliminate unreferenced warnings */
 	widget = widget, damage = damage;

	// set the clipping rectangle
	PtClipAdd(widget, &dispWind.canvas);

	// clear the window if the user pressed the refresh button
	if(dispWind.refresh) {
		PtSuperClassDraw( PtBasic, widget, damage );
		PgDrawRect(&dispWind.canvas, Pg_DRAW_FILL);
		dispWind.refresh = 0;
		userRefreshed = 1;	/* display doesn't need to be refreshed again */
	}

	/* this code determines whether or not to clear the display
	 * if the display refresh is triggered
	 */
	if(dispWind.trigger) {	/* if the display is triggered */
		/* check to see if the running line is at the trigger level */
		triggerMet = 0;
		for(i = 0; i < dispWind.n_data; i++) {
			if(rtDisp[i].rt_bar == dispWind.triggerLevel) {
				triggerMet++;	/* current runline bar meets trigger level */
				break;
			}
		}
		if(!triggerMet) {
			/* remove clipping rectangle and
			 * return if trigger level not met
			 */
			triggered = 0;
			PtClipRemove();
			return;
		}

		/* if autorefresh is on and the display hasn't been cleared */
		if(dispWind.autoRefresh && !triggered) {
			if(dispWind.intervalCounter-- <= 0) {
				if(!userRefreshed) {
					PgSetFillColor(dispWind.fill_color);
					PgDrawRect(&dispWind.canvas, Pg_DRAW_FILL);	/* clear it */
				}
				dispWind.intervalCounter = dispWind.autoInterval;
			}
			triggered = 1;
		}
		/* prevent refresh if user sets auto refresh in the middle of a draw */
		else if(!dispWind.autoRefresh) triggered = 1;
	}

	/* this code determines whether or not to clear the display
	 * if the display refresh is not triggered
	 */
	else {	/* if the display is not triggered */
		if(--dispWind.refreshCounter <= 0) {
			if(!userRefreshed) {
				PgSetFillColor(dispWind.fill_color);
				PgDrawRect(&dispWind.canvas, Pg_DRAW_FILL);	/* clear it */
			}
			dispWind.refreshCounter = dispWind.refreshRate;
		}
	}

	// set the width of the window boundaries
	PgSetStrokeWidth(1);

	/* loop through cursors */
	for(j = 0; j < dispWind.numCurs; j++) {
		/* don't plot cursors that aren't selected */
		if(!dispWind.cursors[j].on) continue;

		switch(dispWind.cursors[j].type) {	/* determine which symbol to use to draw cursor */
		case CU_BOX:
			PgSetStrokeColor(dispWind.traceColors[j]);
			PgSetFillColor(dispWind.traceColors[j]);
			PgSetTextColor(dispWind.traceColors[j]);

			/* loop through each element of the display stack
			 * from the oldest to the newest
			 */
			for(i = dispWind.n_data - 1; i >= 0; i--) {
				if(dispWind.trigger) {
					if(rtDisp[i].rt_bar != dispWind.triggerLevel) continue;
				}
				thisCursPos = &dispWind.cursPos[i][j];	/* current point to be plotted */
				PgDrawPolygon((PhPoint_t const *)&box, 4, thisCursPos, Pg_DRAW_FILL | Pg_CLOSED);
			}
			break;
		case CU_DIAMOND:
			PgSetStrokeColor(dispWind.traceColors[j]);
			PgSetFillColor(dispWind.traceColors[j]);
			PgSetTextColor(dispWind.traceColors[j]);
			/* loop through each element of the display stack
			 * from the oldest to the newest
			 */
			for(i = dispWind.n_data - 1; i >= 0; i--) {
				if(dispWind.trigger) {
					if(rtDisp[i].rt_bar != dispWind.triggerLevel) continue;
				}
				thisCursPos = &dispWind.cursPos[i][j];	/* current point to be plotted */
				PgDrawPolygon((PhPoint_t const *)&diamond, 4, thisCursPos, Pg_DRAW_FILL | Pg_CLOSED);
			}
			break;
		case CU_OCTAGON:
			PgSetStrokeColor(dispWind.traceColors[j]);
			PgSetFillColor(dispWind.traceColors[j]);
			PgSetTextColor(dispWind.traceColors[j]);
			thisCursPos = &dispWind.cursPos[0][j];	/* current point to be plotted */
			PgDrawPolygon((PhPoint_t const *)&octagon, 8, thisCursPos, Pg_DRAW_STROKE | Pg_CLOSED);
			break;
		case CU_JOY:
			PgSetStrokeColor(dispWind.traceColors[j]);
			PgSetFillColor(dispWind.traceColors[j]);
			PgSetTextColor(dispWind.traceColors[j]);
			thisCursPos = &dispWind.cursPos[0][j];	/* current point to be plotted */
			PgDrawPolygon((PhPoint_t const *)&joy, 6, thisCursPos, Pg_DRAW_STROKE);
			break;
		case CU_UTRIANGLE:
			PgSetStrokeColor(dispWind.traceColors[j]);
			PgSetFillColor(dispWind.traceColors[j]);
			PgSetTextColor(dispWind.traceColors[j]);
			thisCursPos = &dispWind.cursPos[0][j];	/* current point to be plotted */
			PgDrawPolygon((PhPoint_t const *)&uTriangle, 3, thisCursPos, Pg_DRAW_FILL | Pg_CLOSED);
			break;
		case CU_DTRIANGLE:
			PgSetStrokeColor(dispWind.traceColors[j]);
			PgSetFillColor(dispWind.traceColors[j]);
			PgSetTextColor(dispWind.traceColors[j]);
			thisCursPos = &dispWind.cursPos[0][j];	/* current point to be plotted */
			PgDrawPolygon((PhPoint_t const *)&dTriangle, 3, thisCursPos, Pg_DRAW_FILL | Pg_CLOSED);
			break;
		case CU_LTRIANGLE:
			PgSetStrokeColor(dispWind.traceColors[j]);
			PgSetFillColor(dispWind.traceColors[j]);
			PgSetTextColor(dispWind.traceColors[j]);
			thisCursPos = &dispWind.cursPos[0][j];	/* current point to be plotted */
			PgDrawPolygon((PhPoint_t const *)&lTriangle, 3, thisCursPos, Pg_DRAW_FILL | Pg_CLOSED);
			break;
		case CU_RTRIANGLE:
			PgSetStrokeColor(dispWind.traceColors[j]);
			PgSetFillColor(dispWind.traceColors[j]);
			PgSetTextColor(dispWind.traceColors[j]);
			thisCursPos = &dispWind.cursPos[0][j];	/* current point to be plotted */
			PgDrawPolygon((PhPoint_t const *)&rTriangle, 3, thisCursPos, Pg_DRAW_FILL | Pg_CLOSED);
			break;
		case CU_X:
			PgSetStrokeColor(dispWind.traceColors[j]);
			PgSetFillColor(dispWind.traceColors[j]);
			PgSetTextColor(dispWind.traceColors[j]);
			thisCursPos = &dispWind.cursPos[0][j];	/* current point to be plotted */
			drawX(thisCursPos);
			break;
		case CU_PLUS:
			PgSetStrokeColor(dispWind.traceColors[j]);
			PgSetFillColor(dispWind.traceColors[j]);
			PgSetTextColor(dispWind.traceColors[j]);
			thisCursPos = &dispWind.cursPos[0][j];	/* current point to be plotted */
			drawPlus(thisCursPos);
			break;
		case CU_STAR:
			PgSetStrokeColor(dispWind.traceColors[j]);
			PgSetFillColor(dispWind.traceColors[j]);
			PgSetTextColor(dispWind.traceColors[j]);
			thisCursPos = &dispWind.cursPos[0][j];	/* current point to be plotted */
			drawStar(thisCursPos);
			break;
		case CU_EYEWIN:	/* draw window cursors */
			PgSetStrokeWidth(1);
			PgSetStrokeColor(dispWind.traceColors[j]);
			thisWindSiz = &dispWind.windSiz[j - WDI_NDATACURS];
			thisCursPos = &dispWind.cursPos[0][j];	/* current point to be plotted */
			PgDrawPolygon((PhPoint_t const *)&thisWindSiz->bnd, 4, thisCursPos, Pg_DRAW_STROKE | Pg_CLOSED);
			break;
		default:
			break;
		} /* end switch */
	}  /* end for loop through cursors */

	userRefreshed = 0;	/* display may need to be refreshed on next interrupt */
	PtClipRemove();
	return;
}

void dispWindExtent( PtWidget_t *widget )
{
	PtSuperClassExtent( PtBasic, widget );
	
	// calculate the canvas of the drawing widget
	PtCalcCanvas(widget, &dispWind.canvas);
	PhRectToArea(&dispWind.canvas, &dispWind.area);
}

void calcCursPos(int nData)
{
	RT_DISP *thisRtDisp;
	RT_CURSOR *thisRtCurs;
	PhPoint_t newPt;
	int i;
	int j;

	extern RT_DISP rtDisp[RT_DISP_LEN];

	dispWind.n_data = nData;

	for(i = 0; i < dispWind.n_data; i++) {
		thisRtDisp = &rtDisp[i];
		for(j = 0; j < WDI_NCURS; j++) {
			thisRtCurs = &thisRtDisp->rt_cursors[j];

			/* convert raw a/d values to tenths of a degree */
			newPt.x = thisRtCurs->x >> 2;
			newPt.y = thisRtCurs->y >> 2;

			/*
			 * recalculate the center point to accomodate Photon coordinate space
			 */
			dispWind.cursPos[i][j].x = dispWind.area.pos.x +
				(dispWind.area.size.w * (-newPt.x - dispWind.eyeMax) / -dispWind.eyeRange);
			dispWind.cursPos[i][j].y = dispWind.area.pos.y +
				(dispWind.area.size.h * (newPt.y - dispWind.eyeMax) / -dispWind.eyeRange);
		}
	}
	return;
}

void calcEyeScale(int scale)
{
	dispWind.scaleFactor = scale;
	dispWind.eyeMin = -(1024 >> dispWind.scaleFactor);
	dispWind.eyeMax = (1024 >> dispWind.scaleFactor);
	dispWind.eyeRange = (dispWind.eyeMax - dispWind.eyeMin) ? (dispWind.eyeMax - dispWind.eyeMin) : 1;
	changeScales(dispWind.scaleFactor);

	return;
}


void changeScales(int magIndex)
{
	PtArg_t args[1];

	extern char *scaleMag0[];
	extern char *scaleMag1[];
	extern char *scaleMag2[];
	extern char *scaleMag3[];
	extern char *scaleMag4[];
	extern char *scaleMag5[];

	/*
	 * change the scale labels based on the new tick division 
	 */
	switch(magIndex) {
	case 0:
		PtSetArg(&args[0], Pt_ARG_SCALE_TAGS, scaleMag0, NUM_TICKS);
		PtSetResources(ABW_bottomScale, 1, args);
		PtSetArg(&args[0], Pt_ARG_SCALE_TAGS, scaleMag0, NUM_TICKS);
		PtSetResources(ABW_leftScale, 1, args);
		break; 
	case 1:
		PtSetArg(&args[0], Pt_ARG_SCALE_TAGS, scaleMag1, NUM_TICKS);
		PtSetResources(ABW_bottomScale, 1, args);
		PtSetArg(&args[0], Pt_ARG_SCALE_TAGS, scaleMag1, NUM_TICKS);
		PtSetResources(ABW_leftScale, 1, args);
		break;
	case 2:
		PtSetArg(&args[0], Pt_ARG_SCALE_TAGS, scaleMag2, NUM_TICKS);
		PtSetResources(ABW_bottomScale, 1, args);
		PtSetArg(&args[0], Pt_ARG_SCALE_TAGS, scaleMag2, NUM_TICKS);
		PtSetResources(ABW_leftScale, 1, args);
		break;
	case 3:
		PtSetArg(&args[0], Pt_ARG_SCALE_TAGS, scaleMag3, NUM_TICKS);
		PtSetResources(ABW_bottomScale, 1, args);
		PtSetArg(&args[0], Pt_ARG_SCALE_TAGS, scaleMag3, NUM_TICKS);
		PtSetResources(ABW_leftScale, 1, args);
		break;
	case 4:
		PtSetArg(&args[0], Pt_ARG_SCALE_TAGS, scaleMag4, NUM_TICKS);
		PtSetResources(ABW_bottomScale, 1, args);
		PtSetArg(&args[0], Pt_ARG_SCALE_TAGS, scaleMag4, NUM_TICKS);
		PtSetResources(ABW_leftScale, 1, args);
		break;
	case 5:
		PtSetArg(&args[0], Pt_ARG_SCALE_TAGS, scaleMag5, NUM_TICKS);
		PtSetResources(ABW_bottomScale, 1, args);
		PtSetArg(&args[0], Pt_ARG_SCALE_TAGS, scaleMag5, NUM_TICKS);
		PtSetResources(ABW_leftScale, 1, args);
		break;
	default:
	  break;
	}

	return;
}

void calcWndSiz(int i, PhDim_t siz)
{
	PhDim_t newPt;

	newPt.w = siz.w >> 2;
	newPt.h = siz.h >> 2;

	dispWind.windSiz[i].dim.w = (newPt.w * dispWind.area.size.w) / dispWind.eyeRange;
	dispWind.windSiz[i].dim.h = (newPt.h * dispWind.area.size.h) / dispWind.eyeRange;

	dispWind.windSiz[i].bnd[0].x = -dispWind.windSiz[i].dim.w;
	dispWind.windSiz[i].bnd[0].y = -dispWind.windSiz[i].dim.h;
	dispWind.windSiz[i].bnd[1].x = -dispWind.windSiz[i].dim.w;
	dispWind.windSiz[i].bnd[1].y = dispWind.windSiz[i].dim.h;
	dispWind.windSiz[i].bnd[2].x = dispWind.windSiz[i].dim.w;
	dispWind.windSiz[i].bnd[2].y = dispWind.windSiz[i].dim.h;
	dispWind.windSiz[i].bnd[3].x = dispWind.windSiz[i].dim.w;
	dispWind.windSiz[i].bnd[3].y = -dispWind.windSiz[i].dim.h;

	return;
}

void drawPlus(PhPoint_t *cntr)
{
	int x1, y1, x2, y2;
	int len = 7; 

	PgSetStrokeWidth(1);

	x1 = cntr->x - len;
	y1 = cntr->y;
 
	x2 = cntr->x + len;
	y2 = cntr->y;

	PgDrawILine( x1, y1, x2, y2 ); 

	x1 = cntr->x;
	y1 = cntr->y + len;
 
	x2 = cntr->x;
	y2 = cntr->y - len;

	PgDrawILine( x1, y1, x2, y2 ); 

	return;
}

void drawX(PhPoint_t *cntr)
{
	int x1, y1, x2, y2;
	int len = 7;

	PgSetStrokeWidth(1);

	x1 = cntr->x - len;
	y1 = cntr->y + len;

	x2 = cntr->x + len;
	y2 = cntr->y - len;

	PgDrawILine( x1, y1, x2, y2 );

	x1 = cntr->x - len;
	y1 = cntr->y - len;

	x2 = cntr->x + len;
	y2 = cntr->y + len;

	PgDrawILine( x1, y1, x2, y2 );

	return;
}

void drawStar(PhPoint_t *cntr)
{
	int x1, y1, x2, y2;
	int len = 5;

	PgSetStrokeWidth(1);
	x1 = cntr->x - (len + 2);
	y1 = cntr->y + (len + 2);
	x2 = cntr->x + (len + 2);
	y2 = cntr->y - (len + 2);
	PgDrawILine( x1, y1, x2, y2 );

	x1 = cntr->x - (len + 2);
	y1 = cntr->y - (len + 2);
	x2 = cntr->x + (len + 2);
	y2 = cntr->y + (len + 2);
	PgDrawILine( x1, y1, x2, y2 );

	x1 = cntr->x - len;
	y1 = cntr->y;
	x2 = cntr->x + len;
	y2 = cntr->y;
	PgDrawILine( x1, y1, x2, y2 ); 

	x1 = cntr->x;
	y1 = cntr->y + len;
	x2 = cntr->x;
	y2 = cntr->y - len;
	PgDrawILine( x1, y1, x2, y2 ); 

	return;
}

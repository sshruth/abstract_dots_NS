
/* Standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* Toolkit headers */
#include <Ph.h>
#include <Pt.h>
#include <Ap.h>

#include "abdefine.h"
#include "abimport.h"
#include "ph_rline.h"
#include "proto.h"

lsrRunLine runLine;
uint16_t runLineMask = 0xffff;

/* lsrRunLine drawFunction */
void runLineDraw(PtWidget_t *widget, PhTile_t *damage)
{
	RT_DISP *thisRtDisp;
	RT_CHAN *thisRtChan;
	RL_SPIKE *thisSpike;
	TRACE *thisTrace;
	PhRect_t blank;
	PhPoint_t point;
	int triggerMet;
	int spikeBit;
	int spikeNum;
	int16_t tempY;
	short i, j, k;

	extern RT_DISP rtDisp[RT_DISP_LEN];
	extern TRACE rlTraces[RL_MAXCHAN + 2];

	/* eliminate 'unreferenced' warnings */
 	widget = widget, damage = damage;

	/* set the clipping rectangle so as not to draw outside the canvas */
	PtClipAdd(widget, &runLine.canvas);

	/* initialize the drawing area */
	if(runLine.drawStart.x == -1) {
		runLine.drawStart.x = runLine.canvas.ul.x;

		/* set the amount of screen that needs to be erased */
		runLine.drawEnd.x = runLine.drawStart.x + runLine.range;

		/* set the display mode */
		runLine.mode = runLine.modeValue;

		runLine.timeStamp = 0;
		runLine.timeInterval = RL_TIME_INTERVAL;

		/* draw a rectangle to clear the display */
		blank.ul.x = runLine.drawStart.x;
		blank.ul.y = 0;
		blank.lr.x = runLine.area.size.w;
		blank.lr.y = runLine.area.size.h;
		PgSetFillColor( Pg_BLACK );
		PgDrawRect(&blank, Pg_DRAW_FILL);
	}

	/* check for triggering if the display is starting */
	if(runLine.drawStart.x == runLine.canvas.ul.x) {
		if(runLine.trigger == 1) {		/* if triggering is on */
			triggerMet = 0;
			for(i = runLine.n_data - 1; i >= 0; i--) {
				if(rtDisp[i].rt_bar == runLine.triggerLevel) {
					triggerMet++;	/* current runline bar meats trigger level */
					break;
				}
			}
			if(!triggerMet) {
				/* remove clipping rectangle and
				 * return if trigger level not met
				 */
				PtClipRemove();
				return;
			}

			/* if trigger level was met,
			 * check for one-shot display
			 */
			if(runLine.display) {		/* one-shot display */
				if(!runLine.dispReset) {
					/* remove clipping rectangle and
					 * return if one-shot display not reset
					 */
					PtClipRemove();
					return;
				}
				else runLine.dispReset = 0;		/* clear the reset flag */
			}
		}
	}
			
	/* set the line thickness */
	PgSetStrokeWidth(LINE_THICK);

	/* loop through each element of the display stack
	 * from the oldest to the newest
	 */
	for(i = runLine.n_data - 1; i >= 0; i--) {
		thisRtDisp = &rtDisp[i];

		/* loop through each of the analog signals */
		for(j = 0; j < RL_MAXCHAN; j++) {
			thisTrace = &rlTraces[j];
			if(thisTrace->rl_on == 1) {			/* only draw signals that are turned on */
				thisRtChan = &thisRtDisp->rt_chans[j];

				/* scale the Y value to map onto the drawing widget */
				tempY = ((thisRtChan->level << runLine.scaleFactor) + thisTrace->rl_offset) & runLineMask;
				point.y = runLine.area.size.h - tempY;
				point.x = runLine.drawStart.x;

				/* set color to match signal color */
				PgSetStrokeColor(runLine.traceColors[j]);

				/* now draw any marks on the analog signals */
				for(k = 0; k < thisRtChan->mark; k++) {
					point.y -= (k + 1) * 3;
					PgDrawPixel(&point);
				}
			}
		}
		
		/* now draw the running line bar */
		thisTrace = &rlTraces[RL_BAR_INDX];
		if(thisTrace->rl_on) {
			tempY = thisRtDisp->rt_bar + thisTrace->rl_offset;
			point.y = runLine.area.size.h - tempY;
			point.x = runLine.drawStart.x;
 
			/* set the color used to draw the bar */
			PgSetStrokeColor(Pg_WHITE);
			
			/* draw the bar point */
			PgDrawPixel(&point);
		}

		/* loop through the units */
		thisTrace = &rlTraces[RL_SPK_INDX];
		if(thisTrace->rl_on) {
			for(j = 0; j < 3; j++) {
				for(k = 0; k < 32; k++) {
					spikeBit = 1 << k;
					spikeNum = (j * 32) + k;
					if(thisRtDisp->rt_spikes[j] & spikeBit) {
						thisSpike = (RL_SPIKE *)&i_b->rl.rl_spike[spikeNum];
						tempY = thisSpike->rl_spike_os;
						point.y = runLine.area.size.h - tempY;
						point.x = runLine.drawStart.x;
						PgSetStrokeColor(runLine.traceColors[thisSpike->rl_spike_color % MAX_COLORS]);
						PgDrawILine(point.x, (point.y - 2), point.x, (point.y + 2));
					}
				}
			}
		}

		/* now draw the timer */
		tempY = runLine.timeStamp;
		point.y = runLine.area.size.h - tempY;
		point.x = runLine.drawStart.x;
		PgSetStrokeColor(Pg_GREEN);
		PgDrawPixel(&point);

		/* increment the start and end indices of the drawing area */
		if(--runLine.timeCounter <= 0) {
			/* loop through each of the analog signals */
			for(j = 0; j < RL_MAXCHAN; j++) {
				thisTrace = &rlTraces[j];
				if(thisTrace->rl_on == 1) {	/* only draw signals that are turned on */
					thisRtChan = &thisRtDisp->rt_chans[j];

					/* scale the Y value to map onto the drawing widget */
					tempY = ((thisRtChan->level << runLine.scaleFactor) + thisTrace->rl_offset) & runLineMask;
					point.y = runLine.area.size.h - tempY;
					point.x = runLine.drawStart.x;

					/* set color to match signal color */
					PgSetStrokeColor(runLine.traceColors[j]);

					/* draw the point in the signal */
					PgDrawPixel(&point);
				}
			}

			/* now draw the running line bar */
			thisTrace = &rlTraces[RL_BAR_INDX];
			if(thisTrace->rl_on) {
				tempY = thisRtDisp->rt_bar + thisTrace->rl_offset;
				point.y = runLine.area.size.h - tempY;
				point.x = runLine.drawStart.x;
 
				/* set the color used to draw the bar */
				PgSetStrokeColor(Pg_WHITE);
			
				/* draw the bar point */
				PgDrawPixel(&point);
			}

			/* now draw the timer */
			tempY = runLine.timeStamp;
			point.y = runLine.area.size.h - tempY;
			point.x = runLine.drawStart.x;
			PgSetStrokeColor(Pg_GREEN);
			PgDrawPixel(&point);

			/* increment the start of the drawing area */
			runLine.drawStart.x++;
			runLine.drawEnd.x++;
			runLine.timeCounter = runLine.timeBase;

			/* draw a rectangle to erase the next point
			 * if the drawing is wrapping, two areas need to be erased
			 */
			if(runLine.mode == 0) {
				blank.ul.x = runLine.drawStart.x;
				blank.ul.y = 0;
				blank.lr.x = runLine.drawEnd.x;
				blank.lr.y = runLine.area.size.h;
				PgSetFillColor( Pg_BLACK );
				PgDrawRect(&blank, Pg_DRAW_FILL);
			}

			if(runLine.drawEnd.x >= runLine.canvas.lr.x) {
				if(!runLine.trigger) {
					/* erase from the beginning of the draw area */
					if(runLine.mode == 0) {
						blank.ul.x = runLine.canvas.ul.x;
						blank.ul.y = 0;
						blank.lr.x = runLine.range - (runLine.drawEnd.x - runLine.canvas.lr.x);
						blank.lr.y = runLine.area.size.h;
						PgSetFillColor(Pg_BLACK);
						PgDrawRect(&blank, Pg_DRAW_FILL);
					}
				}
			}

			/* check to see if the drawing has reached end of display area */
			if(runLine.drawStart.x >= runLine.canvas.lr.x) {
				runLine.drawStart.x = runLine.canvas.ul.x;
				runLine.drawEnd.x = runLine.drawStart.x + runLine.range;
				runLine.mode = runLine.modeValue;
				runLine.timeStamp = 0;
				runLine.timeInterval = RL_TIME_INTERVAL;

				/* if the display is triggered, break out of draw loop */
				if(runLine.trigger) break;
			}
		}
	}

	if(--runLine.timeInterval <= 0) {
		runLine.timeStamp ^= 0x4;
		runLine.timeInterval = RL_TIME_INTERVAL;
	}
	/* 
	 * remove the clipping 
	 */
	PtClipRemove();

	return;
}

void runLineExtent( PtWidget_t *widget )
{
	PtSuperClassExtent( PtBasic, widget );

	// calculate the canvas of the drawing widget
	PtCalcCanvas(widget, &runLine.canvas);
	PhRectToArea(&runLine.canvas, &runLine.area);
}

void scaleData(int nData)
{
	RT_DISP *thisRtDisp;
	RT_CHAN *thisRtChan;
	TRACE *thisTrace;
	int shift_tmp;
	int i;
	int j;

	extern RT_DISP rtDisp[RT_DISP_LEN];
	extern TRACE rlTraces[RL_MAXCHAN + 2];

	runLine.n_data = nData;
	runLine.range = ERASE_WIDTH;
	
	for(i = 0; i < runLine.n_data; i++) {
		thisRtDisp = &rtDisp[i];
		for(j = 0; j < RL_MAXCHAN; j++) {
			thisRtChan = &thisRtDisp->rt_chans[j];
			thisTrace = &rlTraces[j];
			thisRtChan->level = shift_(thisTrace->rl_scale, thisRtChan->level);
		}
	}

	return;
}

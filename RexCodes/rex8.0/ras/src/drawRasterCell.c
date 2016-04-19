/*
 * NOTICE:  This code was developed by employees of the
 * U.S. Government.  This version is distributed,
 * without guarantees or warranties of any kind,
 * for noncommercial use only.  The U.S. Government
 * holds the copyright.
 */
/*Standard headers */
#include <stdio.h>

/* Toolkit headers */
#include <Ph.h>
#include <Pt.h>
#include <Ap.h>

/* Local headers */
#include "ph_ras.h"
#include "abimport.h"
#include "proto.h"

void plotSpkRas(int **spikeTrains, int *spikeCounts, int nTrials, int maxLines, int color,
				int tikht, double lineHeight, GRAPH *graph, TIGSPACE *dataSpace)
{
	DARRAY *dispTime;
	DARRAY *dispData;
	int *spikes;
	float ypoint;
	long xpoint;
	int xp, yp;
	int i, j;
	int start, stop;

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

	/* set the line color */
	newcolor(color, 255);

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

	if(nTrials > maxLines) start = nTrials - maxLines;
	else start = 0;
	stop = nTrials;

	for(i = start; i < stop; i++) {
		/* compute the y position for this raster line in data space */
		ypoint = (i - start) + 1;

		/* convert the data coordinate to a page location */
		yp = TIGconv(&dataSpace->yaxis, 0);

		/* compute the X coordinate of each spike and draw a mark */
		spikes = spikeTrains[i];
		if(spikes == (int *)NULL) continue;			/* continue if no spike train was allocated */
		for(j = 0; j < spikeCounts[i]; j++) {
			/* get the time of the spike in data space */
  			xpoint = spikes[j] + graph->start;
			
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

	/* reset the drawing color to black */
	newcolor(BLACK, 255);
}

void plotSpkDen(float *psd_avg, int color, GRAPH *graph, TIGSPACE *dataSpace)
{
	DARRAY *dispTime;
	DARRAY *dispData;
	float ypoint;
	long xpoint;
	int yp, xp;
	int i;
	int old_xp, old_yp;

	if(!psd_avg) return;

	/* initialize the darray data pointers for spike density data */
	dispTime = dataSpace->xaxis.datarp;
	dispData = dataSpace->yaxis.datarp;
	dispData->left = 0.0;
	dispData->right = (float)graph->maxFreq;
	setcoef(dispData, dispData->left);
	dispTime->datap.lngp = &xpoint;
	dispData->datap.fltp = &ypoint;

	/* set the line thickness */
	newline(0377, 1, THIN);

	/* set the line color */
	newcolor(color, 255);

	/* get the coordinates of the data in data space */
	xpoint = dispTime->left;
	ypoint = psd_avg[0];

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
	for(i = 0; i < graph->interval; i++) {
		/* get the next data point */
		xpoint++;
		ypoint = psd_avg[i];

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

	newcolor(BLACK, 255);
	return;
}

void plotEcodeMarks(int **ecodeTrains, int *ecodeCounts, int nTrials, int maxLines, char symbol,
					int size, double lineHeight, GRAPH *graph, TIGSPACE *dataSpace)
{
	DARRAY *dispTime;
	DARRAY *dispData;
	CINFO *charInfo;
	int *ecodes;
	float ypoint;
	long xpoint;
	int xp, yp;
	int i, j;
	int start, stop;

	/* initialize the darray data pointers */
	dispTime = dataSpace->xaxis.datarp;
	dispData = dataSpace->yaxis.datarp;
	dispData->left = 0.0;
	dispData->right = (float)maxLines;
	setcoef(dispData, dispData->left);
	dispTime->datap.lngp = &xpoint;
	dispData->datap.fltp = &ypoint;

	/* set the mark color */
	newcolor(RED, 255);

	/* set the font of the symbol */
	newfont(0);

	/* set the size of the symbol */
	newsize(size);

	/* get the character info structure */
	charInfo = getcinfo(symbol);
	if(charInfo == (CINFO *)-1) {
		printf("%c is not a valid symbol for ecode mark\n", symbol);
		return;
	}
	
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

	if(nTrials > maxLines) start = nTrials - maxLines;
	else start = 0;
	stop = nTrials;

	for(i = start; i < stop; i++) {
		/* compute the y position for this raster line in data space */
		ypoint = (i - start) + 1;

		/* convert the data coordinate to a page location */
		yp = TIGconv(&dataSpace->yaxis, 0);

		/* move yp down half the size of the symbol
		 * yp is in thousanths of an inch, but size is
		 * in hundredths of an inch, so subtracting
		 * 5 times the size will move the character
		 * down half its height
		 */
		yp -= size * 5;

		/* now move to the x-positions dictated by the
		 * ecode times and draw the symbols
		 */
		ecodes = ecodeTrains[i];
		if(ecodes == (int *)NULL) continue;			/* continue if no ecodes */
		for(j = 0; j < ecodeCounts[i]; j++) {
 			xpoint = ecodes[j];
			
			/* check to see if ecode time is within the plotting interval */
			if(xpoint < dispTime->left) continue;		/* if ecode is too far left */
			else if(xpoint > dispTime->right) break;	/* if ecode is too far right */

			/* plot the ecode if it is in the interval */
			xp = TIGconv(&dataSpace->xaxis, 0);

			move2pt(xp, yp, 0);
			pendown();
			putsymb(symbol);
			penup();
		}
	}

	/* reset the drawing color to black */
	newcolor(BLACK, 255);
}

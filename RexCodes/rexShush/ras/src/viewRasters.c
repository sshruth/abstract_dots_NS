/*
 * NOTICE:  This code was developed by employees of the
 * U.S. Government.  This version is distributed,
 * without guarantees or warranties of any kind,
 * for noncommercial use only.  The U.S. Government
 * holds the copyright.
 */

/* Standard headers */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>

/* Toolkit headers */
#include <Ph.h>
#include <Pt.h>
#include <Ap.h>

/* Local headers */
#include "ph_ras.h"
#include "abimport.h"
#include "proto.h"

/* SD_EST()
 *      spike density estimation control program
 *
 * INPUT ARGS:
 *      sp_times        = pointer to array of spike times
 *      n_trials        = number of trials in this class
 *      n_times         = number of times in sp_times in each trial
 *      n_msec          = number of msec in spike trains
 *      h_width         = 0 ==> adaptive kernel
 *                      < 0 ==> adaptive kernel with pilot width = -h_width
 *                      > 0 ==> use as width for fixed kernel
 *      lag_comp        = controls lag compensation
 *      save_histo      = 1 for loading of shift & sigma & corr histograms
 * OUTPUT:
 *      returns pointer to SD_EST structure
 *
 */
		
/* SD_EST structure {
 *	float *psd_avg          average spike density waveform
 *	float **psd_ind         individual waveforms
 *	float *psd_se           std err waveform
 *	int *psh_left           shift-left lag times
 *	float *psh_corr         shift correlations
 *	int h_lo                lo h width
 *	int h_hi                hi h width
 *	int h_avg               average h width
 *	float *psd_shifthx      histogram of shift values
 *	float *psd_shifthy      histogram of shift values
 *	int psd_shifthn         number of points
 *	float *psd_corrhx       histogram of corr values
 *	float *psd_corrhy       histogram of corr values
 *	int psd_corrhn          number of points
 *	float *psd_sigmahx      histogram of adaptive sigma values
 *	float *psd_sigmahy      histogram of adaptive sigma values
 *	int psd_sigmahn         number of points
 * } SD_EST;
 */

/* this subroutine calculates and displays the rasters */
void drawRasters(void)
{
	static PhRect_t tile;
	RASTER *rasters;
	SELECTIONS *selections;
	DARRAY dispTime;	/* tig struct to hold time values */
	DARRAY dispData;	/* tig struct to hold data values */
	TIGSPACE dataSpace;	/* tig space struct */
	PAGE *page;
	CELL *cell;
	PLOT *plot;
	GRAPH *graph;
	SIGNAL *sigInfo;
	CLASSES *pci;
	TRIALS *dataSet;
	ECODEMARK *pem;
	LABEL *thisLabel;
	DexInfo *di;
	int **markTrains;
	int *markCounts;
	double trialLineHeight;
	int pageWidth, pageHeight;
	int tikht;
	int Xmargin, Ymargin;
	int endTime;
	int nTrials;
	int sigNum;
	int axisColor;
	int start, interval;
	int i, j, k, l;
	char *tigBuffer;
	long tigBufferLength;

	pci = (CLASSES *)NULL;
	
	/* get pointer to the rasters structure */
	if((rasters = getRasters()) == NULL) return;

	/* get pointer to the selections structure */
	if((selections = getSelections()) == NULL) return;

	/* get pointer to the page to be plotted */
	if((page = getRasterPage(rasters->curDrawPage)) == NULL) return;

	/* reset the tig drawing buffer */
	output(CLSFILE, CLSFLTR);

	/* clear the tile count in the tig widget */
	clearTiles();

	/* initialize tig drawing structures */
	dataSpace.xaxis.datarp = &dispTime;
	dataSpace.yaxis.datarp = &dispData;
	nTrials = 0;

	/* increase the drawing margins if necessary to accomodate labels or numbers */
	Xmargin = 0;
	Ymargin = 0;

	for(i = 0; i < page->nDefCells; i++) {
		cell = getRasterCell(page->cellsInfo, i);
		plot = &cell->plotInfo;
		graph = &plot->graph;
		if(graph->numberAxes) {
			if((graph->Nsz + _A_ndis) > Xmargin)
				Xmargin = graph->Nsz + _A_ndis;
			if(strlen(graph->XaxisLabel) && ((graph->Lsz + _A_tdis) > Xmargin))
				Xmargin = graph->Lsz + _A_tdis;
			if(graph->yAxes & R_LEFT) {
				if((graph->Nsz + _A_ndis) > Ymargin)
					Ymargin = graph->Nsz + _A_ndis;
				if(strlen(graph->YaxisLabel) && ((graph->Lsz + _A_tdis) > Ymargin))
					Ymargin = graph->Lsz + _A_tdis;
			}
		}
	}

	/* compute the size of each cell in TIG units */
	newangle(0.0, 0.0, 0.0);
	neworigin(0, 0);
	pageWidth = page->pgLength;
	pageHeight = page->pgWidth;
	page->cellWidth = (pageWidth / page->nCols);
	page->cellHeight = (pageHeight / page->nRows);

	/* draw the page label if required */
	if(page->labelFlag) {
		thisLabel = &page->pageLabel;
		labelPage(pageWidth, pageHeight, &page->pageLabel);
	}

	for(j = 0; j < page->nDefCells; j++) {
		cell = getRasterCell(page->cellsInfo, j);

		/* don't display deleted rows */
		if(cell->rowN < 0) continue;

		plot = &cell->plotInfo;		/* get pointer to the plotInfo structure for this plot */
		graph = &plot->graph;		/* get pointer to the graphInfo structure for this plot */

		/* find the class of trials needed for this cell
		 * this step has to be done here, because the user may have built the
		 * raster specifications before reading in the data
		 */
		for(k = 0; k < selections->nClasses; k++) {
			pci = &selections->classSet[k];			/* assign the kth class pointer to class */
			if((pci->setNum == plot->setNum) && (pci->c_code == plot->cCode)) break;
			else pci = (CLASSES *)NULL;
		}

		/* if the class wasn't found continue to the next cell */
		if(pci == (CLASSES *)NULL) continue;

		/* if this class doesn't need to be redrawn, continue to the next cell */
		if(pci->new == 0) continue;

		/* set the location of this cell on the page and initialize the
		 * lengths of its X and Y axes in TIG units
		 */
		cell->left = (page->cellWidth * cell->colN) + MARGINS + Ymargin;
		cell->bottom = (page->cellHeight * cell->rowN) + MARGINS + Xmargin;

		neworigin(cell->left, cell->bottom);
		
		dataSpace.xaxis.axscale = page->cellWidth - (MARGINS * 2) - Ymargin;
		dataSpace.yaxis.axscale = page->cellHeight - (MARGINS * 2) - Xmargin;

		/* initialize the vertical spacing of raster lines */
		tikht = setTrialVSpace(&plot->maxLines, dataSpace.yaxis.axscale, &trialLineHeight); 

		/* add a tile to the tig widget */
		tile.ul.x = cell->left - MARGINS - Ymargin;
		tile.ul.y = cell->bottom + page->cellHeight - MARGINS - Xmargin;
		tile.lr.x = cell->left + page->cellWidth - MARGINS - Ymargin;
		tile.lr.y = cell->bottom - MARGINS - Xmargin;
		setTile(&tile);

		/* check to see if the user wants a default cell label */
		thisLabel = &plot->plotLabel;
		if(!strcmp(thisLabel->label, "default")) {
			sprintf(thisLabel->label, "%d:%d", pci->setNum, pci->c_code);
		}

		/* class now points to the class of trials needed for this plot */
		/* get pointer to the first trial in the class of trials to be used in this plot */
		if((dataSet = getTrialData(pci->first)) == NULL) return;

		/* set the axis color. New plot will have red axes
		 * Residual plots will have black axes
		 */
		axisColor = pci->new & 0x2;
		newcolor(axisColor, 255);

		/* shift the new mask; the new class will be made residual
		 * the residual plot will be made old
		 */
		pci->new = pci->new >> 1;

		/* initialize the TIG cell for this plot */
		initTigCell(&dataSpace, plot, page->cellWidth, page->cellHeight);

		/* convert the unit names into unit numbers */
		getDexSigNumbs(plot->sigInfo, plot->nSignals);

		/* plot each of the signals in this plot */
		for(k = 0; k < plot->nSignals; k++) {
			sigInfo = getRasterSignal(plot->sigInfo, k);
			sigNum = sigInfo->sigNum;
			start = graph->start;
			interval = graph->interval;
			endTime = graph->start + graph->interval;

			/* get new spike trains and spike density functions */
			nTrials = getSpikeDensity(pci, dataSet, start, endTime, sigNum, plot->sigma);

			/* now plot the spike rasters */
			if(plot->trialFlag) {
				plotSpkRas(pci->spikeDen.ppSpikeTimes, pci->spikeDen.pSpikeCounts, nTrials,
						   plot->maxLines, sigInfo->color, tikht, trialLineHeight, graph, &dataSpace);
			}

			/* now plot the spike densities */
			if(plot->meanFlag) {
				plotSpkDen(pci->spikeDen.spkDen.psd_avg, sigInfo->color, graph, &dataSpace);
			}
		}	/* end of the signals loop */

		/* plot the ecode marks*/
		if(plot->markFlag) {
			pem = &plot->ecodeMark;
				
			/* malloc space for the new markTrains and markCounts buffers
			 * for markTrains, allocate one extra pointer to serve as the
			 * terminating null pointer
			 */
			if((markTrains = (int **)malloc((pci->nCcodes + 1) * sizeof(int *))) != (int **)NULL) {
				if((markCounts = (int *)malloc((pci->nCcodes + 1) * sizeof(int))) != (int *)NULL) {
					/* initialize the N + 1 ecodeTrain pointers to NULL and
					 * the N + 1 ecodeCount array elements to 0
					 */
					for(l = 0; l < pci->nCcodes + 1; l++) {
						markTrains[l] = (int *)NULL;
						markCounts[l] = 0;
					}
							
					/* malloc space for each trial's event codes */
					for(l = 0; l < pci->nCcodes; l++) {
						di = dataSet[l].di;
						if(di->nEvents > 0) {
							markTrains[l] = (int *)malloc(di->nEvents * sizeof(int));
						}
					}
					/* at this point the last markTrains pointer will be NULL */

					/* get ecode trains */
					nTrials = getMarkTimes(dataSet, pci->nCcodes, pem->rMark, markTrains,
										   markCounts, graph->start, graph->interval);

					/* if the user did not set the size of the marking symbols,
					 * compute a size comparable to the raster tick marks
					 */
					if(pem->sz == 0) {
						/* tikht is in thousanths, pem-sz must be in hundredths */
						if(tikht > 20) pem->sz = tikht / 10;

						/* font size can't be less than 2 */
						else pem->sz = 2;
					}

					/* now plot the ecode marks */
					plotEcodeMarks(markTrains, markCounts, nTrials, plot->maxLines, pem->symbolChar,
								   pem->sz, trialLineHeight, graph, &dataSpace);

					/* free up the markTrain and markCount buffers if necessary */
					l = 0;
					while(markTrains[l]) free(markTrains[l++]);
					free(markTrains);
					free(markCounts);
				}
			}
		}	/* end of ecode mark condition */
	} /* end of cells display loop */

	/* send the tig draw buffer to the lsrTig widget */
	tigBufferLength = getTigListPtr();
	tigBuffer = getTigList();

	setTigListLength(tigBufferLength);
	setTigList(tigBuffer);
	return;
}

int getSpikeDensity(CLASSES *pci, TRIALS *pti, int start, int end, int unitCode, double sigma)
{
	SPKDEN *spikeDen;
	SD_EST *psd;
	char *temp;
	int accptTrials;
	int interval;
	int doRealloc;
	int lastCodes;
	int lastTimes;
	int lastPts;
	int i = 0;
	int j = 0;
	char errmsg[64];

	interval = end - start;
	doRealloc = 0;

	lastCodes = pci->maxCcodes;
	lastTimes = pci->maxTimes;
	lastPts = pci->maxPts;

	/* increase the maximum number of trials if necessary */
	if(pci->nCcodes > pci->maxCcodes) {
		while(pci->nCcodes > pci->maxCcodes) pci->maxCcodes += TRIAL_BLOCK;
		doRealloc++;
	}
	
	/* increase the maximum number of spike times if necessary */
	if(pci->nTimes > pci->maxTimes) {
		while(pci->nTimes > pci->maxTimes) pci->maxTimes += BLOCK;
		doRealloc++;
	}
	
	/* increase the maximum number of points if necessary */
	if(interval > pci->maxPts) {
		while(interval > pci->maxPts) pci->maxPts += BLOCK;
		doRealloc++;
	}
	
	spikeDen = &pci->spikeDen;
	
	/* malloc space for new spikeTimes and spikeCounts buffers */
	if(!spikeDen->ppSpikeTimes) {
		spikeDen->ppSpikeTimes = (int **)malloc(pci->maxCcodes * sizeof(int *));
		spikeDen->pSpikeCounts = (int *)malloc(pci->maxCcodes * sizeof(int));
		spikeDen->spkDen.psd_ind = (float **)malloc(pci->maxCcodes * sizeof(float *));
		if(!spikeDen->ppSpikeTimes || !spikeDen->pSpikeCounts || !spikeDen->spkDen.psd_ind) {
			sprintf(errmsg, "getSpikeDensity: couldn't allocate space for %d spike density trials", pci->maxCcodes);
			rxerr(errmsg);
			return(0);
		}
		
		/* malloc space for the average spike density function */
		spikeDen->spkDen.psd_avg = (float *)malloc(pci->maxPts * sizeof(float));
		spikeDen->spkDen.psd_se = (float *)malloc(pci->maxPts * sizeof(float));
		if(!spikeDen->spkDen.psd_avg || !spikeDen->spkDen.psd_se) {
			sprintf(errmsg, "getSpikeDensity: couldn't allocate space for %d average spike density interval", pci->maxPts);
			rxerr(errmsg);
			return(0);
		}

		/* now malloc space for each trial's units and spike density functions */
		for(i = 0; i < pci->maxCcodes; i++) {
			spikeDen->ppSpikeTimes[i] = (int *)malloc(pci->maxTimes * sizeof(int));
			if(!spikeDen->ppSpikeTimes[i]) {
				sprintf(errmsg, "getSpikeDensity: couldn't allocate space for %d spike times", pci->maxTimes);
				rxerr(errmsg);
				return(0);
			}
			spikeDen->spkDen.psd_ind[i] = (float *)malloc(pci->maxPts * sizeof(float));
			if(!spikeDen->spkDen.psd_ind[i]) {
				sprintf(errmsg, "getSpikeDensity: couldn't allocate space for %d density interval%d", pci->maxPts, i);
				rxerr(errmsg);
				return(0);
			}
		}
	}
	
	/* realloc space for the new spikeTimes and spikeCounts buffers */
	else if(doRealloc) {
        /* if we need more trials per class */ 
		if(pci->maxCcodes > lastCodes) {
			/* re-allocate space for the trial spike time pointers */
			if((temp = (char *)malloc(pci->maxCcodes * sizeof(int *))) == (char *)NULL) {
				sprintf(errmsg, "getSpikeDensity: couldn't allocate space for %d density interval%d", pci->maxPts, i);
				rxerr(errmsg);
				return(0);
			}
			memcpy(temp, spikeDen->ppSpikeTimes, (lastCodes * sizeof(int *)));
			free(spikeDen->ppSpikeTimes);
			spikeDen->ppSpikeTimes = (int **)temp;
			for(i = lastCodes; i < pci->maxCcodes; i++) spikeDen->ppSpikeTimes[i] = (int *)NULL;

			/* reallocate space for the numbers of spike counts */
			if((temp = (char *)malloc(pci->maxCcodes * sizeof(int))) == (char *)NULL) {
				sprintf(errmsg, "getSpikeDensity: couldn't allocate space for %d density interval%d", pci->maxPts, i);
				rxerr(errmsg);
				return(0);
			}
			free(spikeDen->pSpikeCounts);
			spikeDen->pSpikeCounts = (int *)temp;

			/* reallocate space for the individual spike density functions */
			if((temp = malloc(pci->maxCcodes * sizeof(float *))) == (char *)NULL) {
				sprintf(errmsg, "getSpikeDensity: couldn't allocate space for %d density interval%d", pci->maxPts, i);
				rxerr(errmsg);
				return(0);
			}
			memcpy(temp, spikeDen->spkDen.psd_ind, (lastCodes * sizeof(float *)));
			free(spikeDen->spkDen.psd_ind);
			spikeDen->spkDen.psd_ind = (float **)temp;
			for(i = lastCodes; i < pci->maxCcodes; i++) spikeDen->spkDen.psd_ind[i] = (float *)NULL;
			
			if(!spikeDen->ppSpikeTimes || !spikeDen->pSpikeCounts || !spikeDen->spkDen.psd_ind) {
				sprintf(errmsg, "getSpikeDensity: couldn't reallocate space for %d spike density trials", pci->maxCcodes);
				rxerr(errmsg);
				return(0);
			}
		}

		/* if we need more points per trial */
		if(pci->maxPts > lastPts) {
			/* malloc space for the average spike density function */
			if((temp = (char *)malloc(pci->maxPts * sizeof(float))) == (char *)NULL) {
				sprintf(errmsg, "getSpikeDensity: couldn't reallocate space for %d spike density trials", pci->maxCcodes);
				rxerr(errmsg);
				return(0);
			}

			free(spikeDen->spkDen.psd_avg);
			spikeDen->spkDen.psd_avg = (float *)temp;

			/* malloc space for the spike density error */
			if((temp = (char *)malloc(pci->maxPts * sizeof (float))) == (char *)NULL) {
				sprintf(errmsg, "getSpikeDensity: couldn't reallocate space for %d spike density trials", pci->maxCcodes);
				rxerr(errmsg);
				return(0);
			}
			free(spikeDen->spkDen.psd_se);
			spikeDen->spkDen.psd_se = (float *)temp;

			if(!spikeDen->spkDen.psd_avg || !spikeDen->spkDen.psd_se) {
				sprintf(errmsg, "getSpikeDensity: couldn't reallocate space for %d spike density interval", pci->maxPts);
				rxerr(errmsg);
				return(0);
			}
		}

		/* now malloc space for each trial's units and spike density functions */
		for(i = 0; i < pci->maxCcodes; i++) {
			if(!spikeDen->ppSpikeTimes[i])	spikeDen->ppSpikeTimes[i] = (int *)malloc(pci->maxTimes * sizeof(int));
			else if(pci->maxTimes > lastTimes) {
				spikeDen->ppSpikeTimes[i] = (int *)realloc(spikeDen->ppSpikeTimes[i], pci->maxTimes * sizeof(int));

				if(!spikeDen->ppSpikeTimes[i]) {
					sprintf(errmsg, "getSpikeDensity: couln't reallocate space for %d spike times", pci->maxTimes);
					rxerr(errmsg);
					return(0);
				}
			}
			if(!spikeDen->spkDen.psd_ind[i]) spikeDen->spkDen.psd_ind[i] = (float *)malloc(pci->maxPts * sizeof(float));
			else if(pci->maxPts > lastPts) {
				spikeDen->spkDen.psd_ind[i] = (float *)realloc(spikeDen->spkDen.psd_ind[i], pci->maxPts * sizeof(float));
				if(!spikeDen->spkDen.psd_ind[i]) {
					sprintf(errmsg, "getSpikeDensity: couln't reallocate space for %d density interval", pci->maxPts);
					return(0);
				}
			}
		}
	}
	
	/* initialize the spike times, spike counts, and spike density functions */
	for(j = 0; j < pci->maxPts; j++) {
		spikeDen->spkDen.psd_avg[j] = 0.0;
		spikeDen->spkDen.psd_se[j] = 0.0;
	}
	for(i = 0; i < pci->maxCcodes; i++) {
		spikeDen->pSpikeCounts[i] = 0;
		for(j = 0; j < pci->maxTimes; j++) spikeDen->ppSpikeTimes[i][j] = 0;
		for(j = 0; j < pci->maxPts; j++) spikeDen->spkDen.psd_ind[i][j] = 0.0;
	}

	spikeDen->npts = interval;
	accptTrials = getSpikeTimes(pci, pti, unitCode, start, interval);

	/* if there are no accecptable trials
	 * zero out the spikedensity buffers
	 */
	if(accptTrials == 0) {
		psd = (SD_EST *)NULL;
		copySpikeDensityFunction(psd, &spikeDen->spkDen, pci->nCcodes, interval);
	}

	/* otherwise compute the spike density and
	 * copy it into this classes spike density struct
	 */
	else {
		psd = sd_est(spikeDen->ppSpikeTimes, accptTrials, spikeDen->pSpikeCounts, interval, sigma, 0, 0);
		copySpikeDensityFunction(psd, &spikeDen->spkDen, accptTrials, interval);
		if(psd == (SD_EST *)NULL) {
			accptTrials = 0;
		}
	}
	return(accptTrials);
}

int getSpikeTimes(CLASSES *pci, TRIALS *pti, int unitCode, int start, int interval)
{
	SPKDEN *spikeDen;
	DexInfo *di;
	int **spikeTimes;
	int *spikeCounts;
	int *theseTimes;
	int totalNumberSpikes;
	int accptSpkIndx;
	int accptTrials;
	int timeThisPoint;
	int relAlgnTime;
	int endTime;
	int i, j, k;
	
	spikeDen = &pci->spikeDen;
	spikeTimes = spikeDen->ppSpikeTimes;
	spikeCounts = spikeDen->pSpikeCounts;
	accptTrials = 0;
	endTime = start + interval;
	totalNumberSpikes = 0;
	for(i = 0; i < pci->nCcodes; i++) {
		spikeCounts[accptTrials] = 0;		/* initialize the spike counter for this trial */
		theseTimes = spikeTimes[accptTrials];
		di = pti[i].di;				/* pointer to the DexInfo structure */
		relAlgnTime = pti[i].algnTime - di->tStartTime;

		/* search through list of units for this trial
		 * for the type of unit that the user wants to plot
		 */
		for(j = 0; j < di->nUnits; j++) {
			if(di->unitCodes[j] != unitCode) continue; /* if this is not the unit to be plotted */

			/* test each unit time against the start time and plot interval */
			accptSpkIndx = 0;
			for(k = 0; k < di->rexunits->nTimes[j]; k++) {
				/* find the time of each unit, relative to the alignment code */
				timeThisPoint = di->rexunits->unitTimes[j][k] - di->tStartTime - relAlgnTime;

				/* test the time of the spike against the plotting interval */
				if(timeThisPoint < start) continue;	/* continue loop if time is earlier than start */
				else if(timeThisPoint > endTime) break;	/* break loop if time is later than end */
				else {			/* record time if the spike is withing the plotting interval */
					/* shift the time of spikes so that the spike train starts at zero */
					theseTimes[accptSpkIndx] = timeThisPoint - start;

					/* increment counter */
					spikeCounts[accptTrials]++;
					totalNumberSpikes++;
					accptSpkIndx++;			/* increment spikeTrains index */
				}
			}
			break;	/* stop searching once the desired unit has been found */
		}
		accptTrials++;				/* increment accepted trial counter */
	}
	if(totalNumberSpikes == 0) accptTrials = 0;
	return(accptTrials);
}

void copySpikeDensityFunction(SD_EST *spsd, SD_EST *dpsd, int nTrials, int interval)
{
	int i, j;
	
	if(!spsd) {
		for(i = 0; i < interval; i++) {
			dpsd->psd_avg[i] = 0.0;
			dpsd->psd_se[i] = 0.0;
			for(j = 0; j < nTrials; j++) dpsd->psd_ind[j][i] = 0.0;
		}
	}
	else {
		for(i = 0; i < interval; i++) {
			dpsd->psd_avg[i] = spsd->psd_avg[i];
			dpsd->psd_se[i] = spsd->psd_se[i];
			for(j = 0; j < nTrials; j++) dpsd->psd_ind[j][i] = spsd->psd_ind[j][i];
		}
	}
}

int getMarkTimes(TRIALS *dataSet, int nTrials, int rMark, int **markTrains, int *markCounts, int start, int interval)
{
	TRIALS *pti;
	DexInfo *di;
	long markTime;
	int *theseTimes;
	int accptCdIndx;
	int accptTrials;
	int timeThisPoint;
	int endTime;
	int i;
	
	accptTrials = 0;				/* initialize accepted trial counter */
	endTime = start + interval;

	for(i = 0; i < nTrials; i++) {			/* for each trial */
		pti = &dataSet[i];

		markCounts[accptTrials] = 0;		/* initialize the mark counter for this trial */
		theseTimes = markTrains[accptTrials];
		if(theseTimes != (int *)NULL) {
			accptCdIndx = 0;			/* initialize the index into the events structure */
			di = pti->di;				/* pointer to the DexInfo structure */

			/* first, search through list of ecodes for this trial
			 * to see if the mark the user wants to plot is an ecode
			 */
			markTime = alignOnEcode(di, pti->algnTime, rMark);

			/* if markTime is the same as pti->algnTime, the mark was not found,
			 * so continue to the next trial
			 */
			if(markTime == pti->algnTime) {
				accptTrials++;
				continue;
			}

			/* if markTime is not equal to pti->algnTime,
			 * then the mark has been found so register it
			 */
			timeThisPoint = markTime - pti->algnTime;
			if((timeThisPoint >= start) && (timeThisPoint < endTime)) {
				theseTimes[accptCdIndx] = timeThisPoint;

				/* increment the mark counter */
				markCounts[accptTrials]++;
				accptCdIndx++;
			}
			accptTrials++;
		}
	}
	return(accptTrials);
}

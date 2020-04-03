/*
 * NOTICE:  This code was developed by employees of the
 * U.S. Government.  This version is distributed,
 * without guarantees or warranties of any kind,
 * for noncommercial use only.  The U.S. Government
 * holds the copyright.
 */

/* Standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>

/* Toolkit headers */
#include <Ph.h>
#include <Pt.h>
#include <Ap.h>

/* Local headers */
#include "ph_ras.h"
#include "abimport.h"
#include "proto.h"

int dataStarted = 0;

int startRexData( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[1];
	SELECTIONS *selections;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get a pointer to the selections structure */
	selections = getSelections();
	
	/* print error if user hasn't defined selection criteria */
	if(!selections->nSelectSets) {
		rxerr("Error: No trial selection criteria");
		return(Pt_CONTINUE);
	}

	/* open the rex file */
	rexFileOpen(selections->maxSampRate, selections->startCode, selections->preTime);

	/* enable the timer widget */
	PtSetArg(&args[0], Pt_ARG_TIMER_INITIAL, 1000, 0);
	PtSetResources(ABW_rastersTimer, 1, args);
	
	/* set the title of the display to indicate that the rasters are running */
	PtSetArg(&args[0], Pt_ARG_WINDOW_TITLE, "Rasters Running", 0);
	PtSetResources(ABW_base, 1, args);

	dataStarted = 1;

	return( Pt_CONTINUE );
}

int stopRexData( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[1];

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* close the rex file */
	rexFileClose();
	
	/* disnable the timer widget */
	PtSetArg(&args[0], Pt_ARG_TIMER_INITIAL, 0, 0);
	PtSetResources(ABW_rastersTimer, 1, args);

	/* set the title of the display to indicate that the rasters are running */
	PtSetArg(&args[0], Pt_ARG_WINDOW_TITLE, "Rasters Stopped", 0);
	PtSetResources(ABW_base, 1, args);

	dataStarted = 0;

	return( Pt_CONTINUE );
}

int loadRexTrial( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	SELECTIONS *selections;
	RexInfo *ri;
	int retval;
	char errmsg[64];

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/*
	 * See if a new trial is available; trial number not used
	 * with online version of rextools.
	 */
	ri = rexGetEvents(0);

	/*
	 * If rexInfo.startCode is null, no trial was available.
	 */
	if(!ri->startCode) {
	    return( Pt_CONTINUE );
	}

	/* get a pointer to the selections structure */
	selections = getSelections();
	
	/* Check to see if the tEndTime for this trial is greater than
	 * tStartTime. If tEndTime is less than tStartTime, then this is
	 * a bad trial
	 */
	if(ri->tEndTime < ri->tStartTime) {
		sprintf(errmsg, "Bad Trial: end Time %ld is less than trial start Time %ld",
			ri->tEndTime, ri->tStartTime);
		rxerr(errmsg);
		return( Pt_CONTINUE );
	}

	retval = selectTrials(ri, selections->curTrial);

	if(retval < 0) {
		rxerr("Error in selectTrials");
		return( Pt_CONTINUE );
	}

	/* call the raster draw function */
	if(selections->nClasses) {
		drawRasters();
	}
	selections->curTrial++;
	return( Pt_CONTINUE );
}


int reInitRasters( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	return( Pt_CONTINUE );

}

int selectTrials(RexInfo *ri, int trialNum)
{
	int j, k, l, m, classCode;
	int bad, good;
	static int maxTrials = BLOCK;
	SELECTIONS *selections;
	SELECTSET *thisSet;
	SELECT *thisSelect;
	TRIALS *thisTrial;
	long algnTime;
	size_t size;
	int retval;
	
	/* get a pointer to the selections structure */
	selections = getSelections();
	
	/* allocate initial space for local data set and class structures */
	if(!selections->dataSet) selections->dataSet = (TRIALS *)malloc(maxTrials * sizeof(TRIALS));

	/* if there is no room for dataSet, return with error condition */
	if(selections->dataSet == NULL) return(-1);

	/* initial all of the trial new flags to 0 */
	for(j = 0; j < selections->nTrials; j++) selections->dataSet[j].new = 0;

	/* go through the selection process to find all of the classes
	 * that this trial satisfies
	 */
	for(j = 0; j < selections->nSelectSets; j++) {
		thisSet = getSelectSet(j);
		thisSelect = &thisSet->select;

		/* First, test each of the codes in the trial against the list of nonOf codes. */
		bad = 0;
		for(k = 0; k < thisSelect->nNonOf; k++) {
			for(l = 0; l < ri->nEvents; l++) {
				if(ri->events[l].e_code == thisSelect->nonOfList[k]) {
					bad++;
					break; /* stop if match */
				}
			}
			if(bad) break; /* stop if match */
		}
		/* If one of the codes in the trial matched one of the
		 * nonOf codes, the trial fails. Continue to test this
		 * trial against the next set of selection criteria.
		 */
		if(bad) continue; /* skip to the next selection set */

		/* Next, test each of the codes in the trial
		 * against the list of allOf codes.
		 */
		good = 0;
		for(k = 0; k < thisSelect->nAllOf; k++) {
			for(l = 0; l < ri->nEvents; l++) {
				if(ri->events[l].e_code == thisSelect->allOfList[k]) {
					good++;
					break;
				}
			}
		}
		/* If the good count is less than the number of allOf
		 * codes, the trial fails.  Continue to test this
		 * trial against the next set of selection criteria.
		 */
		if(good < thisSelect->nAllOf) continue; /* skip to the next selection set */

		/* If there are no nonOf codes and the full
		 * compliment of allOf codes in this trial, test each of
		 * the e_codes in this trial against the alignment code (if any)
		 * for this selection set
		 */
		algnTime = 0;			/* initial time value when there is no separate alignment code */
		if(thisSet->alignCode) {	/* True if there is a separate alignment code */
			/* find the time of the alignment code */
			for(k = 0; k < ri->nEvents; k++) {
				if(ri->events[k].e_code == thisSet->alignCode) {
					algnTime = ri->events[k].e_key;
					break;
				}
			}
		}

		/* Finally, if there are no nonOf codes and the full
		 * compliment of allOf codes in this trial, test each of
		 * the e_codes in this trial to see if it matches one of
		 * the anyOf codes in this set
		 */
		for(k = 0; k < thisSelect->nAnyOf; k++) {
			for(l = 0; l < ri->nEvents; l++) { /* for each of the codes in the trial */
				/* test the current e_code against the current anyof code */
				classCode = 0;
				if(thisSelect->anyOfList[k].beg &&
				   ((ri->events[l].e_code >= thisSelect->anyOfList[k].beg) &&
					(ri->events[l].e_code <= thisSelect->anyOfList[k].end))) {
					classCode = ri->events[l].e_code;
				}
				else {
					m = 0;
					while(thisSelect->anyOfList[k].list[m]) {
						if(ri->events[l].e_code == thisSelect->anyOfList[k].list[m]) {
							classCode = thisSelect->anyOfList[k].list[0];
						}
						m++;
					}
				}
				if(classCode) {
					/* If true, this trial satisfies the selection criteria. */
					thisTrial = getTrialData(selections->nTrials);
					thisTrial->new = 1;
					thisTrial->copy = 0;
					thisTrial->setNum = j + 1;
					thisTrial->c_code = classCode;
					thisTrial->trlNum = trialNum;

					/* if there was a separate alignment code */
					if(algnTime) {
						thisTrial->algnTime = algnTime + thisSet->alignOffsetTime;
					}

					/* if not, use the anyOf code for alignment */
					else {
						thisTrial->algnTime = ri->events[l].e_key + thisSet->alignOffsetTime;
					}
					/* now load the rex data */
					thisTrial->di = (DexInfo *)NULL;
					retval = loadDataSetStructure(ri, thisTrial, trialNum);
					if(retval) {
						rxerr("loadDataSetStructure returned error");
						return(-2);
					}
					selections->nTrials++;
				}
				/* malloc more space for dataSet if necessary */
				if(selections->nTrials >= maxTrials) {
					maxTrials += BLOCK;
					size = maxTrials * sizeof(TRIALS);
					selections->dataSet = (TRIALS *)realloc(selections->dataSet, size);
					if(selections->dataSet == NULL) return(-3);
				}
			}
		}
	}

	/* sort the trials according to selection set number and ecode */
	retval = trialSort(selections->dataSet, selections->nTrials);

	/* if trial sort returned normally,
	 * identify the number classes of data
	 */
	if(!retval) {
		retval = countClasses();
	}

	/* if trialSort returned -1, then there were not enough trials to sort
	 * so just return a 1 to continue the input
	 */
	else retval = 1;
	return(retval);
}

int loadDataSetStructure(RexInfo *ri, TRIALS *thisTrial, int trialNum)
{
	SELECTIONS *selections;
	static int *isLoaded = (int *)NULL;
	static int maxTrials = BLOCK;
	size_t size;
	int i, j;

	/* get a pointer to the selections structure */
	selections = getSelections();
	
	/* It is possible for a trial to satisfy several selection set
	 * criteria.  To avoid loading data from the same trial several times,
	 * each trial is flagged when loaded, and if it is needed again,
	 * only the dexInfo pointer is copied.
	 * The isLoaded array needs to be big enough to hold flags
	 * for all of the trials, not just the accepted trials,
	 */
	if(!isLoaded) {
		if((isLoaded = (int *)malloc(maxTrials * sizeof(int))) == (int *)NULL) {
			return(-5);
		}

		for(i = 0; i < maxTrials; i++) isLoaded[i] = 0;
	}
	else if(trialNum >= maxTrials) {
		size = (maxTrials + BLOCK) * sizeof(int);
		if((isLoaded = (int *)realloc(isLoaded, size)) == (int *)NULL) {
			return(-5);
		}
		for(i = maxTrials; i < maxTrials + BLOCK; i++) isLoaded[i] = 0;
		maxTrials += BLOCK;
	}

	/* if the rasters have been re-initialized
	 * clear the isLoaded flag buffer and
	 * reset the re-initialization flag
	 */
	if(selections->iallRas) {
		for(i = 0; i < maxTrials; i++) isLoaded[i] = 0;
		selections->iallRas = 0;
	}
	
	/* if the data from this trial has not been loaded,
	 * load the data.
	 */
	if(!isLoaded[thisTrial->trlNum - 1]) {
		/* malloc space for the dex info structure */
		thisTrial->di = (DexInfo *)malloc(sizeof(DexInfo));
		if(!thisTrial->di) return(-5);
		
		/* copy this trial's rexInfo structure into
		 * the global dexInfo structure
		 */
		copyRexInfo(thisTrial->di, ri);

		/* mark this trial as being loaded */
		isLoaded[thisTrial->trlNum - 1] = 1;
	}

	/* if the data from this trial has been loaded,
	 * just copy the pointer.
	 */
	else {
		/* search for the loaded data from the first trial
		 * loaded to the current trial
		 */
		thisTrial->copy = 1;
		for(j = 0; j < selections->nTrials; j++) {
			if(selections->dataSet[j].trlNum == thisTrial->trlNum) {
				/* copy the pointer to the data */
				thisTrial->di = selections->dataSet[j].di;

				/* stop searching after finding the data */
				break;
			}
		}
	}
	return(0);
}

int countClasses()
{
	SELECTIONS *selections;
	TRIALS *thisTrial;
	CLASSES *thisClass;
	int lastCode, lastSet;
	int ci;
	size_t size;
	int i, j;

	/* get a pointer to the selections structure */
	selections = getSelections();

	/* Count up the number of different ecodes and selection sets
	 * that are in selections->dataSet to identify the number of
	 * classes of data.
	 */
	lastCode = 0;
	lastSet = 0;
	ci = -1;

	/* if this is the first call to countClasses,
	 * allocate the initial block of classes
	 */
	if(!selections->classSet) {
		selections->maxClasses = BLOCK;
		selections->classSet = (CLASSES *)malloc(selections->maxClasses * sizeof(CLASSES));
		if(selections->classSet == NULL) return(-1);

		/* initialize the parameters for the new block of classes */
		for(i = 0; i < selections->maxClasses; i++) {
			thisClass = &selections->classSet[i];

			thisClass->new = 0;
			thisClass->classStart = 0;
			thisClass->classEnd = 0;
			thisClass->nTimes = 0;
			thisClass->maxTimes = BLOCK;
			thisClass->maxCcodes = TRIAL_BLOCK;
			thisClass->maxPts = 2 * BLOCK;
			thisClass->spikeDen.ppSpikeTimes = (int **)NULL;
			thisClass->spikeDen.pSpikeCounts = (int *)NULL;
			thisClass->spikeDen.spkDen.psd_avg = (float *)NULL;
			thisClass->spikeDen.spkDen.psd_se = (float *)NULL;
			thisClass->spikeDen.spkDen.psd_ind = (float **)NULL;
		}
	}

	for(i = 0; i < selections->nTrials; i++) {
		thisTrial = &selections->dataSet[i];
		
		/* if this is the first condition ccode of its type in the sorted data set */
		if((thisTrial->c_code != lastCode) || (thisTrial->setNum != lastSet)) {
			ci++;		/* increment class number counter */

			/* if the number of classes is greater than the
			 * number for which space has been allocated,
			 * add more space.
			 */
			if(ci >= selections->maxClasses) {
				/* increase the number of classes */
				size = (selections->maxClasses + BLOCK) * sizeof(CLASSES);
				selections->classSet = (CLASSES *)realloc(selections->classSet, size);

				/* if space for the classes can't be allocated, return with an error flag. */
				if(selections->classSet == NULL) return(-6);
				selections->maxClasses += BLOCK;

				/* initialize the parameters for the new block of classes */
				for(j = selections->maxClasses; j < selections->maxClasses + BLOCK; j++) {
					thisClass = &selections->classSet[j];
					
					thisClass->new = 0;
					thisClass->classStart = 0;
					thisClass->classEnd = 0;
					thisClass->nTimes = 0;
					thisClass->maxTimes = BLOCK;
					thisClass->maxCcodes = TRIAL_BLOCK;
					thisClass->maxPts = 2 * BLOCK;
					thisClass->spikeDen.ppSpikeTimes = (int **)NULL;
					thisClass->spikeDen.pSpikeCounts = (int *)NULL;
					thisClass->spikeDen.spkDen.psd_avg = (float *)NULL;
					thisClass->spikeDen.spkDen.psd_se = (float *)NULL;
					thisClass->spikeDen.spkDen.psd_ind = (float **)NULL;
				}
			}

			thisClass = &selections->classSet[ci];
			
			/* if this trial is new, set bit 1 of the trial's class new mask */
			thisClass->new |= (thisTrial->new << 1);

			/* initial the condition code and set number that define this class */
			thisClass->c_code = thisTrial->c_code;
			thisClass->setNum = thisTrial->setNum;

			/* set the index of the first member of dataSet in this class */
			thisClass->first = i;

			/* set the index of the last member of dataSet in this class */
			thisClass->last = i;

			/* set the number of condition codes (trials) in this class */
			thisClass->nCcodes = 1;

			/* set the lastCode and lastSet variables */
			lastCode = thisTrial->c_code;
			lastSet = thisTrial->setNum;
		}

		/* if this is not the first condition code of its type in the sorted data set,
		 * increment nCcodes and last counters.
		 */
		else {
			thisClass = &selections->classSet[ci];
			
			/* increment the new trial counter */
			thisClass->new |= (thisTrial->new << 1);

			/* increment the condition code counter */
			thisClass->nCcodes++;

			/* increment the last member index */
			thisClass->last++;
		}

		/* find the unit in this class with the maximum number of spikes */
		thisClass = &selections->classSet[ci];
		for(j = 0; j < thisTrial->di->nUnits; j++) {
			if(thisTrial->di->rexunits->nTimes[j] > thisClass->nTimes) {
				thisClass->nTimes = thisTrial->di->rexunits->nTimes[j];
			}
		}
	}
	ci++;	/* increment ci so that it has the number of classes */
	if(ci > selections->nClasses) selections->nClasses = ci;

	return(0);
}

int
iallRas( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[1];
	SELECTIONS *selections;
	TRIALS *thisTrial;
	CLASSES *thisClass;
	SPKDEN *spikeDen;
	DexInfo *di;
	RexUnits *du;
	int i, j;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* clear the tig list buffer */
	clearTigList();
	setTigListLength(0);

	/* free all of the trial memory */
	selections = getSelections();

	/* loop through all of the trials */
	if(selections->dataSet) {
		for(i = 0; i < selections->nTrials; i++) {
			thisTrial = getTrialData(i);
		
			/* dont try to delete copies of data */
			if(!thisTrial->copy && thisTrial->di) {
				di = thisTrial->di;
				if(di->events) free(di->events);
				if(di->unitCodes) free(di->unitCodes);

				if(di->rexunits) {
					du = di->rexunits;
					if(du->unitTimes) {
						for(j = 0; j < di->nUnits; j++) {
							if(du->unitTimes[j]) free(du->unitTimes[j]);
						}
						free(du->unitTimes);
					}
					if(du->nTimes) free(du->nTimes);
					free(di->rexunits);
				}
				free(thisTrial->di);
			}
		}
		free(selections->dataSet);
		selections->dataSet = (TRIALS *)NULL;
	}

	/* loop through all of the classes */
	if(selections->classSet) {
		for(i = 0; i < selections->nClasses; i++) {
			thisClass = &selections->classSet[i];
			spikeDen = &thisClass->spikeDen;
			if(spikeDen->ppSpikeTimes) {
				for(j = 0; j < thisClass->maxCcodes; j++) {
					if(spikeDen->ppSpikeTimes[j]) free(spikeDen->ppSpikeTimes[j]);
				}
				free(spikeDen->ppSpikeTimes);
			}
			if(spikeDen->spkDen.psd_ind) {
				for(j = 0; j < thisClass->maxCcodes; j++) {
					if(spikeDen->spkDen.psd_ind[j]) free(spikeDen->spkDen.psd_ind[j]);
				}
				free(spikeDen->spkDen.psd_ind);
			}
			if(spikeDen->spkDen.psd_avg) free(spikeDen->spkDen.psd_avg);
			if(spikeDen->spkDen.psd_se) free(spikeDen->spkDen.psd_se);
			if(spikeDen->pSpikeCounts) free(spikeDen->pSpikeCounts);
		}
		free(selections->classSet);
		selections->classSet = (CLASSES *)NULL;
	}

	selections->nTrials = 0;
	selections->nClasses = 0;
	selections->curTrial = 1;
	selections->iallRas = 1;
	
	/* close the rex file */
	rexFileClose();

	/* disable the timer widget */
	PtSetArg(&args[0], Pt_ARG_TIMER_INITIAL, 0, 0);
	PtSetResources(ABW_rastersTimer, 1, args);

	/* clear the tig widget */
	tigClear();

	/* set the title of the display to indicate that the rasters are running */
	PtSetArg(&args[0], Pt_ARG_WINDOW_TITLE, "Rasters Stopped", 0);
	PtSetResources(ABW_base, 1, args);

	return( Pt_CONTINUE );

}


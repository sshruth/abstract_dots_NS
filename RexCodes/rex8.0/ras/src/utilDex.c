/*
 * NOTICE:  This code was developed by employees of the
 * U.S. Government.  This version is distributed,
 * without guarantees or warranties of any kind,
 * for noncommercial use only.  The U.S. Government
 * holds the copyright.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <sys/file.h>
#include <Ap.h>
#include "ph_ras.h"
#include "abimport.h"
#include "proto.h"

/* This subroutine copies the rex information structure for one trial */
void copyRexInfo(DexInfo *di, RexInfo *ri)
{
	int i;

	/* copy discrete values */
	di->ehdr = (char *)NULL;
	di->tStartTime = ri->tStartTime;
	di->tEndTime = ri->tEndTime;
	di->nEvents = ri->nEvents;
	di->events = (EVENT *)NULL;
 	di->nUnits = ri->nUnits;
	di->unitCodes = (int *)NULL;
	di->rexunits = (RexUnits *)NULL;
	di->numTrials = ri->numTrials;
	di->startCode = ri->startCode;

	/* malloc space for, and copy the events list */
	if(di->nEvents > 0) {
 		if((di->events = (EVENT *)malloc(di->nEvents * sizeof(EVENT))) != (EVENT *)NULL) {
			copyRexEvents(di->events, ri->events, di->nEvents);
		}
		else {
			di->nEvents = 0;
		}
	}

	/* copy the units data if there are any */
	if(di->nUnits > 0) {
		/* malloc space for, and copy the unit types list */
   		if((di->unitCodes = (int *)malloc(di->nUnits * sizeof(int))) != (int *)NULL) {
			   for(i = 0; i < di->nUnits; i++) di->unitCodes[i] = ri->unitCodes[i];

			   /* malloc space for, and copy the rex units buffer */
			   if((di->rexunits = (RexUnits *)malloc(sizeof(RexUnits))) != (RexUnits *)NULL) {
				   copyRexUnits(di->rexunits, ri->rexunits, di->nUnits);
			   }
			   else {
				   free(di->unitCodes);
				   di->nUnits = 0;
			   }
		}
		else {
			di->nUnits = 0;
		}
	}

	return;
}

void copyRexEvents(EVENT *dev, EVENT *sev, int n)
{
	int i;

	for(i = 0; i < n; i++) {
		dev[i].e_seqnum = sev[i].e_seqnum;
		dev[i].e_code = sev[i].e_code;
		dev[i].e_key = sev[i].e_key;
	}
	return;
}

void copyRexUnits(RexUnits *du, RexUnits *su, int nUnits)
{
	int i, j;

	if((du->nTimes = (long *)malloc(nUnits * sizeof(long))) != (long *)NULL) {
		if((du->unitTimes = (long **)malloc(nUnits * sizeof(long *))) != (long **)NULL) {
			for(i = 0; i < nUnits; i++) {
				du->nTimes[i] = 0;
				if(!su->nTimes) continue;

				du->nTimes[i] = su->nTimes[i];
				if(du->nTimes[i]) {
					if((du->unitTimes[i] = (long *)malloc(du->nTimes[i] * sizeof(long))) != (long *)NULL) {
						for(j = 0; j < du->nTimes[i]; j++) {
							du->unitTimes[i][j] = su->unitTimes[i][j];
						}
					}
				}
				else {
					if((du->unitTimes[i] = (long *)malloc(10 * sizeof(long))) != (long *)NULL) {
						for(j = 0; j < 10; j++) du->unitTimes[i][j] = 0;
					}
				}
			}
		}
	}

	return;
}

int trialSort(TRIALS *dataArray, int n)
{
	int compareTrials();

	if(n > 1) {
		qsort(dataArray, n, sizeof(TRIALS), compareTrials);
		return(0);
	}
	return(-1);
}

int compareTrials(TRIALS *element1, TRIALS *element2)
{
	if(element1->setNum < element2->setNum) return(-1);
	else if(element1->setNum == element2->setNum) {
		if(element1->c_code < element2->c_code) return(-1);
		else if(element1->c_code == element2->c_code) {
			if(element1->trlNum < element2->trlNum) return(-1);
			else if(element1->trlNum == element2->trlNum) return(0);
			else return(1);
		}
		else return(1);
	}
	else if(element1->setNum > element2->setNum) return(1);
	return(0);
}

char *getDexUnitLabel(DexInfo *di, int unitIndex)
{
	static char name[10];
	
	if(unitIndex > di->nUnits) {
		fprintf(stderr, "Warning, %d invalid unit number\n", unitIndex);
		return((char *)NULL);
	}

	sprintf(name, "%d", di->unitCodes[unitIndex]);
	return(name);
}

int getDexUnitIndex(DexInfo *di, char *unitLabel)
{
	int unit;
	int i;

	/* convert the unit's label into an integer */
	sscanf(unitLabel, "%d", &unit);
	
	for(i = 0; i < di->nUnits; i++) {
		if(unit == di->unitCodes[i]) return(i);
	}
	
	/* if there was no match, return -1 */
	return(-1);
}

int getDexUnitCode(DexInfo *di, int unitIndx)
{
	int i;

	for(i = 0; i < di->nUnits; i++) {
		if(i == unitIndx) return(di->unitCodes[i]);
	}
	
	/* if there was no match, return -1 */
	return(-1);
}

long alignOnEcode(DexInfo *di, long eventTime, int thisCode)
{
	long algnTime;
	int j;

	algnTime = eventTime;
	
	for(j = 0; j < di->nEvents; j++) {
		if(thisCode == di->events[j].e_code) {
			algnTime = di->events[j].e_key;
			break;
		}
	}
	return(algnTime);
}

/* this function returns the time of an ecode
 * relative to the start of the trial
 */
int getDexEcodeTime(DexInfo *di, int ecode)
{
	int i;
	int relTime;

	for(i = 0; i < di->nEvents; i++) {
		if(di->events[i].e_code == ecode) {
			relTime = di->events[i].e_key - di->tStartTime;
			return(relTime);
		}
	}

	/* if ecode is never equal to events->e_code, then an invalid
	 * ecode has been requested
	 */
	return(-1);
}

/* this function returns the time of an ecode
 * relative to the data alignment time
 */
int getDexEcodeRelTime(DexInfo *di, float relAlgnTime, int ecode)
{
	int i;
	int relTime;

	relTime = MISS_TIME;
	for(i = 0; i < di->nEvents; i++) {
		if(di->events[i].e_code == ecode) {
			relTime = (int)((float)di->events[i].e_key - relAlgnTime);
			break;
		}
	}
	return(relTime);
}

void getDexSigNumbs(SIGNAL *sigInfo, int nSignals)
{
	int i;

	for(i = 0; i < nSignals; i++) {
		sscanf(sigInfo[i].sigName, "%d", &sigInfo[i].sigNum);
	}
}

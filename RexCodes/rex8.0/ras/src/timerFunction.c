#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <sys/neutrino.h>
#include <inttypes.h>

#include "timerFunction.h"

/* private global variables */
int tfNumbHistos;
double usecPerTick;
TFHISTO *tfHistos;

/* Public functions definitions */
void timerFunction(int number)
{
	uint64_t start, stop, interval;
	
	tfNumbHistos = number;
	tfHistos = (TFHISTO *)malloc(number * sizeof(TFHISTO));

	// calibrate the timer
	start = ClockCycles();
	sleep(1);
	stop = ClockCycles();
	interval = stop - start;
	usecPerTick = 1000000.0 / (double)interval;
}

void clearTimerFunction()
{
	int i;
	for(i = 0; i < tfNumbHistos; i++) {
		if(tfHistos[i].tfBins) free(tfHistos[i].tfBins);
	}
	free(tfHistos);
}

void initThisTimerHisto(int index, char *label, int bins, int width)
{
	TFBIN *thsBin;
	int i;

	strcpy(tfHistos[index].label, label);
	tfHistos[index].numBins = 0;
	tfHistos[index].maxBins = bins;
	tfHistos[index].binWidth = width;
	tfHistos[index].tfBins = (TFBIN *)malloc((tfHistos[index].maxBins + 1) * sizeof(TFBIN));
	for(i = 0; i <= tfHistos[index].maxBins; i++) {
		thsBin = &tfHistos[index].tfBins[i];
		thsBin->centerTime = 0;
		thsBin->minTime = MAXLONG;
		thsBin->maxTime = MINLONG;
		thsBin->numberTimes = 0;
	}
}

void startTimer(int index)
{
	tfHistos[index].tfStartTime = ClockCycles();
}

void stopTimer(int index)
{
	TFHISTO *thsHisto;
	TFBIN *thsBin;
	uint64_t stop, elapsdTime;
	double interval;
	int i, counted;

	stop = ClockCycles();                  // get the ending time

	thsHisto = &tfHistos[index];
	elapsdTime = stop - thsHisto->tfStartTime;
	interval = usecPerTick * (double)elapsdTime;

	counted = 0;
	for(i = 0; i < thsHisto->numBins; i++) {
		thsBin = &thsHisto->tfBins[i];
		if((interval >= thsBin->leftEdge) && (interval < thsBin->rightEdge)) {
			counted = 1;
			thsBin->numberTimes++;
			if(interval < thsBin->minTime) thsBin->minTime = interval;
			if(interval > thsBin->maxTime) thsBin->maxTime = interval;
		}
	}
	if(!counted) {
		if(thsHisto->numBins < thsHisto->maxBins) {
			thsBin = &thsHisto->tfBins[thsHisto->numBins];
			thsBin->centerTime = interval;
			thsBin->leftEdge = thsBin->centerTime - (thsHisto->binWidth / 2.0);
			thsBin->rightEdge = thsBin->centerTime + (thsHisto->binWidth / 2.0);
			thsBin->minTime = interval;
			thsBin->maxTime = interval;
			thsBin->numberTimes = 1;
			thsHisto->numBins++;
		}
		else {          // fill in the overflow bin
			thsBin = &thsHisto->tfBins[thsHisto->maxBins];
			thsBin->numberTimes++;
			if(interval < thsBin->minTime) thsBin->minTime = interval;
			if(interval > thsBin->maxTime) thsBin->maxTime = interval;
		}
	}
}

void printTimes(void)
{
	TFHISTO *thsHisto;
	TFBIN *thsBin;
	TFBIN tmpBin;
	double minVal;
	int lowest = 0;
	int i;
	int j;
	int k;

	for(i = 0; i < tfNumbHistos; i++) {
		thsHisto = &tfHistos[i];
		fprintf(stderr, "Histogram %d %s number filled bins = %d\n", i, thsHisto->label, thsHisto->numBins);
		j = thsHisto->numBins;

		// sort the bins in ascending order and print each
		while(j) {
			minVal = MAXLONG;
			for(k = 0; k < j; k++) {
				if(thsHisto->tfBins[k].centerTime < minVal) {
					minVal = thsHisto->tfBins[k].centerTime;
					lowest = k;
				}
			}
			thsBin = &thsHisto->tfBins[lowest];
			fprintf(stderr, "\ttime = %.3f, n = %d min = %.3f max = %.3f\n",
					thsBin->centerTime, thsBin->numberTimes, thsBin->minTime, thsBin->maxTime);
                        
			tmpBin.centerTime = thsHisto->tfBins[j - 1].centerTime;
			tmpBin.minTime = thsHisto->tfBins[j - 1].centerTime;
			tmpBin.maxTime = thsHisto->tfBins[j - 1].centerTime;
			tmpBin.leftEdge = thsHisto->tfBins[j - 1].leftEdge;
			tmpBin.rightEdge = thsHisto->tfBins[j - 1].rightEdge;
			tmpBin.numberTimes = thsHisto->tfBins[j - 1].numberTimes;
                        
			thsHisto->tfBins[j - 1].centerTime = thsHisto->tfBins[lowest].centerTime;
			thsHisto->tfBins[j - 1].minTime = thsHisto->tfBins[lowest].centerTime;
			thsHisto->tfBins[j - 1].maxTime = thsHisto->tfBins[lowest].centerTime;
			thsHisto->tfBins[j - 1].leftEdge = thsHisto->tfBins[lowest].leftEdge;
			thsHisto->tfBins[j - 1].rightEdge = thsHisto->tfBins[lowest].rightEdge;
			thsHisto->tfBins[j - 1].numberTimes = thsHisto->tfBins[lowest].numberTimes;

			thsHisto->tfBins[lowest].centerTime = tmpBin.centerTime;
			thsHisto->tfBins[lowest].minTime = tmpBin.minTime;
			thsHisto->tfBins[lowest].maxTime = tmpBin.maxTime;
			thsHisto->tfBins[lowest].leftEdge = tmpBin.leftEdge;
			thsHisto->tfBins[lowest].rightEdge = tmpBin.rightEdge;
			thsHisto->tfBins[lowest].numberTimes = tmpBin.numberTimes;
			j--;
		}
		// print the overflow bin
		thsBin = &thsHisto->tfBins[thsHisto->maxBins];
		if(thsBin->numberTimes) {
			fprintf(stderr, "\toverflow, n = %d min = %.3f max = %.3f\n", 
					thsBin->numberTimes, thsBin->minTime, thsBin->maxTime);
		}
	}
}

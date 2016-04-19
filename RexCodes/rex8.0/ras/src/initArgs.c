/* Y o u r   D e s c r i p t i o n                       */
/*                            AppBuilder Photon Code Lib */
/*                                         Version 1.13X */

/* Standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* Toolkit headers */
#include <Ph.h>
#include <Pt.h>
#include <Ap.h>

/* Local headers */
#include "ph_ras.h"
#include "abimport.h"
#include "proto.h"

/* Application Options string */
const char ApOptions[] =
	AB_OPTIONS ""; /* Add your options in the "" */

SELECTIONS selections;
RASTER rasters;

int initArgs(int argc, char *argv[])
{
	/* eliminate 'unreferenced' warnings */
	argc = argc, argv = argv;

	/*
	 * Unbuffer stdout for error printf's.
	 */
	setbuf(stdout, NULL);

	/*
	 * Call routine that establishes REX initializations.
	 */
	rex_main();

	initSelections(&selections);
	initRasters(&rasters);
	initTig();

	return( Pt_CONTINUE );
}

void initSelections(SELECTIONS *selections)
{
	/* eliminate 'unreferenced' warnings */
	selections = selections;

	selections->selectSets = (SELECTSET *)NULL;
	selections->nSelectSets = 0;
	selections->curSetIndx = -1;
	selections->newSelects = 1;
	selections->startCode = 1001;
	selections->maxSampRate = 1000;
	selections->preTime = 0;
	selections->dataSet = (TRIALS *)NULL;
	selections->classSet = (CLASSES *)NULL;

	/* initialize the trials and ecode class counters */
	selections->nTrials = 0;
	selections->nClasses = 0;
	selections->curTrial = 1;
	selections->iallRas = 0;
}

SELECTIONS *getSelections(void)
{
	return(&selections);
}

SELECTSET *addSelectSet(SELECTSET *selectSets, int n)
{
	if(!n) return((SELECTSET *)NULL);
	
	if(!selectSets) {
		selectSets = (SELECTSET *)malloc(n * sizeof(SELECTSET));
	}
	else {
		selectSets = (SELECTSET *)realloc(selectSets, (n * sizeof(SELECTSET)));
	}
	return(selectSets);
}

SELECTSET *getSelectSet(int indx)
{
	if(!selections.selectSets) return((SELECTSET *)NULL);
	else return(&selections.selectSets[indx]);
}

TRIALS *getTrialData(int indx)
{
	if(!selections.dataSet) return((TRIALS *)NULL);
	else return(&selections.dataSet[indx]);
}

void initRasters(RASTER *rasters)
{
	rasters->pages = (PAGE *)NULL;
	rasters->nPages = 0;
	rasters->curPageIndx = -1;
	rasters->curDrawPage = 0;
	rasters->newRasters = 1;
}

void initTig()
{
	extern LsrTig lsrTig;
	int i;

	lsrTig.tigList = 0;
	lsrTig.tigListPtr = 0;
	lsrTig.lastTigListPtr = 0;
	lsrTig.tigListLen = 0;
	lsrTig.tigListEnd = &lsrTig.tigListLen;
	lsrTig.lastTigListEnd = lsrTig.tigListEnd;
	for(i = 0; i < NPAGES; i++) lsrTig.pageStart[i] = 0;
	lsrTig.currentPage = 0;
	lsrTig.lastPage = 0;
	lsrTig.clear = 1;
	lsrTig.tileCount = 0;
	lsrTig.currentPoint.x = -1;
	lsrTig.currentPoint.y = -1;
	lsrTig.lastPoint.x = -1;
	lsrTig.lastPoint.y = -1;
	lsrTig.pen = UP;
	lsrTig.tigColors[BLACK] = Pg_BLACK;
	lsrTig.tigColors[BROWN] = PgRGB(255, 77, 77);
	lsrTig.tigColors[RED] = Pg_RED;
	lsrTig.tigColors[ORANGE] = PgRGB(255, 128, 0);
	lsrTig.tigColors[YELLOW] = Pg_YELLOW;
	lsrTig.tigColors[GREEN] = Pg_GREEN;
	lsrTig.tigColors[BLUE] = Pg_BLUE;
	lsrTig.tigColors[VIOLET] = Pg_MAGENTA;
	lsrTig.tigColors[GREY] = Pg_MGRAY;
	lsrTig.tigColors[WHITE] = Pg_WHITE;
	lsrTig.tigColors[CYAN] = Pg_CYAN;
	lsrTig.tigColors[PINK] = PgRGB(255, 150, 150);
}

RASTER *getRasters(void)
{
	return(&rasters);
}

PAGE *addRasterPage(PAGE *pages, int n)
{
	if(!n) return((PAGE *)NULL);

	if(!pages) {
		pages = (PAGE *)malloc(n * sizeof(PAGE));
	}
	else {
		pages = (PAGE *)realloc(pages, (n * sizeof(PAGE)));
	}
	return(pages);
}

PAGE *getRasterPage(int indx)
{
	if(!rasters.pages) return((PAGE *)NULL);
	else return(&rasters.pages[indx]);
}

CELL *getRasterCell(CELL *cellsInfo, int indx)
{
	return(&cellsInfo[indx]);
}

SIGNAL *getRasterSignal(SIGNAL *sigInfo, int indx)
{
	return(&sigInfo[indx]);
}

/* Y o u r   D e s c r i p t i o n                       */
/*                            AppBuilder Photon Code Lib */
/*                                         Version 2.01  */

/* Standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* Local headers */
#include "ablibs.h"
#include "ph_xcorr.h"
#include "abimport.h"
#include "proto.h"
#include "timerFunction.h"

/* Application Options string */
const char ApOptions[] =
	AB_OPTIONS ""; /* Add your options in the "" */

int initArgs( int argc, char *argv[] )
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

	initTig();

	//	timerFunction(3);

	return( Pt_CONTINUE );
}

void initTig()
{
	extern LABEL pageLabel;
	extern GRAPH pageGraph;
	extern EVENT_P eventTop;
	extern LsrTig lsrTig;
	extern int trigUnit;
	extern int corrUnit;
	extern int *unitTimes[MAX_LINES + 1];

	int i;

	eventTop = (EVENT_P)&i_b->bevent[EBUFNUM];
	
	lsrTig.tigList = (char *)NULL;
	lsrTig.tigListPtr = 0;
	lsrTig.lastTigListPtr = 0;
	lsrTig.tigListLen = 0;
	lsrTig.tigListEnd = &lsrTig.tigListLen;
	lsrTig.lastTigListEnd = lsrTig.tigListEnd;
	for(i = 0; i < NPAGES; i++) lsrTig.pageStart[i] = 0;
	lsrTig.currentPage = 0;
	lsrTig.lastPage = 0;
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

	pageLabel.x = 0.10;
	pageLabel.y = 0.97;
	pageLabel.sz = 20;
	pageLabel.font = 12;
	if(trigUnit == corrUnit) {
		sprintf(pageLabel.label, "Unit %d Autocorrelation", trigUnit);
	}
	else {
		sprintf(pageLabel.label, "%d X %d", trigUnit, corrUnit);
	}

	pageGraph.start = -100;
	pageGraph.interval = 200;
	pageGraph.maxFreq = 500;
	pageGraph.timeMark = 20;
	pageGraph.Nfont = 12;
	pageGraph.Nsz = 15;
	strcat(pageGraph.xFormat, "%i");

	for(i = 0; i <= MAX_LINES; ++i) {
		unitTimes[i] = (int *)NULL;
	}
}

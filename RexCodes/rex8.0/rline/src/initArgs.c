/* Y o u r   D e s c r i p t i o n                       */
/*                            AppBuilder Photon Code Lib */
/*                                         Version 0.83B */

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
#include "abimport.h"
#include "ph_rline.h"
#include "proto.h"
#include "timerFunction.h"

RT_DISP rtDisp[RT_DISP_LEN];

TRACE rlTraces[RL_MAXCHAN + 2] = {
	{1, -3, 128},
	{1, -3, 120},
	{0, -3, 112},
	{0, -3, 104},
	{0, -5, 96},
	{0,	-5,	88},
	{0,	-3,	80},
	{0,	-3,	72},
	{1,	 0,	64},	/* running line bar */
	{1,	 0,	0},		/* spikes */
};

extern lsrRunLine runLine;

int initArgs( int argc, char *argv[] )
{
	/* eliminate 'unreferenced' warnings */
	argc = argc, argv = argv;

	/*set default properties for the running line window */
	runLine.fill_color = Pg_BLACK;
	runLine.color = Pg_BLACK;
	runLine.n_data = 0;
	runLine.timeBase = 16;
	runLine.scaleFactor = 1;
	runLine.triggerLevel = 0;
	runLine.trigger = 0;
	runLine.display = 0;
	runLine.mode = 0;
	runLine.modeValue = 0;
	runLine.dispReset = 0;
	runLine.drawStart.x = -1;
	runLine.drawStart.y = -1;
	runLine.drawEnd.x = -1;
	runLine.drawEnd.y = -1;
	runLine.timeCounter = 16;

	/*
	 * Call routine that establishes REX initializations.
	 */
	rex_main();

	return( Pt_CONTINUE );
}


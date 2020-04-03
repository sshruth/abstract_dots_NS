/* Y o u r   D e s c r i p t i o n                       */
/*                            AppBuilder Photon Code Lib */
/*                                         Version 1.14C */

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
#include "ph_ewind.h"
#include "abimport.h"
#include "proto.h"

/* Application Options string */
const char ApOptions[] =
	AB_OPTIONS ""; /* Add your options in the "" */

RT_DISP rtDisp[RT_DISP_LEN];

char *scaleMag0[] = {
	"",
	"-81.9",
	"-61.4",
	"-40.9",
	"-20.5",
	"0",
	"20.5",
	"40.9",
	"61.4",
	"81.9",
	""
};
char *scaleMag1[] = {
	"",
	"-41.0",
    "-30.7",
    "-20.5",
    "-10.2",
    "0",
    "10.2",
    "20.5",
    "30.7",
    "41.0",
	""
};
char *scaleMag2[] = {
	"",
    "-20.5",
    "-15.4",
    "-10.2",
    "-5.1",
    "0",
    "5.1",
    "10.2",
    "15.4",
    "20.5",
	""
};
char *scaleMag3[] = {
	"",
    "-10.0",
    "-7.5",
    "-5.0",
    "-2.5",
    "0",
    "2.5",
    "5.0",
    "7.5",
    "10.0",
	""
};
char *scaleMag4[] = {
	"",
    "-5.12",
    "-3.84",
    "-2.56",
    "-1.28",
    "0",
    "1.28",
    "2.56",
    "3.84",
    "5.12",
	""
};
char *scaleMag5[] = {
	"",
    "-2.56",
    "-1.92",
    "-1.28",
    "-0.64",
    "0",
    "0.64",
    "1.28",
    "1.92",
    "2.56",
	""
};
CURS_DES cursDescrip[WDI_NCURS];

extern lsrDispWindow dispWind;

int initArgs( int argc, char *argv[] )
{
	/* eliminate 'unreferenced' warnings */
	argc = argc, argv = argv;

	/* set default properties for the eye window */
	dispWind.fill_color = Pg_BLACK;
	dispWind.color = Pg_BLACK;
	dispWind.n_data = 0;
	dispWind.refreshRate = 0;
	dispWind.grid = 1;
	dispWind.scaleFactor = 1;
	dispWind.trigger = 0;
	dispWind.triggerLevel = 0;
	dispWind.autoRefresh = 1;
	dispWind.refreshCounter = 0;
	dispWind.intervalCounter = 0;
	dispWind.autoInterval = 0;
	dispWind.eyeMin = -(1024 >> dispWind.scaleFactor);
	dispWind.eyeMax = (1024 >> dispWind.scaleFactor);
	dispWind.eyeRange = (dispWind.eyeMax - dispWind.eyeMin) ? (dispWind.eyeMax - dispWind.eyeMin) : 1;

	/*
	 * Call routine that establishes REX initializations.
	 */
	rex_main();

	return( Pt_CONTINUE );
}


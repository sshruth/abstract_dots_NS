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
#include "ph_procSwitch.h"
#include "abimport.h"
#include "proto.h"

/* Application Options string */
const char ApOptions[] =
	AB_OPTIONS ""; /* Add your options in the "" */

int rexSession = 0;

int initArgs( int argc, char *argv[] )
{

	/* eliminate 'unreferenced' warnings */
	argc = argc, argv = argv;

	rex_main();

	return( Pt_CONTINUE );
}


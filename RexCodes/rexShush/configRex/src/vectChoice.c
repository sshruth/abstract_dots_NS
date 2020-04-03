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
#include "configRex.h"
#include "abimport.h"
#include "proto.h"

int vectChoice( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* figure out which toggle was selected */
	if(ApName(widget) == ABN_vect3Toggle) a2dvect = 3;
	if(ApName(widget) == ABN_vect4Toggle) a2dvect = 4;
	if(ApName(widget) == ABN_vect5Toggle) a2dvect = 5;
	if(ApName(widget) == ABN_vect7Toggle) a2dvect = 7;
	if(ApName(widget) == ABN_vect10Toggle) a2dvect = 10;
	if(ApName(widget) == ABN_vect11Toggle) a2dvect = 11;
	if(ApName(widget) == ABN_vect12Toggle) a2dvect = 12;
	if(ApName(widget) == ABN_vect15Toggle) a2dvect = 15;

	return( Pt_CONTINUE );
}


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

int basePortChoice( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the value entered */
	PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, 0, 0);
	PtGetResources(widget, 1, &arg);

	/* figure out which port was set */
	if(ApName(widget) == ABN_adBasePortInteger) a2dbase = (int)arg.value;
	else if(ApName(widget) == ABN_daBasePortInteger) d2abase = (int)arg.value;
	else if(ApName(widget) == ABN_pcdioBasePortInteger) diobase = (int)arg.value;

	return( Pt_CONTINUE );
}


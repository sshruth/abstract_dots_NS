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

int pstopSwitchChoice( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* figure out which toggle was selected */
	if(ApName(widget) == ABN_hardwareYesToggle) hardwareSwitch = YES;
	else if(ApName(widget) == ABN_hardwareNoToggle) hardwareSwitch = NO;
	return( Pt_CONTINUE );
}


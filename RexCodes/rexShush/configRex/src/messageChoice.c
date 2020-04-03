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

int messageChoice( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;
	long *flags;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the state of the widget */
	PtSetArg(&arg, Pt_ARG_FLAGS, &flags, 0);
	PtGetResources(widget, 1, &arg);

	/* figure out which toggle was selected */
	if(ApName(widget) == ABN_pcmessageToggle) {
		if(*flags & Pt_SET) pcmessageSwitch = YES;
		else pcmessageSwitch = NO;
	}
	else if(ApName(widget) == ABN_pcs_socketToggle) {
		if(*flags & Pt_SET) socketSwitch = YES;
		else socketSwitch = NO;
	}

	return( Pt_CONTINUE );
}


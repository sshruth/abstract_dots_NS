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

int dioChoice( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[2];
	
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* figure out which dio was selected */
	if(ApName(widget) == ABN_isaDioToggle) {
		dioboard = ISA_DIO;

		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SELECTABLE);
		PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_FALSE, Pt_GHOST);
		PtSetResources(ABW_pcdioBasePortInteger, 2, args);
	}
	else if(ApName(widget) == ABN_pciDioToggle) {
		dioboard = PCI_DIO;

		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SELECTABLE);
		PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_TRUE, Pt_GHOST);
		PtSetResources(ABW_pcdioBasePortInteger, 2, args);
	}

	return( Pt_CONTINUE );
}


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
#include "configRex.h"
#include "abimport.h"
#include "proto.h"

extern int d2aboard;

int d2aChoice( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[2];
	
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	
	if(ApName(widget) == ABN_isaD2AToggle) {
		d2aboard = ISA_DtoA;

		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SELECTABLE);
		PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_FALSE, Pt_GHOST);
		PtSetResources(ABW_daBasePortInteger, 2, args);
	}
	else if(ApName(widget) == ABN_pciD2AToggle) {
		d2aboard = PCI_DtoA;

		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SELECTABLE);
		PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_TRUE, Pt_GHOST);
		PtSetResources(ABW_daBasePortInteger, 2, args);
	}

	return( Pt_CONTINUE );
}


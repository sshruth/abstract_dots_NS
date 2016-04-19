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

int atodChoice( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[2];

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* figure out which toggle was selected */
	if(ApName(widget) == ABN_NIATMIOXToggle) {
		a2dboard = NIATMIOX;

		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SELECTABLE);
		PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_FALSE, Pt_GHOST);

		PtSetResources(ABW_vect3Toggle, 2, args);
		PtSetResources(ABW_vect4Toggle, 2, args);
		PtSetResources(ABW_vect5Toggle, 2, args);
		PtSetResources(ABW_vect7Toggle, 2, args);
		PtSetResources(ABW_vect10Toggle, 2, args);
		PtSetResources(ABW_vect11Toggle, 2, args);
		PtSetResources(ABW_vect12Toggle, 2, args);
		PtSetResources(ABW_vect15Toggle, 2, args);

		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_GHOST);
		PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_TRUE, Pt_GETS_FOCUS);
		PtSetResources(ABW_adBasePortInteger, 2, args);
	}
	else if(ApName(widget) == ABN_ANDAS12Toggle) {
		a2dboard = ANDAS12;

		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SELECTABLE);
		PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_FALSE, Pt_GHOST);

		PtSetResources(ABW_vect5Toggle, 2, args);
		PtSetResources(ABW_vect7Toggle, 2, args);
		PtSetResources(ABW_vect10Toggle, 2, args);
		PtSetResources(ABW_vect11Toggle, 2, args);
		PtSetResources(ABW_vect15Toggle, 2, args);

		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SELECTABLE);
		PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_TRUE, Pt_GHOST);
		PtSetResources(ABW_vect3Toggle, 2, args);
		PtSetResources(ABW_vect4Toggle, 2, args);
		PtSetResources(ABW_vect12Toggle, 2, args);

		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_GHOST);
		PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_TRUE, Pt_GETS_FOCUS);
		PtSetResources(ABW_adBasePortInteger, 2, args);
	}
	else if(ApName(widget) == ABN_NIPCIToggle) {
		a2dboard = PCI_AtoD;

		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SELECTABLE);
		PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_TRUE, Pt_GHOST);

		PtSetResources(ABW_vect3Toggle, 2, args);
		PtSetResources(ABW_vect4Toggle, 2, args);
		PtSetResources(ABW_vect5Toggle, 2, args);
		PtSetResources(ABW_vect7Toggle, 2, args);
		PtSetResources(ABW_vect10Toggle, 2, args);
		PtSetResources(ABW_vect11Toggle, 2, args);
		PtSetResources(ABW_vect12Toggle, 2, args);
		PtSetResources(ABW_vect15Toggle, 2, args);
		
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_GHOST);
		PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_FALSE, Pt_GETS_FOCUS);
		PtSetResources(ABW_adBasePortInteger, 2, args);
	}
	return( Pt_CONTINUE );
}


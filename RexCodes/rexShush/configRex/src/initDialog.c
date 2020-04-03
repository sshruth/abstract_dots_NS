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

int initDialog( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[2];

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	/* if the board type is defined in the cnf.h file */
	if(a2dboard == NIATMIOX) {
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(ABW_NIATMIOXToggle, 1, &args[0]);

		/* enable the appropriate vector toggles */
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
	}
	else if(a2dboard == ANDAS12) {
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(ABW_ANDAS12Toggle, 1, &args[0]);

		/* enable the appropriate vector toggles */
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SELECTABLE);
		PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_FALSE, Pt_GHOST);
		PtSetResources(ABW_vect5Toggle, 2, args);
		PtSetResources(ABW_vect7Toggle, 2, args);
		PtSetResources(ABW_vect10Toggle, 2, args);
		PtSetResources(ABW_vect11Toggle, 2, args);
		PtSetResources(ABW_vect15Toggle, 2, args);
	}
	else if(a2dboard == PCI_AtoD) {
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(ABW_NIPCIToggle, 1, &args[0]);
		
		/* enable the appropriate vector toggles */
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

	/* set the a/d vector toggle */
	PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
	switch(a2dvect) {
	case 3:
		PtSetResources(ABW_vect3Toggle, 1, &args[0]);
		break;
	case 4:
		PtSetResources(ABW_vect4Toggle, 1, &args[0]);
		break;
	case 5:
		PtSetResources(ABW_vect5Toggle, 1, &args[0]);
		break;
	case 7:
		PtSetResources(ABW_vect7Toggle, 1, &args[0]);
		break;
	case 10:
		PtSetResources(ABW_vect10Toggle, 1, &args[0]);
		break;
	case 11:
		PtSetResources(ABW_vect11Toggle, 1, &args[0]);
		break;
	case 12:
		PtSetResources(ABW_vect12Toggle, 1, &args[0]);
		break;
	case 15:
		PtSetResources(ABW_vect15Toggle, 1, &args[0]);
		break;
	}

	/* set the a/d base port as defined in the cnf.h file */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, a2dbase, 0);
	PtSetResources(ABW_adBasePortInteger, 1, &args[0]);

	/* set the SMI tracker toggles */
	PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
	if(smiSwitch == YES) PtSetResources(ABW_smiYesToggle, 1, &args[0]);
	else if(smiSwitch == NO) PtSetResources(ABW_smiNoToggle, 1, &args[0]);

	/* set the dio board as defined in the cnf.h file */
	if(dioboard == ISA_DIO) {
	}
	else if(dioboard == PCI_DIO) {
	}
	/* set the dio base port */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, diobase, 0);
	PtSetResources(ABW_pcdioBasePortInteger, 1, &args[0]);

	/* set the d/a board as defined in the cnf.h file */
	if(d2aboard == ISA_DtoA) {
	}
	else if(d2aboard == PCI_DtoA) {
	}
	/* set the d/a base port as defined in the cnf.h file */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, d2abase, 0);
	PtSetResources(ABW_daBasePortInteger, 1, &args[0]);

	/* set the unit toggles */
	PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
	if(unit == LATCH_UNIT) PtSetResources(ABW_latchToggle, 1, &args[0]);
	else if(unit == MEXWEX_UNIT) PtSetResources(ABW_mexwexToggle, 1, &args[0]);
	else if(unit == PLEXON_UNIT) PtSetResources(ABW_plexonToggle, 1, &args[0]);
	else if(unit == FAKE_UNIT) PtSetResources(ABW_fakeToggle, 1, &args[0]);

	/* set the plexon ecode toggles */
	if(plexEcodeSwitch == YES) PtSetResource(ABW_plexEcodeYesToggle, Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
	else if(plexEcodeSwitch == NO) PtSetResource(ABW_plexEcodeNoToggle, Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
	
	/* set the message toggles */
	PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
	if(pcmessageSwitch == YES) PtSetResources(ABW_pcmessageToggle, 1, &args[0]);
	if(socketSwitch == YES) PtSetResources(ABW_pcs_socketToggle, 1, &args[0]);

	/* set the hardward switch toggles */
	PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
	if(hardwareSwitch == YES) PtSetResources(ABW_hardwareYesToggle, 1, &args[0]);
	else if(hardwareSwitch == NO) PtSetResources(ABW_hardwareNoToggle, 1, &args[0]);
	return( Pt_CONTINUE );
}


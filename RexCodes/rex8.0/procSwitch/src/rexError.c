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

PtWidget_t *pRexErrorDialog = (PtWidget_t *)NULL;

int initRexError( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	pRexErrorDialog = link_instance;

	return( Pt_CONTINUE );
}

int ph_rxerr(char *s, int ptx)
{
	char string[120];

	if(!pRexErrorDialog) {
		ApCreateModule(ABM_rexErrorDialog, ABW_base, NULL);
	}

	sprintf(string, "%s\n", s);

	PtMultiTextModifyText(ABW_rexErrorText, 0, 0, -1, string, strlen(string), 0, 0);
	return(0);
}

int closeErrorDialog( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PhWindowEvent_t *winEvent;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get pointer to the window event structure */
	winEvent = (PhWindowEvent_t *)cbinfo->cbdata;

	if(winEvent->event_f == Ph_WM_CLOSE) {
		pRexErrorDialog = (PtWidget_t *)NULL;
	}

	return( Pt_CONTINUE );
}

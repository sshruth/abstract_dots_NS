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
#include "ph_toolBar.h"
#include "abimport.h"
#include "proto.h"

PtWidget_t *pRexMessageDialog = (PtWidget_t *)NULL;

int initRexMessage( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	pRexMessageDialog = link_instance;

	return( Pt_CONTINUE );
}

void rxHelpMessage(char *s)
{
	if(!s) return;

	if(!pRexMessageDialog) {
		ApCreateModule(ABM_rexMessageDialog, ABW_base, NULL);
	}

	PtMultiTextModifyText(ABW_rexMessageText, 0, 0, -1, s, strlen(s), 0, 0);
	return;
}

int closeMessageDialog( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PhWindowEvent_t *winEvent;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	winEvent = (PhWindowEvent_t *)cbinfo->cbdata;

	if(winEvent->event_f == Ph_WM_CLOSE) {
		pRexMessageDialog = (PtWidget_t *)NULL;
	}

	return( Pt_CONTINUE );
}


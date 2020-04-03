/* Y o u r   D e s c r i p t i o n                       */
/*                            AppBuilder Photon Code Lib */
/*                                         Version 1.13X */

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
#include "ph_ras.h"
#include "abimport.h"
#include "proto.h"

int nextPageProc( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[1];
	PtWidget_t *thisModule;
	PtWidget_t *prior;
	SELECTIONS *selections;
	RASTER *rasters;
	int i;
	char title[64];

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get a pointer to the rasters structure */
	rasters = getRasters();

	/* get a pointer to the module that contains this widget */
	thisModule = ApGetInstance(widget);

	if(rasters->curDrawPage < (rasters->nPages - 1)) {
		rasters->curDrawPage++;

		/* get pointer to the prior page button */
		prior = ApGetWidgetPtr(thisModule, ABN_tigPrior);
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_GHOST | Pt_BLOCKED);
		PtSetResources(prior, 1, args);

		/* if this is the last page, disable the next button */
		if(rasters->curDrawPage >= (rasters->nPages - 1)) {
			widget = ApGetWidgetPtr(thisModule, ABN_tigNext);
			PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_GHOST | Pt_BLOCKED);
			PtSetResources(widget, 1, args);
		}
	}

	/* set the title of the display page */
	sprintf(title, "Raster Page %d", rasters->curDrawPage + 1);
	PtSetArg(&args[0], Pt_ARG_WINDOW_TITLE, title, 0);
	PtSetResources(thisModule, 1, args);

	/* set the new data flag in all classes */
	selections = getSelections();
	for(i = 0; i < selections->nClasses; i++) selections->classSet[i].new = 1;

	/* call the raster draw function */
	drawRasters();

	return( Pt_CONTINUE );
}

int priorPageProc( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[1];
	PtWidget_t *thisModule;
	PtWidget_t *next;
	SELECTIONS *selections;
	RASTER *rasters;
	int i;
	char title[64];

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get a pointer to the rasters structure */
	rasters = getRasters();

	/* get a pointer to the module that contains this widget */
	thisModule = ApGetInstance(widget);

	if(rasters->curDrawPage > 0) {
		rasters->curDrawPage--;

		/* get pointer to the prior page button */
		next = ApGetWidgetPtr(thisModule, ABN_tigNext);
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_GHOST | Pt_BLOCKED);
		PtSetResources(next, 1, args);

		/* if this is the last page, disable the next button */
		if(rasters->curDrawPage <= 0) {
			widget = ApGetWidgetPtr(thisModule, ABN_tigPrior);
			PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_GHOST | Pt_BLOCKED);
			PtSetResources(widget, 1, args);
		}
	}

	/* set the title of the display page */
	sprintf(title, "Raster Page %d", rasters->curDrawPage + 1);
	PtSetArg(&args[0], Pt_ARG_WINDOW_TITLE, title, 0);
	PtSetResources(thisModule, 1, args);

	/* set the new data flag in all classes */
	selections = getSelections();
	for(i = 0; i < selections->nClasses; i++) selections->classSet[i].new = 1;

	/* call the raster draw function */
	drawRasters();

	return( Pt_CONTINUE );
}

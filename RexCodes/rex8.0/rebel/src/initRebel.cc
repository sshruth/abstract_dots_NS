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
#include "rebel.h"
#include "abimport.h"
#include "proto.h"

/* Application Options string */
const char ApOptions[] =
	AB_OPTIONS "r:"; /* Add your options in the "" */

rebelFile rFile;
rebelEdit rEdit;
rebelResource *rResources;
rebelWidget *rWidgets;
rebelApp *app = (rebelApp *)NULL;
PhPoint_t rOrigin;
PhDim_t rSize;
PhPoint_t bkgdOrigin;
PhDim_t bkgdSize;
PhRid_t rRid;
ApDBase_t *rPictures;
string rebelName;
PtCallbackInfo_t gcbinfo;

int
initArgs( int argc, char *argv[] )
{

	/* eliminate 'unreferenced' warnings */
	argc = argc, argv = argv;
	extern char *optarg;

	int opt;
	while((opt = getopt(argc, argv, ApOptions)) != -1) {
		switch(opt) {
		case 'r':
			rebelName = optarg;
			break;
		default:
			break;
		}
	}

	rPictures = ApOpenDBase(ABM_rebelPictures);

	return( Pt_CONTINUE );

}


int
initGui( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	if(apinfo->reason == ABR_PRE_REALIZE) {
	}
	else {
		rWidgets = new rebelWidget();
		rResources = new rebelResource();
		rRid = PtWidgetRid(ABW_rebelBkgd);

		if(!rebelName.empty()) {
			rFile.fileName(rebelName);
			new rebelApp(rebelName, &gcbinfo);

			char title[128];
			strcpy(title, "Rebel ");
			strcat(title, rebelName.c_str());
			PtSetResource(ABW_rebelBaseWindow, Pt_ARG_WINDOW_TITLE, title, 0);
		}
	}
	return( Pt_CONTINUE );

}


int
resize( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// get the latest position of the background widget
	PtGetAbsPosition(ABW_rebelBkgd, &bkgdOrigin.x, &bkgdOrigin.y);

	// get the size of the base window
	PhDim_t *s;
	PtGetResource(widget, Pt_ARG_DIM, &s, 0);
	rSize = *s;

	PhPoint_t *p;
	PtGetResource(widget, Pt_ARG_POS, &p, 0);
	rOrigin = *p;

	// get the latest size of the background widget
	PtGetResource(ABW_rebelBkgd, Pt_ARG_DIM, &s, 0);
	bkgdSize = *s;

	return( Pt_CONTINUE );

}

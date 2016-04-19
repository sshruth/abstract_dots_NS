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


rebelWidget::rebelWidget()
{
	PtCallbackInfo_t cbinfo;
	cbinfo.cbdata = (void *)this;
	ApModuleParent(ABM_rebelWidgetDialog, AB_PARENT, ABW_rebelBkgd);
	_window = ApCreateModule(ABM_rebelWidgetDialog, NULL, &cbinfo);
}

rebelWidget::~rebelWidget()
{
	PtDestroyWidget(_window);
}

int
rebelWidget::init( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	if(apinfo->reason == ABR_PRE_REALIZE) {
		rebelWidget *wdgt = (rebelWidget *)cbinfo->cbdata;

		char title[64];
		strcpy(title, "WIDGETS");

		PtArg_t args[2];
		int nArgs = 0;
		PtSetArg(&args[nArgs], Pt_ARG_POINTER, wdgt, 0); ++nArgs;
		PtSetArg(&args[nArgs], Pt_ARG_WINDOW_TITLE, title, 0); ++nArgs;
		PtSetResources(link_instance, nArgs, args);
	}
	else {
		rebelWidget *wdgt;
		PtGetResource(link_instance, Pt_ARG_POINTER, &wdgt, 0);

		PhRect_t extent;
		PtWidgetExtent(link_instance, &extent);
		wdgt->_origin = extent.ul;

		PhArea_t area;
		PhRectToArea(&extent, &area);
		wdgt->_size = area.size;
	}

	return( Pt_CONTINUE );

}


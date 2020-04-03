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

int initRasterDemo( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PhDim_t dim;
	PhRect_t frame;
	short int x;
	short int y;

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	if(apinfo->reason == ABR_PRE_REALIZE) { /* stuff to do before realizing widget */
		PgSetDrawBufferSize(16384);	/* allocate a 16K draw buffer */
	}
	else {	/* stuff to do after reaslizing widget */
		/* set the initial scaling factor for the tig widget */
		calcScale();

		/* save the location and size of the main window for rex session */
		PtWindowGetFrameSize(link_instance, &frame);
		PtGetAbsPosition(link_instance, &x, &y);
		myptp->p_x = x - frame.ul.x;
		myptp->p_y = y - frame.ul.y;

		PtWidgetDim(link_instance, &dim);
		myptp->p_w = dim.w;
		myptp->p_h = dim.h;

		/* read root file from rex session */
		if(strlen(myptp->p_root) > 0) {
			readRoot(myptp->p_root);
		}
	}

	return( Pt_CONTINUE );
}

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

int initSmi( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[4];
	int nArgs;

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

#ifdef SMI_TRACK
	nArgs = 4;
	PtSetArg(&args[0], Pt_ARG_TEXT_FLAGS, Pt_TRUE, Pt_EDITABLE);
	PtSetArg(&args[1], Pt_ARG_NUMERIC_FLAGS, Pt_TRUE, Pt_NUMERIC_ENABLE_UPDOWN);
	PtSetArg(&args[2], Pt_ARG_FLAGS, Pt_FALSE, Pt_GHOST);
	PtSetArg(&args[3], Pt_ARG_NUMERIC_VALUE, smi_offset, 0);
	PtSetResources(ABW_smiOffset, nArgs, args);

	PtSetArg(&args[3], Pt_ARG_NUMERIC_VALUE, smi_gain, 0);
	PtSetResources(ABW_smiGain, nArgs, args);

	PtSetArg(&args[3], Pt_ARG_NUMERIC_VALUE, smi_overrun, 0);
	PtSetResources(ABW_smiOverrun, nArgs, args);

	PtSetArg(&args[3], Pt_ARG_NUMERIC_VALUE, smi_debtog, 0);
	PtSetResources(ABW_smiDebtog, nArgs, args);

	PtSetArg(&args[3], Pt_ARG_NUMERIC_VALUE, smi_debtog2, 0);
	PtSetResources(ABW_smiDebtog2, nArgs, args);

	PtSetArg(&args[3], Pt_ARG_NUMERIC_VALUE, smi_deblval, 0);
	PtSetResources(ABW_smiDeblval, nArgs, args);

	PtSetArg(&args[3], Pt_ARG_NUMERIC_VALUE, smi_debhval, 0);
	PtSetResources(ABW_smiDebhval, nArgs, args);

	PtSetArg(&args[3], Pt_ARG_NUMERIC_VALUE, smi_counts[0], 0);
	PtSetResources(ABW_smiV0, nArgs, args);

	PtSetArg(&args[3], Pt_ARG_NUMERIC_VALUE, smi_counts[1], 0);
	PtSetResources(ABW_smiV1, nArgs, args);

	PtSetArg(&args[3], Pt_ARG_NUMERIC_VALUE, smi_counts[2], 0);
	PtSetResources(ABW_smiV2, nArgs, args);

	PtSetArg(&args[3], Pt_ARG_NUMERIC_VALUE, smi_counts[3], 0);
	PtSetResources(ABW_smiV3, nArgs, args);

	PtSetArg(&args[3], Pt_ARG_NUMERIC_VALUE, smi_counts[4], 0);
	PtSetResources(ABW_smiV4, nArgs, args);

	PtSetArg(&args[3], Pt_ARG_NUMERIC_VALUE, smi_counts[5], 0);
	PtSetResources(ABW_smiV5, nArgs, args);

	PtSetArg(&args[3], Pt_ARG_NUMERIC_VALUE, smi_counts[6], 0);
	PtSetResources(ABW_smiV6, nArgs, args);

	PtSetArg(&args[3], Pt_ARG_NUMERIC_VALUE, smi_counts[7], 0);
	PtSetResources(ABW_smiV7, nArgs, args);

	PtSetArg(&args[3], Pt_ARG_NUMERIC_VALUE, smi_counts[8], 0);
	PtSetResources(ABW_smiV8, nArgs, args);

	PtSetArg(&args[3], Pt_ARG_NUMERIC_VALUE, smi_counts[9], 0);
	PtSetResources(ABW_smiV9, nArgs, args);

	PtSetArg(&args[3], Pt_ARG_NUMERIC_VALUE, smi_late, 0);
	PtSetResources(ABW_smiLater, nArgs, args);
#else
	nArgs = 3;
	PtSetArg(&args[0], Pt_ARG_TEXT_FLAGS, Pt_FALSE, Pt_EDITABLE);
	PtSetArg(&args[1], Pt_ARG_NUMERIC_FLAGS, Pt_FALSE, Pt_NUMERIC_ENABLE_UPDOWN);
	PtSetArg(&args[2], Pt_ARG_FLAGS, Pt_TRUE, Pt_GHOST);
	PtSetResources(ABW_smiOffset, nArgs, args);
	PtSetResources(ABW_smiGain, nArgs, args);
	PtSetResources(ABW_smiOverrun, nArgs, args);
	PtSetResources(ABW_smiDebtog, nArgs, args);
	PtSetResources(ABW_smiDebtog2, nArgs, args);
	PtSetResources(ABW_smiDeblval, nArgs, args);
	PtSetResources(ABW_smiDebhval, nArgs, args);
	PtSetResources(ABW_smiV0, nArgs, args);
	PtSetResources(ABW_smiV1, nArgs, args);
	PtSetResources(ABW_smiV2, nArgs, args);
	PtSetResources(ABW_smiV3, nArgs, args);
	PtSetResources(ABW_smiV4, nArgs, args);
	PtSetResources(ABW_smiV5, nArgs, args);
	PtSetResources(ABW_smiV6, nArgs, args);
	PtSetResources(ABW_smiV7, nArgs, args);
	PtSetResources(ABW_smiV8, nArgs, args);
	PtSetResources(ABW_smiV9, nArgs, args);
	PtSetResources(ABW_smiLater, nArgs, args);
#endif

	
	return( Pt_CONTINUE );
}

int smiOffset( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo, arg = arg;

#ifdef SMI_TRACK
	PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, 0, 0);
	PtGetResources(widget, 1, &arg);
	smi_offset = (int)arg.value;
#endif
	return( Pt_CONTINUE );
}

int smiGain( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo, arg = arg;

#ifdef SMI_TRACK
	PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, 0, 0);
	PtGetResources(widget, 1, &arg);
	smi_gain = (int)arg.value;
#endif
	return( Pt_CONTINUE );
}

int smiOverrun( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo, arg = arg;

#ifdef SMI_TRACK
	PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, 0, 0);
	PtGetResources(widget, 1, &arg);
	smi_overrun = (int)arg.value;
#endif
	return( Pt_CONTINUE );
}

int smiDebtog( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo, arg = arg;

#ifdef SMI_TRACK
	PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, 0, 0);
	PtGetResources(widget, 1, &arg);
	smi_debtog = (int)arg.value;
#endif
	return( Pt_CONTINUE );
}

int smiDebtog2( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo, arg = arg;

#ifdef SMI_TRACK
	PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, 0, 0);
	PtGetResources(widget, 1, &arg);
	smi_debtog2 = (int)arg.value;
#endif
	return( Pt_CONTINUE );
}

int smiDeblval( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo, arg = arg;

#ifdef SMI_TRACK
	PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, 0, 0);
	PtGetResources(widget, 1, &arg);
	smi_deblval = (int)arg.value;
#endif
	return( Pt_CONTINUE );
}

int smiDebhval( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo, arg = arg;

#ifdef SMI_TRACK
	PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, 0, 0);
	PtGetResources(widget, 1, &arg);
	smi_debhval = (int)arg.value;
#endif
	return( Pt_CONTINUE );
}

int smiV0( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo, arg = arg;

#ifdef SMI_TRACK
	PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, 0, 0);
	PtGetResources(widget, 1, &arg);
	smi_counts[0] = (int)arg.value;
#endif
	return( Pt_CONTINUE );
}

int smiV1( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo, arg = arg;

#ifdef SMI_TRACK
	PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, 0, 0);
	PtGetResources(widget, 1, &arg);
	smi_counts[1] = (int)arg.value;
#endif
	return( Pt_CONTINUE );
}

int smiV2( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo, arg = arg;

#ifdef SMI_TRACK
	PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, 0, 0);
	PtGetResources(widget, 1, &arg);
	smi_counts[2] = (int)arg.value;
#endif
	return( Pt_CONTINUE );
}

int smiV3( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo, arg = arg;

#ifdef SMI_TRACK
	PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, 0, 0);
	PtGetResources(widget, 1, &arg);
	smi_counts[3] = (int)arg.value;
#endif
	return( Pt_CONTINUE );
}

int smiV4( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo, arg = arg;

#ifdef SMI_TRACK
	PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, 0, 0);
	PtGetResources(widget, 1, &arg);
	smi_counts[4] = (int)arg.value;
#endif
	return( Pt_CONTINUE );
}

int smiV5( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo, arg = arg;

#ifdef SMI_TRACK
	PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, 0, 0);
	PtGetResources(widget, 1, &arg);
	smi_counts[5] = (int)arg.value;
#endif
	return( Pt_CONTINUE );
}

int smiV6( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo, arg = arg;

#ifdef SMI_TRACK
	PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, 0, 0);
	PtGetResources(widget, 1, &arg);
	smi_counts[6] = (int)arg.value;
#endif
	return( Pt_CONTINUE );
}

int smiV7( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo, arg = arg;

#ifdef SMI_TRACK
	PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, 0, 0);
	PtGetResources(widget, 1, &arg);
	smi_counts[7] = (int)arg.value;
#endif
	return( Pt_CONTINUE );
}

int smiV8( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo, arg = arg;

#ifdef SMI_TRACK
	PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, 0, 0);
	PtGetResources(widget, 1, &arg);
	smi_counts[8] = (int)arg.value;
#endif
	return( Pt_CONTINUE );
}

int smiV9( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo, arg = arg;

#ifdef SMI_TRACK
	PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, 0, 0);
	PtGetResources(widget, 1, &arg);
	smi_counts[9] = (int)arg.value;
#endif
	return( Pt_CONTINUE );
}

int smiLater( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo, arg = arg;

#ifdef SMI_TRACK
	PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, 0, 0);
	PtGetResources(widget, 1, &arg);
	smi_late = (int)arg.value;
#endif
	return( Pt_CONTINUE );
}

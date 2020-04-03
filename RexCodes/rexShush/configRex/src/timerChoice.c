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

extern int ctboard;

int timerChoice( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	
	if(ApName(widget) == ABN_isaCtToggle) ctboard = ISA_CT;
	else if(ApName(widget) == ABN_pciCtToggle) ctboard = PCI_CT;

	return( Pt_CONTINUE );
}


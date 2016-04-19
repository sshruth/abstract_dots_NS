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
#include "ph_rline.h"
#include "abimport.h"
#include "proto.h"

int rlineTimer( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t	arg;
	static int lastPshDwnCntr = -1;
	int stackSize;

	extern RT_DISP rtDisp[RT_DISP_LEN];

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* if the rex clock is not running, just return */
	if(!(i_b->i_flags & I_GO)) return( Pt_CONTINUE );

	/* make a local copy of the real-time display buffer */
	memcpy((RT_DISP *)rtDisp, (const RT_DISP *)i_b->rtDisp, (RT_DISP_LEN * sizeof(RT_DISP)));
	stackSize = i_b->rtDspPshDwnCntr - lastPshDwnCntr;
	lastPshDwnCntr = i_b->rtDspPshDwnCntr;

	/* if there is no change in the display stack, just return */
	if(!stackSize) return( Pt_CONTINUE );
	else if(stackSize > RT_DISP_LEN) stackSize = RT_DISP_LEN;

	scaleData(stackSize);

	updateLabels();

	PtDamageWidget(ABW_runLineWindow);		// Damage the widget to force a redraw
	return( Pt_CONTINUE );
}

void updateLabels()
{
	PtArg_t arg;
	RL *rlp;
	RL_CHAN *rcp;
	static int strlens[RL_MAXCHAN] = { 0 };	/* lengths of the signal titles */
	int len;
	int i;
	char label[MAX_LABEL_LEN];

	/* set the labels on the channel toggles */
	len = 0;
	rlp = (RL *)&i_b->rl;
 	for(i = 0; i < RL_MAXCHAN; i++) {
		rcp = &rlp->rl_chan[i];				/* pointer to this channel */
		if(rcp->rl_signal < 0) {
			len = strlen("-");
			if(len == strlens[i]) continue;

			strcpy(label, "-");
			strcat(label, "\0");
		}
	 	else if(r_b->r_samp.sa_title[rcp->rl_signal][0]) {	/* if this signal has a title */
			len = strlen((char *)r_b->r_samp.sa_title[rcp->rl_signal]);	/* length of the title */
			if(len == strlens[i]) continue;

			strncpy(label, (char *)&r_b->r_samp.sa_title[rcp->rl_signal], MAX_LABEL_LEN-1);
			strcat(label, "\0");
		}
		strlens[i] = len;				/* save the new length */

		PtSetArg(&arg, Pt_ARG_TEXT_STRING, label, 0);
		switch(i) {
		case 0:
			PtSetResources(ABW_chan0Toggle, 1, &arg);
			break;
		case 1:
			PtSetResources(ABW_chan1Toggle, 1, &arg);
			break;
		case 2:
			PtSetResources(ABW_chan2Toggle, 1, &arg);
			break;
		case 3:
			PtSetResources(ABW_chan3Toggle, 1, &arg);
			break;
		case 4:
			PtSetResources(ABW_chan4Toggle, 1, &arg);
			break;
		case 5:
			PtSetResources(ABW_chan5Toggle, 1, &arg);
			break;
		case 6:
			PtSetResources(ABW_chan6Toggle, 1, &arg);
			break;
		case 7:
			PtSetResources(ABW_chan7Toggle, 1, &arg);
			break;
		}
	}
}

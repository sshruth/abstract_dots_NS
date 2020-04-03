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

int onCntrlB( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	int response;
	char *btns[] = { "&Yes", "&No" };
	char *btnFonts[] = { "&Helvetica09ba", "&Helvetica09ba"	};


	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	switch(PtAlert(PtWidgetParent(widget), NULL, "Quit Rex", NULL,
				   "Do you want to exit REX?", "Helvetica09ba",
				   2, (char const **)btns, (char const **)btnFonts, 1, 2, Pt_MODAL)) {
	case 1:
		endproc(NP);		/* kill all process */
		PtExit(EXIT_SUCCESS);
		break;
	case 2:
		resetRex();
		break;
	}

	return( Pt_CONTINUE );
}

void resetRex()
{
	PROCTBL_P cp;
	PROCTBL_P sp;
	PROCTBL_P p;
	int old_value;
	char *old_name;

	extern int sysflags;

	cp = (PROCTBL_P)&i_b->ptbl[0];				/* procSwitch's ptbl entry */
	sp = (PROCTBL_P)&i_b->ptbl[i_b->scrb_pi];	/* scribe's ptbl entry */

	/*	signal(S_CNTRLB, onCntrlB); */
	cp->p_state |= P_NOSIG_ST;	/* stop IPC */
	cp->p_msg = cp->p_vindex = cp->p_rmask = 0;
	cp->p_sem = HIGHBIT;
	sysflags = 0;
	i_b->c_flags = 0;
	old_name = doutput_filename;
	doutput_filename = "/tmp/doutput_comm";
	old_value = doutput_tofile;
	doutput_tofile = 1;
	doutput(0);
	doutput_tofile = old_value;
	doutput_filename = old_name;
	cp->p_state &= ~P_NOSIG_ST;

	kill(sp->p_id, S_CNTRLB);	/* send control-B signal to scribe */

	/* get pointer to the current int process */
	p = (PROCTBL_P)&i_b->ptbl[i_b->ph_int_pi];

	endproc(p);	/*kill current int process */

	return;
}


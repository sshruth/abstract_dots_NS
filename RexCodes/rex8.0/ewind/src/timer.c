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
#include "ph_ewind.h"
#include "abimport.h"
#include "proto.h"

#define TEN_TO_IC    2      /* converts tenths of a degree to internal calibration */
extern CURS_DES cursDescrip[WDI_NCURS];

int windTimer( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
	static int lastPshDwnCntr = -1;
	int stackSize;

	extern RT_DISP rtDisp[RT_DISP_LEN];

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* if the rex clock is not running, just return */
	if(!(i_b->i_flags & I_GO)) return( Pt_CONTINUE);

	/* make a local copy of the real-time display buffer */
	memcpy((RT_DISP *)rtDisp, (const RT_DISP *)i_b->rtDisp, (RT_DISP_LEN * sizeof(RT_DISP)));
	stackSize = i_b->rtDspPshDwnCntr - lastPshDwnCntr;
	lastPshDwnCntr = i_b->rtDspPshDwnCntr;

	/* if there is no change in the display stack, just return */
	if(!stackSize) return( Pt_CONTINUE );
	else if(stackSize > RT_DISP_LEN) stackSize = RT_DISP_LEN;

	/* update the window sizes before sending data size
	 * so the current size will be displayed
	 */
	updateWinSizes();

	calcCursPos(stackSize);
	
	updateEyeOffsets();
	updateLabels();

	PtDamageWidget(ABW_displayWindow);

	return( Pt_CONTINUE);
}

void updateWinSizes()
{
	WND_SIZ wndSiz;
	int i;

	/* check to see if the window sizes have changed */
	for(i = 0; i < WD_MAXNUM; i++) {
		/* don't set window sizes of windows that aren't on */
		if(!i_b->wd[i].wd_cntrl) continue;

		wndSiz.index = i;
		wndSiz.dim.w = i_b->wd[i].wd_xsiz;
		wndSiz.dim.h = i_b->wd[i].wd_ysiz;
		calcWndSiz(wndSiz.index, wndSiz.dim);
	}
}

void updateEyeOffsets()
{
	PtArg_t arg;
	static int eyeOffsets[4] = { -1 };

	/* check to see if the eye offsets have changed */
	if(i_b->eyeHoff != eyeOffsets[0]) {
		PtSetResource(ABW_eyeHorizOffset, Pt_ARG_NUMERIC_VALUE, (i_b->eyeHoff >> TEN_TO_IC), 0);
		eyeOffsets[0] = i_b->eyeHoff;
/*
		PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, (i_b->eyeHoff >> TEN_TO_IC), 0);
		PtSetResources(ABW_eyeHorizOffset, 1, &arg);
*/
	}
	if(i_b->eyeVoff != eyeOffsets[1]) {
		PtSetResource(ABW_eyeVertOffset, Pt_ARG_NUMERIC_VALUE, (i_b->eyeVoff >> TEN_TO_IC), 0);
/*
		eyeOffsets[1] = i_b->eyeVoff;
		PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, (i_b->eyeVoff >> TEN_TO_IC), 0);
*/
	}
	if(i_b->oEyeHoff != eyeOffsets[2]) {
		PtSetResource(ABW_oEyeHorizOffset, Pt_ARG_NUMERIC_VALUE, (i_b->oEyeHoff >> TEN_TO_IC), 0);
/*
		eyeOffsets[2] = i_b->oEyeHoff;
		PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, (i_b->oEyeHoff >> TEN_TO_IC), 0);
*/
	}
	if(i_b->oEyeVoff != eyeOffsets[3]) {
		PtSetResource(ABW_oEyeVertOffset, Pt_ARG_NUMERIC_VALUE, (i_b->oEyeVoff >> TEN_TO_IC), 0);
/*
		eyeOffsets[3] = i_b->oEyeVoff;
		PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, (i_b->oEyeVoff >> TEN_TO_IC), 0);
*/
	}
}

void updateLabels()
{
	PtArg_t arg;
	int type;
	int i;
	char label[MAX_LABEL_LEN];

	for(i = 0; i < WDI_NDATACURS; i++) {
		switch(i_b->cursTyp.datCurs[i].type) {
		case CHAN_CENTER:
			type = CU_PLUS;
			strcpy(label, "center");
			PtSetArg(&arg, Pt_ARG_TEXT_STRING, label, 0);
			break;
		case CHAN_EYE:
			type = CU_BOX;
			strcpy(label, "eye");
			PtSetArg(&arg, Pt_ARG_TEXT_STRING, label, 0);
			break;
		case CHAN_OEYE:
			type = CU_DIAMOND;
			strcpy(label, "o-eye");
			PtSetArg(&arg, Pt_ARG_TEXT_STRING, label, 0);
			break;
		case CHAN_OTHER:
			type = CU_UTRIANGLE;
			strcpy(label, "other");
			PtSetArg(&arg, Pt_ARG_TEXT_STRING, label, 0);
			break;
		case CHAN_ADD:
			type = CU_UTRIANGLE;
			strcpy(label, "additional");
			PtSetArg(&arg, Pt_ARG_TEXT_STRING, label, 0);
			break;
		case CHAN_JOY:
			type = CU_JOY;
			strcpy(label, "joy-stick");
			PtSetArg(&arg, Pt_ARG_TEXT_STRING, label, 0);
			break;
		case CHAN_DA0:
			type = CU_STAR;
			strcpy(label, "DA-0");
			PtSetArg(&arg, Pt_ARG_TEXT_STRING, label, 0);
			break;
		case CHAN_DA1:
			type = CU_STAR;
			strcpy(label, "DA-1");
			PtSetArg(&arg, Pt_ARG_TEXT_STRING, label, 0);
			break;
		case CHAN_RAMP0:
			type = CU_X;
			strcpy(label, "ramp-0");
			PtSetArg(&arg, Pt_ARG_TEXT_STRING, label, 0);
			break;
		case CHAN_RAMP1:
			type = CU_X;
			strcpy(label, "ramp-1");
			PtSetArg(&arg, Pt_ARG_TEXT_STRING, label, 0);
			break;
		case CHAN_VERG:
			type = CU_LTRIANGLE;
			strcpy(label, "vergence");
			PtSetArg(&arg, Pt_ARG_TEXT_STRING, label, 0);
			break;
		case CHAN_CYCLOP:
			type = CU_LTRIANGLE;
			strcpy(label, "cyclopian");
			PtSetArg(&arg, Pt_ARG_TEXT_STRING, label, 0);
			break;
		case CHAN_GAZE:
			type = CU_RTRIANGLE;
			strcpy(label, "eye-in-head");
			PtSetArg(&arg, Pt_ARG_TEXT_STRING, label, 0);
			break;
		case CHAN_OGAZE:
			type = CU_RTRIANGLE;
			strcpy(label, "o-eye-in-head");
			PtSetArg(&arg, Pt_ARG_TEXT_STRING, label, 0);
			break;
		case CHAN_VGAZE:
			type = CU_DTRIANGLE;
			strcpy(label, "verg-e-in-h");
			PtSetArg(&arg, Pt_ARG_TEXT_STRING, label, 0);
			break;
		case CHAN_CGAZE:
			type = CU_DTRIANGLE;
			strcpy(label, "cyclop-e-in-h");
			PtSetArg(&arg, Pt_ARG_TEXT_STRING, label, 0);
			break;
		default:
			type = CU_NONE;
			strcpy(label, "-");
			PtSetArg(&arg, Pt_ARG_TEXT_STRING, label, 0);
			break;
		}
		cursDescrip[i].type = type;
		switch(i) {
		case 0:
			PtSetResources(ABW_curs0Btn, 1, &arg);
			break;
		case 1:
			PtSetResources(ABW_curs1Btn, 1, &arg);
			break;
		case 2:
			PtSetResources(ABW_curs2Btn, 1, &arg);
			break;
		case 3:
			PtSetResources(ABW_curs3Btn, 1, &arg);
			break;
		case 4:
			PtSetResources(ABW_curs4Btn, 1, &arg);
			break;
		case 5:
			PtSetResources(ABW_curs5Btn, 1, &arg);
			break;
		case 6:
			PtSetResources(ABW_curs6Btn, 1, &arg);
			break;
		case 7:
			PtSetResources(ABW_curs7Btn, 1, &arg);
			break;
		}
	}
}

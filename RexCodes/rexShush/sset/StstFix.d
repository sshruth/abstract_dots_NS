#include "pcsSocket.h"
#include "GLcommand.h"
#include "lcode_tst.h"
#include "ldev_tst.h"
#include "position.h"
#define WIND0 0
#define WIND1 1

/*
 * Subroutine that determines the position of the
 * fixation target
 */
int trialCode;
int fixOn = OBJ_ON;
int fixOff = OBJ_OFF;
float fpx;
float fpy;
int errlst[NUM_POS * 2] = { 0 };
int trlcntr = -1;
int curstm = 0;
int range = 1.0;		/* scale factor to increase range of fixation point positions */
int stmerr = 0;
int fixerr = 0;
int cyc_total = 0;	/* total number of stimulus cycles */

int pick_fp_location()
{
	static int ptrlst[NUM_POS * 2] = { 0 };
	static int rs_shift = 10;
	int i, j;
	
	if(!range) range = 1;
	if(--trlcntr <= NUM_POS) {
		if (stmerr > 10 ) {	/* present errors if more than 10 */
			trlcntr = stmerr;
			for (j = 0; j < trlcntr; j++) ptrlst[j] = errlst[j];
			fixerr++;
		}

		/* If there were 10 or less errors, shuffle the error */
		/* conditions into the next block of trials. */
		else {
			trlcntr = NUM_POS + stmerr;
			for (i = 0; i < NUM_POS; i++ ) ptrlst[i] = i;
			for (j = 0; j < stmerr; j++) ptrlst[i + j] = errlst[j];
			fixerr = 0;
		}

		/* reset the error counter after moving error conditions */
		/* into the trial block */
		stmerr = 0;

		/* randomize the stimulus conditions in the trial block */
		shuffle(trlcntr, rs_shift, ptrlst);

		/* if the fixerr flag is 0, increment the counter that */
		/* keeps track of the number of blocks of trials completed */
		if(!fixerr) {
			cyc_total++;
		}
	}

	curstm = ptrlst[trlcntr-1];
	fpx = stm_list[curstm].x;
	fpy = stm_list[curstm].y;
	trialCode = 2000 + curstm;
	
	return(0);
}

/*
 * Subroutine that controls window size and counts total trials
 */
long wndsiz = 20;
int trials = 0;
int otheyeflag = 0;

int set_wnd()
{
	static long wndxctr = 0;
	static long wndyctr = 0;

	trials++;
	wndxctr = (long) stm_list[curstm].x;
	wndyctr = (long) stm_list[curstm].y;
	if(!otheyeflag) {
		wd_pos(WIND0, wndxctr, wndyctr);
		wd_disp(D_W_EYE_X);
		wd_siz(WIND0, wndsiz, wndsiz);
		wd_cntrl(WIND0, WD_ON);
	}
	else {
		wd_pos(WIND1, wndxctr, wndyctr);
		wd_disp(D_W_EYE_O);
		wd_siz(WIND1, wndsiz, wndsiz);
		wd_cntrl(WIND1, WD_ON);
	}
	return (0);
}

int beye = 0;
int mistake()
{
	errlst[stmerr++] = curstm;
	beye++;
	return(0);
}

void rinitf(void)
{
	char *vexHost = "lsr-jwmvex";
	char *mexHost = "lsr-jwmmex2";

	pcsConnectVex(vexHost);
/* 	pcsConnectMex(mexHost); */
	
	wd_src_pos(WIND0, WD_DIRPOS, 0, WD_DIRPOS, 0);
	wd_src_pos(WIND1, WD_DIRPOS, 0, WD_DIRPOS, 0);
	wd_src_check(WIND0, WD_SIGNAL, 0, WD_SIGNAL, 1);
	wd_src_check(WIND1, WD_SIGNAL, 2, WD_SIGNAL, 3);
	wd_cntrl(WIND0, WD_ON);
	wd_cntrl(WIND1, WD_ON);
}

/*
 * statlist menu
 */
 
VLIST state_vl[] = {
	"win siz",	&wndsiz,	NP,	NP,	0,	ME_DEC,
	"trials",	&trials,	NP,	NP,	0,	ME_DEC,
	"other_eye?",	&otheyeflag,	NP,	NP,	0,	ME_DEC,
	"range",	&range,		NP,	NP,	0,	ME_DEC,
	"total_cycles",	&cyc_total,	NP,	NP,	0,	ME_DEC,
	NS,
};

char hm_sv_vl[]= "";

/*
 * User-supplied real-time variables
 */
RTVAR rtvars[] = {
"", 0,
};

%%
id 100
restart rinitf
main_set {
status ON
begin	first:
		to disabl
	disabl:
		to enable on -PSTOP & softswitch  /*is bar enabled?*/
	enable:
		code ENABLECD
		to pckfix
	pckfix:
		code &trialCode 
		do pick_fp_location()
		to setfp
	setfp:
		do PvexSetFixLocation(&fpx, &fpy)
		to awnopn on 0 % tst_rx_new	/* wait for vex response */
	awnopn:
		do awind(OPEN_W)
		to fpncmd
	fpncmd:
		code 3232
		do PvexSwitchFix(&fixOn)
		to fpon on 0 % tst_rx_new	/* wait for vex response */
	fpon:
		code FPONCD
		to winset
	winset:
		do set_wnd()
		to eyein
	eyein:
		time 1000
		rand 500
		to fpdcmd
	fpdcmd:
		do PvexDimFix()
		to fpdim on 0 % tst_rx_new
	fpdim:
		code FPDIMCD
		to eyeout
	eyeout:
		time 100
		to closew
	closew:
		do awind(CLOSE_W)
		to correct
	correct:
		do score(YES)
		to fpfcmd
	fpfcmd:
		do PvexSwitchFix(&fixOff)
		to fpoff on 0 % tst_rx_new	/* wait for vex response */
	fpoff:
		code FPOFFCD
		time 1000
		to first
}

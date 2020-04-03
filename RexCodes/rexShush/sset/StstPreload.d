#include "pcsSocket.h"
#include "GLcommand.h"
#include "lcode_tst.h"
#include "ldev_tst.h"
#include "fixPar.h"
#define WIND0 0
#define WIND1 1

/*
 * Subroutine that determines the position of the
 * fixation target
 */
int trialCode;
int fixOn = OBJ_ON;
int fixOff = OBJ_OFF;
int fpLum = 128;
int fpDim = 128;
int bkLum = 128;
int fpSiz = 4;

int errlst[NUM_POS * 2] = { 0 };
int trlcntr = -1;
int curstm = 0;
int stmerr = 0;
int fixerr = 0;
int cyc_total = 0;	/* total number of stimulus cycles */

int numObj = 1;
int objNum = 1;
int tiffImage = 103;
int objOn = OBJ_ON;
int objOff = OBJ_OFF;

int pick_fp_parameters()
{
	static int ptrlst[NUM_POS * 2] = { 0 };
	static int rs_shift = 10;
	int i, j;
	
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
	fpLum = stm_list[curstm].lum;
	fpDim = stm_list[curstm].dim;
	fpSiz = stm_list[curstm].size;
	trialCode = 2000 + curstm;
	
	return(0);
}

/*
 * Subroutine that controls window size and counts total trials
 */
long wndsiz = 20;
int trials = 0;

int set_wnd()
{
	static long wndxctr = 0;
	static long wndyctr = 0;

	trials++;
	wndxctr = 0;
	wndyctr = 0;

	wd_pos(WIND0, wndxctr, wndyctr);
	wd_disp(D_W_EYE_X);
	wd_siz(WIND0, wndsiz, wndsiz);
	wd_cntrl(WIND0, WD_ON);
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
	char *vexHost = "lsr-xpsbb";
	char *mexHost = "lsr-labmex";		/* if you are going to use Mex */

	pcsConnectVex(vexHost);
	pcsConnectMex(mexHost);		/* if you are going to use Mex */
	
	wd_src_pos(WIND0, WD_DIRPOS, 0, WD_DIRPOS, 0);
	wd_src_pos(WIND1, WD_DIRPOS, 0, WD_DIRPOS, 0);
	wd_src_check(WIND0, WD_SIGNAL, 0, WD_SIGNAL, 1);
	wd_src_check(WIND1, WD_SIGNAL, 2, WD_SIGNAL, 3);
	wd_cntrl(WIND0, WD_ON);
}

/*
 * statlist menu
 */
 
VLIST state_vl[] = {
	"win_siz",	   			&wndsiz,	NP,	NP,	0,	ME_DEC,
	"background",  			&bkLum,		NP, NP, 0, ME_DEC,
	"trials",	   		   	&trials,	NP,	NP,	0,	ME_DEC,
	"total_cycles",			&cyc_total,	NP,	NP,	0,	ME_DEC,
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
		do pick_fp_parameters()
		to fpsiz
	fpsiz:
		do PvexSetFixSiz(&fpSiz)
		to fplum on 0 % tst_rx_new
	fplum:
		do PvexSetFixColors(&fpLum, &fpLum, &fpLum, &fpDim, &fpDim, &fpDim)
		to drwStm on 0 % tst_rx_new
	drwStm:
		do PvexDrawTiffImage(&numObj, &objNum, &tiffImage)
		to awnopn on 0 % tst_rx_new	/* wait for vex response */
	awnopn:
		do awind(OPEN_W)
		to fpncmd
	fpncmd:
		do PvexSwitchFix(&fixOn);
		to fpon on 0 % tst_rx_new
	fpon:
		code FPONCD
		to winset
	winset:
		do set_wnd()
		to eyein
	eyein:
		time 100
		rand 500
		to ldaStm
	ldaStm:
		do PvexPreloadStim(&numObj, &objNum, &objOn)
		to paus1 on 0 % tst_rx_new
	paus1:
		time 500
		to swpBuf
	swpBuf:
		do PvexSwapBuffers()
		to paus2 on 0 % tst_rx_new
	paus2:
		time 1000
		to stfcmd
	stfcmd:
		PvexSwitchStim(&numObj, &objNum, &objOff)
		to paus3 on 0 % tst_rx_new
	paus3:
		time 500
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
		do PvexSwitchFix(&fixOff);
		to fpoff on 0 % tst_rx_new	/* wait for vex response */
	fpoff:
		code FPOFFCD
		time 1000
		to first
}

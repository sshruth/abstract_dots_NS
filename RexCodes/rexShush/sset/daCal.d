#include <sac.h>
#include "ldev_tst.h"
#include "lcode_tst.h"
#include "eyeCal.h"

#define WIND0 0

int wndsiz = 50;
int range = 2;
int density = 1;
int lateral = 1;
int twoEyes = 0;
int blockTot = 5;
int numTarg = 40;
int nTrials = -1;
int nBlocks = -1;
calLoc_t *thsGrid = bi16grid;
int dax = 0;
int day = 1;
int fpx = 0;
int fpy = 0;
int posx_code = 0;
int posy_code = 0;
int trlcntr = -1;
int curstm = 0;		/* index into the table of experimental conditions for this trial */
int stmerr = 0;		/* number of errors in this block */
int fixerr = 0;
int errlst[MAX_POS * 2] = { 0 };
int ptrlst[MAX_POS * 2] = { 0 };	/* list of indices into table of experimental conditions */
calPos_t posArray[400];
int numPos;

int
checkBlock()
{
 	if(nBlocks >= blockTot) return(1);

 	return(0);
}

int sayDone()
{
	dprintf("\007");
	rxerr("Calibration completed");
	i_b->i_flags |= I_PSTOP;
	softswitch |= PSTOP;

	return(0);
}

/*
 * Subroutine that determines the position of the
 * fixation target
 */
int
pick_targ_location()
{
	static int rs_shift = 10;
	int ecode = 0;
	int i;
	int j;
	
	if(--trlcntr <= 0) {
		/* if there were more than 10 errors in a block,
		 * present the error trials by themselves
		 */
		if(stmerr > 10 ) {
			trlcntr = stmerr;
			for (j = 0; j < trlcntr; j++) ptrlst[j] = errlst[j];
			fixerr++;
		}

		/* If there were 10 or less errors, shuffle the error
		 * add the error trials into the next block of trials.
		 */
		else {
			trlcntr = numTarg + stmerr;

			/* initial the ptrlst array to hold a sequence of numbers from 0 to numTarg - 1 */
			for (i = 0; i < numTarg; i++ ) ptrlst[i] = i;

			/* add the indices of the conditions with errors to the ptrlist array */
			for (j = 0; j < stmerr; j++) ptrlst[i + j] = errlst[j];
			fixerr = 0;
		}

		/* at this point the ptrlst array holds a list of indices into the
		 * table that holds the description of the experimental conditions
		 */

		/* reset the error counter after moving error conditions */
		/* into the trial block */
		stmerr = 0;

		/* randomize the ptrlst array so that experimental conditions will be
		 * presented in a random order
		 */
		shuffle(trlcntr, rs_shift, ptrlst);

		/* make sure that the same location doesn't occur
		 * on consequetive trials
		 */
		for(i = 0; i < trlcntr - 2; ++i) {
			if((thsGrid[ptrlst[i]].x == thsGrid[ptrlst[i + 1]].x) &&
			   (thsGrid[ptrlst[i]].y == thsGrid[ptrlst[i + 1]].y)) {
					j = ptrlst[i + 1];
					ptrlst[i + 1] = ptrlst[i + 2];
					ptrlst[i + 2] = j;
			}
		}

		/* if the fixerr flag is 0, increment the counter that */
		/* keeps track of the number of blocks of trials completed */
		if(!fixerr) {
			nBlocks++;
		}
	}

	/* pick an index from the list of indices, working from back to front */
	curstm = ptrlst[trlcntr-1];
	nTrials++;
	
	/* get the location of the fixation point for this trial */
	fpx = thsGrid[curstm].x * range;
	fpy = thsGrid[curstm].y * range;

	/* compute the ecode */
	ecode = 2000 + (range * 1000) + thsGrid[curstm].c;
	return(ecode);
}

/*
 * Subroutine that controls window size and counts total trials
 */
long wndxsiz = 0;
long wndysiz = 0;

int
set_wnd()
{
	static long wndxctr = 0;
	static long wndyctr = 0;
	static long z;

	wndxctr = (long) fpx;
	wndyctr = (long) fpy;

	/* increase the window size for more eccentric saccades */
	z = fpx / 100;
	wndxsiz = wndsiz * (1 + (z * z));
	z = fpy / 100;
	wndysiz = wndsiz * (1 + (z * z));

	wd_pos(WIND0, wndxctr, wndyctr);
	wd_siz(WIND0, wndsiz, wndsiz);
	return (0);
}

int
saveEye()
{
	posArray[numPos].fpx = fpx;
	posArray[numPos].fpy = fpy;
	posArray[numPos].ex = eyeh;
	posArray[numPos].ey = eyev;
	if(twoEyes) {
		posArray[numPos].oex = oeyeh;
		posArray[numPos].oey = oeyev;
	}
	else {
		posArray[numPos].oex = 0;
		posArray[numPos].oey = 0;
	}
	numPos++;
	if(numPos > 400) {
		nBlocks = blockTot;
	}
 	return(0);
}

int
mistake()
{
	if(stmerr < MAX_POS) errlst[stmerr++] = curstm;
	return(0);
}

void
rinitf(void)
{
	wd_pos(WIND0, 0, 0);
	wd_siz(WIND0, wndxsiz, wndysiz);
	wd_src_pos(WIND0, WD_DIRPOS, 0, WD_DIRPOS, 0);
	wd_src_check(WIND0, WD_SIGNAL, 0, WD_SIGNAL, 1);
	wd_cntrl(WIND0, WD_ON);
}

int
setLateral(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd)
{
	switch(lateral) {
	case 1:
		thsGrid = bi16grid;
		numTarg = 40;
		break;
	case 2:
		thsGrid = lf16grid;
		numTarg = 20;
		break;
	case 3:
		thsGrid = rt16grid;
		numTarg = 20;
		break;
	}
	trlcntr = -1;
	stmerr = 0;
	nTrials = -1;
	nBlocks = -1;
	return(0);
}

int
resetBlocks(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd)
{
	trlcntr = -1;
	stmerr = 0;
	nTrials = -1;
	nBlocks = -1;
 	return(0);
}

/*
 * statelist menu
 */
VLIST state_vl[] = {
	"win_siz",		&wndsiz,	NP,	NP,	0,	ME_DEC,
	"range",		&range,		NP,	resetBlocks, ME_AFT, ME_DEC,
	"laterality",	&lateral,	NP, setLateral,  ME_AFT,  ME_DEC,
	"use_2_eyes",	&twoEyes,	NP, resetBlocks, ME_AFT,  ME_DEC,
	"num_blocks",	&blockTot,	NP, resetBlocks, ME_AFT, ME_DEC,
	NS,
};
char hm_sv_vl[]= "
win_siz: target window size in rex units [50]
range: 1 = 5 deg, 2 = 10 deg 3 = 15 deg 4 = 20 deg [2]
laterality: 1 is bilateral, 2 is left only, 3 is right only [1]
use_2_eyes: 0 is calibrate eye only, 1 is calibrate eye and o-eye
num_blocks: number of times each set of targets is presented [5]
";

RTVAR rtvars[] = {
	{"number of trials",		&nTrials},
	{"blocks completed",		&nBlocks},
	{"block trials to go",		&trlcntr},
	{"block errors",			&stmerr},
	{"", 0},
};

/* tests for eye position have been commented out
 * so I can test the code with out using a subject
 * To use this code with subjects, uncomment the lines
 * the test eyeflag and comment out or remove the
 * "to eye1in" and "to oeye1n" lines
 */
%%
id 100
restart rinitf
main_set {
status ON
begin	first:
		to chkblk
    chkblk:
		to done on 1 % checkBlock
		to disabl on 0 % checkBlock
	done:
		do sayDone()
		to disabl
	disabl:
		to enable on -PSTOP & softswitch
	enable:
		code ENABLECD
		do pre_post(0, 0)
		to pckfix
	pckfix:
		do pick_targ_location()
		to winset
	winset:
		do set_wnd()
		to sdOn
	sdOn:
		do sd_set(1)
		to setfpPos
	setfpPos:
		do Pda_set_2(&dax, &fpx, &day, &fpy)
		to awnopn
	awnopn:
		do awind(OPEN_W)
		to fpon
	fpon:
		code FPONCD
		rl 10
		do dio_on(LED1)
		to chkeye
/*
 		time 1000						
 		to chkeye on +SF_ONSET & sacflags 
 		to error
*/
	chkeye:
		time 100
		rl 20
		to eyein
/*
  		to eyein on +SF_GOOD & sacflags
*/
	eyein:
		do sd_set(0)
		rl 30
		time 200
		to blinkOn
	blinkOn:
		do dio_off(LED1)
		time 200
		to error on +WD0_XY & eyeflag
		to blinkOff
	blinkOff:
		do dio_on(LED1)
		time 200
		to error on +WD0_XY & eyeflag
		to sveye
	sveye:	
		do saveEye()
		to closew
	closew:
		do awind(CLOSE_W)
		to rewon
	rewon:
		code REWCD
		dio_on(REW)
		rl 15
		time 20
		to rewoff
	rewoff:
		dio_off(REW)
		rl 5
		time 200
		to correct
	correct:
		do score(YES)
		to fpoff
	fpoff:
		code FPOFFCD			/* drop the fixoff code */
		do dio_off(LED1)
		rl 0
		time 100
		to first
	error:
		code ERR1CD
		do awind(CANCEL_W)
		to cancel
	cancel:
		do dio_off(LED1)
		rl 0
		to coderr
	coderr:
		do mistake()
		to wrong
	wrong:
		do score(NO)
		time 5000
		to first
}

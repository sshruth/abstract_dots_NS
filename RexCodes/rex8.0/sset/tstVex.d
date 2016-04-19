#include <stdlib.h>
#include <time.h>
#include "pcsSocket.h"
#include "GLcommand.h"
#include "ramp.h"
#include "lcode_tst.h"
#include "ldev_tst.h"
#include "vexStim.h"
#define WIND0 0
#define WIND1 1
#define GO 1
#define DONE 0
#define PHOTO_RESET_BIT Dio_id(PCDIO_DIO, 2, 0x04)
#define PHOTO_CELL_INPUT 0x1000
#define PHOTO_TRIG 1

/*
 * Subroutine that determines the position of the
 * fixation target
 */
int photoTrigger = 0;
float fpx;
float fpy;
long wind0Num = WIND0;
long wind1Num = WIND1;
long wndXsiz = 20;
long wndYsiz = 20;
long wndXpos = 0;
long wndYpos = 0;
long trgXsiz = 50;
long trgYsiz = 50;
long trgXpos = 0;
long trgYpos = 0;
long wdOn = WD_ON;
int numObj = 1;
int objects = 1;
int stmXpos = 50;
int stmYpos = 50;
float fstmx;
float fstmy;
float foknx = 0.0;
float fokny = 0.0;
int fred;
int fgreen;
int fblue;
int bred;
int bgreen;
int bblue;
int objOn = OBJ_ON;
int objOff = OBJ_OFF;
int goFlag = 0;
int stimType = 0;
int walsh = 0;
int haar = 0;
int chkSiz = 4;
int cntrst = 1;
int rndCols = 50;
int rndRows = 50;
int rndWhite = 50;
int annInner = 5;
int annOuter = 20;
int barLength = 50;
int barWidth = 5;
int barAngle = 0;
int flwWdth;
int flwHght;
int flwNr;
int flwFr;
int flwSpn;
int flwCv;
int flwSpn;
int flwChr;
int flowWidth = 200;
int flowHeight = 200;
int flowNear = 500;
int flowFar = 1500;
int flowRadius = 0;
int flowCov = 25;
int xyAng = 0;
int zAng = 0;
int vel = 10;
int roll = 0;
int pitch = 0;
int yaw = 0;
int span = 0;
int coher = 100;
int userPat = 1000;
int tiffPat = 101;
int pngPat = 101;
int sndWav = 101;
int rampTiff = 101;
int oknDir = 180;
int oknSpeed = 10;
int oknWidth = 300;
int oknHeight = 100;
int oknBar = 10;
int numRamps = 1;
int ramps = 1;
int flwRamps = 2;
int rampAct = ON_AFTER_RAMP;
int rampCycles = RA_ONCE;
int rampSteps;
float rampXs[120];
float rampYs[120];
int numTrigs = 1;
int trigs;
int rmpAnnInner = 0;
int rmpAnnOuter = 5;
int rampLen = 200;
int rampDir = 0;
int rampVel = 20;
int rampType;
int rampX = 0;
int rampY = 0;
int flowObject = 2;
int numFlwRmpObj = 2;
int flwRmpObjects[2] = { 1, 2 };
int flwRmpObjOn[2] = { OBJ_ON, OBJ_ON };
int flwRmpObjOff[2] = { OBJ_OFF, OBJ_OFF };
int ptrlst[NUM_STIM_TYPE * 2] = { 0 };
int errlst[NUM_STIM_TYPE * 2] = { 0 };
int trlcntr = -1;
int curstm = 0;
int stmerr = 0;
int fixerr = 0;
int cyc_total = 0;	/* total number of stimulus cycles */
int correctTrials = 0;
int errorTrials = 0;
int totalTrials = 0;
int percentCorrect = 0;
int errorFlag = 0;

int orntn = 0;
int spatFreq = 10;
int tempFreq = 10;
int cntrPhase = 0;
int redCntrst = 50;
int greenCntrst = 50;
int blueCntrst = 50;
int ptchDiameter = 100;
int nCycles = 0;
int orientations[1] = { 0 };
float spatialFrequencies[1] = { 1.0 };
float temporalFrequencies[1] = { 1.0 };
int counterPhases[1] = { 0 };
float redContrasts[1] = { 50.0 };
float greenContrasts[1] = { 50.0 };
float blueContrasts[1] = { 50.0 };
int patchSizes[1] = { 100 };

int pick_stim_type()
{
	static int stimLoc = 0;
	static int rs_shift = 10;
	int retCode = 0;
	int i, j;
	
	if(--trlcntr <= NUM_STIM_TYPE) {
		if (stmerr > 10 ) {	/* present errors if more than 10 */
			trlcntr = stmerr;
			for (j = 0; j < trlcntr; j++) ptrlst[j] = errlst[j];
			fixerr++;
		}

		/* If there were 10 or less errors, shuffle the error */
		/* conditions into the next block of trials. */
		else {
			trlcntr = NUM_STIM_TYPE + stmerr;
			for (i = 0; i < NUM_STIM_TYPE; i++ ) ptrlst[i] = i;
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
	stimType = stm_list[curstm].typeStim;
	fred = stm_list[curstm].forgrndRed;
	fgreen = stm_list[curstm].forgrndGreen;
	fblue = stm_list[curstm].forgrndBlue;
	bred = stm_list[curstm].bakgrndRed;
	bgreen = stm_list[curstm].bakgrndGreen;
	bblue = stm_list[curstm].bakgrndBlue;

	fstmx = (float)loc_list[stimLoc].x;
	fstmy = (float)loc_list[stimLoc].y;
	trgXpos = (long)loc_list[stimLoc].x;
	trgYpos = (long)loc_list[stimLoc].y;
	if(++stimLoc >= NUM_STIM_LOC) stimLoc = 0;

	fpx = wndXpos;
	fpy = wndYpos;
	
	return(retCode);
}

int pickWalshPattern()
{
	int retCode = 0;
	float frand;

	frand = (float)rand() / (float)RAND_MAX;

	walsh = (int)(255.0 * frand);

	retCode = 3000 + walsh;

	return(retCode);
}

int pickRandomPattern()
{
	int retCode = 0;

	return(retCode);
}

int pickAnnulusPattern()
{
	int retCode = 0;

	return(retCode);
}

int pickBarPattern()
{
	int retCode = 0;
	float frand;

	frand = (float)rand() / (float)RAND_MAX;

	barAngle = (int)(36.0 * frand) * 10;

	retCode = 3600 + barAngle;
	return(retCode);
}

int pickFlowPattern()
{
	static int flowPar = 0;
	int retCode = 0;
	float frand;

	flwWdth = flowWidth;
	flwHght = flowHeight;
	flwNr = flowNear;
	flwFr = flowFar;
	flwCv = flowCov;
	flwSpn = span;
	flwChr = coher;
	xyAng = flow_list[flowPar].xy;
	zAng = flow_list[flowPar].z;
	vel = flow_list[flowPar].v;
	roll = flow_list[flowPar].r;
	pitch = flow_list[flowPar].p;
	yaw = flow_list[flowPar].y;
	if(++flowPar >= NUM_FLOW_TYPE) flowPar = 0;

	return(retCode);
}

int pickUserPattern()
{
	int retCode = 0;

	if(++userPat >= 1008) userPat = 1000;
	retCode = 3000 + userPat;	

	return(retCode);
}

int pickTiffPattern()
{
	int retCode = 0;

	if(++tiffPat >= 106) tiffPat = 101;
	retCode = 4100 + tiffPat;	

	return(retCode);
}

int pickPngPattern()
{
	int retCode = 0;

	if(++pngPat >= 106) pngPat = 101;
	retCode = 4300 + pngPat;	

	return(retCode);
}

int pickOknPattern()
{
	int retCode = 0;

	switch(oknDir) {
	case 0:
		oknDir = 90;
		break;
	case 90:
		oknDir = 180;
		break;
	case 180:
		oknDir = 270;
		break;
	case 270:
		oknDir = 0;
		break;
	}
	return(retCode);
}

int pickRampPattern()
{
	int retCode = 0;
	double theta = 0.0;
	int i;

	for(i = 0; i < 120; ++i) {
		rampXs[i] = (float)(cos(theta) * 150.0);
		rampYs[i] = 0.0;
		theta += 0.0523599;
	}
	rampSteps = i;
	numTrigs = 1;
	trigs = 60;
	return(retCode);
}

int pickFlow_RampPattern()
{
	int retCode = 0;
	static int flwAngle = 90;
	static int rmpAngle = 0;

	flwWdth = 400;
	flwHght = 200;
	flwNr = 1;
	flwFr = 1;
	flwCv = flowCov;
	flwSpn = 0;
	flwChr = 100;
	if(flwAngle == 90) flwAngle = 270;
	else flwAngle = 90;
	xyAng = flwAngle;
	zAng = 0;
	vel = 20;
	pitch = 0;
	yaw = 0;
	roll = 0;

	rampLen = 200;
	if(rmpAngle == 0) rmpAngle = 180;
	else rmpAngle = 0;
	rampDir = rmpAngle;
	rampVel = 20;
	rampX = 0;
	rampY = 0;
	rampType = RA_CENPT;
	return(retCode);
}

int pickSinePattern()
{
	int retCode = 0;

	orientations[0] = orntn;
	spatialFrequencies[0] = (float)spatFreq / 10.0;
	temporalFrequencies[0] = (float)tempFreq/ 10.0;
	counterPhases[0] = cntrPhase;
	redContrasts[0] = (float)redCntrst;
	greenContrasts[0] = (float)greenCntrst;
	blueContrasts[0] = (float)blueCntrst;
	patchSizes[0] = ptchDiameter;

	return(retCode);
}

int pickSndWave()
{
	int retCode = 0;

	if(++sndWav >= 106) sndWav = 101;
	retCode = 4700 + sndWav;

	return(retCode);
}

int correctTrial()
{
	score(YES);
	correctTrials++;
	if(totalTrials) percentCorrect = 100 * correctTrials / totalTrials;
	return(0);
}

int errorTrial()
{
	score(NO);
	errlst[stmerr++] = curstm;
	errorTrials++;
	if(totalTrials) percentCorrect = 100 * correctTrials / totalTrials;
	return(0);
}

void rinitf(void)
{
	char *vexHost = "lsr-jwmvex";
	char *mexHost = "lsr-jwmmex";

	pcsConnectVex(vexHost);
	pcsConnectMex(mexHost);
	
	wd_src_pos(WIND0, WD_DIRPOS, 0, WD_DIRPOS, 0);
	wd_src_check(WIND0, WD_SIGNAL, 0, WD_SIGNAL, 1);
	wd_cntrl(WIND0, WD_ON);
	wd_src_pos(WIND1, WD_DIRPOS, 0, WD_DIRPOS, 0);
	wd_src_check(WIND1, WD_SIGNAL, 0, WD_SIGNAL, 1);
	wd_cntrl(WIND1, WD_ON);

	srand((unsigned)time(NULL));
}

/*
 * statlist menu
 */
 
VLIST state_vl[] = {
	"winXsiz",	&wndXsiz,	NP,	NP,	0,	ME_DEC,
	"winYsiz",	&wndYsiz,	NP,	NP,	0,	ME_DEC,
	"winXpos",	&wndXpos,	NP,	NP,	0,	ME_DEC,
	"winYpos",	&wndYpos,	NP,	NP,	0,	ME_DEC,
	"trgXsiz",  &trgXsiz,	NP,	NP,	0,	ME_DEC,
	"trgYsiz",  &trgYsiz,	NP,	NP,	0,	ME_DEC,
	"size",		&chkSiz,	NP,	NP,	0,	ME_DEC,
	"contrast", &cntrst,	NP,	NP,	0,	ME_DEC,
	"total_cycles",	&cyc_total,	NP,	NP,	0,	ME_DEC,
	NS,
};
char hm_sv_vl[]= "\
Some basic parameters";

VLIST randomPattern_vl[] = {
	"number_Cols", &rndCols,	NP,	NP,	0,	ME_DEC,
	"number_Rows", &rndRows,	NP,	NP,	0,	ME_DEC,
	"prcnt_white", &rndWhite,	NP,	NP,	0,	ME_DEC,
	NS,
};
char hm_random_vl[] = "\
Random dot pattern parameters";

VLIST annulusPattern_vl[] = {
	"inner_diameter", &annInner,	NP,	NP,	0,	ME_DEC,
	"outer_diameter", &annOuter,	NP,	NP,	0,	ME_DEC,
	NS,
};
char hm_annulus_vl[] = "\
Annulus parameters";

VLIST barPattern_vl[] = {
	"bar_length", &barLength,	NP,	NP,	0,	ME_DEC,
	"bar_width", &barWidth,		NP,	NP,	0,	ME_DEC,
	NS,
};
char hm_bar_vl[] = "";

VLIST flowPattern_vl[] = {
	"pattern_width", &flowWidth,	NP,	NP,	0,	ME_DEC,
	"pattern_height", &flowHeight,	NP,	NP,	0,	ME_DEC,
	"near_distance", &flowNear,		NP,	NP,	0,	ME_DEC,
	"far_distance", &flowFar,		NP,	NP,	0,	ME_DEC,
	"radius",		&flowRadius,	NP,	NP,	0,	ME_DEC,
	"coverage",		&flowCov,		NP,	NP,	0,	ME_DEC,
	"life_span",	&span,			NP,	NP,	0,	ME_DEC,
	"coherance",	&coher,			NP,	NP,	0,	ME_DEC,
	NS,
};
char hm_flow_vl[] = "";

VLIST sinePattern_vl[] = {
	{"grating_orientation",	&orntn,			NP,	NP,	0,	ME_DEC},
	{"spatial_frequency",	&spatFreq,		NP, NP, 0,	ME_DEC},
	{"temporal_frequency",	&tempFreq,		NP, NP, 0,	ME_DEC},
	{"counterphase",		&cntrPhase,		NP, NP, 0,	ME_DEC},
	{"red_contrast",		&redCntrst,		NP, NP, 0,	ME_DEC},
	{"green_contrast",		&greenCntrst,	NP,	NP,	0,	ME_DEC},
	{"blue_contrast",		&blueCntrst,	NP, NP, 0,	ME_DEC},
	{"patch_diameter",		&ptchDiameter,	NP, NP, 0,	ME_DEC},
	{"drift_cycles",		&nCycles,		NP, NP, 0,	ME_DEC},
	NS
};
char hm_sine_vl[] = "";

MENU umenus[] = {
"state_vars",	&state_vl,			NP,	NP,	0,	NP, hm_sv_vl,
"random_vars",	&randomPattern_vl,	NP,	NP,	0,	NP,	hm_random_vl,
"annulus_vars",	&annulusPattern_vl,	NP,	NP,	0,	NP,	hm_annulus_vl,
"bar_vars",		&barPattern_vl,		NP,	NP,	0,	NP,	hm_bar_vl,
"flow_vars",	&flowPattern_vl,	NP,	NP,	0,	NP,	hm_flow_vl,
"sine_vars",	&sinePattern_vl,	NP,	NP,	0,	NP,	hm_sine_vl,
NS,
};

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
		do PsetFlag(&goFlag, 0);
		to pckstm
	pckstm:
		do pick_stim_type()
		to setewsiz
	setewsiz:
		do Pwd_siz(&wind0Num, &wndXsiz, &wndYsiz)
		to setewpos
	setewpos:
		do Pwd_pos(&wind0Num, &wndXpos, &wndYpos)
		to setewctrl
	setewctrl:
		do Pwd_cntrl(&wind0Num, &wdOn)
		to settwsiz
	settwsiz:
		do Pwd_siz(&wind1Num, &trgXsiz, &trgYsiz)
		to settwpos
	settwpos:
		do Pwd_pos(&wind1Num, &trgXpos, &trgYpos)
		to settwctrl
	settwctrl:
		do Pwd_cntrl(&wind1Num, &wdOn)
		to setfpPos
	setfpPos:
		do PvexSetFixLocation(&fpx, &fpy)
		to setStmPos on 0 % tst_rx_new	/* wait for vex response */
	setStmPos:
		do PvexStimLocation(&numObj, &objects, &fstmx, &fstmy)
		to setStmColor on 0 % tst_rx_new
	setStmColor:
		do PvexSetStimColors(&numObj, &objects, &fred, &fgreen, &fblue, &bred, &bgreen, &bblue)
		to awnopn on 0 % tst_rx_new
	awnopn:
		do awind(OPEN_W)
		to fpncmd
	fpncmd:
		do PvexSwitchFix(&objOn)
		to fpon on 0 % tst_rx_new	/* wait for vex response */
	fpon:
		code FPONCD
		do PsetFlag(&goFlag, GO)
		to waitStim
	waitStim:
		to fpdcmd on -GO & goFlag
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
		to correct on -1 & errorFlag
		to error on +1 & errorFlag
	correct:
		code REWCD
		do correctTrial()
		to rewon
	rewon:
		do dio_on(REW)
		time 40
		to rewoff
	rewoff:
		do dio_off(REW)
		to fpfcmd
	error:
		code ERRCD
		do errorTrial()
		to fpfcmd
	fpfcmd:
		do PvexSwitchFix(&objOff)
		to fpoff on 0 % tst_rx_new	/* wait for vex response */
	fpoff:
		code FPOFFCD
		time 1000
		to first
}

walshStim {
status ON
begin	walshfrst:
		to walshdsabl
	walshdsabl:
		to walshcheck on -PSTOP & softswitch
	walshcheck:
		to walshGo on +WALSH_STIM = stimType
		to walshfrst on +PSTOP & softswitch
	walshGo:
		code WALSH_STIM
		do PsetFlag(&stimType, 0)
		rl 20
		to pickWalsh on +GO & goFlag
	pickWalsh:
		do pickWalshPattern()
		to drawWalsh
	drawWalsh:
		do PvexDrawWalsh(&numObj, &objects, &walsh, &chkSiz, &cntrst)
		to walshOnCmd on 0 % tst_rx_new
	walshOnCmd:
		do PvexSwitchStim(&numObj, &objects, &objOn)
		to walshOn on 0 % tst_rx_new
	walshOn:
		code STIMCD
		rl 50
		time 500
		rand 500
		to walshError on +WD0_XY & eyeflag
		to walshCorrect
	walshError:
		do PsetFlag(&errorFlag, 1)
		to walshOffCmd
	walshCorrect:
		do PsetFlag(&errorFlag, 0)
		to walshOffCmd
	walshOffCmd:
		do PvexSwitchStim(&numObj, &objects, &objOff)
		to walshDone on 0 % tst_rx_new
	walshDone:
		code STOFFCD
		do PsetFlag(&goFlag, DONE)
		rl 0
		to walshfrst
}

randomStim {
status ON
begin	randomfrst:
		to randomdsbl
	randomdsbl:
		to randomcheck on -PSTOP & softswitch
	randomcheck:
		to randomGo on +RANDOM_STIM = stimType
		to randomfrst on +PSTOP & softswitch
	randomGo:
		code RANDOM_STIM
		do PsetFlag(&stimType, 0)
		rl 20
		to pickRandom on +GO & goFlag
	pickRandom:
		do pickRandomPattern()
		to drawRandom
	drawRandom:
		do PvexDrawRandom(&numObj, &objects, &rndCols, &rndRows, &chkSiz, &rndWhite)
		to randomOnCmd on 0 % tst_rx_new
	randomOnCmd:
		do PvexSwitchStim(&numObj, &objects, &objOn)
		to randomOn on 0 % tst_rx_new
	randomOn:
		code STIMCD
		rl 50
		time 500
		rand 500
		to randomError on +WD0_XY & eyeflag
		to randomCorrect
	randomError:
		do PsetFlag(&errorFlag, 1)
		to randomOffCmd
	randomCorrect:
		do PsetFlag(&errorFlag, 0)
		to randomOffCmd
	randomOffCmd:
		do PvexSwitchStim(&numObj, &objects, &objOff)
		to randomDone on 0 % tst_rx_new
	randomDone:
		code STOFFCD
		do PsetFlag(&goFlag, DONE)
		rl 0
		to randomfrst
}

annulusStim {
status ON
begin	annulusfrst:
		to annulusdsbl
	annulusdsbl:
		to annuluscheck on -PSTOP & softswitch
	annuluscheck:
		to annulusGo on +ANNULUS_STIM = stimType
		to annulusfrst on +PSTOP & softswitch
	annulusGo:
		code ANNULUS_STIM
		do PsetFlag(&stimType, 0)
		rl 20
		to pickAnnulus on +GO & goFlag
	pickAnnulus:
		do pickAnnulusPattern()
		to drawAnnulus
	drawAnnulus:
		do PvexDrawAnnulus(&numObj, &objects, &annOuter, &annInner, &cntrst)
		to annulusOnCmd on 0 % tst_rx_new
	annulusOnCmd:
		do PvexSwitchStim(&numObj, &objects, &objOn)
		to annulusOn on 0 % tst_rx_new
	annulusOn:
		code STIMCD
		rl 50
		time 500
		rand 500
		to annulusError on +WD0_XY & eyeflag
		to annulusCorrect
	annulusError:
		do PsetFlag(&errorFlag, 1)
		to annulusOffCmd
	annulusCorrect:
		do PsetFlag(&errorFlag, 0)
		to annulusOffCmd
	annulusOffCmd:
		do PvexSwitchStim(&numObj, &objects, &objOff)
		to annulusDone on 0 % tst_rx_new
	annulusDone:
		code STOFFCD
		do PsetFlag(&goFlag, DONE)
		rl 0
		to annulusfrst
}

barStim {
status ON
begin	barfrst:
		to bardsbl
	bardsbl:
		to barcheck on -PSTOP & softswitch
	barcheck:
		to barGo on +BAR_STIM = stimType
		to barfrst on +PSTOP & softswitch
	barGo:
		code BAR_STIM
		do PsetFlag(&stimType, 0)
		rl 20
		to pickBar on +GO & goFlag
	pickBar:
		do pickBarPattern()
		to drawBar
	drawBar:
		do PvexDrawBar(&numObj, &objects, &barAngle, &barWidth, &barLength, &cntrst)
		to barOnCmd on 0 % tst_rx_new
	barOnCmd:
		do PvexSwitchStim(&numObj, &objects, &objOn)
		to barOn on 0 % tst_rx_new
	barOn:
		code STIMCD
		rl 50
		time 500
		rand 500
		to barError on +WD0_XY & eyeflag
		to barCorrect
	barError:
		do PsetFlag(&errorFlag, 1)
		to barOffCmd
	barCorrect:
		do PsetFlag(&errorFlag, 0)
		to barOffCmd
	barOffCmd:
		do PvexSwitchStim(&numObj, &objects, &objOff)
		to barDone on 0 % tst_rx_new
	barDone:
		code STOFFCD
		do PsetFlag(&goFlag, DONE)
		rl 0
		to barfrst
}

flowStim {
status ON
begin	flowfrst:
		to flowdsbl
	flowdsbl:
		to flowcheck on -PSTOP & softswitch
	flowcheck:
		to flowGo on +FLOW_STIM = stimType
		to flowfrst on +PSTOP & softswitch
	flowGo:
		code FLOW_STIM
		do PsetFlag(&stimType, 0)
		rl 20
		to pickFlow on +GO & goFlag
	pickFlow:
		do pickFlowPattern()
		to drawFlow on 0 = flowRadius
		to drawEllipFlow on 0 < flowRadius
	drawFlow:
		do PvexDrawFlowField(&numObj, &objects, &flwWdth, &flwHght, &flwNr, &flwFr, &flwCv, &chkSiz)
		to newFlow on 0 % tst_rx_new
	drawEllipFlow:
		do PvexDrawEllipticalFlowField(&numObj, &objects, &flowRadius, &flwWdth, &flwHght, &flwNr, &flwFr, &flwCv, &chkSiz)
		to newFlow on 0 % tst_rx_new
	newFlow:
		do PvexNewFlow(&numObj, &objects, &xyAng, &zAng, &vel, &roll, &pitch, &yaw, &flwSpn, &flwChr)
		to flowOnCmd on 0 % tst_rx_new
	flowOnCmd:
		do PvexSwitchStim(&numObj, &objects, &objOn)
		to flowOn on 0 % tst_rx_new
	flowOn:
		code STIMCD
		rl 30
		time 50
		to flowError on +WD0_XY & eyeflag
		to flowStartCmd
	flowStartCmd:
		do PvexStartFlow(&numObj, &objects)
		to flowRunning on 0 % tst_rx_new
	flowRunning:
		code FLOWON
		rl 50
		time 1000
		rand 1000
		to flowError on +WD0_XY & eyeflag
		to flowCorrect
	flowError:
		do PsetFlag(&errorFlag, 1)
		to flowStopCmd
	flowCorrect:
		do PsetFlag(&errorFlag, 0)
		to flowStopCmd
	flowStopCmd:
 		do PvexStopFlowRamp()
		to flowStop on 0 % tst_rx_new
	flowStop:
		code FLOWOFF
		rl 30
		time 50
		to flowOffCmd
	flowOffCmd:
		do PvexSwitchStim(&numObj, &objects, &objOff)
		to flowDone on 0 % tst_rx_new
	flowDone:
		code STOFFCD
		do PsetFlag(&goFlag, DONE)
		rl 0
		to flowfrst
}

userStim {
status ON
begin	userfrst:
		to userdsbl
	userdsbl:
		to usercheck on -PSTOP & softswitch
	usercheck:
		to userGo on +USER_STIM = stimType
		to userfrst on +PSTOP & softswitch
	userGo:
		code USER_STIM
		do PsetFlag(&stimType, 0)
		rl 20
		to pickUser on +GO & goFlag
	pickUser:
		do pickUserPattern()
		to drawUser
	drawUser:
		do PvexDrawUserPattern(&numObj, &objects, &userPat, &chkSiz, &cntrst)
		to userOnCmd on 0 % tst_rx_new
	userOnCmd:
		do PvexSwitchStim(&numObj, &objects, &objOn)
		to userOn on 0 % tst_rx_new
	userOn:
		code STIMCD
		rl 50
		time 500
		rand 500
		to userError on +WD0_XY & eyeflag
		to userCorrect
	userError:
		do PsetFlag(&errorFlag, 1)
		to userOffCmd
	userCorrect:
		do PsetFlag(&errorFlag, 0)
		to userOffCmd
	userOffCmd:
		do PvexSwitchStim(&numObj, &objects, &objOff)
		to userDone on 0 % tst_rx_new
	userDone:
		code STOFFCD
		do PsetFlag(&goFlag, DONE)
		rl 0
		to userfrst
}

tiffStim {
status ON
begin	tifffrst:
		to tiffdsbl
	tiffdsbl:
		to tiffcheck on -PSTOP & softswitch
	tiffcheck:
		to tiffGo on +TIFF_STIM = stimType
		to tifffrst on +PSTOP & softswitch
	tiffGo:
		code TIFF_STIM
		do PsetFlag(&stimType, 0)
		rl 20
		to pickTiff on +GO & goFlag
	pickTiff:
		do pickTiffPattern()
		to drawTiff
	drawTiff:
		do PvexDrawTiffImage(&numObj, &objects, &tiffPat)
		to tiffOnCmd on 0 % tst_rx_new
	tiffOnCmd:
		do PvexSwitchStim(&numObj, &objects, &objOn)
		to tiffOn on 0 % tst_rx_new
	tiffOn:
		code STIMCD
		rl 50
		time 500
		rand 500
		to tiffError on +WD0_XY & eyeflag
		to tiffCorrect
	tiffError:
		do PsetFlag(&errorFlag, 1)
		to tiffOffCmd
	tiffCorrect:
		do PsetFlag(&errorFlag, 0)
		to tiffOffCmd
	tiffOffCmd:
		do PvexSwitchStim(&numObj, &objects, &objOff)
		to tiffDone on 0 % tst_rx_new
	tiffDone:
		code STOFFCD
		do PsetFlag(&goFlag, DONE)
		rl 0
		to tifffrst
}

pngStim {
status ON
begin	pngfrst:
		to pngdsbl
	pngdsbl:
		to pngcheck on -PSTOP & softswitch
	pngcheck:
		to pngGo on +PNG_STIM = stimType
		to pngfrst on +PSTOP & softswitch
	pngGo:
		code PNG_STIM
		do PsetFlag(&stimType, 0)
		rl 20
		to pickPng on +GO & goFlag
	pickPng:
		do pickPngPattern()
		to drawPng
	drawPng:
		do PvexDrawPngImage(&numObj, &objects, &pngPat)
		to pngOnCmd on 0 % tst_rx_new
	pngOnCmd:
		do PvexSwitchStim(&numObj, &objects, &objOn)
		to pngOn on 0 % tst_rx_new
	pngOn:
		code STIMCD
		rl 50
		time 500
		rand 500
		to pngError on +WD0_XY & eyeflag
		to pngCorrect
	pngError:
		do PsetFlag(&errorFlag, 1)
		to pngOffCmd
	pngCorrect:
		do PsetFlag(&errorFlag, 0)
		to pngOffCmd
	pngOffCmd:
		do PvexSwitchStim(&numObj, &objects, &objOff)
		to pngDone on 0 % tst_rx_new
	pngDone:
		code STOFFCD
		do PsetFlag(&goFlag, DONE)
		rl 0
		to pngfrst
}

oknStim {
status ON
begin	oknfrst:
		to okndsbl
	okndsbl:
		to okncheck on -PSTOP & softswitch
	okncheck:
		to oknGo on +OKN_STIM = stimType
		to oknfrst on +PSTOP & softswitch
	oknGo:
		code OKN_STIM
		do PsetFlag(&stimType, 0)
		rl 20
		to pickOkn on +GO & goFlag
	pickOkn:
		do pickOknPattern()
		to setOknPos
	setOknPos:
		do PvexStimLocation(&numObj, &objects, &foknx, &fokny)
		to drawOkn on 0 % tst_rx_new
	drawOkn:
		do PvexDrawOknGrating(&numObj, &objects, &oknDir, &oknSpeed, &oknWidth, &oknHeight, &oknBar)
		to oknOnCmd on 0 % tst_rx_new
	oknOnCmd:
		do PvexSwitchStim(&numObj, &objects, &objOn)
		to oknOn on 0 % tst_rx_new
	oknOn:
		code STIMCD
		rl 30
		time 50
		to oknError on +WD0_XY & eyeflag
		to oknStartCmd
	oknStartCmd:
		do PvexStartOkn(&numObj, &objects)
		to oknRunning on 0 % tst_rx_new
	oknRunning:
		code OKNON
		rl 50
		time 1000
		rand 1000
		to oknError on +WD0_XY & eyeflag
		to oknCorrect
	oknError:
		do PsetFlag(&errorFlag, 1)
		to oknStopCmd
	oknCorrect:
		do PsetFlag(&errorFlag, 0)
		to oknStopCmd
	oknStopCmd:
		do PvexStopOkn()
		to oknStop on 0 % tst_rx_new
	oknStop:
		code OKNOFF
		rl 30
		time 50
		to oknOffCmd
	oknOffCmd:
		do PvexSwitchStim(&numObj, &objects, &objOff)
		to oknDone on 0 % tst_rx_new
	oknDone:
		code STOFFCD
		do PsetFlag(&goFlag, DONE)
		rl 0
		to oknfrst
}

rampStim {
status ON
begin	rampfrst:
		to rampdsbl
	rampdsbl:
		to rampcheck on -PSTOP & softswitch
	rampcheck:
		to rampGo on +RAMP_STIM = stimType
		to rampfrst on +PSTOP & softswitch
	rampGo:
		code RAMP_STIM
		do PsetFlag(&stimType, 0)
		rl 20
		to pickRamp on +GO & goFlag
	pickRamp:
		do pickRampPattern()
		to newRampCmd
	newRampCmd:
		do PvexLoadRamp(&numRamps, &ramps, &rampAct, &rampSteps, rampXs, rampYs)
		to setRampTrigs on 0 % tst_rx_new
	setRampTrigs:
		do PvexSetTriggers(&numRamps, &ramps, &numTrigs, &trigs)
		to setRampPos on 0 % tst_rx_new
	setRampPos:
		do PvexToRampStart(&numObj, &objects, &ramps)
		to drawRampStim on 0 % tst_rx_new
	drawRampStim:
		do PvexDrawTiffImage(&numObj, &objects, &rampTiff)
		to rampStimOnCmd on 0 % tst_rx_new
	rampStimOnCmd:
		do PvexSwitchStim(&numObj, &objects, &objOn)
		to rampStimOn on 0 % tst_rx_new
	rampStimOn:
		code STIMCD
		rl 30
		time 50
		to rampError on +WD0_XY & eyeflag
		to rampStartCmd
	rampStartCmd:
		do PvexStartRamp(&numRamps, &ramps, &objects, &rampCycles)
		to rampForward on 0 % tst_rx_new
	rampForward:
		code RAMPON
		rl 50
		to rampError on +WD0_XY & eyeflag
		to rampBack on 0 % tst_rx_new
	rampBack:
		code RAMP_BACK
		rl 60
		to rampError on +WD0_XY & eyeflag
		to rampCorrect on 0 % tst_rx_new
	rampError:
		do PsetFlag(&errorFlag, 1)
		to rampStopCmd
	rampCorrect:
		do PsetFlag(&errorFlag, 0)
		to rampStop
	rampStopCmd:
		do PvexStopFlowRamp()
		to rampStop on 0 % tst_rx_new
	rampStop:
		code RAMPOFF
		rl 30
		time 50
		to rampOffCmd
	rampOffCmd:
		do PvexSwitchStim(&numObj, &objects, &objOff)
		to rampDone on 0 % tst_rx_new
	rampDone:
		code STOFFCD
		do PsetFlag(&goFlag, DONE)
		rl 0
		to rampfrst
}

flow_rampStim {
status ON
begin	flow_rampfrst:
		to flow_rampdsbl
	flow_rampdsbl:
		to flow_rampcheck on -PSTOP & softswitch
	flow_rampcheck:
		to flow_rampGo on +FLOW_RAMP_STIM = stimType
		to flow_rampfrst on +PSTOP & softswitch
	flow_rampGo:
		code FLOW_RAMP_STIM
		do PsetFlag(&stimType, 0)
		rl 20
		to pickFlow_Ramp on +GO & goFlag
	pickFlow_Ramp:
		do pickFlow_RampPattern()
		to drawFlow_RampStim
	drawFlow_RampStim:
		do PvexDrawAnnulus(&numObj, &objects, &rmpAnnOuter, &rmpAnnInner, &cntrst)
		to newFlow_RampCmd on 0 % tst_rx_new
	newFlow_RampCmd:
		do PvexNewRamp(&numRamps, &flwRamps, &rampLen, &rampDir, &rampVel, &rampX, &rampY, &rampType, &rampAct)
		to drawFlow_RampFlow on 0 % tst_rx_new
	drawFlow_RampFlow:
		do PvexDrawFlowField(&numObj, &flowObject, &flwWdth, &flwHght, &flwNr, &flwFr, &flwCv, &chkSiz)
		to newFlow_RampFlow on 0 % tst_rx_new
	newFlow_RampFlow:
		do PvexNewFlow(&numObj, &flowObject, &xyAng, &zAng, &vel, &roll, &pitch, &yaw, &flwSpn, &flwChr)
		to setFlow_RampFlowPos on 0 % tst_rx_new
	setFlow_RampFlowPos:
		do PvexStimLocation(&numObj, &flowObject, &fpx, &fpy)
		to setFlow_RampRampPos on 0 % tst_rx_new
	setFlow_RampRampPos:
		do PvexToRampStart(&numObj, &objects, &flwRamps)
		to flow_rampStimOnCmd on 0 % tst_rx_new
	flow_rampStimOnCmd:
		do PvexSwitchStim(&numFlwRmpObj, flwRmpObjects, flwRmpObjOn)
		to flow_rampStimOn on 0 % tst_rx_new
	flow_rampStimOn:
		code STIMCD
		rl 30
		time 50
		to flow_rampError on +WD0_XY & eyeflag
		to flow_rampStartCmd
	flow_rampStartCmd:
		do PvexStartFlowRamp(&numObj, &flowObject, &numRamps, &flwRamps, &objects)
		to flow_rampForward on 0 % tst_rx_new
	flow_rampForward:
		code FLOW_RAMPON
		rl 50
		to flow_rampError on +WD0_XY & eyeflag
		to flow_rampRampStop on 0 % tst_rx_new
	flow_rampRampStop:
		code RAMPOFF
		rl 60
		to flow_rampCorrect
	flow_rampError:
		do PsetFlag(&errorFlag, 1)
		to flow_rampStopCmd
	flow_rampCorrect:
		do PsetFlag(&errorFlag, 0)
		to flow_rampStopCmd
	flow_rampStopCmd:
		do PvexStopFlowRamp()
		to flow_rampFlowStop on 0 % tst_rx_new
	flow_rampFlowStop:
		code FLOWOFF
		rl 30
		time 50
		to flow_rampOffCmd
	flow_rampOffCmd:
		do PvexSwitchStim(&numFlwRmpObj, flwRmpObjects, flwRmpObjOff)
		to flow_rampDone on 0 % tst_rx_new
	flow_rampDone:
		code STOFFCD
		do PsetFlag(&goFlag, DONE)
		rl 0
		to flow_rampfrst
}

sineStim {
status ON
begin	sinefrst:
		to sinedsbl
	sinedsbl:
		to sinecheck on -PSTOP & softswitch
	sinecheck:
		to sineGo on +SINE_STIM = stimType
		to sinefrst on +PSTOP & softswitch
	sineGo:
		code SINE_STIM
		do PsetFlag(&stimType, 0);
		rl 20
		to pickSine on +GO & goFlag
	pickSine:
		do pickSinePattern()
		to drawSine
	drawSine:
		do PvexDrawSineGrating(&numObj, &objects, orientations, spatialFrequencies, temporalFrequencies, counterPhases, redContrasts, greenContrasts, blueContrasts, patchSizes)
		to sineOnCmd on 0 % tst_rx_new
	sineOnCmd:
		do PvexSwitchStim(&numObj, &objects, &objOn)
		to sineOn on 0 % tst_rx_new
	sineOn:
		code STIMCD
		rl 50
		time 500
		rand 500
		to sineError on +WD0_XY & eyeflag
		to chooseDrift
	chooseDrift:
		to startDrift on 0 = nCycles
		to timeDrift on 0 < nCycles
	startDrift:
		do PvexStartSine(&numObj, &objects);
		to driftRunning on 0 % tst_rx_new
	driftRunning:
		code SINEON
		time 1000
		to sineError on +WD0_XY & eyeflag
		to stopDrift
	stopDrift:
		do PvexStopSine()
		to driftOff on 0 % tst_rx_new
	timeDrift:
		do PvexTimeGrating(&numObj, &objects, &nCycles);
		to timingDrift on 0 % tst_rx_new
	timingDrift:
		code SINEON
		to sineError on +WD0_XY & eyeflag
		to driftOff on 0 % tst_rx_new	
	driftOff:
		code SINEOFF
		time 1000		
		to sineError on +WD0_XY & eyeflag
		to sineCorrect
	sineError:
		do PsetFlag(&errorFlag, 1);
		to stopDrftCmd
	stopDrftCmd:
		do PvexStopSine()
		to sineOffCmd on 0 % tst_rx_new
	sineCorrect:
		do PsetFlag(&errorFlag, 0);
		to sineOffCmd
	sineOffCmd:
		do PvexSwitchStim(&numObj, &objects, &objOff)
		to sineDone on 0 % tst_rx_new
	sineDone:
		code STOFFCD
		do PsetFlag(&goFlag, DONE)
		rl 0
		to sinefrst
}

sndStim {
status ON
begin	sndfrst:
		to snddsbl
	snddsbl:
		to sndcheck on -PSTOP & softswitch
	sndcheck:
		to sndGo on + SOUND_STIM = stimType
		to sndfrst on +PSTOP & softswitch
	sndGo:
		code SOUND_STIM
		do PsetFlag(&stimType, 0)
		rl 20
		to pickSnd on +GO & goFlag
	pickSnd:
		do pickSndWave()
		to loadSnd
	loadSnd:
		do PvexLoadSound(&numObj, &objects, &sndWav)
		to sndOnCmd on 0 % tst_rx_new
	sndOnCmd:
		do PvexSwitchStim(&numObj, &objects, &objOn)
		to sndOn on 0 % tst_rx_new
	sndOn:
		code STIMCD
		rl 50
		time 500
		rand 500
		to sndError on +WD0_XY & eyeflag
		to sndCorrect
	sndError:
		do PsetFlag(&errorFlag, 1)
		to sndOffCmd
	sndCorrect:
		do PsetFlag(&errorFlag, 0)
		to sndOffCmd
	sndOffCmd:
		do PvexSwitchStim(&numObj, &objects, &objOff)
		to sndDone on 0 % tst_rx_new
	sndDone:
		code STOFFCD
		do PsetFlag(&goFlag, DONE)
		rl 0
		to sndfrst
}

photoTrig {
status ON
begin	phfrst:
		to phdsbl
	phdsbl:
		to reset on -PSTOP & drinput
	reset:
		do PsetFlag(&photoTrigger, 0)
		time 10
		to rstpht
	rstpht:
		do dio_on(PHOTO_RESET_BIT)
		time 10
		to clrrst
	clrrst:
		do dio_off(PHOTO_RESET_BIT)
 		to photo on +PHOTO_CELL_INPUT & drinput
	photo:
		do PsetFlag(&photoTrigger, 1)
		to phfrst
}

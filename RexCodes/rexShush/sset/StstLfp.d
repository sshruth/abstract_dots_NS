#include "../hdr/GLcommand.h"
#include "lcode_tst.h"

#define WIND0 0
#define WIND1 1

int nObjects = 1;
int objList[1] = { 1 };
float targXlist[1] = { 50.0 };
float targYlist[1] = { 0.0 };
int orientations[1] = { 0 };
float spatialFrequencies[1] = { 1.0 };
float temporalFrequencies[1] = { 1.0 };
int counterPhases[1] = { 0 };
float redContrasts[1] = { 50.0 };
float greenContrasts[1] = { 50.0 };
float blueContrasts[1] = { 50.0 };
int patchSizes[1] = { 100 };
int fixon = OBJ_ON;
int offList[1] = { OBJ_OFF };
int onList[1] = { OBJ_ON };
int fixoff = OBJ_OFF;
int cntrPhase = 0;
int ptchDiameter = 100;
int nCycles = 0;
int orntn;

int stim_X = 50;
int stim_Y = 0;
int spatFreq = 10;
int tempFreq = 10;
int redCntrst = 50;
int greenCntrst = 50;
int blueCntrst = 50.0;

int setStimuli(){
	int code = 0;

	targXlist[0] = (float)stim_X;
	targYlist[0] = (float)stim_Y;
	orientations[0] = orntn;
	spatialFrequencies[0] = (float)spatFreq / 10.0;
	temporalFrequencies[0] = (float)tempFreq/ 10.0;
	counterPhases[0] = cntrPhase;
	redContrasts[0] = (float)redCntrst;
	greenContrasts[0] = (float)greenCntrst;
	blueContrasts[0] = (float)blueCntrst;
	patchSizes[0] = ptchDiameter;
	
	return(code);
}


void rinitf()
{

	char *vexHost = "lsr-jwmvex";
	char *mexHost = "lsr-jwmrex";

	pcsConnectVex(vexHost);
	pcsConnectMex(mexHost);
	
	wd_src_pos(WIND0, WD_DIRPOS, 0, WD_DIRPOS, 0);
	wd_src_pos(WIND1, WD_DIRPOS, 0, WD_DIRPOS, 0);
	wd_src_check(WIND0, WD_SIGNAL, 0, WD_SIGNAL, 1);
	wd_src_check(WIND1, WD_SIGNAL, 2, WD_SIGNAL, 3);
	wd_cntrl(WIND0, WD_ON);
	wd_cntrl(WIND1, WD_ON);

}

int wndxctr = 0;
int wndyctr = 0;
int wndsiz = 5;
int cnt_wnd()
{
	int code = 0;

	wd_pos(WIND0, wndxctr, wndyctr);
	wd_disp(D_W_EYE_X);
	wd_siz(WIND0, wndsiz, wndsiz);
	wd_cntrl(WIND0, WD_ON);

	return(code);
}


VLIST state_vars_vl[] = {
	{"stimLoc_X",	&stim_X, NP, NP, 0, ME_DEC}, // 0 0
	{"stimLoc_Y",	&stim_Y, NP, NP, 0, ME_DEC}, // 0 35
	{"grating_orientation",	&orntn, NP, NP, 0, ME_DEC}, // 0 70
	{"spatial_frequency",	&spatFreq, NP, NP, 0, ME_DEC}, // 0 105
	{"temporal_frequency",	&tempFreq, NP, NP, 0, ME_DEC}, // 0 140
	{"counterphase",	&cntrPhase, NP, NP, 0, ME_DEC}, // 0 175
	{"red_contrast",	&redCntrst, NP, NP, 0, ME_DEC}, // 0 210
	{"green_contrast",	&greenCntrst, NP, NP, 0, ME_DEC}, // 0 245
	{"blue_contrast",	&blueCntrst, NP, NP, 0, ME_DEC}, // 0 280
	{"patch_diameter",	&ptchDiameter, NP, NP, 0, ME_DEC}, // 0 315
	{"drift_cycles",	&nCycles, NP, NP, 0, ME_DEC}, // 0 350
	NS
};

MENU umenus[] = {
	{"state_vars",	&state_vars_vl, NP, NP, 0, NP, hm_null}, // 457, 3, 150, 398, 1
	NS,
};

USER_FUNC ufuncs[] = { // EMPTY
	{""},
};

RTVAR rtvars[] = { // EMPTY
	{""},
};

%%
id 100
restart rinitf
main_set {
status ON
begin	first:
		to disable
	disable:
		to enable on -PSTOP & softswitch
	enable:
		code 1001
		to setStim
	setStim:
		do setStimuli()
		to targLoc
	targLoc:
		do PvexStimLocation(&nObjects, objList, targXlist, targYlist)
		to setGrating on 0 % tst_rx_new
	setGrating:
		do PvexDrawSineGrating(&nObjects, objList, orientations, spatialFrequencies, temporalFrequencies, counterPhases, redContrasts, greenContrasts, blueContrasts, patchSizes)
		to winset on 0 % tst_rx_new
	winset:
		do cnt_wnd()
		to openwin
	openwin:
		do awind(OPEN_W)
		to fponcmd
	fponcmd:
		do PvexSwitchFix(&fixon)
		to fpon on 0 % tst_rx_new
	fpon:
		code FPONCD
		time 1000
		to stmoncmd
	stmoncmd:
		do PvexSwitchStim(&nObjects, objList, onList)
		to stimon on 0 % tst_rx_new
	stimon:
		code STIMCD
		time 1000
		to chooseDrift
	chooseDrift:
		to startDrift on 0 = nCycles
		to timeDrift on 0 < nCycles
	startDrift:
		do PvexStartSine(&nObjects, objList)
		to startLfp on 0 % tst_rx_new
	startLfp:
		code 2000
		do mwind(OPEN_LFP)
		to driftRunning on 0 % tst_mx_new
	driftRunning:
		time 1000
		rand 500
		to stopDrift
	stopDrift:
		do PvexStopSine()
		to driftOff on 0 % tst_rx_new
	driftOff:
		time 1000
		rand 500
		to stmoffcmd
	stmoffcmd:
		do PvexSwitchStim(&nObjects, objList, offList)
		to stimoff on 0 % tst_rx_new
	stimoff:
		code 1101
		to stopLfp
	stopLfp:
		do mwind(CLOSE_LFP)
		to fpoffcmd on 0 % tst_mx_new
	fpoffcmd:
		do PvexSwitchFix(&fixoff)
		to closeWin on 0 % tst_rx_new
	closeWin:
		code FPOFFCD
		do awind(CLOSE_W)
		time 500
		to first
	timeDrift:
		do PvexTimeGrating(&nObjects, objList, &nCycles)
		to timingDrift on 0 % tst_rx_new
	timingDrift:
		code 3000
		to driftOff on 0 % tst_rx_new
}

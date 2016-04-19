#include "../hdr/GLcommand.h"
#include "lcode_tst.h"

#define WIND0 0
#define WIND1 1

int fixon = OBJ_ON;
int fixoff = OBJ_OFF;

int onList = OBJ_ON;
int offList = OBJ_OFF;

int nObjects = 1;
int objList = 1;

float targXlist = 50.0;
float targYlist = 0.0;
int orientations = 0;
float spatialFrequencies = 1.0;
float temporalFrequencies = 1.0;
int counterPhases = 0;
float redContrasts = 50.0;
float greenContrasts = 50.0;
float blueContrasts = 50.0;
int patchSizes = 100;
int nCycles = 0;


void rinitf()
{

	char *vexHost = "lsr-jwmvex";

	pcsConnectVex(vexHost);
	
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
	{"stimLoc_X",	&targXlist, NP, NP, 0, ME_FLOAT}, // 0 0
	{"stimLoc_Y",	&targYlist, NP, NP, 0, ME_FLOAT}, // 0 35
	{"grating_orientation",	&orientations, NP, NP, 0, ME_DEC}, // 0 70
	{"spatial_frequency",	&spatialFrequencies, NP, NP, 0, ME_FLOAT}, // 0 105
	{"temporal_frequency",	&temporalFrequencies, NP, NP, 0, ME_FLOAT}, // 0 140
	{"counterphase",	&counterPhases, NP, NP, 0, ME_DEC}, // 0 175
	{"red_contrast",	&redContrasts, NP, NP, 0, ME_FLOAT}, // 0 210
	{"green_contrast",	&greenContrasts, NP, NP, 0, ME_FLOAT}, // 0 245
	{"blue_contrast",	&blueContrasts, NP, NP, 0, ME_FLOAT}, // 0 280
	{"patch_diameter",	&patchSizes, NP, NP, 0, ME_DEC}, // 0 315
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
		to targLoc
	targLoc:
		do PvexStimLocation(&nObjects, &objList, &targXlist, &targYlist)
		to setGrating on 0 % tst_rx_new
	setGrating:
		do PvexDrawSineGrating(&nObjects, &objList, &orientations, &spatialFrequencies, &temporalFrequencies, &counterPhases, &redContrasts, &greenContrasts, &blueContrasts, &patchSizes)
		to winset on 0 % tst_rx_new
	winset:
		do cnt_wnd()
		to openwin
	fponcmd:
		do PvexSwitchFix(&fixon)
		to fpon on 0 % tst_rx_new
	fpon:
		code FPONCD
		time 1000
		to stmoncmd
	stmoncmd:
		do PvexSwitchStim(&nObjects, &objList, &onList)
		to stimon on 0 % tst_rx_new
	stimon:
		code STIMCD
		time 1000
		to chooseDrift
	chooseDrift:
		to startDrift on 0 = nCycles
		to timeDrift on 0 < nCycles
	startDrift:
		do PvexStartSine(&nObjects, &objList)
		to driftRunning on 0 % tst_rx_new
	driftRunning:
		code 2000
		time 1000
		to stopDrift
	stopDrift:
		do PvexStopSine()
		to driftOff on 0 % tst_rx_new
	timeDrift:
		do PvexTimeGrating(&nObjects, &objList, &nCycles)
		to timingDrift on 0 % tst_rx_new
	timingDrift:
		code 3000
		to driftOff on 0 % tst_rx_new
	driftOff:
		time 1000
		to stmoffcmd
	stmoffcmd:
		do PvexSwitchStim(&nObjects, &objList, &offList)
		to stimoff on 0 % tst_rx_new
	openwin:
		do awind(OPEN_W)
		to fponcmd
	stimoff:
		code 1101
		time 1000
		to fpoffcmd
	fpoffcmd:
		do PvexSwitchFix(&fixoff)
		to closeWin on 0 % tst_rx_new
	closeWin:
		code FPOFFCD
		do awind(CLOSE_W)
		time 500
		to first
}

#include "ramp.h"
#include "pcsSocket.h"
#include "GLcommand.h"

#define REW		Dio_id(PCDIO_DIO, 2, 0x80)
#define BEEP		Dio_id(PCDIO_DIO, 2, 0x40)
#define PHOTO_RESET_BIT Dio_id(PCDIO_DIO, 2, 0x04)
#define PHOTO_CELL_INPUT 0x1000
#define FPONCD		1010
#define FPDIMCD		1020
#define FPOFFCD		1025
#define REWCD		1030
#define STIMCD		1100
#define STOFFCD		1101
#define TARGONCD	1140
#define TARGOFFCD	1141
#define SACSTART	1300	/* beginning of a saccade */
#define WIND0 0
#define WIND1 1
#define DONE 1
#define CONT 2
#define LOCATE 4
#define GATEOPEN 1
#define GATECLOSE 0

float rampXs[1600];
float rampYs[1600];
int fixOn = OBJ_ON;
int fixOff = OBJ_OFF;
int rampState;
int nRamps = 2;
int objList[2];
int rampList[2] = { 1, 2 };
int actList[2] = { ON_AFTER_RAMP, ON_AFTER_RAMP };
int nSteps[2];
int sectorLength[2] = { 100, 50 };
int cycleList[2] = { RA_ONCE, RA_ONCE };

int onSwitches[2] = { OBJ_ON, OBJ_ON };
int offSwitches[2] = { OBJ_OFF, OBJ_OFF };

int trigList[2] = { 3, 3 };
int trigs[6];
int vexMessage;
int vexX;
int vexY;

int pick_stim()
{
	int i, j, k, l;
	static int currStim = 1;

	vexMessage = 0;

	if(++currStim > 4) currStim = 1;

//	objList[0] = currStim;
//	objList[1] = currStim + 1;
	objList[0] = 1;
	objList[1] = 2;

	k = 0;
	l = 0;
	for(i = 0; i < nRamps; i++) {
		rampXs[k] = -100.0 / (float)(i + 1);
		rampYs[k] = -100.0 / (float)(i + 1);
		k++;
		for(j = 1; j < sectorLength[i]; j++) {
			rampXs[k] = rampXs[k - 1] + 2.0;
			rampYs[k] = rampYs[k - 1];
			k++;
		}
		trigs[l] = j;
		l++;

		for(j = 0; j < sectorLength[i]; j++) {
			rampXs[k] = rampXs[k - 1];
			rampYs[k] = rampYs[k - 1] + 2.0;
			k++;
		}
		trigs[l] = j + trigs[l - 1];
		l++;

		for(j = 0; j < sectorLength[i]; j++) {
			rampXs[k] = rampXs[k - 1] - 2.0;
			rampYs[k] = rampYs[k - 1];
			k++;
		}
		trigs[l] = j + trigs[l - 1];
		l++;

		for(j = 0; j < sectorLength[i]; j++) {
			rampXs[k] = rampXs[k - 1];
			rampYs[k] = rampYs[k - 1] - 2.0;
			k++;
		}
		nSteps[i] = sectorLength[i] * 4;
	}
	rampState = 1200 + currStim * 100;
	return(0);
}

int dropTrigger()
{
	rampState++;
	return(rampState);
}

/*
 * this set of subroutines allow multiple chains to access GLvex safely
 */
int vexGate = GATECLOSE;
int closeVexGate()
{
	vexGate = GATECLOSE;
	return(0);
}

int openVexGate()
{
	vexGate = GATEOPEN;
	return(0);
}

long wndxctr = 0;
long wndyctr = 0;
long wndsiz = 5;
int trgsiz = 20;
int ctr_wnd()
{
	wd_pos(WIND0, wndxctr, wndyctr);
	wd_disp(D_W_EYE_X);
	wd_siz(WIND0, wndsiz, wndsiz);
	wd_cntrl(WIND0, WD_ON);
	return (0);
}

int targ_wnd(void)
{
	wd_pos(WIND1, vexX, vexY);		/* opens a window of variable size */
	wd_siz(WIND1, trgsiz, trgsiz);  /* around the saccade target defined*/
	wd_cntrl(WIND1, WD_ON);		/* by "act" object and its location*/

	return(0);
}

void rinitf()
{
	char *vexHost = "lsr-jwmvex";

	pcsConnectVex(vexHost);

	wd_disp(D_W_EYE_X);
	wd_src_pos(WIND0, WD_DIRPOS, 0, WD_DIRPOS, 0);
	wd_src_pos(WIND1, WD_DIRPOS, 0, WD_DIRPOS, 0);
	wd_src_check(WIND0, WD_SIGNAL, 0, WD_SIGNAL, 1);
	wd_src_check(WIND1, WD_SIGNAL, 2, WD_SIGNAL, 3);
	wd_cntrl(WIND0, WD_ON);
	wd_cntrl(WIND1, WD_ON);
}

VLIST state_vl[] = {
	"win_siz",		&wndsiz,	NP,	NP,	0,	ME_DEC,
	"win_x_ctr",	&wndxctr,	NP,	NP,	0,	ME_DEC,
	"win_y_ctr",	&wndyctr,	NP,	NP,	0,	ME_DEC,
	NS,
};
char hm_sv_vl[]= "";

MENU umenus[] = {
	"state_vars",	&state_vl,	NP,	NP,	0,	NP,	hm_sv_vl,
	NS,
};
USER_FUNC ufuncs[] = {
	{""},
};
RTVAR rtvars[] = {
	{""},
};

%%
id 300
restart rinitf
main {
stat ON
begin	first:
		to disabl
	disabl:
		to enable on -PSTOP & softswitch  /* paradigm enabled? */
	enable:
		code ENABLECD
		to pckstm
	pckstm:
		code 1100
		do pick_stim()
		to setrmp
	setrmp:
		do PvexLoadRamp(&nRamps, rampList, actList, nSteps, rampXs, rampYs)
		to settrg on 0 % tst_rx_new
	settrg:
		do PvexSetTriggers(&nRamps, rampList, trigList, trigs)
		to winset on 0 % tst_rx_new
	winset:
		do ctr_wnd()
		time 100
		to fpncmd
	fpncmd:
		do PvexSwitchFix(&fixOn)
		to fpon on 0 % tst_rx_new
	fpon:
		code FPONCD
		do awind(OPEN_W)
		time 1000
		to stmpos
	stmpos:
		do PvexToRampStart(&nRamps, objList, rampList)
		to stncmd on 0 % tst_rx_new
	stncmd:
		code STIMCD
		do PvexSwitchStim(&nRamps, objList, onSwitches)
		to stimon on 0 % tst_rx_new	/* acknowledge stim on signal */
	stimon:
		time 1000
		to rmpstr
	rmpstr:
		do PvexStartRamp(&nRamps, rampList, objList, cycleList)
		to aopngt on 0 % tst_rx_new	/* start the ramp */
	aopngt:
		code 2000
 		do openVexGate()
		to runrmp
	runrmp:
		to mrkrmp on FLOW_RAMP_CHANGE = vexMessage
		to endrmp on FLOW_RAMP_STOP = vexMessage
	mrkrmp:
		do dropTrigger()
		to runrmp
	endrmp:
		do closeVexGate()
		to stfcmd
	stfcmd:
		do PvexSwitchStim(&nRamps, objList, offSwitches)
		to stmoff on 0 % tst_rx_new
	stmoff:
		code STOFFCD
		time 1000
		to fpdcmd
	fpdcmd:
		do PvexDimFix()
		to fpdim on 0 % tst_rx_new
	fpdim:
		code FPDIMCD
		time 1000
		to clswin
	clswin:
		do awind(CLOSE_W);
		to fpfcmd
	fpfcmd:
		do PvexSwitchFix(&fixOff)
		to fpoff on 0 % tst_rx_new
	fpoff:
		code FPOFFCD
		time 1000
		to first
}

/*
 * Object location query state set
 */
where_set {
status ON
begin	mfirst:
		to halt
	halt:
		to qrywts on -PSTOP & softswitch
	qrywts:
		to qryvex on +GATEOPEN & vexGate 
	qryvex: 
		do PvexReportLocation(&objList[1])
		to tstmsg on 0 % tst_rx_new
	tstmsg:
		do PvexMessage(&vexMessage, &vexX, &vexY)
		to stgwnd on OBJECT_LOCATION = vexMessage
		to wait
	stgwnd:
		do targ_wnd()
		to tstmsg on 0 % tst_rx_new
		to wait
	wait:
		time 50
		to tstmsg on 0 % tst_rx_new
		to halt on FLOW_RAMP_STOP = vexMessage
		to qryvex
		to mfirst on +PSTOP & softswitch
}

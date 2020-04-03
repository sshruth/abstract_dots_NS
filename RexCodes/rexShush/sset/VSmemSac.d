#include "pcsSocket.h"
#include "GLcommand.h"
#include "lcode_tst.h"
#include "ldev_tst.h"
#define WIND0 0
#define WIND1 1
#define PHOTO_RESET_BIT Dio_id(PCDIO_DIO, 2, 0x04)
#define PHOTO_CELL_INPUT 0x1000
#define PHOTO_TRIG 1

int repeatFlag = 0;
int photoTrigger = 0;
int nObj = 1;
int object = 1;
int outer = 6;
int inner = 0;
int cntrst = 1;
float targx;
float targy;
int tarWinX;
int tarWinY;
int fixOn = OBJ_ON;
int fixOff = OBJ_OFF;
int objOn = OBJ_ON;
int objOff = OBJ_OFF;
int targoncd;
int gapcode;

int pick_stim()
{
 	int trialCode;
	int gaptime;
	
	targoncd = 0;
	gapcode = 0;
	
	trialCode = 0;
	gaptime = 100;
	
	set_times("gap", gaptime, -1);
	
	return(trialCode);
}

int wndxctr = 0;
int wndyctr = 0;
int wndsiz = 5;
int targsiz = 20;

int set_window()
{
 	wd_pos(WIND0, wndxctr, wndyctr);
	wd_siz(WIND0, wndsiz, wndsiz);

	wd_pos(WIND1, tarWinX, tarWinY);
	wd_siz(WIND1, targsiz, targsiz);

 	return(0);
}

int correctTrial()
{

 	return(0);
}

int badTrial()
{

 	return(0);
}

void rinitf(void)
{
	char *vexHost = "lsr-xpsbb";
	char *mexHost = "lsr-labmex";		/* if you are going to use Mex */

	pcsConnectVex(vexHost);		/* if you are going to use Vex */
	pcsConnectMex(mexHost);		/* if you are going to use Mex */
	
	wd_src_pos(WIND0, WD_DIRPOS, 0, WD_DIRPOS, 0);
	wd_src_check(WIND0, WD_SIGNAL, 0, WD_SIGNAL, 1);
	wd_cntrl(WIND0, WD_ON);
	return;
}

/*
 * statelist menu
 */
VLIST state_vl[] = {
	NS,
};
char hm_sv_vl[]= "";

/*
 * list of user menus
 */
MENU umenus[] = {
	"state_vars",	&state_vl,	NP,	NP,	0,	NP,	hm_sv_vl,
	NS,
};

/*
 *
 */
USER_FUNC ufuncs[] = {
	{""},
};

/*
 * User-supplied real-time variables
 */
RTVAR rtvars[] = {
	  {"", 0},
};

%%
id 100
restart rinitf
main_set {
status ON
begin	first:
		to disabl
	disabl:
		to enable on -PSTOP & softswitch  /* is bar enabled */
	enable:
		code ENABLECD
	    to pckstm
	pckstm:
   		do pick_stim()
		to drwstm
   	drwstm:
   		do PvexDrawAnnulus(&nObj, &object, &outer, &inner, &cntrst)
		to posstm on 0 % tst_rx_new
   	posstm:
   		do PvexStimLocation(&nObj, &object, &targx, &targy)
		to setWnd on 0 % tst_rx_new
   	setWnd:
   		do set_window()
		to fpncmd
   	fpncmd:
   		do PvexSwitchFix(&fixOn)
		to fpon on +PHOTO_TRIG & photoTrigger
   	fpon:
   		code FPONCD
		do awind(OPEN_W)
		time 1000
		to fixtim on -WD0_XY & eyeflag
		to error
   	fixtim:
   		time 500
		rand 500
		to error on +WD0_XY & eyeflag
  		to tgncmd
   	tgncmd:
   		do PvexSwitchStim(&nObj, &object, &objOn)
		to targon on +PHOTO_TRIG & photoTrigger
   	targon:
   		code &targoncd
		time 100
		to error on +WD0_XY & eyeflag
		to tgfcmd
   	tgfcmd:
   		do PvexSwitchStim(&nObj, &object, &objOff)
		to targoff on +PHOTO_TRIG & photoTrigger
   	targoff:
   		code TARGOFFCD
		to gap
   	gap:
   	   	code &gapcode
		to error on +WD0_XY & eyeflag
		to fpfcmd
  	fpfcmd:
   		do PvexSwitchFix(&fixOff)
		to fpfoff on +PHOTO_TRIG & photoTrigger
  	fpfoff:
  		code FPOFFCD
		time 50
		to waitsac on +WD0_XY & eyeflag
		to error
  	waitsac:
  		time 100
		to fixtarg on -WD1_XY & eyeflag
		to error
   	fixtarg:
   		time 100
		to error on +WD1_XY & eyeflag
		to correct
   	correct:
   		code REWCD
		do correctTrial()
		to rewon
	rewon:
		do dio_on(REW)
		time 50
		to rewoff
	rewoff:
		do dio_off(REW)
		to clswin
	clswin:
		do awind(CLOSE_W)
		to last
	error:
		code ERRCD
		do badTrial()
	  	time 1000
		to cnclwn
	cnclwn:
		do awind(CANCEL_W)
		to last
	last:
		to first
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

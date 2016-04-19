/*
 * Rex Vex ramp test paradigm.
 *
 */
#include <stdlib.h>
#include "ramp.h"
#include "pcsSocket.h"
#include "GLcommand.h"
#include "ldev_tst.h"
#include "lcode_tst.h"

#define WIND0	    0
#define WIND1	    1
#define EYEALL	    ( (WD0_XY << (WIND0 * 2)) | (WD0_XY << (WIND1 * 2)) )
#define EYEH_SIG    0	    /* signal numbers for eyes */
#define EYEV_SIG    1
#define OEYEH_SIG   2
#define OEYEV_SIG   3
#define GATEOPEN	1
#define GATECLOSE	0

typedef struct {
	int	len;
	int	ang;
	int	vel;
	int	xoff;
	int yoff;
	int	xwind;
	int ywind;
	int	oxwind;
	int oywind;
	int	eyehoff;
	int eyevoff;
	int oeyehoff;
	int oeyevoff;
	int	type;
	int	ecode;
} RA_LIST;

#define E_D0	2000		/* ramp direction series */
#define E_D45	2001
#define E_D90	2002
#define E_D135	2003
#define E_D180	2004
#define E_D225	2005
#define E_D270	2006
#define E_D315	2007

/*
 *	Direction series for tracking.
 */
RA_LIST list0[] = { 
	200, 315,  5, 0, 0, 50, 50, 40, 40, 40, 40, 0,  0, RA_CENPT, E_D315,
	200,  90, 10, 0, 0, 50, 50, 40, 40, 20, 20, 0,  0, RA_CENPT, E_D90,
	200, 225, 15, 0, 0, 50, 50, 40, 40, 10, 10, 0,  0, RA_CENPT, E_D225,
	200,   0, 20, 0, 0, 50, 50, 40, 40, 30, 30, 0,  0, RA_CENPT, E_D0,
	200, 180, 25, 0, 0, 50, 50, 40, 40, 40, 40, 0,  0, RA_CENPT, E_D180,
	200,  45, 30, 0, 0, 50, 50, 40, 40, 20, 20, 0,  0, RA_CENPT, E_D45,
	200, 270, 35, 0, 0, 50, 50, 40, 40, 10, 10, 0,  0, RA_CENPT, E_D270,
	200, 135, 40, 0, 0, 50, 50, 40, 40, 30, 30, 0,  0, RA_CENPT, E_D135,
	200,   0,  5, 0, 0, 50, 50, 40, 40, 20, 40, 0,  0, RA_CENPT, E_D0,
	200, 315, 10, 0, 0, 50, 50, 40, 40,  0, 0, 40, 40, RA_CENPT, E_D315,
	200,  45, 15, 0, 0, 50, 50, 40, 40,  0, 0, 20, 20, RA_CENPT, E_D45,
	200, 225, 20, 0, 0, 50, 50, 40, 40,  0, 0, 10, 10, RA_CENPT, E_D225,
	200,  90, 25, 0, 0, 50, 50, 40, 40,  0, 0, 30, 30, RA_CENPT, E_D90,
	200, 180, 30, 0, 0, 50, 50, 40, 40,  0, 0, 40, 40, RA_CENPT, E_D180,
	200, 135, 35, 0, 0, 50, 50, 40, 40,  0, 0, 20, 20, RA_CENPT, E_D135,
	200, 270, 40, 0, 0, 50, 50, 40, 40,  0, 0, 10, 10, RA_CENPT, E_D270,
	 -1,  -1, -1,-1,-1,	-1, -1, -1, -1, -1,-1, -1, -1,        0,     -1,
};

static RA_LIST *rlp= &list0[0];
static int rxwind= 0, rywind= 0;
static int roxwind= 0, roywind= 0;
int fxwd= 100, fywd= 100;
int foxwd= 50, foywd= 50;
int rampCode;
static RL rl_sav;

int fixOn = OBJ_ON;
int fixOff = OBJ_OFF;
int vexCode;
int vexGate;
int vexX;
int vexY;

int nRamps = 1;
int ramps = 1;
int objs = 1;
int lens;
int dirs;
int vels;
int xOffs;
int yOffs;
int types;
int acts = ON_AFTER_RAMP;
int cycles = RA_ONCE;
int onSwitch = OBJ_ON;
int offSwitch = OBJ_OFF;

int nTrials;
int trialsRemaining = 16;
int nBlocks = 0;
int correctTrials = 0;
int errorTrials = 0;
int totalTrials = 0;
int percentCorrect = 0;

int nextramp()
{
	int i;

	if(rlp->len == -1) {
		rlp= &list0[0];
		nBlocks++;
		nTrials = 0;
		trialsRemaining = 16;
	}
	else {
		nTrials++;
		totalTrials++;
		trialsRemaining--;
	}

	lens = rlp->len;
	dirs = (rlp->ang + (i * 180) % 360);
	vels = rlp->vel;
	xOffs = rlp->xoff;
	yOffs = rlp->yoff;
	types = rlp->type;

	rxwind= rlp->xwind;
	rywind= rlp->ywind;
	roxwind= rlp->oxwind;
	roywind= rlp->oywind;

	off_eye(rlp->eyehoff, rlp->eyevoff);
	off_oeye(rlp->oeyehoff, rlp->oeyevoff);

	rampCode = rlp->ecode;
	rlp++;

	return(0);
}

int targ_wnd(void)
{
	wd_pos(WIND1, vexX, vexY);		/* opens a window of variable size */
	wd_siz(WIND1, foxwd, foywd);  /* around the saccade target defined*/
	wd_cntrl(WIND1, WD_ON);		/* by "act" object and its location*/

	return(0);
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
	errorTrials++;
	if(totalTrials) percentCorrect = 100 * correctTrials / totalTrials;
	return(0);
}

void
rinitf(void)
{
	char *vexHost = "lsr-xpsbb";
	char *mexHost = "lsr-labmex";		/* if you are going to use Mex */

	pcsConnectVex(vexHost);
	pcsConnectMex(mexHost);		/* if you are going to use Mex */

    wd_disp(D_W_ALLCUR & ~D_W_JOY);	    /* all cursors but joystick */
	wd_src_pos(WIND0, WD_DIRPOS, 0, WD_DIRPOS, 0);
	wd_src_pos(WIND1, WD_DIRPOS, 0, WD_DIRPOS, 0);
    wd_src_check(WIND0, WD_SIGNAL, EYEH_SIG, WD_SIGNAL, EYEV_SIG);
    wd_src_check(WIND1, WD_SIGNAL, OEYEH_SIG, WD_SIGNAL, OEYEV_SIG);
    wd_siz(WIND0, fxwd, fywd);
    wd_siz(WIND1, foxwd, foywd);
    wd_pos(WIND0, 0, 0);
    wd_pos(WIND1, 0, 0);
    wd_cntrl(WIND0, WD_ON);
    wd_cntrl(WIND1, WD_ON);
    rlp= &list0[0];
    wd_test(1);				/* put windows in test mode */
    ustatus("REX test paradigm");
}

int ral_agf(int call_cnt, MENU *mp, char *astr)
{
	if(call_cnt >= 16) {
		/*
		 * Done.  Return null to terminate writing of root for
		 * this menu.
		 */
		*astr= '\0';
	}
	else itoa_RL(call_cnt, 'd', astr, &astr[P_ISLEN]);
	return(0);
}

int ral_maf(int flag, MENU *mp, char *astr, ME_RECUR *rp)
{
	RA_LIST *rlp;
	int rampnum;

	if(*astr == '\0') rampnum = 0;
	else rampnum = atoi(astr);

	if((rampnum < 0) || (rampnum >= 16)) return(-1);
	rlp = &list0[rampnum];
	mp->me_basep = (unsign)rlp;
	return(0);
}

/*
 * Declaration of statelist menu.
 */
VLIST state_vl[] = {
"fix_xwind",		&fxwd, NP, NP, 0, ME_DEC,
"fix_ywind",		&fywd, NP, NP, 0, ME_DEC,
"fix_oxwind",		&foxwd, NP, NP, 0, ME_DEC,
"fix_oywind",		&foywd, NP, NP, 0, ME_DEC,
NS,
};

char hm_sv_vl[]= "";

/*
 * Declaration of ramp list menu
 */
VLIST ramp_vl[] = {
"length",	&((RA_LIST *)NP)->len, 		NP, NP, ME_GB, ME_DEC,
"angle",	&((RA_LIST *)NP)->ang, 		NP, NP, ME_GB, ME_DEC,
"velocity",	&((RA_LIST *)NP)->vel, 		NP, NP, ME_GB, ME_DEC,
"xoff",		&((RA_LIST *)NP)->xoff,		NP, NP, ME_GB, ME_DEC,
"yoff",		&((RA_LIST *)NP)->yoff,		NP, NP, ME_GB, ME_DEC,
"type",		&((RA_LIST *)NP)->type, 	NP, NP, ME_GB, ME_DEC,
"ecode",	&((RA_LIST *)NP)->ecode, 	NP, NP, ME_GB, ME_DEC,
NS,
};
char hm_ramp_vl[] = "";

/*
 * Declaration of eye windows menu
 */
VLIST eyewnds_vl[] = {
"xwind",	&((RA_LIST *)NP)->xwind,	NP, NP, ME_GB, ME_DEC,
"ywind",	&((RA_LIST *)NP)->ywind,	NP, NP, ME_GB, ME_DEC,
"oxwind",	&((RA_LIST *)NP)->oxwind,	NP, NP, ME_GB, ME_DEC,
"oywind",	&((RA_LIST *)NP)->oywind,	NP, NP, ME_GB, ME_DEC,
NS,
};
char hm_eyewnds_vl[] = "";

/*
 * Declaration of offsets menu
 */
VLIST eyeoffs_vl[] = {
"eyehoff",	&((RA_LIST *)NP)->eyehoff,	NP, NP, ME_GB, ME_DEC,
"eyevoff",	&((RA_LIST *)NP)->eyevoff,	NP, NP, ME_GB, ME_DEC,
"oeyehoff",	&((RA_LIST *)NP)->oeyehoff,	NP, NP, ME_GB, ME_DEC,
"oeyevoff",	&((RA_LIST *)NP)->oeyevoff,	NP, NP, ME_GB, ME_DEC,
NS,
};
char hm_eyeoffs_vl[] = "";
/*
 * User-supplied menu table.
 */
MENU umenus[] = {
"state_vars",	&state_vl, NP, NP, 0, NP, hm_sv_vl,
"separator",	NP,		   NP, NP, 0, NP, 0,
"ramp_list",	&ramp_vl,  NP, ral_maf, ME_BEF, ral_agf, hm_ramp_vl,
"separator",	NP,		   NP, NP, 0, NP, 0,
"eye_winds",	&eyewnds_vl, NP, ral_maf, ME_BEF, ral_agf, hm_eyewnds_vl,
"eye_offsets",	&eyeoffs_vl, NP, ral_maf, ME_BEF, ral_agf, hm_eyeoffs_vl,
NS,
};

/* Declaration of ramp reset function */
void f_rampReset(void)
{
	int i;

	i = 0;
	while(list0[i].len > 0) {
		list0[i].len = 200;
		list0[i].xoff = 0;
		list0[i].yoff = 0;
		i++;
	}

	list0[0].ang = 315;
	list0[1].ang = 90;
	list0[2].ang = 225;
	list0[3].ang = 0;
	list0[4].ang = 180;
	list0[5].ang = 45;
	list0[6].ang = 270;
	list0[7].ang = 135;
	list0[8].ang = 0;
	list0[9].ang = 315;
	list0[10].ang = 45;
	list0[11].ang = 225;
	list0[12].ang = 90;
	list0[13].ang = 180;
	list0[14].ang = 135;
	list0[15].ang = 270;
	return;
}

/* Declaration of ramp length function */
void f_rampLength(int length)
{
	int i;

	for(i = 0; i < 16; i++) list0[i].len = length;
	return;
}

/* Declaration of ramp angle function */
void f_rampAngle(int angle)
{
	int i;

	for(i = 0; i < 16; i++) list0[i].ang += angle;

	return;
}

/* Declaration of ramp offset function */
void f_rampOffset(int Xoff, int Yoff)
{
	int i;

	for(i = 0; i < 16; i++) {
		list0[i].xoff = Xoff;
		list0[i].yoff = Yoff;
	}
	return;
}

/*
 * User-supplied function table.
 */
USER_FUNC ufuncs[] = {
	{"reset",	&f_rampReset, "void"},
	{"length",	&f_rampLength, "%d"},
	{"angle",	&f_rampAngle, "%d"},
	{"offset",	&f_rampOffset, "%d %d"},
	{""},
};

/*
 * User-supplied real-time variable table
 */
RTVAR rtvars[] = {
{"number of trials",		&nTrials},
{"trials remaining", 		&trialsRemaining},
{"blocks completed",		&nBlocks},
{"total trials",		&totalTrials},
{"number correct",		&correctTrials},
{"number wrong",		&errorTrials},
{"percent correct",		&percentCorrect},
{"", 0},
};

%%
id 700
restart rinitf
main_set {
status ON
begin	first:
		to disabl
	disabl:
		to enable on -PSTOP & softswitch
	enable:
		code ENABLECD
		to pckstm;
	pckstm:
		do nextramp()		/* select next ramp */
		to strmp
	strmp:
		do PvexNewRamp(&nRamps, &ramps, &lens, &dirs, &vels, &xOffs, &yOffs, &types, &acts)
		rl 15
		to openw on 0 % tst_rx_new
	openw:
		code &rampCode
		do awind(OPEN_W)
		to fpncmd
	fpncmd:
		code FPONCD
		do PvexSwitchFix(&fixOn)
		rl 5
		to eyeon on 0 % tst_rx_new
	eyeon:
		rl 40
		time 250
		rand 250
		to error on +EYEALL & eyeflag	/* start checking for eye */
		to stmpos	
    stmpos:
		do PvexToRampStart(&nRamps, &objs, &ramps)
		to opngat on 0 % tst_rx_new
	opngat:
		do PsetFlag(&vexGate, 1)
		to stncmd
	stncmd:
		do PvexSwitchStim(&nRamps, &objs, &onSwitch)
		to stimon on 0 % tst_rx_new
	stimon:
		code STIMCD
		time 100
		rl 50
		to rmpstr
	rmpstr:
		do PvexStartRamp(&nRamps, &ramps, &objs, &cycles)
		to runrmp on 0 % tst_rx_new
    runrmp:
		to error on +EYEALL & eyeflag
		to endrmp on FLOW_RAMP_STOP = vexCode
    endrmp:
		time 500
		do PsetFlag(&vexGate, 0)
		to stfcmd
    stfcmd:
		do PvexSwitchStim(&nRamps, &objs, &offSwitch)
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
		do awind(CLOSE_W)
 	    to fpfcmd
    fpfcmd:
		do PvexSwitchFix(&fixOff)
		to fpoff on 0 % tst_rx_new
	fpoff:
		code FPOFFCD
		time 1000
		rl 35
		to stoff
	stoff:
		code STOFFCD
		to closew
	closew:
		do awind(CLOSE_W)
		to rewon
	rewon:
		do dio_on(BEEP)
		time 35
		to rewoff
	rewoff:
		do dio_off(BEEP)
		to correct
	correct:
		do correctTrial()
		to first
	error:				/* abort trial */
		do awind(CANCEL_W)
		to reset
	reset:
		code ERR1CD
		do reset_s(-1)
		to wrong
	wrong:
		do errorTrial()
		to first
abort	list:
		rewoff closew
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
		do PvexReportLocation(&objs)
		to tstmsg on 0 % tst_rx_new
	tstmsg:
		do PvexMessage(&vexCode, &vexX, &vexY)
		to stgwnd on OBJECT_LOCATION = vexCode
		to wait
	stgwnd:
		do targ_wnd()
		to tstmsg on 0 % tst_rx_new
		to wait
	wait:
		time 50
		to tstmsg on 0 % tst_rx_new
		to halt on FLOW_RAMP_STOP = vexCode
		to qryvex
		to mfirst on +PSTOP & softswitch
}

#include "pcsSocket.h"

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
#define OBJ_ON		1
#define OBJ_OFF		0

struct location {
	float x;
	float y;
};

/* locations of flow fiels */
struct location loc_list[] = {
50.0,	50.0,				/* 5 degrees right and up */
50.0,	-50.0,				/* 5 degrees right and down */
-50.0,	-50.0,				/* 5 degrees left and down */
-50.0,	50.0				/* 5 degrees left and up */
};

/* if the screen is 573 millimeters from the subject
 * then the near and far planes of the flow field
 * will be 100 and 2000 millimeters from the subject, respectively
 */
int fixOn = OBJ_ON;
int fixOff = OBJ_OFF;
int flow_near = 1;			/* near plane 1 millimeter from subject */
int flow_far = 1;			/* far plane 1 millimeters from subject */
int flow_rad = 100;		/* flow field 10 degrees in radius */
float flow_height = 1.00;		/* flow field height 100% of radius */
float flow_width = 1.00;		/* flow field width 100% of radius */
int flow_num = 50;			/* 2.5% of flow field covered by checks */
int flow_siz = 4;			/* check size 4 pixels */
int mask_width = 0;			/* mask 5 degrees wide */
int mask_height = 0;		/* mask 5 degree high */
float mask_x = 0.0;			/* mask centered in flow field */
float mask_y = 0.0;
int flow_z = 0;				/* angles of flow in Z plane */
int flow_life = 0;			/* life spans of checks */
int flow_roll = 0;			/* no roll */
int flow_pitch = 0;			/* no pitch */
int flow_yaw = 0;			/* no yaw */
int flow_dirLim = 180;		/* limit of random directions */
int flow_spdLim = 0;		/* limit of random speeds */
int flow_chr = 0;			/* percentage of coherent checks */
int flow_xy[] = {0, 45, 90, 135};	/* angles of flow in X-Y plane */
int flow_vel[] = {5, 10, 15, 20};	/* flow velocities degrees/second */
int curr_flow = -1;

int nFlows = 1;
int flowObjects = 1;
float objXLocs;
float objYLocs;
int flowXys;
int flowVels;
int onSwitches;
int offSwitches;

int pick_stim()
{
	int i;

	if(++curr_flow > 3) curr_flow = 0;

	/* initialize the object locations */
	objXLocs = loc_list[curr_flow].x;
	objYLocs = loc_list[curr_flow].y;

	/* initialize the flow field translation matrices */
	flowXys = flow_xy[curr_flow];
	flowVels = flow_vel[curr_flow];

	/* initialize the on switches */
	onSwitches = OBJ_ON;

	/* initialize the off switches */
	offSwitches = OBJ_OFF;

	return(0);
}

int wndxctr = 0;
int wndyctr = 0;
int wndsiz = 5;
int ctr_wnd()
{
	wd_pos(WIND0, wndxctr, wndyctr);
	wd_disp(D_W_EYE_X);
	wd_siz(WIND0, wndsiz, wndsiz);
	wd_cntrl(WIND0, WD_ON);
	return (0);
}

void rinitf()
{
	char *vexHost = "lsr-jwmvex";
	char *mexHost = "lsr-jwmmex";		/* if you are going to use Mex */

	pcsConnectVex(vexHost);
	pcsConnectMex(mexHost);		/* if you are going to use Mex */
	
	wd_disp(D_W_EYE_X);
	wd_src_pos(WIND0, WD_DIRPOS, 0, WD_DIRPOS, 0);
	wd_src_pos(WIND1, WD_DIRPOS, 0, WD_DIRPOS, 0);
	wd_src_check(WIND0, WD_SIGNAL, 0, WD_SIGNAL, 1);
	wd_src_check(WIND1, WD_SIGNAL, 2, WD_SIGNAL, 3);
	wd_cntrl(WIND0, WD_ON);
}

VLIST state_vl[] = {
	"win_siz",			&wndsiz,		NP,	NP,	0, ME_DEC,
	"win_x_ctr",		&wndxctr,		NP,	NP,	0, ME_DEC,
	"win_y_ctr",		&wndyctr,		NP,	NP,	0, ME_DEC,
	"dir_range",		&flow_dirLim, 	NP, NP, 0, ME_DEC,
	"spd_range",		&flow_spdLim,	NP, NP, 0, ME_DEC,
	"check_time",		&flow_life, 	NP, NP, 0, ME_DEC,
	"percent_coherent",	&flow_chr,		NP, NP, 0, ME_DEC,
	NS,
};
char hm_sv_vl[]= "";

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
		to pickstm
	pickstm:
		do pick_stim()
		to setflw
	setflw:
		do PvexDrawEllipticalFlowField(&nFlows, &flowObjects, &flow_rad, &flow_width, &flow_height, &flow_near, &flow_far, &flow_num, &flow_siz)
		to setDirLim on 0 % tst_rx_new
	setDirLim:
		do PvexFlowDirectionRange(&nFlows, &flowObjects, &flow_dirLim);
		to setSpdLim on 0 % tst_rx_new
	setSpdLim:
		do PvexFlowSpeedRange(&nFlows, &flowObjects, &flow_spdLim);
		to settran on 0 % tst_rx_new
	settran:
		do PvexNewFlow(&nFlows, &flowObjects, &flowXys, &flow_z, &flowVels, &flow_roll, &flow_pitch, &flow_yaw, &flow_life, &flow_chr)
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
		time 1000
 		to stmpos
	stmpos:
		do PvexStimLocation(&nFlows, &flowObjects, &objXLocs, &objYLocs)
 		to stncmd on 0 % tst_rx_new
	stncmd:
		code STIMCD
		do PvexSwitchStim(&nFlows, &flowObjects, &onSwitches)
		to stimon on 0 % tst_rx_new	/* acknowledge stim on signal */
	stimon:
		time 1000
		to flwstr
	flwstr:
		do PvexStartFlow(&nFlows, &flowObjects);
		to runflw on 0 % tst_rx_new	/* start the ramp */
	runflw:
		time 2000
		to flfcmd
	flfcmd:
		do PvexStopFlowRamp()
		to waitoff on 0 % tst_rx_new
	waitoff:
		time 1000
		to stfcmd
	stfcmd:
		do PvexSwitchStim(&nFlows, &flowObjects, &offSwitches)
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


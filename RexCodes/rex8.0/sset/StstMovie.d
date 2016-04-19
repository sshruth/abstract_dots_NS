#include "pcsSocket.h"
#include "GLcommand.h"
#include "lcode_tst.h"
#include "ldev_tst.h"

#define WIND0 0

int fixOn = OBJ_ON;
int fixOff = OBJ_OFF;
int nClips = 1;
int frstObj = 1;
int nObjs = 20;
int nFrames = 20;
int interval = 10;
int cycles = 2;
int xloc = 100;
int yloc = 100;

float xloc_list[20];
float yloc_list[20];
int objList[20];

int movie_setup()
{
	int i;

	nObjs = nFrames;

	for(i = 0; i < nObjs; ++i) {
		objList[i] = frstObj + i;
		xloc_list[i] = (float)xloc;
		yloc_list[i] = (float)yloc;
	}

	return(0);
}

void rinitf(void)
{
	char *vexHost = "lsr-jwmvex";		/* if you are going to use Vex */
	char *mexHost = "lsr-jwmmex2";		/* if you are going to use Mex */

	pcsConnectVex(vexHost);		/* if you are going to use Vex */
	pcsConnectMex(mexHost);		/* if you are going to use Mex */
	
	wd_src_pos(WIND0, WD_DIRPOS, 0, WD_DIRPOS, 0);
	wd_src_check(WIND0, WD_SIGNAL, 0, WD_SIGNAL, 1);
	wd_cntrl(WIND0, WD_ON);
}

/*
 * statelist menu
 */
VLIST state_vl[] = {
	{"X_location",		&xloc, 		NP, NP, 0, ME_DEC},
	{"Y_location",		&yloc, 		NP, NP, 0, ME_DEC},
	{"num_frames",		&nFrames, 	NP, NP, 0, ME_DEC},
	{"interval",		&interval, 	NP, NP, 0, ME_DEC},
	{"cycles",			&cycles, 	NP, NP, 0, ME_DEC},
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
	    to pckloc
	pckloc:
		do movie_setup()
		to setloc
	setloc:
		do PvexStimLocation(&nObjs, objList, xloc_list, yloc_list)
		to awnopn on 0 % tst_rx_new
	awnopn:
		do awind(OPEN_W)
		to fpncmd
	fpncmd:
		do PvexSwitchFix(&fixOn)
		to fpon on 0 % tst_rx_new
	fpon:
		code FPONCD
		time 500
		to mvoncmd
	mvoncmd:
		do PvexShowMovieClip(&nClips, &frstObj, &nFrames, &interval, &cycles);
		to mvon on 0 % tst_rx_new
	mvon:
		code 1500
		to mvoff on 0 % tst_rx_new
	mvoff:
		code 1600
		time 500
		to fpfcmd
	fpfcmd:
		do PvexSwitchFix(&fixOff)
		to fpoff on 0 %tst_rx_new
	fpoff:
		code FPOFFCD
		to last	.
	last:
		do awind(CLOSE_W)
		time 250
		to first
}

#include <stdio.h>
#include <sys/types.h>
#include "../hdr/sys.h"
#include "../hdr/cnf.h"
#include "../hdr/proc.h"
#include "../hdr/cdsp.h"
#include "../hdr/idsp.h"
#include "../hdr/buf.h"
#include "../hdr/menu.h"
#include "../hdr/state.h"
#include "../hdr/ecode.h"
#include "../hdr/device.h"
#include "../hdr/int.h"
#include "StstSine.r.h"
#include "../hdr/GLcommand.h"
#include "lcode_tst.h"

#define WIND0 0
#define WIND1 1

int nObjects = 1;
int objList = 1;

float targXlist = 50.0;
float targYlist =  0.0;
int orientations = 0;
float spatialFrequencies = 1.0;
float temporalFrequencies = 1.0;
int counterPhases = 0;
float redContrasts = 50.0;
float greenContrasts = 50.0;
float blueContrasts = 50.0;
int patchSizes = 100;
int nCycles = 0;

int fixon = OBJ_ON;
int offList = OBJ_OFF;
int onList = OBJ_ON;
int fixoff = OBJ_OFF;

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


// BEGIN GENERATED CODE

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

STATE sfirst = {  // 150 0
	{"first"},0,0,
	0,0,
	{0,0,0,0,0,0,0,0,0,0,0},
	0,0,
	{
		{0,0,&sdisable,TIME,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
	},
	0,127,128,150,0
};
STATE sdisable = {  // 150 35
	{"disable"},0,0,
	0,0,
	{0,0,0,0,0,0,0,0,0,0,0},
	0,0,
	{
		{0,0,&senable,BITOFF,&softswitch,PSTOP},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
	},
	0,127,128,150,35
};
STATE senable = {  // 150 70
	{"enable"},ENABLECD,0,
	0,0,
	{0,0,0,0,0,0,0,0,0,0,0},
	0,0,
	{
		{0,0,&stargLoc,TIME,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
	},
	0,127,128,150,70
};
STATE stargLoc = {  // 150 105
	{"targLoc"},0,0,
	0,0,
	{PvexStimLocation,&nObjects,&objList,&targXlist,&targYlist,0,0,0,0,0,0},
	0,0,
	{
		{0,0,&ssetGrating,FUNC,&tst_rx_new,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
	},
	0,127,128,150,105
};
STATE ssetGrating = {  // 150 140
	{"setGrating"},0,0,
	0,0,
	{PvexDrawSineGrating,&nObjects,&objList,&orientations,&spatialFrequencies,&temporalFrequencies,&counterPhases,&redContrasts,&greenContrasts,&blueContrasts,&patchSizes},
	0,0,
	{
		{0,0,&swinset,FUNC,&tst_rx_new,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
	},
	0,127,128,150,140
};
STATE swinset = {  // 150 175
	{"winset"},0,0,
	0,0,
	{cnt_wnd,0,0,0,0,0,0,0,0,0,0},
	0,0,
	{
		{0,0,&sopenwin,TIME,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
	},
	0,127,128,150,175
};
STATE sfponcmd = {  // 150 245
	{"fponcmd"},0,0,
	0,0,
	{PvexSwitchFix,&fixon,0,0,0,0,0,0,0,0,0},
	0,0,
	{
		{0,0,&sfpon,FUNC,&tst_rx_new,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
	},
	0,127,128,150,245
};
STATE sfpon = {  // 150 280
	{"fpon"},FPONCD,0,
	0,0,
	{0,0,0,0,0,0,0,0,0,0,0},
	1000,0,
	{
		{0,0,&sstmoncmd,TIME,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
	},
	0,127,128,150,280
};
STATE sstmoncmd = {  // 150 315
	{"stmoncmd"},0,0,
	0,0,
	{PvexSwitchStim,&nObjects,&objList,&onList,0,0,0,0,0,0,0},
	0,0,
	{
		{0,0,&sstimon,FUNC,&tst_rx_new,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
	},
	0,127,128,150,315
};
STATE sstimon = {  // 150 350
	{"stimon"},STIMCD,0,
	0,0,
	{0,0,0,0,0,0,0,0,0,0,0},
	1000,0,
	{
		{0,0,&schooseDrift,TIME,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
	},
	0,127,128,150,350
};
STATE schooseDrift = {  // 150 385
	{"chooseDrift"},0,0,
	0,0,
	{0,0,0,0,0,0,0,0,0,0,0},
	0,0,
	{
		{0,0,&sstartDrift,EQUAL,&nCycles,0},
		{0,0,&stimeDrift,LESSOR,&nCycles,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
	},
	0,127,128,150,385
};
STATE sstartDrift = {  // 0 420
	{"startDrift"},0,0,
	0,0,
	{PvexStartSine,&nObjects,&objList,0,0,0,0,0,0,0,0},
	0,0,
	{
		{0,0,&sdriftRunning,FUNC,&tst_rx_new,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
	},
	0,127,128,0,420
};
STATE sdriftRunning = {  // 0 455
	{"driftRunning"},2000,0,
	0,0,
	{0,0,0,0,0,0,0,0,0,0,0},
	1000,0,
	{
		{0,0,&sstopDrift,TIME,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
	},
	0,127,128,0,455
};
STATE sstopDrift = {  // 0 490
	{"stopDrift"},0,0,
	0,0,
	{PvexStopSine,0,0,0,0,0,0,0,0,0,0},
	0,0,
	{
		{0,0,&sdriftOff,FUNC,&tst_rx_new,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
	},
	0,127,128,0,490
};
STATE sdriftOff = {  // 150 455
	{"driftOff"},0,0,
	0,0,
	{0,0,0,0,0,0,0,0,0,0,0},
	1000,0,
	{
		{0,0,&sstmoffcmd,TIME,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
	},
	0,127,128,150,455
};
STATE sstmoffcmd = {  // 150 490
	{"stmoffcmd"},0,0,
	0,0,
	{PvexSwitchStim,&nObjects,&objList,&offList,0,0,0,0,0,0,0},
	0,0,
	{
		{0,0,&sstimoff,FUNC,&tst_rx_new,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
	},
	0,127,128,150,490
};
STATE sopenwin = {  // 150 209
	{"openwin"},0,0,
	0,0,
	{awind,OPEN_W,0,0,0,0,0,0,0,0,0},
	0,0,
	{
		{0,0,&sfponcmd,TIME,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
	},
	0,127,128,150,209
};
STATE sstimoff = {  // 150 525
	{"stimoff"},1101,0,
	0,0,
	{0,0,0,0,0,0,0,0,0,0,0},
	1000,0,
	{
		{0,0,&sfpoffcmd,TIME,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
	},
	0,127,128,150,525
};
STATE sfpoffcmd = {  // 150 560
	{"fpoffcmd"},0,0,
	0,0,
	{PvexSwitchFix,&fixoff,0,0,0,0,0,0,0,0,0},
	0,0,
	{
		{0,0,&scloseWin,FUNC,&tst_rx_new,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
	},
	0,127,128,150,560
};
STATE scloseWin = {  // 150 595
	{"closeWin"},FPOFFCD,0,
	0,0,
	{awind,CLOSE_W,0,0,0,0,0,0,0,0,0},
	500,0,
	{
		{0,0,&sfirst,TIME,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
	},
	0,127,128,150,595
};
STATE stimeDrift = {  // 300 420
	{"timeDrift"},0,0,
	0,0,
	{PvexTimeGrating,&nObjects,&objList,&nCycles,0,0,0,0,0,0,0},
	0,0,
	{
		{0,0,&stimingDrift,FUNC,&tst_rx_new,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
	},
	0,127,128,300,420
};
STATE stimingDrift = {  // 300 455
	{"timingDrift"},3000,0,
	0,0,
	{0,0,0,0,0,0,0,0,0,0,0},
	0,0,
	{
		{0,0,&sdriftOff,FUNC,&tst_rx_new,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
	},
	0,127,128,300,455
};


STATE *snames[] = {
	&sfirst, 
	&sdisable, 
	&senable, 
	&stargLoc, 
	&ssetGrating, 
	&swinset, 
	&sfponcmd, 
	&sfpon, 
	&sstmoncmd, 
	&sstimon, 
	&schooseDrift, 
	&sstartDrift, 
	&sdriftRunning, 
	&sstopDrift, 
	&sdriftOff, 
	&sstmoffcmd, 
	&sopenwin, 
	&sstimoff, 
	&sfpoffcmd, 
	&scloseWin, 
	&stimeDrift, 
	&stimingDrift, 
0};


STATE *aborta[] = {
	0};

int sf_init();

STATE sps_state;

AWAKE init_state[] = {
	{0,ON,ON,&sps_state,&sps_state,0,(STATE *)&init_state[1],0, &sps_state.escape},
	{0,0,0,0,0,0,0,0,&sps_state.escape[1]}
};
STATE sps_state = {
	{"spec"},1,0,1,0,
	{sf_init,100,0,0,0,0,0,0,0,0,0},
	0,0,
	{
		{0,init_state,&sps_state,TIME,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0}
	}
};
AWAKE nowstate[] = {
	{0,ON,ON,&sfirst,&sfirst,aborta,0,0,0}, // main_set 0 0 442 682 1
	{0,0,0,0,0,0,0,0,0}
};

void rinitf();
int (*init_list[])() = {
	rinitf,
	0
};
/**********************************
APPLICATION PROPERTIES
PARADIGM_NUMBER 100
BASE_WINDOW 1090 1075
WIDGET_DIALOG 930 3
RESOURCE_DIALOG 582 592 351 365
CHAIN main_set
CHAIN_DONE
MENU state_vars
MENU_DONE
APP_DONE
 **********************************/

#include "pcsSocket.h"
#include "GLcommand.h"
#include "localEcodes.h"
#include "ldev_tst.h"
#define WIND0 0
#define WIND1 1

/*
 * Subroutine that sets the active object in VEX
 */
int vxCode;
int vxX;
int vxY;
int actObj = 1;
int fixOn = OBJ_ON;
int fixOff = OBJ_OFF;
float fixPtX = 0.0;
float fixPtY = 0.0;
int fponclr = 0;
int fpdimclr = 96;
int fpsize = 4;
int nTarg = 1;
long fixsiz = 10;
long tarsiz = 20;
long fpx = 0;
long fpy = 0;
long wind0 = WIND0;
long wind1 = WIND1;

/*
 * Subroutine that moves the fixation window to the fixation location
 */
int ctr_wnd(void)
{
	wd_pos(WIND0, fpx, fpy);
	wd_siz(WIND0, fixsiz, fixsiz);
	wd_siz(WIND1, tarsiz, tarsiz);
	wd_cntrl(WIND0, WD_ON);
	wd_cntrl(WIND1, WD_ON);

	return(0);
}

void rinitf(void)
{
	char *vexHost = "lsr-jwmvex";

	pcsConnectVex(vexHost);
	
	wd_src_pos(WIND0, WD_DIRPOS, 0, WD_DIRPOS, 0);
	wd_src_pos(WIND1, WD_DIRPOS, 0, WD_DIRPOS, 0);
	wd_src_check(WIND0, WD_SIGNAL, 0, WD_SIGNAL, 1);
	wd_src_check(WIND1, WD_SIGNAL, 0, WD_SIGNAL, 1);
	wd_cntrl(WIND0, WD_ON);
	wd_cntrl(WIND1, WD_ON);
	return;
}

VLIST state_vl[] = {
	"fix_win_siz",	&fixsiz,	NP,	NP,	0,	ME_DEC,
	"tar_win_siz",	&tarsiz,	NP,	NP,	0,	ME_DEC,
	"active_obj",	&actObj,	NP,	NP,	0,	ME_DEC,
	NS,
};
char hm_sv_vl[]= "";

%%
id 400
restart rinitf
main_set {
status ON
begin	first:
		to disabl
	disabl:
		to enable on -PSTOP & softswitch
	enable:
		code ENABLECD
		rl 0
		to actset
	actset:
		do PvexSetActiveObject(&actObj)
		to fploc on 0 % tst_rx_new
	fploc:
		do PvexSetFixLocation(&fixPtX, &fixPtY)
		to fpclr on 0 % tst_rx_new
	fpclr:
		do PvexSetFixColors(&fponclr, &fponclr, &fponclr, &fpdimclr, &fpdimclr, &fpdimclr)
		to fpsiz on 0 % tst_rx_new
	fpsiz:
		do PvexSetFixSiz(&fpsize)
		to fpncmd on 0 % tst_rx_new
	fpncmd:
		do PvexSwitchFix(&fixOn)
		to fpon on 0 % tst_rx_new
	fpon:
		code FPONCD
		rl 10
		to setwnd
	setwnd:
		do ctr_wnd()
		to wfeye
	wfeye:
		time 1000
		to eyein on -WD0_XY & eyeflag
		to setwnd
	eyein:
		do awind(OPEN_W)
		time 500
		rand 500
		rl 20
		to error on +WD0_XY & eyeflag
		to tgncmd
	tgncmd:
		do PvexSwitchStim(&nTarg, &actObj, &fixOn)
		to targon on 0 % tst_rx_new
	targon:
		code TARGONCD
		time 100
		rl 30
		to error on +WD0_XY & eyeflag
		to fpfcmd
	fpfcmd:
		do PvexSwitchFix(&fixOff)
		to fpoff on 0 % tst_rx_new
	fpoff:
		code FPOFFCD
		rl -5
		time 1000
		to chkwnd on +WD0_XY & eyeflag
		to error
	chkwnd:
		code SACSTARTCD
		time 100
		rl +15
		to closew on -WD1_XY & eyeflag
		to error
	closew:
		do awind(CLOSE_W)
		rl +5
		time 20
		to rewon
	rewon:
		code REWCD
		do dio_on(REW)
		time 20
		to rewoff
	rewoff:
		do dio_off(REW)
		time 200
		rl -5
		to tgfcmd
	tgfcmd:
		do PvexSwitchStim(&nTarg, &actObj, &fixOff)
		to taroff on 0 % tst_rx_new
	taroff:
		code TARGOFFCD
		rl -10
		to correct
	correct:
		do score(YES)
		to first
	error:
		do awind(CANCEL_W)
		to reset
	reset:
		code ERR1CD
		do reset_s(-1)		
		rl -25
		to cnclcm
	cnclcm:
		do PvexAllOff()
		to wrong on 0 % tst_rx_new
	wrong:
		do score(NO)
		time 1000
		to first
abort	list:
		rewoff closew
}

/*
 * Object location query state set
 */
where_set{
status ON
begin	start:
		to halt
	halt:
		to query on -PSTOP & softswitch
	query:
		do PvexReportLocation(&actObj)
		to vxmsg on 0 % tst_rx_new
	vxmsg:
		do PvexMessage(&vxCode, &vxX, &vxY)
		to stgwnd on OBJECT_LOCATION = vxCode
		to wait
	stgwnd:
		do Pwd_pos(&wind1, &vxX, &vxY)
		to wait
	wait:
		time 100
		to start
}

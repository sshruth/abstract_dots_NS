#include <stdio.h>
#include <sys/types.h>
#include "rexhdrs.h"
#include "ecode.h"
#include "int.h"




extern int softswitch;
extern int softswitch;
extern int softswitch;
extern int nextramp();
extern int rampstart();
extern int awind();
extern int dio_on();
extern int eyeflag;
extern int dio_on();
extern int eyeflag;
extern int set_wsiz();
extern int ramptset();
extern int eyeflag;
extern int dio_off();
extern int dio_off();
extern int awind();
extern int set_wsiz();
extern int dio_on();
extern int dio_off();
extern int rampstop();
extern int correctTrial();
extern int awind();
extern int reset_s();
extern int errorTrial();
extern int ramptd();
extern int newmark();
extern int newstring();



STATE sfirst;
STATE spause1;
STATE spause2;
STATE sgo;
STATE sselramp;
STATE sstramp;
STATE sopenw;
STATE sfpon;
STATE seyeon;
STATE sstimon;
STATE swdchange;
STATE ssetpres;
STATE sfpoff;
STATE sstoff;
STATE sclosew;
STATE swdback;
STATE srewon;
STATE srewoff;
STATE sstopramp;
STATE scorrect;
STATE serror;
STATE sreset;
STATE swrong;
STATE sramptw;
STATE stimeramp;
STATE sramptd;
STATE smfirst;
STATE smsecond;
STATE smthird;



STATE *snames[] = {
&sfirst,
&spause1,
&spause2,
&sgo,
&sselramp,
&sstramp,
&sopenw,
&sfpon,
&seyeon,
&sstimon,
&swdchange,
&ssetpres,
&sfpoff,
&sstoff,
&sclosew,
&swdback,
&srewon,
&srewoff,
&sstopramp,
&scorrect,
&serror,
&sreset,
&swrong,
&sramptw,
&stimeramp,
&sramptd,
&smfirst,
&smsecond,
&smthird,
0};

/*
 * Rex ramp test paradigm.
 *
 */
#include <stdlib.h>
#include "../hdr/ramp.h"
#include "ldev_tst.h"
#include "lcode_tst.h"

#define RAMP0	    0	    /* ramps and windows used in paradigm */
#define RAMP1	    1
#define WIND0	    0
#define WIND1	    1
#define EYEALL	    ( (WD0_XY << (WIND0 * 2)) | (WD0_XY << (WIND1 * 2)) )
#define EYEH_SIG    0	    /* signal numbers for eyes */
#define EYEV_SIG    1
#define OEYEH_SIG   2
#define OEYEV_SIG   3

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
static int do_two_ramps= 1;
static int r0_xda= 0, r0_yda= 1, r1_xda= 2, r1_yda= 3;
static int clear_all= 0;
static int tst_single= 0; single_step= 0;
static RL rl_sav;

int nTrials;
int trialsRemaining = 16;
int nBlocks = 0;
int correctTrials = 0;
int errorTrials = 0;
int totalTrials = 0;
int percentCorrect = 0;

int
nextramp(long flag)
{
	if(flag || (rlp->len == -1)) {
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

	ra_new(RAMP0, rlp->len, rlp->ang, rlp->vel, rlp->xoff, rlp->yoff,
			rlp->ecode, rlp->type);
	if(do_two_ramps) {
	    ra_new(RAMP1, rlp->len, ((rlp->ang + 180) % 360),
		   rlp->vel, rlp->xoff, rlp->yoff, rlp->ecode, rlp->type);
	}
	rxwind= rlp->xwind;
	rywind= rlp->ywind;
	roxwind= rlp->oxwind;
	roywind= rlp->oywind;

	off_eye(rlp->eyehoff, rlp->eyevoff);
	off_oeye(rlp->oeyehoff, rlp->oeyevoff);

	rlp++;

	return(rlp->ecode);
}

int
rampstart(long LED)
{
	ra_start(RAMP0, 1, LED);
	if(do_two_ramps) ra_start(RAMP1, 1, LED);
	return(0);
}

int
rampstop(void)
{
	ra_stop(RAMP0);
	if(do_two_ramps) ra_stop(RAMP1);
	clear_all_da();
	return(0);
}

int
ramptset(long rnum, long preset, long rand)
{
	RA_RAMP_TIME ra_time;

	ra_time= ra_compute_time(rnum, preset, rand);
	if(ra_time.ra_ramp_time_preset != -1) {
		set_times("timeramp", (long)ra_time.ra_ramp_time_preset, (long)ra_time.ra_ramp_time_random);
		ramp[rnum].ra_rampflag |= RA_TIMESTART;
	}
	return(0);
}

int
set_wsiz(long flag)
{
	if(flag) {
		wd_siz(WIND0, rxwind, rywind);
		wd_siz(WIND1, roxwind, roywind);
	} else {
		wd_siz(WIND0, fxwd, fywd);
		wd_siz(WIND1, foxwd, foywd);
	}
	return(0);
}

int correctTrial()
{
/*	score(YES); */
	correctTrials++;
	if(totalTrials) percentCorrect = 100 * correctTrials / totalTrials;
	return(0);
}

int errorTrial()
{
/* 	score(NO); */
	errorTrials++;
	if(totalTrials) percentCorrect = 100 * correctTrials / totalTrials;
	return(0);
}

void
rinitf(void)
{
    /*
     * Initializations.
     */
    da_cntrl_2(r0_xda, DA_RAMP_X, RAMP0, r0_yda, DA_RAMP_Y, RAMP0);	    /* first ramp */
    da_cntrl_2(r1_xda, DA_RAMP_X, RAMP1, r1_yda, DA_RAMP_Y, RAMP1);	    /* second ramp */
    da_cursor(r0_xda, r0_yda, CU_DA_ONE);	    /* da cursors */
    da_cursor(r1_xda, r1_yda, CU_DA_TWO);
    clear_all_da();
    wd_disp(D_W_ALLCUR & ~D_W_JOY);	    /* all cursors but joystick */
    wd_src_pos(WIND0, WD_DA, r0_xda, WD_DA, r0_yda);
    wd_src_pos(WIND1, WD_DA, r1_xda, WD_DA, r1_yda);
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

int
clear_all_da(void)
{
    da_set_2(r0_xda, 0, r0_yda, 0);
    da_set_2(r1_xda, 0, r1_yda, 0);
    return(0);
}
    
int marknum= 1;
int
newmark()
{
	RL_CHAN *rcp;

	if(marknum > 4) marknum= 1;
	sd_mark(marknum-1, marknum);
	marknum++;

	if(tst_single) {

	  switch(single_step++) {
	  case 0:
	    for(rcp= &i_b->rl.rl_chan[0]; rcp < &i_b->rl.rl_chan[4]; rcp++)
	    rl_trig();
	    break;
	  case 1:
	    for(rcp= &i_b->rl.rl_chan[0]; rcp < &i_b->rl.rl_chan[4]; rcp++)
	    rl_trig();
	    single_step= 0;
	    break;
	  }
	}
	return(0);
}

static int tst_user= 0;
static int strcount= 0;

int
newstring()
{
	if(!tst_user) return(0);
	if(strcount == 0) {
		ustatus("Change 1");
		strcount= 1;
	} else {
		ustatus("Back to test...");
		strcount= 0;
	}
	return(0);
}

#pragma off (unreferenced)
int
do_clear_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd)
{
#pragma on (unreferenced)
	clear_all_da();
	return(0);
}
	
static int rl_valid= 0;
#pragma off (unreferenced)
int
setup_single_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd)
{
#pragma on (unreferenced)

    if(tst_single) {

	/*
 	 * Setup single trace mode.  Save current rl struct.
	 */
	rl_sav= i_b->rl;
	rl_valid= 1;
	i_b->d_flags |= D_RL_SINGLE;
	single_step= 0;
    } else {

	/*
	 * Return to continuous mode.
	 */
	if(rl_valid) i_b->rl= rl_sav;
	i_b->d_flags &= ~D_RL_SINGLE;
    }
    return(0);
}

/*
 * Declaration of statelist menu.
 */
VLIST state_vl[] = {
	{"clear_all_da",		&clear_all, NP, do_clear_vaf, ME_AFT, ME_DEC},
	{"do_two_ramps",		&do_two_ramps, NP, NP, 0, ME_DEC},
	{"ramp0_xda",		&r0_xda, NP, NP, 0, ME_DEC},
	{"ramp0_yda",		&r0_yda, NP, NP, 0, ME_DEC},
	{"ramp1_xda",		&r1_xda, NP, NP, 0, ME_DEC},
	{"ramp1_yda",		&r1_yda, NP, NP, 0, ME_DEC},
	{"fix_xwind",		&fxwd, NP, NP, 0, ME_DEC},
	{"fix_ywind",		&fywd, NP, NP, 0, ME_DEC},
	{"fix_oxwind",		&foxwd, NP, NP, 0, ME_DEC},
	{"fix_oywind",		&foywd, NP, NP, 0, ME_DEC},
	{"test_user_msg",	&tst_user, NP, NP, 0, ME_DEC},
	{"test_single_trace",	&tst_single, NP, setup_single_vaf, ME_AFT, ME_DEC},
	NS,
};

char hm_sv_vl[]= "\
do_two_ramps-\n\
  0: one ramp active\n\
  1: two ramps active";

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
 * Declaration of ramp list menu
 */
VLIST ramp_vl[] = {
	{"length",	&((RA_LIST *)NP)->len, 		NP, NP, ME_GB, ME_DEC},
	{"angle",	&((RA_LIST *)NP)->ang, 		NP, NP, ME_GB, ME_DEC},
	{"velocity",	&((RA_LIST *)NP)->vel, 		NP, NP, ME_GB, ME_DEC},
	{"xoff",		&((RA_LIST *)NP)->xoff,		NP, NP, ME_GB, ME_DEC},
	{"yoff",		&((RA_LIST *)NP)->yoff,		NP, NP, ME_GB, ME_DEC},
	{"type",		&((RA_LIST *)NP)->type, 	NP, NP, ME_GB, ME_DEC},
	{"ecode",	&((RA_LIST *)NP)->ecode, 	NP, NP, ME_GB, ME_DEC},
	NS,
};
char hm_ramp_vl[] = "";

/*
 * Declaration of eye windows menu
 */
VLIST eyewnds_vl[] = {
	{"xwind",	&((RA_LIST *)NP)->xwind,	NP, NP, ME_GB, ME_DEC},
	{"ywind",	&((RA_LIST *)NP)->ywind,	NP, NP, ME_GB, ME_DEC},
	{"oxwind",	&((RA_LIST *)NP)->oxwind,	NP, NP, ME_GB, ME_DEC},
	{"oywind",	&((RA_LIST *)NP)->oywind,	NP, NP, ME_GB, ME_DEC},
	NS,
};
char hm_eyewnds_vl[] = "";

/*
 * Declaration of offsets menu
 */
VLIST eyeoffs_vl[] = {
	{"eyehoff",	&((RA_LIST *)NP)->eyehoff,	NP, NP, ME_GB, ME_DEC},
	{"eyevoff",	&((RA_LIST *)NP)->eyevoff,	NP, NP, ME_GB, ME_DEC},
	{"oeyehoff",	&((RA_LIST *)NP)->oeyehoff,	NP, NP, ME_GB, ME_DEC},
	{"oeyevoff",	&((RA_LIST *)NP)->oeyevoff,	NP, NP, ME_GB, ME_DEC},
	NS,
};
char hm_eyeoffs_vl[] = "";
/*
 * User-supplied menu table.
 */
MENU umenus[] = {
	{"state_vars",	&state_vl, NP, NP, 0, NP, hm_sv_vl},
	{"separator",	NP,		   NP, NP, 0, NP, 0},
	{"ramp_list",	&ramp_vl,  NP, ral_maf, ME_BEF, ral_agf, hm_ramp_vl},
	{"separator",	NP,		   NP, NP, 0, NP, 0},
	{"eye_winds",	&eyewnds_vl, NP, ral_maf, ME_BEF, ral_agf, hm_eyewnds_vl},
	{"eye_offsets",	&eyeoffs_vl, NP, ral_maf, ME_BEF, ral_agf, hm_eyeoffs_vl},
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
	{"number of trials",	&nTrials},
	{"trials remaining", 	&trialsRemaining},
	{"blocks completed",	&nBlocks},
	{"total trials",		&totalTrials},
	{"number correct",		&correctTrials},
	{"number wrong",		&errorTrials},
	{"percent correct",		&percentCorrect},
	{"", 0},
};

STATE sfirst = {
	{"first"},STARTCD,0
	,0,0
	,{0,0,0,0,0,0,0,0,0,0,0}
	,1500,0
	,{
	{0,0,&spause1,TIME,0,0},
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
STATE spause1 = {
	{"pause1"},0,0
	,0,0
	,{0,0,0,0,0,0,0,0,0,0,0}
	,0,0
	,{
	{0,0,&spause2,BITON,&softswitch,PSTOP },
	{0,0,&sgo,BITOFF,&softswitch,PSTOP },
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
STATE spause2 = {
	{"pause2"},PAUSECD,0
	,0,0
	,{0,0,0,0,0,0,0,0,0,0,0}
	,0,0
	,{
	{0,0,&sgo,BITOFF,&softswitch,PSTOP },
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
STATE sgo = {
	{"go"},ENABLECD,0
	,1,0
	,{0,0,0,0,0,0,0,0,0,0,0}
	,0,0
	,{
	{0,0,&sselramp,TIME,0,0},
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
STATE sselramp = {
	{"selramp"},0,0
	,0,0
	,{nextramp,0,0,0,0,0,0,0,0,0,0}
	,0,0
	,{
	{0,0,&sstramp,TIME,0,0},
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
STATE sstramp = {
	{"stramp"},0,0
	,1,15
	,{rampstart,LED1,0,0,0,0,0,0,0,0,0}
	,0,0
	,{
	{0,0,&sopenw,TIME,0,0},
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
STATE sopenw = {
	{"openw"},0,0
	,0,0
	,{awind,OPEN_W,0,0,0,0,0,0,0,0,0}
	,0,0
	,{
	{0,0,&sfpon,BITON,&ramp[RAMP0].ra_rampflag,RA_STARTED },
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
STATE sfpon = {
	{"fpon"},FPONCD,0
	,1,5
	,{dio_on,LED1,0,0,0,0,0,0,0,0,0}
	,250,0
	,{
	{0,0,&seyeon,TIME,0,0},
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
STATE seyeon = {
	{"eyeon"},0,0
	,1,40
	,{0,0,0,0,0,0,0,0,0,0,0}
	,250,0
	,{
	{0,0,&serror,BITON,&eyeflag,EYEALL },
	{0,0,&sstimon,TIME,0,0},
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
STATE sstimon = {
	{"stimon"},STIMCD,0
	,0,0
	,{dio_on,BACKLT,0,0,0,0,0,0,0,0,0}
	,0,0
	,{
	{0,0,&serror,BITON,&eyeflag,EYEALL },
	{0,0,&swdchange,TIME,0,0},
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
STATE swdchange = {
	{"wdchange"},0,0
	,1,50
	,{set_wsiz,1,0,0,0,0,0,0,0,0,0}
	,0,0
	,{
	{0,0,&ssetpres,TIME,0,0},
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
STATE ssetpres = {
	{"setpres"},0,0
	,0,0
	,{ramptset,RAMP0,80,20,0,0,0,0,0,0,0}
	,0,0
	,{
	{0,0,&serror,BITON,&eyeflag,EYEALL },
	{0,0,&sfpoff,BITON,&ramp[RAMP0].ra_rampflag,RA_TIMEDONE },
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
STATE sfpoff = {
	{"fpoff"},FPOFFCD,0
	,1,35
	,{dio_off,LED1,0,0,0,0,0,0,0,0,0}
	,0,0
	,{
	{0,0,&sstoff,TIME,0,0},
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
STATE sstoff = {
	{"stoff"},STOFFCD,0
	,0,0
	,{dio_off,BACKLT,0,0,0,0,0,0,0,0,0}
	,0,0
	,{
	{0,0,&sclosew,TIME,0,0},
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
STATE sclosew = {
	{"closew"},0,0
	,0,0
	,{awind,CLOSE_W,0,0,0,0,0,0,0,0,0}
	,0,0
	,{
	{0,0,&swdback,TIME,0,0},
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
STATE swdback = {
	{"wdback"},0,0
	,0,0
	,{set_wsiz,0,0,0,0,0,0,0,0,0,0}
	,0,0
	,{
	{0,0,&srewon,TIME,0,0},
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
STATE srewon = {
	{"rewon"},0,0
	,0,0
	,{dio_on,BEEP,0,0,0,0,0,0,0,0,0}
	,35,0
	,{
	{0,0,&srewoff,TIME,0,0},
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
STATE srewoff = {
	{"rewoff"},0,0
	,0,0
	,{dio_off,BEEP,0,0,0,0,0,0,0,0,0}
	,0,0
	,{
	{0,0,&sstopramp,TIME,0,0},
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
STATE sstopramp = {
	{"stopramp"},0,0
	,1,20
	,{rampstop,1,0,0,0,0,0,0,0,0,0}
	,0,0
	,{
	{0,0,&scorrect,TIME,0,0},
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
STATE scorrect = {
	{"correct"},0,0
	,0,0
	,{correctTrial,0,0,0,0,0,0,0,0,0,0}
	,0,0
	,{
	{0,0,&sfirst,TIME,0,0},
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
STATE serror = {
	{"error"},0,0
	,0,0
	,{awind,CANCEL_W,0,0,0,0,0,0,0,0,0}
	,0,0
	,{
	{0,0,&sreset,TIME,0,0},
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
STATE sreset = {
	{"reset"},ERR1CD,0
	,0,0
	,{reset_s,-1,0,0,0,0,0,0,0,0,0}
	,0,0
	,{
	{0,0,&swrong,TIME,0,0},
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
STATE swrong = {
	{"wrong"},0,0
	,0,0
	,{errorTrial,0,0,0,0,0,0,0,0,0,0}
	,0,0
	,{
	{0,0,&sfirst,TIME,0,0},
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
STATE sramptw = {
	{"ramptw"},0,0
	,0,0
	,{0,0,0,0,0,0,0,0,0,0,0}
	,0,0
	,{
	{0,0,&stimeramp,BITON,&ramp[RAMP0].ra_rampflag,RA_TIMESTART },
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
STATE stimeramp = {
	{"timeramp"},0,0
	,0,0
	,{0,0,0,0,0,0,0,0,0,0,0}
	,0,0
	,{
	{0,0,&sramptw,BITOFF,&ramp[RAMP0].ra_rampflag,RA_TIMESTART },
	{0,0,&sramptd,TIME,0,0},
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
STATE sramptd = {
	{"ramptd"},RTDONECD,0
	,0,0
	,{ramptd,RAMP0,0,0,0,0,0,0,0,0,0}
	,0,0
	,{
	{0,0,&sramptw,TIME,0,0},
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
STATE smfirst = {
	{"mfirst"},0,0
	,0,0
	,{0,0,0,0,0,0,0,0,0,0,0}
	,0,0
	,{
	{0,0,&smsecond,TIME,0,0},
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
STATE smsecond = {
	{"msecond"},0,0
	,0,0
	,{newmark,0,0,0,0,0,0,0,0,0,0}
	,3000,0
	,{
	{0,0,&smthird,TIME,0,0},
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
STATE smthird = {
	{"mthird"},0,0
	,0,0
	,{newstring,0,0,0,0,0,0,0,0,0,0}
	,0,0
	,{
	{0,0,&smfirst,TIME,0,0},
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



STATE *aborta[] = {
&sfpoff,
&sstoff,
&srewoff,
&sstopramp,
&swdback,
&sclosew,
0};

STATE *abortb[] = {
0};

STATE *abortc[] = {
0};

int sf_init();

STATE sps_state;

AWAKE init_state[] = {
	{0,ON,ON,&sps_state,&sps_state,0,(STATE *)&init_state[1],0,
			&sps_state.escape}
	,{0,0,0,0,0,0,0,0,&sps_state.escape[1]}
};
STATE sps_state = {
	{"spec"},1,0
	,1,0
	,{sf_init,700,0,0,0,0,0,0,0,0,0}
	,0,0
	,{
	{0,init_state,&sps_state,TIME,0,0}
	,{0,0,0,0,0,0}
	}
};
AWAKE nowstate[] = {
	{0,ON,ON,&sfirst,&sfirst,aborta,0,0,0}
	,{0,ON,ON,&sramptw,&sramptw,abortb,0,0,0}
	,{0,ON,ON,&smfirst,&smfirst,abortc,0,0,0}
	,{0,0,0,0,0,0,0,0,0}
};

void rinitf();
int (*init_list[])() = {
rinitf,
0};
NOUN unouns[] = {
"",
};

/*
 * Rex memory source test paradigm.
 *
 */

#include "ldev_tst.h"
#include "lcode_tst.h"
#include <math.h>

#define MEM0	    0	    /* ramps and windows used in paradigm */
#define MEM1	    1
#define DAX		    0
#define DAY			1
#define WIND0	    0
#define WIND1	    1
#define EYEALL	    ( (WD0_XY << (WIND0 * 2)) | (WD0_XY << (WIND1 * 2)) )
#define EYEH_SIG    0	    /* signal numbers for eyes */
#define EYEV_SIG    1
#define OEYEH_SIG   2
#define OEYEV_SIG   3


#define M_PI		3.14159265358979323846
#define BSIZE 500
int mem0[BSIZE * 2];	/* place to store memory sources */
int mem1[BSIZE * 2];
int scale0= 800, scale1= 800, phase= 90;
int fxwd= 100, fywd= 100;
int rate0= 5, rate1= 5;
int repeat0= 1, repeat1= 1;


int
mem_compute(void)
{
    double ang_inc, phase_inc;
    int i;

    /*
     * Two sines 90 degs out of phase driving horizontal and vertical
	 * mirrors generates a circle.
     */
    ang_inc= (2 * M_PI) / BSIZE;
    phase_inc= phase;
    for(i=0; i < BSIZE; i++) {
		mem0[i]= sin(i * ang_inc) * scale0;
		mem1[i]= sin( (i * ang_inc) + ((phase_inc * M_PI) / 180) ) * scale1;
    }
    ma_cntrl(MEM0, mem0, BSIZE, rate0, repeat0);
    ma_cntrl(MEM1, mem1, BSIZE, rate1, repeat1);
    return(0);
}
		
int
mem_begin()
{
    ma_start(MEM0);
    ma_start(MEM1);
    return(0);
}

int
mem_stop()
{
    ma_stop(MEM0);
    ma_stop(MEM1);
    return(0);
}

int
clear_all_da(void)
{
    da_set_2(DAX, 0, DAY, 0);
    return(0);
}

void
rinitf(void)
{
	
    /*
     * Initializations.
     */
    mem_compute();
    ma_reset(MEM0);
    ma_reset(MEM1);
    da_cntrl_2(DAX, DA_MEM, MEM0, DAY, DA_MEM, MEM1);
    da_cursor(DAX, DAY, CU_DA_ONE);	    /* da cursors */
    clear_all_da();
    wd_disp(D_W_ALLCUR & ~D_W_JOY);	    /* all cursors but joystick */
    wd_src_pos(WIND0, WD_DA, DAX, WD_DA, DAY);
    wd_src_check(WIND0, WD_SIGNAL, EYEH_SIG, WD_SIGNAL, EYEV_SIG);
    wd_siz(WIND0, fxwd, fywd);
    wd_pos(WIND0, 0, 0);
    wd_cntrl(WIND0, WD_ON);
    wd_test(1);
}

/*
 * Declaration of statelist menu.
 */
int clear_all;
VLIST state_vl[] = {
"clear_all_da",	&clear_all, NP, clear_all_da, ME_AFT, ME_DEC,
"phase",		&phase, NP, NP, 0, ME_NVALD,
"scale0",		&scale0, NP, NP, 0, ME_NVALD,
"scale1",		&scale1, NP, NP, 0, ME_NVALD,
"rate0",		&rate0, NP, NP, 0, ME_NVALD,
"rate1",		&rate1, NP, NP, 0, ME_NVALD,
"repeat0",		&repeat0, NP, NP, 0, ME_NVALD,
"repeat1",		&repeat1, NP, NP, 0, ME_NVALD,
"fix_xwind",	&fxwd, NP, NP, 0, ME_DEC,
"fix_ywind",	&fywd, NP, NP, 0, ME_DEC,
NS,
};

char hm_sv_vl[]= "\
phase: phase angle\n\
 between sines\n\
scale: scale\n\
 factor";

%%

id 701
restart rinitf
main_set {
status ON
begin	first:
		code STARTCD
		to disable
    disable:
		to go on -PSTOP & softswitch
	go:
		code ENABLECD
		to selmem
	selmem:
		do mem_compute()		/* select next mem */
		to stmem
	stmem:
		do mem_begin()			/* start output */
		to openw
	openw:
		do awind(OPEN_W)
		time 1000			/* if repeat count == 0 MA_RUN will never set */
		to fpon on +MA_RUN & ma[MEM0].ma_status
		to error		
	fpon:
		code FPONCD
		do dio_on(LED1)
		time 250
		to eyeon
	eyeon:
		time 250
		to error on +EYEALL & eyeflag	/* start checking for eye */
		to stimon
	stimon:
		code STIMCD
		do dio_on(BACKLT)
		to error on +EYEALL & eyeflag
		to fpoff on -MA_RUN & ma[MEM0].ma_status
	fpoff:
		code FPOFFCD
		do dio_off(LED1)
		to stoff
	stoff:
		code STOFFCD
		do dio_off(BACKLT)
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
		to stopmem
	stopmem:
		do mem_stop()
		to correct
	correct:
		do score(YES)
		time 250
		to first
	error:				/* abort trial */
		do awind(CANCEL_W)
		to reset
	reset:
		code ERR1CD
		do reset_s(-1)
		to wrong
	wrong:
		do score(NO)
		time 250
		to first
abort	list:
		fpoff stoff rewoff stopmem
}

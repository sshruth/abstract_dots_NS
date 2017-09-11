#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

#include "ldev.h"
#include "../labTools/lcode.h"
#include "../labTools/toys.h"
#include "../labTools/matlab.h"
#include "../labTools/timer.h"
#include "../labTools/udpmsg.h"
#include "../labTools/dio_lab.h"

/* signal numbers for eyes */
#define EYEH_SIG   0	   
#define EYEV_SIG   1

/* signal numbers for joy */
#define JOYH_SIG   6
#define JOYY_SIG   7

/* for gl_eye_flag */
#define E_OFF	0
#define E_FIX	1	 

/* windows for eyeflag and screen */
#define WIND0	    0
#define WIND1       1
#define WIND7	    7

#define NUM_OBJECTS 1

#define CALC_JOY(x)	((int) (0.25 * (double) (x)))

struct visObj {
	long x; 
	long y;
    long diameter;
	RGB color;
};

static struct visObj            gl_fixObjEye;
static struct visObj            gl_fixObjHand;

int 							gl_eye_flag = 0;
int  	                        gl_prize_count = 0;
int  	                        gl_max_prize = 0;

int                             gl_eyeFixDiam;
int                             gl_eyeFixX;
int                             gl_eyeFixY;
int                             gl_joy_flag;
int                             storex = 0, storey = 0;

/* ROUTINE: rinitf
 **
 ** initialize at first pass or at r s from keyboard 
 */
void rinitf(void)
{  
	/* close/open udp connection */
	
	 /*	 	
	 ** This is now done in clock.c so that we can open multiple paradigms
	 ** without causing clashes when binding to the socket
	 **
	 */
	 
//	udp_close();
//	udp_open(rexIP, udpPORT);
	
	/* initialize interface (window) parameters */
	wd_cntrl 		(WIND0, WD_ON);
	wd_src_check 	(WIND0, WD_SIGNAL, EYEH_SIG, WD_SIGNAL, EYEV_SIG);
	wd_src_pos 	    (WIND0, WD_DIRPOS, 0, WD_DIRPOS, 0);
	

	/* static window for screen limits */
#ifdef SCR_LIM_HOR /* defined in ldev.h */
	wd_cntrl	 	(WIND7, WD_ON);
	wd_src_pos 	    (WIND7, WD_DIRPOS, 0, WD_DIRPOS, 0);
	wd_pos 		    (WIND7,	 0, 0);
	wd_siz	        (WIND7, SCR_LIM_HOR, SCR_LIM_VER); /* from ldev.h! */
#endif
	
}

int setup_screen(long mon_width_cm, long view_dist_cm, long num_targets, long long repeat_flag)
{
	static int first_time = 1;
	
	/* Do these only if first time through or if explicitly
	 ** told to do each time... enable pcmsg & init the screen
	 */
	if(repeat_flag || first_time) {
		first_time = 0;
		
		/* init the screen */
		printf("\n");
		printf("Initializing screen with values from state list (root file):\n");
		mat_initScreen(mon_width_cm, view_dist_cm, num_targets, 0);
		
	}
	
	printf("\n");
	printf("Horizontal screen size: %d cm\n", mon_width_cm);
	printf("Viewing distance: %d cm\n", view_dist_cm);
	printf("\n");
	
	return 0; 
}

/*  
 ** ROUTINE: drawTarg, 1 to show, 0 to erase
 **    
 */
int drawTarg(long showFixEye)
{
	int num_show=0, num_hide=0;
	int show_ind[NUM_OBJECTS], hide_ind[NUM_OBJECTS];
	int flags[1] = {showFixEye};
	int i;
	
	for(i=0; i<1; i++)	{
		if (flags[i])
		  show_ind[num_show++] = i+1;
		else
		  hide_ind[num_hide++] = i+1;
	}
	
	mat_targDraw(num_show, show_ind, num_hide, hide_ind);
	
	return 0;
}

int initScreen_done()
{ 
	return mat_getWent(MAT_INIT_SCREEN_CMD, IS_EXECUTED); 
}

int drawTarg_done()
{
	return mat_getWent(MAT_TARG_DRAW_CMD, IS_EXECUTED);
}


/* ROUTINE: nexttrl
**
*/
int nexttrl()
{

	if (gl_joy_flag == 0) {
		gl_fixObjEye.x = gl_eyeFixX;
		gl_fixObjEye.y = gl_eyeFixY;
	} else if (gl_joy_flag == 1) {
		gl_fixObjEye.x = storex;
		gl_fixObjEye.y = storey;
	}
	gl_fixObjEye.color.R = 255;
	gl_fixObjEye.color.G = 0;
	gl_fixObjEye.color.B = 0;
	gl_fixObjEye.diameter = gl_eyeFixDiam;
	

	/* send the target setup commands */
	mat_targDefine(1, gl_fixObjEye.x, gl_fixObjEye.y, gl_fixObjEye.diameter, &(gl_fixObjEye.color));
	
	/* set consecutive reward counter */
	gl_prize_count = gl_max_prize;
	
	return 0;
}

/* ROUTINE position_eyewindow
**
** sets window location
*/
int position_eyewindow(long wd_width, long wd_height, long flag)
{
 
 	wd_pos(WIND0, gl_fixObjEye.x, gl_fixObjEye.y);
	EC_TAG2(I_EFIX_ACCEPTHX, wd_width);
	EC_TAG2(I_EFIX_ACCEPTHY, wd_height);
	wd_siz(WIND0, wd_width, wd_height);
	wd_cntrl(WIND0, WD_ON);
	return(0);
}

/* ROUTINE: set_eye_flag
**
*/

int set_eye_flag(long flag)
{
	gl_eye_flag = flag;
	if (flag == E_FIX) 
	   ec_send_code(EFIXACQ);
	return(0);
}
		
int joySet()
{
	storex=CALC_JOY(joyh);
	storey=CALC_JOY(joyv);
	return 0;
}


int give_reward(long dio, long duration)
 {	
   dio_on(dio);
   timer_set1(0,0,0,0,duration,0);
   
   return 0;
 }
 
int total(long score)
{
	/* set globals for eye checking */
	gl_eye_flag   = 0;

	/* turn off reward if it was on */
	dio_off(REW);
	

	/* Drop the appropriate code */
	if(score == CORRECT) {
		ec_send_code(CORRECTCD);
		EC_TAG1(I_RESPONSE, 1);
	} else if(score == WRONG) {
		ec_send_code(WRONGCD);
		EC_TAG1(I_RESPONSE, 0);
	} else if(score == NCERR) {
		ec_send_code(NOCHCD);
		EC_TAG1(I_RESPONSE, -1);
	} else if(score == BRFIX) {
		ec_send_code(FIXBREAKCD);
		EC_TAG1(I_RESPONSE, -2);
		again(); 			/* this is equivalent of reset_s(), it executes the abort list */
	} else if(score == BRHANDFIX) {
		ec_send_code(FIXHANDBREAKCD);
		EC_TAG1(I_RESPONSE, -3);
		again(); 			/* this is equivalent of reset_s(), it executes the abort list */
	}
	/* outta */
	return(0);
}

/* ROUTINE: end_trial
**
*/
int end_trial(long aflag)
 {
	 /* turn eye position windows off */
	 wd_cntrl(WIND0, WD_OFF); 	
	 
	 /* blank the screen */
	 printf("Aborting\n");
	 mat_allOff();
	 
	 ec_send_code(LASTCD);		   
	 
	 /* close the analog data window */
	 awind(aflag);
	 
	 return(0);
 }
 
/* ROUTINE: abort_cleanup
**
** called only from abort list
*/
int abort_cleanup(void)
 {
	timer_pause(100);				/* wait in case of went				*/
	
	end_trial(CANCEL_W);			/* cancel analog window, blank screen */
	
	return(0);	
 } 

VLIST state_vl[] = {
	{"EyeFixX", 		        &(gl_eyeFixX),          NP, NP, 0, ME_DEC},
	{"EyeFixY", 		        &(gl_eyeFixY),          NP, NP, 0, ME_DEC},
	{"EyeFixDiameter", 		    &(gl_eyeFixDiam),       NP, NP, 0, ME_DEC},
	{"MaxConsecutiveRew", 		&(gl_max_prize),        NP, NP, 0, ME_DEC},
	{"SetPosWithJoystick", 		&(gl_joy_flag),         NP, NP, 0, ME_DEC},
	{NS}};


RTVAR rtvars[] = {};


/* THE STATE SET 
*/
%%
id 001
restart rinitf
main_set {
status ON
begin	first:
		to setup
	setup: 		/* SETUP THE SCREEN */
		do setup_screen(60, 27, 1, 0)
		to loop on 1 % initScreen_done
 	loop: 		/* START THE LOOP -- loop on # trials */
		time 500
		to pause on +PSTOP & softswitch
		to settrl
	pause:
		to settrl on -PSTOP & softswitch
	settrl:		/* set up current trial */
		do nexttrl()
		to fixon
	fixon:
		do drawTarg(1)
		to fixeyewinpos on MAT_WENT % drawTarg_done
		/* position window, wait for fixation */
	fixeyewinpos:
	    do position_eyewindow(15, 15, 0)
	    time 10	/* this is very important - it takes time to set window */
		to fixeyewait
	fixeyewait:    /* wait for either eye or hand fixation */
	    time 5000
	    to fixeyedelay on -WD0_XY & eyeflag
		to nofix
	nofix:		/* failed to attain either eye or hand fixation */
		time 2000
		do end_trial(CANCEL_W)
		to loop
	
	/* Monkey attained eye fixation */
	fixeyedelay:
	    time 20 /* delay before activating eye_flag - noise on the eye position signal should not be able to "break eye fixation" */
		to fixeyeset
	fixeyeset:		/* set flag to check for eye fixation breaks */
		do set_eye_flag(E_FIX)
	    time 10 /* give the gaze and hand position time to settle into place */
	    to prrew
	  
	prrew:
		do give_reward(REW, 150)
		to prrew_off on +MET % timer_check1
	prrew_off:
		do dio_off(REW)
  		to prize_loop on 0 < gl_prize_count
		to pref
	prize_loop:
		to pref on 0 ? gl_prize_count
		to prize_delay
	prize_delay:
		time 150		/* the delay between consecutive rewards */
		to prize 
	prize:
		do give_reward(REW, 50, PRIZE) /* prize reward size */
		to prize_off on +MET % timer_check1
	prize_off:
		do dio_off(REW)
		to prize_loop

	pref:
		do total(CORRECT)
		to prend
	prend:
		do end_trial(CLOSE_W)
		to prdone
	prdone:
		time 0
	    to loop
		
	abtst:	/* for abort list */
		do abort_cleanup()
		to prdone		
		
abort list:
    abtst
}

/* set to check for fixation break during task...
**	use set_eye_flag to set gl_eye_state to non-zero
** to enable loop
*/
eye_set {
status ON
begin	efirst:
		to etest
	etest:
		to echk on E_FIX = gl_eye_flag
	echk:
		to efail on +WD0_XY & eyeflag
		to etest on E_OFF = gl_eye_flag
	efail:
		do total(BRFIX)
		to etest

abort list:
}

joy_set {
status ON
begin   jfirst:
        to jchk
	jchk:  
	   	to joyButton on 0 % dio_check_joybut
    joyButton:
        do joySet()
	    to jchk

abort list:
}
		
udp_set {
status ON
begin ufirst:
		to uchk
	uchk:	
		do udpCheckReceiveFork()
		to uchkAgain
	uchkAgain: // simply prevents looping back on same state, which Rex doesn't always like	
		do udpCheckReceiveFork()
		to uchk
		
abort list:
} 

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

long 		gl_time_fpoff = 0;
long 		gl_time_sacc = 0;

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
int nexttrl(long minn, long maxx, long alfa, long bea) {
//    double unival, duration;
//    double drawnnum, drawnnum1;
    
//    unival = (((double) rand()) / ((double) RAND_MAX));
//    drawnnum1 = (double) (-1*(log(1-unival)/3));
//    drawnnum = pow(drawnnum1,0.333)*1000;
//    if (drawnnum > 1200){
//        drawnnum = 1200;
//    }
//    duration = round(drawnnum) + 500;
//    printf("%f\n",drawnnum1);
// 	printf("%f\n",drawnnum);
//    printf("%f\n",duration);
//    printf("----\n");

//	gl_time_fpoff = i_b->i_time;
//	printf("%f\n",gl_time_fpoff);
	
	int j = 10 + TOY_RAND(5);
	printf("%d\n",j);

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
		
int give_reward(long dio, long duration)
 {	
   dio_on(dio);
   timer_set1(0,0,0,0,duration,0);
   
   return 0;
 }
 
int total(long score)
{	long elapsedtime;
	gl_time_sacc = i_b->i_time;
	elapsedtime = gl_time_sacc - gl_time_fpoff;
	printf("%f\n",elapsedtime);
	printf("----\n");
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
		to loop
	loop: 		/* START THE LOOP -- loop on # trials */
		time 1000
		to settrl
	settrl:		/* set up current trial */
		do nexttrl(500,1200,3,3)
		to prdone
	prdone:
		time 250
		do total(0)
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

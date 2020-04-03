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

#define REWARD_LOOP_OFF	0
#define REWARD_LOOP_ON	1	 

/* windows for eyeflag and screen */
#define WIND0	    0
#define WIND1       1
#define WIND7	    7

/* Number of objects in this task, Fix Point and Single Target */
#define NUM_OBJECTS 2

/*  calculate actual position from voltages from joystick. 
** In Rex's standard, every 40 steps of an analog signal corresponds to 1 degree  
** multiply by 0.25 to get to a resolution of 10 steps per degree. 
*/
#define CALC_JOY(x)	((int) (0.25 * (double) (x)))

struct visObj {
	long x; 
	long y;
    long diameter;
	RGB color;
};

static struct visObj            gl_fixObjEye;
static struct visObj            gl_targObj;

/* GLOBAL VARIABLES */
int 							gl_delay;
int                             gl_ecc;
int                             gl_rft;

int                             gl_eyeFixDiam;
int                             gl_eyeFixX;
int                             gl_eyeFixY;

int                             gl_targDiam;

int 							gl_eye_flag = 0;
int                             gl_reward_fixation = 0;
int                             gl_reward_flag = 0;


int gl_joySet = 0, storex = 0, storey = 0;


int *gl_xPositions = NULL;
int *gl_yPositions = NULL;
int *gl_trialIndices = NULL;

int   gl_record = 0;
int   gl_ntrials = 0;
int   gl_trialCtr = 0;
int   gl_remain = 1;
int   gl_positions = 1;


/* ROUTINES */

/*
 ***** INITIALIZATION routines
 */

/* ROUTINE: rinitf
 ** initialize at first pass or at r s from keyboard 
 */
void rinitf(void)
{  
	/* close/open udp connection */
	/*
	 ** This is now done in clock.c so that we can open multiple paradigms
	 ** without causing clashes when binding to the socket
	 */
	 
//	udp_close();
//	udp_open(rexIP, udpPORT);
	
	/* initialize interface (window) parameters */
	wd_cntrl 		(WIND0, WD_ON);
	wd_src_check 	(WIND0, WD_SIGNAL, EYEH_SIG, WD_SIGNAL, EYEV_SIG);
	wd_src_pos 	    (WIND0, WD_DIRPOS, 0, WD_DIRPOS, 0);
	
	/* for correct reponse */
	wd_cntrl 		(WIND1, WD_ON);
	wd_src_check 	(WIND1, WD_SIGNAL, EYEH_SIG, WD_SIGNAL, EYEV_SIG);
	wd_src_pos 	    (WIND1, WD_DIRPOS, 0, WD_DIRPOS, 0);
	
	/* static window for screen limits */
#ifdef SCR_LIM_HOR /* defined in ldev.h */
	wd_cntrl	 	(WIND7, WD_ON);
	wd_src_pos 	    (WIND7, WD_DIRPOS, 0, WD_DIRPOS, 0);
	wd_pos 		    (WIND7,	 0, 0);
	wd_siz	        (WIND7, SCR_LIM_HOR, SCR_LIM_VER); /* from ldev.h! */
#endif
}


/* ROUTINE: setup_screen
 **
 **	initialize parameters/routines... two sets are
 **	considered: those that typically
 **	need to be set once during a session and those
 **	that should be set each time through the state set
 **
 **  args:
 **		mon_horiz_cm	... screen width
 **		view_dist_cm   ... viewing distance (eyes to screen)
 **		repeat_flag		... a hedge -- if != 0 then always do
 **								initializations
 */
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
		
		printf("\n");
		printf("Horizontal screen size: %d cm\n", mon_width_cm);
		printf("Viewing distance: %d cm\n", view_dist_cm);
		printf("\n");

	}
	

	return 0; 
}

/****
 ***** VISUAL STIMULUS (MATLAB) routines
 ****/

/*  
 ** ROUTINE: drawTarg, 1 to show, 0 to erase
 **    
 */
int drawTarg(long showFixEye, long showTarg)
{
	int num_show=0, num_hide=0;
	int show_ind[NUM_OBJECTS], hide_ind[NUM_OBJECTS];
	int flags[2] = {showFixEye, showTarg};
	int i;
	
	for(i=0; i<2; i++)	{
		if (flags[i])
		  show_ind[num_show++] = i+1;
		else
		  hide_ind[num_hide++] = i+1;
	}
	
	mat_targDraw(num_show, show_ind, num_hide, hide_ind);
	
	return 0;
}

/*
 ** ROUTINE: defTargLum
 ** calls drawTarg(1,1) after setting luminance
 */

int defTargLum(long fixEyeLum, long targLum)
{
	long lum[NUM_OBJECTS] = {fixEyeLum, targLum};
	RGB color;
	struct visObj v_obj;
	int i;
	
	for(i=0; i<2; i++)	{
		switch (i) {
		case 0:
			v_obj = gl_fixObjEye; break;
		case 1:
			v_obj = gl_targObj; break;
		}
		if (lum[i]>=0) {
			color.R = lum[i]*v_obj.color.R / 1000.;
			color.G = lum[i]*v_obj.color.G / 1000.;
			color.B = lum[i]*v_obj.color.B / 1000.;
			mat_targDefine(i+1, v_obj.x, v_obj.y, v_obj.diameter,&color);
			
		}
	}
	drawTarg(1,1);
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


/****
 ***** UTILITY routines
 */

/* ROUTINE position_eyewindow
**
** sets window location
*/

/* ROUTINE position_eyewindow
**
** sets window location
*/
int position_eyewindow(long wd_width, long wd_height, long flag)
{
 
  	if (flag == 0) { // Fixation point
		wd_pos(WIND0, gl_fixObjEye.x, gl_fixObjEye.y);
		EC_TAG2(I_EFIX_ACCEPTHX, wd_width);
		EC_TAG2(I_EFIX_ACCEPTHY, wd_height);
	} else if (flag == 1) { // Target
		wd_pos(WIND0, gl_targObj.x, gl_targObj.y);
		//EC_TAG2(I_ETC_ACCEPTHX, wd_width);
		//EC_TAG2(I_ETC_ACCEPTHY, wd_height);
	}
	wd_siz(WIND0, wd_width, wd_height);
	wd_cntrl(WIND0, WD_ON);
	return(0);
}

int setup_eyewindows(long wd_width, long wd_height)
{
	wd_pos(WIND1, gl_targObj.x, gl_targObj.y);
	wd_siz(WIND1, wd_width, wd_height);
	wd_cntrl(WIND1, WD_ON);
	EC_TAG2(I_ETARG_ACCEPTHX, wd_width);
	EC_TAG2(I_ETARG_ACCEPTHY, wd_height);
}


/* ROUTINE: open_adata
**
** Opens analog data
*/
int open_adata(void)
 {
   awind(OPEN_W);
   return(0);
 }

/* ROUTINE: end_trial
**
*/
int end_trial(long aflag)
 {
	 printf("end_trial\n");
	 /* turn eye position windows off */
	 wd_cntrl(WIND0, WD_OFF); 	
	 wd_cntrl(WIND1, WD_OFF); 	
	 
	 /* blank the screen */   	
	 mat_allOff();
	 
	 ec_send_code(LASTCD);		   
	 
	 /* close the analog data window */
	 awind(aflag);
	 
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


int set_reward_flag(long flag)
{
	gl_reward_flag = flag;
	return(0);
}


/* ROUTNE: give_reward
** 
** description: activates the water reward system
*/
int give_reward(long dio, long duration)
{	
	dio_on(dio);
	timer_set1(0,0,0,0,duration,0);
	
	EC_TAG2(I_REWSIZE_COR, duration);
	
	return 0;
}

int make_task(void) {

	int i, cx, cy;
	
	if (!gl_record) {
		gl_remain = 1;
		return 0;
	}
	
	/* x positions */
	gl_xPositions = (int *) realloc(gl_xPositions,gl_ntrials * sizeof(int));
	if (gl_xPositions == NULL) {
		free(gl_xPositions);
		gl_xPositions = NULL;
		printf("Could not allocate memory for gl_xPositions\n");
	} 
	
	/* y positions */
	gl_yPositions = (int *) realloc(gl_yPositions,gl_ntrials * sizeof(int));
	if (gl_yPositions == NULL) {
		free(gl_yPositions);
		gl_yPositions = NULL;
		printf("Could not allocate memory for gl_yPositions\n");
	}
	
	/* trial indices */
	gl_trialIndices = (int *) realloc(gl_trialIndices,gl_ntrials * sizeof(int));
	if (gl_trialIndices == NULL) {
		free(gl_trialIndices);
		gl_trialIndices = NULL;
		printf("Could not allocate memory for gl_trialIndices\n");
	} 
	
	/* If eccentricity is greater than 0, then we're using 7 positions, not 2 flipped about vertical axis */
//	if (gl_ecc > 0) {
//		float x[]={-1*gl_ecc, 1*gl_ecc, 0,
//			  -1*gl_ecc/sqrt(2), -1*gl_ecc/sqrt(2),
//	     	   1*gl_ecc/sqrt(2), 1*gl_ecc/sqrt(2)
//	 	};
//
//		float y[]={0, 0, 1*gl_ecc,
//			  1*gl_ecc/sqrt(2), -1*gl_ecc/sqrt(2),
//			  1*gl_ecc/sqrt(2), -1*gl_ecc/sqrt(2)
//		};
//
//		for (i = 0; i<gl_ntrials; i++) {
//			//printf("here %f\n", x[i%7]);
//			gl_xPositions[i] = x[i%7]+gl_eyeFixX;
//			gl_yPositions[i] = y[i%7]+gl_eyeFixY;
//		}
//	} else {
//		for (i = 0; i < gl_ntrials/2; i++) {
//			//printf("%d ", i);
//			gl_xPositions[i]= 1 * storex;
//			gl_yPositions[i]= 1 * storey;
//		}
		
//		for (i = gl_ntrials/2; i < gl_ntrials; i++) {
//			//printf("%d ", i);
//			gl_xPositions[i]= -1 * storex*gl_positionMult/10.;
//			gl_yPositions[i]=  1 * storey;
//		}
//	}

    /* Do number of target locations based on gl_positions*/
    cx = TOY_RT_TO_X(gl_eyeFixX,gl_ecc,gl_rft);
    cy = TOY_RT_TO_Y(gl_eyeFixY,gl_ecc,gl_rft);
    if (gl_positions==2){
        
        float x[]={-1*cx, 1*cx };
        float y[]={-1*cy, 1*cy };

        for (i = 0; i<gl_ntrials; i++) {
            //printf("here %f\n", x[i%7]);
            gl_xPositions[i] = x[i%2]+gl_eyeFixX;
            gl_yPositions[i] = y[i%2]+gl_eyeFixY;
        }
        
    } else { /* defaults to presenting at just one location */
        for (i = 0; i < gl_ntrials; i++) {
            //printf("%d ", i);
            gl_xPositions[i]= 1 * cx;
            gl_yPositions[i]= 1 * cy;
        }
    }

	  	
	/* shuffle up the trials */
	for (i = 0; i < gl_ntrials; i++) 
	  gl_trialIndices[i]=i;
	for (i = gl_ntrials-1; i > 0; i--) {
		int c = rand() % gl_ntrials;
		if (c > gl_ntrials-1)
		  printf("uh oh");
		int t = gl_trialIndices[c];
		gl_trialIndices[c] = gl_trialIndices[i];
		gl_trialIndices[i] = t;
	}

	gl_trialCtr = gl_ntrials-1;
	printf("\nX positions = \n");
	for ( i = 0; i < gl_ntrials; i++) {
		printf("%d ", gl_xPositions[gl_trialIndices[i]]);
	}
	printf("\n");
	printf("\nY positions = \n");
	for ( i = 0; i < gl_ntrials; i++) {
		printf("%d ", gl_yPositions[gl_trialIndices[i]]);
	}
	printf("\n");
	if (gl_trialCtr >= 0)
	  gl_remain = 1;
	return 0;
}

/* ROUTINE: nexttrl
**
*/
int nexttrl()
{

//	float x[]={-1*dist*dpc, -1*dist*dpc/sqrt(2), 0, dist*dpc/sqrt(2), dist*dpc, 
//		  dist*dpc/sqrt(2), 0, -1*dist*dpc/sqrt(2)};
//	float y[]={0, dist*dpc/sqrt(2), dist*dpc, dist*dpc/sqrt(2), 0, -1*dist*dpc/sqrt(2),
//		-1*dist*dpc, -1*dist*dpc/sqrt(2)};


	/*float x[]={-1*dist*dpc, dist*dpc,
	 *	  -1*sqrt(3)/2.*dist*dpc, -1*dist*dpc/sqrt(2), -1*dist*dpc/2., 
	 *	  -1*sqrt(3)/2.*dist*dpc, -1*dist*dpc/sqrt(2), -1*dist*dpc/2., 
	 *	  sqrt(3)/2.*dist*dpc, dist*dpc/sqrt(2), dist*dpc/2.,
	 *	  sqrt(3)/2.*dist*dpc, dist*dpc/sqrt(2), dist*dpc/2.,
	 *	  
	 *};
	 * 
	 *float y[]={0, 0,
	 *	  dist*dpc/2., dist*dpc/sqrt(2), sqrt(3)/2.*dist*dpc,
	 *	  -1*dist*dpc/2., -1*dist*dpc/sqrt(2), -1*sqrt(3)/2.*dist*dpc,
	 *	  dist*dpc/2., dist*dpc/sqrt(2), sqrt(3)/2.*dist*dpc,
	 *	  -1*dist*dpc/2., -1*dist*dpc/sqrt(2), -1*sqrt(3)/2.*dist*dpc,
	 *};
	 */
	 open_adata();

	printf("nexttrl\n");
	if (gl_record) {
		if (gl_trialCtr < 0) {
			gl_remain = 0;
			return(0);
		} else {
			gl_targObj.x = (int) round(gl_xPositions[gl_trialIndices[gl_trialCtr]]);
			gl_targObj.y = (int) round(gl_yPositions[gl_trialIndices[gl_trialCtr]]);
			gl_remain = 1;
		}
	} else {
		/* just get the joystick */
		gl_targObj.x = storex;
		gl_targObj.y = storey;
		gl_remain = 1;
	}
	
	ec_send_code(STARTCD);	/* official start of trial ! */
	
	/* Update position to stored joystick position */
	
	gl_fixObjEye.color.R = 255;
	gl_fixObjEye.color.G = 0;
	gl_fixObjEye.color.B = 0;
	
	gl_fixObjEye.x = gl_eyeFixX;
	gl_fixObjEye.y = gl_eyeFixY;
	
	gl_targObj.color.R=255;
	gl_targObj.color.G=255;		
	gl_targObj.color.B=255;
		
	gl_fixObjEye.diameter = gl_eyeFixDiam;
	gl_targObj.diameter = gl_targDiam;
	
	
	/* send the target setup commands */
	mat_targDefine(1, gl_fixObjEye.x, gl_fixObjEye.y, gl_fixObjEye.diameter, &(gl_fixObjEye.color));
	mat_targDefine(2, gl_targObj.x, gl_targObj.y, gl_targObj.diameter, &(gl_targObj.color));

	/* save the parameters */ 

	/* 
	 * TASK IDENTIFIER
	 * 0 is overlap saccade, 1 is overlap reach
	 * 10 is memory saccade, 11 is memory reach 
	 * 
	 */
	EC_TAG2(I_TRIALIDCD,gl_delay+10); // 10 is overlap, 11 is memory guided
	EC_TAG2(I_MONITORDISTCD, VIEW_DIST_CM);
	
	/* eye/hand fixation x, y, diameter */
	EC_TAG1(I_FIXXCD, gl_fixObjEye.x);
	EC_TAG1(I_FIXYCD, gl_fixObjEye.y);
	EC_TAG2(I_EFIXDIAMCD, gl_fixObjEye.diameter);
	
	/* target 1 x, y, diameter */
	EC_TAG1(I_TRG1XCD, gl_targObj.x);
	EC_TAG1(I_TRG1YCD, gl_targObj.y);
	EC_TAG2(I_TRG1DIAMCD, gl_targObj.diameter);
	da_set_2(0, gl_targObj.x, 1, gl_targObj.y);
	return 0;
}

int total(long score)
{
	/* set globals for eye checking */
	gl_eye_flag   = 0;
	gl_reward_flag = 0;

	
	/* Drop the appropriate code */
	if(score == CORRECT) {
		ec_send_code(CORRECTCD);
		EC_TAG1(I_RESPONSE, 1);
		if (gl_record) 
		  gl_trialCtr--;
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
	}
	/* outta */
	return(0);
}

/* ROUTINE: abort_cleanup
**
** called only from abort list
*/
int abort_cleanup(void)
 {
	// turn off reward if it was left on
	printf("abort_cleanup\n");
	dio_off(REW);	 
	 
	timer_pause(100);				/* wait in case of went				*/
	
	end_trial(CANCEL_W);			/* cancel analog window, blank screen */
	
	return(0);	
 }
 
USER_FUNC ufuncs[] = {};

/* Top-level state menu
*/

VLIST state_vl[] = {
	{"NTRIALS", 		        &(gl_ntrials), 	        NP, make_task, ME_AFT, ME_DEC},
	{"nPositions",              &(gl_positions),        NP, NP, 0, ME_DEC},
	{"RF_Radius",               &(gl_ecc), 	            NP, NP, 0, ME_DEC},
    {"RF_Theta",                &(gl_rft), 	            NP, NP, 0, ME_DEC},
	{"Delay", 		            &(gl_delay), 	        NP, NP, 0, ME_DEC},
	{"EyeFixX", 		        &(gl_eyeFixX),          NP, NP, 0, ME_DEC},
	{"EyeFixY", 		        &(gl_eyeFixY),          NP, NP, 0, ME_DEC},
	{"EyeFixDiameter", 		    &(gl_eyeFixDiam),       NP, NP, 0, ME_DEC},
	{"TargDiameter", 		    &(gl_targDiam),         NP, NP, 0, ME_DEC},
    {"RewardFixation", 	        &(gl_reward_fixation), 	NP, NP, 0, ME_DEC},
    {"RECORDING", 		        &(gl_record),           NP, make_task, ME_AFT, ME_DEC},
	{NS}};

/* Help strings */
char no_help[] = "";

MENU umenus[] = {
   {"State_vars", 		&state_vl, 		NP, NP, 0, NP, no_help},
   {NS}
};

RTVAR rtvars[] = {};

/* THE STATE SET 
*/
%%
id 900
restart rinitf
main_set {
status ON
begin	first:
		to firstcd
	firstcd:
		do ec_send_code(HEADCD)
		to setup
	setup: 		/* SETUP THE SCREEN */
		do setup_screen(32, 48, 3, 0)
		to loop on 1 % initScreen_done
        		
 	loop: 		/* START THE LOOP -- loop on # trials */
		time 1000
		to pause on +PSTOP & softswitch
	    to go
	pause:
		do ec_send_code(PAUSECD)
		to go on -PSTOP & softswitch
	go: 			/**** TRIAL !!! ****/
		to donelist on -ONES & gl_remain
		to settrl 
	donelist:	/* done with current set... wait for "gl_remain" to update */
		do ec_send_code(LISTDONECD)
		to loop on +ONES & gl_remain
	settrl:		/* set up current trial */
		do nexttrl()
		to task on +ONES & gl_remain
		to loop
	task:			/**** TASK BRANCHING ... ****/
		to teye_start 

	/* position window, wait for fixation */
	fixeyewinpos:
	    do position_eyewindow(10, 10, 0)
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
	    to fixeyedone
		
	/* Done with fixating stuff */
	fixeyedone:
	    do position_eyewindow(25, 25, 3)
		to teye_targ_wait 

/*** CHAIN FOR TASK EYE
***/
	teye_start:
	    do setup_eyewindows(25,25)
		to teye_fp
	teye_fp:
		do drawTarg(1, 0)
		to teye_fp_cd on MAT_WENT % drawTarg_done
	  	/* to teye_fp_cd on PHOTO_DETECT_UP % dio_check_photodetector */
	teye_fp_cd:
		do ec_send_code(FPONCD)
		to fixeyewinpos
	teye_targ_wait:
		do timer_set1_shell(1000,200,600,100,0,0)
		to teye_targ on +MET % timer_check1
	teye_targ: /* show the targets */
		do drawTarg(1, 1)
		to teye_targ_cd on MAT_WENT % drawTarg_done
	  	/* to teye_targ_cd on PHOTO_DETECT_DOWN % dio_check_photodetector */
	teye_targ_cd:
		do ec_send_code(TARGC1CD)
	    to teye_activate_reward_loop on 1 = gl_reward_fixation
	    to teye_removeTargWait on 1 = gl_delay	 
	    to teye_waitfpoff
	teye_activate_reward_loop:
		do set_reward_flag(REWARD_LOOP_ON)
	    to teye_removeTargWait on 1 = gl_delay /* memory or overlap saccade */
	    to teye_waitfpoff
	teye_removeTargWait:
	    do timer_set1_shell(0,0,0,0,200,0)
	    to teye_removeTarg on +MET % timer_check1
	teye_removeTarg:
	    do defTargLum(1, 0)
	    //do drawTarg(1, 0)
		to teye_removeTarg_cd on MAT_WENT % drawTarg_done 
	  	/* to teye_removeTarg_cd on PHOTO_DETECT_UP % dio_check_photodetector */
	teye_removeTarg_cd:
	    do ec_send_code(TARGC1OFFCD)
	    to teye_waitfpoff
	  
	/* FP OFF */
	teye_waitfpoff:
        do timer_set1_shell(1000,200,1500,1000,0,0)	
		to teye_fpoffBranch on +MET % timer_check1
	teye_fpoffBranch:
	    to teye_fpoffOverlap on 0 = gl_delay
	    to teye_fpoffMemory on 1 = gl_delay
	teye_fpoffOverlap: /* turn the FP off */
		do drawTarg(0,1)
		to teye_fpoff_cd on MAT_WENT % drawTarg_done 
	    /* to teye_fpoff_cd on PHOTO_DETECT_UP % dio_check_photodetector */
	teye_fpoffMemory: /* turn the FP off */
		do drawTarg(0,0)
		to teye_fpoff_cd on MAT_WENT % drawTarg_done
	  	/* to teye_fpoff_cd on PHOTO_DETECT_DOWN % dio_check_photodetector */
	teye_fpoff_cd:
		do ec_send_code(FPOFFCD)
		to teye_deactivate_reward_loop
	teye_deactivate_reward_loop:
		do set_reward_flag(REWARD_LOOP_OFF)
		to teye_grace	/* wait for sac */
	
	/* grace period in which monsieur le monk has to 
	** break fixation and start the saccade
	*/
	teye_grace:
		do set_eye_flag(E_OFF)
		to teye_gracea
	teye_gracea:
		time 2000
		to teye_saccd on +WD0_XY & eyeflag
		to wcshow
	teye_saccd:
		do ec_send_code(SACMADCD)
		to check_eye_response
		
	/* end trial checking, can only be right or wrong */
	check_eye_response:
		time 50
	    to peyehold on -WD1_XY & eyeflag /* got it!	*/
		to wcshow
	peyehold: 
		do ec_send_code(TRGACQUIRECD)
		time 50   /* gotta hold for this long	*/
		to wcshow on +WD1_XY & eyeflag
		to pref


	/* visual feedback if not successful */
	wcshow: /* show the targets */		
		do drawTarg(0, 1)
		to wcwait on MAT_WENT % drawTarg_done
	wcwait:
		time 0
		to wcerr  
	
	/* NO CHOICE: didn't complete the task */
	wcerr:
		do total(NCERR) 
		to wcend
	wcend:
		do end_trial(CLOSE_W)
		time 1000
		to loop

	/* PREF: update the totals and give a reward
	*/
	pref:
		do total(CORRECT)
		to prend
	prend:
		do end_trial(CLOSE_W)
		to prrew 
	prrew:
		do give_reward(REW, 150)
		to prrew_off on +MET % timer_check1
	prrew_off:
		do dio_off(REW)
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

reward_loop {
status ON
begin   rfirst:
	    to r_rtest
	r_rtest:
	    to r_prrew on REWARD_LOOP_ON = gl_reward_flag 
	r_prrew:
	    time 100
		do dio_on(REW)
		to r_prrew_off
	r_prrew_off:
	    time 100
		do dio_off(REW)
		to r_rtest 

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

//check_touch {
//status ON
//begin cfirst:
//	  	to cprint
//   cprint:
//   		time 500
//	    do printtouch()
//       to cprintagain
//   cprintagain:
//        time 500
//        do printtouch()
//		to cprint

//abort list:
//} 


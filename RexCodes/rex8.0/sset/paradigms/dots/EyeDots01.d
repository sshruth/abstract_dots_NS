#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <hw/inout.h>

#include "ldev.h"
#include "../labTools/lcode.h"
#include "../labTools/toys.h"
#include "../labTools/matlab.h"
#include "../labTools/timer.h"
#include "../labTools/vs_dots.h"
#include "../labTools/udpmsg.h"
#include "../labTools/dio_lab.h"

/* signal numbers for eyes */
#define EYEH_SIG   0	   
#define EYEV_SIG   1

/* signal numbers for joy */
#define JOYH_SIG   6
#define JOYY_SIG   7

#define OTHERH_SIG 4
#define OTHERV_SIG 5

/* for gl_eye_flag */
#define E_OFF	0
#define E_FIX	1 

/** windows for eyeflag and objects **/
#define WIND0	    0       /* will be used to enforce fixation */
#define WIND1	    1	    /* ts */
#define WIND2	    2	    /* dot patch */
#define WIND3       3       /* correct response window */
#define WIND4       4       /* incorrect response window */
#define WIND5       5       /* sure bet */
#define WIND7	    7

/*  calculate actual position from voltages from joystick. 
** In Rex's standard, every 40 steps of an analog signal corresponds to 1 degree  
** multiply by 0.25 to get to a resolution of 10 steps per degree. 
*/
#define CALC_JOY(x)	((int) (0.25 * (double) (x)))

#define NUM_TARGETS			4

/* tasks in this paradigm */
#define NUM_TASKS			1
#define TASK_EYE			0

/* PRIVATE data structures */

typedef struct menu_info_struct *menu_info;
struct menu_info_struct {
	int 	joystick;
	int	repetitions;
	int     seed;
};

typedef struct rtvar_struct *rtvar;
struct rtvar_struct {
	int total_trials;
	int total_correct;
	int num_completed;
	int num_correct;
	int num_wrong;
	int num_sbet;	/* ts */
	int num_ncerr;
	int num_brfix;
	int num_brhandfix;
	int last_score;
	int coherence;
	int pulse_time;
	int duration;
	int delay;
	int direction;
	int rt;
	int joy_x_abs;
	int joy_y_abs;
	int joy_x;
	int joy_y;
	int joy_ecc;
	int joy_ang;
};



/* GLOBAL VARIABLES */

//static int gl_coh_list[] = {32, 64, 91, 128, 256, 512, 756};
static int gl_coh_list[] = {0, 32, 64, 128, 256, 512, 756};
//static int gl_coh_list[] = {64, 128, 192, 256, 384, 512, 756};
//static int gl_coh_list[] = {32, 64, 91, 128, 181, 256, 362};
static int gl_max_coh_num = sizeof(gl_coh_list)/sizeof(int);

static _VSrecord 						gl_vsd=NULL;                       /* the big kahuna. see vs* */
static struct menu_info_struct 			gl_menu_infoS;
static struct _VSDtask_info_struct 		gl_task_infoS[NUM_TASKS];
static struct _VSDdot_object_struct	    gl_dotS[2];
static struct _VSobject_struct 			gl_object_teyeS[VSD_NUM_OBJECTS];	/* task eye 	*/
static struct _VSobject_struct 			gl_object_thandS[VSD_NUM_OBJECTS];	/* task hand 	*/

struct rtvar_struct	            gl_rtvar;
int 				gl_task;
int 							gl_remain;
int 							gl_eye_flag = 0;
int 							gl_hand_flag = 0;
int 							gl_dots_flag;
int							    gl_prize_count;
int							    gl_prize_max = 0;
int 							gl_rt_flag = 0; /*Is current trial an RT task?*/
long							gl_ref_time = 0; /* reference time for the RT task */
long							gl_resp_time = 0; /* (absolute) response time for the RT task */
int							    gl_teye_perf_cor[7]; /* performance counters */
int							    gl_teye_perf_sbet[7];
int							    gl_teye_perf_tot[7];
long							gl_teye_rt_cor_sum[7];
int							    gl_teye_rt_cor_n[7];
long							gl_teye_rt_err_sum[7];
int							    gl_teye_rt_err_n[7];
long							gl_teye_rt_sbet_sum[7];
int							    gl_teye_rt_sbet_n[7];
int							    gl_sbet_shown = 0;		/* ts */
int                             gl_elestim_flag = 0;	/* electrical stimulation */


int gl_correct_side;
int storex, storey;

/* ROUTINES */

/*
***** INITIALIZATION routines
*/

/* ROUTINE: autoinit
**
** initialization of task infos
** executed automatically when running the paradigm
*/
void autoinit(void) 
 { 
   int i;
   
   for (i=0; i<NUM_TASKS; i++)
       vsd_init_task_info(&gl_task_infoS[i]);
}


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
	 
//	 udp_close();
//	 udp_open(rexIP, udpPORT);
	 
	 /* initialize interface (window) parameters */
	 wd_cntrl 		(WIND0, WD_ON);
	 wd_src_check 	(WIND0, WD_SIGNAL, EYEH_SIG, WD_SIGNAL, EYEV_SIG);
	 wd_src_pos 	(WIND0, WD_DIRPOS, 0, WD_DIRPOS, 0);
	 
	 /* static window for screen limits */
#ifdef SCR_LIM_HOR /* defined in ldev.h */
	 wd_cntrl	 	(WIND7, WD_ON);
	 wd_src_pos 	(WIND7, WD_DIRPOS, 0, WD_DIRPOS, 0);
	 wd_pos 		(WIND7,	 0, 0);
	 wd_siz	        (WIND7, SCR_LIM_HOR, SCR_LIM_VER); /* from ldev.h! */
#endif
	 
	 /* WIND1 is used to mark the appearance of the sure bet target */
	 wd_cntrl	 		(WIND1, WD_ON);
	 wd_src_pos 		(WIND1, WD_DIRPOS, 0, WD_DIRPOS, 0);
	 wd_pos 			(WIND1, 0, 0);
	 wd_siz 			(WIND1, 0, 0);
	 
	 /* WIND2 is used to mark the appearance of the dots patch */
	 wd_cntrl 			(WIND2, WD_ON);
	 wd_src_pos 		(WIND2, WD_DIRPOS, 0, WD_DIRPOS, 0);
	 wd_pos 			(WIND2, 0, 0);
	 
	 /* WIND3 will be checking correct response */
	 wd_cntrl 		(WIND3, WD_ON);
	 wd_src_check 	(WIND3, WD_SIGNAL, EYEH_SIG, WD_SIGNAL, EYEV_SIG);
	 wd_src_pos 	(WIND3, WD_DIRPOS, 0, WD_DIRPOS, 0);
	 
	 /* WIND4 will be checking incorrect response */
	 wd_cntrl 		(WIND4, WD_ON);
	 wd_src_check 	(WIND4, WD_SIGNAL, EYEH_SIG, WD_SIGNAL, EYEV_SIG);
	 wd_src_pos 	(WIND4, WD_DIRPOS, 0, WD_DIRPOS, 0);
	 
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
int setup_screen(long mon_width_cm, long view_dist_cm, long num_targets, long num_dots_patch, long long repeat_flag)
 {
   static int first_time = 1;
   
   	/* Do these only if first time through or if explicitly
   	** told to do each time... enable pcmsg & init the screen
   	*/
   if(repeat_flag || first_time) {
       first_time = 0;
       
       	/* Added here so reset states doesn't necessarily clear gl_vsd */
       make_tasks();
       
       	/* init the screen */
       printf("\n");
       printf("Initializing screen with values from state list (root file):\n");
       mat_initScreen(mon_width_cm, view_dist_cm, num_targets, num_dots_patch);
   }
   
   printf("\n");
   printf("Horizontal screen size: %d cm\n", mon_width_cm);
   printf("Viewing distance: %d cm\n", view_dist_cm);
   printf("\n");
   
   return 0; 
 }



/* ROUTINE: make_tasks
**
**	function to determine TASKS in current record following update of
**	count
*/
int make_tasks(void)
{
	int i;
	
	/* make sure the VSD REC exist and are clear */
	if(!gl_vsd) {
		gl_vsd					= vsd_init_record();
		gl_rtvar.total_correct 	= 0;
	} else {
		vsd_clear_record(gl_vsd);
	}
	
	/* Luke was here, seed? */
	gl_vsd->seed = (unsigned int) gl_menu_infoS.seed;
	
	/* initialize performance counters */
	for (i=0; i<7; i++)	{
		gl_teye_perf_cor[i]=0;
		gl_teye_perf_sbet[i]=0;	/* ts */
		gl_teye_perf_tot[i]=0;
		gl_teye_rt_cor_sum[i]=0;
		gl_teye_rt_cor_n[i]=0;
		gl_teye_rt_err_sum[i]=0;
		gl_teye_rt_err_n[i]=0;
		gl_teye_rt_sbet_sum[i]=0;	/* ts */
		gl_teye_rt_sbet_n[i]=0;
	}

	/* initialize the tasks */
	for ( i=0 ; i < NUM_TASKS ; i++ ) {
	  vsd_add_task(gl_max_coh_num, gl_coh_list, gl_vsd, &(gl_task_infoS[i]), i);
	}
	/* set the counters */
	gl_vsd->num_repetitions 	= gl_menu_infoS.repetitions;
	gl_remain 					= 1; /* dummy before we can really calculate it */
    
	/* FOR DEBUGGING */
	vsd_print_record(gl_vsd);
	
	return 0;
}


/****
***** VISUAL STIMULUS (MATLAB) routines
****/

 
/*  ROUTINE: defTargLum
** changes the defined luminance of targets, no change is made on the screen until drawTarg is called
** the routine scales the original colors defined in the task menu. the original color is assigned a luminance
** of 1000. balck has a luminance of zero. 
*/ 
int defTargLum(long lum1, long lum2, long lum3, long lum4)
 {
   long lum[4] = {lum1, lum2, lum3, lum4};		//currently we have only 4 valid targets 
   RGB color;
   _VSobject obj_v;
   int i;
   
   for(i=0; i<5; i++)	{
       switch(i)	{
           case 0:
               obj_v = VSD_GET_FP(gl_vsd);    break;
           case 1:
               obj_v = VSD_GET_T1(gl_vsd);    break;
           case 2:
               obj_v = VSD_GET_T2(gl_vsd);	    break;
           case 3:
               obj_v = VSD_GET_TS(gl_vsd);	    break;
       }
       if (lum[i]>=0)	 {
           color.R = lum[i] * obj_v->color.R / 1000;
           color.G = lum[i] * obj_v->color.G / 1000;
           color.B = lum[i] * obj_v->color.B / 1000;
           mat_targDefine(i+1, obj_v->x, obj_v->y, obj_v->diameter, &color);
       }
   }
   
   return 0;
 }

 
 
/*  ROUTINE: drawTarg
** 
** prob1-5 refer to FP, T1, T2, TCERT0 and TCERT1 if tflag is zero
** if tflag is 1, prob2 and prob3 will refer to correct and wrong targets, respectively
** 
*/
int drawTarg(long prob1, long prob2, long prob3, long prob4, long tflag)
 {
   long prob[4] = {prob1, prob2, prob3, prob4};
   int num_show=0, num_hide=0;
   int show_ind[VSD_NUM_OBJECTS], hide_ind[VSD_NUM_OBJECTS];
   _VSobject obj_v;
   int i, tind;
   
   for(i=0; i<4; i++)	{
       if(i==0)
           tind = (VSD_GET_FP(gl_vsd))->matlab_index;
       else if(i==1)
           tind = (tflag==1) ? (VSD_GET_TC(gl_vsd))->matlab_index : (VSD_GET_T1(gl_vsd))->matlab_index;
       else if(i==2)
           tind = (tflag==1) ? (VSD_GET_TW(gl_vsd))->matlab_index : (VSD_GET_T2(gl_vsd))->matlab_index;
       else if(i==3)
           tind = (VSD_GET_TS(gl_vsd))->matlab_index;
       else
           tind = i;
       
       if (prob[i]>0 && TOY_RAND(1000.0)<(int)prob[i]) 
           show_ind[num_show++] = tind;
       else if (prob[i]>=0)
           hide_ind[num_hide++] = tind;
   }
   
   mat_targDraw(num_show, show_ind, num_hide, hide_ind);
   
	/* set the da markers. Remember that by default:
	** 	CU_DATA4 (octagon) is connected to DA channels 0 (x) and 1 (y)
	** 	CU_DATA5 (X)       is connected to DA channels 2 (x) and 3 (y)	
	*/
   obj_v = VSD_GET_TC(gl_vsd);
   da_set_2(0, obj_v->x, 1, obj_v->y);
   obj_v = VSD_GET_TW(gl_vsd);
   da_set_2(2, obj_v->x, 3, obj_v->y);
   
   return 0;
 }


/* ROUTINE: showDots
**
**	Note: starts not just dots, but cases on whether
**		we're showing the dots or just changing the fixation
**		point color
*/
int showDots(void)
 {
   	/* showing dots */
   if(SHOW_DOTS(gl_vsd)) {
       _VSDdot_object dot = VSD_GET_DTOBJECT(gl_vsd);
       
       	/* start the dots and mark it */
       mat_dotsShow(dot->seed_base, dot->seed_var, 5000);
       gl_dots_flag = -1;
       		
       	/* update real-time variables */
       gl_rtvar.coherence = dot->coherence;
       gl_rtvar.direction = dot->direction;
       
       	/* show dots patch on the Window Display */
       wd_pos ( WIND2, (VSD_GET_AP(gl_vsd))->x, (VSD_GET_AP(gl_vsd))->y);
       wd_siz ( WIND2, (VSD_GET_AP(gl_vsd))->diameter/2, (VSD_GET_AP(gl_vsd))->diameter/2);
       wd_cntrl ( WIND2, WD_ON );
   }
   
   return(0);
 }


/* ROUTINE: stopDots
**
*/
int stopDots(void)
 {
	/* dots are on .. call matlab routine to stop 'em */
   if(gl_dots_flag == -1) {	/* dots are on */
       mat_dotsStop();
       
	   wd_siz ( WIND2, 0, 0 );
	   wd_cntrl ( WIND2, WD_ON );
   }
   	
   gl_dots_flag = 0;
   return(0);
 }


int initScreen_done()
 { 
   return mat_getWent(MAT_INIT_SCREEN_CMD, IS_EXECUTED); 
 }
 
int drawTarg_done()
 {
   //printf("time = %d\n", (int) i_b->i_time);
   //printf("matdone = %d\n", mat_getWent(MAT_TARG_DRAW_CMD, IS_EXECUTED));
   return mat_getWent(MAT_TARG_DRAW_CMD, IS_EXECUTED);
 }
 
int showDots_done()
 {
   return mat_getWent(MAT_DOTS_SHOW_CMD, IS_EXECUTED);
 }
 
int stopDots_done()
 {
   return mat_getWent(MAT_DOTS_STOP_CMD, IS_EXECUTED);
 } 

/****
***** UTILITY routines
*/


/* ROUTINE position_eyewindow
**
** sets window location
*/
int position_eyewindow(long wd_width, long wd_height, long flag)
{
	_VSobject vso;
	
	/* position window @ fix point */
	if(flag == 0) {
		vso = VSD_GET_FP(gl_vsd);
		wd_pos(WIND0, vso->x, vso->y);
		EC_TAG2(I_EFIX_ACCEPTHX, wd_width);
		EC_TAG2(I_EFIX_ACCEPTHY, wd_height);		
		/* position window @ correct target */
	} else if(flag == 1) {
		vso = VSD_GET_TC(gl_vsd);
		wd_pos(WIND0, vso->x, vso->y);
		/* position window @ incorrect target */
	} else if(flag == 2) {
		vso = VSD_GET_TW(gl_vsd);
		wd_pos(WIND0, vso->x, vso->y);
		/* position window @ eye position */
	} else if(flag == 3) {
		wd_pos(WIND0, (long)(eyeh/4), (long)(eyev/4));
		/* position window @ sure target */ /* ts */
	} else if(flag == 4) {
		vso = VSD_GET_TS(gl_vsd);
		wd_pos(WIND0, vso->x, vso->y);
	}
	
	wd_siz(WIND0, wd_width, wd_height);
	wd_cntrl(WIND0, WD_ON);
	
	return(0);
}

int setup_eyewindows(long wd_width, long wd_height)
{
	_VSobject vso;
	
	vso = VSD_GET_TC(gl_vsd);
	wd_pos(WIND3, vso->x, vso->y);
	wd_siz(WIND3, wd_width, wd_height);
	wd_cntrl(WIND3, WD_ON);

	if (vso->x < 0) 
		gl_correct_side = -1;
	else
		gl_correct_side = 1;

	vso = VSD_GET_TW(gl_vsd);
	wd_pos(WIND4, vso->x, vso->y);
	wd_siz(WIND4, wd_width, wd_height);
	wd_cntrl(WIND4, WD_ON);

	
	EC_TAG2(I_ETARG_ACCEPTHX, wd_width);
	EC_TAG2(I_ETARG_ACCEPTHY, wd_height);
}

/* ROUTINE: open_adata
**
** Opens analog data
*/
int open_adata(void)
{
	
	//pl_start_record_();
	//awind(OPEN_W);
	return(0);
}



/* ROUTINE: end_trial
**
*/
int end_trial(long aflag)
{ 
	/* turn eye position window off */
	wd_cntrl(WIND0, WD_OFF); 	
	wd_cntrl(WIND1, WD_OFF);	/* ts */
	wd_cntrl(WIND2, WD_OFF);	/* dots patch */
	wd_cntrl(WIND3, WD_OFF);	/* correct response */
	wd_cntrl(WIND4, WD_OFF);	/* incorrect response */ 
	
	/* blank the screen */
	mat_allOff();
	
	ec_send_code(LASTCD);		   
	
	/* close the analog data window */
	//pl_stop_record_();
	//awind(aflag);

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
	return(0);;
}
 

/* ROUTINE elestim_on
** 
**	turn on electrical stimulation
*/
int elestim_on(long dio1, long dio2, long prob)
 {
  if (TOY_RAND(1000)<prob)
   { dio_off(dio1);		/* put in stim mode */
	 dio_on(dio2);		/* stim trigger on */
  	 gl_elestim_flag = 1;
	 ec_send_code(ELESTM);
     ec_send_dio(dio1);
     EC_TAG2(I_ELESTMCD, 1); }
  else
   { /* EC_TAG2(I_ELESTMCD, 0); */
	 gl_elestim_flag = 0; }

  return 0;
 }


/* ROUTINE elestim_off
**
** turn off electrical stimulation
*/
int elestim_off(long dio1, long dio2, long msgflag)
 {
  dio_off(dio2);
  dio_on(dio1);

  if (msgflag)
   EC_TAG2(I_ELESTMSTCD, 1);

  return 0;
 }


/* ROUTINE: print_status
**
** 	Probably should use PF_print_all
*/
void print_status(void)
 {
	printf("status\n");
 }

/* ROUTINE: print_experiment_info
**
*/
void print_experiment_info(void)
 {
	register int i;

	printf("\n");
	printf("------------------------------------------------------------\n");
	
		printf("Number of repetitions      = %d\n", gl_vsd->num_repetitions);
		printf("Number of unique trials    = %d\n", gl_vsd->num_trials);
		printf("Number of total trials     = %d\n", gl_vsd->num_trials * gl_vsd->num_repetitions);	

	printf("Number of remaining trials = %d\n", gl_remain);
	printf("-TASK 1-----------------------------------------------------\n");
	printf("\t\t 0.0\t 3.2\t 6.4\t12.8\t25.6\t51.2\t99.9\n");
	printf("Performance");

	for (i=0;i<7;i++)
		if (gl_teye_perf_tot[i])
			printf("\t %3d",(int)(gl_teye_perf_cor[i]/(gl_teye_perf_tot[i]/100.0)+.5));
		else
			printf("\t ---");

	printf("\n");
	printf("Sure-bet");		/* ts */

	for (i=0;i<7;i++)
		if (gl_teye_perf_tot[i])
			printf("\t %3d",(int)(gl_teye_perf_sbet[i]/(gl_teye_perf_tot[i]/100.0)+.5));
		else
			printf("\t ---");
	
	printf("\n");
	printf("Correct RT");

	for (i=0;i<7;i++)
		if (gl_teye_rt_cor_n[i])
			printf("\t%4d",(int)((float)gl_teye_rt_cor_sum[i]/gl_teye_rt_cor_n[i]+.5));
		else
			printf("\t----");

	printf("\n");
	printf("Error RT");

	for (i=0;i<7;i++)
		if (gl_teye_rt_err_n[i])
			printf("\t%4d",(int)((float)gl_teye_rt_err_sum[i]/gl_teye_rt_err_n[i]+.5));
		else
			printf("\t----");
	
	printf("\n");
	printf("Sure-bet RT");		/* ts */
	
	for (i=0;i<7;i++)
		if (gl_teye_rt_sbet_n[i])
			printf("\t%4d",(int)((float)gl_teye_rt_sbet_sum[i]/gl_teye_rt_sbet_n[i]+.5));
		else
			printf("\t----");
	
 }


/* ROUTINE: get_ref_time
*/
int get_ref_time(void)
 {
	gl_ref_time=i_b->i_time;
	return 0;
 }

/* ROUTINE: get_resp_time
*/
int get_resp_time(void)
 {
	gl_resp_time=i_b->i_time;

	return 0;
 }

/* ROUTINE: set_dots_duration
**
** Wrapper that allows us to set the global "duration"
** to show as a real-time variable
*/
int set_dots_duration(long a, long b, long c, long d, long e, long f)
 {
	gl_rtvar.duration = timer_set1(a,b,c,d,e,f);
	EC_TAG2(I_MAXDOTSDUR, e/10); // in ms/10, so units digit will be rounded
	return(0);
 }

/*ROUTINE: set_delay_duration
**
** Wrapper that allows us to set the global "delay"
** to show as a real-time variable
*/
int set_delay_duration(long a, long b, long c, long d, long e, long f)
 {
	gl_rtvar.delay = timer_set1(a,b,c,d,e,f);
	return(0);
 }


int set_delay_rtvar (long a)
 {
	gl_rtvar.delay = a;
  	return(0);
 }


/* ROUTINE: set_reward_delay_timer (guarantee a minimum time between dot onset and reward)
*/
int set_reward_delay(long min_time)
 {
	int set_timer_to = 0;
	//long cur_time = i_b->i_time;

	//if ((cur_time-gl_ref_time)<min_time) /* extra delay necessary? */
	//	set_timer_to=min_time-(cur_time-gl_ref_time);

	/* Luke was here, based on actual RT time */ 
	if ((gl_resp_time - gl_ref_time)<min_time) /* extra delay necessary? */
		set_timer_to=min_time-(gl_resp_time-gl_ref_time); 

	timer_set1(0,0,0,0,set_timer_to,0);

	return 0;
 }	


/* ROUTINE: set_punishment_timer (timeout is an exponential function of RT)
            timeout = max_time * exp (-RT/scaling)
			if timeout > cutoff -> timeout = cutoff
			timeout = non_rt for non-RT trials
*/
int set_punishment_timer(long max_time, long scaling, long cutoff, long non_rt)
 {
   int temp;
   
   if (scaling==0)
       scaling=1000; /* prevent division by zero */
   
   if (!gl_rt_flag)
       temp=non_rt; /* non-RT trial */
   else
       temp=min(cutoff, max_time*exp(-(gl_resp_time-gl_ref_time)/(float)scaling));
   
   timer_set1(0,0,0,0,temp,0);
   
   return 0;
 }
 
 
/* ROUTNE: give_reward
** 
** description: activates the water reward system
*/
int give_reward(long dio, long duration, long who_calling)
 {	
   static int gl_rew_size_cor = 0;	/* size of the reward */
   static int gl_rew_size_sbet = 0;
   
   if (gl_correct_side == -1)
	   duration = duration;

   dio_on(dio);
   timer_set1(0,0,0,0,duration,0);
   
   switch ( who_calling ) {
       case CORRECT:
           gl_rew_size_cor = duration;
           break;
       case SBET:
           gl_rew_size_sbet = duration;
           break;
   }	
   
   	/* save the reward size only once on each trial */ 
   if (gl_prize_count==0) {
       EC_TAG2(I_REWSIZE_COR, gl_rew_size_cor);
       EC_TAG2(I_REWSIZE_SBET, gl_rew_size_sbet);
       printf("--reward. cor=%d, sbet=%d\n", gl_rew_size_cor, gl_rew_size_sbet);
   }
   
   return 0;
 }
 

/* ROUTUNE: update_prize_count
**
** update gl_prize_count based on the score (CORRECT, WRONG, NCERR, BRFIX or BRHANDFIX) of
** the current trial and previous trials
** prizes are additional reward drops that are given to the monkey for doing 
** several trials correct in a row
** <gl_prize_count> is a global variable that is updated by  <update_prize_count>
** at the end of each trial. 
** we want to increase the number of prizes when monkey makes correct responses
** in a row without making mistakes
** don't worry about the maximum number of prizes, it will be managed by 
** <set_max_prize_count>
** 
*/ 
void update_prize_count ( long score )
 {
	static int prev_score = WRONG;
	static int prize_count = 0;
	
	if ( score == CORRECT )	{
		if ( prev_score==CORRECT )		
			prize_count++;
		prev_score = CORRECT;
	} else if ( score==WRONG || score==NCERR )	{
		prize_count = 0;
		prev_score = WRONG;
	}
	gl_prize_count = min(prize_count, gl_prize_max);
//	dprintf("prize count: %d, max: %d\n", gl_prize_count, gl_prize_max); 
 }



/* ROUNTINE: randsample
**
** randomly samples from an array of numbers. the array is defined by 4 variables
** 	base	first element of the array
**  step	distance of consecutive elements in the array (on a linear or log scale)
**  n		number of elements in the array
**  flag	"log" or "linear", defines the scaling of elements
*/
double randsample(double base, double step, int n, char *flag)
 {
  double ret;
  
  if (stricmp(flag,"log")==0)
	ret = base * exp(log(step)*(double)TOY_RAND(n));
  
  if (stricmp(flag,"linear")==0)
    ret = base+step*(double)TOY_RAND(n);
  
  return(ret);
 }


/* ROUTINE show_sbet
**
** shows the sure bet target the probability by prob
*/
int show_sbet(long prob)		/* ts */
 {
  	if ( prob > 0 && TOY_RAND(1000.0) < (int) prob ) {
		wd_pos ( WIND1, VSD_GET_TS(gl_vsd)->x, VSD_GET_TS(gl_vsd)->y);
		wd_siz ( WIND1, VSD_GET_TS(gl_vsd)->diameter/2, VSD_GET_TS(gl_vsd)->diameter/2);
		drawTarg(-1,-1,-1,1000,0);		
		gl_sbet_shown = 1;		
  	} else {
		wd_siz ( WIND1, 0, 0 );
		gl_sbet_shown = 0;
	}
	wd_cntrl ( WIND1, WD_ON );
  	
  	return(0);
 }


int dropcode_sbet(void)
 {
	if (gl_sbet_shown)
		ec_send_code(SBETSHOWCD);
	
	return(0);
 }


/* ROUTINE: nexttrl
**
**	Arguments:
**		do_over 	   ... if >0, need to get the current trial correct
**								this many times before proceeding to next trial
**		min_block_size ... if non-staircase, min # of trials per block
**								(see vs_get_next_trial)
**		randomize_flag ... flag sent to vs_get_next_trial
*/
int nexttrl (long do_over, long min_block_size, long randomize_flag)
{
	static int num_correct = 0;
	int i, j;
	_VStrial trial = NULL;
	int corrtarg;
	
	gl_sbet_shown = 0;		/* ts */
	wd_siz ( WIND1, 0, 0 ); /* ts */
	wd_cntrl ( WIND1, WD_ON );
	
	
	if ( !trial ) {
		/* check "do_over" flag to see if we should just redo last trial */
		if(do_over > 0) {
			if((gl_vsd->last == CORRECT) && (++num_correct >= do_over))
			  num_correct = 0;
			else
			  gl_vsd->last = DO_OVER;
		}
		
		/* vs_get_next_trial is the real workhorse.. makes
		 ** trial_array in gl_vsd if necessary, randomizes it,
		 ** & selects the next trial
		 */
		trial = vs_get_next_trial(gl_vsd, min_block_size, randomize_flag);
	}
    
	/* make sure we have a trial and are continuing... */
	if(!trial) {
		gl_remain = 0;
		return(0);
	}
	
	
	 /* if so, open the data window... we're doing it
	 ** here because it toggles Plexon data collection,
	 ** which includes ecodes that we'll drop now
	 */
	open_adata();
	ec_send_code(STARTCD);	/* official start of trial ! */
	
	/* set some globals */
	gl_task = trial->task->id;
	gl_remain = gl_vsd->num_trials;
	gl_rt_flag = gl_task_infoS[gl_task].rt_flag;
	
	/* 
	 ** Clear all the old objects,then call the 
	 ** big kahuna to update the display.
	 ** This is where the sun/planet weirdness is actually
	 ** converted into x,y coordinates for all of the screen
	 ** objects..
	 */
	vsd_clear_display(gl_vsd->display);
	
	/* copy the appropriate gl_object_t*S struct into the gl_vsd display */
	if(gl_task == TASK_EYE) {
		for(i=0;i<VSD_NUM_OBJECTS;i++)
		  vs_copy_object(&(gl_object_teyeS[i]), gl_vsd->display->object_array[i]);
		vsd_copy_dot_object(&gl_dotS[0], VSD_GET_DTOBJECT(gl_vsd));
	} 
	
	/* Luke was here to modify how joystick is used to update positions of T1 and T2 */
	i = gl_menu_infoS.joystick;
	if( i>=VSD_OBJECT_FP && i<=VSD_OBJECT_DT) {
		gl_vsd->display->object_array[i]->angle = NULLI;
		gl_vsd->display->object_array[i]->wrt   = NULLI;

		if (i == VSD_OBJECT_T1) {

			j = VSD_OBJECT_T2;

			
			if (gl_vsd->display->object_array[i]->x * storex > 0) { /* in the same hemifield */
				gl_vsd->display->object_array[i]->x = storex;
				gl_vsd->display->object_array[i]->y = storey;
			} else if (gl_vsd->display->object_array[i]->x * storex < 0) { /* in opp hemifields */
				gl_vsd->display->object_array[i]->x = storex;
				gl_vsd->display->object_array[i]->y = storey;
			}
			

			/* Now flip about the vertical axis the position of T2 */
			gl_vsd->display->object_array[j]->x  = -1 * gl_vsd->display->object_array[i]->x;
			gl_vsd->display->object_array[j]->y = gl_vsd->display->object_array[i]->y; 
			gl_vsd->display->object_array[j]->angle = NULLI;
			gl_vsd->display->object_array[j]->wrt   = NULLI;
		} else {
			gl_vsd->display->object_array[i]->x = storex;
			gl_vsd->display->object_array[i]->y = storey;
		}
	}
	
	/* call update display to parse the object info */
	vsd_update_display(gl_vsd, 0); /* Zero indicates no reverse dot-motion/target association */
	/* send the dots setup command if necessary */

	if(SHOW_DOTS(gl_vsd))	{
		/* define the dots patch */
		mat_dotsDefine(1, (VSD_GET_AP(gl_vsd))->x, (VSD_GET_AP(gl_vsd))->y, (VSD_GET_AP(gl_vsd))->diameter,
					   (VSD_GET_DTOBJECT(gl_vsd))->direction, (VSD_GET_DTOBJECT(gl_vsd))->coherence, (VSD_GET_DTOBJECT(gl_vsd))->speed);
	}
	
	/* send the target setup commands */
	mat_targDefine(1, (VSD_GET_FP(gl_vsd))->x, (VSD_GET_FP(gl_vsd))->y, (VSD_GET_FP(gl_vsd))->diameter, &((VSD_GET_FP(gl_vsd))->color));
	mat_targDefine(2, (VSD_GET_T1(gl_vsd))->x, (VSD_GET_T1(gl_vsd))->y, (VSD_GET_T1(gl_vsd))->diameter, &((VSD_GET_T1(gl_vsd))->color));
	mat_targDefine(3, (VSD_GET_T2(gl_vsd))->x, (VSD_GET_T2(gl_vsd))->y, (VSD_GET_T2(gl_vsd))->diameter, &((VSD_GET_T2(gl_vsd))->color));
	mat_targDefine(4, (VSD_GET_TS(gl_vsd))->x, (VSD_GET_TS(gl_vsd))->y, (VSD_GET_TS(gl_vsd))->diameter, &((VSD_GET_TS(gl_vsd))->color));
	
	
	/*
	 ** Also set gl_dots_flag... kinda ugly:
	 **		--  0 if showing nothing
	 **		-- -2 if showing either DOTS
	 **		(later we'll set it to -1 while dots are actually showing)
	 */
	if(SHOW_DOTS(gl_vsd))
	  gl_dots_flag = -2;
	else
	  gl_dots_flag = 0;
	
	/* FOR DEBUGGING
	 **
	 */
	vsd_print_record(gl_vsd);

	
	/* save the parameters */ 
	/* task identifier */
//	EC_TAG2(I_TRIALIDCD,SHOW_DOTS(gl_vsd)?8:(SHOW_FPCOLOR(gl_vsd)?2:1));  

	EC_TAG2(I_TRIALIDCD,20+gl_task); // 20 is eye, 21 is hand
	EC_TAG2(I_MONITORDISTCD, VIEW_DIST_CM);
	/* fixation x, y, diameter */
	EC_TAG1(I_FIXXCD, (VSD_GET_FP(gl_vsd))->x);
	EC_TAG1(I_FIXYCD, (VSD_GET_FP(gl_vsd))->y);
	EC_TAG2(I_EFIXDIAMCD, (VSD_GET_FP(gl_vsd))->diameter);
	/* target 1 x, y, lum, diameter*/
	EC_TAG1(I_TRG1XCD, (VSD_GET_T1(gl_vsd))->x);
	EC_TAG1(I_TRG1YCD, (VSD_GET_T1(gl_vsd))->y);
	EC_TAG2(I_TRG1DIAMCD, (VSD_GET_T1(gl_vsd))->diameter);
	/* target 2 x, y, lum, diameter */
	EC_TAG1(I_TRG2XCD, (VSD_GET_T2(gl_vsd))->x);
	EC_TAG1(I_TRG2YCD, (VSD_GET_T2(gl_vsd))->y);
	EC_TAG2(I_TRG2DIAMCD, (VSD_GET_T2(gl_vsd))->diameter);
	/* sure target x, y, lum */
	EC_TAG1(I_TRG3XCD, (VSD_GET_TS(gl_vsd))->x);
	EC_TAG1(I_TRG3YCD, (VSD_GET_TS(gl_vsd))->y);
	/* correct target */
	EC_TAG1(I_CORRTARGCD, gl_vsd->display->t_correct==VSD_OBJECT_T1?1:2);
	/* Dots dir, coh (if necessary), speed, and aperture size	*/
	if(SHOW_DOTS(gl_vsd)) {
		_VSDdot_object dot = VSD_GET_DTOBJECT(gl_vsd);
		EC_TAG1(I_DOTDIRCD, dot->direction);
		EC_TAG2(I_COHCD, dot->coherence);
		EC_TAG2(I_SEEDBASECD, dot->seed_base);
		EC_TAG2(I_SEEDVARCD, dot->seed_var);
		EC_TAG2(I_SPDCD, dot->speed);
		EC_TAG2(I_STDIACD, (VSD_GET_AP(gl_vsd))->diameter);
		EC_TAG1(I_STXCD, (VSD_GET_AP(gl_vsd))->x);
		EC_TAG1(I_STYCD, (VSD_GET_AP(gl_vsd))->y);    
		/* also update real-time variables */
		gl_rtvar.direction = dot->direction;
		gl_rtvar.coherence = dot->coherence;
	}
	
	/* update joystick coordinates in RT variables */
	gl_rtvar.joy_x_abs=CALC_JOY(joyh);
	gl_rtvar.joy_y_abs=CALC_JOY(joyv);
	gl_rtvar.joy_x=CALC_JOY(joyh)-(VSD_GET_FP(gl_vsd))->x;
	gl_rtvar.joy_y=CALC_JOY(joyv)-(VSD_GET_FP(gl_vsd))->y;
	gl_rtvar.joy_ecc=(int)(sqrt((double)gl_rtvar.joy_x*gl_rtvar.joy_x+(double)gl_rtvar.joy_y*gl_rtvar.joy_y)+.5);
	gl_rtvar.joy_ang=toy_atan(gl_rtvar.joy_x,gl_rtvar.joy_y);
	
	return 0;
}



/* ROUTINE: total
**
** use vs_score_trial to keep track of monkey's performance 
*/
int total(long score)
{

	/* local variables */
	int coh, ind, i;
	
	/* update the prize count */
	update_prize_count(score);
	
	/* score the trial in the global rec */
	vs_score_trial(gl_vsd, score, gl_sbet_shown);
	
	/* set globals for eye checking */
	gl_eye_flag   = 0;
	gl_hand_flag  = 0;

	
	/* Drop the appropriate code */
	if(score == CORRECT) {
		ec_send_code(CORRECTCD);
		EC_TAG1(I_RESPONSE, 1);
		gl_rtvar.total_correct++;	/* keeping track of total correct */
	} else if(score == WRONG) {
		ec_send_code(WRONGCD);
		EC_TAG1(I_RESPONSE, 0);
	} else if(score == SBET) {		/* ts */
		ec_send_code(SBETCD);
	} else if(score == NCERR) {
		ec_send_code(NOCHCD);
		EC_TAG1(I_RESPONSE, -1);
	} else if(score == BRFIX) {
		ec_send_code(FIXBREAKCD);
		EC_TAG1(I_RESPONSE, -2);
		again(); 			/* this is equivalent of reset_s(), it executes the abort list */
	}  else {
		EC_TAG1(I_RESPONSE, -10); /* default, for no fixation acquired */
	}
	
	/* record in real-time variables (for display) */
	gl_rtvar.total_trials = gl_vsd->total;
	gl_rtvar.num_completed = gl_vsd->correct + gl_vsd->wrong + gl_vsd->sbet;	/* ts */
	gl_rtvar.num_correct = gl_vsd->correct;
	gl_rtvar.num_wrong = gl_vsd->wrong;
	gl_rtvar.num_sbet = gl_vsd->sbet;		/* ts */
	gl_rtvar.num_ncerr = gl_vsd->ncerr;
	gl_rtvar.num_brfix = gl_vsd->brfix;
	gl_rtvar.last_score = gl_vsd->last;
	
	/* find the type of last trial, which coherence */ 
	coh=-1;
	ind=-1;
	if (SHOW_DOTS(gl_vsd)){
		coh = (VSD_GET_DTOBJECT(gl_vsd))->coherence;
		for(i=0; i<sizeof(gl_coh_list); i++)
		  if(coh == gl_coh_list[i])	{
			  ind = i;
			  break;
		  }
	}
	
	/* set reat time variable for RT to zero, revise below if an RT task */   	
	gl_rtvar.rt = 0;	
	
	/* counts and RT for task 1 */
	if (gl_task==TASK_EYE)	{
		/* total correct/wrong/sbet counts */
		if (ind!=-1)		{
			if ((score==CORRECT)||(score==WRONG)||(score==SBET))	{
				if (score==CORRECT)
				  gl_teye_perf_cor[ind]++;
				else if (score==SBET)			/* ts */
				  gl_teye_perf_sbet[ind]++;
				gl_teye_perf_tot[ind]++;
			}
		}
		/* Reaction times */
		if (gl_rt_flag)	{
			if ((score==CORRECT)||(score==WRONG)||(score==SBET))	{
				gl_rtvar.rt = gl_resp_time - gl_ref_time;
				if (ind!=-1)		{
					if (score==CORRECT)		{
						gl_teye_rt_cor_sum[ind]+=gl_rtvar.rt;
						gl_teye_rt_cor_n[ind]++;
					} else if (score==WRONG)		{
						gl_teye_rt_err_sum[ind]+=gl_rtvar.rt;
						gl_teye_rt_err_n[ind]++;
					} else if (score==SBET)	{	/* ts */
						gl_teye_rt_sbet_sum[ind]+=gl_rtvar.rt;
						gl_teye_rt_sbet_n[ind]++;
					}
				}
			}
		} 
	}
	
	/* outta */
	return(0);
}


int joySet()
{
	storex=CALC_JOY(joyh);
	storey=CALC_JOY(joyv);
	return 0;
}

/* ROUTINE: abort_cleanup
**
** called only from abort list
*/
int abort_cleanup(void)
 {
	timer_pause(100);				/* wait in case of went				*/
	
//mat_getWent();/* make sure not waiting for went*/
	
	if(gl_dots_flag == -1) 	 	/* stop the dots, if they're on 	*/
     	mat_dotsAbort();
     	
	end_trial(CANCEL_W);			/* cancel analog window, blank screen */
	
	return(0);	
 }


 
/* USER FUNCTIONS
**
** functions that we can call from the command
** menu.
**
**		performance ... task-specific information (% correct, etc)
**		status      ... status information (trials remaining)
**		geometry    ... geometry information
*/
USER_FUNC ufuncs[] = {
{"status",      &print_status,    			"void"},
{"performance", &print_experiment_info, 	"void"},
{""},
};

/*  
** The geometry of a "vs_dots" task is defined as follows:
**
**	- Fixation point  ... X-Y coordinates defined at the top-level
**						  	menu & thus the same for all tasks/trials
**	- Dots' aperture  ... R-T coordinates (AP_amp and AP_angle) relative to
**						 	the fixation point
**	- Target centroid ... Central point of the two targets, in R-T (TC_amp
**							and TC_angle) relative to AP
**	- Target p		  ... Pref (correct) target, in R-T (T1_amp and T1_angle)
**							relative to TC						
**	- Target n		  ... Null (incorrect) target, in R-T (T1_amp and T1_angle)
**							relative to TC						
**
**	INDEX SCHEME FOR SUN/PLANET/JOYSTICK
**		0  ... fixation point
**		1  ... center of dots' aperture
**		2  ... centroid of targets
**		3  ... target p (pref or correct target)
**		4  ... target n (null or incorrect target)
*/

/* ACCESS FUNCTIONS */
/* ROUTINE: o_agf
**
** Object Argument Generation Function, from REX 7.* manual
*/
int o_agf(int call_cnt, MENU *mp, char *astr)
 {
   if(call_cnt >= VSD_NUM_OBJECTS)
       *astr='\0';
   else
       itoa_RL(call_cnt, 'd', astr, &astr[P_ISLEN]);
   return(0);
 }

/* ROUTINE: o*_maf
**
** Menu Access Function, from REX 7.* manual
*/
int o1_maf(int flag, MENU *mp, char *astr, ME_RECUR *rp)
 {
   int num;
   
   if(*astr == '\0')
       num = 0;
   else
       num = atoi(astr);
   
   if ( num<0 || num>=VSD_NUM_OBJECTS )
       return(-1);
   
   mp->me_basep = (unsign)&gl_object_teyeS[num];
   return(0);
 }
 
int o2_maf(int flag, MENU *mp, char *astr, ME_RECUR *rp)
 {
   int num;
   
   if(*astr == '\0')
       num = 0;
   else
       num = atoi(astr);
   
   if ( num<0 || num>=VSD_NUM_OBJECTS)
       return(-1);
   
   mp->me_basep = (unsign)&gl_object_thandS[num];
   return(0);
 }


/* Top-level state menu
** "Joystick" means that the position of the joystick cursor
**	determines the position of the selected object ('-' means no joystick)
*/
VLIST state_vl[] = {
	{"Joystick",				&(gl_menu_infoS.joystick), 			NP, NP, 0, ME_NVALD},
	{"Proportion_TEYE", 		&(gl_task_infoS[0].proportion), 	NP, NP, 0, ME_DEC},
	{"Repetitions",			    &(gl_menu_infoS.repetitions),		NP, make_tasks, ME_AFT, ME_DEC},
	{"RNG_Seed",			    &(gl_menu_infoS.seed),		        NP, NP, 0, ME_DEC},	
	{"Max_prize_count",         &gl_prize_max, 						NP, NP, 0, ME_DEC},
	{NS}};

/* task_info vlist ... used for the two dots-style tasks */
VLIST task1_info_vl[] = {
   {"object_1", 				&(gl_task_infoS[0].object_1), NP, NP, ME_GB, ME_DEC},
   {"object_2", 				&(gl_task_infoS[0].object_2), NP, NP, ME_GB, ME_DEC},
   {"series_o", 				&(gl_task_infoS[0].series_o), NP, NP, ME_GB, ME_DEC},
   {"series_n", 				&(gl_task_infoS[0].series_n), NP, NP, ME_GB, ME_DEC},
   {"series_delta", 		    &(gl_task_infoS[0].series_delta), NP, NP, ME_GB, ME_DEC},
   {"t_flag", 					&(gl_task_infoS[0].t_flag), NP, NP, ME_GB, ME_DEC},
   {"coherence_lo", 		    &(gl_task_infoS[0].coherence_lo), NP, NP, ME_GB, ME_DEC},
   {"coherence_hi", 		    &(gl_task_infoS[0].coherence_hi), NP, NP, ME_GB, ME_DEC},
   {"RT_task", 				    &(gl_task_infoS[0].rt_flag), NP, NP, ME_GB, ME_DEC},
   {NS}};

VLIST task2_info_vl[] = {
   {"object_1", 				&(gl_task_infoS[1].object_1), NP, NP, ME_GB, ME_DEC},
   {"object_2", 				&(gl_task_infoS[1].object_2), NP, NP, ME_GB, ME_DEC},
   {"series_o", 				&(gl_task_infoS[1].series_o), NP, NP, ME_GB, ME_DEC},
   {"series_n", 				&(gl_task_infoS[1].series_n), NP, NP, ME_GB, ME_DEC},
   {"series_delta", 	    	&(gl_task_infoS[1].series_delta), NP, NP, ME_GB, ME_DEC},
   {"t_flag", 					&(gl_task_infoS[1].t_flag), NP, NP, ME_GB, ME_DEC},
   {"coherence_lo", 		    &(gl_task_infoS[1].coherence_lo), NP, NP, ME_GB, ME_DEC},
   {"coherence_hi", 		    &(gl_task_infoS[1].coherence_hi), NP, NP, ME_GB, ME_DEC},
   {"RT_task",					&(gl_task_infoS[1].rt_flag), NP, NP, ME_GB, ME_DEC},
   {NS}};

/* Object vlist... remember that there are VSD_NUM_OBJECTS # of objects
** per task, so we make an array of that many objects
*/
#define OS &((_VSobject)NP)
VLIST object_vl[] = {
   {"x",          	OS->x,            NP, NP, ME_GB, ME_DEC},
   {"y",          	OS->y,            NP, NP, ME_GB, ME_DEC},
   {"amplitude",  	OS->amplitude,    NP, NP, ME_GB, ME_DEC},
   {"angle",      	OS->angle,        NP, NP, ME_GB, ME_DEC},
   {"vertex",     	OS->vertex,       NP, NP, ME_GB, ME_DEC},
   {"wrt",        	OS->wrt,          NP, NP, ME_GB, ME_DEC},
   {"angle_offset",	OS->angle_offset, NP, NP, ME_GB, ME_DEC},
   {"diameter",   	OS->diameter,     NP, NP, ME_GB, ME_DEC},
   {"color.R",  	OS->color.R,      NP, NP, ME_GB, ME_DEC},
   {"color.G",  	OS->color.G,      NP, NP, ME_GB, ME_DEC},
   {"color.B",  	OS->color.B,      NP, NP, ME_GB, ME_DEC},
   {NS}};

/* Dots parameters vlist */
#define DOTS_VL(DOS)  \
   {"coherence",  &(DOS.coherence), NP, NP, ME_GB, ME_DEC}, \
   {"direction",  &(DOS.direction), NP, NP, ME_GB, ME_DEC}, \
   {"speed",      &(DOS.speed),     NP, NP, ME_GB, ME_DEC}, \
   {"novar_pct",  &(DOS.novar_pct), NP, NP, ME_GB, ME_DEC}, \
   {"seed_base",  &(DOS.seed_base), NP, NP, ME_GB, ME_DEC}, \
   {NS}
VLIST dots1_vl[] = {DOTS_VL(gl_dotS[0])};
VLIST dots2_vl[] = {DOTS_VL(gl_dotS[1])};

/* Help strings */
char no_help[] = "";

MENU umenus[] = {
   {"State_vars", 		&state_vl, 		NP, NP, 0, NP, no_help},
   {"separator", 		NP},
   {"Task1_info", 		&task1_info_vl, NP, NP, 0, NP, no_help},
   {"Task1_objList", 	&object_vl,     NP, o1_maf, ME_BEF, o_agf, no_help},
   {"Task1_dots", 		&dots1_vl, 		NP, NP, 0, NP, no_help},
   {"separator", 		NP},
   {"Task2_info", 		&task2_info_vl, NP, NP, 0, NP, no_help},
   {"Task2_objList", 	&object_vl, 	NP, o2_maf, ME_BEF, o_agf, no_help},
   {"Task2_dots", 		&dots2_vl, 		NP, NP, 0, NP, no_help},
   {NS}
};

/* REAL-TIME VARIABLES */
RTVAR rtvars[] = {
   {"Total trials", 			&(gl_rtvar.total_trials)},
   {"Total correct", 	    	&(gl_rtvar.total_correct)},
   {"Trials this set", 	    	&(gl_rtvar.num_completed)},
   {"Previous score",       	&(gl_rtvar.last_score)},
   {"Broke fixation", 	    	&(gl_rtvar.num_brfix)},
   {"No choice", 		    	&(gl_rtvar.num_ncerr)},
   {"Correct", 			    	&(gl_rtvar.num_correct)},
   {"Wrong", 					&(gl_rtvar.num_wrong)},
   {"Sure Bet", 				&(gl_rtvar.num_sbet)},
   {"Coherence", 	    		&(gl_rtvar.coherence)},
   {"Duration", 				&(gl_rtvar.duration)},
   {"Direction", 				&(gl_rtvar.direction)},
   {"Elec stim", 				&gl_elestim_flag},
   {"Pulse", 					&(gl_rtvar.pulse_time)},
   {"Show SBET", 		    	&gl_sbet_shown},
   {"Delay", 					&(gl_rtvar.delay)},
   {"RT", 						&(gl_rtvar.rt)},
   {"Joystick X (abs.)",     	&(gl_rtvar.joy_x_abs)},
   {"Joystick Y (abs.)",    	&(gl_rtvar.joy_y_abs)},
   {"Joystick X", 	    		&(gl_rtvar.joy_x)},
   {"Joystick Y", 				&(gl_rtvar.joy_y)},
   {"Joystick eccentr.",    	&(gl_rtvar.joy_ecc)},
   {"Joystick angle", 	    	&(gl_rtvar.joy_ang)},
   {"", 0}
};




/* THE STATE SET 
*/
%%
id 901
restart rinitf
main_set {
status ON
begin	first:
		to firstcd
	firstcd:
		do ec_send_code(HEADCD)
		to setup
	setup: 		/* SETUP THE SCREEN */
		do setup_screen(40, 57, 4, 1, 0)
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
		do nexttrl(0, 10, 1)
		to task on +ONES & gl_remain
		to loop
	task:			/**** TASK BRANCHING ... ****/
		to teye_start on TASK_EYE = gl_task

	/* position window, wait for fixation */
	fixeyewinpos:
	    do position_eyewindow(10, 10, 0)
	    time 10	/* this is very important - it takes time to set window */
		to fixeyewait
	fixeyewait:    /* wait for either eye or hand fixation */
	    time 5000
	    to fixeyedelay on -WD0_XY & eyeflag
		to nofix
	nofix:		/* failed to attain either eye or hand fixation or both*/
	    do total(DO_OVER) // just redo the trial
	    to nofixDone
	nofixDone:
	    time 2000
		do end_trial(CANCEL_W)
		to loop
	
	fixeyedelay:
	    time 100 /* delay before activating eye_flag - noise on the eye position signal should not be able to "break eye fixation" */
		to fixeyeset
	fixeyeset:		/* set flag to check for eye fixation breaks */
		do set_eye_flag(E_FIX)
	    time 2000 /* additional time to attain hand fixation */
	    to fixeyedone
		
	/* Done with fixating stuff */
	fixeyedone:		/**** TASK BRANCHING ... ****/
	    do position_eyewindow(25, 25, 0)
		to teye_targ_wait on TASK_EYE = gl_task

/*** CHAIN FOR TASK EYE
***/
	teye_start:
	    do setup_eyewindows(25, 25)
		to teye_fp
	teye_fp:
		do drawTarg(1000,0,0,0,0)
		to teye_fp_cd on MAT_WENT % drawTarg_done
		/* to teye_fp_cd on PHOTO_DETECT_UP % dio_check_photodetector */
	teye_fp_cd:
		do ec_send_code(FPONCD)
		to fixeyewinpos
	teye_targ_wait:
		do timer_set1_shell(1000,200,600,100,0,0)
		to teye_targ_def on +MET % timer_check1
	teye_targ_def:
		do defTargLum(-1, -1, -1, -1)
		to teye_targ
	teye_targ: /* show the targets */
		do drawTarg(1000, 1000, 1000, -1, 0)
		to teye_targ_cd on MAT_WENT % drawTarg_done
  	    /* to teye_targ_cd on PHOTO_DETECT_DOWN % dio_check_photodetector */ 
	teye_targ_cd:
		do ec_send_code(TARGC1CD)
		to teye_waitdots
		
	/* START SHOWING DOTS */
	teye_waitdots:
		do timer_set1_shell(1000,100,600,150,0,0)
		to teye_startdots on +MET % timer_check1
	teye_startdots:	/* START DOTS */
		do showDots()
		to teye_wentdots on MAT_WENT % showDots_done
	    /* to teye_wentdots on PHOTO_DETECT_UP % dio_check_photodetector */
	teye_wentdots:
		do ec_send_code(GOCOHCD)
		to teye_elestondots
	teye_elestondots:		/* define the delay from dots onset to electrical stim */
		do timer_set1_shell(0,0,0,0,0,0)
		to teye_eleston on +MET % timer_check1
	teye_eleston:		/* deliver the electrical stim, with the specified probability */
		do elestim_on(FTTL1, FTTL2, 0)
		to teye_rtreft on 1 = gl_rt_flag
		to teye_setdotdur
	
	/* RT task */
	teye_rtreft:        /* store reference time */
		do get_ref_time()
		to teye_rteyef
	teye_rteyef:	    /* monkey is now allowed to break fixation */
		do set_eye_flag(E_OFF)
		to teye_rtdotdur
	teye_rtdotdur:	    /* set the dots timeout */
		do set_dots_duration(0,0,0,0,5000,0)
		to teye_rtsacc on +WD0_XY & eyeflag	    /* saccade detected */
		to teye_rtnore on +MET % timer_check1	/* timeout */
	teye_rtnore:
		do stopDots()
 		to teye_rtdone on MAT_WENT % stopDots_done
	  	/* to teye_rtdone on PHOTO_DETECT_DOWN % dio_check_photodetector */
	teye_rtdone:
		do ec_send_code(STOFFCD)
		to ncerr
	teye_rtsacc:
		do ec_send_code(SACMADCD)
		to teye_rtrest
	teye_rtrest:        /* store response time */
		do get_resp_time()
		to teye_rtstod
	teye_rtstod:
		do stopDots()		
		to teye_rtsdcd on MAT_WENT % stopDots_done
	    /* to teye_rtsdcd on PHOTO_DETECT_DOWN % dio_check_photodetector */
	teye_rtsdcd:		/* drop the stop dots code */
		do ec_send_code(STOFFCD)
		to teye_rtelestoff
	teye_rtelestoff:
		do elestim_off(FTTL1, FTTL2, 0)
		to check_eye_response
		
	/* FIXED DURATION TASK */
	teye_setdotdur:		/* set the dots duration timer */
		do set_dots_duration(1000,100,1500,300,0,0)
		to teye_stopdots on +MET % timer_check1 /* check the timer */
	teye_stopdots:
		do stopDots()
		to teye_stopdotscd on MAT_WENT % stopDots_done
	teye_stopdotscd:		/* drop the stop dots code */
		do ec_send_code(STOFFCD)
		to teye_elestoffd
	teye_elestoffd:	/* define the delay from dots offset to end of electrical stim */
		do timer_set1_shell(0,0,0,0,0,0)
		to teye_elestoff on +MET % timer_check1		
	teye_elestoff:		/* stop electrical stim */ 
		do elestim_off(FTTL1, FTTL2, 0)		/* the third argument, 1, means store the time in the datafile */
		to teye_waitfpoff
	
	/* FINAL WAIT, FP OFF */
	teye_waitfpoff:
        do set_delay_duration(1000,500,1000,100,0,0)	
		to teye_waitsbet
	teye_waitsbet:
		do timer_set2_shell(0,0,0,0,0,0)
		to teye_showsbet on +MET % timer_check2
	teye_showsbet:
		do show_sbet(0)
		to teye_wentsbet on MAT_WENT % drawTarg_done
	teye_wentsbet:
		do dropcode_sbet()
		to teye_chkwait
	teye_chkwait:
		to teye_fpoff on +MET % timer_check1
	teye_fpoff: /* turn the FP off */
		do drawTarg(0,-1,-1,-1,1000, 0)
		to teye_fpoff_cd on MAT_WENT % drawTarg_done
	teye_fpoff_cd:
		do ec_send_code(FPOFFCD)
		to teye_grace	/* wait for sac */

	
	/* grace period in which monsieur le monk has to 
	** break fixation and start the saccade
	*/
	teye_grace:
		do set_eye_flag(E_OFF)
		to teye_gracea
	teye_gracea:
		time 500
		to teye_saccd on +WD0_XY & eyeflag
		to ncshow
	teye_saccd:
		do ec_send_code(SACMADCD)
		to check_eye_response
		

	/* end trial possibilities:
	**
	** - brfix. broke fixation, counted regardless of trial type
	** - ncerr. didn't complete trial once something happened
	** - null.  chose wrong target in dots discrimination task
	** - pref.  finished correctly and got rewarded
	*/
	check_eye_response:
		time 500
    	to peyehold on -WD3_XY & eyeflag		/* got correct target! */
	    to neyehold on -WD4_XY & eyeflag		/* got incorrect target! */
        to ncshow                               /* got nada */
	peyehold: 	
		do ec_send_code(TRGACQUIRECD)
		time 50	/* gotta hold for this long	*/
		to ncshow on +WD3_XY & eyeflag
		to pref
	neyehold:	
		do ec_send_code(TRGACQUIRECD)
		time 50 /* gotta hold for this long	*/
		to ncshow on +WD4_XY & eyeflag
		to nushow
	seyehold:
		do ec_send_code(TRGACQUIRECD)
		time 50 /* gotta hold for this long	*/
		to ncshow on +WD5_XY & eyeflag
		to sbet

	/* visual feedback if not successful (NO CHOICE) */
	ncshow: /* show the targets */		
		do drawTarg(-1, -1, -1, -1, -1, 0)
		to ncwait on MAT_WENT % drawTarg_done
	ncwait:
		time 0
		to ncerr  
	
	/* NO CHOICE: didn't complete the task */
	ncerr:
		do total(NCERR) 
		to ncend
	ncend:
		do end_trial(CLOSE_W)
		time 1000
		to loop
	
	/* visual feedback if not successful (WRONG CHOICE) */
	nushow: /* show the targets */		
		do drawTarg(-1, -1, -1, -1, -1, 0)
		to nuwait on MAT_WENT % drawTarg_done
	nuwait:
		time 0
		to null  
	
	/* NULL: chose wrong target in dots task */
	null:
		do total(WRONG)
		to nuclose
	nuclose:
		do end_trial(CLOSE_W)
		to nupunish
	nupunish:
		do set_punishment_timer(0,1000,0,0)
		to nuend on +MET % timer_check1
	nuend:
		time 1000 
		to loop

	/* PREF: update the totals and give a reward
	*/
	pref:
	    time 250
		do total(CORRECT)
		to prend
	prend:
		do drawTarg(-1, 1000, -1, -1, -1, 1)
		to prdelay
	prdelay: /* guarantee a minimum time between dot onset and reward */
		do set_reward_delay(0)
		to prrew on +MET % timer_check1
	prrew:
		do give_reward(REW, 150, CORRECT)
		to prrew_off on +MET % timer_check1
	prrew_off:
		do dio_off(REW)
		to prize_loop on 0 < gl_prize_count
		to prdone 
	prize_loop:
		to prdone on 0 ? gl_prize_count
		to prize_delay
	prize_delay:
		time 150		/* the delay between consecutive rewards */
		do drawTarg(-1, 0, -1, -1, -1, 1)
		to prize 
	prize:
		do give_reward(REW, 50, PRIZE)
		to prize_off on +MET % timer_check1
	prize_off:
		do dio_off(REW)
		to prize_loop
	sbet:
		do total(SBET)
		to sbend
	sbend:
		do end_trial(CLOSE_W)
		to sbdelay
	sbdelay:
		do set_reward_delay(0)
		to sbrew on +MET % timer_check1
	sbrew:
		do give_reward(REW, 50, SBET)
		to sbrew_off on +MET % timer_check1
	sbrew_off:
		do dio_off(REW)
		to prdone
	prdone:
		do end_trial(CLOSE_W)
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

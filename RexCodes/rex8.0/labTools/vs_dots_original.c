/*
** VS_dots.C
**
** "VisualStimulation_DOTS.c" ... 
**
**		create stimulus objects for dot discrimination
**		paradigms (e.g., 502)
*/

#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <math.h>
#include <sys/types.h>
#include "rexhdrs.h"

#include "toys.h"
#include "vs_dots.h"
#include "matlab.h"

/* PRIVATE ROUTINE PROTOTYPES */
static void vsd_parse_objects(_VSobject *, int);

/* GLOBAL VARIABLES */

/* PRIVATE CONSTANTS */

/* PUBLIC ROUTINES */

	/* RECORD */
/* PUBLIC ROUTINE: vsd_init_record
**
** initialize a dots-specific VSrecord
**
**	add tasks later. for now just add the display struct.
**	jig changed 6.6.01 so that there are now 6 objects
**	stored, including the dots aperture and the dot direction
*/
_VSrecord vsd_init_record(void)
 {
	_VSrecord 	rec  		= vs_init_record();
	_VSdisplay	display 	= vs_init_display();
	register int i;

	/* make all the objects. Remember the last one is the Dots object */
	display->num_objects  = VSD_NUM_OBJECTS;
	display->object_array = SAFE_ZALLOC(_VSobject, VSD_NUM_OBJECTS);
	for(i=0;i<VSD_NUM_OBJECTS-1;i++)
		display->object_array[i] = vs_init_object();
	display->object_array[i] = vsd_init_dot_object();
	
	/* set the MATLAB indices */
	display->object_array[VSD_OBJECT_FP]->matlab_index = MAT_FP;
	display->object_array[VSD_OBJECT_T1]->matlab_index = MAT_T1;
	display->object_array[VSD_OBJECT_T2]->matlab_index = MAT_T2;
	display->object_array[VSD_OBJECT_TS]->matlab_index = MAT_T3;	/* ts */
   
	
	/* attach the display to the rec  & outta */
	rec->display = display;
	return(rec);
 }


/* PUBLIC ROUTINE: vsd_clear_record
**
*/
void vsd_clear_record(_VSrecord rec)
{
	if(rec) {

		/* free all the tasks */
		vsd_free_all_tasks(rec);

		/* clear all the objects */
		vsd_clear_display(rec->display);

		/* clear other stuff */
		vs_clear_record(rec);
	}
}

/* PUBLIC ROUTINE: vsd_free_record
**
*/
void vsd_free_record(_VSrecord rec)
{
	if(rec) {

		printf("freeing the rec\n");

		/* free the tasks */
		vsd_free_all_tasks(rec);

		/* free the display */
		if(rec->display) {
			register int i;

			for(i=0;i<VSD_NUM_OBJECTS-1;i++)
				vs_free_object(rec->display->object_array[i]);
			vsd_free_dot_object(rec->display->object_array[i]);
			SAFE_FREE(rec->display->object_array);
			SAFE_FREE(rec->display);
		}

		/* free the rest */
		SAFE_FREE(rec);
	}
}

/* PUBLIC ROUTINE: vsd_print_record
**
*/
void vsd_print_record(_VSrecord rec)
{
	if(rec) {
		register int i;
		
		printf("\n------REC INFO (%d trials)------\n", rec->num_trials);
		
		for(i=0;i<rec->num_tasks;i++)
			vsd_print_task(rec->task_array[i]);

		if(rec->num_trials > 0 && rec->cur_trial_index >= 0 &&
			rec->cur_trial_index < rec->num_trials) {
			printf("TRIALS: ");
			for(i=rec->cur_trial_index;i<rec->num_trials&&i<rec->cur_trial_index+100;i++)
				printf("%d, ", rec->trial_array[i]->id);
			printf("\n");
		}
	}
}

	/* TASKS */

/* PUBLIC ROUTINE: vsd_init_task
**
**	 init a _VStask struct with dots-specific task information
*/
_VStask vsd_init_task(_VSDtask_info task_info)
 {
   _VStask task	= vs_init_task();
   
   	/* if NULL given, make new task info */
   if(!task_info)
       vsd_init_task_info(task_info);
   task->task_info = task_info;
   
   return(task);
 }

/* PUBLIC ROUTINE: vsd_init_task_info
**
**	 init a _VSDtask_info struct
*/
_VSDtask_info vsd_init_task_info(_VSDtask_info task_info)
 {
   	/* if NULL given, make new task info */
   if(!task_info)
       task_info = SAFE_STALLOC(_VSDtask_info_struct);
   
   if (task_info) {
       memset(task_info, 0, sizeof(struct _VSDtask_info_struct));
       task_info->series_n = 1;
   }
   
   return(task_info);
 }

/* PUBLIC ROUTINE: vsd_copy_task_info
**
**	 copy dots-specific task information
*/
_VSDtask_info vsd_copy_task_info(_VSDtask_info task_info_in)
 {
   _VSDtask_info task_info_out = SAFE_STALLOC(_VSDtask_info_struct);
   
   if(task_info_in)
       memcpy(task_info_out, task_info_in, sizeof(struct _VSDtask_info_struct));
   else
       vsd_init_task_info(task_info_out);
   task_info_out->proportion = 0;
   
   return(task_info_out);
 }

/* PUBLIC ROUTINE: vsd_free_task
**
*/
void vsd_free_task(_VStask task)
{
	register int i;

	if(task) {

		/* free the trials */
		for(i=0;i<task->num_trials;i++)
			vsd_free_trial(task->trial_array[i]);
		SAFE_FREE(task->trial_array);

		/* free the paradigm-specific task info */
		if(task->task_info) {
			_VSDtask_info task_info = (_VSDtask_info) (task->task_info);

			SAFE_FREE(task->task_info);
		}

		/* free it all */
		vs_free_task(task);
	}
}

/* PUBLIC ROUTINE: vsd_free_all_tasks
**
*/
void vsd_free_all_tasks(_VSrecord rec)
{
	register int i;

	if(rec) {

		for(i=0;i<rec->num_tasks;i++)
			vsd_free_task(rec->task_array[i]);
		SAFE_FREE(rec->task_array);
		rec->num_tasks = 0;

		/* clear up other stuff in the record */
		SAFE_FREE(rec->trial_array);
		rec->num_trials 		= 0;
		rec->cur_trial_index = 0;
		rec->cur_trial 		= NULL;
	}
}

/* PUBLIC ROUTINE: vsd_add_task
**
*/
void vsd_add_task(int coh_num, int *coh_list, _VSrecord rec, _VSDtask_info task_info, int type)
 {
	int	*coh, coh_i, coh_n, 
		series_o, series_n, series_delta, series_angle;
	register int i, j, k, count;
	_VSDtrial_info trial_info;
	_VStask 			task;
	
	/* anal error checking */
	if(!rec || !task_info || (task_info->proportion <= 0))
		return;
	
	/* make the task with a copy of the task_info */
	task 		= vsd_init_task(task_info ? vsd_copy_task_info(task_info) : NULL);
	task->id	= type;
	task->reps 	= task_info->proportion;
	
	/* figure out which coherences we'll use */
	coh			= coh_list;
	count 		= 0;
	while(count++ < coh_num && *coh++ < task_info->coherence_lo);
	coh_i		= count - 1;
	while(count++ < coh_num && *coh++ <= task_info->coherence_hi);
	coh_n		= count - coh_i - 1 > 0 ? count - coh_i - 1 : 1;
	
	/* error check the "series" arguments */
	series_o 	 = task_info->series_o == NULLI ? 0 : task_info->series_o;
	series_n 	 = task_info->series_n == NULLI || !task_info->series_n ?
						1 : task_info->series_n;
	series_delta = task_info->series_delta == NULLI ? 0 : task_info->series_delta;
	
	/* add the trials...
	**		- rotate object_2 around object_1 "angl_n" times
	**		- make "coh_ind_n" different coherences
	*/
	task->num_trials  = series_n * coh_n;
	task->trial_array = SAFE_ZALLOC(_VStrial, task->num_trials);
	
	/* loop for each coh, angle */
	for ( i=0,k=0 ; k<coh_n ; coh_i++,k++ ) {
		for ( series_angle=series_o,j=0 ; j<series_n ; series_angle+=series_delta,j++,i++ ) {
			trial_info = SAFE_STALLOC(_VSDtrial_info_struct);
			trial_info->planet_angle 	= series_angle;
			trial_info->coherence	 	= coh_list[coh_i];
			
			task->trial_array[i] 		= vsd_init_trial(trial_info);
			task->trial_array[i]->id	= type*100 + i;	
			task->trial_array[i]->task 	= task;
			
/*			task->trial_array[i]->reps = (long)100/series_n; */
			/* Luke was here */
//			if (coh_list[coh_i] == 32)
//			  task->trial_array[i]->reps = 20; 
//			else 
//			  task->trial_array[i]->reps = 10; 
			task->trial_array[i]->reps = 10;

			// Luke was here
			task->trial_array[i]->perf_loopback = task->trial_array[i];
		}
	}
	
	if(i != task->num_trials)
		printf("vsd_add_task: uh oh\n");

	/* add the task to the record */
	vs_rec_add_task(rec, task);
 }


/* ROUTINE: vsd_print_task
**
** prints out info about trials, e.g., #correct & #total,
** on a task-by-task basis
*/
void vsd_print_task(_VStask task)
{
	if(task) {
		register int i;

		printf("\tTASK id <%d>, %d total trials:\n", task->id, task->num_trials);
		for(i=0;i<task->num_trials;i++)
			vsd_print_trial(task->trial_array[i]);
	}	
}

	/* TRIAL */

/* PUBLIC ROUTINE: vsd_init_trial
**
**	 dots-specific trial information
*/
_VStrial vsd_init_trial(_VSDtrial_info trial_info)
{
	_VStrial trial	= vs_init_trial();

	if(!trial_info) {
		trial_info = SAFE_STALLOC(_VSDtrial_info_struct);

		trial_info->planet_angle	= NULLI;
		trial_info->coherence		= NULLI;
		
		/* Luke was here */
		trial_info->seed_base		= NULLI;
		trial_info->seed_var		= NULLI;		
		/* Luke stopped here */
	}

	trial->trial_info = trial_info;

	return(trial);
}

/* PUBLIC ROUTINE: vsd_free_trial
**
**	 dots-specific trial information
*/
void vsd_free_trial(_VStrial trial)
{
	if(trial) {
		SAFE_FREE(trial->trial_info);
		vs_free_trial(trial);
	}
}

/* PUBLIC ROUTINE: vsd_print_trial
**
*/

/* void vsd_print_trial(_VStrial trial)
** {
**	int tot;
**	
**	if(trial) {		
**		_VSDtrial_info trial_info = (_VSDtrial_info) (trial->trial_info);
**		
**		tot = trial->correct+trial->null+trial->sbet;
**		printf("\t   trial(%dx%ld) %3d/%3d: %3d cor, %3d nul, %3d brEfix, %3d/%3d sbet, %3d nc,  %5.1lf pct, %5.1lf psb\n",
**			trial->id, trial->reps, trial_info->planet_angle, trial_info->coherence,
**			trial->correct, trial->null, trial->brfix, trial->sbet, trial->sbet_total, trial->ncerr,
**			tot ? (100.0) * ((double)(trial->correct))/((double)(tot)) : 0,
**			tot ? (100.0) * ((double)(trial->sbet))/((double)(tot)) : 0 );		
**	}
** }
*/

void vsd_print_trial(_VStrial trial)
 {
	int tot;
	
	if(trial) {		
		_VSDtrial_info trial_info = (_VSDtrial_info) (trial->trial_info);
		
		tot = trial->perf_loopback->correct + trial->perf_loopback->null + trial->perf_loopback->sbet;
		printf("\t   trial(%dx%ld) %3d/%3d: %3d cor, %3d nul, %3d brEfix, %3d nc, %5.1lf pct\n",
			trial->id, trial->reps, trial_info->planet_angle, trial_info->coherence,
			trial->perf_loopback->correct, trial->perf_loopback->null, 
			trial->perf_loopback->brfix, trial->perf_loopback->ncerr,
			tot ? (100.0) * ((double)(trial->perf_loopback->correct))/((double)(tot)) : 0);
	}
 }


/* PUBLIC ROUTINE: vsd_print_all_trials
**
*/
void vsd_print_all_trials(_VSrecord rec)
{
	if(rec) {
		register int i;

		for(i=0;i<rec->num_trials;i++)	
			vsd_print_trial(rec->trial_array[i]);
	}	
}

/* DISPLAY */

/* PUBLIC ROUTINE: vsd_update_display
**
**	replaces vsd_trial_geometry as the real workhorse.
**	Translates info in the current trial into
**	info in the current display (i.e., figures out
**	how & where the fp/targets/dots will be shown...
**
** Arguments:
**		rec 			... the big kahuna
**		dots			... dots parameters
**
*/
void vsd_update_display(_VSrecord rec, int rev_dot_targ_association)
 {
	_VSobject *objAP, obj;
	_VSDtrial_info trial_info;
	_VSDtask_info	task_info;
	register int i;
	int j1, j2, m1, m2, do_again;	/* ts */
	int dist, dsave[6], dcount;		/* ts */
	static int seed_base = 0;
	_VSDdot_object dot;
	int dx, dy, dtarg_ang, dot_ang, del_ang;
		
	/**
	*** generate the random seed base if needed
	***/	
	if(!seed_base)
		seed_base = TOY_RAND(1000); /* different base for each experiment */

	 
	/**
	*** get structs & do some error checking 
	***/
	if(!rec || !(rec->cur_trial) || 
		!(trial_info = (_VSDtrial_info) (rec->cur_trial->trial_info)) ||
		!(task_info  = (_VSDtask_info) (rec->cur_trial->task->task_info)) ||
		!(objAP 	 = rec->display->object_array))
		return;

	
	/**
	*** we should make sure that the sun and planet are valid
	***/
	#define GOOD_SUN(t) 	(((t)->object_1 >= VSD_OBJECT_FP) && ((t)->object_1 <= VSD_OBJECT_TS))	/* ts */
	#define GOOD_PLANET(t) 	(((t)->object_2 >= VSD_OBJECT_AP) && ((t)->object_2 <= VSD_OBJECT_DT))
	/* sun, planet */
	if(GOOD_PLANET(task_info)) {
		if(objAP[task_info->object_2]->angle == NULLI)
			objAP[task_info->object_2]->angle = trial_info->planet_angle;
		if((objAP[task_info->object_2]->vertex == NULLI) && GOOD_SUN(task_info))
			objAP[task_info->object_2]->vertex = task_info->object_1;
	}

	 /* Fixation point should be set */
	obj = objAP[VSD_OBJECT_FP];
	if(obj->x < -500 || obj->x > 500) obj->x = 0;
	if(obj->y < -500 || obj->y > 500) obj->y = 0;
	obj->angle = 0;
	/* Error-check the rest */
	for(i=VSD_OBJECT_AP;i<=VSD_OBJECT_DT;i++) {
		obj = objAP[i];
		if(obj->amplitude  	== NULLI) 	obj->amplitude 		= 0;
		if(obj->angle_offset == NULLI) 	obj->angle_offset 	= 0;
		if(obj->vertex < VSD_OBJECT_FP || obj->vertex > VSD_OBJECT_DT) {
			switch(i) {
				case VSD_OBJECT_AP:
					obj->vertex = VSD_OBJECT_FP;
					break;
				case VSD_OBJECT_TC:
					obj->vertex = VSD_OBJECT_AP;
					break;
				case VSD_OBJECT_T1: case VSD_OBJECT_T2:	case VSD_OBJECT_TS: 	/* ts */
					obj->vertex = VSD_OBJECT_TC;
					break;
			}
		}
		if((obj->wrt < VSD_OBJECT_FP || obj->wrt > VSD_OBJECT_DT) && obj->wrt != NULLI)
			obj->wrt = NULLI;
		/* if x & y are set, mark the angle as unset to parse later (for wrt) */
		if(obj->x != NULLI && obj->y != NULLI)
			obj->angle = NULLI;	
	}
	/* set some dots things */
	objAP[VSD_OBJECT_DT]->vertex 		= VSD_OBJECT_AP;
	objAP[VSD_OBJECT_DT]->amplitude 	= 1;

	/* adding junko to deal with the possibility that we
	** generate target positions that are very similar...
	** the logic is to save the current positions, parse (with
	** possible random offsets), test if they're too close,
	** & if so try again (a limited number of times)
	*/
	dsave[0] = objAP[VSD_OBJECT_T1]->x;
	dsave[1] = objAP[VSD_OBJECT_T1]->y;
	dsave[2] = objAP[VSD_OBJECT_T2]->x;
	dsave[3] = objAP[VSD_OBJECT_T2]->y;
	dsave[4] = objAP[VSD_OBJECT_TS]->x;		/* ts */
	dsave[5] = objAP[VSD_OBJECT_TS]->y;
	dcount   = 15;

	/**
	*** Call recursive "vsd_parse_objects" to try to set the rest 
	***  put in a loop to make sure targets are sufficiently separated
	***/
	again:
		if(GOOD_SUN(task_info))
			vsd_parse_objects(objAP, task_info->object_1);
		if(GOOD_PLANET(task_info)) 
			vsd_parse_objects(objAP, task_info->object_2);
		vsd_parse_objects(objAP, VSD_OBJECT_AP);
		vsd_parse_objects(objAP, VSD_OBJECT_TC);
		vsd_parse_objects(objAP, VSD_OBJECT_T1);
		vsd_parse_objects(objAP, VSD_OBJECT_T2);
		vsd_parse_objects(objAP, VSD_OBJECT_TS);	/* ts */
		vsd_parse_objects(objAP, VSD_OBJECT_DT);

	/* if x,y set explicitly beforehand, don't test */
	do_again = 0;									/* ts */
	for ( i=0 ; i < 3 ;i++ )	{					/* ts */
		switch ( i )	{
			case 0:		j1=0;	m1=VSD_OBJECT_T1; j2=2;	m2=VSD_OBJECT_T2; break;
			case 1:		j1=0;	m1=VSD_OBJECT_T1; j2=4;	m2=VSD_OBJECT_TS; break;
			case 2:		j1=2;	m1=VSD_OBJECT_T2; j2=4;	m2=VSD_OBJECT_TS; break;
		}
		if((dsave[j1]==NULLI || dsave[j1+1]==NULLI ||
			dsave[j2]==NULLI || dsave[j2+1]==NULLI) && --dcount > 0) { 
			dist = toy_mag(objAP[m1]->x-objAP[m2]->x,
							objAP[m1]->y-objAP[m2]->y);
			if(dist < 30) {
				objAP[m1]->x = dsave[j1];
				objAP[m1]->y = dsave[j1+1];
				objAP[m2]->x = dsave[j2];
				objAP[m2]->y = dsave[j2+1];
				do_again = 1;
				break;
			}
		}
	}
	if(do_again)
		goto again;
		
	
	/**
	*** set the dots info
	***/
	if(SHOW_DOTS(rec)) {
		dot = VSD_GET_DTOBJECT(rec);
		
		/* IF value is given in "dots", it's an override so use it
		** otherwise figure out something...
		*/
		if(dot->coherence == NULLI)
			dot->coherence = trial_info->coherence != NULLI ? trial_info->coherence : 0;
		if(dot->speed 		== NULLI)
			dot->speed 		= VSD_DOT_SPEED;
		if(dot->direction == NULLI) {
			dot->direction = VSD_GET_DT(rec)->angle;
			
		}
		if(dot->novar_pct	== NULLI)
			dot->novar_pct	= 0;
		
		/* VAR/NOVAR cconditions:
		** VAR = send a different seed each time
		** the seed is the product of two numbers, each
		** in the range 0-999 (so we can send each as a paramter
		** to MATLAB and be able to store it using the ecode messages
		*/
		if(TOY_RAND(100) > ((int) (dot->novar_pct))) {
			dot->seed_base = TOY_RAND(1000);
			dot->seed_var 	= TOY_RAND(1000);
		/* NOVAR: send the same seed each time (for the same
		** coherence and direction)... seed is base *
		** (coherence/10 + <dir=0,1>
		*/
		} else {
			/* Luke is commenting out the following line and replacing */
			//dot->seed_base = dot->seed_base != NULLI ? dot->seed_base : seed_base; 
			//dot->seed_var	= (int) (dot->coherence/10 + (cos(DEG2RAD*dot->direction) >= 0)); 

			dot->seed_base = trial_info->seed_base != NULLI ? trial_info->seed_base : seed_base;
			dot->seed_var = trial_info->seed_var != NULLI ? trial_info->seed_var : 0;
			printf("\n\nThe random number generator was seeded with the following: %u\n", rec->seed);
			printf("Seed base and seed var for this particular trial are: %d %d\n", dot->seed_base, dot->seed_var);
		}	
	}
	
	/** 
	*** set T1/T2...
	*** 	if t_flag == 0, this means interpret T1/T2 as correct/wrong
	***	    if t_flag == 1, interpret T1/T2 as left/right
	***		if t_flag == 2, determine correct target based on dots direction
	***		if t_flag == 3, T1 is correct for rightward motion and T2 for leftward motion. 
	***						The correct target reverses if rev_dot_targ_association is set.
	***/
		
	rec->display->t_sbet = VSD_OBJECT_TS;			/* ts */
	dot = VSD_GET_DTOBJECT(rec);
	dot_ang   = dot->direction;
	 
	if(task_info->t_flag == 0) {
		rec->display->t_correct = VSD_OBJECT_T1;
		rec->display->t_wrong 	= VSD_OBJECT_T2;
		
		if(cos(DEG2RAD*((VSD_GET_DT(rec))->angle)) >= 0) {
			rec->display->t_left  = VSD_OBJECT_T2;
			rec->display->t_right = VSD_OBJECT_T1;
		} else {
			rec->display->t_left  = VSD_OBJECT_T1;
			rec->display->t_right = VSD_OBJECT_T2;
		}
		
	} else if(task_info->t_flag == 1) {
		rec->display->t_left 	= VSD_OBJECT_T1;
		rec->display->t_right 	= VSD_OBJECT_T2;
		
		if(cos(DEG2RAD*((VSD_GET_DT(rec))->angle)) >= 0) {
			rec->display->t_correct = VSD_OBJECT_T2;
			rec->display->t_wrong 	= VSD_OBJECT_T1;
		} else {
			rec->display->t_correct = VSD_OBJECT_T1;
			rec->display->t_wrong 	= VSD_OBJECT_T2;
		}
		
	} else if(task_info->t_flag == 2) {
		dx = (VSD_GET_T1(rec))->x - (VSD_GET_T2(rec))->x;
		dy = (VSD_GET_T1(rec))->y - (VSD_GET_T2(rec))->y;
		dtarg_ang = toy_atan(dx,dy);
		del_ang   = abs(dtarg_ang - dot_ang);
		
		if(del_ang>180)
			del_ang = 360 - del_ang;
		
		if(del_ang <= 90) {
			rec->display->t_correct = VSD_OBJECT_T1;
			rec->display->t_wrong   = VSD_OBJECT_T2;
		} else {
			
			rec->display->t_correct = VSD_OBJECT_T2;
			rec->display->t_wrong   = VSD_OBJECT_T1;
		}
		
	} else if (task_info->t_flag == 3) {
		if(cos(DEG2RAD*dot_ang)>=0) { /** rightward motion, T1 is correct unless reverse association is set **/
			if (rev_dot_targ_association==0)	{			/* direct association */
				rec->display->t_correct = VSD_OBJECT_T1;
				rec->display->t_wrong =   VSD_OBJECT_T2;
			} else {										/* reverse association */
				rec->display->t_correct = VSD_OBJECT_T2;
				rec->display->t_wrong   = VSD_OBJECT_T1;
			}
		} else {			    		/** leftward motion, T2 is correct unless reverse association is set **/
			if (rev_dot_targ_association==0)	{			/* direct association */
				rec->display->t_correct = VSD_OBJECT_T2;
				rec->display->t_wrong =   VSD_OBJECT_T1;
			} else {										 /* reverse association */
				rec->display->t_correct = VSD_OBJECT_T1;
				rec->display->t_wrong   = VSD_OBJECT_T2;
			}
		}

				
	
	} else {
		printf("oops -- bad tflag\n");
	}
		
 }

/* PUBLIC ROUTINE: vsd_clear_display
**
*/
void vsd_clear_display(_VSdisplay display)
 {
	/* clear the display objects */
	if(display && display->object_array) {
		register int i;
		
		display->t_correct	= 0;
		display->t_wrong 	= 0;
		display->t_left 	= 0;
		display->t_right 	= 0;
		display->t_sbet		= 0;		/* ts */
		
		for(i=0;i<VSD_NUM_OBJECTS-1;i++)
			vs_clear_object(display->object_array[i]);
		vsd_clear_dot_object(display->object_array[i]);
	}
 }


/* PUBLIC ROUTINE: vsd_print_display
**
*/
void vsd_print_display(_VSdisplay display)
 {
	if(display) {
		register int i;

		printf("display num objects = %d (C=%d,W=%d,L=%d,R=%d, SBET=%d)\n",
			display->num_objects, display->t_correct, display->t_wrong,
			display->t_left, display->t_right, display->t_sbet);	/* ts */
		for(i=0;i<display->num_objects;i++)
			vsd_print_object(display->object_array[i]);
	}
 }


/* OBJECT */

/* PUBLIC ROUTINE: vsd_init_dot_object
**
**	 dots-specific display object information
*/
_VSobject vsd_init_dot_object(void)
{
	_VSobject vso 					= vs_init_object();
	_VSDdot_object dot_object 	= SAFE_STALLOC(_VSDdot_object_struct);

	dot_object->coherence 	= 0;
	dot_object->direction 	= 0;
	dot_object->speed 	 	= 0;
	dot_object->novar_pct 	= 0;
	dot_object->seed_base 	= 0;
	dot_object->seed_var 	= 0;

	vso->object_info = dot_object;

	return(vso);
}

/* PUBLIC ROUTINE: vsd_copy_dot_object
**
*/
void vsd_copy_dot_object(_VSDdot_object dot_from, _VSDdot_object dot_to)
{
	if(dot_from && dot_to) {
		dot_to->coherence = dot_from->coherence;
		dot_to->direction = dot_from->direction;
		dot_to->speed 		= dot_from->speed;
		dot_to->novar_pct = dot_from->novar_pct;
		dot_to->seed_base = dot_from->seed_base;
		dot_to->seed_var 	= dot_from->seed_var;
	}
}

/* PUBLIC ROUTINE: vsd_clear_dot_object
**
*/
void vsd_clear_dot_object(_VSobject vso)
{
	if(vso && vso->object_info) {
		_VSDdot_object dot_object = (_VSDdot_object) (vso->object_info);

		/* clear the dots info */
		dot_object->coherence 	= 0;
		dot_object->direction 	= 0;
		dot_object->speed 	 	= 0;
		dot_object->novar_pct 	= 0;
		dot_object->seed_base 	= 0;
		dot_object->seed_var 	= 0;
	}

	/* clear the object */
	vs_clear_object(vso);
}

/* PUBLIC ROUTINE: vsd_free_dot_object
**
*/
void vsd_free_dot_object(_VSobject vso)
{
	if(vso && vso->object_info)
		SAFE_FREE(vso->object_info);

	/* free the object */
	vs_free_object(vso);
}

/* PUBLIC ROUTINE: vsd_print_object
**
*/
void vsd_print_object(_VSobject vso)
{
	if(vso) {
		printf("   Object(%d) x/y/d/s/r/t = %d/%d/%d/%d/%d/%d\n",
			vso->matlab_index, vso->x, vso->y, vso->diameter, vso->shown,
			vso->amplitude, vso->angle);
		printf("\twrt/vertex/RGB           = %d/%d/[%d %d %d]\n",
			vso->wrt, vso->vertex, vso->color.R, vso->color.G, vso->color.B);

		if(vso->object_info) {
			_VSDdot_object dot = (_VSDdot_object) (vso->object_info);
			printf("\tDOT INFO coh/dir/speed/novar/seeda/b = %d/%d/%d/%d/%d/%d\n",
				dot->coherence, dot->direction, dot->speed,
				dot->novar_pct, dot->seed_base, dot->seed_var);
		}
	}
}

	/* UTILITIES */


/* PRIVATE ROUTINES */

#define RECUR_MARKER -999

/* PRIVATE ROUTINE: vsd_parse_objects
**
**	actually not-so-bad recursive routine (okok, it's
** horrible. so sue me. but remember -- it actutally
** works). It tries to figure out x,y,angle of the given object.
**	Recursive because the computation depends on
** the position of the "vertex" object (determines
**	how "angle" is interpreted") and the position of
**	the "wrt" object (additive offset of the angle
**	of the computed object).
**
**	Arguments:
**		objA	... array of pointers to the _VSobject structures 
**						(see vs.h) in rec->display
**		index ... index of the object being parsed (see vs_dots.h)
*/ 
static void vsd_parse_objects(_VSobject *objA, int index)
{
	_VSobject obj = objA[index];
	int wrt, vx, vy;

	/**
	*** if it's already done, leave 
	***/
	if((obj->x != NULLI && obj->y != NULLI) || (obj->r == RECUR_MARKER))
		return;

	/**
	*** get the wrt angle 
	***/
	if(obj->wrt == NULLI || obj->wrt == index) {
		wrt = 0;

	} else if(objA[obj->wrt]->angle != NULLI) {
		wrt = objA[obj->wrt]->angle;

	} else { 	/* recursively get wrt angle */

		obj->r = RECUR_MARKER; /* avoid infinite recursion */
		vsd_parse_objects(objA, obj->wrt);
		obj->r = NULLI;

		/* use it if it worked */
		wrt = objA[obj->wrt]->angle != NULLI ? objA[obj->wrt]->angle : 0;
	}

	/** 
	*** get the vertex
	***/
	if(obj->vertex == index) {	 /* means interpret r/theta as x/y */
		if(obj->x == NULLI && obj->y == NULLI) {
			obj->x = obj->amplitude;
			obj->y = obj->angle;
		} else if(obj->angle == NULLI) {
			obj->angle = 0;
		}
		return;

	} else if(objA[obj->vertex]->x != NULLI && objA[obj->vertex]->y != NULLI) {
		vx = objA[obj->vertex]->x;
		vy = objA[obj->vertex]->y;

	} else { /* recursively get vertex x,y */

		obj->r = RECUR_MARKER;
		vsd_parse_objects(objA, obj->vertex);
		obj->r = NULLI;

		/* use it if it worked */
		if(objA[obj->vertex]->x != NULLI && objA[obj->vertex]->y != NULLI) {
			vx = objA[obj->vertex]->x;
			vy = objA[obj->vertex]->y;
		} else {
			vx = 0;
			vy = 0;
		}
	}

	/**
	*** finally compute x, y, angle
	***/
	if(obj->angle == NULLI && obj->x != NULLI && obj->y != NULLI)
		obj->angle = toy_atan(obj->x - objA[obj->vertex]->x,
									 obj->y - objA[obj->vertex]->y);

	/* jig & mm changed 5/7/02 to include wrt in ALL cases,
	** not just when angle not given
	** fixed by jd 1/6/03
	*/
	if(obj->angle != NULLI)
		obj->angle += wrt + obj->angle_offset;
	else
		obj->angle = wrt + obj->angle_offset;

	if(obj->x == NULLI || obj->y == NULLI) {
		obj->x = TOY_RT_TO_X(vx, obj->amplitude, obj->angle);
		obj->y = TOY_RT_TO_Y(vy, obj->amplitude, obj->angle);
	} 

	if(obj->x == NULLI) obj->x = 0;
	if(obj->y == NULLI) obj->y = 0;
}

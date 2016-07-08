/*
** VS.C
**
** "VisualStimulation.c" ... to make sets of visual
**	stimulation records
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <sys/types.h>
#include "rexhdrs.h"

#include "toys.h"
#include "vs.h"
#include "vs_dots.h"

/* PRIVATE ROUTINE PROTOTYPES */

/* RECORD */

/* PUBLIC ROUTINE: vs_init_record
**
*/
_VSrecord vs_init_record(void)
 {
	_VSrecord rec = SAFE_STALLOC(_VSrecord_struct);

	rec->num_repetitions	= 0;
	rec->total				= 0;
	rec->correct			= 0;
	rec->wrong				= 0;
	rec->ncerr				= 0;
	rec->brfix				= 0;
    rec->brhandfix   		= 0;
	rec->sbet				= 0;	/* ts */
	rec->sbet_total			= 0;	/* ts */
	rec->last				= -1;
	
	rec->num_tasks 			= 0;
	rec->task_array			= NULL;
	
	rec->num_trials			= 0;
	rec->trial_array		= NULL;
	rec->cur_trial_index	= 0;
	rec->cur_trial			= NULL;
	
	rec->display			= NULL;
	
	rec->seed               = NULLI; /* for seeding dots */
	 
	/* seed the random # generator */
	srand((unsigned int) clock());
	
	return(rec);
 }


/* PUBLIC ROUTINE: vs_clear_record
**
*/
void vs_clear_record(_VSrecord rec)
 {
	if(rec) {
	
	/* don't clear "total" so we can keep
	** track of the total # of trials even
	** if we re-set the tasks
	**	rec->total				= 0;
	*/
		rec->num_repetitions	= 0;
		rec->correct			= 0;
		rec->wrong				= 0;
		rec->ncerr				= 0;
		rec->brfix				= 0;
		rec->brhandfix			= 0;
		rec->sbet				= 0;	/* ts */
		rec->sbet_total			= 0;	/* ts */
		rec->last				= -1;
	}
 }


/* PUBLIC ROUTINE: vs_free_record
**
**	This should be called AFTER vs*_free_record
**	has been called to free any of the *-specific
** structures
*/
void vs_free_record(_VSrecord rec)
{
	if(rec && rec->task_array) {
		fprintf(stderr, "vs_free_record: free tasks first\n");
		return;
	}
	
	SAFE_FREE(rec);
}


/* TASK */

/* PUBLIC ROUTINE: vs_init_task
**
** initialize a _VStask data structure and return
** a pointer
*/
_VStask vs_init_task(void)
 {
	_VStask task 			= SAFE_STALLOC(_VStask_struct);

	task->name 				= NULL;
	task->id				= 0;
	task->reps				= 0;
		
	task->num_trials		= 0;
	task->trial_array		= NULL;
	
	task->task_info			= NULL;
	
	return(task);
 }


/* PUBLIC ROUTINE: vs_free_task
**
*/
void vs_free_task(_VStask task)
 {
	if(task && (task->task_info || task->trial_array)) {
		fprintf(stderr, "vs_free_task: free trial array &/or task info first\n");
		return;
	}
	
	SAFE_FREE(task->name);
	SAFE_FREE(task);
 }


/* PUBLIC ROUTINE: vs_rec_add_task
**
*/
void vs_rec_add_task(_VSrecord rec, _VStask task)
 {
	if(!rec || !task)
		return;
		
	/* allocate a new ptr in the array */
	if(!(rec->num_tasks))
		rec->task_array = (_VStask *) SAFE_MALLOC(sizeof(_VStask));
	else 
		rec->task_array = (_VStask *) SAFE_REALLOC(rec->task_array,
									((rec->num_tasks + 1) * sizeof(_VStask)));
	
	/* set it & update the count */
	rec->task_array[rec->num_tasks++] = task;	
 }


/* TRIAL */

/* PUBLIC ROUTINE: vs_init_trial
**
*/
_VStrial vs_init_trial(void)
{
	_VStrial trial = SAFE_STALLOC(_VStrial_struct);
	
	trial->id						= 0;
	trial->reps						= 1;
	
	trial->total					= 0;
	trial->correct 					= 0;
	trial->null 					= 0;
	trial->ncerr 					= 0;
	trial->brfix 					= 0;
	trial->sbet						= 0;	/* ts */
	trial->sbet_total				= 0;	/* ts */
	
	trial->trial_info				= NULL;
	trial->trial_performance		= NULL;
	
	trial->task						= NULL;
	trial->perf_loopback     		= NULL;	

	return(trial);
 }


/* PUBLIC ROUTINE: vs_free_trial
**
*/
void vs_free_trial(_VStrial trial)
 {
	if(trial && trial->trial_info) {
		fprintf(stderr, "vs_free_trial: free trial info first\n");
		return;
	}
	
	SAFE_FREE(trial);
 }

// Luke's function
/* PUBLIC ROUTINE: vs_copy_trial
**
*/
void vs_copy_trial(_VStrial trial_from, _VStrial trial_to)
{

	if (trial_from && trial_to) {
	
		trial_from->id                   = 		trial_to->id;	
		trial_from->reps		         =		trial_to->reps;
	
		trial_from->total		         =		trial_to->total;
		trial_from->correct 			 = 		trial_to->correct;
		trial_from->null 				 =		trial_to->null;
		trial_from->ncerr 				 =		trial_to->ncerr; 					
		trial_from->brfix 				 = 		trial_to->brfix;
		trial_from->sbet				 = 		trial_to->sbet;
		trial_from->sbet_total			 = 		trial_to->sbet_total;
   
		trial_from->trial_info			 = 		trial_to->trial_info;
		trial_from->trial_performance	 = 		trial_to->trial_performance;
	
		trial_from->task				 = 		trial_to->task;
	}
 }


/* PUBLIC ROUTINE: vs_score_trial
**
**	Score trial as Correct (1), Wrong (0), 
**	no choice error (2), broken fixation (3) or
**  sure bet (4)
*/
void vs_score_trial(_VSrecord rec, int score, int sbet_shown)
 {
	/* make sure there's a current trial */
	if(!rec || !rec->cur_trial)
		return;
	
	/* keep track of record info */
	rec->last = score;
	
	/* mark the trial */
	switch(score) {
		case CORRECT:
			rec->total++;
			rec->correct++;
			rec->cur_trial->perf_loopback->total++;
			rec->cur_trial->perf_loopback->correct++;
			break;
		case WRONG:
			rec->total++;
			rec->wrong++;
			rec->cur_trial->perf_loopback->total++;
			rec->cur_trial->perf_loopback->null++;
		    
		    /* Luke was here */
		    //rec->last = DO_OVER; // duh, redo incorrects
			break;
		case NCERR:
			rec->ncerr++;
			rec->cur_trial->perf_loopback->ncerr++;
			break;
		case BRFIX:
			rec->brfix++;
			rec->cur_trial->perf_loopback->brfix++;
			break;
		case SBET:
			rec->total++;				/* ts */
			rec->sbet++;
			rec->cur_trial->perf_loopback->total++;
			rec->cur_trial->perf_loopback->sbet++;
		    break;
	    default:
	        //do nothing, this is for no fixation acquired
		    break; 
	}
	if ( (score==CORRECT || score==WRONG || score==SBET) && sbet_shown )	{	/* ts */
		rec->sbet_total++;
		rec->cur_trial->perf_loopback->sbet_total++;
	}
 }


/* PUBLIC ROUTINE: vs_get_next_trial
**
**	Arguments:
**		rec  				... the big kahuna, see vs.h
**		min_block_size ... sent to vs_make_trial_array
**		randomize_flag ... sent to vs_make_trial_array
*/
_VStrial vs_get_next_trial(_VSrecord rec, int min_block_size,
			int randomize_flag)
 {
	/* return if there's nothing there */
	if(!rec) 
		return(NULL);
	
	/* make the trial array if it's not there */
	if(!rec->trial_array) {
		/* Are we already done with the repetitions? */
		if(--rec->num_repetitions < 0){
			rec->cur_trial=NULL;
			return(NULL);
		};
		
		vs_make_trial_array(rec, min_block_size, randomize_flag);
		return(rec->cur_trial);
	}
	
	/* If there is a trial array, get the next trial based on
	** the last "score"
	*/
	switch(rec->last) {
		
		case WRONG: case CORRECT: case SBET:
			/* go to the next trial, but if you're at the end
			** of the block you have to re-shuffle...
			*/
		
			if(++rec->cur_trial_index >= rec->num_trials){
				/* Are we already done with the repetitions? */
				if(--rec->num_repetitions < 0){
					rec->cur_trial=NULL;
					return(NULL);
				};
				
				vs_make_trial_array(rec, min_block_size, randomize_flag);
			};
			rec->cur_trial = rec->trial_array[rec->cur_trial_index];
			return(rec->cur_trial);
				
    	case NCERR: case BRFIX: 
			/* keep in mind that this could be problematic ...
			** if you're at the end of a block and get an
			** NCERR, then this means that you'll just keep
			** giving the same trial until a CORRECT/WRONG choice
			*/
			return(vs_swap_current_trial(rec));
			
	    case DO_OVER:
			/* just do it again 		*/
			return(rec->cur_trial); 
			
		default:
			/* take the first trial */
			rec->cur_trial 		= rec->trial_array[0];
			rec->cur_trial_index = 0;
			return(rec->cur_trial);
	}
 }


/* PUBLIC ROUTINE: vs_make_trial_array
**
** set up the "trial_array" list of trials
** in the _VSrecord structure.
**
** Arguments:
**		min_block_size ... the minimum number of trials that can
**				be in the trial array. We need this because if a block
**				is too small the subject can anticipate (and gain
**				some advantage by anticipating) the end of a block...
**		randomize_flag	... to choose between the types of randomization
** 			    schemes available
**
*/
void vs_make_trial_array(_VSrecord rec, long min_block_size, long randomize_flag)
{
	register int i, j, k, l, m, count;
	int num_blocks;
	_VStask 	*taP;
	_VStrial    *trP, *tr2P;
	
	/* Luke was here, for repeating dot seeds */
	_VStrial trTo;
	_VSDtrial_info trial_info_orig, trial_info_new;

	/* for repeating a particular seed for dot stimuli */
	int seed_base, seed_var, repeat = 0;
	
	
	if(!rec || !rec->num_tasks)
	  return;
	
	/* clear the list if it's there */
	SAFE_FREE(rec->trial_array);
	
	/* count the number of trials to include */
	for(count=0,taP=rec->task_array,i=rec->num_tasks;i>0;i--,taP++)
	  for(trP=(*taP)->trial_array,j=(*taP)->num_trials;j>0;j--,trP++)
		count += ((*taP)->reps * (*trP)->reps);
	
	/* return if there's nothing */
	if(!count)
	  return;
	
	/* calculate the number of blocks */
	num_blocks = (int) ceil((double)min_block_size/(double)count);
	count	   = num_blocks * count;

	
	/* allocate the array */
	tr2P = rec->trial_array = SAFE_ZALLOC(_VStrial, count);

	
	/* overwrite random seed? */
	/* CAREFUL!!! each time we run through a repetition of a task, seeds will be repeated */
	if (rec->seed != NULLI) {
	  srand((unsigned int) rec->seed);
	} else {
		rec->seed = (unsigned int) clock();
		srand(rec->seed);
	}

	
	/* fill that baby with pointers to all the trials */
	for(i=num_blocks;i>0;i--)
	  for(taP=rec->task_array,j=rec->num_tasks;j>0;j--,taP++)
		for(k=(*taP)->reps;k>0;k--)
		  for(trP=(*taP)->trial_array,l=(*taP)->num_trials;l>0;l--,trP++)
			for(m=(*trP)->reps;m>0;m--) {
				/* Luke was here */
				/* This particular way of repeating seeds ruins the online performance tracking*/
				/* That's OK for me since I have Python on the Plexon side doing this */
				/* I have now actually fixed this by adding a loopback pointer... yes, very spaghetti,
				 * but it was the simplest thing to do; look at perf_loopback */
				
				// I hope there isn't a memory leak somewhere 
				
			    _VStrial trTo = SAFE_STALLOC(_VStrial_struct);
				_VSDtrial_info trial_info_orig = (_VSDtrial_info) (*trP)->trial_info;
				trial_info_new = SAFE_STALLOC(_VSDtrial_info_struct);				

				trTo->id                 = 		(*trP)->id;
				trTo->reps		         =		(*trP)->reps;
				trTo->total		         =		(*trP)->total;
				trTo->correct 			 = 		(*trP)->correct;
				trTo->null 				 =		(*trP)->null;
				trTo->ncerr 			 =		(*trP)->ncerr; 					
				trTo->brfix 			 = 		(*trP)->brfix;
				trTo->sbet				 = 		(*trP)->sbet;
				trTo->sbet_total		 = 		(*trP)->sbet_total;
   
				trial_info_new->planet_angle	= trial_info_orig->planet_angle;
				trial_info_new->coherence		= trial_info_orig->coherence;
		
				trial_info_new->seed_base		= 0;
				trial_info_new->seed_var		= 0;						

				/* repeat each seed twice, assumes an even number of reps per dir/coh combo */
				if (repeat == 0) {
					seed_base = TOY_RAND(1000);
					seed_var  = TOY_RAND(1000);
					trial_info_new->seed_base = seed_base;
					trial_info_new->seed_var = seed_var;
					repeat = 1;
				} else if (repeat == 1) {
					trial_info_new->seed_base = seed_base;
					trial_info_new->seed_var = seed_var;					
					repeat = 0;
				}
				
				trTo->trial_info		 =      trial_info_new;
				trTo->trial_performance	 = 		(*trP)->trial_performance;
	
				trTo->task				 = 		(*trP)->task;
				trTo->perf_loopback  	 = 		(*trP)->perf_loopback;
				
				*tr2P++ = trTo;
				/* Luke stopped */
				
				// this is what was done before //
				//*tr2P++ = *trP;
			}
	
	/* randomize, if necessary */
	if(randomize_flag == 1) {
		/* reseed, because above we seeded it just to repeat same seeds, not same trial order */
		srand((unsigned int) clock());
		_VStrial *dummy_array;
		
		/* get pointer to trial array & make a dummy array */
		trP 	= rec->trial_array;
		tr2P 	= dummy_array = SAFE_ZALLOC(_VStrial, count);
		
		/* loop through, gathering random trials */
		for(i=0,j=count;j>0;j--,i++) {
			k = TOY_RAND(j);
			printf("%d ", k);
			tr2P[i] = trP[k];
			trP[k]	= trP[j-1];
		}

		/* move the dummy array into trial_array */
		SAFE_FREE(rec->trial_array);
		rec->trial_array = dummy_array;
	}

	/* clean up & outta */
	rec->num_trials		 = count;
	rec->cur_trial_index = 0;
	rec->cur_trial 		 = rec->trial_array[0];
}

/* PUBLIC ROUTINE: vs_swap_current_trial
**
** if a trial was aborted and we're gonna do it again,
** we'll put it back at some random location in the array
** return the randomly chosen trial, which is now current
** note: it is possible to select the cur trial again
*/
_VStrial vs_swap_current_trial(_VSrecord rec)
{
	int  		index, remain;

	/* make sure we got trials remaining */
	if(!rec || !rec->num_trials || !rec->trial_array)
		return(NULL);

	remain = rec->num_trials - rec->cur_trial_index;
	if(remain == 1) return(rec->cur_trial);

	/* choose a random # between cur and end */
	index = TOY_RAND(remain);

	/* swap */
	rec->cur_trial = rec->trial_array[rec->cur_trial_index + index];
	rec->trial_array[rec->cur_trial_index + index] =
						  rec->trial_array[rec->cur_trial_index];
	rec->trial_array[rec->cur_trial_index]	= rec->cur_trial;

	return(rec->cur_trial);
}

/* DISPLAY */

/* PUBLIC ROUTINE: vs_init_display 
**
*/
_VSdisplay vs_init_display(void)
 {
	_VSdisplay vsd = SAFE_STALLOC(_VSdisplay_struct);

	/* object array */
	vsd->num_objects 	= 0;
	vsd->object_array = NULL;

	vsd->t_correct		= 0;
	vsd->t_wrong		= 0;
	vsd->t_left			= 0;
	vsd->t_right		= 0;
	vsd->t_sbet			= 0;	/* ts */
	
	return(vsd);
 };

/* PUBLIC ROUTINE: vs_clear_display 
**
*/
void vs_clear_display(_VSdisplay vsd)
 {
	if(vsd) {

		vsd->t_correct		= 0;
		vsd->t_wrong		= 0;
		vsd->t_left			= 0;
		vsd->t_right		= 0;
		vsd->t_sbet			= 0;	/* ts */
	}
 }

/* PUBLIC ROUTINE: vs_free_display 
**
*/
void vs_free_display(_VSdisplay vsd)
{
	if(vsd && vsd->num_objects) {
		fprintf(stderr,"vs_free_display: free objects first\n");
		return;
	}

	SAFE_FREE(vsd);
}

/* PUBLIC ROUTINE vs_display_add_object
**
**
*/
void vs_display_add_object(_VSdisplay vsd, _VSobject vso)
{
	if(!vsd || !vso)
		return;

	/* allocate a new ptr in the array */
	if(!(vsd->num_objects))
		vsd->object_array = (_VSobject *) SAFE_MALLOC(sizeof(_VSobject));
	else
		vsd->object_array = (_VSobject *) SAFE_REALLOC(vsd->object_array,
									((vsd->num_objects + 1) * sizeof(_VSobject)));

	/* set it & update the count */
	vsd->object_array[vsd->num_objects++] = vso;
}

/* OBJECT */

/* PUBLIC ROUTINE: vs_init_object 
**
*/
_VSobject vs_init_object(void)
{
	_VSobject vso = SAFE_STALLOC(_VSobject_struct);

	vso->matlab_index	= 0;
	vso->x				= 0;
	vso->y				= 0;
	vso->amplitude		= 0;
	vso->angle			= 0;
	vso->angle_offset	= 0;
	vso->wrt			= 0;
	vso->vertex			= 0;
	vso->diameter		= 0;
	vso->color.R        = 0;
	vso->color.G        = 0;
	vso->color.B        = 0;
	vso->r              = 0;
	vso->shown			= 0;

	vso->object_info	= NULL;

	return(vso);
}

/* PUBLIC ROUTINE: vs_copy_object 
**
*/
void vs_copy_object(_VSobject vso_from, _VSobject vso_to)
{
	if(vso_from && vso_to) {
	/* DO NOT COPY MATLAB INDEX
	**	vso_to->matlab_index	= vso_from->matlab_index;
	*/
		vso_to->x				= vso_from->x;
		vso_to->y				= vso_from->y;
		vso_to->amplitude		= vso_from->amplitude;
		vso_to->angle			= vso_from->angle;
		vso_to->angle_offset	= vso_from->angle_offset;
		vso_to->wrt				= vso_from->wrt;
		vso_to->vertex			= vso_from->vertex;
		vso_to->diameter		= vso_from->diameter;
		vso_to->color.R			= vso_from->color.R;
		vso_to->color.G			= vso_from->color.G;
		vso_to->color.B			= vso_from->color.B;
		vso_to->r				= vso_from->r;
		vso_to->shown			= 0;		/* Mark as not shown */
	}
}

/* PUBLIC ROUTINE: vs_clear_object 
**
*/
void vs_clear_object(_VSobject vso)
{
	if(vso) {
		/* NOTE: DO NOT CLEAR OBJECT INDEX & OBJECT INFO */
		vso->x				= 0;
		vso->y				= 0;
		vso->amplitude		= 0;
		vso->angle			= 0;
		vso->angle_offset	= 0;
		vso->wrt			= 0;
		vso->vertex			= 0;
		vso->diameter		= 0;
		vso->color.R		= 0;
		vso->color.G		= 0;
		vso->color.B		= 0;
		vso->r				= 0;
		vso->shown			= 0;
	}
}

/* PUBLIC ROUTINE: vs_free_object 
**
*/
void vs_free_object(_VSobject vso)
{
	if(vso && vso->object_info) {
		fprintf(stderr, "vs_free_object: free object info first\n");
		return;
	}

	SAFE_FREE(vso);
}

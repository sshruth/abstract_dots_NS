/*
** VS.H
**
** "VisualStimulation.h" ... header file that contains
**	data structures and public routine prototypes dealing
** with standardized methods for generating visual
**	simuli in a REX paradigm...
*/

#ifndef VS_H_
#define VS_H_

/* DATA STRUCTURES */

typedef struct RGB_struct			RGB;
struct RGB_struct {
	int R, 
	  G,
	  B;
};


/* these are the data structures defined in this file
**
**	_VSobject 	-- a screen object (i.e., fixation point, dots, etc.)
** _VSdisplay	-- a collection of screen objects
** _VStrial		-- info needed for single trial (display presentation)
** _VStask		-- a collection of like trials
** _VSrecord	-- the big kahuna. everything needed for an experiment
*/
typedef struct _VSobject_struct 	*_VSobject;
typedef struct _VSdisplay_struct	*_VSdisplay;
typedef struct _VStrial_struct	*_VStrial;
typedef struct _VStask_struct		*_VStask;
typedef struct _VSrecord_struct	*_VSrecord;

/* _VSobject_struct
**
** stimulus objects... position, etc.
**
**	for now I'll include diameter & color info, but
**	at some point these might be object-specific (e.g.,
**	object w/multiple colors)
*/
struct _VSobject_struct {

	int	matlab_index;	/* index of ShadlenDots Target */

	/* position in cartesian coordinates
	*/
	int	x;
	int	y;
	
	/* position in polar coordinates relative to
	** a given center point
	*/
	int	amplitude;
	int	angle;
	int	angle_offset;

	int	wrt;			/* object defining offset of angle */
	int	vertex;		/* object defining origin of polar coordinates */

	/* size
	*/
	int 	diameter;

	/* color info
	*/
	RGB color;
	
	int r;

	/* flag 
	*/
	int	shown;

	/* pointer to a structure containing
	** object-specific information
	*/
	void *object_info;
};
	
/* _VSdisplay_struct
**
**	arrays of DOTS and TARGETS structures
*/
struct _VSdisplay_struct {
	
	int		  num_objects;		/* dots, targets, fix, ...	*/
	_VSobject *object_array;

	int		  t_correct;		/* index of correct target object 	*/
	int		  t_wrong;			/* index of incorrect target object */
	int		  t_left;			/* index of left target object		*/
	int		  t_right;			/* index of right target object		*/	
	int		  t_sbet;			/* index of sure bet target object 	*/	/* ts */
};

/* _VStrial_struct
**
**	6/1/01 jig added "reps"... this is just a way of 
**	incorporating changes in prior probability. This value
** defines the # of times that this trial is repeated in 
** a block of trials. Thus, if there are two trials and TR1
** has reps=1 and TR2 has reps=2, then 2 TR2's will be presented
** for each TR1.
*/
struct _VStrial_struct {

	long			id;
	long			reps;			/* # of times trial is presented in block */

	long			total;
	long			correct;		/* correct responses 						*/
	long			null;			/* null responses (choose wrong target) 	*/
	long			ncerr;			/* no-choice responses						*/
	long			brfix;			/* break fixation							*/
	long			brhandfix;		/* break hand fixation						*/
	long			sbet;			/* sure bet responses */ /* ts */
	long			sbet_total;		/* number of time that the sure bet was presented */ /* ts */

	void 		  *trial_info;				/* paradigm-specific */
	void		  *trial_performance;	/* paradigm-specific */

	_VStrial    perf_loopback;
	_VStask		task;			/* ptr back to task */
};

/* _VStask_struct
**
**		this structure is kind of strange and really
**		just a place-holder for paradigm-specific info
*/
struct _VStask_struct {

	char		 *name;
	long		  id;				/* paradigm-specific */
	long		  reps;			/* was "proportion" in 712.d */

	int		  num_trials;
	_VStrial  *trial_array;

	void	 	 *task_info;	/* paradigm-specific */
};

/* _VSrecord_struct
**
**	the big kahuna. keeps track of all of the
**	trials, tasks, and stimuli used in an experiment
** block
*/
struct _VSrecord_struct {

	/* per block info */
	int		  num_repetitions;
	int		  total;					/* total # of trials so far in block		 */
	int		  correct;				    /* total # of trials correct 				 */
	int		  wrong;					/* total # of trials wrong 					 */
	int		  ncerr;					/* total # of trials ncerr 					 */
	int		  brfix;					/* total # of trials brfix					 */
	int 	  brhandfix;				/* total # of trials brhandfix			     */
	int		  sbet;						/* total # of trials sbet was selected  */	/* ts */
	int		  sbet_total;				/* total # of trials sbet was presented */	/* ts */
	int		  last;					    /* keep track of the last "score" 		    */

	int		  num_tasks;
	_VStask	 *task_array;

	int		  num_trials;
	_VStrial	 *trial_array;
	int		  cur_trial_index;
	_VStrial   cur_trial;

	_VSdisplay display;				/* this could be an array */
	
	unsigned int       seed; /* if we want to seed the random number generator (good for repeating 
							  * dot seeds across tasks */
	
};

/* PUBLIC MACROS */
#define VS_CUR_ID(r) (r->cur_trial->id)
	/* see vs_score_trial */
#define WRONG		0
#define CORRECT 	1
#define NCERR		2	/* no choice error 		*/
#define BRFIX		3	/* broke fixation error */
#define SBET		5	/* sure bet target */
#define PRIZE		6

#define DO_OVER	4

/* PUBLIC ROUTINE PROTOTYPES */

	/* RECORD 		*/
_VSrecord	vs_init_record				(void);
void			vs_clear_record			(_VSrecord);
void			vs_free_record				(_VSrecord);

	/* TASK 			*/
_VStask		vs_init_task				(void);
void			vs_free_task				(_VStask);
void			vs_rec_add_task			(_VSrecord, _VStask);

	/* TRIAL 		*/
_VStrial		vs_init_trial				(void);
void			vs_free_trial				(_VStrial);
void			vs_copy_trial				(_VStrial, _VStrial);
void			vs_score_trial				(_VSrecord, int, int);
_VStrial		vs_get_next_trial			(_VSrecord, int, int);
void			vs_make_trial_array		(_VSrecord, long, long);
_VStrial		vs_swap_current_trial	(_VSrecord);

	/* DISPLAY 		*/
_VSdisplay	vs_init_display			(void);
_VSdisplay	vs_copy_display			(_VSdisplay);
void			vs_free_display			(_VSdisplay);
void			vs_display_add_object	(_VSdisplay, _VSobject);

	/* OBJECT 		*/
_VSobject 	vs_init_object				(void);
void			vs_copy_object				(_VSobject, _VSobject);
void			vs_clear_object			(_VSobject);
void			vs_free_object				(_VSobject);

#endif /* VS_H_ */

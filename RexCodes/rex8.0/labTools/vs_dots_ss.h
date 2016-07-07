/*
** VS_DOTS_SS.H
**
** "VisualStimulation_DOTS.h" ... 
**
**	VisualStimulation objects specific to Abstract Dots
**
** Objects:
**		FIXATION POINT
**		TARGET 1
**		TARGET 2
**		Sure TARGET 3
**		DOTS
*/

#ifndef VS_DOTS_H_
#define VS_DOTS_H_

#include <stdio.h>
#include "vs.h"

/* DATA STRUCTURES */

/* _VSDdot_object_struct
**
*/
typedef struct _VSDdot_object_struct *_VSDdot_object;
struct _VSDdot_object_struct {
   int coherence;
   int direction;
   int speed;
   int novar_pct;
   int seed_base;
   int seed_var;
};

/* _VSDtrial_info_struct
**
** holds dots-trial-specific info 
** Luke added seed_base and seed_var 
*/
typedef struct _VSDtrial_info_struct *_VSDtrial_info;
struct _VSDtrial_info_struct {
   int planet_angle;
   int coherence;
   int seed_base;	
   int seed_var;		
   
};

/* _VSDtask_info_struct
**
**	dots-task-specific info (for _VStask_struct)
*/
typedef struct _VSDtask_info_struct *_VSDtask_info;
struct _VSDtask_info_struct {
   
   int proportion;
   
   int object_1;
   int object_2;
   
   int series_o;
   int series_n;
   int series_delta;
   
   	/* flag indicating how to interpret T1/T2:
   	** t_flag == 0: T1 = CORRECT, T2 = WRONG
   	** t_flag == 1: T1 = LEFT, 	T2 = RIGHT
   	** t_flag == 2: choose correct target based on dot direction
   	** t_flag == 3: T1 is correct for rightward motion and T2 for leftward motion. 
	**					    the correct target reverses if rev_dot_targ_association is set
   	*/
   	int t_flag;
   	
   	int coherence_lo;
   	int	 coherence_hi;
   	
   	int rt_flag;			/* is it an RT task  */
   	int fix_type;		/* is it a fixation type task */
};

/* PUBLIC DEFINES/MACROS */

/* some default values */
#define VSD_DOT_SPEED	70
#define VSD_SEED_BASE	800

/* OBJECT INDICES (FOR OVERRIDES, ETC):
*/ 
#define VSD_NUM_OBJECTS 7
#define VSD_OBJECT_FP 0
#define VSD_OBJECT_AP 1	    /* Aperture + dots */
#define VSD_OBJECT_TC 2
#define VSD_OBJECT_T1 3
#define VSD_OBJECT_T2 4
#define VSD_OBJECT_TS 5     /* Sure bet target */ 
#define VSD_OBJECT_DT 6

/* macros to get the appropriate object pointer
** these return _VSobject or NULL
** use this if we wanna be anal:
** 	(r && r->display && (r->display->num_objects > VSD_FP_INDEX)
*/
#define VSD_GET_FP(r) (r)->display->object_array[VSD_OBJECT_FP]
#define VSD_GET_AP(r) (r)->display->object_array[VSD_OBJECT_AP]
#define VSD_GET_T1(r) (r)->display->object_array[VSD_OBJECT_T1]
#define VSD_GET_T2(r) (r)->display->object_array[VSD_OBJECT_T2]
#define VSD_GET_TC(r) (r)->display->object_array[(r)->display->t_correct]
#define VSD_GET_TW(r) (r)->display->object_array[(r)->display->t_wrong]
#define VSD_GET_TL(r) (r)->display->object_array[(r)->display->t_left]
#define VSD_GET_TR(r) (r)->display->object_array[(r)->display->t_right]
#define VSD_GET_DT(r) (r)->display->object_array[VSD_OBJECT_DT]
#define VSD_GET_DTOBJECT(r) (_VSDdot_object) ((r)->display->object_array[VSD_OBJECT_DT]->object_info)
#define VSD_GET_TS(r) (r)->display->object_array[VSD_OBJECT_TS]

#define VSD_GET_TASK_INFO(r) ((r) && (r)->cur_trial) ? \
					(_VSDtask_info) ((r)->cur_trial->task->task_info) : NULL)

/* CONDITIONAL to show dots
**		determined by diameter of the aperture
**			- if > 0, show dots
**			- if = 0, show nothing
**			- if < 0, show fixation color change
*/
#define SHOW_DOTS(r) 	(VSD_GET_AP(r)->diameter > 0)
#define SHOW_FPCOLOR(r) (VSD_GET_AP(r)->diameter < 0)

/* PUBLIC ROUTINE PROTOTYPES */

	/* RECORD */
_VSrecord 			vsd_init_record (void);
void 						vsd_clear_record (_VSrecord);
void 						vsd_free_record (_VSrecord);
void 						vsd_print_record (_VSrecord);

	/* TASK	*/
_VStask 				vsd_init_task (_VSDtask_info);
_VSDtask_info 		vsd_init_task_info (_VSDtask_info);
_VSDtask_info 		vsd_copy_task_info (_VSDtask_info);
void 						vsd_free_task (_VStask);
void 						vsd_free_all_tasks (_VSrecord);
void 						vsd_add_task (int, int *, _VSrecord, _VSDtask_info, int);
void 						vsd_print_task (_VStask);

	/* TRIAL */
_VStrial 				vsd_init_trial (_VSDtrial_info);
void 						vsd_free_trial (_VStrial);
void 						vsd_print_trial (_VStrial);
void 						vsd_print_all_trials (_VSrecord);

	/* DISPLAY */
void 						vsd_update_display (_VSrecord, int);
void 						vsd_clear_display (_VSdisplay);
void 						vsd_print_display (_VSdisplay);

	/* OBJECT */
_VSobject 			vsd_init_dot_object (void);
void 						vsd_copy_dot_object (_VSDdot_object, _VSDdot_object);
void 						vsd_clear_dot_object (_VSobject);
void 						vsd_free_dot_object (_VSobject);
void 						vsd_print_object (_VSobject);


#endif

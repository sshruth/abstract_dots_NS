/* MATLAB.C
**
**	a collection of c-routines
**	to be called from REX to communicate matlab display
**	routines via the pcmsg protocol
**
**	Revision history:
**		6/14/01	jig added "ecode" calls
**		2/17/03	jd added code for showing whole target arrays
**
*/

/* INCLUDED files */
#include <stdio.h>

#include "matlab.h"
#include "lcode.h"

/* PRIVATE VARIABLES */

udpmsg mat_udpmsg[NUM_MAT_CMD];


/* PUBLIC ROUTINES */

/* PUBLIC ROUTINE: mat_initScreen
**
** args:
**		mon_width_cm	... horizontal dimension of monitor	(cm)
**		view_dist_cm	... distance from viewer to center of monitor (cm)
**		num_targets		... total # of targets (fix + targets)
**    num_dots_patch 
*/
#define INIT_SCREEN_ARGS 4
void mat_initScreen(long mon_width_cm, long view_dist_cm, long num_targets, long num_dots_patch)
 {
   udpmsg *packet;
   
   packet = &(mat_udpmsg[MAT_INIT_SCREEN_CMD]);
   packet->id[0] = 0;		/* ask for a randomly asigned id */
   sprintf(packet->msg, "initScreen(%d, %d, %d, %d)",
                                    num_targets, num_dots_patch,  mon_width_cm, view_dist_cm);
   udpmsg_send(packet);
    
   	/* save the codes */
   ec_send_matlab_command(MAT_INIT_SCREEN_CMD, INIT_SCREEN_ARGS);
   ec_send_matlab_arg(1, mon_width_cm);
   ec_send_matlab_arg(2, view_dist_cm);
   ec_send_matlab_arg(3, num_targets);
   ec_send_matlab_arg(4, num_dots_patch);
   ec_send_matlab_arg(0, 0); 	/* signal end of args */
 }


/* PUBLIC ROUTINE: mat_allOff
**
**		Blanks the screen. Does not wait for went.
*/
#define ALL_OFF_ARGS 0
void mat_allOff(void)
 {
   udpmsg *packet;
   
   packet = &(mat_udpmsg[MAT_ALL_OFF_CMD]);
   packet->id[0] = 0;		/* ask for a randomly asigned id */
   strcpy(packet->msg, "allOff;");   
   udpmsg_send(packet);
   
   	/* save the codes */
   ec_send_matlab_command(MAT_ALL_OFF_CMD, ALL_OFF_ARGS);
   ec_send_code(ALLOFFCD);
 }


/* PUBLIC ROUTINE: mat_flipScreen
**
*/
#define FLIP_SCREEN_ARGS 1
void mat_flipScreen(void)
 {
   udpmsg *packet;
   
   packet = &(mat_udpmsg[MAT_FLIP_SCREEN_CMD]);
   packet->id[0] = 0;		/* ask for a randomly asigned id */
   strcpy(packet->msg, "flipScreen(1);");   
   udpmsg_send(packet);
   
   	/* save the codes */
   ec_send_matlab_command(MAT_FLIP_SCREEN_CMD, FLIP_SCREEN_ARGS);
   ec_send_matlab_arg(1, 1);
 }
   


/* PUBLIC ROUTINE: mat_targDefine
**
*/
#define TARG_DEFINE_ARGS 4
void mat_targDefine(int index, int x, int y, int diameter, RGB *color)
 {
   udpmsg *packet;
   
   packet = &(mat_udpmsg[MAT_TARG_DEFINE_CMD]);
   packet->id[0] = 0;		/* ask for a randomly asigned id */
   sprintf(packet->msg,"targDefine(%d,[%d %d]/10,[%d %d]/10,[],[%d %d %d]);",
                                    index, x, y, diameter, diameter, color->R, color->G, color->B);
   udpmsg_send(packet);
	 
   	/* save the codes */
   ec_send_matlab_command(MAT_TARG_DEFINE_CMD, TARG_DEFINE_ARGS);
   ec_send_matlab_arg(1, index);
   ec_send_matlab_arg(2, x);
   ec_send_matlab_arg(2, y);
   ec_send_matlab_arg(3, diameter);
   ec_send_matlab_arg(3, diameter);
   ec_send_matlab_arg(4, color->R);
   ec_send_matlab_arg(4, color->G);
   ec_send_matlab_arg(4, color->B);
   ec_send_matlab_arg(0, 0);	/* signal end of args */
 }


/* PUBLIC ROUTINE: mat_targDraw
**
*/
#define TARG_DRAW_ARGS 2
void mat_targDraw(int num_show, int *show_ind, int num_hide, int *hide_ind)
 {
   udpmsg *packet;
   char show[50]="[";
   char hide[50]="[";
   int i;
   
   packet = &(mat_udpmsg[MAT_TARG_DRAW_CMD]);
   packet->id[0] = 0;		/* ask for a randomly asigned id */
   
   	/* do not call matlab if no change is going to happen on the screen */
   if ( num_show==0 && num_hide==0) {
       packet->sent = packet->received = packet->executed = 1;
       return;
   } 
   
   	/* otherwise show and hide the targets */
   for (i=0 ; i < num_show ; i++)
       sprintf(show, "%s %d", show, show_ind[i]);
   strcat(show, "]");   
   for (i=0 ; i < num_hide ; i++)
       sprintf(hide, "%s %d", hide, hide_ind[i]);
   strcat(hide, "]");
   sprintf(packet->msg, "targDraw(%s,%s);", show, hide);
   udpmsg_send(packet);
   
   	/* save the codes */
   if (num_hide>0 && hide_ind!=0)
       ec_send_matlab_command(MAT_TARG_DRAW_CMD, TARG_DRAW_ARGS);
   else
       ec_send_matlab_command(MAT_TARG_DRAW_CMD, 1);
   for (i=0 ; i < num_show ; i++)
       ec_send_matlab_arg(1, show_ind[i]);
   for (i=0 ; i < num_hide ; i++)
       ec_send_matlab_arg(2, hide_ind[i]);
   ec_send_matlab_arg(0, 0);	/* signal end of args */
 }


/* PUBLIC ROUTINE: mat_targDrawWait
**
*/
#define TARG_DRAW_WAIT_ARGS 2
void mat_targDrawWait(int num_show, int *show_ind, int num_hide, int *hide_ind)
 {
   udpmsg *packet;
   char show[50]="[";
   char hide[50]="[";
   int i;
   
   	/*  send the command to Matlab even if num_show and num_hide are both zero. */
   	
   packet = &(mat_udpmsg[MAT_TARG_DRAW_WAIT_CMD]);
   packet->id[0] = 0;		/* ask for a randomly asigned id */
   for (i=0 ; i < num_show ; i++)
       sprintf(show, "%s %d", show, show_ind[i]);
   strcat(show, "]");   
   for (i=0 ; i < num_hide ; i++)
       sprintf(hide, "%s %d", hide, hide_ind[i]);
   strcat(hide, "]");
   sprintf(packet->msg, "targDraw(%s,%s,0);", show, hide);
   udpmsg_send(packet);
   
   	/* save the codes */
   if (num_hide>0 && hide_ind!=0)
       ec_send_matlab_command(MAT_TARG_DRAW_WAIT_CMD, TARG_DRAW_WAIT_ARGS);
   else
       ec_send_matlab_command(MAT_TARG_DRAW_WAIT_CMD, 1);
   for (i=0 ; i < num_show ; i++)
       ec_send_matlab_arg(1, show_ind[i]);
   for (i=0 ; i < num_hide ; i++)
       ec_send_matlab_arg(2, hide_ind[i]);
   ec_send_matlab_arg(0, 0);	/* signal end of args */
 }




/* PUBLIC ROUTINE: mat_dotsDefine
**
** Calls Dots8 with aperture information.
**	Does NOT wait for went. Ever.
*/
#define DOTS_DEFINE_ARGS 5
void mat_dotsDefine(int patch, int ap_x, int ap_y, int ap_d, int direction, int coherence, int speed)
 {
   udpmsg *packet;
   
   packet = &(mat_udpmsg[MAT_DOTS_DEFINE_CMD]);
   packet->id[0] = 0;		/* ask for a randomly asigned id */
   sprintf(packet->msg, "dotsDefine(%d,[%d %d %d %d]/10,%d,%d/1e3,%d/10);", 
                                    patch, ap_x, ap_y, ap_d, ap_d, direction, coherence, speed);
   udpmsg_send(packet);
   
   	/* save the codes */
   ec_send_matlab_command(MAT_DOTS_DEFINE_CMD, DOTS_DEFINE_ARGS);
   ec_send_matlab_arg(1, patch);
   ec_send_matlab_arg(2, ap_x);
   ec_send_matlab_arg(2, ap_y);
   ec_send_matlab_arg(2, ap_d);
   ec_send_matlab_arg(2, ap_d);
   ec_send_matlab_arg(3, direction);
   ec_send_matlab_arg(4, coherence);
   ec_send_matlab_arg(5, speed);	
   ec_send_matlab_arg(0, 0);	/* signal end of args */
 }





/* PUBLIC ROUTINE: mat_dotsShow
**
**
*/
#define DOTS_SHOW_ARGS 2
void mat_dotsShow(int seed_base, int seed_var, int max_duration)
 {
   udpmsg *packet;
   
   packet = &(mat_udpmsg[MAT_DOTS_SHOW_CMD]);
   packet->id[0] = 0;		/* ask for a randomly asigned id */
   	/* include fp in the file to continue logging the commands */
   sprintf(packet->msg, "dotsShow([%d %d],%d/1000,fp);", seed_base, seed_var, max_duration);
   udpmsg_send(packet);
   
   	/* save the codes */
   ec_send_matlab_command(MAT_DOTS_SHOW_CMD, DOTS_SHOW_ARGS);
   ec_send_matlab_arg(1, seed_base);
   ec_send_matlab_arg(1, seed_var);
   ec_send_matlab_arg(2, max_duration);
   ec_send_matlab_arg(0, 0);	/* signal end of args */
 }


/* PUBLIC ROUTINE: mat_dotsStop
**
*/
#define DOTS_STOP_ARGS 0
void mat_dotsStop(void)
 {
   udpmsg *packet;
   
   packet = &(mat_udpmsg[MAT_DOTS_STOP_CMD]);
   packet->id[0] = 0;		/* ask for a randomly asigned id */
   strcpy(packet->msg, "break_loop=1;");
   udpmsg_send(packet);
   
   	/* save the codes */
   ec_send_matlab_command(MAT_DOTS_STOP_CMD, DOTS_STOP_ARGS);
 }



/* PUBLIC ROUTINE: mat_dotsAbort
**
*/
#define DOTS_ABORT_ARGS 0
void mat_dotsAbort(void)
 {
   udpmsg *packet;
   
   packet = &(mat_udpmsg[MAT_DOTS_ABORT_CMD]);
   packet->id[0] = 0;		/* ask for a randomly asigned id */
   strcpy(packet->msg, "break_loop=1;");
   udpmsg_send(packet);
   
   	/* save the codes */   
   ec_send_matlab_command(MAT_DOTS_ABORT_CMD, DOTS_ABORT_ARGS);
 }



/* PUBLIC ROUTINE: mat_changeCoh
**
** allows change of coherence within the trial
**
** Arguments:
**		patch			which dots patch 
**		new_coh		new coherence
**		dxdy_mult	-1 if the change of coherence also involves change of direction
**					+1 otherwise
** 
** dxdy_mult is needed because the matlab program expects coherence to be always 
** positive 
*/
#define DOTS_DCOH_ARGS 3
void mat_changeCoh (int patch, int new_coh , int dxdy_mult)
 {
   udpmsg *packet;
   
   packet = &(mat_udpmsg[MAT_DOTS_DCOH_CMD]);   
   packet->id[0] = 0;		/* ask for a randomly asigned id */
   sprintf(packet->msg, "dots_struct(%d).coherence=%d/1e3; dxdy{%d}=%d*dxdy{%d};", 
                                     patch, new_coh, patch, dxdy_mult, patch);
   udpmsg_send(packet);
   
   	/* save the codes */
   ec_send_matlab_command(MAT_DOTS_DCOH_CMD, DOTS_DCOH_ARGS);
   ec_send_matlab_arg(1, patch);
   ec_send_matlab_arg(2, new_coh);
   ec_send_matlab_arg(3, dxdy_mult);
   ec_send_matlab_arg(0, 0);	/* signal end of args */  
 }


 void mat_dispMsg(char *msg)
 {
   udpmsg *packet;
   
   packet = &(mat_udpmsg[MAT_DISP_MSG_CMD]);
   packet->id[0] = 0;
   sprintf(packet->msg, "disp('%s');", msg);
   udpmsg_send(packet);
 }


int udpCheckReceiveFork()
 {
   udpmsg packet = {"", "", 0, 0, 0};
   int processed = 0;
   udpmsg *p;
   int i;

   if(udpmsg_receive(&packet) == 1) { // Means message was received from Matlab machine
	   /* check if the newly received message is related to the commands sent to matlab */
       for(i=0,p=mat_udpmsg; i<NUM_MAT_CMD; i++,p++)
		 if (strncmp(packet.id, p->id,9)==0) {
			 p->received = packet.received;
			 p->executed = packet.executed;
			 processed = 1;
               break;
		 }
	   
	   /* if the new message is not related to the commands sent to matlab, see what matlab wants */
       if (processed == 0) {
       }
   }
   
   return 0;
 }



/* PUBLIC ROUTINE: mat_getWent
**
** tests if a command was received or executed by matlab
** cmd:		which command we are asking for. should be one of MAT_..._CMD
** recv_or_exec: are we asking about receipt of the command or its execution
**                        should be either IS_RECEIVED or IS_EXECUTED 
*/
int mat_getWent(int cmd, int recv_or_exec)
 {
   udpmsg *packet;
   
   	/* if the command is unknown, return Went */
   if (cmd<0 || cmd>=NUM_MAT_CMD)
       return MAT_WENT;
   
   packet = &(mat_udpmsg[cmd]);
   if ((recv_or_exec==IS_RECEIVED && packet->received) ||
       (recv_or_exec==IS_EXECUTED && packet->executed))
           return MAT_WENT;
   else
           return MAT_WAIT;
 }









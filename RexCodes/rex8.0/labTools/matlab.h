/* MATLAB.H
*
*	header file for MATLAB.c, a collection of c-routines
*	to be called from REX to communicate matlab display
*	routines via the pcmsg protocol
*/

#ifndef _MATLAB_H_
#define _MATLAB_H_

#include "vs_dots.h"
#include "ecode.h"
#include "udpmsg.h"
/*#include "../sset/lcode.h"*/

/* PUBLIC DEFINES */
#define MAT_FP 	1
#define MAT_T1	2
#define MAT_T2	3
#define MAT_T3	4
#define MAT_T4	5



/* VALUES RETURNED BY mat_getWent */
#define MAT_WAIT		0
#define MAT_WENT		1

/*  indices of matlab commands, used for saving matlab commands in the E-file
** by using ec_send_matlab_command, and for the mat_udpmsg
*/

#define NUM_MAT_CMD					12

#define MAT_INIT_SCREEN_CMD			0
#define MAT_ALL_OFF_CMD				1
#define MAT_FLIP_SCREEN_CMD			2
#define MAT_TARG_DEFINE_CMD			3
#define MAT_TARG_DRAW_CMD			4
#define MAT_TARG_DRAW_WAIT_CMD	    5
#define MAT_DOTS_DEFINE_CMD			6
#define MAT_DOTS_SHOW_CMD			7
#define MAT_DOTS_STOP_CMD			8
#define MAT_DOTS_ABORT_CMD			9
#define MAT_DOTS_DCOH_CMD			10
#define MAT_DISP_MSG_CMD			11

void mat_initScreen 			(long, long, long, long);
void mat_allOff 				(void);
void mat_flipScreen 			(void);

void mat_targDefine 		(int, int, int, int, RGB *);
void mat_targDraw 			(int, int *, int, int *);
void mat_targDrawWait 	    (int, int *, int, int *);

void mat_dotsDefine			(int, int, int, int, int, int, int);
void mat_dotsShow 			(int, int, int);
void mat_dotsStop 			(void);
void mat_dotsAbort 			(void);
void mat_changeCoh 		    (int, int, int);

void mat_disoMsg            (char *);

int mat_getWent				(int, int);

int udpCheckReceiveFork     (void);

#endif /* _MATLAB_H_ */

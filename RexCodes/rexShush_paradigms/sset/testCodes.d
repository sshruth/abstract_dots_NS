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

/* GLOBAL VARIABLES */

int   gl_ntrials = 13;
int   gl_trialCtr = 0;
int   gl_positions = 1;
static int gl_strbs[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096};

/* ROUTINES */
/* ROUTINE: open_adata
**
** Opens analog data
*/
int open_adata(void)
 {
   awind(OPEN_W);
   return(0);
 }

 /* ROUTINE: rinitf
  ** initialize at first pass or at r s from keyboard
  */
 void rinitf(void)
 {

 }


/* ROUTINE: nexttrl
**
*/
int nexttrl()
{
	open_adata();
	printf("sending number: %d\n",);
	ec_send_code(gl_strbs[gl_trialCtr]);

	/* increment trial counter */
  if (gl_trialCtr>=gl_ntrials){
    gl_trialCtr = 0;
  } else {
    gl_trialCtr++
  }

	return 0;
}



USER_FUNC ufuncs[] = {};

/* Top-level state menu
*/

VLIST state_vl[] = {
	  {"nPositions",              &(gl_positions),        NP, NP, 0, ME_DEC},
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
		to loop
 	loop: 		/* START THE LOOP -- loop on # trials */
		time 1000
		do nexttrl()
	  to prdone
	prdone:
		time 1000
		to loop


	abtst:	/* for abort list */
		to prdone


abort list:
}



// udp_set {
// status ON
// begin ufirst:
// 		to uchk
// 	uchk:
// 		do udpCheckReceiveFork()
// 		to uchkAgain
// 	uchkAgain: // simply prevents looping back on same state, which Rex doesn't always like
// 		do udpCheckReceiveFork()
// 		to uchk
//
// abort list:
// }

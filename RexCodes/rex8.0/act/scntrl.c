/*
 *-----------------------------------------------------------------------*
 * NOTICE:  This code was developed by the US Government.  The original
 * versions, REX 1.0-3.12, were developed for the pdp11 architecture and
 * distributed without restrictions.  This version, REX 4.0, is a port of
 * the original version to the Intel 80x86 architecture.  This version is
 * distributed only under license agreement from the National Institutes 
 * of Health, Laboratory of Sensorimotor Research, Bldg 10 Rm 10C101, 
 * 9000 Rockville Pike, Bethesda, MD, 20892, (301) 496-9375.
 *-----------------------------------------------------------------------*
 */

/*
 * scntrl.c:	Functions to control state set execution.
 */
#include <stdio.h>
#include <sys/types.h>
#include "rexhdrs.h"
#include "int.h"

/* The following functions allow a synchronous paradigms to be
*	awakened during a behavioral cycle (and to be put back
*	to sleep, e.g. turn on eye movement action only during
*	the part of a cycle when it is wanted.
*/
int
awake(long x)
{
	extern AWAKE nowstate[];

	nowstate[x].current_state = nowstate[x].initial_state;
	nowstate[x].now_flag = ON;
	return(0);
}

int
asleep(long x)
{
	AWAKE *stateptr;
	extern AWAKE nowstate[];

	stateptr = &nowstate[x];
	stateptr->current_state = stateptr->initial_state;
	stateptr->now_flag = OFF;
	return(0);
}

/* The following is the abort function. It will abort and halt the
*	the behavioral list which is addressed.  If it receives
*	a negative number (usually -1) it will halt all behavioral
*	cycling until reset is done.
*/
int
abort_s(long x)
{
	extern AWAKE nowstate[];
	STATE **ptr,*sptr;
	int i;

	if ( x < 0 ){
		for (i = 0; nowstate[i].initial_state != 0; i++) {
			nowstate[i].now_flag = OFF;
			if (nowstate[i].abort_list == 0) continue;
			ptr = nowstate[i].abort_list;
			while (*ptr != 0){
				sptr = *ptr;
				if (sptr->device.action != 0) 
					(*sptr->device.action)
						(sptr->device.act_code,
						sptr->device.act_2code,
						sptr->device.act_3code,
						sptr->device.act_4code,
						sptr->device.act_5code,
						sptr->device.act_6code);
				ptr++;
			}
		}
	}
	else {
		i = x;
		nowstate[i].now_flag = OFF;
		if (nowstate[i].abort_list == 0) return(0);
		ptr = nowstate[i].abort_list;
		while (*ptr != 0){
			sptr = *ptr;
			if (sptr->device.action != 0) 
				(*sptr->device.action)(sptr->device.act_code,
					sptr->device.act_2code,
					sptr->device.act_3code,
					sptr->device.act_4code,
					sptr->device.act_5code,
					sptr->device.act_6code);

			ptr++;
		}
	}
	return(0);

}

/* The following is the reset function.  It will reset a behavioral list
*	to its initial condition or reset condition.  This is done by
*	aborting the list addressed and resetting the "now_flag" to
*	be equal to the "init_flag".  Once again a '-1' will reset all
*	of the lists.
*/
int
reset_s(long x)
{
	extern AWAKE nowstate[];
	int i;

	abort_s(x);

	if ( x < 0) {
		for (i = 0; nowstate[i].initial_state != 0; i++) {
			nowstate[i].now_flag = nowstate[i].init_flag;
		}
	}

	else {
		nowstate[x].now_flag = nowstate[x].init_flag;
	}
	return(0);
}

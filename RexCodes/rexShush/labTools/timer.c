/* TIMER.C
*
*	a collection of c-routines
*	to be called from REX to run timers within the state set
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>	/*needed for ../hdr/sys.h, which is included in rexhdrs*/
#include "rexhdrs.h"
#include "timer.h"
#include "toys.h"

/* PRIVATE DEFINES */
#define NUM_TIMERS 		5
#define DTIMER_XSTATES 	20	/* number of extra states to add */

/* PRIVATE ROUTINE PROTOTYPES */
static int timer_set			(long, long, long, long, long, long, long);
static int timer_continue	(long, long, long);
static int timer_check		(long);

/* PRIVATE GLOBALS */
static int gl_time_begin[NUM_TIMERS];
static int gl_time_end[NUM_TIMERS];

/* PUBLIC ROUTINES: timer_setr
**
**	description: sets a timer from an exponential distribution
**		of values
*/
int timer_set1(long a, long b, long c, long d, long e, long f)
	{ return(timer_set(0,a,b,c,d,e,f)); }
int timer_set2(long a, long b, long c, long d, long e, long f)
	{ return(timer_set(1,a,b,c,d,e,f)); }
int timer_set3(long a, long b, long c, long d, long e, long f)
	{ return(timer_set(2,a,b,c,d,e,f)); }
int timer_set4(long a, long b, long c, long d, long e, long f)
	{ return(timer_set(3,a,b,c,d,e,f)); }
int timer_set5(long a, long b, long c, long d, long e, long f)
	{ return(timer_set(4,a,b,c,d,e,f)); }


/* ROUTINE: timer_setX_shell
** 
** these routines create a shell for timer_setX
** it is better not to call timer_setX directly from with states because 
** Rex stores the return value of any function that returns nonzero values
** the stored values from timer can interfere with spike CDs (600-608) creating 
** spurious spikes
*/
int timer_set1_shell(long a, long b, long c, long d, long e, long f)
 { timer_set1(a, b, c, d, e, f);		return(0); }
int timer_set2_shell(long a, long b, long c, long d, long e, long f)
 {	timer_set2(a, b, c, d, e, f);		return(0); }
int timer_set3_shell(long a, long b, long c, long d, long e, long f)
 { timer_set3(a, b, c, d, e, f);		return(0); }
int timer_set4_shell(long a, long b, long c, long d, long e, long f)
 {	timer_set4(a, b, c, d, e, f);		return(0);	}
int timer_set5_shell(long a, long b, long c, long d, long e, long f)
 {	timer_set5(a, b, c, d, e, f);		return(0); }

/* arguments:
**  	probability 	 ... chance that we'll use exponential time
**								([0..1000]: 1000 for always, 0 for never)
**		min_time    	 ... minimum if we use exponential
**		max_time    	 ... maximum if we use exponential
**		mean_time   	 ... mean if we use exponential
**		override_time 	 ... time we use if we don't use exponential
**		override_random ... offset to override_time if used
*/
static int timer_set(long index, long probability, long min_time, 
							long max_time, long mean_time,
							long override_time, long override_random)
{
	gl_time_begin[index] = (int) (i_b->i_time);

	if(TOY_RAND(1000) < probability)	/* use expoential time 	*/
		gl_time_end[index] = gl_time_begin[index] - DTIMER_XSTATES +
									toy_exp(min_time, max_time, mean_time);
	else										/* use override value 	*/
		gl_time_end[index] = gl_time_begin[index] - DTIMER_XSTATES + 
									override_time + (override_random ? 
										rand() % override_random : 0);
	
	return(gl_time_end[index] + DTIMER_XSTATES - gl_time_begin[index]);
}

/* PUBLIC ROUTINES: timer_continue
**
**	description: extends the timer to the given value
**	(or does nothing if the current timer is longer than the given
**	value -- this allows you to pass in a 0 & have it not affect
**	anything)
*/
int timer_continue1(long a, long b) { return(timer_continue(0,a,b)); }
int timer_continue2(long a, long b) { return(timer_continue(1,a,b)); }
int timer_continue3(long a, long b) { return(timer_continue(2,a,b)); }
int timer_continue4(long a, long b) { return(timer_continue(3,a,b)); }
int timer_continue5(long a, long b) { return(timer_continue(4,a,b)); }

static int timer_continue(long index, long abs_time, long rel_time )
{

	if(rel_time > 0) {
		gl_time_end[index] += rel_time;
	
	} else {
		int tmp_end = gl_time_begin[index] - DTIMER_XSTATES + abs_time;
	
		if(tmp_end > gl_time_end[index])
			gl_time_end[index] = tmp_end; 
	}
	
	return(0);
}

/* PUBLIC ROUTINES: timer_check
**
**	description: checks if timer is done
*/
int timer_check1(void) { return(timer_check(0)); }
int timer_check2(void) { return(timer_check(1)); }
int timer_check3(void) { return(timer_check(2)); }
int timer_check4(void) { return(timer_check(3)); }
int timer_check5(void) { return(timer_check(4)); }

static int timer_check(long index)
{
	return((i_b->i_time < gl_time_end[index]) ? 0 : 1);
}

/* PUBLIC ROUTINE: timer_pause
**
**	pause for a given number of msec
*/
int timer_pause(long msec)
{
	long diff;
	struct timespec start, now;

	if(clock_gettime(CLOCK_REALTIME, &start) == -1)
		return(0);

	do {
		clock_gettime(CLOCK_REALTIME, &now);

		diff = (now.tv_sec - start.tv_sec) * 1000L +
				 (now.tv_nsec - start.tv_nsec) / 1000000L;

	} while(diff < msec);

	return(0);
}

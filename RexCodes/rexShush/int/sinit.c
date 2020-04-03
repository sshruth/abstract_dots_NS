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
 * 	Initialize state lists.  Called from main, and whenever a
 * reset is done.  These functions CAN be run at interrupt level;
 * interrupt level restrictions apply.
 */

#include <stdio.h>
#include <sys/types.h>
#include "rexhdrs.h"
#include "int.h"

static int first= 1;

void
stateinit(void)
{
	AWAKE *pawake;
	STATE *pstate;
	ESCAPE *pescape;

	AWAKE *pre, *post;
	extern AWAKE nowstate[];
	extern ESCAPE *firstesc;
	extern STATE sps_state;

	pre = post = 0;

	/* link each chain */
	for (pawake = nowstate, pstate = pawake->initial_state;
		 pstate; pstate = (++pawake)->initial_state) {


		/* set up list pointers in each escape */
		if (first == 0) {	/* reset last escape */
			/* zero the current next escape link */
			pawake->lastesc->nextesc = 0;

			/* find first state's last escape */
			for (pescape = &pstate->escape;
				 pescape->nextesc; pescape++);

			/* save its address */
			pawake->lastesc = pescape;
		}
		else setescapes(pstate,pawake);

		/* re-set post pointer in prestate */
		if (pre) {
			pre->poststate = (STATE *)pawake;
			pre->lastesc->nextesc = &pstate->escape;
		}

		/* set pre pointer */
		pawake->prestate = (STATE*)pre;
		pre = pawake;

		/* set up post pointers, assuming last state */
		pawake->poststate = 0;
		pawake->lastesc->nextesc = 0;

	}

	/* set up first escape */
	if (first == 0) firstesc = &nowstate[0].initial_state->escape;
	else {
		firstesc = &sps_state.escape;

		/* no longer first time through */
		first = 0;
	}
}

/*
 * Set up escapes.  This is a recursive function called
 * once from stateinit() for each chain.  It links all the esacpes
 * in each state to each other.  
 */
void
setescapes(STATE *pstate, AWAKE *pawake)
{
	ESCAPE *pescape;

	/*
	 * If pointers are already set, done.
	 * This check is necessary because this fucntion can be
	 * recursively called more than once for a particular state.
	 */
	if(pstate->escape[0].awakestate) return;

	for(pescape = &pstate->escape;	pescape->stateptr; pescape++) {

		pescape->awakestate = pawake;

		/*
		 * Link in next escape in this state.
		 */
		pescape->nextesc = pescape + 1;

		/*
		 * Recursively call for the new state specified
		 * in this escape.  This results in 'setescapes()' being
		 * called eventually for all states.
		 */
		setescapes(pescape->stateptr, pawake);
	}

	pescape--;	/* dec from the last increment in for() loop */
	pawake->lastesc= pescape;

	/*
	 * The for() loop links in the null term escape before
	 * finishing.  Fix this.
	 */
	pescape->nextesc = 0;
}

/*
 * Called when reinitialize statelist command is executed.
 */
void
again(void)
{
	extern STATE sps_state;
	extern ESCAPE *firstesc;

	firstesc = &sps_state.escape;
}

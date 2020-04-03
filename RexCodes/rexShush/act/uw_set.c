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
 * Actions to manipulate the unit storage window.
 * Flag true, unit window opened.
 */
#include <stdio.h>
#include <sys/types.h>
#include "rexhdrs.h"
#include "ecode.h"
#include "int.h"

/*
 * Open, close window.  Returns ecode of operation.
 */
int 
uw_set(long flag)
{
	if(m_uwind != NULLI) {
		uwind= m_uwind;
		return(0);
	}
	if(flag == NULLI) return(0);
	uwind= flag;
	return(flag ? UWOPENCD : UWCLOSECD);
}

int
PspkCount(long *time, int *code, int *count)
{
	EVENT_P ep;
	long startTime;
	long endTime;

	// initial the counter
	*count = 0;

	// get pointer to the last event stored
	ep = (EVENT_P)&i_b->bevent[i_b->evlx - 1];
	endTime = ep->e_key;

	// compute the time stamp of the beginning of the window
	startTime = ep->e_key - *time;

	// count all the spikes in the window
	if(code == NULL) {
		while(ep->e_key > startTime) {
			if((ep->e_code > 600) && (ep->e_code < 700)) (*count)++;

			// point to the previous event
			--ep;

			// check for buffer wrap
			if(ep < (EVENT_P)&i_b->bevent[0]) ep = (EVENT_P)&i_b->bevent[EBUFNUM - 1];

			// check for time wrap
			if(ep->e_key == endTime) break;
			
			// check for beginning of experiment
			if(ep->e_key == 0) break;
		}
	}

	// count all the designated spikes in the window
	else {
		while(ep->e_key > startTime) {
			if(ep->e_code == *code) (*count)++;

			// point to the previous event
			--ep;

			// check for buffer wrap
			if(ep < (EVENT_P)&i_b->bevent[0]) ep = (EVENT_P)&i_b->bevent[EBUFNUM - 1];

			// check for time wrap
			if(ep->e_key == endTime) break;
			
			// check for beginning of experiment
			if(ep->e_key == 0) break;
		}
	}

	return(0);
}

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
 * Routines to unpack unit info from incore Event Buffer.  This code emulates
 * a few of the rextools routines for getting events and units, and borrows
 * appropriate routines from rextools.  However, these
 * routines look at the incore rotating Event buffer instead of an Efile.
 */

/* Standard headers */
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sched.h>
#include <process.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/* Toolkit headers */
#include <Ph.h>
#include <Pt.h>
#include <Ap.h>

/* Local headers */
#include "ph_ras.h"
#include "abimport.h"
#include "proto.h"

PRIVATE RexInfo rexInfo= {0};		/* RexInfo struct */
PRIVATE int	startcode;				/* code that delineates a trial */
PRIVATE int	pretime;				/* pretime in msec to start trial before
									 * startcode time */
PRIVATE EVENT_P	laststartcodeEp;	/* pointer to last start code event */
PRIVATE EVENT_P lastsearchEp;		/* pointer to last event that
									 * has been searched for a startcode */

int ras_debug_level= 0;


/*
 * These routines assume that events are split.
 */
void rexSplitEvents(void)
{
    if(ras_debug_level >= 3) {
		dprintf("rexSplitEvents()\n");
    }

    return;
}

int rexFileOpen(int rexmaxSampRate, int rexstartCode, int rexpreTime)
{
	if(ras_debug_level >= 3) {
	    dprintf("rexFileOpen(): startcode %d, pretime %d\n", rexstartCode, rexpreTime);
	}
	if(rexstartCode  < 0) {
	    rxerr("rexFileOpen(): startcode must be a positive ecode");
	    return(0);
	}
	startcode= rexstartCode;
	pretime= rexpreTime;
	lastsearchEp= (EVENT_P)&i_b->bevent[i_b->evlx];
	laststartcodeEp= 0;

	return(0);  /* cannot return number of trials as in offline version*/
}

void rexFileClose(void)
{
	if(ras_debug_level >= 3) {
	    printf("rexFileClose()\n");
	}
	startcode= 0;
	return;
}

RexInfo *rexGetEvents(int trialNum)
{
	EVENT_P begEp, scEp;
	int curTime, finalTime;

	/* this is used to inform caller that there
	 * are no events or units returned
	 */

	if(ras_debug_level >= 3) {
	    dprintf("rexgetEvents(): trialnum %d, startcode %d \n", trialNum, startcode);
	}
	rexInfo.startCode= 0;

	/* if startcode is 0, then rexGetEvents was called
	 * without a previous rexFileOpen()
	 */
	if(startcode == 0) {
	    return(&rexInfo);
	}

	/*
	 * See if another trial has been completed.  Look forward in the E buffer to
	 * discover if another startcode event has been entered.  This will mark
	 * the end of the next trial to process.  Note that evlx always points to next
	 * event to load.
	 */
	if(ras_debug_level >= 3) {
	    dprintf("rexgetEvents(): searching, lastsearchEp %d, laststartcodeEp %d\n",
				lastsearchEp, laststartcodeEp);
	}

	scEp= lastsearchEp;
	for(;;) {
	    if(scEp == &i_b->bevent[i_b->evlx]) {
			/*
			 * Cannot find a new startcode event.
			 */
			lastsearchEp= scEp;
			if(ras_debug_level >= 3) {
				dprintf("rexgetEvents(): no new startcode\n");
			}
			return(&rexInfo);
	    }
	    if(scEp->e_code == startcode) {
			/*
			 * Found a startcode event.
			 */
			if(laststartcodeEp == 0) {
				/*
				 * This startcode is the beginning of the first trial to
				 * process since the rexFileOpen().  Continue looking for
				 * another startcode that marks end of first trial.
				 */
				laststartcodeEp= scEp;
			} else break;
	    }

	    if(++scEp == &i_b->bevent[EBUFNUM]) scEp= (EVENT_P)&i_b->bevent;
	}

	/*
	 * Have found next trial.  Backup start of trial if pretime is not zero.
	 */
	begEp= laststartcodeEp;

	if(ras_debug_level >= 3) {
	    dprintf("rexgetEvents(): new trial: begEp %d, begEp time %d, scEp %d, scEp time %d\n",
				begEp, begEp->e_key, scEp, scEp->e_key);
	}

	if(pretime != 0) {
	    curTime= scEp->e_key;
	    if(curTime < pretime) finalTime= 1;
	    else finalTime= curTime - pretime;
	    for(;;) {

			if(--begEp < (EVENT_P)&i_b->bevent) begEp= (EVENT_P)&i_b->bevent[EBUFNUM-1];
			if(begEp->e_code > 0) {         /* check for event code */

				if(begEp->e_key <= finalTime) break;
			}
	    }
	}

	/*
	 * Now have a good trial that starts with begEp and ends with scEp.  Load
	 * rextools event buffer and unit buffers.
	 */
	if(ras_debug_level >= 3) {
	    dprintf("rexgetEvents(): pretime backup begEp %d, begEp time %d\n", begEp, begEp->e_key);
	}
	rexloadbufs(begEp, scEp);

	/*
	 * Advance pointers.
	 */
	laststartcodeEp= scEp;
	if(++scEp == &i_b->bevent[EBUFNUM]) scEp= (EVENT_P)&i_b->bevent;
	lastsearchEp= scEp;
	rexInfo.startCode= startcode;

	return(&rexInfo);
}

PRIVATE EVENT *evbuf = { 0 };			/* contains ecodes but not event */
PRIVATE	EVENT *ebuf = { 0 };			/* full event buffer for sorting */
PRIVATE int lebuf = { 0 };				/* length of ebuf in events */    
PRIVATE int levbuf = { 0 };				/* length of evbuf in events */    
PRIVATE int nevbuf = { 0 };				/* current number of events in
										 * evbuf */
PRIVATE long evnum = { 0 };				/* event (not units) counter */
PRIVATE long *ubuf[rexMaxUnits] = { 0 };/* unit pointers for sorting */
PRIVATE int lubuf[rexMaxUnits] = { 0 };	/* length of unit buffers */
PRIVATE int nubuf[rexMaxUnits] = { 0 };	/* unit count */
PRIVATE int nUnits = 0;					/* number of non-zero units */ 
PRIVATE int unitCodes[rexMaxUnits];		/* codes for non-zero units */
PRIVATE RexUnits ru;
PRIVATE int lnTimes = { 0 };			/* length of ru.nTimes and ru.unitTimes pointer array */

PRIVATE void *rexAlloc(void *ptr, long size, long number, char *err_msg)
{
	size_t nbytes;
	char buf[100];

	nbytes = size * number;

	if (ptr == NULL) {
		ptr = malloc(nbytes);
	}
	else {
		ptr = realloc(ptr, nbytes);
	}
	if (ptr == NULL) {
		sprintf(buf, "rexAlloc: Can't alloc %ld items of size %ld: %s::Exiting",
				number, size, err_msg);
		rxerr(buf);

		PtExit(1);
	}
	return(ptr);
}

/*
 * Reorder an event by moving it back in the list.
 */
PRIVATE void percolate(register EVENT *buf, register EVENT *a, register EVENT *b) {
	register EVENT t;
	register EVENT *c;

	/* find the place in line one below the new place for event b */
	for (c = a-1; c > buf; c--) {
		if (c->e_code < 0) continue;
		if (b->e_key >= c->e_key) break;
	}

	t = *b;	/* hold */

	/* move all events up one */
	while (a > c) {
		*b = *a;
		b = a--;
	}

	a[1] = t;	/* restore */
}
	 
PRIVATE void esort(EVENT *buf, int n)
{
	register EVENT *a, *b, *last;

	last = &buf[n];

	/* get first non-Analog event */
	for (a = buf; a < last; a++) {
		if (a->e_code > 0) break;
	}

	/* swap out-of-order non-Analog events */
	for (b = a+1; b < last; b++) {
		if (b->e_code > 0) {
			if (b->e_key < a->e_key) percolate(buf, a, b);
			a = b;
		}
	}
}

/*
 * Load event into split event buffer
 */
PRIVATE int loadEbuf(EVENT *ev)
{
	evnum++;
	if (nevbuf >= levbuf) {
		levbuf += 512;
		evbuf = (EVENT *) rexAlloc((void *) evbuf,
								   sizeof(EVENT), levbuf, "evbuf");
	}

	evbuf[nevbuf] = *ev;
	return(nevbuf++);
}

/*
 * Load unit time into buffer
 */
PRIVATE void loadUbuf(int code, int time)
{
	register int u, n;

	u = code - UNIT1CD;		/* get index */
	n = nubuf[u]++;
	
	if (n >= lubuf[u]) {
		lubuf[u] += 512;
		ubuf[u] = (long *) rexAlloc((void *) ubuf[u],
									sizeof(long), lubuf[u], "ubuf");
	}
	ubuf[u][n] = time;
}

/*
 * Load event and unit buffers in rextools 'split event' data structures for
 * trial delineated by sEp and eEp, inclusive.
 */
PRIVATE void rexloadbufs(EVENT_P sEp, EVENT_P eEp)
{
	EVENT_P sourceEp, destEp, ev;
	int umin, umax, i, esize, code, u, j;

	nevbuf= evnum= 0;
	for(i= 0; i < rexMaxUnits; i++) {
	    nubuf[i]= unitCodes[i]= 0;
	}

	/*
	 * Read in all events into ebuf and sort them in increasing time.
	 */
	if(sEp <= eEp) {	/* no wrap */
	    esize= eEp - sEp;
	}
	else {		/* wrap */
	    esize= &i_b->bevent[EBUFNUM] - sEp;
	    esize += eEp - &i_b->bevent[0];
	}

	esize++;			/* range is inclusive */
	if(esize > lebuf) {
	    ebuf= (EVENT *)rexAlloc((void *) ebuf, sizeof(EVENT), esize, "ebuf");
	    lebuf= esize;
	}
	
	if(sEp <= eEp) {	/* no wrap */
	    for(destEp= ebuf, sourceEp= sEp; sourceEp <= eEp; sourceEp++, destEp++)
			*destEp= *sourceEp;
		
	}
	else {		/* wrap */
	    for(destEp= ebuf, sourceEp= sEp; sourceEp < &i_b->bevent[EBUFNUM];
			sourceEp++, destEp++)
			*destEp= *sourceEp;
	    for( sourceEp= (EVENT_P)&i_b->bevent; sourceEp <= eEp; sourceEp++, destEp++)
			*destEp= *sourceEp;
	}
	esort(ebuf, esize);

	/*
	 * Copy events and units from ebuf into the 'splitevents' rextools buffers.
	 */
	umin = UNIT1CD;
	umax = UNIT1CD + rexMaxUnits - 1;

	/*
	 * Code Sieve to split out units and regular events.
	 */
	for (i= 0, ev= ebuf; i < esize; i++, ev++) {
		code = ev->e_code;
		if (umin <= code && code <= umax) {	/* is a unit */
			loadUbuf(code, ev->e_key);
		} else {
			loadEbuf(ev);
		}
	}

	/*
	 * Assemble rexInfo struct.
	 */
	nUnits= 0;
	for (nUnits= u= 0; u < rexMaxUnits; u++) {	/* how many units present? */
	    if(nubuf[u]) unitCodes[nUnits++] = u + UNIT1CD;
	}
	rexInfo.events= evbuf;
	rexInfo.nEvents= nevbuf;
	rexInfo.tStartTime= sEp->e_key;
	rexInfo.tEndTime= eEp->e_key;
	rexInfo.nUnits = nUnits;
	rexInfo.unitCodes = unitCodes;
	if(nUnits == 0) return;			/* no unit data present */

	/*
	 * Assemble RexUnits struct.
	 */
	if(nUnits > lnTimes) {			/* allocate space */
	    ru.nTimes= (long *)rexAlloc((void *) NULL, sizeof(long), nUnits, "ru.nTimes");
	    ru.unitTimes= (long **)rexAlloc((void *) NULL, sizeof(long *), nUnits, "ru.unitTimes");
	    lnTimes= nUnits;
	}
	for (i= u= 0; u < rexMaxUnits; u++) {  /* initialize pointers */
	    if(nubuf[u]) {
			ru.nTimes[i]= nubuf[u];
			ru.unitTimes[i]= ubuf[u];
			i++;
	    }
	}
	rexInfo.rexunits= &ru;

	if(ras_debug_level) {
	    int i;

	    if(ras_debug_level >= 1) {
			dprintf("\nbeg time %d end time %d nUnits %d nEvents %d\n",
					sEp->e_key, eEp->e_key, nUnits, nevbuf);
	    }
	    if(ras_debug_level >= 2) {
			if(nUnits) {
				dprintf("Unitcode - count:\n");
				for(i= 0; i < nUnits; i++) {
					dprintf("%d - %d, ", unitCodes[i], ru.nTimes[i]);
				}
			}
	    }
	    if(ras_debug_level >= 4) {

			for(i= 0; i < nUnits; i++) {
				dprintf("\nUnitcode %d, Count %d: ", unitCodes[i], ru.nTimes[i]);
				for(j= 0; j < ras_debug_level; j++)
					dprintf("%d ", ru.unitTimes[i][j]);
			
			}
			dprintf("\nUbuf: code - count\n");
			for(i= 0; i < rexMaxUnits; i++) {
				dprintf("%d - %d, ", i + UNIT1CD, nubuf[i]);
			}
			dprintf("\n");
	    }
	}
}

	  

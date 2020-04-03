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
 * Actions to control ramp generator.
 */
#include <stdio.h>
#include <x86/inline.h>
#include <sys/types.h>
#include "rexhdrs.h"
#include "ramp.h"
#include "ecode.h"
#include "int.h"

/*
 * Specify new ramp.  Since this call uses more than 3 args it
 * cannot be called as an action, but must be called from an action.
 */
int
ra_new(int rnum, int len, int ang, int vel,
int xoff, int yoff, int ecode, int type)
{
	RAMP *rp;
	int menu;
	unsigned eflags;

	if(rnum < 0 || rnum >= RA_MAXNUM) {
	    rxerr("ra_new(): Bad ramp number");
	    return(-1);
	}
	rp= &ramp[rnum];
	menu= rp->m_ra_menu;
	if(rp->ra_back != NP) {
	    rxerr("ra_new(): Called when ramp is active");
	    return(-1);
	}
	
	if((eflags= pswget()) & INTR_IF) InterruptDisable();
	if(menu && (rp->m_ra_len != NULLI)) rp->ra_len= rp->m_ra_len;
	else if(len != NULLI) rp->ra_len= len;
	if(menu && (rp->m_ra_ang != NULLI)) rp->ra_ang= rp->m_ra_ang;
	else if(ang != NULLI) rp->ra_ang= ang;
	if(menu && (rp->m_ra_vel != NULLI)) rp->ra_vel= rp->m_ra_vel;
	else if(vel != NULLI) rp->ra_vel= vel;
	if(menu && (rp->m_ra_xoff != NULLI)) rp->ra_xoff= rp->m_ra_xoff;
	else if(xoff != NULLI) rp->ra_xoff= xoff;
	if(menu && (rp->m_ra_yoff != NULLI)) rp->ra_yoff= rp->m_ra_yoff;
	else if(yoff != NULLI) rp->ra_yoff= yoff;
	if(menu && (rp->m_ra_ecode != NULLI)) rp->ra_ecode= rp->m_ra_ecode;
	else if(ecode != NULLI) rp->ra_ecode= ecode;
	if(menu && (rp->m_ra_type != NULLI)) rp->ra_type= rp->m_ra_type;
	else if(type != NULLI) rp->ra_type= type;
	if(eflags & INTR_IF) InterruptEnable();
	ra_compute(rnum);
	return(0);
}

int Pra_new(int *rnum, int *len, int *ang, int *vel,
			int *xoff, int *yoff, int *ecode, int *type)
{
	ra_new(*rnum, *len, *ang, *vel, *xoff, *yoff, *ecode, *type);
	return(0);
}

/*
 * Initiate ramp.  If flag is true there will be a 40msec delay after the
 * mirrors move to the starting position to allow them to settle.  Also,
 * if ra_type & RA_NO25MS is not true, there will be a 25msec acceleration
 * start up time.  In any case RA_STARTED is set in rampflag when the
 * ramp has started and is over the start position, taking into account
 * any of the aforementioned delays.  At this point, for example, the
 * light on the mirror can be turned on.
 * Arguemnt 'device' is a device to turn off when the ramp completes.  If
 * 0 it is ignored.
 */
int
ra_start(long rnum, long flag, DIO_ID device)
{
	RAMP *rp;
	unsigned eflags;

	if(rnum < 0 || rnum >= RA_MAXNUM) {
	    rxerr("ra_start(): Bad ramp number");
	    return(0);
	}
	rp= &ramp[rnum];
	rp->ra_device= device;
	if(flag) rp->ra_state= RA_DELST;
	else rp->ra_state= RA_NODEL;

	/*
	 * Link ramp into active list at end.
	 */
	if((eflags= pswget()) & INTR_IF) InterruptDisable();
	if(rp->ra_back == NP) {
	    rp->ra_back= ra_last.ra_back;
	    rp->ra_back->ra_for= rp;
	    rp->ra_for= &ra_last;
	    ra_last.ra_back= rp;
	}
	if(eflags & INTR_IF) InterruptEnable();
	
	return(0);
}

int Pra_start(long *rnum, long *flag, DIO_ID *device)
{
	ra_start(*rnum, *flag, *device);
	return(0);
}

/*
 * Stop ramp.
 */
int
ra_stop(long rnum)
{
	RAMP *rp;
	int returncd= 0;

	if(rnum < 0 || rnum >= RA_MAXNUM) {
	    rxerr("ra_stop(): Bad ramp number");
	    return(0);
	}
	rp= &ramp[rnum];
	if(rp->ra_state != RA_OFF) returncd= RDONECD;
	ra_unlink(rp);
	return(returncd);
}

int Pra_stop(long *rnum)
{
	ra_stop(*rnum);
	return(0);
}
/*
 *	Called to move mirrors to starting position of next ramp.
 */
int
ra_tostart(long rnum)
{
	RAMP *rp;

	if(rnum < 0 || rnum >= RA_MAXNUM) {
	    rxerr("ra_tostart(): Bad ramp number");
	    return(0);
	}
	rp= &ramp[rnum];
	if(rp->ra_state != RA_OFF) {
	    rxerr("ra_tostart(): Called after ramp already started");
	    return(0);
	}
	if((rp->ra_rampflag & RA_CDONE) == 0) {
	    rxerr("ra_tostart(): Valid ramp is not computed");
	    return(0);
	}
	rp->ra_x.ra_valid= 1;
	rp->ra_y.ra_valid= 1;
	return(0);
}

int Pra_tostart(long *rnum)
{
	ra_tostart(*rnum);
	return(0);
}

/*
 *	Used to control phi tracking paradigms.  Switches
 * ra_device to phi device and starts phi blinking.
 */
int
ra_phistart(long rnum)
{
	RAMP *rp;

	if(rnum < 0 || rnum >= RA_MAXNUM) {
	    rxerr("ra_phistart(): Bad ramp number");
	    return(0);
	}
	rp= &ramp[rnum];
	rp->ra_device= rp->ra_phidev;
	rp->ra_phiflag |= RA_PHIGO;
	return(0);
}

int Pra_phistart(long *rnum)
{
	ra_phistart(*rnum);
	return(0);
}

/*
 *	Ends phi blinking.
 */
int
ra_phiend(long rnum)
{
	RAMP *rp;

	if(rnum < 0 || rnum >= RA_MAXNUM) {
	    rxerr("ra_phiend(): Bad ramp number");
	    return(0);
	}
	rp= &ramp[rnum];
	rp->ra_phiflag &= ~RA_PHIGO;
	dio_off(rp->ra_device);
	return(0);
}

int Pra_phiend(long *rnum)
{
	ra_phiend(*rnum);
	return(0);
}

/*
 * Unlink ramp from active list.
 */
void
ra_unlink(RAMP *rp)
{
    unsigned eflags;
    
    rp->ra_rampflag &= ~(RA_STARTED|RA_CDONE);
    rp->ra_phiflag &= ~(RA_PHIGO);
    rp->ra_state= RA_OFF;
    rp->ra_x.ra_valid= 0;
    rp->ra_y.ra_valid= 0;
    rp->ra_timeleft= 0;
    if(rp->ra_device) dio_off(rp->ra_device);

    /*
     * Unlink from active list.
     */
	if((eflags= pswget()) & INTR_IF) InterruptDisable();
    if(rp->ra_back == NP) goto done;
    else {
        rp->ra_back->ra_for= rp->ra_for;
        rp->ra_for->ra_back= rp->ra_back;
        rp->ra_back= NP;
    }

done:
	if(eflags & INTR_IF) InterruptEnable();
    return;
}

/*
 *	Computes time remaining until specified percentage of
 * ramp duration has elapsed.  Returns correct preset and random
 * (if specified) which then can be inserted into a state.  This state should
 * be in a separate set that will set bit RA_TIMEDONE when time
 * has elapsed.  Then this flag can be tested by more than
 * one state.
 */
RA_RAMP_TIME
ra_compute_time(long rnum, long presetper, long randper)
{
	RA_RAMP_TIME ra_time;
	long lpreset, lrandom;
	unsigned preset, random;
	RAMP *rp;

	if(rnum < 0 || rnum >= RA_MAXNUM) {
	    rxerr("ra_tset(): Bad ramp number");
	    ra_time.ra_ramp_time_preset= -1;
	    return(ra_time);
	}
	rp= &ramp[rnum];

	rp->ra_rampflag &= ~RA_TIMEDONE;
	presetper= 100 - presetper;
	lpreset= (long)presetper * rp->ra_duration;
	preset= lpreset / 100;
	if(rp->ra_timeleft <= preset) {	/* time has already elapsed */
		ra_time.ra_ramp_time_preset= 0;
		ra_time.ra_ramp_time_random= 0;
		return(ra_time);
	}
	ra_time.ra_ramp_time_preset= (rp->ra_timeleft - preset) *
	    rp->ra_urate;
	if(randper) {
		lrandom= (long)randper * rp->ra_duration;
		random= lrandom / 100;
		ra_time.ra_ramp_time_random= random * rp->ra_urate;
	} else ra_time.ra_ramp_time_random= 0;
	return(ra_time);
}

int Pra_compute_time(long *rnum, long *presetper, long *randper, RA_RAMP_TIME *ra_time)
{
	*ra_time = ra_compute_time(*rnum, *presetper, *randper);
	return(0);
}

/*
 *	Function called from state set timing ramp to set RA_TIMEDONE
 * flag.  This bit is being tested by main paradigm state set.
 */
int
ramptd(long rnum)
{
	ramp[rnum].ra_rampflag |= RA_TIMEDONE;
	ramp[rnum].ra_rampflag &= ~RA_TIMESTART; /* stop state set until
						    next time */
	return(0);
}

int Pramptd(long *rnum)
{
	ramptd(*rnum);
	return(0);
}

/*
 * Init ramp struct.
 */
int
ra_init(void)
{
    RAMP *rp;
    int i;

    ra_first.ra_back= NP;
    ra_first.ra_for= &ra_last;
    ra_last.ra_back= &ra_first;
    ra_last.ra_for= NP;
    for(rp= &ramp[0], i= 0; rp < &ramp[RA_MAXNUM]; rp++, i++) {
		rp->m_ra_menu= 0;
		rp->ra_type= RA_NO25MS|RA_CENPT;
		rp->m_ra_type= NULLI;
		rp->ra_len= 100;
		rp->m_ra_len= NULLI;
		rp->ra_ang= 0;
		rp->m_ra_ang= NULLI;
		rp->ra_vel= 100;
		rp->m_ra_vel= NULLI;
		rp->ra_xoff= 0;
		rp->m_ra_xoff= NULLI;
		rp->ra_yoff= 0;
		rp->m_ra_yoff= NULLI;
		rp->ra_ecode= RDONECD;
		rp->m_ra_ecode= NULLI;
		rp->ra_state= RA_OFF;
		rp->ra_urate= 1;
		rp->ra_vfactor= 4;  /* see comment in int/ramp.c */
		rp->ra_pontime= 1;
		rp->ra_pofftime= 99;
		rp->ra_for= NP;
		rp->ra_back= NP;
    }
    return(0);
}

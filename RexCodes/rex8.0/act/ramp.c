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
 *	Called from interrupt routine to compute ramp parameters.
 * Uses no floating point, but requires longops.
 *
 *	17Mar83 AVH	Put in Aki's mod for lengths specified in
 *			tenths of degree.
 */

#include <stdio.h>
#include <sys/types.h>
#include "rexhdrs.h"
#include "ramp.h"
#include "int.h"


extern long cosine[];
static long elength= 0, ltmp1= 0, ltmp2= 0;
static long ltmp1a= 0, ltmp2a= 0;
static int quadr= 0;
static int vel= 0;
static unsign xoff= 0, yoff= 0;
static unsign xcen= 0, ycen= 0;

/*
 *	This function computes parameters to generate a ramp
 * from polar coordinate inputs: length, angle, and velocity.
 * In addition, an x, y offset can be specified for either the begin
 * point, middle, or end point of the ramp.
 *	To allow time for mirrors to reach constant velocity,
 * a 25ms startup time can be added to each ramp.  This time results
 * in varying distances prepended to the start of the ramp
 * depending on the velocity.
 *	Calibration is 40 D/A steps per degree.  For a 12
 * bit D/A this yields 102.4 degress full scale.  Tangent error
 * will result if mirrors are not the same distance from screen as
 * eyeball.
 */
void
ra_compute(int rnum)
{
	RAMP *rp;
	RA_PARAM *r3_xp, *r4_yp, *r3_rp;
	int acctime;

	if(rnum < 0 || rnum >= RA_MAXNUM) {
	    rxerr("ra_compute(): Bad ramp number");
	}
	rp= &ramp[rnum];
	rp->ra_rampflag= 0;

	/*
	 * Multiply by 1000 for lengths in degrees, 100 in tenths.
	 */
	if(rp->ra_vel == 0) {
	    rp->ra_duration= 0;
	} else {
	    rp->ra_duration= ((100 * (unsigned)rp->ra_len)/(unsigned)rp->ra_vel) * 2;
	    rp->ra_duration= rp->ra_duration / rp->ra_urate;
	}

	/*
	 * dtime can include 25ms extra to allow mirror to accelerate.
 	 */
	if(rp->ra_type & RA_NO25MS) acctime= 0;
		else acctime= 25 / rp->ra_urate;
	rp->ra_x.ra_dtime= rp->ra_y.ra_dtime= rp->ra_duration + acctime;
	for(quadr= 0; ; quadr++)  {	/* get quadrant */
		if(rp->ra_ang < 90) break;
		rp->ra_ang -= 90;
	}

	/*
	 * Compute (with scaled arithmetic) dx and dy.
	 * 12 bit A/D, 40 steps/deg.
	 * Scale factor= 2^17.
	 * If specified, the initial position of the ramp is adjusted so
	 * that the mirrors have 25ms to accelerate.  Variable "vel" is
	 * used below to compute how far to back up the initial position
	 * for the specified velocity.  After 25ms the mirrors will be
	 * at the starting position, hopefully at the correct velocity.
	 * "vfactor", currently set at 4, is applied to compensate for
	 * acceleration time of mirrors.
	 */
	if(rp->ra_type & RA_NO25MS) vel= 0;
	else vel= rp->ra_vel;
	vel =- (vel / rp->ra_vfactor);

	/*
	 * For lengths in degrees, mul by 80; for tenths use 8.
	 */
	elength= (8 * rp->ra_len) + vel;	/* increase len by 25ms */
	ltmp1= elength * cosine[rp->ra_ang];
	ltmp1a= (vel) * cosine[rp->ra_ang];		/* increment */
	ltmp2= elength * cosine[90-rp->ra_ang];
	ltmp2a= (vel) * cosine[90-rp->ra_ang];

	/*
	 * Must switch dx, dy depending on quadrant.
	 */
	if(quadr & 01)  {		/* odd quadrant */
		rp->ra_y.ra_d= longhi_(ltmp1) >> 1;
		rp->ra_y.ra_de= longhi_(ltmp1a) >> 1;
		rp->ra_x.ra_d= longhi_(ltmp2) >> 1;
		rp->ra_x.ra_de= longhi_(ltmp2a) >> 1;
	} else {
		rp->ra_x.ra_d= longhi_(ltmp1) >> 1;
		rp->ra_x.ra_de= longhi_(ltmp1a) >> 1;
		rp->ra_y.ra_d= longhi_(ltmp2) >> 1;
		rp->ra_y.ra_de= longhi_(ltmp2a) >> 1;
	}

	/*
 	 * Compute slopes, etc.
	 */
	r3_rp= &rp->ra_x;
	for(;;)  {
		if(r3_rp->ra_d == 0)  {		/* slope = 0 */
			r3_rp->ra_mode= RA_NOMOVE;
			goto next;
		}
		if(r3_rp->ra_dtime == 0) {
		    r3_rp->ra_quot= 0;
		    r3_rp->ra_rem= 0;
		} else {
		    r3_rp->ra_quot= r3_rp->ra_d / r3_rp->ra_dtime;
		    r3_rp->ra_rem= r3_rp->ra_d % r3_rp->ra_dtime;
		}
		if(r3_rp->ra_quot)  {		/* slope >= 1 */
			if(r3_rp->ra_rem == 0)  {	/* slope == int */
				r3_rp->ra_mode= RA_EQINT;
				goto next;
			}
			r3_rp->ra_mode= RA_GREAT1;
			r3_rp->ra_err= r3_rp->ra_dtime >> 1;
			goto next;
		}
		r3_rp->ra_mode= RA_LESS1;	/* slope < 1 */
		r3_rp->ra_err= r3_rp->ra_dtime >> 1;

	next:				/* do y */
		if(r3_rp == &rp->ra_y) break;
		r3_rp= &rp->ra_y;
	}

	/*
	 * Adjust signs depending on quadrant.
	 * Currently not optimizing endpoints or starting point on
	 * lines of slope > 2.
	 */
	r3_xp= &rp->ra_x;
	r4_yp= &rp->ra_y;
	xcen= rp->ra_xoff << TEN_TO_IC;
	ycen= rp->ra_yoff << TEN_TO_IC;
	if(rp->ra_type & RA_BEGINPT) {
		xoff= r3_xp->ra_de;
		yoff= r4_yp->ra_de;
	} else if(rp->ra_type & RA_ENDPT) {
		xoff= r3_xp->ra_d;
		yoff= r4_yp->ra_d;
	} else {
		xoff= ((r3_xp->ra_d + r3_xp->ra_de) >> 1);
		yoff= ((r4_yp->ra_d + r4_yp->ra_de) >> 1);
	}

	/*
	 *	REX internal calibrations are 2s comp, x and y positive in 
	 * quad 0.  The xmout_, ymout_ macros perform conversions necessary
	 * for specific D/A being used.
	 *	Direction of a ramp is defined as proceeding
	 * into the quadrant the angle falls in.  For
	 * example, and angle of 10 deg falls in quad 0, but the
	 * initial position would be in quad 3 with the scan ending
	 * in quad 0.
	 *	The above assumes actual monkey view from chair.
	 */
	switch(quadr)  {

	case 0:
		r3_xp->ra_inc= 1;
		r4_yp->ra_inc= 1;
		r3_xp->ra_pos= xcen - xoff;
		r4_yp->ra_pos= ycen - yoff;
		break;
	case 1:
		r3_xp->ra_inc= -1;
		r4_yp->ra_inc= 1;
		r3_xp->ra_quot= -r3_xp->ra_quot;
		r3_xp->ra_pos= xcen + xoff;
		r4_yp->ra_pos= ycen - yoff;
		break;
	case 2:
		r3_xp->ra_inc= -1;
		r4_yp->ra_inc= -1;
		r3_xp->ra_quot= -r3_xp->ra_quot;
		r4_yp->ra_quot= -r4_yp->ra_quot;
		r3_xp->ra_pos= xcen + xoff;
		r4_yp->ra_pos= ycen + yoff;
		break;
	case 3:
		r3_xp->ra_inc= 1;
		r4_yp->ra_inc= -1;
		r4_yp->ra_quot= -r4_yp->ra_quot;
		r3_xp->ra_pos= xcen - xoff;
		r4_yp->ra_pos= ycen + yoff;
		break;
	}
	rp->ra_rampflag |= RA_CDONE;
}

/*
 * Cosine table.  Each entry scaled by 2^17.
 */
long cosine[] = {
	131072L,	/* 0 */
	131052L,	/* 1 */
	130992L,	/* 2 */
	130892L,	/* 3 */
	130753L,	/* 4 */
	130573L,	/* 5 */
	130354L,	/* 6 */
	130095L,	/* 7 */
	129796L,	/* 8 */
	129458L,	/* 9 */
	129081L,	/* 10 */
	128664L,	/* 11 */
	128208L,	/* 12 */
	127713L,	/* 13 */
	127179L,	/* 14 */
	126606L,	/* 15 */
	125994L,	/* 16 */
	125345L,	/* 17 */
	124657L,	/* 18 */
	123931L,	/* 19 */
	123167L,	/* 20 */
	122366L,	/* 21 */
	121528L,	/* 22 */
	120652L,	/* 23 */
	119740L,	/* 24 */
	118792L,	/* 25 */
	117807L,	/* 26 */
	116786L,	/* 27 */
	115730L,	/* 28 */
	114638L,	/* 29 */
	113512L,	/* 30 */
	112351L,	/* 31 */
	111155L,	/* 32 */
	109926L,	/* 33 */
	108664L,	/* 34 */
	107368L,	/* 35 */
	106039L,	/* 36 */
	104679L,	/* 37 */
	103286L,	/* 38 */
	101862L,	/* 39 */
	100407L,	/* 40 */
	98921L,		/* 41 */
	97405L,		/* 42 */
	95860L,		/* 43 */
	94285L,		/* 44 */
	92682L,		/* 45 */
	91050L,		/* 46 */
	89391L,		/* 47 */
	87704L,		/* 48 */
	85991L,		/* 49 */
	84251L,		/* 50 */
	82486L,		/* 51 */
	80696L,		/* 52 */
	78881L,		/* 53 */
	77042L,		/* 54 */
	75180L,		/* 55 */
	73295L,		/* 56 */
	71387L,		/* 57 */
	69458L,		/* 58 */
	67507L,		/* 59 */
	65536L,		/* 60 */
	63545L,		/* 61 */
	61535L,		/* 62 */
	59505L,		/* 63 */
	57458L,		/* 64 */
	55393L,		/* 65 */
	53312L,		/* 66 */
	51214L,		/* 67 */
	49100L,		/* 68 */
	46972L,		/* 69 */
	44829L,		/* 70 */
	42673L,		/* 71 */
	40503L,		/* 72 */
	38322L,		/* 73 */
	36128L,		/* 74 */
	33924L,		/* 75 */
	31709L,		/* 76 */
	29485L,		/* 77 */
	27251L,		/* 78 */
	25010L,		/* 79 */
	22760L,		/* 80 */
	20504L,		/* 81 */
	18242L,		/* 82 */
	15974L,		/* 83 */
	13701L,		/* 84 */
	11424L,		/* 85 */
	9143L,		/* 86 */
	6860L,		/* 87 */
	4574L,		/* 88 */
	2288L,		/* 89 */
	0L,		/* 90 */
};

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
 * Actions for calibration paradigms.
 */
#include <stdio.h>
#include <sys/types.h>
#include "rexhdrs.h"
#include "int.h"

/*
 *	Get eye position for possible storage if good trial.
 * Convert from internal calibration of 40st/deg to 10st/deg.
 */
int
hold_eye(void)
{
	c_xhold= eyeh >> 2;
	c_yhold= eyev >> 2;
	c_oxhold= oeyeh >> 2;
	c_oyhold= oeyev >> 2;
	return(0);
}

/*
 *	Store eye position in array.
 */
int
keep_eye(void)
{
	if(c_curp < 0 || c_curp >= C_KEEPMAX) c_curp= 0;
	c_eyekeep[c_curp].c_xsav= c_xhold;
	c_eyekeep[c_curp].c_ysav= c_yhold;
	c_eyekeep[c_curp].c_oxsav= c_oxhold;
	c_eyekeep[c_curp].c_oysav= c_oyhold;
	c_curp++;
	return(0);
}


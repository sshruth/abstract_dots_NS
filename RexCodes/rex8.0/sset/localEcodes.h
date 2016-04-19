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
 *	Event codes specific per laboratory.
 */
#define FPONCD			1010
#define FPOFFCD			1011
#define FPDIMCD			1012
#define FPONBLINKCD		1013
#define FPOFFBLINKCD	1014
#define REWCD			1030
#define ERRCD			1031
#define STIMCD			1100
#define STOFFCD			1101
#define STONBLINKCD		1102	/* stim blink */
#define STOFFBLINKCD	1103	/* stim blink off */
#define TARGONCD		1110
#define TARGOFFCD		1111
#define SACSTARTCD		1120
#define PHIONCD			1125	/* phi blink on */
#define PHISTARTCD		1127	/* phi sequence started */
#define PHIENDCD		1129	/* phi sequence ended */
#define STBONCD			1148
#define STBOFFCD		1164

#define BASECD			1500	/* Code that can be modified by an
								   action return.  Leave with 300 consecutive
								   unused numbers.  */

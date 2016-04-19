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
 *	Initializations for scribe process.
 */

#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include "rexhdrs.h"
#include "scribe.h"

/*
 * Version indentifier for scribe process.
 */
char version[P_LVERSION]= "6.0 Oct 1999";

char NS[]= "";
int analog_fd= 0;
int event_fd=  0;
PROCTBL_P myptp= NP;
int myptx= -1;
int derror= 0;
sigset_t alert_set;

/*
 *	Channel info for E and A files.
 */
CHANNEL a_chan= {
	0
};
CHANNEL e_chan= {
	0
};

INT_BLOCK_P i_b= 0;
RAW_BLOCK_P r_b= 0;

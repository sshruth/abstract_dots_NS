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
 * Request by Rex processes other than comm to put comm to sleep.
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include "rexhdrs.h"

void sleep_comm(void)
{
	r_sendmsg(COMM, CM_SLEEP);
	while( ! (i_b->c_flags & C_ASLEEP)) sleep(1);
}

void wakeup_comm(void)
{
	r_sendmsg(COMM, CM_WAKEUP);
}

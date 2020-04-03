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
 *	Reinitialize the state lists.
 */
#include <stdio.h>
#include <sys/types.h>
#include "rexhdrs.h"
#include "ecode.h"
#include "int.h"

int
sf_init(long x)
{
	int (**ptr)();
	extern int (*init_list[])();

	reset_s(-1);
	for (ptr = init_list; *ptr != 0; ptr++) {
		(**ptr)();
	}
	/* set up state linked list */
	stateinit();
	return(x | INIT_MASK);
}

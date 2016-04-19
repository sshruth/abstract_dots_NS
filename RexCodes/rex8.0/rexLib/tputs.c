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
 * Write strings to tty using one write call.
 */

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include "sys.h"

void tputs(char *p)
{
	char *sp= p;
	int count, rval;

	while(*sp++);
	sp--;
	count= sp - p;

	/*
	 * Make provision for interruption by signal.
	 */
	for(;;) {
	    if((rval=write(1, p, count)) == -1) {
		if(errno == EINTR) continue;
		else break;
	    }
	    if(rval != count) {
		p += rval;
		count -= rval;
		continue;
	    }
	    break;
	}
}

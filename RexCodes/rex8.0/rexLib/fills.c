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

#include <sys/types.h>
#include "sys.h"

/*
 * Fill string starting at given address with 'num' instances of char 'c'.
 * Return pointer to next char after end of filled chars.
 */
char *
fills(char c, int num, char *p, char *ep)
{
	if(p >= ep) return(ep);

	while(num--) {
		*p++ = c;
		if(p == ep) break;
	}
	return(p);
}

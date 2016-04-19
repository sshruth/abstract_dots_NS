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
 * Copy first string (including null) into second string.  Return pointer
 * to null termination in second string.
 */
/*
char far *
_fstufs(char far *from, char far *to, char far *ep)
{
	if(to >= ep) goto out;
	while(to < ep) if((*to++ = *from++) == '\0') return(--to);

	*(ep - 1) = '\0';

out:
	return(ep);
}
*/

char *
stufs(char *from, char *to, char *ep)
{
	if(to >= ep) goto out;
	while(to < ep) if((*to++ = *from++) == '\0') return(--to);

	/*
	 * Get here if string overflows.
	 * Null terminate and return ep.
	 */
	*(ep - 1) = '\0';

out:
	return(ep);
}

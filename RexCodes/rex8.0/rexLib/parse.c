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
 
#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
#include "sys.h"

/*
 * Parse string using white space as delimiter.  Possibly null terminate each
 * parsed arg in string, and place address of arg into array *argv[],
 * as in standard C argument passing convention.  Array is also
 * null terminated.  Returns number of args, or -1 for error.
 * Parsing terminates if '\n' seen; the '\n' is replaced by '\0'.
 */
int parse(int ntflag, char *p, char *argv[], int nargs)
{
	int argc;

	if(nargs <= 0) goto errout;
	nargs--;		/* guarantee space for null termination */

	for(argc= 0; argc < nargs; argc++, argv++) {

		for(;; p++) {
			if(*p == '\n' || *p == '\0') {
				*p= '\0';
				*argv= NP;
				return(argc);
			}
			if(isspace(*p)) continue;
			break;
		}
		if(!isgraph(*p)) {
		    rxerr("parse(): Illegal char detected in parse string");
		    return(-1);
		}
		*argv= p;
		for(; isgraph(*p); p++);
		if((*p != '\0') && (*p != '\n') && ntflag) *p++= '\0';
	}

	/*
	 * Error: argc overflow.
	 */
	*argv= NP;

errout:
	rxerr("parse(): Too many args or illegal argcnt passed to parse()");
	return(-1);
}

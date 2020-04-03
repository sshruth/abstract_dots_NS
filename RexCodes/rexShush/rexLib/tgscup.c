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
 * Generate cursor positionings string.
 * These routines are tty dependent.  VT100 version.
 */

#include <sys/types.h>
#include "sys.h"

/*
 * Generate absolute position string.
 */
char *
tgscup(int line, int col, char *sp, char *ep)
{
	*sp++ = '\033';
	*sp++ = '[';
	sp = itoa_RL(line, 'd', sp, ep);
	*sp++ = ';';
	sp = itoa_RL(col, 'd', sp, ep);
	*sp++ = 'f';
	*sp = '\0';
	return(sp);
}

/*
 * Generate string to move cursor either ahead or backward on same line.
 */
char *
tgsfb(int col, char *sp, char *ep)
{
	int back;

	*sp++ = '\033';
	*sp++ = '[';
	if(col <= 0) {
		back= 1;
		col= -col;
	} else back= 0;
	sp= itoa_RL(col, 'd', sp, ep);
	*sp++ = (back ? 'D' : 'C');
	*sp= '\0';
	return(sp);
}

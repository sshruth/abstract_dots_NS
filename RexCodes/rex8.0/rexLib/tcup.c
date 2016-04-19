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
 * Position cursor on tty.
 * If line arg is negative, take col arg to be number of positions to move
 * cursor forward (if positive) or backward (if negative) on same line.
 */

#include <stdio.h>
#include <sys/types.h>
#include "sys.h"
#include "tty.h"

void tcup(int line, int col)
{
	char s[20];
	register char *ep= &s[20];

	if(line >= 0) tgscup(line, col, s, ep);
	else tgsfb(col, s, ep);
	fputs(s, stdout);
}

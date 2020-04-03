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
 * Set scrolling regions.
 * This routine is tty dependent.  VT100 version.
 */

#include <stdio.h>
#include <sys/types.h>
#include "sys.h"
#include "tty.h"

void tsetsc(int top, int bot)
{
	char s[20];
	char *sp= s;
	char *ep= &s[20];

	*sp++ = '\033';
	*sp++ = '[';
	sp = itoa_RL(top, 'd', sp, ep);
	*sp++ = ';';
	sp = itoa_RL(bot, 'd', sp, ep);
	*sp++ = 'r';
	*sp++ = '\0';
	fputs(s, stdout);
}

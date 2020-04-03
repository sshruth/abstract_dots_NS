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
#include <sys/types.h>
#include "rexhdrs.h"
#include "ecode.h"
#include "int.h"

int ttotal= 0;

int
score(long x)
{
	if(x == YES) {
		tyes++;
		i_b->inthdr.i_correct++;
#ifdef NIH_ACUC	/* code compiled to comply with NIH ACUC requirements */
		i_b->inthdr.i_lastCorrect = i_b->i_time;
#endif
	} else {
		tno++;
		i_b->inthdr.i_wrong++;
		noflag++;
	}
	ttotal++;

	return(0);
}

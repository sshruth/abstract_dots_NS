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
 * Find first instance of a char in string.  Return pointer to char.
 */

#include <sys/types.h>
#include "sys.h"

/*
char far *
_findex_RL(char c, char far *str)
{
	do {
		if(*str == c) return(str);
	} while(*str++ != '\0');
	return(NP);
}
*/

char *
index_RL(char c, char *str)
{
	do {
		if(*str == c) return(str);
	} while(*str++ != '\0');
	return(NP);
}

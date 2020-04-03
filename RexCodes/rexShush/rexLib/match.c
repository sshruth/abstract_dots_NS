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
 * Match character strings.
 * Returns 1 if first sting is either the same as or a subset of the second
 * string.  If the first string is null and the second is not, the first string
 * is not considered a subset.
 */
int match(char *str1, char *str2)
{
	if(*str1 == '\0' && *str2) return(0);

	do {
		if (*str1++ != *str2++) return(0);
	} while(*str1 != '\0');

	return(1);
}

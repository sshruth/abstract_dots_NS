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
#include <stdio.h>
#include "sys.h"
#include "tty.h"

/*
 *	Match strings and return the index into the array
 * for the first match.  It expects a pointer to the buffer to
 * be matched, a pointer to the first pointer of a string in the
 * array, and the size of each array entry -- e.g. the size of
 * a single struct of the array of structs.  This whole routine
 * should be removed and it's function accomplished some other
 * way :-).
 */
int sindex(char *mstr, char **ptr, int size)
{
	char *ptr1, *bptr;
	int tmp;
	int i;

	i = 0;

	while (**ptr != '\0'){

		ptr1 = *ptr;
		bptr = mstr;
		for (;;) {
			if ((*bptr != *ptr1) || (*ptr1 == '\0')) {
				if (*bptr == '\0') return(i);
				else break;
			}
			bptr++;
			ptr1++;
		}
		i++;
		tmp= (int)ptr;
		tmp += size;
		ptr= (char **)tmp;
	}
	return(-1);
}

#include <stdio.h>
#include "tig.h"

/*
 * GETWD
 *	read a short int from stdin
 *
 * 02-06-89  Modified to work on sparc.  Brad J. Zoltick and Jim Carl
 */

short int
get_tig_wd()
{
	short int x, tmpx;

#ifdef sun
	tmpx = getchar();
	x = (tmpx & 0377) + (short int) (256 * getchar());
#endif
#ifdef sgi
	tmpx = getchar();
	x = (tmpx & 0377) + (short int) (256 * getchar());
#endif
#ifdef vax
	((BYTES)&x)->lobyte = getchar();
	((BYTES)&x)->hibyte = getchar();
#endif
#ifdef linux
	((BYTES)&x)->lobyte = getchar();
	((BYTES)&x)->hibyte = getchar();
#endif
#ifdef __QNX__
	((BYTES)&x)->lobyte = getchar();
	((BYTES)&x)->hibyte = getchar();
#endif

	return(x);
}

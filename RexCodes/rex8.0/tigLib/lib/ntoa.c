#include <stdio.h>

#define	BUFS	33	/* Maximum number of characters in long */
			/* Could be in base 2 */
/*
 *	NTOA( <long integer>, <base>, <pointer to char count> )
 *
 *	Returns:	<ptr to buffer>
 *			 NULL		if Base is out of range
 */

char *ntoa( n, base, retp )
long n;
register int base;
int *retp;
{
	register char *cp;
	int sign = {0};
	static char buf[BUFS+1];

	if(base < 2 || base > 62)
		return(NULL);
	*(cp = &buf[BUFS]) = 0;
	if(n == 0) {		/* Special case */
		if(retp)
			*retp = 1;
		*--cp = '0';	/* Want to return string with null-term zero */
		return(cp);
	}
	if(n < 0) {
		n = -n;
		sign--;
	}

	while(n) {
		*--cp = n%base + '0';
		if(*cp > '9')
			*cp += 'A' - '9' - 1;
		if(*cp > 'Z')
			*cp += 'a' - 'Z' - 1;
		n = n/base;
	}
	if(sign)
		*--cp = '-';

	if(retp)
		*retp = &buf[BUFS] - cp;

	return(cp);
}

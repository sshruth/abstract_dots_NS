/*
 *	ATON( <string pointer>, <base>, <maximum char>, <returned pointer> )
 *
 *	Returns:	<long value>
 *			  -1		Improper base; errno set to EINVAL
 *
 *	NOTE:	The maximum character allows one to limit the input bandwidth
 *		for out-of-base-range characters, e.g. if one is calculating
 *		in base 10 but wants to allow HEX representations, he will
 *		set the limit to 'F'.
 */
#include	<errno.h>
#include	<ctype.h>

extern int errno;

long aton(strp,base,maxc,retp)	/* ASCII to Number in any base from 1-62 */
register char *strp;
register int base;
char maxc, **retp;
{
	register int sign = {0};
	long n;

	if(base < 2 || base > 62) {
		errno = EINVAL;
		return(-1);
	}
	n = 0L;
	while(*strp == ' ' || *strp == '\t')
		strp++;
	switch(*strp) {
	case '-':
		sign++;
	case '+':
		strp++;
	}

	while( *strp <= maxc && isalnum(*strp)) {
		n = n * base;
		n = n + *strp - '0';
		/* Say lower|upper since if lower, then need to add this difference also */
		if(isalpha(*strp))
			n = n + '9' - 'A' + 1;	/* Difference from 0-9-->A-Z */
		if(islower(*strp++))
			n = n + 'Z' - 'a' + 1;	/* Difference from A-Z-->a-z */
		if(n < 0) {	/* Overflow */
			errno = EDOM;
			return(-1);
		}
	}

	if(retp)
		*retp = strp;	/* Return the character just past the end */
	return(sign? -n:n);
}

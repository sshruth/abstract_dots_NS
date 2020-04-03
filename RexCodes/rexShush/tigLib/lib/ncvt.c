#include <stdio.h>

/*
 *	U S E R   L I B R A R Y   R O U T I N E
 *
 *	By:  G. E. Toth
 *
 *	This is a modified ecvt.c program.
 */

#define	NDIG	100	/* Number of digits allowed max in buffer */

extern double modf();

/*
 *	NCVT( <dbl value>, <no. of digits>, <base>, &<dec. pt. flag>, &<sign flg> )
 *
 *	Returns:	<string pointer to ndigits>
 *				NULL	Base out of range
 */

char *ncvt(arg, ndigits, base, decpt, sign)	/* Convert double to any base */
double arg;
int ndigits;
double base;
int *decpt, *sign;
{
	register int r2;
	double fi, fj;
	register char *p, *p1;
	static char buf[NDIG];

	if(base < 2 || base > 62)
		return(NULL);

	if (ndigits<0)
		ndigits = 0;
	if (ndigits>=NDIG-1)
		ndigits = NDIG-2;
	r2 = 0;
	*sign = 0;
	p = &buf[0];
	if (arg<0) {
		*sign = 1;
		arg = -arg;
	}
	arg = modf(arg, &fi);
	p1 = &buf[NDIG];
	/*
	 * Do integer part
	 */
	if (fi != 0) {
		p1 = &buf[NDIG];
		while (fi != 0) {
			fj = modf(fi/base, &fi);
			/* Remember bases:  0-9A-Za-z */
			*--p1 = (int)((fj+.03)*base) + '0';
			if(*p1 > '9')
				*p1 += 'A' - '9' - 1;
			if(*p1 > 'Z')
				*p1 += 'a' - 'Z' - 1;
			r2++;
		}
		while (p1 < &buf[NDIG])
			*p++ = *p1++;
	} else if (arg > 0) {
		while ((fj = arg*base) < 1) {
			arg = fj;
			r2--;
		}
	}
	p1 = &buf[ndigits];
	*decpt = r2;
	if (p1 < &buf[0]) {
		buf[0] = '\0';
		return(buf);
	}
	for( ; p<=p1 && p<&buf[NDIG]; p++) {
		arg *= base;
		arg = modf(arg, &fj);
		/* Rember bases:  0-9A-Za-z */
		*p = (int)fj + '0';
		if(*p > '9')
			*p += 'A' - '9' - 1;
		if(*p > 'Z')
			*p += 'a' - 'Z' - 1;
	}
	if (p1 >= &buf[NDIG]) {
		buf[NDIG-1] = '\0';
		return(buf);
	}
	p = p1;
	/* Now round the last digit and take care of carry */
	*p1 += (((int)base)>>1);
	r2 = base + '0' - 1;	/* Compose the maximum character */
	if(r2 > '9')
		r2 += 'A' - '9' - 1;
	if(r2 > 'Z')
		r2 += 'a' - 'Z' - 1;

	while(*p1 > r2) {
		*p1 = '0';
		if (p1>buf) {
			++*--p1;
			if(*p1 > '9' && *p1 < 'A')	/* Make the first quantum jump */
				*p1 += 'A' - '9' - 1;
			if(*p1 > 'Z' && *p1 < 'a')	/* Make other jump */
				*p1 += 'a' - 'Z' - 1;
		}
	/*
	 * If rounding occurs all the way through the chain then first
	 * digit becomes 1 and all decimal point is moved forward one
	 */
		else {
			*p1 = '1';
			(*decpt)++;
		}
	}
	*p = '\0';
	return(buf);
}

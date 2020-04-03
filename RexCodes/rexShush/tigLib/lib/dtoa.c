/*
 *	USER LIBRARY ROUTINE		DTOA
 *
 *	By:	George E. Toth,		April 6, 1980
 *
 *	(C) - Copyright, 1980	All Rights reserved.
 */

#include	<math.h>
#include	<stdio.h>

extern char *ncvt();

#define	NCHARS	200	/* Number of characters in buffer allowed */

	/* Default values */

#define	DEF_WID	10	/* Default field width */
#define	DEF_PDC	6	/* Default post-decimal point digits */

#define	ENG	0	/* Engineering format */
#define	DEC	1	/* Decimal format */
#define	EXP	2	/* Exponential format */

/*
 *	DTOA( <number>, <base>, <total field width>, <digits after decimal point>, <padding char>, <mode>, &<char count> )
 *
 *	Calls:	ncvt(VI)
 *
 *	Returns:	<addr>	Success
 *			NULL	Error
 */

char *dtoa( num, base, width, pstdec, padchr, mode, retp )	/* Double to ASCII */
double num, base;
int width, pstdec, padchr, mode, *retp;
{
	register char *cp, *c2p;
	register int i;
	int padcnt, engshft = {0};
	int decpt, sgnf;	/* Decimal point and sign for ncvt() */
	int expf = {0};		/* Exponential format flag */
	static char cbuf[NCHARS];

/*
fprintf(stderr, "dtoa(num=%f, base=%f, wd = %d, pstdec=%d,\npadchr=%c, mode=%d, retp = %x\n",
num, base, width, pstdec, padchr, mode, retp); fflush(stderr);
*/
	c2p = cbuf;	/* Initialize */
	if( mode > EXP ) {	/* see if it's characters */
		switch(mode) {
		case 'E':
			mode = ENG;
			break;
		case 'e':
			mode = EXP;
			break;
		case 'd':
		default:
			mode = DEC;
		}
	}

	if(width <= 0)
		width = DEF_WID;
	if(pstdec < 0)
		pstdec = DEF_PDC;

	if(width < pstdec)	/* Digits after can't be > width */
		width = pstdec;

	if((int)(cp=ncvt(num,width+3,base,&decpt,&sgnf)) == NULL)
		return(NULL);

	/* If negative put out minus sign */

	if( sgnf )
		*c2p++ = '-';

	/*
	 * Calculate # padding chars, # predec digits,
	 * # of pstdec digits
	 *
	 * For eng. Format (E):
	 * If the number can be printed as a simple
	 * decimal number without sacrificing pstdec
	 * digits, the all parameters follow the rules
	 * for simple decimal.
	 * If the number cannot, a exponential form is
	 * is built.  The exponential form is first
	 * tried by shifting the digits up (lowering
	 * the exponential power).  If the padding
	 * space does not allow this, then the number
	 * is shifted down (raising the exponent).
	 * At all times the exponent is made a multiple
	 * of three.
	 *
	 * For decimal format (d):
	 * The number of digits before the decimal
	 * is calculated.  If this number is greater
	 * than the number of possible predec digits,
	 * sacrificing all pstdec digits if needed,
	 * the number is printed as an exponential.
	 * If decimal point is to the left of the
	 * digits (decpt < 0) then the padding width
	 * is the width minus 1.  A zero will always
	 * preceed the decimal point.  If the number
	 * would not have at least one digit in the
	 * digits following the decimal point (pstdec)
	 * then the number is printed as a simple
	 * exponential.
	 *
	 * For exponential format (e):
	 * There will always be one digit before the
	 * decimal point.  If the exponent is 1 then
	 * a zero will be placed before the number,
	 * and the remaining digits follow.  On all
	 * other circumstances the first digit will
	 * be placed before the decimal point.  pstdec
	 * digits will always follow the decimal point.
	 * If decpt (the exponent) is other than 0 or 1
	 * the letter 'E' will appear with the sign of
	 * the number followed by the exponent in its
	 * own base.
	 */

	switch(mode) {
	case ENG:
		if( decpt < 0 )
			engshft = 3 + decpt%3;
		else	engshft = decpt%3;

		if(decpt <= -pstdec || decpt > width-pstdec) {
			if(engshft > width-pstdec) {
				engshft -= 3;
				padcnt = width-pstdec-1;
			}
			else	padcnt = width-pstdec-engshft;
			expf = 1;	/* Exponential form */
			break;
		}
	case DEC:
		if(decpt <= width && decpt > -pstdec) {
			if( decpt < 0 )
				padcnt = width-pstdec-1;
			else {
				if(decpt <= width-pstdec)
					padcnt = width-pstdec-decpt;
				else {	/* Sacrifice pstdec digits */
					padcnt = 0;
					pstdec = width-decpt;
				}
			}
			expf = 0;	/* Decimal format */
			break;
		}
	case EXP:
		padcnt = width-pstdec-1;
		expf = 1;	/* Exponential format */
	}

	/* Put in padding characters if wanted */

	if( padchr )	/* Pad left with this character */
		for(i=0; i++ < padcnt && c2p-cbuf < NCHARS-1; *c2p++ = padchr);


	/* Digits before decimal point */

	switch(mode) {
	case ENG:
		if( expf ) {
			if(engshft <= 0) {
				if( c2p-cbuf < NCHARS-1 )
					*c2p++ = '0';
			}
			else {
				for(i=0; i++ < engshft; cp++ )
					if( c2p-cbuf < NCHARS-1 )
						*c2p++ = *cp;
			}
			break;
		}
	case DEC:
		if( !expf ) {
			if(decpt <= 0) {
				if( c2p-cbuf < NCHARS-1 )
					*c2p++ = '0';
			}
			else	for(i=0; i++ < decpt; cp++)
					if( c2p-cbuf < NCHARS-1 )
						*c2p++ = *cp;
			break;
		}
	case EXP:
		if( !decpt ) {	/* Special case: 0.#### */
			if( c2p-cbuf < NCHARS-1 )
				*c2p++ = '0';
		}
		else {
			if( c2p-cbuf < NCHARS-1 )
				*c2p++ = *cp++;
			else	cp++;
			--decpt;	/* one less */
		}
	}

	/* Now put in decimal point and post decimal digits */

	if( pstdec ) {
		if( c2p-cbuf < NCHARS-1 )
			*c2p++ = '.';
		if( expf ) {
			for(i=engshft; i++ < 0 && c2p-cbuf < NCHARS-1; pstdec--)
				*c2p++ = '0';
			for(i=0; i++ < pstdec; cp++)
				if( c2p-cbuf < NCHARS-1 )
					*c2p++ = *cp;
		}
		else {
			for( ; decpt < 0; decpt++, pstdec--)
				if( c2p-cbuf < NCHARS-1 )
					*c2p++ = '0';
			while( pstdec-- > 0 && c2p-cbuf < NCHARS-1 )
				*c2p++ = *cp++;
		}
	}
	if( expf && decpt != engshft) {		/* Exponential form */
		if( c2p-cbuf < NCHARS-1 )
			*c2p++ = 'E';
		cp = ncvt((double)decpt-engshft,50,base,&decpt,&sgnf);
		if( c2p-cbuf < NCHARS-1 ) {
			if( sgnf )
				*c2p++ = '-';
			else	*c2p++ = '+';
		}
		while( decpt-- > 0 )
			if( c2p-cbuf < NCHARS-1 )
				*c2p++ = *cp;
	}

	*c2p++ = 0;	/* Null terminate */

	if( retp )	/* Return character count */
		*retp = c2p-cbuf;

	return( cbuf );
}

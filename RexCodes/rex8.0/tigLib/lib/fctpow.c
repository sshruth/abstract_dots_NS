#

/*
 *	T E R M I N A L   I N D E P E N D E N T   G R A P H I C S
 *
 *			by:  George E. Toth
 *			  March 29, 1980
 *
 *	These routines produce the Terminal Independent Graphics
 *	Intermediate language designed by Michael J. Muuss.
 *
 *		R E V I S I O N  H I S T O R Y
 *	===================================================
 *	03/29/80  GET	Wrote subroutine.
 *
 */
#include	<stdio.h>
#include	<math.h>
#include	"tig.h"

/*
 *	_FCTPOW( <number>, <mode>, &<coefficient struct>, &<format> )
 *
 *	Calls:	exp(3M), pow(3M), pltstr(), _strconv()
 *
 *	Returns:	&<info struct>	Always
 */

struct Fcttype *_Fctpow( num, mode, coefp, formatp )	/* Perform POWER functions */
double num;
int mode;
register struct coef *coefp;
char *formatp;
{
	double ddarray[2];
	double args[8];
	static struct Fcttype fctret;

	switch(mode) {
	case DAT2DIS:		/* Convert a Data point to a Display point */
		fctret.Retdbl = num - coefp->_Coef[INTERCEPT];
		fctret.Retdbl /= coefp->_Coef[SLOPE];
		fctret.Retdbl = pow( fctret.Retdbl, 1/coefp->_Coef[POWER] );
		break;
	case DIS2DAT:		/* Convert a Display point to a Data point */
		fctret.Retdbl = pow( num, coefp->_Coef[POWER] );
		fctret.Retdbl *= coefp->_Coef[SLOPE];
		fctret.Retdbl += coefp->_Coef[INTERCEPT];
		break;
	case DIS2dis:		/* A display number is as it is */
	case dis2DIS:
		fctret.Retdbl = num;
		break;
	case NUMPREP:		/* Prepare number so that someone else can print it */
		ddarray[0] = num;
		ddarray[1] = coefp->_Coef[POWER];
		fctret.Retinfo.TIGstrp = _strconv(formatp, ddarray, &fctret.Retinfo.soffset, &fctret.Retinfo.slength);
		break;
	case NUMPRNT:		/* Print the number in the current location */
		args[0] = num;
		args[1] = coefp->_Coef[POWER];
		args[2] = (double)0;
		pltstr(formatp, args);
	}

	return( &fctret );
}

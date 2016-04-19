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
 *	_FCTLIN( <number>, <mode>, &<coefficient struct>, &<format> )
 *
 *	Calls:	pltstr(), _strconv()
 *
 *	Returns:	&<Information union>	Always
 */

struct Fcttype *_Fctlin( num, mode, coefp, formatp )	/* Perform LINEAR functions */
double num;
int mode;
register struct coef *coefp;
char *formatp;
{
	double args[8];
	static struct Fcttype fctret;

	switch(mode) {
	case DAT2DIS:		/* Convert a Data point into a Display point */
		fctret.Retdbl = (num - coefp->_Coef[INTERCEPT]) / coefp->_Coef[SLOPE];
		break;
	case DIS2DAT:		/* Convert a Display point into Data */
		fctret.Retdbl = coefp->_Coef[SLOPE] * num + coefp->_Coef[INTERCEPT];
		break;
	case DIS2dis:		/* A display number is as it is */
	case dis2DIS:
		fctret.Retdbl = num;
		break;
	case NUMPREP:		/* Prepare number and return pointer to struct */
		fctret.Retinfo.TIGstrp = _strconv(formatp, &num, &fctret.Retinfo.soffset, &fctret.Retinfo.slength);
		break;
	case NUMPRNT:		/* Print the number in the current location */
		args[0] = num;
		args[1] = (double)0;
		pltstr(formatp, args);
	}
	return( &fctret );
}

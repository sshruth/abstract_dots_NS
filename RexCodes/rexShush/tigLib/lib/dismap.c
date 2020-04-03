
/*
 *	T E R M I N A L   I N D E P E N D E N T   G R A P H I C S
 *
 *			by:  George E. Toth
 *			  April 20, 1980
 *
 *	These routines produce the Terminal Independent Graphics
 *	Intermediate language designed by Michael J. Muuss.
 *
 *		R E V I S I O N  H I S T O R Y
 *	===================================================
 *	04/20/80  GET	Wrote subroutine.
 *	02/09/89  BJR & BJZ Test in DIS2TIG:
 *	whether value - coefp->_CB arbitrarily close to 0.00 (1E-11)  
 */
#include 	<stdio.h>
#include	<math.h>
#include	"tig.h"

/*
 *	DIS2TIG( <value>, &<coefficient struct> )
 *
 *	Returns:	<TIG point>	Always
 */

double DIS2TIG( value, coefp )	/* Display point to TIG point conversion */
double value;
register struct coef *coefp;
{

	if( coefp->_CA == 0 )
		return( HUGE );
	return( (value - coefp->_CB) / coefp->_CA );
}

/*
 *	TIG2DIS( <value>, &<coefficient struct> )
 *
 *	Returns:	<Display pt.>	Always
 */

double TIG2DIS( value, coefp )	/* TIG space to display space conversion */
double value;
register struct coef *coefp;
{

	return( coefp->_CA * value + coefp->_CB );
}

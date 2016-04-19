#

/*
 *	T E R M I N A L   I N D E P E N D E N T   G R A P H I C S
 *
 *			by:  George E. Toth
 *			  March 10, 1980
 *
 *	These routines produce the Terminal Independent Graphics
 *	Intermediate language designed by Michael J. Muuss.
 *
 *		R E V I S I O N  H I S T O R Y
 *	===================================================
 *	03/10/80  GET	Wrote subroutine.
 *	01/29/89  BJZ	Fixed argument list in *Fctcall(). 
 *	02/12/89  BJZ	Rewrote fit() to work on sparc.
 */
#include	<stdio.h>
#include	<math.h>
#include	"tig.h"

extern double numnice();

#define	MNBASE	2	/* Minimum base */
#define	MXBASE	62	/* Maximum base */

#define	ALLOWNC	0.05	/* Allowance of graph expansion at each extreme */

/*
 *	FIT( &<data hdr>, <base> )
 *
 *	Calls:	numnice(VI), (*Fctcall)()
 *
 *	Method:
 *
 *	The subroutine tries to adjust the coefficient structure so
 *	that the map from Data space to Display space will be `nice'.
 *	The user supplies the address of a coefficient structure,
 *	which contains the constants and data characterizing function to
 *	map the Data space into Display space.  Once this procedure is done
 *	the final map from Display to Normalized TIG space is done, setting
 *	the span from `left' to `right' equal to unity, and setting the
 *	Normalized TIG zero point to the Display-value(`left').
 *
 *
 *	NOTE:	Here to map into TIG space, one is mapping such that
 *		0.0 = f(left value) and 1.0 = f(right value).
 */

/* Fit the data extremes ``nicely'' to coefficient struct */
void fit(struct darray *thedatap, double base)
{
	register struct coef *coefp;
	double left, right, diff;

	coefp = &thedatap->dcoef;

	right = (*coefp->Fctcall)(thedatap->right,DAT2DIS,coefp,(char *)NULL)->Retdbl;
	left = (*coefp->Fctcall)(thedatap->left,DAT2DIS,coefp,(char *)NULL)->Retdbl;

	right = (*coefp->Fctcall)( right, DIS2dis, coefp, (char *)NULL )->Retdbl;
	left = (*coefp->Fctcall)( left, DIS2dis, coefp, (char *)NULL )->Retdbl;

	diff = (right - left) * ALLOWNC;

	left = (*coefp->Fctcall)( numnice(left-diff,left,base), dis2DIS, coefp, (char *)NULL )->Retdbl;
	right = (*coefp->Fctcall)( numnice(right,right+diff,base), dis2DIS, coefp, (char *)NULL )->Retdbl;

	thedatap->right = (*coefp->Fctcall)( right, DIS2DAT, coefp, (char *)NULL )->Retdbl;
	thedatap->left = (*coefp->Fctcall)( left, DIS2DAT, coefp, (char *)NULL )->Retdbl;

	/* Set coefficients */

	coefp->_CA = right - left;	/* Map into a span of 1 */
	coefp->_CB = left;	/* Map to start at 0 */
}

/*
 *	SETCOEF( &<data hdr>, <display offset> )
 *
 *	Calls:	(*Fctcall)()
 *
 *	Method:
 *
 *	This routine sets the coefficient structure values _CA and _CB
 *	so that the difference between NTIG(`left') and NTIG(`right')
 *	is unity.  The NTIG intercept is set to <doffset>.
 */
void setcoef(struct darray *thedatap, double doffset )	/* Set coefficient structure acc. to left/right */
{
	register struct coef *coefp;
	double left, right;

	coefp = &thedatap->dcoef;
	right = (*coefp->Fctcall)( thedatap->right, DAT2DIS, coefp, (char *)NULL )->Retdbl;
	left = (*coefp->Fctcall)( thedatap->left, DAT2DIS, coefp, (char *)NULL )->Retdbl;
	coefp->_CA = right - left;	/* Map into a span of 1 */
	coefp->_CB = doffset;		/* Map so `doffset' is at 0 */
}

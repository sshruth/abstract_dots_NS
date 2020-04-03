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
 *	02/05/89  BJZ	ported to Sparc
 */
#include	<stdio.h>
#include	<math.h>
#include	"tig.h"

/*
 *	MINMAX( &<data array header> )
 */

minmax( thedatap )	/* Find and fill-in extrema values */
register struct darray *thedatap;
{
	register int mode, i;
	double x;

	if(thedatap->mode > MAXMODE)
		fixmode( &thedatap->mode );	/* If mode is not in index form; make it */
	mode = thedatap->mode;

	/* Find the maximum and minimum data values */
	/* The `left' is the smallest, and the `right' is the largest */

	thedatap->right = -HUGE;	/* Smallest possible number */
	thedatap->left = HUGE;		/* Largest possible number */

	for( i=0; i < thedatap->nelems; i++ )  {
		switch(mode) {
		case SHORT:		/* Short Integer */
			x = thedatap->datap.shortp[i];
			break;
		case LONG:		/* Long */
			x = thedatap->datap.lngp[i];
			break;
		case FLOAT:		/* Float */
			x = thedatap->datap.fltp[i];
			break;
		case DOUBLE:		/* Double */
			x = thedatap->datap.dblp[i];
		}

		if( x > thedatap->right )
			thedatap->right = x;
		if( x < thedatap->left )
			thedatap->left = x;
	}
}

/*
 *	USER LIBRARY SUBROUTINE		NUMNICE
 *
 *	By:  George E. Toth,	April 6, 1980
 *	     (C) - Copyright, 1980   All rights reserved.
 *
 *	Oct. 23, '81	LMO	Fixed boundary checks
 *	Feb. 10, '89	BJZ	Fixed floating point errors
 */
#include	<stdio.h>
#include	<math.h>

#define	NDIV	7	/* Maximum number of divisional boundaries to a base:  aesthetics */
#define EPSILON  1E-15	/* Small positive number for roundoff errors	*/
/*
 *	NUMNICE( <low limit>, <hi limit>, <output base> )
 *
 *	Calls:	log(3M), modf(3), pow(3M)
 *
 *	Returns:	<nice number>	always
 *
 *	Method:
 *
 *	First we take the difference between the low and hi numbers.
 *	We take its logarithm in the particular base of interest; this
 *	represents the order of magnitude in which our interest lies.
 *	We then modulo the lo number to this magnitude plus one (to in-
 *	clude the magnitude).  This leaves the fractional and integer
 *	parts.  The integer part will be the final number plus a adjusted
 *	fractional part.
 *	Then we find the ``nicest'' number between the lo.fraction and
 *	(lo.fraction+diff.fraction).  If the numbers span into the following
 *	order of magnitude, obviously the ``nicest'' number is the boundary.
 *	Otherwise a number is nicest if has a boundary within the limits
 *	on integral product boundaries.  If this is not true, we try div-
 *	isional boundaries.  If that finally doesn't work, we just return
 *	xlo.
 */

double numnice( xlo, xhi, base )	/* Return a ``nice'' number */
double xlo, xhi;
double base;
{
	register int i, j, ndiv;
	int sign ={0};
	double ii, sum, diff;
	double lnbase, lndiff;
	double integer, fraction;	/* Integer and fractional parts */
	double order;			/* order of magnitude shift */

	if( xlo <= EPSILON && xhi >= 0.0 )
		return( 0.0 );		/* It's simple */

	if( base <= 2.0 )
		return( xlo );	/* How to handle bases less than 2? */

	if( xlo > xhi ) {	/* Flip since we want xhi to be higher */
		ii = xlo;
		xlo = xhi;
		xhi = ii;
	}

	if( xlo < 0.0 ) {		/* Negative numbers are treated as positives */
		ii = xlo;
		xlo = -xhi;
		xhi = -ii;
		sign++;
	}

	/* Now that xhi > xlo find difference */

	lnbase = log( base );

	if( (diff = xhi - xlo) == 0 )
		return( xlo );
	lndiff = log( diff ) / lnbase;

	if( modf(lndiff,&order) >= 0.0 )
		order += 1;

	/* Raise xlo and diff, by (-order) so we can modulo results */

	diff *= (order=pow( base, -order ));
	xlo *= order;

	/* Now modulo off top into integer part, and bottom into fractional */

	fraction = modf( xlo, &integer );

	/* Now find nicest number from integer to integer+diffint */

	sum = fraction + diff;
	if( sum > 1.0 ) {	/* Fractionals added to new power */
		integer += 1;
		integer /= order;
		return( sign? -integer : integer );
	}

	/* Job is more difficult */

	/* A number is ``nicest'' if it is an integral multiple of the base */

	if( base > NDIV )
		ndiv = NDIV;
	else	ndiv = base;

	for( i=2; i < ndiv; i++ ) {
		double kk;

		j = base/i;
		if( i*j != base )	/* Not an integral multiple */
			continue;

		for(kk=0; kk < i; kk += 1) {
			double jj;

			jj = kk/i;	/* k * (base/i) / base ::: rt. of decpt */
			if( jj >= fraction && jj <= sum ) {
				integer += jj;
				integer /= order;
				return( sign? -integer : integer );
			}
		}
	}

	/* If can't find integral number, find first largest multiple less than base */

	for( i=ndiv; i > 0; i-- ) {
		double kk;

		for( kk=0; kk < i; kk += 1) { 
			double jj;

			jj = kk/i;
			if( jj >= fraction && jj <= sum ) {
				integer += jj;
				integer /= order;
				return( sign? -integer : integer );
			}
		}
	}

	/* If still couldn't find one, just return `xlo' */

	return( xlo );
}

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
 *
 */
#include <stdio.h>
#include <math.h>
#include "tig.h"

/*
 *	POL2CART( <radius>, <theta>, <phi> )
 *
 *	Returns:	&<TIGdvec>
 */

struct TIGdvec *pol2cart( radius, theta, phi )
double radius, theta, phi;
{
	static struct TIGdvec retvec;
	double sinp;

	sinp = sin( phi );

	retvec.ZZpos = cos( phi );
	retvec.YYpos = sinp * sin( theta );
	retvec.XXpos = sinp * cos( theta );

	return( &retvec );
}

/*
 *	CART2POL( <X>, <Y>, <Z> )
 *
 *	Returns:	&<TIGpvec>
 */

struct TIGpvec *cart2pol(double x, double y, double z)	/* Convert Cartesian to Polar */
{
	double ii, jj;
	static struct TIGpvec retvec;

	jj = (x * x) + (y * y);

	ii = sqrt(jj);
	
	/* Do it this way to avoid problems with infinity in atan() */
	if(ii) retvec.Theta = acos(x / ii);
	else retvec.Theta = 0;

	/* For Theta angles from PI --> 2PI */
	if(y < 0) retvec.Theta = twopi - retvec.Theta;

	if((retvec.Radius = sqrt( jj + (z * z)))  != 0) retvec.Phi = acos(z/retvec.Radius);
	else retvec.Phi = 0;

	return( &retvec );
}

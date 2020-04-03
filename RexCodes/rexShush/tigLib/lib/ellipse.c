#

/*
 *	T E R M I N A L   I N D E P E N D E N T   G R A P H I C S
 *
 *			by:  George E. Toth
 *			  May 15, 1980
 *
 *	These routines produce the Terminal Independent Graphics
 *	Intermediate language designed by Michael J. Muuss.
 *
 *		R E V I S I O N  H I S T O R Y
 *	===================================================
 *	05/15/80  GET	Wrote subroutine.
 *
 */
#include	<stdio.h>
#include	<math.h>
#include	"tig.h"

/* Memory for the Ellipse functions */

double _E_maj, _E_min;	/* Major, minor axis length */
struct TIGvec _E_ctr;	/* Center of ellipse */
struct TIGxform _E_xform;	/* Transformation matrix for ellipse plane */

/*
 *	SETELLIP( &<X vec>, &<Y vec>, &<center>, <maj>, <min> )
 *
 *	Calls: vecnorm(6T), xfrmxpos(6T)
 */

setellip( xvecp, yvecp, cvecp, maj, min )	/* Set ellipse plane, and major-minor axes */
register struct TIGvec *xvecp, *yvecp, *cvecp;
register int maj;
int min;
{
	double ii;

	_E_xform.newX = *vecnorm( xvecp );
	_E_xform.newY = *vecnorm( yvecp );

	/* Make sure Y is orthogonal to X */

	ii =	_E_xform.newX.XXpos * _E_xform.newY.XXpos	+
		_E_xform.newX.YYpos * _E_xform.newY.YYpos	+
		_E_xform.newX.ZZpos * _E_xform.newY.ZZpos;
	_E_xform.newY.XXpos -= ii * _E_xform.newX.XXpos;
	_E_xform.newY.YYpos -= ii * _E_xform.newX.YYpos;
	_E_xform.newY.ZZpos -= ii * _E_xform.newX.ZZpos;

	_E_xform.newZ.XXpos = _E_xform.newZ.YYpos = _E_xform.newZ.ZZpos = 0;

	_E_xform = *xfrmxpos( &_E_xform );

	if( maj > min ) {
		_E_maj = 1;
		_E_min = (double)min/maj;
	}
	else {
		_E_min = 1;
		_E_maj = (double)maj/min;
	}
	_E_ctr = *cvecp;
}

/*
 *	WELLIP( <angle> )
 *
 *	Calls:	modf(3)
 *
 *	Returns:	<value>
 *
 *	The return value is a number from 0 to 1.
 */

double wellip( angle )	/* Tell where on the ellipse the person is */
register double angle;
{
	double temp;

	if( (angle = modf(angle/360.0, &temp)*360.0) < 0 )
		angle = 360 - angle;
	return( angle/360 );
}

/*
 *	_FELLIP( <position>, <scale> )
 *
 *	Calls:	cos(3M), sin(3M), TIGdot(6T)
 *
 *	Returns:	&<TIGvector>
 */

struct TIGvec *_Fellip( pos, scale )	/* Return position on ellipse */
double pos;
register int scale;
{
	static struct TIGvec retvec;

	retvec.Xpos = _E_maj * cos( twopi*pos ) * scale + 0.5;
	retvec.Ypos = _E_min * sin( twopi*pos ) * scale + 0.5;
	retvec.Zpos = 0;
	retvec = *TIGdot( &retvec, &_E_xform );	/* Map */
	retvec.Xpos += _E_ctr.Xpos;
	retvec.Ypos += _E_ctr.Ypos;
	retvec.Zpos += _E_ctr.Zpos;

	return( &retvec );
}

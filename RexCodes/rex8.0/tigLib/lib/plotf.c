
/*
 *	T E R M I N A L   I N D E P E N D E N T   G R A P H I C S
 *
 *			by:  George E. Toth
 *			  May 15, 1980
 *
 *	These routines produce the Terminal Independent Graphics
 *	Intermediate language designed by Michael J. Muuss.
 *
 *	(C) Copyright, 1980		All Rights Reserved
 *
 *		R E V I S I O N  H I S T O R Y
 *	===================================================
 *	05/15/80  GET	Wrote subroutine.
 *
 */
#include <stdio.h>
#include <math.h>
#include "tig.h"

double c1 = { 0.70710678118654752440084 };	/* SQRT(2)/2 */
double c2 = { 0.29289321881345247559916 };	/* 1 - SQRT(2)/2 */

#define	SINCR	4096	/* 2 ^ 10; starting increment (arbitrary) */

#define	abs(a)	((a<0)? -(a) : (a))
#define	VECDIF(x,y)	(abs(x.Xpos-y.Xpos)+abs(x.Ypos-y.Ypos)+abs(x.Zpos-y.Zpos))

/*
 *	PLOTF( <start position>, <stop position>, <scale fact>, &<func> )
 *
 *	Calls:	move(6T)
 *
 *	Method:
 *
 *	This routine will plot an arbitrary function on the plotter.
 *	The function must be parametrized in such a way that as its
 *	argument varies from 0 to 1, it returns single values throughout
 *	its maximal range.
 *	The function must return a pointer to a TIG vector.
 *
 *	This subroutine is NOT GUARANTEED OF PLOTTING ALL FUNCTIONS!!!
 *	It does a modest job of well behaved global behaviour functions.
 *	It will fall apart on highly localized disordered functions,
 *	for example a real delta function is unlikely to work, but
 *	Bessel functions will.  All in all it is the next best thing
 *	to doing it without doing it.
 */

plotf( start, stop, scale, pfct )	/* Plot the function `pfct' */
double start, stop;
register int scale;
register struct TIGvec *(*pfct)();
{
	register int flag = {0};
	double incr;
	struct TIGvec vecref, vecnew, vectry, vecdif, vecj;

	if( start > stop ) {	/* accross 0-1 boundary, i.e. opposite dir */
		plotf( start, 1.0, scale, pfct );
		plotf( 0.0, stop, scale, pfct );
		return;
	}

	incr = (stop - start) / SINCR;
	vecref = *(*pfct)(start,scale);	/* Initialize reference */
	move2pt( vecref.Xpos, vecref.Ypos, vecref.Zpos );	/* Move to it */

	while( start < stop ) {
		vecnew = *(*pfct)( start+incr, scale );
		vecj = vecnew;
		vecdif = vecnew;
		vecdif.Xpos -= vecref.Xpos;
		vecdif.Ypos -= vecref.Ypos;
		vecdif.Zpos -= vecref.Zpos;

		/* See if it can be made larger */

		if( !flag && start+incr+incr <= stop ) {
			vectry = *(*pfct)( start+incr+incr, scale );	/* Real value */
			vecj.Xpos += vecdif.Xpos;
			vecj.Ypos += vecdif.Ypos;
			vecj.Zpos += vecdif.Zpos;
			if( VECDIF( vectry, vecj ) <= 1 ) {	/* It worked... */
				incr *= 2;			/* so make it bigger */
				continue;
			}
		}

		/*
		 * Check curve in between points.  This is where it fails
		 * to detect things like delta functions.  It checks only
		 * three points, which for most functions will be good
		 * enough since they are in rather strange places:
		 * 	1-sqrt(2)/2,	0.5,	sqrt(2)/2
		 * Why there?  Why not?
		 *
		 * Flag is set once the LARGER test is passed so if it
		 * fails any of the smaller ones, it doesn't get enlarged
		 * again.
		 */


		flag = 1;

		vectry = *(*pfct)( start+incr/2, scale );
		vecj = vecref;
		vecj.Xpos += vecdif.Xpos>>1;
		vecj.Ypos += vecdif.Ypos>>1;
		vecj.Zpos += vecdif.Zpos>>1;
		if( VECDIF( vectry, vecj ) > 1 ) {
			incr /= 2;
			continue;
		}

		vectry = *(*pfct)( start+incr*c1, scale );
		vecj = vecref;
		vecj.Xpos += vecdif.Xpos*c1;
		vecj.Ypos += vecdif.Ypos*c1;
		vecj.Zpos += vecdif.Zpos*c1;
		if( VECDIF( vectry, vecj ) > 1 ) {
			incr /= 2;
			continue;
		}

		vectry = *(*pfct)( start+incr*c2, scale );
		vecj = vecref;
		vecj.Xpos += vecdif.Xpos*c2;
		vecj.Ypos += vecdif.Ypos*c2;
		vecj.Zpos += vecdif.Zpos*c2;
		if( VECDIF( vectry, vecj ) > 1 ) {
			incr /= 2;
			continue;
		}

		if( vecdif.Xpos | vecdif.Ypos | vecdif.Zpos )	/* If motion resulted */
			move( vecdif.Xpos, vecdif.Ypos, vecdif.Zpos );

		start += incr;		/* Went through an interation */
		vecref = vecnew;	/* Move forward */
		flag = 0;		/* Reset */
	}

	vecref = *(*pfct)(stop,scale);	/* Goto to stop point */
	move2pt( vecref.Xpos, vecref.Ypos, vecref.Zpos );
}

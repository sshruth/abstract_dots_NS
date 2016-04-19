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
#include	"tig.h"

#define	CSIZE	10	/* Marker height in 1/100ths in. */

/*
 *	MARKER( <character> )
 *
 *	Calls:	move(6T), move2pt(6T), newfont(6T), newsize(6T), putsymb(6T)
 */

marker(c)	/* Place a marker at the current location */
register int c;
{
	register int ofont, osize;
	char oxskew, oyskew;
	struct TIGvec pos;

	ofont = ppfont;
	osize = ppfsize;
	oxskew = ppxskew;
	oyskew = ppyskew;
	pos = ppmap;

	newfont( 0 );		/* Standard font set hold markers */
	newsize( CSIZE );
	newskew( 0, 0 );	/* No skewing */
	penup();		/* Make sure it is up first */
	move(	(int)(-ppwrpln.newX.YYpos * CSIZE * 5),
		(int)(-ppwrpln.newY.YYpos * CSIZE * 5),
		(int)(-ppwrpln.newZ.YYpos * CSIZE * 5)
	    );

	putsymb( c );

	newfont( ofont );		/* Return to old stuff */
	newsize( osize );
	newskew( oxskew, oyskew );
	move2pt( pos.Xpos, pos.Ypos, pos.Zpos );
}

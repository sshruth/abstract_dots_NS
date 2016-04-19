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

/*
 *	LINE( &<space structure>, <marker char>, <interval>, <mode> )
 *
 *	Calls:	move(6T), move2pt(6T), penup(6T), pendown(6T), TIG3dcnv(6T)
 */

line( spacep, mark, interval, mode )	/* Draw 3D line */
register struct space *spacep;
int mark, interval;
int mode;
{
	register int i;
	register struct TIGvec *vecp;
	struct TIGvec origin;
	register int first;

	if( interval <= 0 )
		interval = 1;

	if( (int)(vecp=TIG3dcnv(spacep,i=0)) == -1 )
		return;

	origin = ppmap;		/* Reference point */

	penup();
	move2pt( origin.Xpos + vecp->Xpos,
		origin.Ypos + vecp->Ypos,
		origin.Zpos + vecp->Zpos );

	/* Now put out arrays */

	first = 1;
	do {
		if( !(i++%interval) && (mode != OLINES) ) marker( mark );
		if (mode == OMARKS) penup();
		else if (mode == MRKNLIN) pendown();
		else if (first) {
			first = 0;
			pendown();
		}
		move2pt( origin.Xpos + vecp->Xpos,
			origin.Ypos + vecp->Ypos,
			origin.Zpos + vecp->Zpos );
	} while((int)(vecp=TIG3dcnv(spacep,i)) != -1 );

	/* No matter how the interval was, always put in the last marker */

	if( !((i-1)%interval) && mode != OLINES )	/* Only if it isn't there already */
		marker( mark );
}

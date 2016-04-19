
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
#include	<stdio.h>
#include	"tig.h"

/*
 *	BOX( &<Xvec>, &<Yvec>, &<Zvec>, <mode> )
 */

box( xvecp, yvecp, zvecp, mode )	/* Draw a box */
struct TIGvec *xvecp;
register struct TIGvec *yvecp, *zvecp;
register char mode;
{
	char edges[12];
	int i;
	struct TIGvec origin;

	origin = ppmap;		/* Save reference point */

	for(i=0; i < 12; edges[i++] = 1);	/* Initialize array */

	/* First 4 edges are lower surface, next 4 top surface, last 4 vertical edges */
	/* Remove edges which will not be drawn */

	if( !(mode&LOD) )
		edges[0] = edges[3] = edges[8] = 0;
	if( !(mode&ROD) )
		edges[0] = edges[1] = edges[9] = 0;
	if( !(mode&RID) )
		edges[1] = edges[2] = edges[10] = 0;
	if( !(mode&LID) )
		edges[2] = edges[3] = edges[11] = 0;
	if( !(mode&LOU) )
		edges[4] = edges[7] = edges[8] = 0;
	if( !(mode&ROU) )
		edges[5] = edges[4] = edges[9] = 0;
	if( !(mode&RIU) )
		edges[6] = edges[5] = edges[10] = 0;
	if( !(mode&LIU) )
		edges[7] = edges[6] = edges[11] = 0;

	/* Walk around bottom surface, then top putting in verticals */

	if( !edges[0] )
		penup();
	else	pendown();
	move( xvecp->Xpos, xvecp->Ypos, xvecp->Zpos );

	if( !edges[1] )
		penup();
	else	pendown();
	move( yvecp->Xpos, yvecp->Ypos, yvecp->Zpos );

	if( !edges[2] )
		penup();
	else	pendown();
	move( -xvecp->Xpos, -xvecp->Ypos, -xvecp->Zpos );

	if( !edges[3] )
		penup();
	else	pendown();
	move( -yvecp->Xpos, -yvecp->Ypos, -yvecp->Zpos );

	if( !edges[8] )		/* Walk up to top surface */
		penup();
	else	pendown();
	move( zvecp->Xpos, zvecp->Ypos, zvecp->Zpos );

	if( !edges[4] )
		penup();
	else	pendown();
	move( xvecp->Xpos, xvecp->Ypos, xvecp->Zpos );

	if( !edges[9] )		/* Put in vertical if needed */
		penup();
	else {
		pendown();
		move( -zvecp->Xpos, -zvecp->Ypos, -zvecp->Zpos );
		penup();
		move( zvecp->Xpos, zvecp->Ypos, zvecp->Zpos );
	}

	if( !edges[5] )
		penup();
	else	pendown();
	move( yvecp->Xpos, yvecp->Ypos, yvecp->Zpos );

	if( !edges[10] )	/* Put in vertical if needed */
		penup();
	else {
		pendown();
		move( -zvecp->Xpos, -zvecp->Ypos, -zvecp->Zpos );
		penup();
		move( zvecp->Xpos, zvecp->Ypos, zvecp->Zpos );
	}

	if( !edges[6] )
		penup();
	else	pendown();
	move( -xvecp->Xpos, xvecp->Ypos, xvecp->Zpos );

	if( !edges[11] )	/* Put in vertical if needed */
		penup();
	else {
		pendown();
		move( -zvecp->Xpos, -zvecp->Ypos, -zvecp->Zpos );
		penup();
		move( zvecp->Xpos, zvecp->Ypos, zvecp->Zpos );
	}

	if( !edges[7] )
		penup();
	else	pendown();
	move( -yvecp->Xpos, -yvecp->Ypos, -yvecp->Zpos );

	/* Return before we return */

	penup();
	move2pt( origin.Xpos, origin.Ypos, origin.Zpos );
}

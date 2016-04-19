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
 *
 */
#include	<stdio.h>
#include	"tig.h"

/*
 *	FIXMODE( &<mode> )
 */

fixmode( modep )		/* Change character mode into integer */
register char *modep;
{

	switch(*modep) {
	case 'D':
	case 'd':		/* Double */
		*modep = DOUBLE;
		return;
	case 'F':
	case 'f':
		*modep = FLOAT;
		return;
	case 'L':
	case 'l':
		*modep = LONG;
		return;
	default:
		*modep = SHORT;
	}
}

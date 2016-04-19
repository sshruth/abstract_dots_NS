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
 *	01/28/89  BJZ   Fixed argument count *Fctcall()
 *
 */
#include	<stdio.h>
#include	"tig.h"

/*
 *	TIGCONV( &<axis structure>, <index> )
 *
 *	Calls:	fixmode(6T)
 *
 *	Returns:	<value>	Success
 *			   -1	Error
 */
int TIGconv(struct spaxis *axisp, int i)	/* Convert data to a TIG point */
{
	register struct darray *datp;
	int length;

	if( (datp=axisp->datarp) == 0 || datp->nelems <= i )
		return(-1);	/* No axis, or not enough elements */

	if( datp->mode > MAXMODE )
		fixmode( &datp->mode );

	switch( datp->mode ) {
	case SHORT:
		length = DIS2TIG(
					(*datp->dcoef.Fctcall)(
						(double)datp->datap.shortp[i],
						DAT2DIS, &datp->dcoef, (char *)NULL
					)->Retdbl,
					&datp->dcoef
				     ) * axisp->axscale;
		break;
	case LONG:
		length = DIS2TIG(
					(*datp->dcoef.Fctcall)(
						(double)datp->datap.lngp[i],
						DAT2DIS, &datp->dcoef, (char *)NULL
					)->Retdbl,
					&datp->dcoef
				     ) * axisp->axscale;
		break;
	case FLOAT:
		length = DIS2TIG(
					(*datp->dcoef.Fctcall)(
						(double)datp->datap.fltp[i],
						DAT2DIS, &datp->dcoef, (char *)NULL
					)->Retdbl,
					&datp->dcoef
				     ) * axisp->axscale;
		break;
	case DOUBLE:
		length = DIS2TIG(
					(*datp->dcoef.Fctcall)(
						(double)datp->datap.dblp[i],
						DAT2DIS, &datp->dcoef, (char *)NULL
					)->Retdbl,
					&datp->dcoef
				     ) * axisp->axscale;
	}

	return(length);
}

/*
 *	TIG3DCNV( &<space header>, <data index> )
 *
 *	Returns:	<struct addr>	Success
 *				-1	Out of a data-range index
 */

struct TIGvec *TIG3dcnv( spacep, index )	/* Convert data to TIG vector */
register struct space *spacep;
register int index;
{
	register int error = {0};
	static struct TIGvec TIGret;	/* Returned X, Y, Z */

	if( !spacep->xaxis.datarp + !spacep->yaxis.datarp + !spacep->zaxis.datarp > 1 )
		return((struct TIGvec *) -1);	/* Must have at least two axis to be valid */

	/* Do X */

	if( ( TIGret.Xpos = TIGconv( &spacep->xaxis, index ) ) == -1 ) {
		TIGret.Xpos = 0;	/* Clear it then */
		error++;
	}

	/* Do Y */

	if( ( TIGret.Ypos = TIGconv( &spacep->yaxis, index ) ) == -1 ) {
		TIGret.Ypos = 0;	/* Clear it then */
		error++;
	}

	/* Do Z */

	if( ( TIGret.Zpos = TIGconv( &spacep->zaxis, index ) ) == -1 ) {
		TIGret.Zpos = 0;	/* Clear it then */
		error++;
	}

	if( error > 1 )
		return((struct TIGvec *) -1);	/* Can't have less than two axes */

	return( &TIGret );
}

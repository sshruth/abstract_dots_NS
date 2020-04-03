/*
 *	T E R M I N A L   I N D E P E N D E N T   G R A P H I C S
 *
 *			by:  George E. Toth
 *			  April 29, 1980
 *
 *	These routines produce the Terminal Independent Graphics
 *	Intermediate language designed by Michael J. Muuss.
 *
 *		R E V I S I O N  H I S T O R Y
 *	===================================================
 *	04/29/80  GET	Wrote subroutine.
 *	01/26/89  BJZ	Fixed pointer alignment problems with _Fctexp
 *	05/24/89  LMO	fix number printing so is sensitive to format.
 *			e.g.:  z or a*e^x or e^x
 *	12/12/89  LMO	more work on printing format.  add comments to
 *			_numprep().
 *	7/23/90	  LMO	Fix bug causing misplotting if axis extremes
 *			are not powers of base.
 *	8/15/90	  LMO	Correct intform for fractions
 *	1/29/91	  LMO	Fix to accept format when it is a null string
 */
#include	<math.h>
#include	<stdio.h>
#include	<string.h>	/* system 5 call */
#include	"tig.h"

#define	FACTOR	0	/* Index to `factor' part of return of _numprep() */
#define	EXPON	1	/* Index to `exponent' part */

#define	EPSILON	1E-11	/* Some small error amount which may cause errors */

static double *_numprep();
static int _expsetnum();
static int _exptype();

/*
 *	_FCTEXP( <number>, <mode>, &<coefficient struct>, &<format> )
 *
 *	Calls:	log(3M), pow(3M), pltstr(), _strconv(), _numprep()
 *
 *	Returns:	&<info struct>	Always
 */


struct Fcttype *_Fctexp( num, mode, coefp, formatp )	/* Perform Exponential functions */
double num;
int mode;
register struct coef *coefp;
char *formatp;
{
	double *ddp;
	double args[8];
	char *formp, *intform();
	static struct Fcttype fctret;
	char buf[10];

	switch(mode) {
	case DAT2DIS:		/* Convert Data point to Display point */
		fctret.Retdbl = (num - coefp->_Coef[INTERCEPT]) / coefp->_Coef[ESLOPE];
		fctret.Retdbl = log( fctret.Retdbl ) / log( coefp->_Coef[BASE] );
		fctret.Retdbl /= coefp->_Coef[SLOPE];
		break;

	case DIS2DAT:		/* Convert a Display point to a Data point */
 		fctret.Retdbl = pow( coefp->_Coef[BASE], coefp->_Coef[SLOPE] * num );
		fctret.Retdbl *= coefp->_Coef[ESLOPE];
		fctret.Retdbl += coefp->_Coef[INTERCEPT];
		break;

	case DIS2dis:		/* A display number is not what it seems to be
				 * e.g.:   E^1.5  ---->	E^(-0.5) * E^2
				 *	DIS = 1.5  --->  dis = 1 + E^(-0.5)
				 */
		ddp = _numprep( num );
		fctret.Retdbl = ddp[EXPON] - 1;
		fctret.Retdbl += pow( coefp->_Coef[BASE], ddp[FACTOR] );
		break;

	case dis2DIS:		/* Now do converse:  funny-dis --> real DIS
				 * 2.5   ----> 0.5E * E^2  ---> E^DIS
				 */
		if( (num = modf( num, &fctret.Retdbl )) < 0 ) {
			num = 1 + num;
			fctret.Retdbl -= 1;
		}
		fctret.Retdbl += 1;
		if( num > EPSILON ){
			fctret.Retdbl += log( num ) / log(coefp->_Coef[BASE] );
		}
		else	fctret.Retdbl -= 1;
		break;

	case NUMPREP:		/* Prepare number so that someone else can print it */
		if ((formatp == NULL) || (*formatp == '\0')) {
			fctret.Retinfo.TIGstrp = NULL;
			break;
		}
		ddp = _numprep( num );
		_expsetnum(formatp, num, coefp, ddp);
		switch(_exptype(formatp)) {
		case 0:			/* just number */
			formp = intform(formatp, ddp[FACTOR], coefp->_Coef[BASE], buf);
			fctret.Retinfo.TIGstrp = _strconv(
				formp, ddp,
				&fctret.Retinfo.soffset, &fctret.Retinfo.slength);
			break;
		case 1:			/* exponential form of number */
			fctret.Retinfo.TIGstrp = _strconv(
				formatp, &ddp[EXPON],
				&fctret.Retinfo.soffset, &fctret.Retinfo.slength);
			break;
		case 2:				/* full factor & expon format */
			fctret.Retinfo.TIGstrp = _strconv(
				formatp, ddp,
				&fctret.Retinfo.soffset, &fctret.Retinfo.slength
		   	);
			break;
		default:
			fprintf(stderr, "Bad exponential format:  (%s)!\n", formatp);
			break;			
		}
		break;

	case NUMPRNT:		/* Print the number in the current location */
		if ((formatp == NULL) || (*formatp == '\0')) {
			fctret.Retinfo.TIGstrp = NULL;
			break;
		}
		ddp = _numprep( num );
		_expsetnum(formatp, num, coefp, ddp);
		switch(_exptype(formatp)) {
		case 0:			/* just number */
			formp = intform(formatp, ddp[FACTOR], coefp->_Coef[BASE], buf);
			args[0] = ddp[FACTOR];
			args[1] = (double)0;
			pltstr(formp, args);
			break;
		case 1:			/* exponential form of number */
			args[0] = ddp[EXPON];
			args[1] = (double)0;
			pltstr(formatp, args);	/* assumes ddp[FACTOR] = 1 */
			break;
		case 2:				/* full factor & expon format */
			args[0] = ddp[FACTOR];
			args[1] = ddp[EXPON];
			args[2] = (double)0;
			pltstr(formatp, args);
			break;
		default:
			fprintf(stderr, "Bad exponential format:  (%s)!\n", formatp);
			break;			
		}
		break;
	}
	return( &fctret );
}

/*
 *	_NUMPREP( <number> )
 *
 *	Calls:	modf(3)
 *
 *	Returns:	&<dbl array>	Always
 *
 *	Method:
 *
 *	Take all numbers and separates into integer and fractional
 *	parts, where the fraction is always negative.  E.g.:
 *		 1.5	--->	 2.0, -0.5	(exponent, factor)
 *		-1.5	--->	-1.0, -0.5
 *		 1.0	--->	 1.0,  0.0
 *		-1.0	--->	-1.0,  0.0
 *		10.3	--->	11.0, -0.7
 *	       -10.3	--->   -10.0, -0.3
 */

static double *_numprep( num )	/* Break number into integer exp plus factor */
double num;
{
	double ii;
	static double retdbl[2];

	retdbl[FACTOR] = modf( num, &ii );
	retdbl[EXPON] = ceil( num );		/* Greatest possible integer */
	retdbl[FACTOR] -= retdbl[EXPON] - ii;

	if( 1 + retdbl[FACTOR] <= EPSILON ) {	/* If close to -1 */
		retdbl[FACTOR] = 0;
		retdbl[EXPON] -= 1;
	}

	return( retdbl );
}

/*
 * _EXPTYPE( <formatp> )
 *
 *	this returns:
 *		0 if plain number, with no exponent
 *		1 if only exponent
 *		2 if number and exponent	
 */
static int
_exptype(formatp)
	char *formatp;
{
	int num = 0;
	int exp = 0;

	if (strchr(formatp, 'U') != NULL) exp = 1;
	if ((exp == 0) && (strchr(formatp, 'i') != NULL)) num = 1;
	if (strchr(formatp, 'd') != NULL) num = 1;

	return (num + 2 * exp -1);
}

static int
_expsetnum(formatp, num, coefp, ddp)
	char *formatp;
	double num;
	register struct coef *coefp;
	double *ddp;
{

	switch(_exptype(formatp)) {
	case 0:			/* just number */
		ddp[FACTOR] = pow( coefp->_Coef[BASE], num);
		break;
	case 1:			/* exponential form of number */
		if (ddp[FACTOR] != 0.0)
		    fprintf(stderr, "Warning:  Factor = %f != 0\n", ddp[FACTOR]);
		break;
	case 2:				/* full factor &  expon format */
		ddp[FACTOR] = pow( coefp->_Coef[BASE], ddp[FACTOR] );
		break;
	default:
		fprintf(stderr, "Bad exponential format:  (%s)!\n", formatp);
		break;			
	}
	return(0);
}

char *
intform(formatp, x, base, buf)
char *formatp, *buf;
double x, base;
{
	int k;

	if ((0.0 < x) && (x < 1.0)) {	/* fractional part */
/*		if (base == 10.0) k = 0.5 + log10(1/x); */
		if (base == 10.0) k = -floor(log10(x));
		else k = -floor((log10(x)) / log10(base));

		sprintf(buf, "%%5^.%dd", k);
		return(buf);
	}
	else return(formatp);
}

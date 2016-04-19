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
 *	04/20/80  GET		Wrote subroutine.
 *	11/11/85  LMO		Fixed pointer bugs.  Changed to VAX
 *	01/18/89  BJZ and BJR	Fixed pointer alignment problems on Sparc.
 *				Fixed argument list in (*Fctcall)()
 *	11jul90	LMO		fix spacing of small tics on log plots
 *	24jul90	LMO		set very small negative numbers to zero
 *				when plotting left edge of axis
 *	15aug90	LMO		Fix extra right label
 *
 */

#include	<stdio.h>
#include	<math.h>
#include	<tig.h>

#define TOL	1.0e-8	/* round-off error tolerance */

			/* Distances negative since "-Y" direction; printing always "+X" */
#define	MAJTCK	100	/* Major Tick length */
#define	MINTCK	 50	/* Minor Tick length */
#define	NUMDIS	250	/* Distance of number from line */
#define	TITDIS	650	/* Distance of title from line */
#define	TITHT	 20	/* Height of title */
#define	NUMHT	 11	/* Height of number in 1/100'ths */

#define	MINDIS	50	/* Minimum allowable distance between printed numbers */

	/* Global Variables */

int	_A_mjt	= { MAJTCK };
int	_A_mnt	= { MINTCK };
int	_A_ndis	= { NUMDIS };
int	_A_nht	= { NUMHT };
int	_A_tdis	= { TITDIS };
int	_A_tht	= { TITHT };

static int sign;
static struct TIGvec _Distpos;		/* Reference point for _dist() */
static double _dist();
static void _putnum();
static void _c_move();
	
/*
 *	AXIS( &<title string>, &<number format>, <no. of labels>,
 *		<no. of ticks between labels>, &<axis struct pointer>,
 *		 &<axis direction vector>, <nice-ing base>, <mode> )
 *
 *	Calls:	ceil(3M), fsgn(3M), log(3M), pow(3M), setcoef(6T), veclen(6T)
 */
void axis(char *title, char *numform, int ndiv, int nticks, struct spaxis *axisp,
	  struct TIGdvec *direcp, double base, int mode)
{
	register struct darray *datap1;
	int i, ofont, osize;
	int neg_diff;
	double xleft, xright, diff;	/* Display-LO, Display-HI, Display-Difference */
	double xincr;		/* Display increment step */
	double ntigpos;		/* Normalized TIG space position */
	double order;
	double fraction;
	double bdist, tdist, dist;	/* Labels are printed on the ends first
					 * then filled in.  These represent the
					 * bottom and top limits
					 */
	double xend;			/* Fictitious end; a NICE end */
	double ntiglt, ntigrt;		/* NTIG left and right values */
	double temp;			/* temp for modf() */
	double fsgn();			/* returns -1.0, 1.0, or 0.0 based on sign */
	struct TIGvec origin;		/* Reference point; starting pos */
	struct TIGvec torigin;		/* Reference for title */
	struct Fctinfo *strinfp;
	extern struct Fcttype *_Fctexp();
	double z;

	if( base < 2 ) {
		fprintf(stderr, "Can't have bases of that order.");
		return;		/* Can't have bases of that order */
	}	
	datap1 = axisp->datarp;
#ifdef DEBUG
fprintf(stderr,"datap->left = %f\n", datap1->left);
fprintf(stderr,"datap->right = %f\n", datap1->right);
#endif
	xleft = (*datap1->dcoef.Fctcall)(datap1->left, DAT2DIS, &datap1->dcoef, (char *)NULL)->Retdbl;
	xright = (*datap1->dcoef.Fctcall)(datap1->right, DAT2DIS, &datap1->dcoef, (char *)NULL)->Retdbl;

	origin = ppmap;		/* Save as reference point */

	/* If split accross NTIG zero point, break into two pieces */
	ntigrt = DIS2TIG( xright, &datap1->dcoef );
	ntiglt = DIS2TIG( xleft, &datap1->dcoef ); 

#ifdef DEBUG
fprintf(stderr, "ntiglt = %f, ntigrt = %f\n", ntiglt, ntigrt);
#endif
	if ((-TOL < ntiglt) && (ntiglt < TOL)) ntiglt = 0;
	if ((-TOL < ntigrt) && (ntigrt < TOL)) ntigrt = 0;

#ifdef DEBUG
fprintf(stderr, "ntiglt = %f, ntigrt = %f\n", ntiglt, ntigrt);
#endif
	if((ntiglt * ntigrt) < -TOL) {  
		int wmode, wndiv;
		struct spaxis waxis;
		struct darray wdata;
#ifdef DEBUG
fprintf(stderr, "do axis in 2 halves\n");
		/* Do RIGHT side first */
fprintf(stderr, "right side\n");
#endif
		waxis.datarp = &wdata;
		waxis.axscale = axisp->axscale * ntigrt;
		wdata = *axisp->datarp;		/* Load waxis and wdata */
		wdata.left = (*wdata.dcoef.Fctcall)( 0.0, DIS2DAT, &wdata.dcoef, (char *)NULL)->Retdbl;
		setcoef( &wdata, 0.0 );		/* map 0->1 F/ left->right */
		wmode = mode & (GG|NORIGHT);
		if( mode&NOZERO )
			wmode |= NOLEFT;
		if( (wndiv=ndiv*ntigrt) <= 0 )
			wndiv = 1;

		axis( title, numform, wndiv, nticks, &waxis, direcp, base, wmode );

		/* Now do LEFT side */
#ifdef DEBUG
fprintf(stderr, "left side\n");
#endif
		waxis.axscale = axisp->axscale * (-ntiglt);
		wdata.right = (*wdata.dcoef.Fctcall)( 0.0, DIS2DAT, &wdata.dcoef, (char *)NULL)->Retdbl;
		wdata.left = axisp->datarp->left;
		setcoef( &wdata, 0.0 );		/* map 0->1 F/ right->left */
		wmode = mode & (GG|NOLEFT);
		wmode |= NORIGHT;	/* Always since RIGHT would have done zero point */
		if( (wndiv=ndiv-wndiv) <= 0 )
			wndiv = 1;

		/* First move back to reference point */

		move2pt( origin.Xpos, origin.Ypos, origin.Zpos );

		axis((char*)NULL, numform, wndiv, nticks, &waxis, direcp, base, wmode );

		return;
	}

	if( mode&GG ) sign = 1;
	else sign = -1;		/* Normal is below, GG is above */

#ifdef DEBUG
fprintf(stderr, "normal xleft = %f, xright = %f\n", xleft, xright);
#endif
	/* We want to find out about the differences, etc. in funny Display space */
	xleft = (*datap1->dcoef.Fctcall)( xleft, DIS2dis, &datap1->dcoef, (char *)NULL)->Retdbl;
	xright = (*datap1->dcoef.Fctcall)( xright, DIS2dis, &datap1->dcoef, (char *)NULL)->Retdbl;

#ifdef DEBUG
fprintf(stderr, "funny xleft = %f, xright = %f\n", xleft, xright);
#endif
	diff = xright - xleft;
	if (diff < 0) {
		neg_diff = 1;
		diff = -diff;
	}
	else neg_diff = 0;
#ifdef DEBUG
fprintf(stderr, "total diff = %f\n", diff);
#endif
	if( ndiv > 0 )
		diff /= (ndiv+1);	/* Real difference is diff/(# of divisions) */
#ifdef DEBUG
fprintf(stderr, "partial diff = %f over %d divisions\n", diff, ndiv+1);
#endif
	/*
	 * Now that the low and high values of `X' are found, find the
	 * nicest division of the axis into at most ndiv parts
	 * such that the numbers have some relationship to the base, i.e.
	 * some integral power.
	 */

	/* Get the integer part of the possible X increment */
	if( (fraction=modf( log(diff)/log(base), &order )) < 0 ) {
#ifdef DEBUG
fprintf(stderr, "base = %f, order = %f, fraction = %f\n", base, order, fraction);
#endif
		order -= 1;
		fraction += 1;
	}
#ifdef DEBUG
fprintf(stderr, "base = %f, order = %f, fraction = %f\n", base, order, fraction);
#endif

	xincr = ceil( pow( base, fraction ) );
#ifdef DEBUG
fprintf(stderr, "xincr = ceil() = %f\n", xincr);
#endif
	/* Find first multiple of increment into base */
	for( ; xincr < base; xincr += 1)
		if( modf(base/xincr,&temp) == 0 ) break;
#ifdef DEBUG
fprintf(stderr, "xincr++ = %f\n", xincr);
#endif

	/* Now make the `nice' increment a real increment */

	xincr *= pow( base, order );
#ifdef DEBUG
fprintf(stderr, "xincr *= %f\n", xincr);
#endif
	if (neg_diff) xincr = -xincr;
#ifdef DEBUG
fprintf(stderr, "xincr neg = %f\n", xincr);
#endif

	/* THE NTIG ZERO POINT IS ALWAYS THE REFERENCE POINT */
	ofont = ppfont;
	osize = ppfsize;

	/* Now draw axis and labels */
	/* Put out left and right numbers, and draw axis as one vector */
	/* Do this way so axis is nice and solid */

	/* Move to starting point first, at left end of axis */
#ifdef DEBUG
fprintf(stderr, "left label = %f\n", xleft);
#endif
	z = ntiglt * axisp->axscale;
	move(	(int)(direcp->XXpos * z),
		(int)(direcp->YYpos * z),
		(int)(direcp->ZZpos * z) );

	torigin = ppmap;	/* This is the reference for the title */
	newfont( 0 );		/* Select Standard font */

	newsize( _A_nht );
	strinfp = & (*datap1->dcoef.Fctcall)(
			(*datap1->dcoef.Fctcall)(
				xleft,
				dis2DIS,
				&datap1->dcoef,
				(char *)NULL
			  )->Retdbl,
			NUMPREP, &datap1->dcoef, numform
		   )->Retinfo;

	/* Now draw axis' line and compute right extremity */

	_Distpos = ppreal;		/* Find out how much you really move */

	if( !(mode&NOLEFT) && (strinfp->TIGstrp != NULL) ) {
		bdist = _dist( strinfp->soffset + strinfp->slength );
		_putnum( strinfp, mode ); 	/* Print left end */
	}
	else	bdist = 0;

	pendown();
	move(	(int)(direcp->XXpos*axisp->axscale),
		(int)(direcp->YYpos*axisp->axscale),
		(int)(direcp->ZZpos*axisp->axscale) );	/* draw line */

#ifdef DEBUG
fprintf(stderr, "right label = %f\n", xright);
#endif
	newsize( _A_nht );
	strinfp = & (*datap1->dcoef.Fctcall)(
			(*datap1->dcoef.Fctcall)(
				xright,
				dis2DIS,
				&datap1->dcoef,
				(char *)NULL
			  )->Retdbl,
			NUMPREP, &datap1->dcoef, numform
		   )->Retinfo;
	if( !(mode&NORIGHT) )
		tdist = _dist( strinfp->soffset ) ;
	else	tdist = _dist((int)0.0 );

	if( tdist - bdist < MINDIS)
		goto dotitle;		/* Little else can be done */

	if( !(mode&NORIGHT) && (strinfp->TIGstrp != NULL) )
		_putnum( strinfp, mode );  	/* Print high-end number */

	/* Now start from the right end and work down to left end
	 * First compute the `nice' starting and ending numbers
	 */

	xend = xright + (xincr - fmod(xright, xincr));

#ifdef DEBUG
fprintf(stderr, "xend = %f\n", xend);
#endif

	ntigpos = DIS2TIG( (*datap1->dcoef.Fctcall)
		( xend, dis2DIS, &datap1->dcoef, (char *)NULL )->Retdbl,
		&datap1->dcoef );

#ifdef DEBUG
fprintf(stderr, "start of axis big tick loop:\n");
fprintf(stderr, "ntigpos = %f, xend = %f\n", ntigpos, xend);
#endif
#ifdef DEBUG
fprintf(stderr, "loop xincr = %f\n", xincr);
#endif

	for( ; ntigpos > ntiglt; xend -= xincr ) {
		double nextx = 0.0;
		double newbot, newtop;
		nextx = (*datap1->dcoef.Fctcall)
			( xend-xincr, dis2DIS, &datap1->dcoef,
			(char *)NULL)->Retdbl;
		if( nticks > 1 ) {
			int ntks;
			double z;
			double lo, hi, lobase, loexp;
			double dx, dattop;

			hi = dattop = (*datap1->dcoef.Fctcall)(
					(*datap1->dcoef.Fctcall)(
						xend, dis2DIS,
						&datap1->dcoef,
						(char *)NULL
					    )->Retdbl,
					DIS2DAT,
					&datap1->dcoef,
					(char *)NULL
				      )->Retdbl;
			lo = (*datap1->dcoef.Fctcall)( nextx, DIS2DAT, &datap1->dcoef, (char *)NULL)->Retdbl;

			dx = hi - lo;
			if ((*datap1->dcoef.Fctcall) == _Fctexp) {	/* get correct ntick for logarithmic axes */
				z = log(lo)/log(base);	/* get lowest base component, e.g., 50 --> 10 */
				loexp = floor(z);
				lobase = pow((double) base, (double) loexp);
				ntks = 0.5 + dx / lobase;
#ifdef DEBUG
fprintf(stderr, "lo = %f, hi = %f, lobase = %f, ntks = %d\n", lo, hi, lobase, ntks);
fprintf(stderr, "loexp = floor(%f) = %f\n", z, loexp);
#endif
			}
			else {
				ntks = nticks;
			}

			dx /= ntks;
			/* Really only want ticks BETWEEN labels */
			/* That's why it is ntks-1 */
			for( i=0; i++ < ntks-1; ) {
				dist = DIS2TIG( (*datap1->dcoef.Fctcall)(
						  dattop-i*dx,
						  DAT2DIS,
						  &datap1->dcoef,
						  (char *)NULL
						)->Retdbl,
						&datap1->dcoef
					    );
				if( dist > ntigrt || dist < ntiglt )
					continue;	/* Out of range */

				dist *= axisp->axscale;
				move2pt( origin.Xpos + (int)(dist*direcp->XXpos),
					 origin.Ypos + (int)(dist*direcp->YYpos),
					 origin.Zpos + (int)(dist*direcp->ZZpos) );
				pendown();
				move(	(int)(ppwrpln.newX.YYpos * sign * _A_mnt),
					(int)(ppwrpln.newY.YYpos * sign * _A_mnt),
					(int)(ppwrpln.newZ.YYpos * sign * _A_mnt)
				    );
				penup();
			}
		}

		/* Now put in label markers if you can */

		/*
		 * Sometimes the number should be 0 but really is something
		 * small instead; correct the error
		 */
		if( xincr > 1000*fabs(nextx) )
			nextx = 0;

		i = (ntigpos=DIS2TIG( nextx, &datap1->dcoef ))
		 * axisp->axscale;


#ifdef DEBUG
fprintf(stderr, "ntigpos = %f\n", ntigpos);
#endif
		if (ntigpos > ntigrt) continue;
		if( ntigpos < ntiglt )		/* Done! */
			break;
#ifdef DEBUG
fprintf(stderr, "label: nextx = %f, i = %d\n", nextx, i);
#endif
		move2pt( origin.Xpos + (int)(direcp->XXpos * i),
			 origin.Ypos + (int)(direcp->YYpos * i),
			 origin.Zpos + (int)(direcp->ZZpos * i) );
		newsize( _A_nht );
		strinfp = & (*datap1->dcoef.Fctcall)( nextx, NUMPREP,
			&datap1->dcoef, numform)->Retinfo;
		if (strinfp->TIGstrp == NULL) continue;

		newbot = _dist( strinfp->soffset );
		newtop = _dist( strinfp->soffset + strinfp->slength );
		if( tdist-newtop < MINDIS || newbot-bdist < MINDIS)
			strinfp->TIGstrp = 0;		/* Put tick mark, but not number */
		else	tdist = newbot;		/* Replace new upper limit */
		_putnum( strinfp, mode );
	}

dotitle:
	if( title ) {
		struct Fctinfo strinf;
#ifdef DEBUG
fprintf(stderr, "dotitle: title = %s\n", title);
#endif
		newsize( _A_tht );
		strinf.TIGstrp = _strconv(title,0,&strinf.soffset,&strinf.slength);

		i = axisp->axscale / 2;
#ifdef DEBUG
fprintf(stderr, "dotitle: i = %d Xpos = %d Ypos = %d Zpos = %d\n",
	i, torigin.Xpos, torigin.Ypos, torigin.Zpos);
#endif
		move2pt( torigin.Xpos + (int)(i * direcp->XXpos),
			 torigin.Ypos + (int)(i * direcp->YYpos),
			 torigin.Zpos + (int)(i * direcp->ZZpos)
			);
		/*
		 * Calculate string real offset; center line and subtract
		 * offset (add it since it is negative
		 */
		if( strinf.soffset == 0 )	/* If no center-char, center string */
			strinf.soffset = -strinf.slength/2;

#ifdef DEBUG
fprintf(stderr, "dotitle: calling move\n");
#endif
		move((int)(ppwrpln.newX.XXpos * strinf.soffset),
			(int)(ppwrpln.newY.XXpos * strinf.soffset),
			(int)(ppwrpln.newZ.XXpos * strinf.soffset)
		    );

#ifdef DEBUG
fprintf(stderr, "dotitle: calling _c_move\n");
#endif
		_c_move( _A_tdis, mode );

#ifdef DEBUG
fprintf(stderr, "dotitle: calling _pltstr\n");
#endif
		_pltstr(strinf.TIGstrp);
#ifdef DEBUG
fprintf(stderr, "dotitle: done\n");
#endif
	}
}

/*
 *	_PUTNUM( <number format string>, <mode> )
 *
 *	Calls:	move(), move2pt(), newsize(), pendown(), penup(), pltstr()
 */

static void
_putnum( sinfop, mode )	/* Place a number at a tick mark acc. to mode */
register struct Fctinfo *sinfop;
register int mode;
{
	struct TIGvec origin;

	origin = ppmap;		/* Save current position */

	pendown();
	move(	(int)(ppwrpln.newX.YYpos * sign * _A_mjt),
		(int)(ppwrpln.newY.YYpos * sign * _A_mjt),
		(int)(ppwrpln.newZ.YYpos * sign * _A_mjt)
	    );
	penup();
	_c_move( _A_ndis - _A_mjt, mode );
	if( sinfop->TIGstrp ) {
		move( (int)(ppwrpln.newX.XXpos * (sinfop->soffset)),
		    (int)(ppwrpln.newY.XXpos * (sinfop->soffset)),
		    (int)(ppwrpln.newZ.XXpos * (sinfop->soffset))
		   );
/* this call prints axes labels */
		_pltstr(sinfop->TIGstrp); 
	}
	move2pt( origin.Xpos, origin.Ypos, origin.Zpos );
}

/*
 *	_DIST( <distance from current position> )
 *
 *	Calls:	sqrt(IIIM), veclen(6T)
 *
 *	Returns:	<dist from origin in real space>
 */
static double _dist( offset )	/* Return distance from origin */
register int offset;
{
	double distance;
	struct TIGvec jvec;

	jvec = _Distpos;		/* Current position */
	jvec.Xpos -= ppreal.Xpos;
	jvec.Ypos -= ppreal.Ypos;
	jvec.Zpos -= ppreal.Zpos;
	distance = veclen( &jvec );

	jvec.Xpos = ppwrpln.newX.XXpos * offset;
	jvec.Ypos = ppwrpln.newY.XXpos * offset;
	jvec.Zpos = ppwrpln.newZ.XXpos * offset;

	/* Map into real world, take length, and return */
	if(offset < 0) return( distance - veclen( TIGdot( &jvec, &_ppmtor ) ) );
	return( distance + veclen( TIGdot( &jvec, &_ppmtor ) ) );
}

/*
 *	_A_MOVE( <amount>, <mode> )
 *
 *	Calls:	move(6T)
 */

static void
_c_move( amount, mode )	/* Move in the GG or NORMAL mode */
register int amount, mode;
{

	if( !(mode&GG) )
		amount = -amount;
	else	amount -= ppfsize*9;

	move(	(int)(ppwrpln.newX.YYpos * amount),
		(int)(ppwrpln.newY.YYpos * amount),
		(int)(ppwrpln.newZ.YYpos * amount)
	    );
}

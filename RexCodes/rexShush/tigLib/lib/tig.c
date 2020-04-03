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
 *	07/01/86  LMO	Add perspective to move() and move2pt()
 *	12/17/86  rbt	Added the device-exclusive calls using int code
			I_SPECIAL for shading on matrox.
 *	03/29/87  scg	Integrated the device-exclusive calls into
 *			tig.  The I_SPECIAL instruction format is:
 *			byte 1:	tig op code I_SPECIAL 
 *			byte 2: op code indicating what the 
 *				special instruction is.
 *			byte 3: (octal)number of bytes remaining 
 *				in the instruction, so that devices 
 *				not using the instruction can skip ahead.
 *			bytes 4 and after contain the instruction parameters.
 *	02/11/88 lmo	convert to machine independent byte
 *			calls in newshade()
 *	2Mar88	 lmo	fix newshade, so no negative numbers are passed to
 *			putplt()
 *	8Aug89	 brad	rewrote newplane
 *	23Oct89	 brad	extended the I_SPECIAL commands
 *
 */
#include <stdio.h>
#include <math.h>
#include "tig.h"	/* Standard TIG header */

/*
 *	NEWFONT( <font> )
 */
void newfont(int font)		/* Change current font */
{
	if(font >= NSTYLES) {
		fprintf(stderr,"Request for %d font out of range\n",font);
		font = NSTYLES-1;
	}
	if(font < 0) {
		fprintf(stderr,"Negative font selections are not allowed\n");
		font = 0;
	}

	ppfont = font;
}

/*
 *	NEWSIZE( <point size>, <mode> )
 */
void newsize(int psize)		/* Change current point size; size in 1/100-ths! (0.01) */
{

	if(psize < MINPSZ) {
		fprintf(stderr,"newsize():  Size %d less than minimum %d\n",psize,MINPSZ);
		psize = MINPSZ;
	}
	if(psize > MAXPSZ) {
		fprintf(stderr,"newsize():  Size %d greater than maximum %d\n",psize,MAXPSZ);
		psize = MAXPSZ;
	}

	ppfsize = psize;
}

/*
 *	NEWSKEW( <X skew>, <Y skew> )
 */
void newskew(char xskew, char yskew)	/* Change Character skews */
{
	ppxskew = xskew;
	ppyskew = yskew;
}

/*
 *	NEWANGLE( <theta radians>, <alpha radians>, <beta radians> )
 */
void newangle(double alpha, double beta, double theta)
{
	register double del;
	double acos, bcos, tcos;	/* cosines of Alpha, beta, theta */
	double asin, bsin, tsin;	/* sines of Alpha, beta, theta */

	/* Don't do it if you don't have to */
	if( theta == pptheta && alpha == ppalpha && beta == ppbeta )
		return;

	acos = cos( alpha * deg2rad );
	bcos = cos( beta * deg2rad );
	tcos = cos( theta * deg2rad );
	asin = sin( alpha * deg2rad );
	bsin = sin( beta * deg2rad );
	tsin = sin( theta * deg2rad );

	if( (del=acos*acos + asin*asin*bcos*bcos) == 0 ) {
		fprintf(stderr,"ALPHA-BETA angles such that inverse matrix is impossible\n");
		return;
	}

	/* Now load Xform table from Real to Mapped space */
	/* For explanation of equas. look at MAPPING FCTS */

	_pprtom.newX.XXpos =  acos * tcos;
	_pprtom.newX.YYpos =  tsin * bcos;
	_pprtom.newX.ZZpos =  tcos * asin  +  bsin * tsin;
	_pprtom.newY.XXpos = -tsin * acos;
	_pprtom.newY.YYpos =  tcos * bcos;
	_pprtom.newY.ZZpos = -asin * tsin  +  tcos * bsin;
	_pprtom.newZ.XXpos = -bcos * asin;
	_pprtom.newZ.YYpos = -acos * bsin;
	_pprtom.newZ.ZZpos =  acos * bcos;
	set_idtrans(&_pprtom);

	/* Now load Xform table from Mapped to Real space */

	_ppmtor.newX.XXpos =  (acos*tcos - asin*acos*bsin*tsin)/del;
	_ppmtor.newX.YYpos = -(acos*tsin + tcos*bsin*acos*asin)/del;
	_ppmtor.newX.ZZpos = - bcos*asin/del;
	_ppmtor.newY.XXpos =  (bcos*tsin - bcos*asin*bsin*tcos)/del;
	_ppmtor.newY.YYpos =  (tcos*bcos + bsin*tsin*asin*bcos)/del;
	_ppmtor.newY.ZZpos = - acos*bsin/del;
	_ppmtor.newZ.XXpos =  (acos*acos*bsin*tsin + bcos*bcos*asin*tcos)/del;
	_ppmtor.newZ.YYpos =  (acos*acos*tcos*bsin - bcos*bcos*tsin*asin)/del;
	_ppmtor.newZ.ZZpos =   acos*bcos/del;
	set_idtrans(&_ppmtor);

	ppmap = *TIGdot( &ppreal, &_pprtom );

	pptheta = theta;
	ppalpha = alpha;
	ppbeta = beta;
}

/*
 * check to see if this is identity transform and set flag
 */
void set_idtrans(struct TIGxform *pxm)
{
	if (
		(pxm->newX.XXpos !=  1.0) ||
		(pxm->newY.YYpos !=  1.0) ||
		(pxm->newZ.ZZpos !=  1.0)
	) pxm->_idtrans = 0;
	else if (
		(pxm->newX.YYpos != 0.0) ||
		(pxm->newX.ZZpos != 0.0) ||
		(pxm->newY.XXpos != 0.0) ||
		(pxm->newY.ZZpos != 0.0) ||
		(pxm->newZ.XXpos != 0.0) ||
		(pxm->newZ.YYpos != 0.0)
	) pxm->_idtrans = 0;
	else  pxm->_idtrans = 1;
}

/*
 *	NEWPLANE( &<X vector>, &<Y vector> )
 */
void newplane(struct TIGvec *xvecp, struct TIGvec *yvecp)	/* Change writing plane for PLTSTR etc */
{
	register double ii;

	if(xvecp != NULL && yvecp != NULL) {
		/* Both vectors are specified */
		ppwrpln.newX = *vecnorm(xvecp);
		ppwrpln.newY = *vecnorm(yvecp);

	}
	else if(xvecp == NULL && yvecp == NULL) {
		return;  /* Neither vector is specified, just leave */
	}
	else if(xvecp) {
		/* Only X vector is specified */
		ppwrpln.newX = *vecnorm( xvecp );

		/* Take REAL X vector and rotate 90 CCW degrees to make Y vector */
		ppwrpln.newY = *TIGddot( &ppwrpln.newX, &_ppmtor );

		ii = ppwrpln.newY.XXpos;
		ppwrpln.newY.XXpos = -ppwrpln.newY.YYpos;
		ppwrpln.newY.YYpos = ii;

		/* Map back into mapped space */
		ppwrpln.newY = *TIGddot( &ppwrpln.newY, &_pprtom );
	}
	else if(yvecp) {
		/* Only Y vector is specified */
		ppwrpln.newY = *vecnorm( yvecp );

		/* Take REAL Y vector and rotate 90 CW degrees to make X vector */
		ppwrpln.newX = *TIGddot( &ppwrpln.newY, &_ppmtor );

		ii = ppwrpln.newX.XXpos;
		ppwrpln.newX.XXpos = ppwrpln.newX.YYpos;
		ppwrpln.newX.YYpos = -ii;

		/* Map back into mapped space */
		ppwrpln.newX = *TIGddot( &ppwrpln.newX, &_pprtom );
	}

	ppwrpln.newZ.XXpos = ppwrpln.newZ.YYpos = ppwrpln.newZ.ZZpos = 0;
	ppwrpln = *xfrmxpos(&ppwrpln);
}

/*
 *			N E W O R I G I N
 *
 *	This routine is used to move the apparent origin of the plot
 * to an arbitrary location in the viewing field.  Initially, the
 * apparent origin and the actual origin are identical, but the
 * apparent origin may be moved, usually to leave a margin on the
 * plot.
 *	It is important to note that the co-ordinates passed to this
 * routine represent absolute new origins, and are not mapped by the
 * origin previously in effect.  Thus, if the current origin was
 * located at 1000,1000 and it was desired to move it to 3000,3000
 * then the call would be neworigin(3000,3000) and NOT neworigin(2000,2000).
 */
void neworigin(int x, int y)
{
	ppxoffset = x;
	ppyoffset = y;
}


/*
 *		MOVE / MOVE2PT		RMOVE / RMOVE2PT
 *
 *	Cause the TIG interface routine to move the virtual pen
 * to a new location.  For MOVE2PT the location must be expressed in TIG
 * Universal Position Address (UPA) format, which is an
 * unsigned integer containing an absolute position in thousandths
 * of inches.  If the virtual pen is down, this will cause
 * visual output, otherwise the virtual pen is moved without
 * causing any output.
 *
 *	For MOVE the motion is relative to the current position.
 * Perspective for 3-D projections is controlled by the global _Z_persp,
 * an integer giving the distance of the observer along the real z-axis.
 *
 *	The routines RMOVE and RMOVE2PT are identical to MOVE and
 * MOVE2PT except that these routines move to absolute positions
 * and do not map the positions by PPTHETA.
 *
 * The format of the message sent for this command is:
 *	<I_MOVE> <low X> <high X> <low Y> <high Y>.
 *	Where the X and Y integers are absolute positions relative to
 *	the current offsets
 */
/* Move without angle mapping */
void rmove2pt(int x, int y, int z)	/* Move to positions x, y, z relative to current offsets */
{
	short int args[5];
	short int numArgs;
	short int ix, iy;

	ppreal.Xpos = x;
	ppreal.Ypos = y;
	ppreal.Zpos = z;

	ppmap = *TIGdot( &ppreal, &_pprtom );

	ix = ppxoffset + x;
	iy = ppyoffset + y;

	/* load argument buffer */
	numArgs = 5;
	args[0] = I_MOVE;
	args[1] = (short int)LOBYTE(ix);
	args[2] = (short int) HIBYTE(ix);
	args[3] = (short int)LOBYTE(iy);
	args[4] = (short int) HIBYTE(iy);

	/* now call putplt with the argument count and pointer to the argument buffer */
	putplt(numArgs, args);
}

void rmove(int dx, int dy, int dz)	/* Move from current position by dx, dy, and dz */
{
	short int args[5];
	short int numArgs;
	short int ix, iy;

	ppreal.Xpos += dx;
	ppreal.Ypos += dy;
	ppreal.Zpos += dz;

	ppmap = *TIGdot( &ppreal, &_pprtom );

	ix = ppreal.Xpos + ppxoffset;
	iy = ppreal.Ypos + ppyoffset;

	/* load argument buffer */
	numArgs = 5;
	args[0] = I_MOVE;
	args[1] = (short int)LOBYTE(ix);
	args[2] = (short int) HIBYTE(ix);
	args[3] = (short int)LOBYTE(iy);
	args[4] = (short int) HIBYTE(iy);

	/* now call putplt with the argument count and pointer to the argument buffer */
	putplt(numArgs, args);
}

	/* Move but with angle mapping */
void move2pt(int x, int y, int z)	/* Move to positions x, y, z relative to current offsets */
{
	short int args[5];
	short int numArgs;
	short int ix, iy;
	
	ppmap.Xpos = x;
	ppmap.Ypos = y;
	ppmap.Zpos = z;

	ppreal = *TIGdot( &ppmap, &_ppmtor );

	if(_Z_persp != 0) {
		double fac;
		fac = 1 + (ppreal.Zpos/_Z_persp); 
		ix = ppxoffset + (ppreal.Xpos / fac);
		iy = ppyoffset + (ppreal.Ypos / fac);
	}
	else {
		ix = ppxoffset + ppreal.Xpos;
		iy = ppyoffset + ppreal.Ypos;
	}

	/* load argument buffer */
	numArgs = 5;
	args[0] = I_MOVE;
	args[1] = (short int)LOBYTE(ix);
	args[2] = (short int) HIBYTE(ix);
	args[3] = (short int)LOBYTE(iy);
	args[4] = (short int) HIBYTE(iy);

	/* now call putplt with the argument count and pointer to the argument buffer */
	putplt(numArgs, args);
}

void move(int dx, int dy, int dz)	/* Move from current position by dx, dy, and dz */
{
	short int args[5];
	short int numArgs;
	short int ix, iy;		/* Temporary working spaces */

	ppmap.Xpos += dx;
	ppmap.Ypos += dy;
	ppmap.Zpos += dz;

	ppreal = *TIGdot( &ppmap, &_ppmtor );

	if (_Z_persp != 0) {
		double fac;
		fac = 1 + (ppreal.Zpos/_Z_persp); 
		ix = ppxoffset + (short int)(ppreal.Xpos / fac);
		iy = ppyoffset + (short int)(ppreal.Ypos / fac);
	}
	else {
		ix = ppxoffset + ppreal.Xpos;
		iy = ppyoffset + ppreal.Ypos;
	}

	/* load argument buffer */
	numArgs = 5;
	args[0] = I_MOVE;
	args[1] = (short int)LOBYTE(ix);
	args[2] = (short int) HIBYTE(ix);
	args[3] = (short int)LOBYTE(iy);
	args[4] = (short int) HIBYTE(iy);

	/* now call putplt with the argument count and pointer to the argument buffer */
	putplt(numArgs, args);
}

/*
 *	MAPPING FUNCTIONS:	TIGVEC / TIGDVEC types
 *
 *	These functions map points from TIG space into the mapped space and
 *	from the mapped space into TIG space, using the pp?last and ppm?lst
 *	global variables.
 *
 *	Mapping is done according the to following understanding:
 *
 *	The coordinate system is NOT a spherical coordinate system.
 *	Given a point (x,y,z), place it on the surface of the sphere,
 *	then draw a plane through the center of the sphere at an angle
 *	to the original XY plane according to the angles ALPHA and BETA,
 *	and the new XY coordinates in this slanted plane will have a
 *	rotation angle of THETA degrees.  The distance to this plane will
 *	be the new Z' value, and its position in the rotated X'Y' plane
 *	will be its new X and Y coordinates.
 *
 *	The angle of inclination is angle between the X'-Y' plane and
 *	the original X-Y plane measured as two angles at the X axis,
 *	Y axis; the angles are ALPHA and BETA respectively.  The angle
 *	of rotation is measured with respect to the Z' axis with the angle
 *	measured in the CCW direction as THETA degress.  In other words,
 *	ALPHA and BETA represent a tilt of the entire coordinate system
 *	while THETA represents the number of degrees of rotation about the
 *	Z' axis of the X'-Y'-Z' coordinate system.
 *
 *	Vector analysis:
 *
 *	The process is done in a two step mapping.  First map the old
 *	XY plane into the new XY plane.  Then map the rotation of the
 *	new coordinate system into the X'Y' plane (intermediate).
 *
 *	The two vectors which define the new X'Y' plane are:
 *	V1: (Mapping of X-Axis)		( cosALPHA, 0, sinALPHA )
 *	V2: (Mapping of Y-Axis)		( 0, cosBETA, sinBETA )
 *
 *	So the normal is the cross-product of these two vectors:
 *
 *		N = V1 x V2 = ( -cosBETAsinALPHA, -cosALPHAsinBETA, cosALPHAcosBETA )
 *
 *	Checking the system's normalization we find:
 *
 *		del = det< N > = (cosALPHA)^2 + (sinALPHA*cosBETA)^2
 *
 *	and not 1.  This means the inverse will needed to be scaled, and
 *	that when ALPHA and BETA = ( n*PI + PI/2 ) where n = 0, 1, ...
 *	the inverse will NOT EXIST!
 *	We are still not done, since we must map the projected XY axis
 *	into the rotated axes.  For that operation the additional vector
 *	set comes into play:
 *
 *	VX:	 cosTHETA,	sinTHETA,	0
 *	VY:	-sinTHETA,	cosTHETA,	0
 *	VZ:	0,		0,		1
 *
 *	Checking this system determinate one finds it equals 1, so it
 *	it a normal system.  The entire mapping function is in turn the
 *	matrix product of the two.
 *	For convenience let us state that the rotation is done first,
 *	then the tilting of the axes.  Let MR represent the rotation tran-
 *	sform, and MT the tilting transform.
 *	Then our final matrix is:		MF = MR*MT
 *	And takes the following form:
 *
 *	cosALPHA*cosTHETA	sinTHETA*cosBETA	cosTHETA*sinALPHA+sinBETA*sinTHETA
 *	-sinTHETA*cosALPHA	cosTHETA*cosBETA	cosTHETA*sinBETA-sinALPHA*sinTHETA
 *	-cosBETA*sinALPHA	-cosALPHA*sinBETA	cosALPHA*cosBETA
 *
 *
 *	And the new coordinates become simply the dot product with this matrix.
 *
 *	Once this is done, one has the mapping from real TIG space into
 *	the mapped world.  To find the inverse one has to invert the above
 *	matrix and divide each element by its determinate (our `del').
 *	For lack of interest on the part of this programmer, he will not
 *	show you the results.  They are present in parametric representation
 *	in newangle().
 */

/*
 *	TIGDOT( &<vector>, &<Xform table> )
 *
 *	Returns:	&<TIGvec>	Always
 */
struct TIGvec *TIGdot(struct TIGvec *vecp, struct TIGxform *xformp )	/* Perform DOT product with Xform matrix */
{
	static struct TIGvec vecret;

	if (xformp->_idtrans == 1) {
		vecret.Xpos = vecp->Xpos;
		vecret.Ypos = vecp->Ypos;
		vecret.Zpos = vecp->Zpos;
	}
	else {
		vecret.Xpos =	xformp->newX.XXpos * vecp->Xpos +
				xformp->newX.YYpos * vecp->Ypos +
				xformp->newX.ZZpos * vecp->Zpos;

		vecret.Ypos =	xformp->newY.XXpos * vecp->Xpos +
				xformp->newY.YYpos * vecp->Ypos +
				xformp->newY.ZZpos * vecp->Zpos;

		vecret.Zpos =	xformp->newZ.XXpos * vecp->Xpos +
				xformp->newZ.YYpos * vecp->Ypos +
				xformp->newZ.ZZpos * vecp->Zpos;
	}
	return( &vecret );
}

/*
 *	TIGDDOT( &<dvector>, &<Xform table> )
 *
 *	Returns:	&<TIGdvec>	Always
 */
struct TIGdvec *TIGddot(struct TIGdvec *dvecp, struct TIGxform *xformp)	/* Perform DOT product with Xform matrix */
{
	static struct TIGdvec vecret;

	if (xformp->_idtrans == 1) {
		vecret.XXpos = dvecp->XXpos;
		vecret.YYpos = dvecp->YYpos;
		vecret.ZZpos = dvecp->ZZpos;
	}
	else {
		vecret.XXpos =	xformp->newX.XXpos * dvecp->XXpos +
				xformp->newX.YYpos * dvecp->YYpos +
				xformp->newX.ZZpos * dvecp->ZZpos;

		vecret.YYpos =	xformp->newY.XXpos * dvecp->XXpos +
				xformp->newY.YYpos * dvecp->YYpos +
				xformp->newY.ZZpos * dvecp->ZZpos;

		vecret.ZZpos =	xformp->newZ.XXpos * dvecp->XXpos +
				xformp->newZ.YYpos * dvecp->YYpos +
				xformp->newZ.ZZpos * dvecp->ZZpos;
	}
	return( &vecret );
}

/*
 *	VECNORM( <vecp> )
 *
 *	Calls:	dveclen()
 *
 *	Returns:	&<normalized vector>
 */
struct TIGdvec *vecnorm(struct TIGvec *vecp)
{
	register double length;
	static struct TIGdvec retvec;

	retvec.XXpos = vecp->Xpos;
	retvec.YYpos = vecp->Ypos;
	retvec.ZZpos = vecp->Zpos;
	
	length = dveclen( &retvec );
	
	retvec.XXpos /= length;
	retvec.YYpos /= length;
	retvec.ZZpos /= length;

	return( &retvec );
}

/*
 *	DVECNORM( <vecp> )
 *
 *	Calls:	dveclen()
 *
 *	Returns:	&<normalized vector>
 */
struct TIGdvec *dvecnorm(struct TIGdvec *vecp)
{
	register double length;
	static struct TIGdvec retvec;

	retvec = *vecp;
	length = dveclen( vecp );

	retvec.XXpos /= length;
	retvec.YYpos /= length;
	retvec.ZZpos /= length;

	return( &retvec );
}

/*
 *	DVECLEN( &<vector> )
 *
 *	Calls:	sqrt(IIIM)
 *
 *	Returns:	<length>	Always
 */
double dveclen(struct TIGdvec *vecp)		/* Return the vector's length */
{
	return(sqrt((vecp->XXpos * vecp->XXpos) +
		    (vecp->YYpos * vecp->YYpos) +
		    (vecp->ZZpos * vecp->ZZpos)));
}

/*
 *	VECLEN( &<vector> )
 *
 *	Calls:	sqrt(IIIM)
 *
 *	Returns:	<length>	Always
 */
double veclen(struct TIGvec *vecp)		/* Return the vector's length */
{
	return(sqrt((double)(vecp->Xpos * vecp->Xpos) +
		    (double)(vecp->Ypos * vecp->Ypos) +
		    (double)(vecp->Zpos * vecp->Zpos)));
}

/*
 *	XFRMXPOS( &<Xform matrix> )
 *
 *	Returns:	&<transpose>	Always
 */
struct TIGxform *xfrmxpos(struct TIGxform *xformp )	/* Take the transpose of the matrix */
{
	static struct TIGxform xret;

	xret.newX.XXpos = xformp->newX.XXpos;
	xret.newY.XXpos = xformp->newX.YYpos;
	xret.newZ.XXpos = xformp->newX.ZZpos;

	xret.newX.YYpos = xformp->newY.XXpos;
	xret.newY.YYpos = xformp->newY.YYpos;
	xret.newZ.YYpos = xformp->newY.ZZpos;

	xret.newX.ZZpos = xformp->newZ.XXpos;
	xret.newY.ZZpos = xformp->newZ.YYpos;
	xret.newZ.ZZpos = xformp->newZ.ZZpos;

	return( &xret );
}

/*
 *	DVEC2VEC( &<vector> )
 *
 *	Returns:	&<vector>	Always
 */
struct TIGvec *dvec2vec(struct TIGdvec *dvecp)	/* Convert doubles vectors to int vectors */
{
	static struct TIGvec retvec;

	retvec.Xpos = dvecp->XXpos;
	retvec.Ypos = dvecp->YYpos;
	retvec.Zpos = dvecp->ZZpos;

	return( &retvec );
}

/*
 *	VEC2DVEC( &<vector> )
 *
 *	Returns:	&<vector>	Always
 */
struct TIGdvec *vec2dvec(struct TIGvec *vecp)	/* Convert int vectors to doubles */
{
	static struct TIGdvec retvec;

	retvec.XXpos = vecp->Xpos;
	retvec.YYpos = vecp->Ypos;
	retvec.ZZpos = vecp->Zpos;

	return( &retvec );
}


/*
 *	NEWPATRN( <X pattern>, <Y pattern>, <rotation> )
 *
 *	Calls:	modf(3)
 */
void newpatrn(char xpat, char ypat, double angle)
{
	register int i;
	double temp;

	/*
	 * First map angle into region from 0 - 180.  The by
	 * interchanging X & Y map into 0 - 90
	 */

	if((angle = modf(angle / 180.0, &temp) * 180.0) < 0)
		angle = 180 - angle;		/* Make angle from 0 - 180 */

	if(angle > 90) {	/* Interchange X & Y style */
		ppxpat = ypat;
		ppypat = xpat;
		pppang = angle - 90;
	}
	else {
		ppxpat = xpat;
		ppypat = ypat;
		pppang = angle;
	}
}

/*
 *	FILL( <X>, <Y>, <Z> )
 *
 *	Calls:	TIGdot(6T), putplt(6T)
 */
void fill(short int x, short int y, short int z)		/* Fill the plane at the given point (mapped space) */
{
	short int args[8];
	short int numArgs;
	struct TIGvec vec, *vecp;

	vec.Xpos = x;
	vec.Ypos = y;
	vec.Zpos = z;
	x = (vecp = TIGdot( &vec, &_ppmtor ))->Xpos;
	y = vecp->Ypos;

	/* load argument buffer */
	numArgs = 8;
	args[0] = I_PATRN;
	args[1] = (short int)LOBYTE(x);
	args[2] = (short int) HIBYTE(x);
	args[3] = (short int)LOBYTE(y);
	args[4] = (short int)HIBYTE(y);
	args[5] = (short int)ppxpat;
	args[6] = (short int)ppypat;
	args[7] = (short int)(pppang * 90.0 / 256.0);

	/* now call putplt with the argument count and pointer to the argument buffer */
	putplt(numArgs, args);
}

/*
 *	RFILL( <X>, <Y>, <Z> )
 *
 *	Calls:	putplt(6T)
 */
void rfill(short int x, short int y, short int z)	/* Fill the plane at the given point (real space) */
{
	short int args[8];
	short int numArgs;

	/* load argument buffer */
	numArgs = 8;
	args[0] = I_PATRN;
	args[1] = (short int)LOBYTE(x);
	args[2] = (short int) HIBYTE(x);
	args[3] = (short int)LOBYTE(y);
	args[4] = (short int)HIBYTE(y);
	args[5] = (short int)ppxpat;
	args[6] = (short int)ppypat;
	args[7] = (short int)(pppang * 90.0 / 256.0);

	/* now call putplt with the argument count and pointer to the argument buffer */
	putplt(numArgs, args);
}

/* The following are special commands.  The
 * code produced by these commands is in the device-exclusive
 * format:
 *
 *
 *
 *
 *      SHADE()
 *
 *      Special command for the matrox to generate shading 
 *
 *
 *	Calls putplt() with args:
 *		x and y are the coordinates
 *		high and low refer to the 2 bytes in each short int
 *		level is the shading from max dark (0) to max light (256)
 */
void shade(short int x, short int y, short int level)
{
	short int args[8];
	short int numArgs;

	/* load argument buffer */
	numArgs = 8;
	args[0] = I_SPECIAL;
	args[1] = S_SHADE;
	args[2] = 5;
	args[3] = (int)LOBYTE(x);
	args[4] = (int)HIBYTE(x);
	args[5] = (int)LOBYTE(y);
	args[6] = (int)HIBYTE(y);
	args[7] = (int)LOBYTE(level);

	/* now call putplt with the argument count and pointer to the argument buffer */
	putplt(numArgs, args);
}

/*
 *	RECTANGLE( )
 *
 *	Special command for the matrox to generate a shaded rectangle.
 *
 *	
 *	Calls putplt() with args:
 *		x1,y1 is the starting corner for the rectangle.
 *		x2,y2 is the ending corner.
 *		outline 0 or 1. 
 *		high, low, and level: refer to SHADE ()
 */
void rectangle(short int x1, short int y1, short int x2, short int y2, short int level)
{
	short int args[13];
	short int numArgs;

	/* load argument buffer */
	numArgs = 13;
	args[0] = I_SPECIAL;
	args[1] = S_RECTANGLE;
	args[2] = 10;
	args[3] = (int)LOBYTE(x1);
	args[4] = (int)HIBYTE(x1);
	args[5] = (int)LOBYTE(y1);
	args[6] = (int)HIBYTE(y1);
	args[7] = (int)LOBYTE(x2);
	args[8] = (int)HIBYTE(x2);
	args[9] = (int)LOBYTE(y2);
	args[10] = (int)HIBYTE(y2);
	args[11] = (int)LOBYTE(level);
	args[12] = (int)HIBYTE(level);
}


/*
 *	NEWSHADE(table_no,t_ptr)
 *
 *	Change the table used by the shading algorthim to a user defined
 *      table.
 */
void newshade(int table_no, short int *t_ptr)
{
	short int args[5];
	short int numArgs;
	char nskip;
	char lo, hi;

	nskip = 2;
 	if (table_no < 0) {
		table_no = 4;
		nskip += 64;
		lo = table_no % 256;
		hi = table_no / 256;

		/* load arguemnt buffer */
		numArgs = 5;
		args[0] = I_SPECIAL;
		args[1] = S_NEWSHADE;
		args[2] = nskip;
		args[3] = lo;
		args[4] = hi;
		putplt(numArgs, args);

		numArgs = -64;
 		putplt(numArgs, t_ptr);

	}
	else {
		lo = table_no % 256;
		hi = table_no / 256;
		numArgs = 5;
		args[0] = I_SPECIAL;
		args[1] = S_NEWSHADE;
		args[2] = nskip;
		args[3] = lo;
		args[4] = hi;
		putplt(numArgs, args);
	}
}

/*
 * RGBCOLOR
 * 	change rgb values of a TIG color
 * input:
 *	color		0 to (NUMCOLORS - 1), the color being changed
 *	r, g, b		the values in range [0-255].
 */
void rgbcolor(int color, int r, int g, int b)
{
	int nskip = 4;
	short int args[7];
	short int numArgs;

	/* load argument buffer */
	numArgs = 7;
	args[0] = I_SPECIAL;
	args[1] = S_RGBCOLOR;
	args[2] = nskip;
	args[3] = color;
	args[4] = r;
	args[5] = g;
	args[6] = b;
}

/* The routines here are real fcts instead of macros */

/*
 *	BUFFER( )
 *
 *	Calls:	putplt(6T)
 *
 *	Tell post-processors to buffer the output.
 */
void buffer()
{
	short int args[1];
	short int numArgs;

	numArgs = 1;
	args[0] = I_BUFR;
	putplt(numArgs, args);
}

/*
 *	NOBUFR( )
 *
 *	Calls:	putplt(6T)
 *
 *	Tell post-processors not to buffer the output.
 */
void nobufr()
{
	short int args[1];
	short int numArgs;

	numArgs = 1;
	args[0] = I_NOBUF;
	putplt(numArgs, args);
}

/*
 *			P E N U P
 *
 *	This routine is called to raise the 'virtual pen'.  Any pen
 * motions will not cause visable output.
 */
void penup()
{
	short int args[1];
	short int numArgs;

	numArgs = 1;
	args[0] = I_UP;
	putplt(numArgs, args);
}

/*
 *			P E N D O W N
 *
 *	This routine is called to lower the virtual pen.  All
 * subsequent pen motions will cause visual output.
 */
void pendown()
{
	short int args[1];
	short int numArgs;

	numArgs = 1;
	args[0] = I_DOWN;
	putplt(numArgs, args);
}


/*
 *	NEWCOLOR( <color>, <intensity>, <width> )
 *
 *	Calls:	putplt(6T)
 *
 *	Request the interpreter to select a new color, brightness and width
 *	of the current pen.  The call may have partial effects with devices
 *	with only one color or intensity.
 */
void newcolor(short int color, short int bright)		/* modified to work on Sgi Iris */
{
	short int args[3];
	short int numArgs;

	numArgs = 3;
	args[0] = I_COLR;
	args[1] = (int)LOBYTE(color);
	args[2] = (int)LOBYTE(bright);
	putplt(numArgs, args);
}

/*
 *			N E W F O R M
 *
 *	This routine requests that the TIG interpreter select a new page
 *	and/or clear the screen.
 */
void newform()
{
	short int args[1];
	short int numArgs;

	numArgs = 1;
	args[0] = I_FORM;
	putplt(numArgs, args);
}

/*
 *	NEWBGND( <color>, <intensity> )
 *
 *	Calls:	putplt(6T)
 */
void newbgnd(short int color, short int intens)	/* Change background to new color and intens */
{
	short int args[3];
	short int numArgs;

	numArgs = 3;
	args[0] = I_BGND;
	args[1] = (int)LOBYTE(color);
	args[2] = (int)LOBYTE(intens);

	putplt(numArgs, args);
}

/*
 *	NEWKEARN( <% of EN space kearn> )
 */
void newkearn(int kearn)	/* Change current kearn space */
{
	ppkearn = kearn - KRNOFF;
}

/*
 *	NEWLINE( <pattern>, <magnification>, <width> )
 *
 *	Calls:	putplt(6T)
 *
 *	Change the line characteristics to look like the bit pattern
 *	of <pat> times the magnification of <mag>, with a line width
 *	of <width>
 */
void newline(short int pat, short int mag, short int width )	/* New LINE characteristics */
{
	short int args[4];
	short int numArgs;

	numArgs = 4;
	args[0] = I_LINE;
	args[1] = (int)LOBYTE(pat);
	args[2] = (int)LOBYTE(mag);
	args[3] = (int)LOBYTE(width);

	putplt(numArgs, args);
}

/* end of special sub-routines */

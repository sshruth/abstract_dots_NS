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
 *	23/8/83   LMO	Change default kern to 0% for Hershey fonts
 *	7/1/86	  LMO	Add _Z_persp for perspective
 *
 */
#include	<stdio.h>
#include	"tig.h"

#define	DEF_KRN	(-KRNOFF)	/* 0% EN space kearn */

	/* Last X, Y, Z coordinates in REAL and MAPPED space */
struct TIGvec ppreal, ppmap;

	/* Transform for writing plane for PLTSTR, PUTSYMB, etc */

struct TIGxform ppwrpln = {	/* Straight mapping */
	{ 1.0, 0.0, 0.0 },
	{ 0.0, 1.0, 0.0 },
	{ 0.0, 0.0, 1.0 },
	 1.0
};

	/* Mapping transformation tables; all angles = 0.0 radians */

struct TIGxform _pprtom = {	/* Real to Mapped */
	{ 1.0, 0.0, 0.0 },
	{ 0.0, 1.0, 0.0 },
	{ 0.0, 0.0, 1.0 },
	{ 1 }
};

struct TIGxform _ppmtor = {	/* Mapped to Real */
	{ 1.0, 0.0, 0.0 },
	{ 0.0, 1.0, 0.0 },
	{ 0.0, 0.0, 1.0 },
	{ 1 }
};

	/* Axis orientations */
double pptheta = { 0.0 };		/* Rotation about Z' axis */
double ppalpha = { 0.0 };		/* Angle between X'Y' plane and X axis */
double ppbeta = { 0.0 };		/* Angle between X'Y' plane and Y axis */

int _Z_persp = 0;			/* turn off perspective */
int ppxoffset, ppyoffset;		/* Offset from real physical 0,0 */
char ppxskew, ppyskew;			/* Current x, y skews */
char ppkearn = { DEF_KRN };		/* Current kearning */
char ppxpat = {-1}, ppypat = {-1};	/* Pattern for filling */
double pppang = 0;				/* Pattern angle */
char ppfont = 0;				/* Current font */
unsigned int ppfsize = { DEFPSZ };	/* Current point size; 1/100-ths inches */

	/* Structs for FONTS */

int ppfindx = {-1};		/* Current index into font lists */
				/* Initialized to -1 to make code read in font
				 * tables the first time.  See putsymb.c: _getfont()
				 */
struct cinfo ppcinfo = {0};		/* Last accessed character */

	/* Std. TIG colors */

char *ppcolor[NCLRS] = {
	"black",
	"brown",
	"red",
	"orange",
	"yellow",
	"green",
	"blue",
	"violet",
	"grey",
	"white",
	"cyan",
	"pink"
};

/* matrox.g
 * June 16, 1980	-- UNIX version with physical mapping of I/O
 *
 *	L. M. Optican and S. G. Lisberger
 */

#include <matrox.h>
/* global variables for matrox subroutines */
struct matrox *mxptr = 0;	/* pointer for phys to map to */
short mxcolor = MXBRT;	/* color drawn, initialize to on */
short mxback = MXDRK;	/* color of background, initialize to off */

/* save = locations for last poshort of vector generator
 * set with mxmove(x,y) or mxvect(x,y), returned by mxwhere(&x,&y);
 * also set by mxtext()
 */
short mxoldx = 0;
short mxoldy = 0;

/* globals for text generator */
short mxcsiz = 1;	/* character size */
short mxcrwd = 4;	/* character width */
short mxcrht = 7;	/* character height */
short mxcrsp = 2;	/* character spacing */
short mxlnsp = 2;	/* line spacing */
short mxmrgn = 0;  	 /* left margin */
short mxtopf = 496; 	 /* top of form */
short mxline = 0;	/* scroll line at top of page */
short mxangl = 0;	/* horizontal/vertical - 1 means vertical */

/* MATROX to PRINTRONIX conversion globals */
short mxbegin = 0;	/* first row to be plotted */
short mxend = MXMAX;	/* last row to be plotted */
short mxspace = MXSPACE;	/* space between windows */
/* scale factors to convert matrox to printronix: dots per inch */
double vpxfac = 50.0;		/* dots per inch vertically */
double hpxfac = 50.0;		/* dots per inch horizontally */

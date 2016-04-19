/* matroy.h:  definitions for REX tilted matrox video display
 *  This means that the lower left corner of the tilted matrox
 * (tilted so controls are to the right of the screen)
 * is (0,0), the abscissa is the y-reg, and the ordinate is the x-reg
 * requires physical mapping with mxphys()
 */

/* struct for matrox */
struct matrox {
	short dr;		/* data register */
	short xr;		/* x-address register */
	short yr;		/* y-address register */
	short sr;		/* scroll register */
} ;


/* MATROX flags */

#define MXCLR	0100000
#define MXBRT	01
#define MXDRK	00
#define MXRDY	0200
#define MXBLNK	0100
#define MXMAX	511
#define MXMINY	31
#define MXMASK	(1 << 9) - 1

/* space between windows */
#define MXSPACE	15


/* MACROs for matrox */
#define mxerase {if(mxcolor) mxptr->dr=MXCLR; else mxptr->dr=MXCLR | 01;}
#define mxscroll(z) {mxptr->sr = (z);}
#define mxwait while((mxptr->sr & MXRDY) == 0)
#define mxfast(xx,yy,zz) {mxptr->xr=(yy)<<1;mxptr->yr=(xx); \
	mxptr->dr=(zz);}
#define mxnewy(yy) {mxptr->xr=(yy)<<1;}
#define mxnewx(xx) {mxptr->yr=(xx);}
#define mxonlyy(yy) {mxptr->xr=(yy)<<1;mxptr->dr=mxcolor;}
#define mxonlyx(xx) {mxptr->yr=(xx);mxptr->dr=mxcolor;}
#define mxwrite(zz) {mxptr->dr=(zz);}
#define mxlim(zz)	{mxbegin=max(mxbegin,(zz));mxend=min(mxend,(zz));}
#define mxmove(xx,yy)	{mxoldx = (xx); mxoldy = (yy);}
#define setmxcolor(z)	if ((z) == 1) {mxcolor=1;mxback=0;} \
				else {mxcolor=0;mxback=1;}

/* DATA DEFINITIONS ----------------------------------------------------- */
/*
 * June 16, 1980
 *
 * Unified globals for NIH distribution of matrox functions
 *	L. M. Optican and S. G. Lisberger
 */

struct matrox *mxptr;

short mxcolor;	/* color drawn, initialize to on */
short mxback;	/* color of background, initialize to off */

/* save locations for last poshort of vector generator
 * set with mxmove(x,y) or mxvect(x,y), returned by mxwhere(&x,&y);
 * also set by mxtext()
 */
short mxoldx;
short mxoldy;

/* globals for text generator */
short mxcsiz;	/* character size */
short mxcrwd;	/* character width */
short mxcrht;	/* character height */
short mxcrsp;	/* character spacing */
short mxlnsp;	/* line spacing */
short mxmrgn; 	/* left margin */
short mxtopf;	/* top of form */
short mxline;	/* scroll line at top of page */
short mxangl;	/* horizontal/vertical - 1 means vertical */

/* MATROX to PRINTRONIX conversion globals */
short mxbegin;	/* first row to be plotted */
short mxend;	/* last row to be plotted */
short mxspace;	/* space between windows */
/* scale factors to convert matrox to printronix: dots per inch */
double vpxfac;		/* dots per inch vertically */
double hpxfac;		/* dots per inch horizontally */


/*
 *		S T A N D A R D   H E A D E R   F I L E
 *
 *				T I G
 *
 *	For the Terminal Independent Graphics package.
 */

/*
 *	January 18, 1989 - Brad Zoltick and Barry Richmond - Fixed macros 
 *		HIBYTE(x) and LOBYTE(x) to extract bytes with portability 
 *              across processors.  Depends on compiler doing proper sign 
 *		extension for char.
 *	22nov91	LMO	set up identity transform flags & clean up for
 *			ANSI-C under IRIX 4.0
 *	28jun94	LMO	fix HIBYTE LOBYTE for VAX & PC
 *
 *	NOTE:  must include <stdio.h> above "tig.h" in user progs.
 */
#ifndef NULL
#define NULL 0
#endif

#ifndef HUGE
#define HUGE ((float) 3.40282346638528860e+38)
#endif

	/* Intermediate Codes */

#define	I_BUFR	'B'	/* Buffer output */
#define	I_COLR	'C'	/* Change color; 2 bytes */
#define	I_DOWN	'D'	/* Pen down */
#define	I_FORM	'F'	/* newform */
#define	I_BGND	'G'	/* Background change; 2 bytes */
#define	I_LINE	'L'	/* Change line parameters; 3 bytes */
#define	I_MOVE	'M'	/* Move pen; 2 words */
#define	I_NOBUF	'N'	/* No buffering */
#define	I_PATRN	'P'	/* Pattern fill a polygon; 2 words, 3 bytes */
#define	I_UP	'U'	/* pen up */
#define I_SPECIAL 'S' /*  special peripheral dependent command  */

	/* Special periphery dependent commands  */
	
#define S_ARC	    'a' /* arc designator */
#define S_RGBCOLOR  'b'	/* change rgb values of a color */
#define S_CIRCLE    'c' /* circle shade designator */
#define S_ELLIPSE   'e' /* ellipse shade designator */
#define S_LINEWIDTH 'l' /* linewidth designator */
#define S_NEWSHADE  'n' /* new shade table */
#define S_RECTANGLE 'r' /* rectangle shade designator  */
#define S_SHADE     's' /* pixel shade designator  2 words, 1 byte  */
#define S_ARROW	    'w' /* arrow shade designator */


	/* Std. Colors */

#define	NCLRS	12

#define	BLACK	0
#define	BROWN	1
#define	RED	2
#define	ORANGE	3
#define	YELLOW	4
#define	GREEN	5
#define	BLUE	6
#define	VIOLET	7
#define	GREY	8
#define	WHITE	9
#define	CYAN	10
#define	PINK	11

#define	KRNOFF	100	/* Kearn offset; -28 ---> 127 */

	/* Macro Pseudo-Functions */
/*
#define	buffer()	putplt(1,I_BUFR)
#define	nobufr()	putplt(1,I_NOBUF)
#define penup()		putplt(1,I_UP)
#define pendown()	putplt(1,I_DOWN)
#define	newbgnd(c,i)	putplt(3,I_BGND,(c),(i))
#define newcolor(c,i)	putplt(3,I_COLR,(c),(i))
#define newform()	putplt(1,I_FORM)
#define	newline(p,m,w)	putplt(4,I_LINE,(p),(m),(w))
*/

#define	newkearn(x)	(ppkearn = (x)-KRNOFF)

	/* Font Related things */

#define	STD	0	/* Standard font (Hershey Roman Simplex plus specials) */
#define	TR	1	/* Times Roman font */
#define	TI	2	/* Times Italic font */
#define	TB	3	/* Times Bobl font */
#define	MATH	4	/* Special Math font */
#define	BDR	5	/* Bodoni Roman font */
#define	NONR	6	/* Nonie Roman font */
#define	NONI	7	/* Nonie Italic font */
#define	NONB	8	/* Nonie Bold font */
#define	SCR	9	/* Connecting Script font */
#define	SHD	10	/* Shadow character font */
#define	SGN	11	/* Sign font */
#define	HRD	12	/* Hershey Roman Duplex font */
#define	HRC	13	/* Hershey Roman Complex font */
#define	HRT	14	/* Hershey Roman Triplex font */
#define HIC	15	/* Hershey Italic Complex font */
#define HIT	16	/* Hershey Italic Triplex font */
#define	ARRO	17	/* Arrowhead font for putarro(6T) */

#define	NSTYLES  17	/* Number of different fonts */


#define	NCHARS	128	/* Number of characters to a font, if character doesn't
			 * exist then the vaddr is zero */
#define	FNT_REF	84	/* A font character is 84 TIG units high (height of `M') */
#define	MINPSZ	2	/* Smallest allowed character in 1/100-ths in. */
#define	DEFPSZ	20	/* Default height 0.2 in or 20/100-ths */
#define	MAXPSZ	600	/* Largest allowed character in 1/100-ths */

	/* To scale a font character into TIG units use... */

#define	CSCALE(x)	((x)*10L*ppfsize/FNT_REF)	/* Character N space */
#define	SSCALE(x)	((x)*ppfsize/200)		/* %/10 skew of EN space */
#define	KSCALE(x)	(((int)(x)+KRNOFF)*ppfsize/20)	/* % kearn of EN space */

struct fntabl {
	int fdfont;		/* Fildes to font file ( for open and read) */
	char *fname;		/* Name of font file */
};

struct fdict {			/* Font file dictionary */
	long vaddr;		/* Address in font file */
	struct fhdr {		/* Font header */
		short int width;/* Width of character */
		short int nvecs;/* Number of vectors comprising character */
	} finfo;
};

	/* General character information structure */

struct cinfo {
	char font;		/* Font of the character */
	char letr;		/* Letter */
	struct fhdr ftinfo;	/* Font information */
};

#define	IO_USE	01	/* Channel in use */
#define	IO_BUF	02	/* Channel is buffered */

	/* Control modes for tigio() */

#define	NOCHG	-1	/* Don't change current state */
#define	BUFR	0	/* Make buffered */
#define	NOBUF	1	/* Make unbuffered */
#define	FLUSH	2	/* Flush the buffers */


	/* Line Drawing mode defines */

#define	OLINES	0	/* Only line segments */
#define	OMARKS	1	/* Only marker characters */
#define	MRKNLIN	2	/* Markers and line segments */

	/* Axis Drawing modes */

#define	NORMAL	000	/* Axis labels are `below' the axis */
#define	GG	001	/* (Greener Grass, comp. of RNJ) labels `above' */
#define	NOZERO	002	/* No not place a label at the TIG zero point */
#define	NORIGHT	004	/* Do not place a label at the right extreme */
#define NOLEFT	010	/* Do not place a label at the left extreme */
#define	RTARRO	020	/* Place arrowhead at right end of axis */
#define	LTARRO	040	/* Place arrowhead at left end of axis */
			/* The convention of `above' and `below' can be
			 * specified as follows:  Something is `above'
			 * the axis if the labels and tick mark motions
			 * are in the same direction as the character vertical
			 * motions, and `below' is when then labels and tick
			 * mark motions are in the opposite direction as the
			 * character vertical motions.
			 * Something is to the `right' when the NTIG value
			 * is a maximum, and is the `left' when the NTIG
			 * value is a minimum.
			 */

	/* Box drawing defines for corners */

/*
 * Boxes are considered in the following orientation:
 *	Base on XY plane, Z stands up.
 * Then the convention of letters makes sense as follows:
 *	L (Left): -X		R (Right): +X
 *	O (Out): -Y		I (In): +Y
 *	D (Down): -Z		U (Up): +Z
 *
 *	Where the letters are directions.  Always remember the
 *	order XYZ, and select the appropriate letters.
 */

#define	LOD	(1<<0)		/* 0,0,0 */
#define	ROD	(1<<1)		/* X,0,0 */
#define	RID	(1<<2)		/* X,Y,0 */
#define	LID	(1<<3)		/* 0,Y,0 */
#define	LOU	(1<<4)		/* 0,0,Z */
#define	ROU	(1<<5)		/* X,0,Z */
#define	RIU	(1<<6)		/* X,Y,Z */
#define	LIU	(1<<7)		/* 0,Y,Z */

	/* Vector representation of 3D space */

struct TIGvec {
	int Xpos;	/* X position */
	int Ypos;	/* Y position */
	int Zpos;	/* Z position */
};

struct TIGdvec {	/* Same as above but doubles */
	double XXpos;
	double YYpos;
	double ZZpos;
};

struct TIGpvec {	/* Polar Coordinates */
	double Radius;
	double Theta;	/* Between X and proj of Radius on XY */
	double Phi;	/* Between Z and radius */
};

struct TIGxform {	/* Transformation table (mapping) */
	struct TIGdvec newX;
	struct TIGdvec newY;
	struct TIGdvec newZ;
	int _idtrans;		/* identity transformation */
};

	/* Data assignment structure */

struct data {		/* Structure to access different data types */
	short int * shortp;	/* Signed short integer pointer */
	long *lngp;		/* Long integer pointer */
	float *fltp;		/* Floating point -- single prec. */
	double *dblp;		/* Floating point -- double prec. */
};

struct Fctinfo {	/* Information structure for NUMPREP mode */
	char *TIGstrp;	/* Compacted TIG string pointer */
	int soffset;	/* String offset */
	int slength;	/* String length */
};


	/* Coefficient assignment structure */

/*
 *	The way this works:
 *
 *	The user supplies a functions which performs five operations:
 *		1)	Map User Data space into Display space
 *		2)	Map Display space into User Data space
 *		3)	Map Display space into real Display space
 *		4)	Prepare Data number for printing in Display space
 *		5)	Print data number at current location in Display space.
 *
 *	The scheme works on the principle that the user will use a
 *	linear function of (X) (TIG space) such that when the SLOPE
 *	of (X), (_CA), is adjusted, and an intercept found, (_CB), the
 *	User's data space delta-range will map into the Normalized
 *	TIG space delta range of unity, such that
 *		min = f(dataleft,_CA,_CB,--other coefficients--)
 *		max = f(dataright,_CA,_CB,--other coefficients--)
 *	and:	1.0 = max - min
 *
 *	_CA and _CB are set by the function SETCOEF().  DATALEFT and DATARIGHT
 *	are gererally adjusted by taking the real data left and data
 *	right, converting it to display-number space, NUMNICE()-ing the
 *	numbers and reconverting them into data space (the new DATALEFT and
 *	DATARIGHT).  The other coefficients are completely dependent on the
 *	type of function the user uses.
 *
 *	Generally the DATARIGHT value is selected as the data minimum,
 *	and the DATALEFT value is selected as the data maximum.
 *
 *	Fortunately for the user, three of the most used functions are
 *	already available:  _fctlin(), _fctexp(), _fctpow() for linear,
 *	exponential, and power displays.
 *
 *	For _Fctlin():
 *
 *	DIS-to-DAT:		(Y) = (SLOPE*(D)) + INTERCEPT
 *
 *	For _Fctexp():
 *
 *	DIS-to-DAT:		(Y) = ESLOPE * BASE ^ (SLOPE*(D)) + INTERCEPT
 *
 *	For _Fctpow():
 *
 *	DIS-to-DAT:		(Y) = (SLOPE*(D)) ^ POWER + INTERCEPT
 *
 *	Conversion from TIG-to-DIS and DIS-to-TIG is done by the mapping
 *	functions TIG2DIS(), and DIS2TIG() respectively; they are always
 *	linear, since they just scale the space.
 *
 *	TIG-to-DIS:	(D) = _CA * (X) + _CB
 *	DIS-to-TIG:	(X) = ((D) - _CB) / _CA
 *
 *	Where (X) is the integer value in TIG space, and (Y) is the user's
 *	desired data point, and (D) is the displayed number.
 *
 *	The format for a arguments passed to these functions is as follows:
 *
 *	(*Fctcall)( number, mode, &(coef struct), &(format) )
 *		double number;		Number which is operated upon
 *		int mode;		Mode in which operation is to take place
 *		struct coef *coefp;	Pointer to coefficient structure
 *		char *format;		Format for printed-display number
 *
 *	NOTE:	Since the user function maps the data points into a space from
 *	with a range of unity this means that when data is displayed additional
 *	scaling is also needed to map it into MTU (Mapped TIG-UPA space).
 */

	/* Modes of operation for (*Fctcall)() */

#define	DAT2DIS	0	/* Convert NUM from Data space into DISPLAY space */
#define	DIS2DAT	1	/* Convert NUM from DISPLAY space into Data space */
#define	DIS2dis	2	/* Convert Display number into Funny Display number
			 * This is primarily used by logarithmic functions
			 * where a logarithmic appearance is desired.
			 */
#define	dis2DIS	3	/* Conver Funny Display number back to real Display */
#define	NUMPREP	4	/* Prepare number string; return info pointer */
#define	NUMPRNT	5	/* Print NUM at current location */

#define	NCOEFS	5	/* Number of coefficients allowed for equas. */

	/* Standard functions; coefficient assignments */

	/* For _Fctlin() */

#define	SLOPE		0
#define	INTERCEPT	1

	/* For _Fctexp() */

#define	SLOPE		0
#define	INTERCEPT	1
#define	ESLOPE		2	/* Exponential slope */
#define	BASE		3	/* Exponential base */

	/* For _Fctpow() */

#define	SLOPE		0
#define	INTERCEPT	1
#define	POWER		2

	/* Values for `mode' */

#define	SHORT	0	/* Short Integer */
#define	LONG	1	/* Long Interger */
#define	FLOAT	2	/* Float */
#define	DOUBLE	3	/* Double */
#define	MAXMODE	DOUBLE

struct coef {
	struct Fcttype {		/* User function sometime returns other stuff */
		struct Fctinfo Retinfo;	/* Returns structure */
		double Retdbl;		/* Returns double */
	} *(*Fctcall)();		/* A pointer to one of the above */
	double _CA;
	double _CB;
	double _Coef[NCOEFS];		/* Number of Coefficients allowed */
};

struct darray {			/* Data arrays have headers */
	struct coef dcoef;	/* Function describing this data */
	char mode;		/* One of four data types */
	int nelems;		/* Number of elements in array */
	double left, right;	/* Extrema values for mapping into DISPLAY space */
	struct data datap;	/* Pointer structure to different data types */
};

	/* A Data array with a scale factor makes a space axis */

struct spaxis {
	struct darray *datarp;		/* Data array structure pointer */
	unsigned int axscale;		/* Axis scale factor */
};

	/* A number of data axes together make a space */

struct space {		/* Primarily used by TIG3dcnv() */
	struct spaxis xaxis;
	struct spaxis yaxis;
	struct spaxis zaxis;
};


	/* Global variables and functions of interest */

extern char *ppcolor[];			/* In-core color table */
extern int ppfindx;			/* Current index into font list */
extern struct fdict ppfinfo[];		/* In core font table core */
extern struct fntabl fonttab[];		/* In core font table entries */

	/* Positions in REAL space and TIG space */

extern int ppxoffset, ppyoffset;	/* Origin's real X,Y position */
extern struct TIGvec ppreal, ppmap;	/* Real and mapped positions */

extern struct TIGxform _pprtom;		/* Real to Mapped Xform matrix */
extern struct TIGxform _ppmtor;		/* Mapped to Real Xform matrix */
extern struct TIGxform ppwrpln;		/* Transform for writing plane for PLTSTR */

extern char ppxskew, ppyskew, ppfont;	/* Current X,Y skews and font */
extern char ppkearn;			/* Current kearn space */
extern unsigned int ppfsize;		/* Current point size in 1/100-ths in. (0.01) */
extern struct cinfo ppcinfo;		/* Last accessed character information */
extern struct cinfo *getcinfo();	/* Get character information and load above */
extern char *_strconv();		/* Convert escaped string into compact TIG string */

	/* Orientation angles of Mapped (Prime) system / real system */
extern double pptheta;			/* Angle of rotation about Z' */
extern double ppalpha;			/* Angle between X'Y' plane and X */
extern double ppbeta;			/* Angle between X'Y' plane and Y */
					/* Angles are in DEGREES */
extern int _Z_persp;			/* perspective distance to
					 * observer on Z-axis, in
					 * 1000ths of an inch
					 */
extern char *ncvt(), *dtoa();

	/* Mapping functions using struct coef */

extern struct Fcttype *_Fctlin(), *_Fctexp(), *_Fctpow();

	/* Axis external variables */

extern int _A_mjt, _A_mnt;	/* Major and minor tick mark lengths */
extern int _A_ndis, _A_nht;	/* Number's distance from axis and height */
extern int _A_tdis, _A_tht;	/* Title's distance from axis and height */

	/* Ellipse/circle function for PLOTF */

extern double _E_maj, _E_min;		/* Major minor axes */

extern struct TIGvec _E_ctr;		/* Center */
extern struct TIGxform _E_xform;	/* Ellipse function plane transform */
extern double wellip();			/* query position function */
extern struct TIGvec *_Fellip();	/* Actual PLOTF function */

	/* Fill function pattern variables */

extern char ppxpat, ppypat;			/* X and Y pattern, pattern angle */
extern double pppang;

	/* Other mapping functions */

extern short int lgetwd();		/* pick up short int from stdin */
extern double dveclen(), veclen();	/* Get length of vector */
extern double DIS2TIG(), TIG2DIS();	/* Mapping fcts from DIS --> TIG and back */
extern double wellip();			/* Where on ellipse return */
extern struct TIGvec *dvec2vec();	/* Double vector ---> Int vector conv */
extern struct TIGvec *TIG3dcnv();	/* Space coordinate conversion routine */
extern struct TIGvec *TIGdot();		/* Mapping function */
extern struct TIGdvec *vec2dvec();	/* Int vector ---> Double vector conv */
extern struct TIGdvec *TIGddot();	/* Same as above only with `TIGdvec' */
extern struct TIGdvec *vecnorm();	/* Normalize a int vector */
extern struct TIGdvec *dvecnorm();	/* Normalize a double vector */
extern struct TIGdvec *pol2cart();	/* Polar to Cartesian conversion */
extern struct TIGpvec *cart2pol();	/* Cartesian to polar conversion */
extern struct TIGxform *xfrmxpos();	/* Transpose a transformation matrix */

	/* Tig I/O functions */

char *getTigList(void);
void freeTigList(void);
int getTigListPtr(void);
void setTigListPtr(int);
void dumpTigList(void);

/*
 * tig_misc.h
 *	contains constants and structures that are application
 * or machine dependent
 */

#define MAX_X	10610	/* max x-deviation on laser writer */
#define MAX_Y	 8000	/* max y-deviation on laser writer */
#define MIN_RES	1	/* Minimum resolution in 1000ths of an inch */


typedef unsigned unsign;

/*
/*
 * structure to access an
 * integer in bytes
 */
#ifdef vax
#define LOSHIFT 0
#define HISHIFT 8
typedef struct
{
	char	lobyte;
	char	hibyte;
} * BYTES;
#else	/* defines for sun4 and iris */
#define LOSHIFT 8
#define HISHIFT 0
typedef struct
{
	char	hibyte;
	char	lobyte;
} * BYTES;
#endif

#ifdef vax
typedef struct
{
	char	hibyte;
	char	lobyte;
} * BYTES;
#endif

/*
 * Macros to access hi and lo bytes - more portable than other methods.
 *	The casting is necessary to force the correct sign extension
 *	after byte extraction.
 */
/* correct macros for sun4 and iris */


#define HIBYTE(x)	((char)(((x) >> 8 ) & 0xff))
#define LOBYTE(x)	((char)((x) - (x & ~0377)))

/* #define LOBYTE(x)	((char)((x) - 256 * HIBYTE(x))) */


/*
 * structure to access a long as integers
 */
typedef struct {
	short int	loword;
	short int	hiword;
} * WORDS;

/*
 * math stuff
 */
#ifndef PI
#define	PI	3.14159265358979323846264338
#endif
#ifndef twopi
#define twopi	(2 * PI)
#endif
#ifndef pi
#define pi	(PI)
#endif
#ifndef deg2rad
#define deg2rad	 0.017453292519943
#endif
#ifndef rad2deg
#define rad2deg 57.295779513082320
#endif
#ifndef sqrt2
#define sqrt2	1.41421356237309504880
#endif

/* HDDN.H -- Header for hidden-line surface generation */


/* XFORM
 *	structure to hold all variables needed to perform transformation
 * from user space to TIG page
 */
typedef struct {
	float xtrans, ytrans, ztrans;	/* user-unit translations
					 * to center of rotation */
	float alpha, beta, theta;	/* rotation angles (in deg), same as TIG */
	float zht;			/* distance to observer on +z-axis */
	float hscl, vscl;		/* user-to-TIG scale factor */
	float hoff, voff;		/* user-to-TIG offset */
	float xlo, xhi;			/* min, max in user-units */
	float ylo, yhi;
	float zlo, zhi;
} XFORM;

/* BOUNDS
 *	structure for lower and upper bounds of plot space for
 * hidden lines
 */
typedef struct {
	int lower;
	int upper;
} BOUNDS;

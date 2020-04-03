/*
 *-----------------------------------------------------------------------*
 * NOTICE:  This code was developed by the US Government.  The original
 * versions, REX 1.0-3.12, were developed for the pdp11 architecture and
 * distributed without restrictions.  This version, REX 4.0, is a port of
 * the original version to the Intel 80x86 architecture.  This version is
 * distributed only under license agreement from the National Institutes 
 * of Health, Laboratory of Sensorimotor Research, Bldg 10 Rm 10C101, 
 * 9000 Rockville Pike, Bethesda, MD, 20892, (301) 496-9375.
 *-----------------------------------------------------------------------*
 */

/*
 * Header for graphics API.  This API is based on the Matrox card
 * originally used on the pdp11.  It was an x,y addressable 512x512x1
 * frame buffer.  A pixel was addressed by loading the x and y registers.
 * Its value (on or off) was set by loading a third register.
 * This API is now designed to be adapter independent.  All hardware
 * dependent functions are located in mxlow.c.
 */
#define VGA_640	1	/* define for VGA 480x640 */

#define swap(a,b,c)	(c = a, a = b, b = c)
#define sgn(x)		((x) < 0 ? -1 : 1)

extern int lastx, lasty;    /* last x and y addresses used */
extern int mxcolor;	    /* current foreground color */
extern int mxback;	    /* current background color */
extern int mxwrap;	    /* if true, wrap display */

#define MXMAX	511
#define MXBRT	15	    /* default foreground color */
#define MXDRK	0	    /* default background color */

/*
 * save locations for last point of vector generator
 * set with mxmove(x,y) or mxvect(x,y), returned by mxwhere(&x,&y);
 * also set by mxtext()
 */
extern int mxoldx;
extern int mxoldy;

/*
 * Globals for text generator.
 */
extern int mxcsiz;	/* character size */
extern int mxcrwd;	/* character width */
extern int mxcrht;	/* character height */
extern int mxcrsp;	/* character spacing */
extern int mxlnsp;	/* line spacing */
extern int mxmrgn; 	/* left margin */
extern int mxtopf;	/* top of form */
extern int mxline;	/* scroll line at top of page */
extern int mxangl;	/* horizontal/vertical - 1 means vertical */
 
#define setmxcolor(z)	{mxcolor= z;}

/*
 * mxfast macro.
 */
#ifdef GOO
#define mxfast(x, y, z) \
{\
    unsigned int byte_add;\
    unsigned int off;\
    extern unsigned char far *fb_p;\
\
    lastx= (x);\
    lasty= (y);\
    if((y) < 0 || (y) > 479 || (x) < 0 || (x) > 639) return;\
    byte_add= ((479-(y)) * 80) + ((x) / 8);\
    off= 0200 >> ((x) % 8);\
    if((z)) *(fb_p + byte_add) |= off;\
    else  *(fb_p + byte_add) &= ~off;\
}
#endif

/*
 * Macros (old code for reference when porting).
 */
#ifdef GOO
#define mxerase_ {if(mxcolor) mxptr->dr=MXCLR; else mxptr->dr=(MXCLR | 01);}
#define mxscroll_(z) {mxptr->sr = (z);}
#define mxfast_(xx,yy,zz) {mxptr->xr=(xx)<<1;mxptr->yr=MXMAX-(yy); \
	mxptr->dr=(zz);}
#define mxnewx_(xx) {mxptr->xr=(xx)<<1;}
#define mxnewy_(yy) {mxptr->yr=MXMAX-(yy);}
#define mxonlyx_(xx) {mxptr->xr=(xx)<<1;mxptr->dr=mxcolor;}
#define mxonlyy_(yy) {mxptr->yr=MXMAX-(yy);mxptr->dr=mxcolor;}
#define mxwrite_(zz) {mxptr->dr=(zz);}
#define mxlim_(zz)	{mxbegin=max(mxbegin,(zz));mxend=min(mxend,(zz));}
#define mxmove_(xx,yy)	{mxoldx = (xx); mxoldy = (yy);}
#define setmxcolor_(z)	if ((z) == 1) {mxcolor=1;mxback=0;} \
				else {mxcolor=0;mxback=1;}
#endif

/*
 * Function prototypes.
 */
void mxchar(char c);
void mxsize(int fac);
void mxerase(int flag);
void mxscroll(int z);
void mxfast(int xx, int yy, int zz);
void mxnewx(int xx);
void mxnewy(int yy);
void mxonlyx(int xx);
void mxonlyy(int yy);
void mxwrite(int zz);
void mxmove(int xx,int yy);
void mxcircle(int xoff,int yoff,int radius);
void mxform(int mar, int itop);
void mxover(int *pcurs, int newx, int newy);
void mxvect(int x1, int y1);
void mxwhere(int *px, int *py);
void mxwait(void);
void mxtext(char *p);
int mxmode(int flag);
int mxmap(int flag);
void mxdot(int x,int y);
int mxread(void);

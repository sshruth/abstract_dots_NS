/*
 *	T E R M I N A L   I N D E P E N D E N T   G R A P H I C S
 *
 *			by:  George E. Toth
 *			  March 29, 1980
 *
 *	These routines produce the Terminal Independent Graphics
 *	Intermediate language designed by Michael J. Muuss.
 *
 *	Routines:
 *
 *	putsymb()	Draw a symbol in the current point size, in the current
 *			font, in the current position.  The symbol is passed
 *			and must be >0 and <0200.
 *
 *	struct cinfo
 *	*getcinfo()	Get a character in the current font.  Return a pointer
 *			the the structure
 *
 *	Internal routines:
 *
 *	_drawc()	Draw a character in the current position, given the
 *			vector list, the number of vectors, and scale
 *
 *	struct charstr
 *	*_getfc()	Get character structure address in the
 *			current font, and return address.
 *
 *	struct fntabl
 *	*_getfont()	Get the current font in core if it is not.  Return
 *			pointer to table.  Character header is loaded acc.
 *
 *		R E V I S I O N  H I S T O R Y
 *	===================================================
 *	03/29/80  GET	Wrote subroutine.
 *
 *	10/13/80  GET	Characters with widths > 255 are now allowed and
 *			handled correctly.
 *	19 Oct 81 AVH, LMO	Put the GT alloc stuff in here locally
 *				so as not to confuse it with the ones that
 *				are standard V7. (No longer relevant. BJZ)
 *	04 Jan 83 LMO	& char with 0377 before test!
 *	24 Aug 83 LMO	add new Hershey fonts to fonttab[]
 *	11 Jan 88 LMO	port to sun, put fonts in standard place
 *	25 Oct 88 BJZ   replaced the linked list data structure for
 *			character data by loading font files into memory.
 *			Only standard C lib memory allocation routines used.
 *	08 Feb 89 BJZ & BJR Converted byte addressing scheme to current portability
 *			standards, i.e., using shifts, masks, and casts to produce desired
 *			portable byte extraction with proper sign extension.
 *	17jun94	LMO	fix drawc pointer input
 */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include "tig.h"

#ifndef FNTLIB  /* specify on the cc command line with -D option */
#define FNTLIB "/usr/local/lib/tig/"
#endif

#define	NFILES	10	/* Number of font files which can be open at a time */

struct charstr {
	struct cinfo chinfo;
	short int *fdata;  /* fdata is the address of 1st xy vector */
};

/*
 * These #defines are taken from <sys/file.h> needed for Solaris 7.0
 */
#ifndef L_SET
#define L_SET   0       /* for lseek */
#endif
#ifndef L_INCR
#define L_INCR          1       /* relative to current offset */
#endif
#ifndef L_XTND
#define L_XTND          2       /* relative to end of file */
#endif
/*
 * end of #defines for Solaris 7.0
 */

extern int errno;
static struct charstr *_getfc(), *_find();
static struct fntabl *_getfont();
static void _drawc(short int *vp, short int nvecs, double scale, short int width);

/*
 *	Layout of font files:
 *
 *	All files are composed of a header followed by character definitions.
 *	More exactly:
 *			struct fdict ppfinfo[NCHARS];	/* Header */
 *			int fdata0[];
 *			int fdata1[];
 *			 :	:
 *			int fdata127[];			/* Characters */
 *
 *	Each `fdict' structure holds the virtual address of the vector
 *	character, and the header information for the character: the width
 *	and the number of vectors which comprise the character.
 */
/* FONTS for TIG */
struct fntabl fonttab[NSTYLES] = {
	{ (FILE *)NULL, "STD.f" },	/* The standard graphics symbols */
	{ (FILE *)NULL, "TR.f" },	/* Times Roman */
	{ (FILE *)NULL, "TI.f" },	/* Times Italic */
	{ (FILE *)NULL, "TB.f" },	/* Times Bold */
	{ (FILE *)NULL, "MATH.f" },	/* Math font */
	{ (FILE *)NULL, "BDR.f" },	/* Bodoni Roman */
	{ (FILE *)NULL, "NONR.f" },	/* Nonie Roman */
	{ (FILE *)NULL, "NONI.f" },	/* Nonie Italic */
	{ (FILE *)NULL, "NONB.f" },	/* Nonie Bold */
	{ (FILE *)NULL, "SCR.f" },	/* Script font */
	{ (FILE *)NULL, "SHD.f" },	/* Shadow */
	{ (FILE *)NULL, "SGN.f" },	/* Sign */
	{ (FILE *)NULL,  "HRD.f" },	/* Hershey Roman Duplex */
	{ (FILE *)NULL,  "HRC.f" },	/* Hershey Roman Complex */
	{ (FILE *)NULL,  "HRT.f" },	/* Hershey Roman Triplex */
	{ (FILE *)NULL,  "HIC.f" },	/* Hershey Italic Complex */
	{ (FILE *)NULL,  "HIT.f" }	/* Hershey Italic Triplex */
/*	{ (FILE *)NULL,  "ARRO.f" }	/* Arrow font for AXIS(6T) */
};
	/* NSTYLES = 17   */

/* The first member of fontab is replaced with the file 
 *  descriptor if font file is opened.
 */

char *block[NSTYLES];		/* pointer to memory storing font files  */
struct fdict ppfinfo[NCHARS];

/*
 *	PUTSYMB( <character> )
 */
void putsymb(int c)	/* Draw character of the current font, point size at current position */
{  
	register struct charstr *datap;
	double scale;
	struct TIGvec vecsave;

	vecsave = ppmap;	/* Save current mapped position */

	c &= 0377;
	if((c < 0) || (c >= 0200)) {
		fprintf(stderr,"0%o: Illegal character\n",c);
		return;
	}

	scale = 10.0 * ppfsize / FNT_REF;

	if((datap = _getfc(c)) == (struct charstr*)NULL) {
		fprintf(stderr,"getfc fails\n");  
		return;
	}

	_drawc(datap->fdata, datap->chinfo.ftinfo.nvecs,
		scale, datap->chinfo.ftinfo.width);

	if( ppxskew || ppyskew ) {	/* Want skewed double print */
		struct TIGvec jvec;

		jvec.Xpos = vecsave.Xpos + SSCALE(ppxskew);
		jvec.Ypos = vecsave.Ypos + SSCALE(ppyskew);
		jvec.Zpos = vecsave.Zpos;
		move2pt(jvec.Xpos, jvec.Ypos, jvec.Zpos);
		_drawc(datap->fdata, datap->chinfo.ftinfo.nvecs,
			scale, datap->chinfo.ftinfo.width);
	}
	/* Return to start location */
	move2pt(vecsave.Xpos, vecsave.Ypos, vecsave.Zpos);
}

/*
 *	_DRAWC( <vector list>, <no. of vectors>, <scale> )
 *
 *	Calls:	move2pt(), pendown(), penup()
 */

/* Draw a character at the current position */
static void _drawc(short int *vp, short int nvecs, double scale, short int width)
{
	register short int nx, ny;	/* New x,y coordinates */
	short int ox, oy;		/* Old x,y coordinates */
	short int i;

	struct TIGvec jvec, *vecp;
	struct TIGvec origin;		/* Origin */

	jvec.Zpos = 0;		/* Do it now and don't do it again */
	origin = ppmap;
	for( ; nvecs--; vp++ ) {
		if( !*vp ) {
			if( width < 256 ) {
				/* sign extension is necessary */
				ox = HIBYTE(*++vp);
				oy = LOBYTE(*vp);
				nx = (double)(ox) * scale;
				ny = (double)(oy) * scale;

				jvec.Xpos = nx;
				jvec.Ypos = ny;

			}
			else {
				if(*(vp + 1) != 0 )
					goto jumpout;	/* Vector bonified */
				vp += 2;	/* This zero, and next */
				jvec.Xpos = nx = (ox = *vp++) * scale;
				jvec.Ypos = ny = (oy = *vp) * scale;

			}

			/* Move back to origin, move out to new position */
			penup();
			--nvecs;	/* Since we have to jump over 0 vector -- the reset marker */
			vecp = TIGdot( &jvec, &ppwrpln );	/* Map into writing plane */
			move2pt( origin.Xpos+vecp->Xpos, origin.Ypos+vecp->Ypos, origin.Zpos+vecp->Zpos );
			pendown();
			continue;
		}
jumpout:
		/* We do it this way to minimize truncation errors */

		if( width < 256 ) {
			/* Sign extension is necessary  */
			ox += HIBYTE(*vp);
			oy += LOBYTE(*vp);
 		}
		else {
			ox += *vp++;
			oy += *vp;
		}

		/* defines evaluation order properly, BJZ */
		nx += ((ox - nx/scale) * scale);
		ny += ((oy - ny/scale) * scale);
		jvec.Xpos = nx;
		jvec.Ypos = ny;

		vecp = TIGdot( &jvec, &ppwrpln );	/* Map into writing plane */
		move2pt( origin.Xpos+vecp->Xpos, origin.Ypos+vecp->Ypos, origin.Zpos+vecp->Zpos );
	}
	penup();
	move2pt(origin.Xpos, origin.Ypos, origin.Zpos);
}

/*
 *	GETCINFO( <character> )
 *      Note: font file has already been loaded into memory.
 *	Calls:	_getfc()
 */
struct cinfo *getcinfo(int c)	/* Get character information */
{
	register struct charstr *lstlmp;
	
	if((lstlmp = _getfc(c)) == (struct charstr*)NULL)
		return((struct cinfo *) NULL);
	return( &lstlmp->chinfo );  /* returns this if successful */
}


/*
 *	_GETFC( <character> )
 *
 *	Calls:	read(II)
 *		_find(), _getfont()
 *
 *	Returns:	<address of vector list>	If font is available
 *				NULL			No character
 */

static struct charstr *
_getfc(int c)	/* Get font-character into core */
{
	register int i;
	register struct fntabl *fntp;
	register struct charstr *charp;
 	struct fhdr *fhdrp;
	int cnt;

	void print_header();

	if((c < 0) || (c >= 0200)) {
		fprintf(stderr,"%d: Character out of range\n",c);
		return((struct charstr *)NULL);
	}

	fntp = &fonttab[ppfont]; 

	/* See if the font table is in memory */

	if(!fntp->fdfont) {  /* load font file into memory */
		if((fntp = _getfont()) == (struct fntabl*)NULL) {
			return((struct charstr *)NULL);
		}
	}
	
	memcpy(&ppfinfo[0], block[ppfont], sizeof(ppfinfo));	

	/*this routine done only for debugging purposes */
/*
	fprintf( stderr, "Font loaded: %s\n", fntp->fname );
	print_header(); 
*/
	fhdrp = &ppfinfo[c].finfo;
	i = (fhdrp->nvecs )*2;		/* A complete vector is an X and Y 
				   	   component packed as bytes .   */

	if( fhdrp->width > 255 )
		i <<= 1;	/* Don't use BYTEs but INTs */
	if( i == 0 ) {
		if( isprint(c) )
			fprintf(stderr,"(%c = 0%o):  No such character\n",c,c);
		else	fprintf(stderr,"0%o:  No such character index\n",c);
		return((struct charstr *)NULL);
	}

 	charp = (struct charstr *) malloc(sizeof(struct charstr)); 
 	if(charp == ((struct charstr *)NULL)) {
		fprintf(stderr, "malloc failed");
		return ((struct charstr *)NULL);
	}

	charp->fdata = (short int *)(block[ppfont] + ppfinfo[c].vaddr);

	charp->chinfo.font = ppfont;
	charp->chinfo.letr = c;
	charp->chinfo.ftinfo = *fhdrp;	/* Struct-to-struct move */


	/* Return vector address */
	return( charp );
}

/*
 *	_GETFONT( )
 *
 *	Calls:	 open(II), read(II)
 *
 *	Calls:	read(II), perror(III)
 *
 *	Called by:	putsymb(), strgln()
 *
 *	Returns:	<addr>	Success
 *			  NULL	Error
 */
static struct fntabl *_getfont( ) /* Make internal character table reflect current font */
{
	register struct fntabl *fntp;
	static struct stat *statBuf = 0;
	int fileStat;
	char name[100];

	if(!statBuf) statBuf = (struct stat *)malloc(sizeof(struct stat));

	/* NOTE:  This will initialize properly since `ppfont' is set to 0
	 *	  and `ppfindx' is set to -1 at compile time.
	 *        Font files are now kept in memory after being opened.
	 */
	fntp = &fonttab[ppfindx = ppfont]; /* ppfindx is set here */

	/* build font file name */
	if(!fntp->fname) {
		fprintf(stderr,"#%d: No such font\n",ppfont);
		return((struct fntabl *)NULL);
	}
	strncpy(name, FNTLIB, 100 - strlen(fntp->fname));
	strcat(name, fntp->fname);

	/* find size of the font file */
	fileStat = stat(name, statBuf);
	if(fileStat || (statBuf->st_size == 0)) {
		perror(name);
		return((struct fntabl *)NULL);
	}

	/* allocate enough memory space for font file */
 	if((block[ppfont] = (char *)malloc(statBuf->st_size)) == (char *)NULL) {
		fprintf(stderr, "malloc failed for font file: %d\n", ppfont);
		return(( struct fntabl *)NULL);
	}

	/* open font file for reading only */
	if((fntp->fdfont = fopen(name, "r")) == NULL) {
		perror(name);
		return((struct fntabl *)NULL);
	}
	
	/* read in entire font file  here */
	if(fread(block[ppfont], sizeof(char), statBuf->st_size, fntp->fdfont) < statBuf->st_size) {
		fprintf(stderr,"_getfont(): read failed for font %s\n",name);
		fntp->fdfont = (FILE *)NULL;	/* File is worthless - flag file descriptor */
		return((struct fntabl *)NULL); 
	}
	return( fntp );  	/* returns this on success */ 
}

void print_header()
{
  struct fdict *f;

  short *i;
  char *c;
  long *l;

  f = ppfinfo;  

  fprintf( stderr, "sizeof ppinfo: %d\n", sizeof ppfinfo );

  f = &ppfinfo[0];
  fprintf( stderr, "First header: vaddr = %ld width = %hd nvecs = %hd\n",
	 f->vaddr, f->finfo.width, f->finfo.nvecs );

  f = &ppfinfo[1];
  fprintf( stderr, "Second header: vaddr = %ld width = %hd nvecs = %hd\n",
	 f->vaddr, f->finfo.width, f->finfo.nvecs );

  f = &ppfinfo[2];
  fprintf( stderr, "Third header: vaddr = %ld width = %hd nvecs = %hd\n",
	 f->vaddr, f->finfo.width, f->finfo.nvecs );

  f = &ppfinfo[3];
  fprintf( stderr, "Fourth header: vaddr = %ld width = %hd nvecs = %hd\n",
	 f->vaddr, f->finfo.width, f->finfo.nvecs );

  f = &ppfinfo[4];
  fprintf( stderr, "Fifth header: vaddr = %ld witht = %hd nvecs = %hd\n",
	 f->vaddr, f->finfo.width, f->finfo.nvecs );

}









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
 *	09/06/83  LMO	Added change color to strings: %nC
 *	04/02/84  LMO	Added mode 3 for default centering of integers
 *	05/23/86  LMO	Fixed bug in padding character of _strconv()
 *	02/10/88  LMO	convert all byte oriented code to machine
 *			independent form.
 *	01/19/89  BJZ   Ported to sparc architecture.  This required
 *			making numerous small changes in how _pltstr(char *)
 *			parses its argument. 
 */
#include	<stdio.h>
#include	<ctype.h>
#include 	<math.h> 	/* Put math first to eliminate a peculiar problem. */
#include	"tig.h"

#define DEBUG 1
#define MAXARGS 50	
extern char *ncvt();
extern char *_TIGstf();
extern long aton();
char *_chstf();

#define	BFSIZE	256

#define	INIT	-1
#define	DEF_WID	8		/* Default field width */
#define	DEF_PDC	4		/* Default post decimal point digits */
#define	DEF_PAD	0		/* Default padding character */
#define	DEF_BAS	10		/* Default base */
#define	NSPACE	(5*ppfsize)	/* An en space */
#define	VERSP	12		/* Vertical space: 12*ppfsize */

#define	REL	001		/* Relative point size change */
#define	ARG1	002		/* Skew flags */
#define	ARG2	004
#define	ARG3	010


	/* Internal TIG string escapes */

#define	HORMOVE	0200
#define	VERMOVE	0201
#define	HALFUP	0202
#define	HALFDWN	0203
#define	NOSPC	0204	/* Overspace a character with the next one */
#define	CHGSKEW	0205
#define	CHGFONT	0206
#define	CHGSIZE	0207
#define	CHGKRN	0210	/* Change kearn space */
#define CHGCOLOR 0211	/* change color */

/*
 *	PLTSTR( <string pointer>, <argument list> )
 *
 *	Terminate argument list with (double)NULL.
 *
 *	Calls:	move(), _pltstr(), _strconv()
 */
pltstr(char *formatPtr, double *argPtr)
{
	register char *cp;
	int length;
	int argno = 0;  	 
	struct TIGvec jvec;
	double args[8];	 /* increase if necessary to support new arguments */

	jvec.Ypos = jvec.Zpos = 0;
		
	while(*argPtr != (double)0) {
		args[argno++] = *argPtr;
		argPtr++;
	}
		
	cp = _strconv(formatPtr, args, &jvec.Xpos, &length);
	jvec = *TIGdot( &jvec, &ppwrpln );
	move(jvec.Xpos, jvec.Ypos, jvec.Zpos);
	_pltstr(cp);
}

/*
 *	_STRCONV( <ASCII str. ptr>, &<argument list>, &<ctr. offst>, &<length> )
 *
 *	Calls:	dtoa(), ncvt(VI), newfont(),
 *		newkearn(), newsize(), newskew(), _chstf(), _TIGstf()
 *
 *	Returns:	<addr TIG-string>
 */

char *_strconv( strp, argp, offset, len )	/* Map ASCII str. into TIG str. */
register char *strp;
double *argp;
int *offset, *len;
{
	register char *cp;
	register int i;
	int j, length;		/* Length of current line */
	int savepsz, savefnt;
	int svxskew, svyskew, savekrn;
	int args[BFSIZE];
	int numArgs;

	/* Initialize */

	savefnt = ppfont;	/* Save present information */
	savepsz = ppfsize;
	svxskew = ppxskew;
	svyskew = ppyskew;
	savekrn = ppkearn + KRNOFF;

	*offset = 0;		/* Initialize returns */
	*len = 0;
	length = 0;

	for( ; *strp; strp++ ) {
		if( length > *len )
			*len = length;	/* Update */
		if(*strp == '%') {	/* Special option selected */
		char *c2p;
		char padchr, mode;
		int predec, pstdec;	/* digits before decimal pt, digits after */
		int width, padcnt, base;
		int decpt, sgnf;	/* Decimal point & sign flag F/ ncvt() */
		int expf;		/* Exponential form flag: 1 exp form */

			/* Initialize */
			mode = INIT;	/* Semaphore: +1, 0, -1 */
			base = DEF_BAS;
			pstdec = DEF_PDC;
			width = DEF_WID;
			padchr = DEF_PAD;

			i = 0;		/* Used as flag for skew and sign */
			if( *++strp == '^' ) {	/* Mark offset */
				strp++;
				*offset = -length;
			}

			if( *strp == ':' ) {	/* Store padding character */
				++strp;
				if ((*strp != '.') && (*strp != '^'))
					padchr = *strp++;
				while((*strp != '.') && (*strp != '^'))
					++strp;
				if( *strp == '^' ) {	/* Mark offset after padding character */
					mode = 0;
					++strp;
				}
				while(*strp++ != '.');
			}
			sgnf = 0;
			switch( *strp ) {
			case '-':
				--sgnf;
			case '+':
				i |= REL;
				strp++;
			}
			if( isdigit(*strp) ) {
				width = aton(strp,10,'9',&c2p);

				if( sgnf )
					width = -width;
				strp = c2p;
				i |= ARG1;	/* Flag for skew says xskew set */
			}

			if( *strp == '^' ) {	/* Mark is at decimal point */
				strp++;
				mode = 1;
			}

			if( *strp == '.' ) {
				pstdec = aton(++strp,10,'9',&c2p);
				strp = c2p;
				i |= ARG2;	/* Flag for yskew set */
			}

			if( *strp == '.' ) {
				base = aton(++strp,10,'9',&c2p);
				strp = c2p;
				i |= ARG3;	/* Third argument used */
			}

			if( *strp == '^' ) {	/* Mode at option end */
				mode = 2;
				strp++;
			}

			switch(*strp) {
			case 0:		/* Null termination -- format goof */
				continue;

		/*
		 * Numbers are handled in fixed field widths (field) allowing
		 * width-pstdec digits before the decimal point.  If a padding
		 * character is given, the balance of the field will be padded.
		 * If a decimal format is requested and overflow or underflow
		 * would result, a exponential format is used.  The field total
		 * width does not include the sign of the number if it is pre-
		 * sent.  For exponential forms, the exponent and its sign is
		 * also exempt to width restrictions.  Exponential forms always
		 * will make the mantissa have a single digit followed by
		 * pstdec digits.
		 * Engineer's formats obey all of the above rules but have as
		 * their foremost priority making the exponent a power of 3.
		 * Any number which begins after the decimal point will always
		 * be preceeded by a zero.
		 */

			case 'i':	/* Integer format */
				if( i&ARG2 )
					base = pstdec;
				pstdec = 0;	/* No pstdec in this form */
						/* dtoa() works since default is 'd' */
				if (mode == (char) INIT) mode = 3;  /* center integers */
			case 'E':	/* Eng. format: exponent mult of 3 */
			case 'd':	/* Decimal format [+-]###.#### */
			case 'e':	/* Exponential format [+-]#.####E+## */
				if(width <= 0)
					width = DEF_WID;
				if(pstdec < 0)
					pstdec = 0;

				if(width < pstdec)	/* Digits after can't be > */
					width = pstdec;
/*
fprintf(stderr, "argp=%x\n", argp);fflush(stderr);
fprintf(stderr, "*argp=%f\n", *argp);fflush(stderr);
fprintf(stderr, "base = %d\n", base);fflush(stderr);
fprintf(stderr, "width=%d\n", width);fflush(stderr);
fprintf(stderr, "pstdec=%d\n", pstdec);fflush(stderr);
fprintf(stderr, "padchr = %c\n", padchr);fflush(stderr);
fprintf(stderr, "strp = %x\n", strp);fflush(stderr);
fprintf(stderr, "*strp = %s\n", strp);fflush(stderr);
*/
				if((int)(cp=dtoa(*argp++,(double)base,width,pstdec,padchr,*strp,(int *)0)) == -1) {
					fprintf(stderr,"%d: base out of range\n",base);
					goto lcarat;
				}

				if( *cp == '-' )	/* Has a leading minus */
					_chstf(*cp++,&length);	/* Comes before padding */

				if( padchr )
					while( *cp == padchr )
						_chstf(*cp++,&length);

				if( !mode )	/* Record after padding character */
					*offset = -length;

				while( *cp != '.' && *cp )
					_chstf(*cp++,&length);

				if( mode == 1 )	/* Record position before period */
					*offset = -length;

				while( *cp )	/* Dump rest of line */
					_chstf(*cp++,&length);

				if( mode == 2 )
					*offset = -length;	/* If at end */
				if (mode == 3)
					*offset = -length/2;	/* middle */
				continue;
			case 's':	/* Change point size */
				if( i&REL ) width += ppfsize;
				newsize( width );
				numArgs = 3;
				args[0] = CHGSIZE;
				args[1] = ppfsize % 256;
				args[2] = ppfsize / 256;
				_TIGstf(numArgs, args);
				goto lcarat;
			case 'f':	/* Change font */
				newfont( width );

				numArgs = 2;
				args[0] = CHGFONT;
				args[1] = ppfont;
				_TIGstf(numArgs, args);
				goto lcarat;

			case 'C':	/* change color */
				if( !(i&ARG1) )	/* No color */
					width = 1;
				if( !(i&ARG2) )	/* No intensity */
					pstdec = 5;
				numArgs = 3;
				args[0] = CHGCOLOR;
				args[1] = width;
				args[2] = pstdec;
				_TIGstf(numArgs, args);
				goto lcarat;
			case 'S':	/* Change skew */
				if( !(i&ARG1) )	/* No xskew */
					width = ppxskew;
				if( !(i&ARG2) )	/* No yskew */
					pstdec = ppyskew;
				newskew( width, pstdec );
				numArgs = 3;
				args[0] = CHGSKEW;
				args[1] = width;
				args[2] = pstdec;
				_TIGstf(numArgs, args);
				goto lcarat;
			case 'k':	/* Change character kearn space */
				numArgs = 2;
				args[0] = CHGKRN;
				args[1] = width;
				_TIGstf(numArgs, args);
				goto lcarat;
			case 'b':	/* Backspace over last character */
				numArgs = 1;
				args[0] = NOSPC;
				_TIGstf(numArgs, args);
			case 'n':	/* Null character; to allow ^ marking
					 * at places where other letters are
					 * interpolated, e.g.: %^ndecimal
					 */
	lcarat:
				if( mode != (char) INIT )
					*offset = -length;
				continue;
			case 'h':	/* Horizontal motion */
				numArgs = 3;
				args[0] = HORMOVE;
				args[1] = width % 256;
				args[2] = width / 256;
				_TIGstf(numArgs, args);
				goto lcarat;
			case 'v':	/* Vertical motion */
				numArgs = 3;
				args[0] = VERMOVE;
				args[1] = width % 256;
				args[2] = width / 256;
				_TIGstf(numArgs, args);
				goto lcarat;
			case 'U':	/* Half-line vertical motion up */
				numArgs = 1;
				args[0] = HALFUP;
				_TIGstf(numArgs, args);
				goto lcarat;
			case 'D':	/* Half-line vertical motion down */
				numArgs = 1;
				args[0] = HALFDWN;
				_TIGstf(numArgs, args);
				goto lcarat;
			}
		}

		/* It's a simple character */

		switch(*strp) {
		case ' ':	/* A space */
			j = NSPACE + KSCALE(ppkearn);
			length += j;
			numArgs = 3;
			args[0] = HORMOVE;
			args[1] = j % 256;
			args[2] = j / 256;
			_TIGstf(numArgs, args);
			break;
		case '\b':	/* Backspace even though escape exists */
			numArgs = 1;
			args[0] = NOSPC;
			_TIGstf(numArgs, args);
			break;
		case '\t':	/* A tab:  NPSACE%8 */
			j = NSPACE + KSCALE(ppkearn);
			j = j*8 - length%(j*8);
			length += j;
			numArgs = 3;
			args[0] = HORMOVE;
			args[1] = j % 256;
			args[2] = j / 256;
			_TIGstf(numArgs, args);
			break;
		case '\n':	/* Newline:  Yep even this */
			j = -ppfsize * VERSP;
			numArgs = 3;
			args[0] = VERMOVE;
			args[1] = j % 256;
			args[2] = j / 256;
			_TIGstf(numArgs, args);
			length = -length;
			numArgs = 3;
			args[0] = HORMOVE;
			args[1] = length % 256;
			args[2] = length / 256;
			_TIGstf(numArgs, args);
			length = 0;
			break;
		default:		/* A simple character */
			_chstf( *strp, &length );
		}
	}

	if( length > *len )	/* Check one last time */
		*len = length;

	newkearn( savekrn );
	newskew( svxskew, svyskew );
	newsize( savepsz );
	newfont( savefnt );
	numArgs = 0;
	args[0] = 0;
	return(_TIGstf(numArgs, args));
}


/*
 *	_TIGSTF( <# of args>, <arglst... )
 *	NOTE: arglst must be chars stuffed into lowest byte of ints.
 *
 *	01/18/89	BJZ and BJR	Rewrote subroutine using varargs routine.
 *	
 *	Returns:	<buf-addr>	On flush (cmds = 0)
 *			    0		Normal stuff
 *			    1		Stuff has caused an overflow
 */

char * _TIGstf(int cnt, int *args)	/* Stuff into character buffer checking for overflow */
{
	int i;
	static int index = 0;		/* index into cbuf  */
	static int flag = {0};		/* Flag is set when overflow has occured */
	static char cbuf[BFSIZE];	/* Buffer which is filled */

	if(cnt <= 0) {
		cbuf[index] = 0;	/* Null terminate */
		flag = 0;		/* Reset the world */
		index = 0;
		return(cbuf);
	}
	
	if ((cnt + index) > (BFSIZE -1)) 		/*Overflow occurs ; no more stuffing */
		return((char *)(flag=1));
	
	/* load the arguments into the buffer */
	for(i = 0; i < cnt ; i++) cbuf[index++] = args[i];

 	return(0);
}


/*
 *	_CHSTF( <character>, &<length register> )
 *
 *	Calls:	getcinfo()
 *
 *	Returns:	<_TIGstf()>	Success
 *			   -1	No such character
 */

char * _chstf(letr, length)	/* Stuff a character */
register int letr;
register int *length;
{
	register struct cinfo *cinfop;
	int args[BFSIZE];
	int numArgs;

	if((cinfop = getcinfo(letr)) == (struct cinfo *)NULL) {
		return((char *)-1);	/* No character to play with */
	}

	*length += CSCALE(cinfop->ftinfo.width) + SSCALE(abs(ppxskew)) + KSCALE(ppkearn);
	numArgs = 1;
	args[0] = letr;
	return(_TIGstf(numArgs, args));
}


/*
 *	_PLTSTR( <string pointer> )
 *
 *	Calls:	getcinfo(), move(), newfont(), newkearn(),
 *		newsize(), newskew(), putsymb()
 */
void _pltstr(char *strp)		/* Output a TIG string */
{
	int inc;
	register int prevmov = {0};	/* Previous motion */
	register struct cinfo *charp;
	int nospc = {0};			/* No-space flag */
	int i;
	struct TIGvec jvec;
	char *temp;

	for( ; *strp; strp++ ) {
		/* Note: these cases can be misinterpreted if *strp is not cast properly */
		switch((unsigned int)(*strp & 0377)) {
		case HORMOVE:
 			jvec.Xpos = (unsigned int)(*++strp & 0377);
			jvec.Xpos += ((unsigned int)(*++strp & 0377)) * 256;
			jvec.Ypos = jvec.Zpos = 0;
			jvec = *TIGdot( &jvec, &ppwrpln );
			move( jvec.Xpos, jvec.Ypos, jvec.Zpos );
			break;
		case VERMOVE:
			jvec.Ypos =  (unsigned int)(*++strp & 0377);
			jvec.Ypos = ((unsigned int)(*++strp) & 0377) * 256;
			jvec.Xpos = jvec.Zpos = 0;
			jvec = *TIGdot( &jvec, &ppwrpln );
			move( jvec.Xpos, jvec.Ypos, jvec.Zpos );
			break;
		case HALFUP:
			jvec.Xpos = jvec.Zpos = 0;
			jvec.Ypos = 5*ppfsize;
			jvec = *TIGdot( &jvec, &ppwrpln );
			move( jvec.Xpos, jvec.Ypos, jvec.Zpos );
			break;
		case HALFDWN:
			jvec.Xpos = jvec.Zpos = 0;
			jvec.Ypos = -5*ppfsize;
			jvec = *TIGdot( &jvec, &ppwrpln );
			move( jvec.Xpos, jvec.Ypos, jvec.Zpos );
			break;
		case NOSPC:
			nospc++;
			break;
		case CHGSKEW:
			i = (unsigned int)(*++strp & 0377);
			newskew( i, ((unsigned int)(*++strp) & 0377));
			break;
		case CHGCOLOR:
			i = (unsigned int)(*++strp & 0377);
			newcolor( i, (unsigned int)(*++strp & 0377));
			break;
		case CHGFONT:
			newfont( (unsigned int)(*++strp & 0377 ));
	   		break;
		case CHGSIZE:
			i = (unsigned int)(*++strp & 0377); 
			i += 256 * (unsigned int)(*++strp & 0377);
			newsize( i );
			break;
		case CHGKRN:
			newkearn( (unsigned int)(*++strp & 0377));
			break;
		default:
			if( (charp=getcinfo((unsigned int)(*strp & 0377))) == (struct cinfo *)NULL)
				break;
			if( nospc ) {
				jvec.Xpos = -prevmov;
				jvec.Ypos = jvec.Zpos = 0;
				jvec = *TIGdot( &jvec, &ppwrpln );
				move( jvec.Xpos, jvec.Ypos, jvec.Zpos);	/* Move back a step */
				putsymb((unsigned int)(*strp & 0377)); 
				/* Find out which post-motion is greater so we can
				 * clear the character completely
				 */
				i = CSCALE(charp->ftinfo.width)/2
					+ SSCALE(abs(ppxskew));
				i += KSCALE(ppkearn);
				if( i > prevmov)
					prevmov = i;
				jvec.Xpos = prevmov;
				jvec.Ypos = jvec.Zpos = 0;
				jvec = *TIGdot( &jvec, &ppwrpln );
				move( jvec.Xpos, jvec.Ypos, jvec.Zpos );
				nospc = 0;
				break;
			}

			i = ( CSCALE(charp->ftinfo.width) + SSCALE(abs(ppxskew)) )/2;
			jvec.Xpos = i;
			jvec.Ypos = jvec.Zpos = 0;
			jvec = *TIGdot( &jvec, &ppwrpln );
			move( jvec.Xpos, jvec.Ypos, jvec.Zpos );
			putsymb((unsigned int)(*strp & 0377));      
			jvec.Xpos = prevmov = i + KSCALE(ppkearn);
			jvec.Ypos = jvec.Zpos = 0;
			jvec = *TIGdot( &jvec, &ppwrpln );
			move( jvec.Xpos, jvec.Ypos, jvec.Zpos );
		}
	}
}

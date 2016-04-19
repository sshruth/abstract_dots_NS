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
 *	11/6/85   LMO	Ported to VAX; corrected logic tests.
 *	02/14/89  BJZ	Ported to Silicon Graphics Iris Workstation.
 *			Now compatiable with Unix System V.
 *	02/15/89  BJZ	tigio(mode, tigiop) rewritten using setbuf().
 *	03/13/97  LMO	Fix output so (0,0) closes files
 */
#include	<stdio.h>
#include	"tig.h"
#include	"tigio.h"

struct TIGIO diskio = {0,0};
/* struct TIGIO pipeio = {IO_USE, (FILE *)&__iob[1]};*/
struct TIGIO pipeio = {IO_USE, stdout};
extern int errno;

/*
 *			 O U T P U T
 *
 *	This routine accepts two arguments.  The first argument is
 * a pointer to a string containing the name of a disk file into
 * which a copy of the output is to be placed, or a zero if no file
 * is to be made.
 *	The second argument is a pointer to a string containing the
 * name of the TIG interpreter routine which a concurrent output is
 * to be performed to, or a zero if no visual output is to made.
 *	If both arguments are zero, all output until the next NEWOUTPUT
 * call will be discarded.  This is more efficient than specifying
 * the file /dev/null.
 *
 *	When an argument is set to -1, the effect is to leave the
 * channel unchanged.  When an argument is 0, the channel is closed.
 */

void output(char * file, char *interp)
{

	/* Process disk portion */
	/* Flush output before changing Fildes */

	if( !file && diskio._ppflag & IO_USE ) {
		fclose(diskio._ppiop );
		diskio._ppflag = 0;
	}

	if( !interp && pipeio._ppflag & IO_USE ) {
		if (pipeio._ppiop == stdout) fflush(stdout);
		else pclose(pipeio._ppiop);

		pipeio._ppflag = 0;
	}
		

	/* Process for new file */

	if( file ) {
		if( (diskio._ppiop = fopen(file, "w")) == NULL ) {
			fprintf(stderr, "Error openning file: %s\n", file);
			exit(1);
		}
		else  diskio._ppflag = (IO_USE | IO_BUF);
	}


	/* Process pipe portion */

	if( interp ) {

		if( (pipeio._ppiop = popen(interp, "w")) == (FILE *)NULL) {
			fprintf(stderr, "\nError openning interpreter: %s\n", interp);
			exit(1);
		}
		else  pipeio._ppflag = (IO_USE | IO_BUF);
	}
	setTigListPtr(0);	/* reset the tig list pointer to 0 */
}



/*
 *	TIGIO( <mode>, &<I/O structure> )
 *	Kept for historical reasons.
 *	Calls:	setbuf(3S), fflush(3S)
 *	B. Zoltick, 	02-15-89
 */

tigio( mode, tigiop )	/* Change output channel characteristics */
register int mode;
register struct TIGIO *tigiop;
{
		
	if( tigiop->_ppflag & IO_USE )
		switch( mode ) {
		case BUFR:		/* Change to buffered output */
			tigiop->_ppflag |= IO_BUF; /* default is block buffered  */
			break;
		case NOBUF:		/* Make unbuffered */
			setbuf(tigiop->_ppiop, NULL);
			tigiop->_ppflag &= ~IO_BUF;
			break;
		case FLUSH:		/* Flush channel */
			fflush( tigiop->_ppiop );
		}
}


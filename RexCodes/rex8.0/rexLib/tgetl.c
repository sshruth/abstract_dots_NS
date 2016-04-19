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
 *	Function to return one line of input.  All REX processes should use
 * this routine to acquire input from tty, for root files are processed
 * here.  Input line is terminated by a '\n'.  When the comment delimiter
 * '~' is seen it and all chars after it are ignored until the newline
 * char.
 *
 *	Returns number of chars in input line including '\n';  line is null
 * terminated (null termination is not counted in return count).  Reports
 * any errors with 'rxerr()' and returns -1.  Arguments are pointer
 * to string to store input line, and pointer to end of receiving string to
 * protect against overflow.
 */

#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include "rexhdrs.h"

extern int rt_abort_read;

int tgetl(char *p, char *endp)
{
	INT_BLOCK_P i_br= i_b;
	int cnt;
	char *savp= p;
	int comment= 0;

	/*
	 * Check for bad arguments.
	 */
	if(p >= endp) {
		rxerr("tgetl(): Bad arguments passed to function");
		return(-1);	/* cannot null term; no room */
	}

	/*
	 * Decrement endp to guarantee room for null termination in case
	 * of overflow.
	 */
	for(endp--; ; p= (comment ? p : p+1)) {

	    if(p >= endp) {
		rxerr("tgetl(): Input line overflow");
		goto errout;
	    }
	    if(infd == 0) {		/* input is from tty */

		for(;;) {
		    if( (cnt= read(0, p, 1)) != 1) {
			if(cnt == -1) {

			    /*
			     * Signals cause return of reads to slow devices;
			     * try again if interrupted sys call error.
			     */
			    if(errno == EINTR) {
				continue;
			    }
			    else {
				rxerr("tgetl(): Error on read from tty");
				goto errout;
			    }
			}
			*p= '\0';	/* EOF seen */
		    }
		    break;
		}

	    } else {			/* input is from root file */
		if(rt_abort_read) {
		    *savp= '\0';
		    return(0);
		}
		for(;;) {
		    if(i_br->i_rtcx < 512) {
			*p= i_br->i_rtbuf[i_br->i_rtcx];
			break;
		    } else {		/* need to read more into buffer */

			/*
			 * Root files are buffered in much the same way
			 * as the standard i/o library buffers input;
			 * the input buffer is in the INT_BLOCK and shared
			 * by all processes.
			 *
			 * Each process reading the root file has opened
			 * it independently and therefore has a separate
			 * read pointer.  If my read pointer is not equal
			 * to the current read pointer do a seek.
			 * i_rtseekp points to the end of the 
			 * block currently in the shared buffer.
			 */
			if(lastrseek != i_br->i_rtseekp) {
			    if(lseek(infd, i_br->i_rtseekp, 0) == -1) {
				rxerr("tgetl(): Error on seek on root file");
				goto errout;
			    }
			}
			if((cnt= read(infd, &i_br->i_rtbuf, 512)) != 512) {
			    if(cnt < 0) {
				rxerr("tgetl(): Error on root read");
				goto errout;
			    }
			    i_br->i_rtbuf[cnt]= '\0';	/* EOF */
			}
			i_br->i_rtcx= 0;
			lastrseek= (i_br->i_rtseekp += 512);
			continue;
		    }
		}
	    }

	    if(*p == '\0') return(p - savp);	/* EOF */

	    /*
	     * i_rtcx must be incremented after
	     * test for EOF so that a subsequent call to tgetl
	     * will not return invalid chars after the EOF.
	     */
	    if(infd != 0) i_br->i_rtcx++;

	    /*
	     * Ignore anything after the comment delimiter '~'.
	     */
	    if(*p == '~') comment= 1;

	    if(*p == '\n') {		/* end of line */
		i_br->i_rtcln++;
		*++p= '\0';
		return(p - savp);
	    }
	}


    errout:
	*p= '\0';
	return(-1);
}

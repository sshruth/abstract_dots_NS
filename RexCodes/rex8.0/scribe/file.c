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
 * Analog, Event file creation.
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include "rexhdrs.h"
#include "scribe.h"

extern CHANNEL a_chan, e_chan;

char A_exists[] = "filecr(): Afile already exists";
char E_exists[] = "filecr(): Efile already exists";
char A_cerr[] = "filecr(): Cannot creat Afile";
char E_cerr[] = "filecr(): Cannot creat Efile";
char hd_err[] = "filecr(): Cannot write file header block";

int
filecr(char *astr)
{
	int size, sx;
	char *endp;
	int fd;
	char *argv[P_NARG];
	char file[P_ISLEN];
	char *errmsg;

	if(parse(1, astr, argv, P_NARG) != 1) {
		rxerr("filecr(): Bad filename");
		return(-1);
	}
	stufs(argv[0], file, &file[P_ISLEN]);

	/*
	 * Make sure there is room for A, E extensions.
	 * Find last name in pathname and store in INT_BLOCK.
	 */
	sx= size= strlen(file);
	if(size >= (P_ISLEN - 2)) {

	too_long:
		rxerr("filecr(): File name too long");
		return(-1);
	}
	for(sx-- ; sx >= 0; sx--) if(file[sx] == '/') break;
	sx++;				/* correct index */

	/*
	 * Name must fit in P_LFNAME chars reserving one char for A, E
	 * extensions.
	 */
	if((size - sx) > (P_LFNAME-1)) goto too_long;
	stufs(&file[sx], i_b->inthdr.i_name, &i_b->inthdr.i_name[P_LFNAME]);

	/*
	 * Attempt to open Afile.
	 */
	fd= analog_fd= event_fd= -1;
	endp= &file[size];
	*endp= 'A';
	*(endp+1)= '\0';
	if((fd= open(file, 0)) >= 0) {
		errmsg= A_exists;
		goto err_out;
	}
	if((analog_fd= creat(file, 0644)) < 0) {
		errmsg= A_cerr;
		goto err_out;
	}
	if(stufhdr(&a_chan, analog_fd, sizeof(ANALOGHDR)) < 0) {
		errmsg= hd_err;
		goto err_out;
	}

	/*
	 * Attempt to open Efile.
	 */
	*endp= 'E';
	if((fd= open(file, 0)) >= 0) {
		errmsg= E_exists;
		goto err_out;
	}
	if((event_fd= creat(file, 0644)) < 0) {
		errmsg= E_cerr;
		goto err_out;
	}
	if(stufhdr(&e_chan, event_fd, EVSIZE) < 0) {
		errmsg= hd_err;
		goto err_out;
	}

	/*
	 * Initializations.
	 */
	e_chan.filedes= event_fd;
	a_chan.filedes= analog_fd;
	i_b->seqnum_e = i_b->seqnum_a = 0;
	i_b->i_flags |= (I_FILEOPEN | I_NEWSAMPHDR);
	derror= 0;			/* clear possible previous error */
	r_b->r_wsdx= r_b->r_wslx= 0;	/* shouldnt be necessary, but just in
					   case */
	return(0);

err_out:
	if(fd >= 0) close(fd);
	if(analog_fd >= 0) {
		*endp= 'A';
		unlink(file);
	}
	if(event_fd >= 0) {
		*endp= 'E';
		unlink(file);
	}
	rxerr(errmsg);
	return(-1);
}

/*
 * Create Afile, Efile block 0 header.
 */
int
stufhdr(CHANNEL *pchan, int fd, int size)
{
	char head[512];
	char *hp= head, *ehp= &head[512];
	time_t tvec;

	fills('\0', 512, hp, ehp);
	*((int *)hp)= size;
	hp += 2;

	/*
	 * Format:
	 * 	file name \n
	 * 	date \n
	 *	REX ascii version num \n
	 */
	hp= stufs(i_b->inthdr.i_name, hp, ehp);
	*hp++ = '\n';
	tvec= time(NULL);		/* get time in seconds */
	hp= stufs(ctime(&tvec), hp, ehp);
	*hp++ = '\n';
	hp= stufs(i_b->i_rxver, hp, ehp);
	*hp++ = '\n';

	/*
	 * Write out entire block and initialize channel seek pointer.
	 */
	if(write(fd, head, 512) != 512) return(-1);
	pchan->fkey = tell(fd);
	return(0);
}

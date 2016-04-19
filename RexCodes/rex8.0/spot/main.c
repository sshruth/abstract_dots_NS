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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "spot.h"

FILE *outfd = {0};

void spot(void);

char infile[50];
main(int argc, char **argv)
{
	register char *ptr;
	FILE *ifd;
	char outfile[50],ifile[50];
	char *iptr;
	char *copystr(char *, char *);

	argv++;

	while((argc-- > 0) && (**argv == '-')) {
		ptr = *argv++;
		for (++ptr; *ptr; ptr++) {
			switch (*ptr) {
				default:
					fprintf(stderr,"bad flag: %c\n",*ptr);
					break;
			}
		}
	}

	if(argc--) {
		if(strcmp((iptr = copystr(infile, *argv++)) - 2,".d")) {
			fprintf(stderr,"invalid input name - must be 'name.d'\n");
			exit(1);
		}

		/* open the file for user included code */
		copystr(copystr(ifile, infile) - 2,".i.c");

/*
		unlink(ifile);
*/

	}
	else {
		fprintf(stderr,"use -- spot file.d [name.d.c]\n");
		fprintf(stderr,"file.d - input file, must be '.d'\n");
		fprintf(stderr,"name - optional output name, will have '.d.c' added\n");
		exit(1);
	}

	if(!argc) {
		copystr(copystr(outfile, infile),".c");
	}
	else {
		copystr(copystr(outfile, *argv++),".d.c");
	}

	if((ifd = freopen(infile,"r",stdin)) == 0) {
		fprintf(stderr,"spot: failed to open %s\n",infile);
		exit(1);
	}

	if((outfd = freopen(outfile,"w",stdout)) == 0) {
		perror("spot: failed to open output");
	}

	spot();

	exit(0);
}

/* copy string s2 to s1, return pointer s1 at end of string */
char *copystr(char *s1, char *s2)
{
	while(*s1++ = *s2++);

	return(s1 - 1);
}

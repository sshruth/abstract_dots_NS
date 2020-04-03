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
 * These routines are called from interrupt level, and must be compiled
 * -Wc,-zu and -Wc,-s.
 */

/*
 * Printing functions useful for debugging the lower level of int.
 *
 *	dputs()-	Prints null term string.
 *	dputchar()-	Prints single char.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <x86/inline.h>
#include "sys.h"

#define DB_BUF_SIZ  5000
char db_buf[DB_BUF_SIZ]= {'$', {0}};
char *db_fill= &db_buf[1];

/*
 * Pointers to local automatic storage must be far when routine is
 * called from interrupt level.
 */
void
/* dputs(char far *p) */
dputs(char *p)
{
	while(*p != '\0') {
	    dputchar(*p++);
	}
}

/*
 * If 'doutput_inmem' is true, results are stored in a memory
 * buffer instead of printing on screen.  If 'doutput_rot' is true,
 * the contents of the circular buffer rotate;  otherwise when it
 * is filled it no longer stores new data.
 * Note: when printing from lower level (interrupt level) results
 * HAVE to be stored in memory.  System call to print on screen cannot
 * be executed from interrupt level.
 */
int doutput_rot= 1, doutput_inmem= 0, doutput_tofile= 0;
char *doutput_filename= "/tmp/doutput_file";
extern int inside_int;

void
dputchar(char c)
{
	unsigned eflags;

	/* 	if(doutput_inmem || inside_int || ((pswget() & INTR_IF) == 0)) { */
	if(doutput_inmem || inside_int) {
		if(c == '\0') return;

		/* necessary because this routine may be re-entered
		 * if called from a signal handler
		 */
		if((eflags= pswget()) & INTR_IF) InterruptDisable();
		*db_fill++ = c;
		if(db_fill >= &db_buf[DB_BUF_SIZ]) {
			if(doutput_rot) db_fill= &db_buf[0];
			else db_fill--;
		}
		*db_fill= '\0';

		/* only reenable if originally ints were enabled */
		if(eflags & INTR_IF) InterruptEnable();
	} else {
		putc(c, stderr);
	}
}

/*
 * Called to print out contents of memory buffer.  If 'doutput_exit'
 * is true, will also exit program after completion of printout.
 */
int doutput_exit= 0;

void
doutput(int sig)
{
    int tmp;
    char *p= db_buf;
    FILE *dout;

    tmp= sig;

    /*
     * Find beginning of circular buffer.
     */
    p= db_fill;
    if(*p != '\0') {
	fprintf(stderr, "doutput: circular buffer scrambled\n");
	fflush(stderr);
	return;
    }
    while(*p == '\0') {
	if(++p >= &db_buf[DB_BUF_SIZ]) p= &db_buf[0];
    }
    if(doutput_tofile) {
	if((dout= fopen(doutput_filename, "w")) == NULL) {
	    fprintf(stderr, "doutput: cannot create output file");
	}
    } else dout= NULL;
    while(*p != '\0') {
	    if(dout == NULL) putc(*p, stderr);
	    if(dout != NULL) fputc(*p, dout);
	    if(++p >= &db_buf[DB_BUF_SIZ]) p= &db_buf[0];
    }
    fflush(stderr);
    if(dout != NULL) {
	fflush(dout);
	fclose(dout);
    }
    if(doutput_exit) exit(1);
}

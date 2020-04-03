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
 * Root read processing.
 */
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "rexhdrs.h"

/*
 * Globl variables used by all REX processes for roots.  Defined here
 * so that the definitions do not have to be duplicated in each process'
 * i.c file.  See menu.h for comments.
 */
FILE *rtout;
long lastrseek= 0L;
int infd= 0, echo= 1;

/*
 * Called from alert() to setup for reading root file.
 */
int
rt_read(void)
{
	extern int rt_abort_read;
	int fd;

	rt_abort_read= 0;
	if((fd= open(i_b->i_rtname, 0)) < 0) {
		rxerr("rt_read(): Cannot open root file");
		return(-1);
	}
	if(i_b->i_rtflag & RT_ECHO) echo= 1;
	else echo= 0;
	lastrseek= 0;
	infd= fd;
	return(0);
}

/*
 * Called to terminate reading of root file.
 */
int
rt_close(void)
{
	if(infd == 0) return(0);
	close(infd);
	infd= 0;
	echo= 1;
	return(0);
}

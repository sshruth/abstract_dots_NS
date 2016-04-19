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
 *	03/29/80  GET		Wrote subroutine.
 *	01/18/89  BJZ and BJR   Rewrote subroutine using varargs routine.
 *	02/12/89  BJZ		Incorporated I/O into this subroutine.
 *	11/22/91  LMO		try to speed up i/o
 */
#include <stdio.h>
#include <malloc.h>
#include "tig.h"
#include "tigio.h"

#define MAXARGS  1024
/*
 *			P U T P L T
 *
 *	This routine directs a character to the appropriate file
 * descriptors, as controled by the diskio or pipeio.  One copy may
 * be going to a file, another to a pipe.
 *	The first argument is a count of the number of following
 * arguments.  If count is negative, then it is the number of
 * arguments in an array of (short ints) referenced by
 * the next argument on the stack.
 */
char *tigList;
long tigListPtr = 0;
long tigListLength = 0;
long tigListFree = 0;

void putplt(short int cnt, short int *args)
{
	int i;

	if(!tigList) {
		tigListPtr = 0;
		tigListLength = TIGBLOCK;
		tigListFree = tigListLength - tigListPtr;
		tigList = (char *)malloc(tigListLength * sizeof(char));
		if(!tigList) {
			fprintf(stderr, "libtig putplt: Couldn't allocate space for tigList\n");
			exit(1);
		}
	}

	if(((-cnt) > MAXARGS) || (cnt > MAXARGS)) {
		fprintf(stderr, "libtig putplt: Bad count in (%d)\n", cnt);
		exit(1);
	}

	if(cnt < 0) {	/* If negative, points to argument list */
		cnt = -cnt;
		if(cnt > tigListFree) {
			tigListLength += TIGBLOCK;
			tigListFree = tigListLength - tigListPtr;
			tigList = (char *)realloc(tigList,  tigListLength * sizeof(char));
			if(!tigList) {
				fprintf(stderr, "libtig putplt: Couldn't re-allocate space for tigList\n");
				exit(1);
			}
		}
		for(i = 0; i < cnt; i++) {
			tigList[tigListPtr++] = args[i];
			tigListFree--;
		}
	}
	else {		/* if positive, arguments are on stack */
		if(cnt > tigListFree) {
			tigListLength += TIGBLOCK;
			tigListFree = tigListLength - tigListPtr;
			tigList = (char *)realloc(tigList,  tigListLength * sizeof(char));
			if(!tigList) {
				fprintf(stderr, "libtig putplt: Couldn't re-allocate space for tigList\n");
				return;
			}
		}
		for(i = 0; i < cnt; i++) {
			tigList[tigListPtr++] = args[i];
			tigListFree--;
		}
	}
}

char *getTigList(void)
{
	return(tigList);
}

void freeTigList(void)
{
	if(tigList) {
		free(tigList);
		tigList = (char *)NULL;
	}
}

int getTigListPtr(void)
{
	return(tigListPtr);
}

void clearTigList()
{
	memset(tigList, 0, tigListPtr);
	tigListPtr = 0;
}

void setTigListPtr(int p)
{
	tigListPtr = p;
}

void dumpTigList(void)
{
	if(diskio._ppflag & IO_USE) {
		fwrite(tigList, sizeof(*tigList), tigListPtr, diskio._ppiop);
	}
	if(pipeio._ppflag & IO_USE) {
		fwrite(tigList, sizeof(*tigList), tigListPtr, pipeio._ppiop);
	}
}

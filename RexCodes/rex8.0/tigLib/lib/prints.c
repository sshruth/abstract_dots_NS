/*
 *		User Library Routine	:::	=GET=
 *	P R I N T S	Print out formatted Strings
 *
 *	(C) Copyright, February 4, 1980	-- All rights Reserved
 *
 *	01/20/1989	BJZ	Fixed stack problems. Ported to sparc architecture.
 */
#include <stdio.h>
#include "tig.h"

extern char *ntoa();
extern long aton();

/*
 *	PRINTS( <fildes>, <master string>, <arguments> )
 *
 *	Calls:	write(II), ntoa(VI)
 *
 *	Returns:	<nchars>	Success
 *			  -1		Write error
 */

prints( fd, strp, args )	/* Print formatted strings */
int fd;
register char *strp;
int args;
{
	register char *cp;
	register int i;
	char *c2p;
	int count, nchars = {0}, width = {0};
	long il;
	union {
		int *intp;	/* An integer pointer */
		long *longp;	/* A long pointer */
	} type;			/* Used to increment `args' properly */

	type.intp = &args;
	((WORDS)&il)->hiword = 0;		/* Long used to get unsigned INTS */
	for(cp=strp; *cp; cp++) {
		if(*cp == '%') {
			i = cp - strp;
			if(write(fd,strp,i) != i)
				return(-1);
			nchars += i;
		sw:
			switch(*(strp = ++cp)) {	/* Option selected */
			case '.':		/* Max field width specified */
				width = aton( ++cp, 10, '9', &c2p );
				cp = --c2p;
				goto sw;
			case 'c':		/* A single character */
				c2p = (char *) type.intp++;
				count = 1;
				break;
			case 'd':		/* Decimal integer */
				c2p = ntoa( (long)*type.intp++, 10, &count);
				break;
			case 'o':		/* Octal integer */
				((WORDS)&il)->loword = *type.intp++;
				c2p = ntoa( il, 8, &count);
				break;
			case 's':		/* String */
				count = 0;
				for(c2p =(char *) *type.intp++; c2p[count]; count++);
				break;
			case 'u':		/* Unsigned Decimal */
				((WORDS)&il)->loword = *type.intp++;
				c2p = ntoa( il, 10, &count);
				break;
			case 'x':		/* Hexidecimal integer */
				((WORDS)&il)->loword = *type.intp++;
				c2p = ntoa( il, 16, &count);
				break;
			case 'D':		/* Decimal long */
				c2p = ntoa( *type.longp++, 10, &count);
				break;
			case 'O':		/* Octal Long */
				c2p = ntoa( *type.longp++, 8, &count);
				break;
			case 'X':		/* Hex Long */
				c2p = ntoa( *type.longp++, 16, &count);
				break;
			default:
				continue;
			}
			if(width) {
				count = width;
				width = 0;
			}
			if(write(fd,c2p,count) != count)
				return(-1);
			nchars += count;
			strp++;		/* Don't want option char printed */
		}
	}
	i = cp - strp;
	if(write(fd,strp,i) != i)
		return(-1);
	return(nchars+i);
}

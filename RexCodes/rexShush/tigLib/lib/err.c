/*
 *	ERR( <string pointer>, <exit code> )
 *
 *	Calls:	exit(II), write(II)
 */

err( sptr, value )		/* Print error message and exit */
register char *sptr;
register int value;
{
	register char *cp;

	for(cp=sptr; *cp; cp++);
	*cp++ = '\n';	/* Newline terminate where NULL used to be */
	write(2,sptr,cp-sptr);
	exit(value);
}

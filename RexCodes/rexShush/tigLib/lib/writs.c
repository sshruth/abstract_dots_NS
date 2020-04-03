#

/*
 *	WRITS( <channel>, <string pointer> )
 *
 *	Calls:	write(II)
 *
 *	Returns:	write()
 */

writs(fd,ptr)	/* Write string on channel */
register int fd;
register char *ptr;
{
	register char *cp;

	for(cp=ptr; *cp; cp++);
	return(write(fd,ptr,cp-ptr));
}

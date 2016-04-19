
/*
 *	STANDARD HEADER			GETC(3) / PUTC(3) / FOPEN(3) / FFLUSH(3)
 */

struct iobuf {
	int fd;
	int xleft;
	char *cptr;
	char cbuf[512];
};

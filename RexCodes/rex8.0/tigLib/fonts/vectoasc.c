/*
 *	SYSTEM SUPPORT PROGRAM		TIG		VECSTAT
 *
 *	George E. Toth				May 16, 1980
 *	
 *	B. Zoltick				Oct 22, 1989
 *	L. Optican				Dec 6, 1989
 *
 * 	Program to dump in ASCII a vector font table used by TIG
 */

#include	<stdio.h>
#include	<fcntl.h>
#include	<ctype.h>
#include	"/usr/local/include/tig.h"

extern int errno;

struct fdict chars[NCHARS];

main(argc, argv, envp)
int argc;
char **argv;
char **envp;
{
	int fd;		/* file descriptor */
	register i;

	if(argc < 2) {
		fprintf(stderr, "Usage:\nvectoasc font_file\n");
		exit(0);
	}

	if((fd = open(argv[1], O_RDONLY)) < 0) {
		fprintf(stderr, "Unable to open font file: %s\n", argv[1]);
		exit(1);
	}

	if(read(fd, chars, sizeof(chars)) != sizeof(chars)) {
		fprintf(stderr, "Premature EOF for header\n");
		exit(1);
	}

	for(i = 0; i < NCHARS; i++) {
if(isprint(i)) fprintf(stderr, "char = %c vaddr = %ld nvecs = %d\n", i, chars[i].vaddr, chars[i].finfo.nvecs);
		if(!chars[i].vaddr) continue;

		printf("\033%o:", i);
		if(isprint(i)) printf(" %c\n", i);
		else printf("\n");

		printf(" %d\n",chars[i].finfo.width);
		lseek(fd,chars[i].vaddr,0);
		dump(fd, chars[i].finfo.nvecs,  chars[i].finfo.width);
	}

	exit(0);
}

dump(fd, nvecs, width)	/* Dump ASCII out, make sure nvec vectors */
int fd;
int nvecs;
register int width;
{
	register int j, k;
	short int buf[256];
	
/*	printf("nvecs = %d\n", nvecs); */
	if(width > 255)	nvecs <<= 1;	/* Then integers and not bytes/vector */

	for(; nvecs > 0; nvecs -= j) {
		if(nvecs > 256)	j = 256;
		else j = nvecs;
		if(read(fd,buf,j*sizeof(short int)) != j*sizeof(short int)) {
			for(; nvecs-- > 0; printf("\t0,0\n"));
			return;
		}
		if(width < 256) {
			for(k=0; k < j; k++)
				printf("\t%d,%d\n",HIBYTE(buf[k]),LOBYTE(buf[k]));
		}
		else for(k=0; k < j; k += 2)
				printf("\t%d,%d\n",buf[k],buf[k + 1]);
	}
}

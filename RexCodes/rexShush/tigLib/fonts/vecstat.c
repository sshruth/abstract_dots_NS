

/*
 *	SYSTEM SUPPORT PROGRAM		TIG		VECSTAT
 *
 *	George E. Toth				May 16, 1980
 *	
 *	B. Zoltick				Oct 22, 1989
 *
 *	This program reports the statistics about a vector font.
 */

#include	<stdio.h>
#include	<fcntl.h>
#include	<ctype.h>
#include	"/usr/local/include/tig.h"

struct fdict store[NCHARS];

main(argc, argv, envp)
int argc;
char **argv;
char **envp;
{
	int d;		/* file descriptor */
	register int i;
	
	if (argc < 2) {
		fprintf(stderr, "Usage:\nvecstat font_file\n");
		exit(1);
	}
	
	if ( ( d = open(argv[1], O_RDONLY) ) < 0) {
		fprintf(stderr, "Unable to open font file: %s\n", argv[1]);
	}

	if( read(d, store, sizeof(store)) != sizeof(store) ){
		fprintf(stderr, "Premature EOF on header\n");
		exit(1);
	}
	
	for( i=0; i < NCHARS; i++ ) {
		if( isprint(i) )
			putchar(i);
		else	printf("%o",i);
		if( store[i].vaddr == 0 ) {
			fprintf(stderr, "Missing character\n");
			continue;
		}
		printf("\twidth:  %-hd\tnvecs:  %-hd\tvaddr:  %-D\n",store[i].finfo.width,store[i].finfo.nvecs,store[i].vaddr);
	}

	exit(0);
}

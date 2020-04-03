#

/*
 *	Old skelatal font display system
 *
 *	Usage: oldfntdis <pos>
 *
 *	To compile:	cc -n -O oldfntdis.c -lu -o oldfntdis
 */

struct {
	char ht;
	char vof;
	char width;
	char hof;
	char buf[512*3];
} fbuf;

extern int errno;

main(argc,argv)
int argc;
char **argv;
{
	register int i, j, k;
	int height, afd;

	if( (afd=open("/lib/vers/fonts",0)) < 0 ) {
		perror( "/lib/vers/fonts" );
		exit(errno);
	}

	if( argc != 2 )
		err("Usage: oldfntdis <position>",75);

	seek( afd, 3*atoi(argv[1]), 3 );
	if( read(afd, &fbuf, 4 ) != 4 ) {
		perror("Can't read");
		exit(errno);
	}

	k = ((fbuf.width&0377)+7)&~07;
	height = fbuf.ht + fbuf.vof;
	read( afd, fbuf.buf, height*(k>>3) );

	for( i=0; i < height; i++ ) {
		for( j=0; j < (fbuf.width&0377); j++ )
			if( ibitad( fbuf.buf, i*k + j, 0) )
				putchar('#');
			else	putchar(' ');
		putchar('\n');
	}

	printf("Height, Baseline height: %d,%d\n",fbuf.ht+fbuf.vof,fbuf.ht);
	printf("Width, H-offset: %d,%d\n",fbuf.width&0377,fbuf.hof);

	exit(0);
}

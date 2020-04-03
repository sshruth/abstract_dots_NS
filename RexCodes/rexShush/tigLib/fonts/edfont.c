/*
 *	Program to read ASCII table generated from vectoasc
 *
 *	B. Zoltick; December 4, 1989
 */

#include <stdio.h> 
#include <ctype.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/file.h>
#include "/usr/local/include/tig.h";

extern int errno;
FILE *fdopen();
short int getnum();

char fname[100];

main(argc,argv, envp)
int argc;
char **argv;
char **envp;
{
	register int c;
	short int x, y;
	int fdin, fdout;
	FILE *fin, *fout;	/* associated streams with fildes */

	if( argc < 2 ) {
		fprintf(stderr,"Usage:\nedfont ascii_font_file\n");
		exit(0);
	}
	
	strcpy(fname, argv[1]);
	strcat(fname, ".mod");
	
	if( (fdin=open(argv[1], O_RDONLY )) < 0 ) {
		perror(argv[1]);
		exit(errno);
	}
	
	if( (fdout=open(fname, O_WRONLY|O_CREAT, 0644 )) <0) {
		perror(fname);
		exit(1);
	}

	fin  = fdopen(fdin, "r");		/* streams needed for stdlib */
	fout = fdopen(fdout, "w"); 

	
	while((c=getc(fin)) != EOF) {
		putc(c, fout);
		switch(c) {

		case '\t':		/* All lines begin with tabs */
			x = getnum(10, fin);
			y = getnum(10, fin);
			
			fprintf(fout, "%d,%d", x, y);
			break;
		default:
			putc(c, fout);
		}
	}

	fflush(fout);
	exit(0);
}

short int
getnum(base, fin)		/* Get a number in base `base' */
int base;
FILE *fin;
{
	int i=0, c, neg =0;

start:
	while( isdigit(c = fgetc(fin)) ) {
		i *= base;
		i += c - '0';
	}
	if(c == '-') {
		neg = 1;
		goto start;
	}
	if(c == '+') {
		neg = 0;
		goto start;
	}
	if (neg) i = -i;

	return(i);
}

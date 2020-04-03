/*
 *	Program to read ASCII data and generate a font table for TIG
 *
 *	GET; April 3, 1980
 *	B. Zoltick; December 4, 1989
 *	L. Optican; correct bytes; Dec 6, 1989
 */

#include <stdio.h> 
#include <ctype.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/file.h>
#include </usr/local/include/tig.h>

extern int errno;
extern off_t lseek();
FILE *fdopen();
short int getnum();

char fname[100];
struct fdict chars[NCHARS] = { 0 };

main(argc,argv, envp)
int argc;
char **argv;
char **envp;
{
	register int c;
	short int width, nvecs = 0;
	short int x, y;
	int fdin, fdout;
	FILE *fin, *fout;	/* associated streams with fildes */
	static short int index = 0;

	if( argc < 2 ) {
		fprintf(stderr,"Usage:\nasctovec vec-file\n");
		exit(0);
	}
	
	strcpy(fname, argv[1]);
	strcat(fname, ".f");
	
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

	/*
	 * reserve space for chars[] at front of file
	 */
	if( fseek(fout, (long)(sizeof(chars)), 0) < 0) {
		perror(fname);
		exit(errno);
	}
	
	while((c=getc(fin)) != EOF) {
		switch(c) {
		case ' ':		/* Width number follows */
			chars[index].finfo.width = width = getnum(10, fin);
			if(width > 256) fprintf(stderr, "width = %d\n", width);
			break;
		case '\t':		/* All lines begin with tabs */
			x = getnum(10, fin);
			y = getnum(10, fin);
#ifdef SGI
			if( width < 256 ) {
				putc(x, fout);
				putc(y, fout);
			}
			else {
/*				putw(x, fout);
				putw(y, fout);
*/
			}
#else
			if( width < 256 ) {
				putc(y, fout);
				putc(x, fout);
			}
			else {
/*				putw(y, fout);
				putw(x, fout);
*/
			}
#endif
			nvecs++;
			break;
		case '\033':		/* ESC plus a number */
			chars[index].finfo.nvecs = nvecs;
			nvecs = 0;
			index = getnum(8, fin);
			if (index > NCHARS) {
				fprintf(stderr, "ESC index = %d\n", index);
				exit(1);
			}
			while(getc(fin) != '\n');
			chars[index].vaddr = ftell(fout);
			break;
		default:
			chars[index].finfo.nvecs = nvecs;
			nvecs = 0;
			index = c;
			if (index > NCHARS) {
				fprintf(stderr, "index = %d\n", index);
				exit(1);
			}
			while(getc(fin) != '\n');
			chars[index].vaddr = ftell(fout);
		}
	}
	chars[index].finfo.nvecs = nvecs;

	rewind(fout);
#ifdef OLD
	if (rewind(fout) < 0) {	/* rewind */
		perror(fname);
		exit(errno);
	}
#endif
	if (fwrite(chars,sizeof(*chars), NCHARS, fout) < 0) {
		fprintf(stderr, "Error writing file: %s\n", fname);
		exit(1);
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

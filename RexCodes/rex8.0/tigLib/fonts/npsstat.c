#

/*
 *	SYSTEM DEVELOPMENT PROGRAM		TIG		NPSSTAT
 *
 *	George E. Toth				May 16, 1980
 *
 *	This program reports the statistics about a NPS vector font.
 *
 *	To compile:		cc -n -O npsstat -lu -o npsstat
 */

#include	<ctype.h>
#include	<tig.h>

struct {
	int rwidth, lftkrn, rft, drc;		/* See page 38 of thesis */
	char buf[512*3];
} ofnt;

struct {
	char cwidth;
	char block;
	int coffset;
} header[128];		/* Header of NPS fonts */

struct {
	int ftht;	/* Font height */
	int mxcwid;	/* Max char width */
	int loght;	/* logical height; see page 39 */
} fntdim;		/* Font's dimensions */

main()
{
	register int i;

	if( read(0, header, sizeof header ) != sizeof header	||
	    read(0, &fntdim, sizeof fntdim ) != sizeof fntdim
	  )
		err("Premature EOF on header",75);

	for( i=0; i < NCHARS; i++ ) {
		if( isprint(i) )
			putchar(i);
		else	printf("%o",i);
		printf(": ");
		if( header[i].cwidth == 0 ) {
			printf("Missing character\n");
			continue;
		}
		printf("w: %d",header[i].cwidth);
		printf(" Blk,Byt: %d,%d\n",header[i].block,header[i].coffset);
	}

	printf("Font height, Logical Height, Max-width: %d,%d,%d\n",
		fntdim.ftht,fntdim.loght,fntdim.mxcwid);
	exit(0);
}

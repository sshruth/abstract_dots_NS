/*
 *	DISFNT
 *
 *	Display an entire TIG font, reporting those which are not present
 *	and placing a small roman `x' in their place.  Draw the baseline
 *	in another color.  Make the characters 1 inch high.
 *
 *	Brad Zoltick, modified from GET,	25Nov89
 *
 *	Usage:	disfnt <font #>
 */

#include	<stdio.h>
#include	"/usr/local/include/tig.h"

main(argc,argv, envp)
int argc;
char **argv;
char **envp;
{
	register int i, j;
	int font;
	struct cinfo *getcinfo();
	
	if( argc != 2 ) {
		
		fprintf(stderr, "Usage:\ndisfnt <font #>\n");
		exit(0);
	}

	newfont(font=atoi(argv[1]));
	newsize(30);
	newkearn(40);
	move2pt(300, 300, 0);

	for(i=0; i < 128; i++) {
		if(ppmap.Xpos > 7000) {
			move2pt(8500, ppmap.Ypos, 0);
			pendown();
			move2pt(0, ppmap.Ypos, 0);
			penup();
			if(ppmap.Ypos > 5500) {
				newform();
				move2pt(300, 300, 0);
			}
			else	move(300, 1500, 0);
		}
		if(getcinfo(i) == (struct cinfo *)NULL) {
			move( 250, 0, 0 );
			marker( 'x' );
			move(250, 0, 0);
			continue;
		}

		j = CSCALE(ppcinfo.ftinfo.width) + KSCALE( ppkearn );
		j >>= 1;
		move(j +300, 0, 0);
		putsymb(i);
		move(j, 0, 0);
	}

	move2pt(8500, ppmap.Ypos, 0);
	pendown();
	move(-8500, 0, 0);
	penup();

	exit(0);
}

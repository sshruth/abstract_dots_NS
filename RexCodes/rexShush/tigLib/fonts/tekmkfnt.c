#

/*
 *	Make an ASCII vector file in the ASCTOVEC format to make
 *	figures for the TIG fonts.
 *
 *	Usage: tekmkfnt <afile> <fnt pos> <cwidth> [<fntref #> <ref char>]
 *
 *
 *	To compile:	cc -n -O tekmkfnt.c -lg -lm -lu -o tekmkfnt
 */

#include	<stty.h>
#include	<tig.h>
#include	<vector.h>

extern int errno;
extern int fix();
extern long aton();

struct iobuf Iob_in;

#define	getchar()	getc(&Iob_in)
#define	getwd()		getw(&Iob_in)		/* Does not work in RAW mode */

#define	TIG2TEK(x)	((x)*64L/1000)	/* TIG points to TEK points */

struct sttybuf save;

main(argc,argv)
int argc;
char **argv;
{
	register int width, c, afd;
	struct sttybuf work;

	if( argc != 4 && argc != 6 )
		err("Usage: tekmkfnt <afile> <fnt pos> <cwidth> [<fntref #> <refchar>]", 75);

	if( (afd=open(argv[1],1)) < 0 && (afd=creat(argv[1],0644)) < 0 ) {
		perror(argv[1]);
		exit(errno);
	}

	seek( afd, 0, 2 );
	if( argv[2][1] == 0 )
		prints(afd,"%c:\n",*argv[2]);
	else	prints(afd,"\0330%o:\n",(int)aton(argv[2],8,'9',0));
	prints(afd," %d\n",width=aton(argv[3],10,'9',0));

	writs(1,"!cle\r!era w\r!wor 31\r!gra 1,31\r");

	/* Draw working boundaries */

	writs(1,"!col c4\r!lin 4\r!vec 0,110,511,110\r!vec 256,0,256,500\r");
	writs(1,"!col c1\r!lin 1\r");
	width <<= 1;		/* width*2 */
	prints(1,"!vec %d,10,%d,10\r",256-width,256+width);
	prints(1,"!rve 0,0,0,400,%d,0,0,-400\r",-2*width);
	prints(1,"!col c6\r!lin 2\r!vec %d,310,%d,310\r",256-width,256+width);
	writs(1,"!lin 1\r");

	if( argc == 6 )
		display(atoi(argv[4]),argv[5]);

	signal(2,&fix);
	signal(3,&fix);
	gtty(0,&save);
	work = save;
	work.st_mode |= RAW;
	stty(0,&work);

	writs(1,"!col c2\r!lea f12 /!ink y/\r!lea f11 /!ink n/\r!ena\r");

	/* Now gather information */

	while( (c=getchar()) == '!' ) {
		int oldx, oldy;
		int x, y, type;

		getchar();		/* get `DAT 03,' */
		getchar();
		getchar();
		getchar();
		getchar();
		getchar();
		if( getchar() != ',' )
			finish("Phase error",77);
		type = getnum();	/* pad 5 = 165; PT = 171 */
		x = (getnum()-256) * FNT_REF/300;
		y = (getnum()-110) * FNT_REF/300;
		getnum();
		if( type == 171 ) {
			writs(afd,"\t0,0\n");
			oldx = oldy = 0;
		}
		prints(afd,"\t%d,%d\n",x-oldx,y-oldy);
		oldx = x;
		oldy = y;
	}

	finish(0,0);
}

display( font, charp )	/* Display an old font */
int font;
char *charp;
{
	register int *ip, i;
	register struct cinfo *cinfp;

	newfont( font );
	if( charp[1] == 0 )
		i = charp[0];
	else	i = aton(charp,8,'9',0);

	if( (cinfp=getcinfo(i)) == -1 )
		err("Reference character non-existent\n",75);

	ip = cinfp+1;		/* Data lies after it */
	for( i=0; i < cinfp->ftinfo.nvecs; i++ ) {
		if( ip[i] == 0 ) {
			if( i > 0 )
				writs(1,"\r");
			writs(1,"!col c7\r!vec 256,110,256,110\r!col c3\r!rvec");
			continue;
		}
		prints(1," %d,%d",(int)(ip[i].X*300L/FNT_REF),
				  (int)(ip[i].Y*300L/FNT_REF));
	}
	writs(1,"\r");
}

getnum()
{
	register int i;

	i = getchar() - '0';
	i *= 10;
	i += getchar() - '0';
	i *= 10;
	i += getchar() - '0';
	getchar();		/* Remove comma or semi */

	return(i);
}

fix()
{

	finish("Signal death",50);
}

finish(cp,code)
register char *cp;
register int code;
{

	writs(2,"!disa\r");
	stty(0,&save);
	if(cp)
		writs(2,cp);
	exit(code);
}

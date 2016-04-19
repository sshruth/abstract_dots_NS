#include <stdio.h>

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

/*
 * cnv.c   Make a .chr file from a .spec file (LeFebvre format,
 *		on standard input) and Hershey data in Hurt
 *		format in (default) file occidental.
 * This code is adapted from hershey.c by James Hurt
 * (translated by Pete Holzmann)
 * and from hfont.c by William LeFebvre.
 *		--Greg Lee, 10/12/87
 */

   	FILE	*fopen();

      /* some variables to record the largest bounding rectangle of
         the displayed characters. Printed when all finished. */

      int minx = 999;
	int miny = 999;
	int maxx = -999;
	int maxy = -999;

/*    .. font data file name */
      char name[80];

/* readspec globals: */

# define  Off   0
# define  On	1

# define  No	0
# define  Yes	1

# define  MAX_ORD	4000		/* maximum Hershey font number */

char   input[256];
char	order[MAX_ORD];
int    left_max, right_max;
int    up_max, down_max;
int    vtrans;
double x_factor;
double real_x_factor;
double x_trans;
FILE   *vecfont;

int  left_ex;
int  map_to_char;
int  baseline;
int  hi_char = 168;

int ytop = 22;
int base = 9;
int xheight = 0;

char verbose = No;

char *gets();
char *vgets();
char *index();
char *process_option();


/*
 * scanint: a function to scan an integer, using n characters of
 *          the input file, ignoring newlines. (scanf won't work
 *          because it also ignores blanks)
 */
int scanint(file,n)
FILE *file;
int n;
{
char buf[20];
int i,c;

   for (i=0;i<n;i++){
      while ((c = fgetc(file)) == '\n') ; /* discard spare newlines */
      if (c == EOF) return(-1);
       buf[i] = c;
   }
   
   buf[i] = 0;
   return(atoi(buf));
}


int skipflag = 1; /* 1 if next draw is 'pen up' */
int cocnt = 2;
int oldx,oldy;

static void
skip()
{
skipflag = TRUE;
	printf(" Pen Up\n   ");
	cocnt = 0;
}

static void
draw(newx,newy)
int newx,newy;
{
	if (cocnt > 8) {
		printf("\n   ");
		cocnt = 0;
	}
	printf("(%d,%d)", newx, -newy);
	cocnt++;
}

/*
 * The main program...
 */

main(argc,argv)
int argc;
char **argv;
{
/*    .. font data   */
      char line[2][256];
      int x,y;
      float col,row;
/*    .. which data point and which character */
      int     ipnt,ich,nch,i,ichar;
	int	letter;

/*
    	if (argc != 2) {
         printf("usage: cnv file\n");
         exit(1);
      }
*/
	ytop = 22;
	base = 9;

	readspec();

	printf("Height(%d)\n", ytop);
	if (xheight) printf("x Height(%d)\n", xheight);
	if (base) printf("Base(%d)\n", base);

/*		     .. loop per character */
      while (TRUE) {

/*		     .. read character number and data */
      if ((ich = scanint(vecfont,5)) < 1) {
            exit(0);
      }

	if (ich >= MAX_ORD) {
	fprintf(stderr, "Hershey data has big character number");
	exit(1);
	}

      nch = scanint(vecfont,3);

      for (i=0; i<nch;i++) {
         if ((i==32) ||(i==68) ||(i==104) ||(i==140)) fgetc(vecfont); /* skip newlines */
         line[0][i] = fgetc(vecfont);
         line[1][i] = fgetc(vecfont);
      }
      fgetc(vecfont);

  if ( (letter = order[ich]) != 0)
  {
/*		     .. identify character */
	if (letter < ' ') printf("^%c': ", letter + 64);
	else if (letter >= 127) printf("\\%o: ", letter);
	else printf("'%c': ", letter);


/*		     .. draw left and right lines */
/*		     .. Note: this data can be used for proportional spacing */

      x=(int)line[0][0] - (int)'R';
      y=(int)line[1][0] - (int)'R';

/*      skip();
      draw(x,-10);draw(x,10);
      skip();
      draw(y,-10);draw(y,10);
*/
	printf("Bounds(%d,%d)", x, y);
	if (letter == ' ') {
	    draw(x,0);
	    draw(y,0);
	}

/*		     .. first data point is a move */
/*      skip();*/
/*		     .. loop per line of data */
    for (ipnt=1;ipnt<nch;ipnt++) {

/*		     .. process vector number ipnt */
      if (line[0][ipnt] == ' ') {
/*		        .. next data point is a move */
         skip();
      } else {
/*		        .. draw (or move) to this data point */
         x=(int)line[0][ipnt] -(int) 'R';
         y=(int)line[1][ipnt] -(int) 'R';
         if (x < minx) minx = x;
         if (x >maxx) maxx = x;
         if (-y < miny) miny = -y;
         if (-y >maxy) maxy = -y;
/*		        .. Note that Hershey Font data is in TV coordinate system */
		   draw(x,-y);
      }
    } /* for loop */
/*		     .. end of this character */
	printf(" End Char\n");
	cocnt = 2;
   } /* end if */

   } /* while true */
}

/*
 *  Read a .spec file
 *  adapted from hfont.c, which was
 *  written by William LeFebvre, LCSE, Rice University
 */

readspec()

{
    int i;
    int j;
    int arg;
    int bytecnt;
    int pt_size = 0;
    int char_num;
    int space_char;
    int filesize;
    unsigned char bytemask;
    unsigned char byte;
    char *inp;
    char *p;
    static char repertory[256] = "occidental";
    static char pen_file[256] = "pen.round";
    static char buff[256];
    struct dispatch *character;

    /* read options from stdin */

    while (vgets(input) != NULL && strcmp(input, "charset") != 0)
    {
	/* get the option name */
	if ((p = index(input, ' ')) != 0)
	{
	    *p = '\0';
	}
	while (*(++p) == ' ');

	/* string switch on option names */
	if (strcmp(input, "repertory") == 0)
	{
	    strcpy(repertory, p);
	}
	else if (strcmp(input, "maxchar") == 0)
	{
	    hi_char = atoi(p);
	}
	else if (strcmp(input, "pen") == 0)
	{
	    strcpy(pen_file, p);
	}
	else if (strcmp(input, "aspect") == 0)
	{
	    x_factor *= atof(p);
	}
	else if (strcmp(input, "base") == 0)
	{
	    base = atoi(p);
	}
	else if (strcmp(input, "ytop") == 0)
	{
	    ytop = atoi(p);
	}
	else if (strcmp(input, "xheight") == 0)
	{
	    xheight = atoi(p);
	}
	else
	{
	    fprintf(stderr, "Unknown option: `%s'\n", input);
	    exit(1);
	}
    }

    /* open the font files */
    if ((vecfont = fopen(repertory, "r")) == NULL)
    {
	perror(repertory);
	exit(1);
    }


    /* remember what the real x factor is */

    real_x_factor = x_factor;

    /* translate the characters */

    while(vgets(input) != NULL)
    {
	/* get the character desired */

	space_char = char_num = 0;
	if (input[0] == 's')
	{
	    /* space character of specified width */
	    space_char = atoi(&(input[1]));
	}
	else if (input[0] == 'p')
	{
	    /* image of the drawing pen */
	    char_num = -1;
	}
	else
	{
	    /* normal vector character */
	    char_num = atoi(input);
	}

	/* find the font character to use */

	if ((inp = index(input, ',')) == 0)
	{
	    fprintf(stderr, "bad character designation in `%s'\n",
		input);
	    break;
	}
	if ((inp[1] == '\\') && (inp[2] != '\0'))
	{
	    /* octal format */
	    sscanf(&(inp[2]), "%o", &map_to_char);
	}
	else
	{
	    /* character format */
	    map_to_char = inp[1];
	}

	if (char_num < MAX_ORD) order[char_num] = map_to_char;

	/* check for extra options */

	vtrans = 0;
	inp += 2;
	if ((inp = process_option(inp)) != 0)
	{
	    process_option(inp);
	}

    } /* end while */

}

/*
 *  process_option(ptr) - process possible line option pointed to by ptr.
 */

char *process_option(ptr)

char *ptr;

{
    if ((ptr = index(ptr, ',')) != 0)
    {
	ptr++;
	if (ptr[0] == 'r')
	{
	    /* aspect ratio */
	    x_factor *= atof(&ptr[1]);
	}
	else
	{
	    /* assume it's vertical translation */
	    vtrans = atoi(&(ptr[0]));
	}
    }
    return(ptr);
}

char *vgets(buff)

register char *buff;

{
    register char *retval;

    if ((retval = gets(buff)) != NULL && verbose)
    {
	fprintf(stderr, "%s\n", buff);
    }
    return(retval);
}



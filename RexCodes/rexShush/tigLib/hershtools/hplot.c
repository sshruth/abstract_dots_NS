

#include <stdio.h>
#include <sys/file.h>

#define TRUE 1
#define FALSE 0
#define ERROR -1

int tty_fid;
extern char *term_name;
char w300 = FALSE;

#define movmem bcopy

#define STSIZE	32000
#define KEEPSIZE 20
#define GRSIZE 160
#define GROFF 80

char debug, ins[90];
FILE *ibuf;
int cnum;

int itempt, co1, co2, w, x, y;
int deltax, deltay, xinc, yinc;
int lx,llx,ly,lly, cx,ccx,cy,ccy, x1,x2,y1,y2, a1,a2,d1,d2, i1,i2;
int xx1,xx2,yy1,yy2;
int minx, miny, maxx, maxy, base, obase, xheight, ytop, ybot, serifs;
char itemtype, linelet, retrace, openflag, lighten, hairflag, cvflag;
int lbear, rbear;
unsigned char letter;
int xheavy, xlight, yheavy, ylight, penwidth, pxdelta, pydelta,
	squarepen, shading, xshade, yshade;
int ppenw, blob, bblob, dblob;
int fgrey,fvertical,fhorizontal,fchecked,fhatched,fdiagonal,fwidth;
int bwidth,bedge,breverse;


#define WYSE	1
#define ESC	27

#define	FTSIZE 	12000
#define PRPTNL  2

#define BLACK	'1'
#define BLUE	'2'
#define GREEN	'3'
#define CYAN	'4'
#define RED	'5'
#define MAGENTA '6'
#define YELLOW	'7'
#define WHITE	'8'

#define UPPRDOT	','
#define LOWRDOT	'-'
#define BOTHDOT '7'
#define NODOT	' '

char upperdot=UPPRDOT, lowerdot=LOWRDOT, bothdot=BOTHDOT;

char	cmd, currch, cch, cbyte, cbyteo, mxflag;
int	col, row, currlen;

char	vkeep[GRSIZE][KEEPSIZE], gxoff, gyoff, ib;
char ckeep[6][GRSIZE];

int i, fd;
char ftname[30];


char  duplflag /*, ftlen[128]*/;
short int fix[128];
char  ft[FTSIZE];




char gr[GRSIZE][GRSIZE];

unsigned stsize, cindex[128], storept;
char store[STSIZE];


#include "vfont.c"

main()
{	char key();

/* stuff from xum */

	if (term_setup() != 0)
	{
		printf("Warning: This program needs to have the\n");
		printf("TERM environment variable setup before\n");
		printf("being run. Make sure this is done before\n");
		printf("executing hplot again (set yourself up like\n");
		printf("you were going to use vi).\n");
		exit(1);
	}
	printf("Terminal is a %s.\n", term_name);
	if (!strcmp(term_name, "w300")) w300 = TRUE;
	if (!w300)
	{	upperdot='^'; lowerdot='j'; bothdot='g';
	}

	tty_fid = cbreak_tty("/dev/tty");
	if (tty_fid == -1)
	{
		printf("Error opening terminal for cbreak i/o\n");
		exit(1);
	}

	debug = 0;

	stsize = STSIZE;

	printf("\nHplot - (c) Greg Lee, Oct. 1984\n\n");

	base = 9;
	ytop = 22;
	ybot = 7;
	cvflag = FALSE;
	lbear = rbear = 2;
	strcpy(ftname,"vwork");
	letter = currch = duplflag = 0;

	storept = x1 = x2 = y1 = y2 = a2 = d2 = 1;
	serifs = 2;

	do
	{	printf("\n");
		if (currch > ' ') printf("%c",currch);
			else printf("^%c",currch+64);
		printf("* ");
		cmd = key();
	switch (cmd)
	{
	case 'r': rdchr(); break;
	case 'z': bzero(cindex, 128*sizeof(int));
		  bzero(store, STSIZE);
		  storept = 1;
		  printf("zapped");
		  break;
	case 'p': plotlet(); break;
	case 'g': printf("enerating the font\n");
		  /*for (letter=1;letter<127;letter++)
			lplot(letter);*/
		  vfont();
		  cvflag = FALSE;
		  break;
#if WYSE
	case 'v': if (currch) showbits(); break;
#endif
	case 'b': gvar("ase",&base); break;
	case 'h': gvar("eight",&ytop); break;
	case 'd': gvar("epth",&ybot); break;
	case 'f': printf("actors:\n");
		  gvar("X1",&x1);
		  gvar("X2",&x2);
		  gvar("Y1",&y1);
		  gvar("Y2",&y2);
		  break;
	case '2': printf(" FACTORS (plot 2):\n");
		  gvar("XX1",&xx1);
		  gvar("XX2",&xx2);
		  gvar("YY1",&yy1);
		  gvar("YY2",&yy2);
		  break;
	case 'A': printf("SCENDERS:\n");
		  gvar("A1",&a1);
		  gvar("A2",&a2);
		  break;
	case 'D': printf("ESCENDERS:\n");
		  gvar("D1",&d1);
		  gvar("D2",&d2);
		  break;
	case 'I': printf("^ITALIC:\n");
		  gvar("I1",&i1);
		  gvar("I2",&i2);
		  break;
	case 't': if (duplflag) fix[0] &= ~PRPTNL;
		  else fix[0] |= PRPTNL;
		  chkdupl();
		  if (!duplflag) printf("not "); printf("tall");
		  break;
	case 'P': printf("EN STYLE:\n");
		  gvar("Width",&penwidth);
		  gvar("Delta X",&pxdelta);
		  gvar("Delta Y",&pydelta);
		  gvar("Square Pen",&squarepen);
		  gvar("Blob",&blob);
		  gvar("Delta Blob",&dblob);
		  break;
	case 'H': printf(" SHADE:\n");
		  gvar("Shading",&shading);
		  gvar("X Shade",&xshade);
		  gvar("Y Shade",&yshade);
		  break;
	case 'F': printf("ILL:\n");
		  gvar("Grey",&fgrey);
		  gvar("Vertical",&fvertical);
		  gvar("Horizontal",&fhorizontal);
		  gvar("Checked",&fchecked);
		  gvar("Hatched",&fhatched);
		  gvar("Diagonal",&fdiagonal);
		  gvar("Width",&fwidth);
		  break;
	case 'w': printf("idth adjust:\n");
		  gvar("Left Bearing",&lbear);
		  gvar("Right Bearing",&rbear);
		  break;
	case 'B': printf("OX:\n");
		  gvar("Width",&bwidth);
		  gvar("Edge",&bedge);
		  gvar("Reverse",&breverse);
		  break;
	case 'L': printf("INE STYLE:\n");
		  gvar("X Heavy",&xheavy);
		  gvar("X Light",&xlight);
		  gvar("Y Heavy",&yheavy);
		  gvar("Y Light",&ylight);
		  break;
	case 'O': if (openflag) printf("not ");
		  openflag = ~openflag;
		  printf("open");
		  break;
	case 'l': if (lighten) printf("not ");
		  lighten = ~lighten;
		  printf("light");
		  break;
	case 's': gvar("erifs",&serifs);
		  break;
	case 'R': if (retrace) printf(" not r");
		  retrace = !retrace;
		  printf("etrace");
		  break;
	case 'n': printf("ame of font(%s) = ",ftname);
		  scanf("%s",ftname);
		  break;
	case '?': cmdinfo();
		  printf("\nstore room %d, font room %d",
			stsize-storept,FTSIZE-fix[127]);
		  break;
	case '\\': printf("debug");
		  if (debug) printf(" off");
		  debug = !debug;
	case 'q':
	case 'x': break;
	case 'c': loadft();
		  cvflag = TRUE;
		  break;
	default: conout('?'); break;
	}
	}
	while ((cmd != 'x') && (cmd != 'q'));

	reset_tty(tty_fid);

}

cmdinfo()
{
      printf("\nRead   Plot      Name font  mag.Factors ^Pen style\n");
	printf("Zap    Generate            ^Ascenders   ^Line style\n");
	printf("Base   Vdisplay  Serifs    ^Descenders  ^Open face\n");
	printf("Height eXit/Quit^Italic     s^Hading     Convert\n");
	printf("Depth ^Fill     ^Box       ^Retrace      Width adjust\n");
}


gvar(s,v)
char *s; int *v;
{	char is[80];

	printf(s);
	printf("(%d) = ", *v);
	gets(is);
	if (is[0] < '0' || is[0] > '9') return;
	sscanf(is, "%d", v);
}

plotlet()
{
	printf("lot a letter? : ");
	scanf("%c", &letter);
	lplot(letter);
}


rdchr()
{	char fname[20];


	printf("ead what .chr file?  : "); scanf("%s", fname);
	if (!fname[0]) return;
	strcat(fname, ".chr");
	if ((ibuf = fopen(fname, "r")) == NULL) return;

	while (fgets(ins, 90, ibuf))
	{	itempt = 0;
		while (itemtype = scnitem())
		switch(itemtype)
		{
		case 1: letter = 0;
			break;
		case 2: cindex[letter] = storept;
			lbear = rbear = 2;
			break;
		case 3: sto2(co1 + 64, co2 + 64);
			break;
		case 4: sto2(0, 64);
			break;
		case 5: sto2(0, 0);
			break;
		case 6: sto2(0, 'H');
			break;
		}
	}


	fclose(ibuf);

	printf("\nUsed %u of buffer (%d remaining).", storept,stsize-storept);
}

sto2(x,y)
char x, y;
{
	if (letter < 1 || letter > 127) return;
	if (storept > stsize - 2) return;

	store[storept++] = x;
	store[storept++] = y;

}

scnitem()
{	char dc;

	while (ins[itempt] == ' ' || ins[itempt] == '\t') itempt++;

	if (!ins[itempt] || ins[itempt] == '\n') return(0);

	if (sscanf(ins + itempt, "%d:", &cnum))
	{	match(':');	return(1); }

	if (sscanf(ins + itempt, "'%c':", &letter))
	{	match(':');	return(2); }

	if (sscanf(ins + itempt, "^%c':", &letter))
	{	match(':');	letter &=  0x1F; return(2); }

	if (sscanf(ins + itempt, "\\%o:", &letter))
	{	match(':');	return(2); }

	if (sscanf(ins + itempt, "Bounds(%d,%d)", &co1, &co2))
	{	match(')');	return(66); }

	if (sscanf(ins + itempt, "Bearings(%d,%d)", &lbear, &rbear))
	{	match(')');	return(66); }

	if (sscanf(ins + itempt, "s(%d,%d)", &co1, &co2))
	{	match(')');	if (serifs) return(3); return(66); }

	if (sscanf(ins + itempt, "S(%d,%d)", &co1, &co2))
	{	match(')');	if (serifs>1) return(3); return(66); }

	if (sscanf(ins + itempt, "(%d,%d)", &co1, &co2))
	{	match(')');	return(3); }

	if (sscanf(ins + itempt, "Pen Up%c", &dc))
	{	match('p');	return(4); }

	if (sscanf(ins + itempt, "End Char%c", &dc))
	{	match('r');	return(5); }

	if (sscanf(ins + itempt, "Hair%c", &dc))
	{	match('r');	return(6); }

	if (sscanf(ins + itempt, "Base(%d)", &base))
	{	match(')');	obase = base; return(66); }

	if (sscanf(ins + itempt, "x Height(%d)", &xheight))
	{	match(')');	return(66); }

	if (sscanf(ins + itempt, "Height(%d)", &ytop))
	{	match(')');	return(66); }

	if (sscanf(ins + itempt, "Depth(%d)", &ybot))
	{	match(')');	return(66); }

	return(0);

}

match(c)
char c;
{	while (ins[itempt++] != c);
}

lplot(let)
char let;
{	int ox1,ox2,oy1,oy2;
	int oopen;

	if (xx1 || xx2 || yy1 || yy2)
	{	ox1 = x1; oy1 = y1;
		ox2 = x2; oy2 = y2;
		oopen = openflag;

		if (xx1 || xx2)
		{	x1 = xx1 ; x2 = xx2; }
		if (yy1 || yy2)
		{	y1 = yy1 ; y2 = yy2; }

		openflag = FALSE;
		retrace = 2;
		llplot(let);
		openflag = oopen;

		x1 = ox1; y1 = oy1;
		x2 = ox2; y2 = oy2;
		retrace = TRUE;
	}

	llplot(let);
}


llplot(let)
char let;
{	char c1, c2, penstate;
	unsigned xpt;

	currch = let;
	chkdupl();
/*
	if (let < 0 || let > 128) return;
*/
	if (!(xpt = cindex[let])) return;
	/*if (kbhit()) return;*/

	if (retrace != TRUE) cleargr();

	bblob = blob;

	for (penstate = 0 ; (c1 = store[xpt++])
			  + (c2 = store[xpt++]) ; )
	if (!c1)
	{	penstate = 0;
		if (c2 == 'H') hairflag = TRUE;

		bblob = blob;
	}

	else
	{	lx = cx; ly = cy;
		cx = c1 - 64; cy = c2 - 64;

		if (penstate) drawline();

		penstate = 1;

	}


	showgr();

	retrace = 0;
}


cleargr()
{
	bzero(gr, GRSIZE * GRSIZE);
	linelet = '0';
/*	penwidth = abs(penwidth);*/
}


drawline()
{
	if (debug) printf("\nDrawing from (%d,%d) to (%d,%d).", lx,ly,cx,cy);

	linelet++;
	if (linelet > '9' && linelet < 'A') linelet = 'A';
	else if (linelet > 'Z' && linelet < 'a') linelet = 'a';
	else if (linelet > 'z') linelet = '!'+1;

	if (abs(cy) <= abs(ly))
	{	llx = lx;
		lly = ly;
		ccx = cx;
		ccy = cy;
	}
	else
	{	llx = cx;
		lly = cy;
		ccx = lx;
		ccy = ly;
	}


	if (i1 && i2) llx += fadjust(lly,i1,i2)-GROFF;
	x = fadjust(llx,x1,x2);

	if (i1 && i2) ccx += fadjust(ccy,i1,i2)-GROFF;
	ccx = fadjust(ccx,x1,x2);

	deltax = abs(ccx - x);
	if (ccx >= x) xinc = 1; else xinc = -1;

	if (xheight && letter >= 'a' && letter <= 'z')
	{	if (lly > obase) lly += ((lly-obase)*d1)/d2;
		else if (lly + xheight <= obase)
			lly -= ((obase+1-xheight-lly)*a1)/a2;
		if (ccy > obase) ccy += ((ccy-obase)*d1)/d2;
		else if (ccy + xheight <= obase)
			ccy -= ((obase+1-xheight-ccy)*a1)/a2;
	}

	y = fadjust(lly,y1,y2);
	ccy = fadjust(ccy,y1,y2);
	deltay = abs(ccy - y);
	if (ccy >= y) yinc = 1; else yinc = -1;

	drawdot();

	if (deltax >= deltay) doforx();
	else dofory();

	hairflag = FALSE;
}

fadjust(u,f1,f2)
int u,f1,f2;
{
	return( (u * f1 + f2-1)/f2 + GROFF);
}

drawdot()
{
	dotset();

	if (bblob > 0) bblob -= dblob;
	else if (bblob < 0) bblob += dblob;

	ppenw = abs(penwidth + bblob/8);
	if (ppenw) doforpen();
}



doforpen()
{	int sx,sy;

	if (hairflag) return;

	sx = x; sy = y;

	x -= ppenw * pxdelta / 2;
	y -= ppenw * pydelta / 2;

	w = ppenw;
	while(w--)
	{	x += pxdelta;
		dotset();
		y += pydelta;
		dotset();
	}


	w = ppenw;
	if (squarepen)
	while(w--)
	{	x -= pydelta;
		y -= pxdelta;
		dotset();
	}

	w = ppenw;
	if (squarepen > 1)
	while(w--)
	{	x -= pxdelta;
		y -= pydelta;
		dotset();
	}

	w = ppenw;
	if (squarepen > 2)
	while(w--)
	{	x += pydelta;
		y += pxdelta;
		dotset();
	}

	x = sx; y = sy;
}


dotset()
{

	if (x<0 || x>=GRSIZE || y<0 || y>=GRSIZE) return;
	if (gr[x][y]) return;
	gr[x][y] = linelet;

}



doforx()
{	int err, i;

	err = deltax / 2;
	i = deltax;

	while (i--)
	{	err += deltay;
		if (err >= deltax)
		{	err -= deltax;
			y += yinc;
			if (xheavy) drawdot();
			if (xlight) {x += xinc; continue;}
		}
		x += xinc;
		if (hairflag && (i&1)) continue;
		drawdot();
		if (debug) printf("\n   @(%d,%d) [%d,%d]",
			x-GROFF, y-GROFF, x,y);
	}
	
}

dofory()
{	int err, i;

	gr[x][y] = linelet;

	err = deltay / 2;
	i = deltay;

	while (i--)
	{	err += deltax;
		if (err >= deltay)
		{	err -= deltay;
			x += xinc;
			if (yheavy) drawdot();
			if (ylight) { y+= yinc; continue; }
		}
		y += yinc;
		if (hairflag && (i&1)) continue;
		drawdot();
		if (debug) printf("\n   @(%d,%d) [%d,%d]",
			x-GROFF, y-GROFF, x,y);
	}
	
}

showgr()
{	/* int x, y;*/

	minx = miny = GRSIZE-1;
	maxx = maxy = 0;

	for (y = 0; y < GRSIZE; y++)
	{	for (x = 0; x < GRSIZE; x++)
		if (gr[x][y])
		{	if (x < minx) minx = x;
			if (y < miny) miny = y;
			if (x > maxx) maxx = x;
			if (y > maxy) maxy = y;
		}
	}

	if (bwidth)
	{	i = max(bedge+2, 1 + (bwidth + minx - maxx) / 2);
		minx = max(0, minx - i);
		maxx = min(GRSIZE-1, maxx + i);
		while (maxx - minx > GRSIZE) {minx++; maxx--; }
		miny = base + GROFF + 1 - ytop;
		maxy = miny + ybot;
	}

	printf("\n");

	if (openflag || lighten)
	{ for (y = miny; y <= maxy; y++)
	   for (x = minx; x <= maxx; x++)
		if (gr[x][y])
		{
		 if (!gr[x+1][y] && !gr[x+2][y] && !gr[x+3][y]) continue;
		 if (!gr[x][y+1] && !gr[x][y+2] && !gr[x][y+3]) continue;
		 if (!gr[x-1][y] && !gr[x-2][y] && !gr[x-3][y]) continue;
		 if (!gr[x][y-1] && !gr[x][y-2] && !gr[x][y-3]) continue;
			gr[x][y] = 1;
		}

	if (breverse)
	  for (y = miny; y <= maxy; y++)
	   for (x = minx; x <= maxx; x++)
		{ if (!gr[x][y]) gr[x][y] = 1;
		  else if (gr[x][y] == 1) gr[x][y] = 0;
		}

	  for (y = miny; y <= maxy; y++)
	   for (x = minx; x <= maxx; x++)
		if (gr[x][y])
		{	gr[x][y]--;
			if (lighten)
			 if (gr[x][y])
			 {	if (gr[x-1][y])
				{	if (gr[x-1][y-1])
					  gr[x-1][y] = 0;
					else gr[x][y] = 0;
				}
			 }
			 else if (!gr[x-1][y] && gr[x+1][y] < 2)
				gr[x][y] = '*';

			if (fgrey<0) gr[x][y] = 0;
			if (gr[x][y]) continue;

			if (fgrey)
			{	if (x % fgrey) continue;
				if (y % fgrey) continue;
				if (!fwidth) gr[x][y] = '+';
			}

			if (fwidth)
			{ if (t(x,fvertical))		continue;
			  if (t(y,fhorizontal))	continue;
			  if (fchecked &&
			   (!t(x,fchecked) == !t(y,fchecked))) continue;
			  if (t(x,fhatched) && t(y,fhatched)) continue;
			  if (fdiagonal < 0)
			  {	if (t(abs(x-y),-fdiagonal)) continue;  }
			  else if (t(x+y, fdiagonal))	continue;

				gr[x][y] = '#';
			}
		}
	}


	if (shading && retrace != 2)
	{	shading = abs(shading);
		xshade = abs(xshade);

		lx = minx + shading + yshade;
		ly = min(maxy + shading + yshade,GRSIZE-1);

		llx = maxx;
		
		while (lx <= llx)
		{	raybounce();
			lx += 2 + xshade;
		}

		while (ly >= miny)
		{	raybounce();
			ly -= 2 + xshade;
		}
	}


	if (bedge && retrace != 2)
	  for (y = miny; y <= maxy; y++)
	   for (x = minx; x <= maxx; x++)
		  if (	   y < miny+bedge
			|| y > maxy-bedge
			|| x < minx+bedge
			|| x > maxx-bedge
		     )
		  gr[x][y] = '|';

	if (maxx - minx > GRSIZE-1) maxx = minx + GRSIZE-1;

	for (y = miny; y <= maxy; y++)
	{	for (x = minx; x <= maxx; x++)
		{	if (x-minx > 78) break;
			if (gr[x][y]) printf("%c",gr[x][y]);
		  	else if (y == base+GROFF) putchar('-');
		  	else putchar(' ');
		}
		/*if (y-miny == 23) key();*/
		printf("\n");
	}

	printf("\nHeight %d; Depth %d; Width %d\n",
		 base+GROFF+1-miny, maxy-GROFF-base, maxx-minx+1);

	rdedf();
}


t(a,period)
int a, period;
{	if (!period) return(FALSE);
	a %= period;
	if (fwidth == 1) return(a);
	if (fwidth < 0) a = (a >= abs(period) / -fwidth);
	else		a = (a <  abs(period) /  fwidth);

	if (period < 0) a = !a;

	return(a);
}



radar()
{	x = lx; y = ly;

	while (	!gr[x][y]
	   &&	!gr[x-1][y]
	   &&	!gr[x][y-1]
	   &&	!gr[x-1][y-1] )
	{	x--; y--;
		if (x < minx || y < miny) return(FALSE);
	}
	return(TRUE);
}

raybounce()
{
	if (!radar()) return;

	i = shading;
	cx = x + yshade; cy = y + yshade;

	while(i-- && cx<98 && cy<98 )
	{	cx++;  cy++;
		if (gr[cx][cy]) i = 0;
		else gr[cx][cy] = '.';
	}
	maxx = max(cx,maxx);
	maxy = max(cy,maxy);
}


char conin()
{	return (0x7f & getchar());
}

conout(c)
char c;
{	putchar(c);
}


tokeep()
{
	tokepp(0, currch);
	if (duplflag) tokepp(3, dtrans(currch, FALSE));
}

tokepp(ib, currch)
unsigned char ib, currch;
{	int i, j, cbase;

	cbase = fix[currch];
	currlen = (fix[currch+1] - cbase) / 3;

	if (currlen > GRSIZE) currlen = GRSIZE;
	for (i = 0; i < 3; i++)
	 for (j = 0; j < GRSIZE; j++)
	  if (j >= currlen) ckeep[ib+i][j] = 0;
   		else  ckeep[ib+i][j] =  ft[cbase + j + i*currlen];
}


#if WYSE
fromkeep()
{
	frommkeep(0,currch);
	if (duplflag) frommkeep(3, dtrans(currch, FALSE));
}

frommkeep(ib, currch)
char ib, currch;
#else
fromkeep()
#endif
{	int i, j, cend, cbase, cnext, mlen;

	cbase = fix[currch];
	cnext = fix[currch+1];

	cend = cbase + currlen * 3;
	mlen = (cend-cnext) ? FTSIZE - 1 - max(cnext,cend)
			      : 0;
	if (fix[127] + cend - cnext >= FTSIZE)
	{	printf("\nfont overflow\n"); return; }

	if (mlen)
	{	movmem(ft+cnext, ft+cend, mlen); 
	 	for (i = currch+1; i < 128; i++)
			fix[i] = fix[i] + cend - cnext;
		/*for (i = 0; i < 127; i++)
			ftlen[i] = (fix[i+1] - fix[i])/3;*/
		/*if (fix[127] >= FTSIZE) ovlflag = TRUE;*/
	}

	if (currlen)
	for (i = 0; i < 3; i++)
	 for (j = 0; j < currlen; j++)
	   ft[cbase + j + i*currlen] = vkeep[ib+i][j];
}


char key()
{	char c;

	c = conin();
	if (c < ' ') c += '@';
		else if (c >='A' && c <= 'Z')
		c += ' ';
	if (c == 'J') return(key());
	return(c);
}


dtrans(c, upper)
char c, upper;
{	if (c == 0x20 || c == 0x5F) return(c);

	if (c >= 0x60) c -= 0x20;
	else if (c < 0x20) c += 0x20;

	if (upper) return(c);

	if (c < 0x40) c -= 0x20;
	else c += 0x20;

	return(c);
}

chkdupl()
{
	duplflag = fix[0] & PRPTNL;
#if WYSE
	if (duplflag) currch = dtrans(currch, TRUE);
#endif
}

rdedf()
{	char c;
	int i, y, lasty, dcurrlen;


	if  (duplflag)	currch = dtrans(currch, TRUE);


	lbear = rbear = 0;


	bzero(vkeep, KEEPSIZE*GRSIZE);
	row = currlen = 0; dcurrlen = 10000;

	y = base + GROFF + 1 - ytop;
	lasty = y + ytop + ybot;
	if (lasty >= GRSIZE) lasty = GRSIZE-1;

	for ( ; y <= lasty; row++,y++)
	{
		for (i = minx, col = lbear; i <= maxx; col++,i++)
		 if (gr[i][y]) chngbit(TRUE);

		if (currlen < col + rbear)
			currlen = col + rbear;
	}

/*	fromkeep();*/
}


chngbit(biton)
char biton;
{	unsigned char bit, mask;
	
	bit = col % 8;
	mask = 128 >> bit; 
	cbyte = vkeep[row][col/8];
	if (biton) cbyte |= mask;
		else cbyte &= ~mask;
	vkeep[row][col/8] = cbyte;
}






gxy(x,y)
char x,y;
{	if (w300)
	{	conout(ESC); conout('=');
		conout(y+' '); conout(x+' ');
	} else mov_cur(x,y);
}

showbits()
{
/*	conout(0x1A);
	sleep(1);
*/	cls();
	graph(TRUE);
/*	tokeep();*/

	gxoff = (78 - currlen)/2;
	gyoff = (46 - ytop - ybot)/4;
	sshowbits();
	graph(FALSE);
	gxy(0,22);
}

sshowbits()
{	unsigned char i, j, bit, mask, r;

	r = gyoff + (ytop + ybot + 1)/2;

     if (currlen > 0)
     {
	color(RED,WHITE);
	gxy(gxoff-1,gyoff-1);
	conout(w300 ? '2':'+');
	for (i = 0; i < currlen; i++) conout(w300 ? ':':'-');
	conout(w300 ? '3':'+');

	for (i = gyoff; i < r; i++)
	  {
		gxy(gxoff-1,i); conout(w300 ? '6':'|');
		gxy(currlen+gxoff,i); conout(w300 ? '6':'|');
	  }
	gxy(gxoff-1,r);
	conout(w300 ? '1':'+');
	for (i = 0; i < currlen; i++) conout(w300 ? ':':'-');
	conout(w300 ? '5':'+');

	color(BLACK,WHITE);
     }

	for (i = 0; i < ytop + ybot; i++,i++)
	 for (j = 0; j < currlen; j += 8)
	  {	cbyte = vkeep[i][j/8];
		cbyteo = vkeep[i+1][j/8];
		mask = 128;
		for (bit = 0; bit < 8; bit++, mask >>= 1)
		 if ( /* there is a dot in this row or the next */
		     (mask & cbyte) | (mask & cbyteo)
		    )
		 {	gxy(j+bit+gxoff, i/2+gyoff);
			 /* both dots? */
			if ((mask & cbyte)&& (mask & cbyteo))
				conout(bothdot);
			/* dot in this row only? */
			else if (mask & cbyte)
				conout(upperdot);
			else conout(lowerdot);	/* must be dot in next row */
		 }
	  }
}


graph(yes)
char yes;
{	if (!w300) return;

	conout(ESC);	conout('H');
	if (yes) conout(2); else conout(3);
	conout(ESC);	conout('`');	/* cursor off/on	*/
	if (yes) conout('0'); else conout('1');
	if (!yes) color(BLACK, WHITE);
}

color(fc, bc)
char fc, bc;
{	if (!w300) return;

	conout(ESC);	conout('g');
	conout(fc);	conout(bc);
}


max(x,y)
int x,y;
{	if (x >= y) return(x);
	return(y);
}

min(x,y)
int x,y;
{	if (x >= y) return(y);
	return(x);
}

ctovkeep()
{	bzero(vkeep,KEEPSIZE*GRSIZE);

	for (row = 0; row < 24; row++)
	for (col = 0; col < currlen+lbear; col++)
		if (chkcbit()) chngbit(TRUE);
	currlen += lbear + rbear;
}

chkcbit()
{	if (duplflag && row > 23)
		return( cchkcbit(3, row - 24));
	else return( cchkcbit(0, row));
}

cchkcbit(ib, row)
char ib, row;
{	unsigned char bit, mask, cbyte;

	if (col < lbear) return(0);
	bit = row / 3;
	mask = 128 >> bit; 
	cbyte = ckeep[ib + (row % 3)][col-lbear];
	if (cbyte & mask) return(1);
	return(0);
}



loadft()
{	int fdft, i, ftrecs;
	char fname[20];

	strcpy(fname,ftname); strcat(fname,".fn2");

	if ((fdft = open(fname,O_RDONLY,"r")) == ERROR)
		{printf("no font"); return;}
	if (read(fdft, fix, 2*128) != 2*128)
		{printf("bad font"); return;}

	/* only read enough of the font to get info for char's
		up to ascii nul (which is not used)		*/
	ftrecs = (fix[127]+127)/128;

	if (read(fdft, ft, ftrecs*128) != ftrecs*128)
		{printf("bad font"); return;}
	close(fdft);
/*
		if (ftrecs > FTSIZE/128) ovlflag = TRUE;
	for (i = 1; i < 127; i++)
		ftlen[i] = (fix[i+1] - fix[i])/3;
*/
	chkdupl();

}




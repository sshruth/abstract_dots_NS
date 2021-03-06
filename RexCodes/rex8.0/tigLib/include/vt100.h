/*	vt.h:	header for VT100 commands */

#define	ESC	033		/*	ESCAPE character */
#define CTRLB	002		/* CNTROL B */
#define	CTRLC	003		/* CNTROL C */
#define CTRLD	004		/* CNTROL D */
#define BELL	007		/* tty bell character */
#define VTMAX	24	/* last line */
#define vtcsi	putchar(ESC);putchar('[')
#define	vterase	vtcsi;putchar('2');putchar('J')
#define	vtansi	putchar(ESC);putchar('<')
#define	vtkpam	putchar(ESC);putchar('=')
#define	vtkpnm	putchar(ESC);putchar('>')
#define vtsav	putchar(ESC);putchar('7');
#define vtrest	putchar(ESC);putchar('8');

/* cursor back, down, forward, up */
#define vtcub(x)	vtcsi;vtpn(x);putchar('D')
#define vtcud(x)	vtcsi;vtpn(x);putchar('B')
#define vtcuf(x)	vtcsi;vtpn(x);putchar('C')
#define vtcuu(x)	vtcsi;vtpn(x);putchar('A')

/* random position for cursor */
#define vtcup(row,col)	vtcsi;vtpn(row);putchar(';');vtpn(col);putchar('H')

#define vthome	vtcup(0,0)

/* clean the line containing the cursor */
#define vtcl	vtcsi;putchar('2');putchar('K')

/* clean from cursor to end of line */
#define vtecl	vtcsi;putchar('K')

/* clean from cursor back to beginning of line */
#define vtbcl	vtcsi;putchar('1');putchar('K')

/* delete to end of screen */
#define vtwipe	vtcsi;putchar('J')

/* define a line to be double width */
#define vtdwl(x)	vtcup(x,0);putchar(ESC);putchar('#');putchar('6')

/* define a line to be top of double height */
#define vttop(x)	vtcup(x,0);putchar(ESC);putchar('#');putchar('3')

/* define a line to be bottom of double height */
#define vtbot(x)	vtcup(x,0);putchar(ESC);putchar('#');putchar('4')

/* set top and bottom margins */
#define vtmargins(top,bot)	vtcsi;vtpn(top);putchar(';');vtpn(bot);putchar('r')

/* scroll smoothly, or jerkily */
#define vtsmooth	vtcsi;putchar('?');putchar('4');putchar('h')
#define vtjerky		vtcsi;putchar('?');putchar('4');putchar('l')

/* set character parameters */
#define attriboff	vtcsi;putchar('m')
#define boldon		vtcsi;putchar('1');putchar('m')
#define underon		vtcsi;putchar('4');putchar('m')
#define blinkon		vtcsi;putchar('5');putchar('m')
#define revvidon	vtcsi;putchar('7');putchar('m')


#define STATUSLINE 1
#define STICSLINE 2
#define WARNLINE 3
#define INLINE 4
#define HELPLINE 24
#define SCROLLTOP 4
#define SCROLLBOT 24

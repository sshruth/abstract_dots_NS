/*
 *-----------------------------------------------------------------------*
 * NOTICE:  This code was developed by the US Government.  The original
 * versions, REX 1.0-3.12, were developed for the pdp11 architecture and
 * distributed without restrictions.  This version, REX 4.0, is a port of
 * the original version to the Intel 80x86 architecture.  This version is
 * distributed only under license agreement from the National Institutes 
 * of Health, Laboratory of Sensorimotor Research, Bldg 10 Rm 10C101, 
 * 9000 Rockville Pike, Bethesda, MD, 20892, (301) 496-9375.
 *-----------------------------------------------------------------------*
 */

/*
 * Header for tty control.
 */

/*
 * Defines for tty type.
 */
#define T_VT100 1
#undef T_Z29			/* Zenith Z-29 */

/*
 * Lines used for special purposes on screen.
 */
#define T_STAT0		1	/* status line 0 */
#define T_STAT1		2	/* status line 1 */
#define T_ERROR		3	/* error status line */
#define T_SCTOP		4	/* beginning of scrolled region */
#define T_VPROMPT	23	/* menu variable access prompt line */
#define T_VERROR	24	/* menu variable access error */
#define T_SCBOT		24	/* bottom of scrolled region */

/*
 * State set variable changing defines.
 */
#define T_SBOT		18	/* bottom line of printed state names */
#define T_SPRINT	20	/* line where state variables are printed */
#define T_SPROMPT	23	/* state changing prompt */
#define T_SERROR	24	/* error line */

#define T_MENUMAX	(T_VPROMPT-T_SCTOP)	/* number of menu lines */
#define T_CPVALUE	25	/* column position of value of menu variable */
#define T_CPHELP	49	/* column position of help message printed
				   alongside menu */

/*
 * tty control sequences.  Defined in rex/sys/tty/tty.c.
 */
extern char tscsav[];		/* save cursor and attributes */
extern char tscres[];		/* restore cursor and attributes */
extern char tseltcur[];		/* erase line from beginning to cursor */
extern char tsecurtel[];	/* erase from cur to end of line */
extern char tsecline[];		/* erase line containing cursor */
extern char tsebcur[];		/* erase below cursor; from cursor to end of
				   screen */
extern char tseall[];		/* erase entire screen */
extern char tsulon[];		/* underline on */
extern char tsuloff[];		/* underline off */
extern char tshome[];		/* home cursor */
extern char tsrindex[];		/* reverse index */
extern char tsansi[];		/* set ansi mode */

/*
 * Macros using above control sequences.
 */
#define tcsav_	  fputs(tscsav, stdout)	   /* save cursor */
#define tcres_	  fputs(tscres, stdout)	   /* restore cursor */
#define teltcur_  fputs(tseltcur, stdout)  /* erase line: beg to cursor */
#define tecurtel_ fputs(tsecurtel, stdout) /* erase line: cursor to end */
#define tecline_  fputs(tsecline, stdout)  /* erase line containing cursor */
#define	tebcur_	  fputs(tsebcur, stdout)   /* erase screen: cursor to end */
#define teall_	  fputs(tseall, stdout)	   /* erase entire screen */
#define tulon_	  fputs(tsulon, stdout)	   /* underline on */
#define tuloff_	  fputs(tsuloff, stdout)   /* underline off */
#define thome_	  fputs(tshome, stdout)	   /* cursor to home */
#define trindex_  fputs(tsrindex, stdout)  /* reverse index */
#define tansi_	  fputs(tsansi, stdout)    /* set ansi mode */

/*
 * Other screen macros.
 */
#define ttop_		tcup(T_SCTOP, 1)   /* cursor to scroll top */
#define tescroll_	ttop_; tebcur_	   /* erase scroll area */
#define tflush_		fflush(stdout)	   /* flush std out */

/*
 * Function prototypes.
 */
int tgetl(char *p, char *endp);
void tsetsc(int top, int bot);
void tputs(char *p);
char *tgscup(int line, int col, char *sp, char *ep);
char *tgsfb(int col, char *sp, char *ep);
void tcup(int line, int col);

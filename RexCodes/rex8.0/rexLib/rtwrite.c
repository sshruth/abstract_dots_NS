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
 * Root write processing.
 */

#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include "rexhdrs.h"
#include "tty.h"

extern FILE *rtout;

/*
 * Only the int process calls mksroot() to write state list variables to
 * root file.  Following pointer is initialized by int to point to mksroot()
 * function.  For other REX processes it is initialized to point to a
 * null function that is never called.
 */
extern int nullf();
int (*p_mksroot)() = &nullf;

/*
 * Called from alert() to write root file for this process.
 */
int rt_write(char *i_rtname)
{
	INT_BLOCK_P I_b= i_b;
	MENU *mp;
	ME_RECUR recur;
	char *begp;
	char *endp;
	int error= 0, argc, mx;
	char *argv[P_NARG];

	if(I_b->i_rtappend) {
		if((rtout= fopen(i_rtname, "a")) == NULL) {
			rxerr("rt_write(): Cannot open root file");
			return(-1);
		}
	}
	else {
		if((rtout= fopen(i_rtname, "w")) == NULL) {
			rxerr("rt_write(): Cannot open root file");
			return(-1);
		}
	}

	if(I_b->i_rtflag & RT_WMENU) {
		for(mp= &menus; *mp->me_name != '\0'; mp++) {
			if(!mp->me_vlp) continue;	/* don't print separator commands */
			recur.me_rec_bp= NP;
			recur.me_rec_menup= mp;
			recur.me_rec_astr= NS;
			argv[0]= NP;
			if(mkroot(mp, argv, NP, &recur, NP) < 0) {
				error= 1;
				goto out;
			}
		}
	}
	if(I_b->i_rtflag & RT_WUMENU) {
		for(mp= &umenus; *mp->me_name != '\0'; mp++) {
			if(!mp->me_vlp) continue;	/* don't print separator commands */
			recur.me_rec_bp= NP;
			recur.me_rec_menup= mp;
			recur.me_rec_astr= NS;
			argv[0]= NP;
			if(mkroot(mp, argv, NP, &recur, myptp->p_name) < 0) {
				error= 1;
				goto out;
			}
		}
	}

	if((I_b->i_rtflag & RT_WSTATE) && (myptx == I_b->ph_int_pi)) {
		if((*p_mksroot)() < 0) error= 1;
	}

 out:
	fflush(rtout);
	fclose(rtout);
	return(error);
}

/*
 * Write specified menu to root file.
 * If tvlp != NP, mkroot() has been called recursively to write a submenu;
 * name of submenu is tvlp->vl_name.  Otherwise mkroot() has been called
 * to write a top level menu and will output the full syntax for the menu
 * name with the .process extension.
 */
int mkroot(MENU *mp, char *argv[], VLIST *tvlp, ME_RECUR *rp, char *procName)
{
	VLIST *vlp;
	int call_cnt= 0;
	int (*fp)(), (*afp)();
	char astr[P_ISLEN];
	char *nullarg[1]= {0};

	/*
	 * If called with no args and there is an argument
	 * generation function, initialize fp.
	 */
	if(*argv == NP && (mp->me_rtagen != NP)) fp= mp->me_rtagen;
	else fp= NP;

	for(;;) {
	    if(fp != NP) {
			if((*fp)(call_cnt++, mp, astr) < 0) {
				rxerr("mkroot(): Bad return from me_rtagen()");
				return(-1);
			}
			if(astr[0] == '\0') break;	/* done */
			*argv= astr;
			rp->me_rec_astr= astr;
	    }
	    if(mp->me_flag & ME_BEF) {
			afp= mp->me_accf;
			if((*afp)(ME_BEF|ME_ROOT, mp, *argv, rp) < 0) {
				rxerr("mkroot(): Illegal menu access");
				return(-1);
			}
	    }

	    /*
	     * Print menu name.
	     */
	    if(tvlp == NP) {
			fputs("~\n~Menu:\n", rtout);
			fputs(mp->me_name, rtout);
			if(procName != (char *)NULL) {
				putc('.', rtout);
				fputs(procName, rtout);
			}
	    }
		else {
			fputs(tvlp->vl_name, rtout);
		}
	    if(*argv != NP) {
			putc(' ', rtout);
			fputs(*argv, rtout);
	    }
	    putc('\n', rtout);

	    /*
	     * Print entries and values.
	     */
	    for(vlp= mp->me_vlp; *vlp->vl_name != '\0'; vlp++) {
			/*
			 * If entry is a submenu, recurse and call mkroot again.
			 * Arg passed is always ptr to NP for submenus;  all
			 * submenus will be written.
			 */
			if(vlp->vl_type == ME_SUBMENU) {
				ME_RECUR recur;

				recur.me_rec_bp= rp;
				recur.me_rec_menup= (MENU *)vlp->vl_add;
				recur.me_rec_astr= &nullarg;
				if(mkroot((MENU *)vlp->vl_add, &nullarg, vlp, &recur, procName) < 0)
					return(-1);
			}
			else {
				pr_line(mp, vlp, 0, rtout);
			}
	    }

	    /*
	     * Following terminates loop when args are not supplied by
	     * me_rtagen().  First statement is for case when mkroot() is
	     * called with no arg strings, second for case in which an
	     * array of arg strings is passed.
	     */
	    if(tvlp != NP) fputs("~--End submenu------------>\n", rtout);
	    if(fp == NP) {
			if(*argv == NP) break;
			if(*++argv == NP) break;
	    }
	}
	return(0);
}

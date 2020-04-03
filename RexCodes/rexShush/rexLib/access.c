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
 * Menu processing.
 */
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include "rexhdrs.h"
#include "tty.h"

/*
 * Access a menu to display and change variables.
 */
int
access_m(ME_ACCP *ap, ME_RECUR *rp)
{
	MENU *mp;
	char *astr;
	VLIST *vlp;
	char **pp, *rptr;
	int (*fp)();
	char tvadd[P_LNAME], is[P_ISLEN], *pbuf[P_NARG];
	u_int vadd;
	int error= 0, nlines, pcnt, line, rflag, ctmp, errvaf, base, lflag, fflag;
	int err_erase= 2;

	mp= rp->me_rec_menup;
	astr= rp->me_rec_astr;

	/*
	 * Set rflag if currently reading from a root.
	 */
	if(infd != 0) rflag= 1;
	else rflag= 0;

	if(mp->me_flag & ME_BEF) {
		if( ((fp= mp->me_accf) == NP)
			||  ((*fp)(ME_BEF, mp, astr, rp) < 0) ) {
			rxerr("access_m(): Illegal menu access");
			error= -1;
			goto out;
		}
	}

    /*
     * Print menu on screen.  Return to here after
     * submenu recursion to reprint previous menu.
     */
 begin:

    if((nlines= (*ap->ap_pmenu)(mp)) < 0) {
		error= -1;
		goto out;
    }
    if(nlines == 0) goto out;

    /*
     * Main loop to get line of input, parse, and change menu
     * variables.
     */
    for(;;) {
		if(echo) {
			if(err_erase++ == 1) {
				/*
				 * Erase previous error message.
				 */
				tcup(T_VERROR, 1);
				tecline_;
			}
			tcup(T_VPROMPT, 0);
			tecline_;
			fputs(mp->me_name, stdout);
			if(*astr != '\0') {
				fputs(" ", stdout);
				fputs(astr, stdout);
			}
			fputs(": ", stdout);
			tflush_;
		}
		if( tgetl(is, &is[P_ISLEN]) <= 0
			|| (pcnt= parse(1, is, pbuf, P_NARG)) < 0 ) {
			if(rflag == 0) continue;
			error= -1;
			goto out;
		}
		if(pcnt == 0) break;		/* done */
		if(pbuf[0][0] == '.') {
			error= 1;
			goto out;		/* return all way to top from
							   submenu */
		}

		/*
		 * Loop to allow more than one var to be changed from single
		 * command.  Each time thru pp should be inc'd by 2 even if
		 * errors encountered so that parsing stays in sync.  In case
		 * of odd num of args, however, *pp must be checked for NP
		 * both before and after incrementing in for() statement below.
		 */
		for(pp= pbuf; *pp != NP; pp= (*pp == NP ? pp : pp+1)) {
			vlp= mp->me_vlp;
			if(isdigit(*pbuf[0])) line= atoi(*pp++);
			else line= sindex(*pp++, &vlp->vl_name, sizeof(VLIST));
			if(line < 0 || line > nlines) {
				if(rflag == 0) {		/* not processing a root */
					err_erase= 0;		/* cause error message to
										   be erased later */
					tcup(T_VERROR, 0);
					tecline_;
					fputs("Bad index number or variable name.", stdout);
					tflush_;
					continue;
				} else {
					rxerr("access_m(): Bad index num or var name");
					error= -1;
					goto out;
				}
			}
			vlp= &mp->me_vlp[line];
			if(vlp->vl_type == ME_SUBMENU) {
				ME_RECUR recur;
				/*
				 * Recurse and call access_m() again.  In case of a submenu
				 * vl_add field is pointer to submenus MENU struct.
				 * Pass next arg in input line to submenu access_m().
				 * Rest of args in input line are flushed.  If errors
				 * are found in submenu access_m()s when processing a
				 * root the error status is returned all the way up;
				 * all higher up recursively called access_m()s also abort.
				 * Note: rflag must be tested here and not infd to determine
				 * if a root is being read.  When rxerr() is 
				 * called in an access_m() lower down comm will send message
				 * to process to terminate root reading;  this message will
				 * interrupt menu processing and be serviced, infd will be
				 * set to 0.  Then as the recursively called access_m()s
				 * return the test for infd would indicate that a root
				 * was not being read for that access_m().
				 */
				recur.me_rec_bp= rp;
				recur.me_rec_menup= (MENU *)vlp->vl_add;
				if(*pp == NP) recur.me_rec_astr= NS;
				else recur.me_rec_astr= *pp;
				error= access_m(ap, &recur);
				if( (error < 0 && rflag)
					||  (error > 0) ) goto out;
				goto begin;		/* reprint previous menu */
			}

			if(*pp == NP) goto bval;
			vadd= (u_int)vlp->vl_add;
			if(vlp->vl_flag & ME_GB) vadd += mp->me_basep;
			if(vlp->vl_flag & ME_LB) vadd += (u_int)vlp->vl_basep;
			if(vlp->vl_flag & ME_ILB) vadd += *(u_int *)vlp->vl_basep;
			if(vlp->vl_flag & ME_IGB) vadd += *(u_int *)mp->me_basep;

			switch(vlp->vl_type) {
			case ME_ACHAR:
				if(isdigit(*pp[0])) goto bval;
				tvadd[0]= *pp[0];
				break;

			case ME_UDCHAR:
				ctmp= atoi(*pp);
				if(ctmp < 0 || ctmp > 255) goto bval;
				ctmp &= 0377;
				*(int *)tvadd= ctmp;
				break;

			case ME_STR:
				stufs(*pp, tvadd, tvadd + P_LNAME);
				break;

			case ME_OCT:
				base= 8; lflag= 0; fflag= 0;
				goto convert;

			case ME_DEC:
				base= 10; lflag= 0; fflag= 0;
				goto convert;

			case ME_HEX:
				base= 16; lflag= 0; fflag= 0;
				goto convert;

			case ME_LOCT:
				base= 8; lflag= 1; fflag= 0;
				goto convert;

			case ME_LDEC:
				base= 10; lflag= 1; fflag= 0;
				goto convert;

			case ME_LHEX:
				base= 16; lflag= 1; fflag= 0;
				goto convert;

			case ME_NVALO:
				base= 8; lflag= 0; fflag= 0;
				if((pp[0][0] == '-') && (pp[0][1] == '\0')) {
					*(int *)tvadd= NULLI;
					break;
				}
				goto convert;
		
			case ME_NVALD:
				base= 10; lflag= 0; fflag= 0;
				if((pp[0][0] == '-') && (pp[0][1] == '\0')) {
					*(int *)tvadd= NULLI;
					break;
				}
				goto convert;
		
			case ME_NVALH:
				base= 16; lflag= 0; fflag= 0;
				if((pp[0][0] == '-') && (pp[0][1] == '\0')) {
					*(int *)tvadd= NULLI;
					break;
				}
				goto convert;
		
			case ME_NVALLO:
				base= 8; lflag= 1; fflag= 0;
				if((pp[0][0] == '-') && (pp[0][1] == '\0')) {
					*(long *)tvadd= NULLI_L;
					break;
				}
				goto convert;
		
			case ME_NVALLD:
				base= 10; lflag= 1; fflag= 0;
				if((pp[0][0] == '-') && (pp[0][1] == '\0')) {
					*(long *)tvadd= NULLI_L;
					break;
				}
				goto convert;
		
			case ME_NVALLH:
				base= 16; lflag= 1; fflag= 0;
				if((pp[0][0] == '-') && (pp[0][1] == '\0')) {
					*(long *)tvadd= NULLI_L;
					break;
				}
				goto convert;
		
			case ME_NVALACTARG:
				base= 0; lflag= 1; fflag= 0;
				if((pp[0][0] == '-') && (pp[0][1] == '\0')) {
					*(long *)tvadd= NULLI_L;
					break;
				}
				goto convert;
			case ME_FLOAT:
				base= 0; lflag= 0; fflag= 1;
				goto convert;

			convert:
				if(lflag == 1) {
					*(u_long *)tvadd= (u_long)strtoul(*pp, &rptr, base);
				}
				else if(fflag == 1) {
					*(float *)tvadd= (float)strtod(*pp, &rptr);
				}
				else {
					*(int *)tvadd= (int)strtol(*pp, &rptr, base);
				}
					

				/*
				 * The 'strtol' function stops converting when it hits
				 * an incorrect char- e.g. a '9' when the base is octal.
				 * Therefore, check that the function successfully
				 * converted the entire string.
				 */
				if(rptr != (*pp + strlen(*pp))) goto bval;
				break;

			default:
				if(rflag) {
					rxerr("access_m(): Undefined menu variable type");
					error= -1;
					goto out;
				} else {
					err_erase= 0;
					tcup(T_VERROR, 0);
					tecline_;
					fputs("Undefined menu variable type", stdout);
					tflush_;
					continue;
				}
			}	/* end switch */

			if(vlp->vl_flag & ME_BEF) {

				/*
				 * Call variable access function.  If minus return
				 * dont change and abort.  If positive return
				 * dont change and continue;  this is considered a
				 * soft error.
				 */
				fp= vlp->vl_accf;
				errvaf= (*fp)(ME_BEF, mp, astr, vlp, tvadd);
				if(errvaf > 0) continue;
				if(errvaf < 0) goto bval;
			}

			/*
			 * Now change acutal menu variable.
			 */
			if( (vlp->vl_type == ME_ACHAR)
				||  (vlp->vl_type == ME_UDCHAR) ) {

				*(char *)vadd= *(char *)tvadd;
			} else if(vlp->vl_type == ME_STR) {

				stufs((char *)tvadd, (char *)vadd, (char *)vadd + P_LNAME);

				/*
				 * Remaining cases are for int variables- either long or short.
				 */
			} else if(lflag) {

				*(long *)vadd= *(long *)tvadd;
			} else {

				*(int *)vadd= *(int *)tvadd;
			}

			if(vlp->vl_flag & ME_AFT) {
				fp= vlp->vl_accf;
				(*fp)(ME_AFT, mp, astr, vlp, tvadd);
			}
			if(echo && (ap->ap_type == ME_MENU)) {
				tcup(T_SCTOP+line, 1);
				(*ap->ap_pline)(mp, vlp, line, stdout);
			}
			continue;

		bval:
			if(rflag) {
				rxerr("access_m(): Bad value");
				error= -1;
				goto out;
			} else {
				err_erase= 0;
				tcup(T_VERROR, 0);
				tecline_;
				fputs("Bad value", stdout);
				tflush_;
			}

		}   /* end inner for(;;) */

		/*
		 * When changing statelist varibles update screen only after
		 * all vars specified on an input line have been changed.
		 */
		if(echo && (ap->ap_type == ME_SLIST))
			(*ap->ap_pline)(mp, vlp, line, stdout);

    }	/* end outer for(;;) */

 out:
	if( (mp->me_flag & ME_AFT)
		&&  ((fp= mp->me_accf) != NP) ) {
		(*fp)(ME_AFT, mp, astr, rp);
	}
	if(echo && (ap->ap_type == ME_MENU)) {
		ttop_;
		tebcur_;
		tflush_;
	}
	return(error);
}

/*
 * Print menu on tty.
 */
pr_menu(mp)
	MENU *mp;
{
	VLIST *vlp;
	char *sp;
	int ntabs, len, lcnt, cnt, line;

	if(echo) {
		ttop_;
		tebcur_;
	}

	/*
	 * Print help message if present.  This is much faster when
	 * hardware tabs are used.
	 */
	if( (*mp->me_help != '\0')
		&& echo ) {

	    /*
	     * Print help message beside menu fitting it in to right
	     * of margin specified by T_CPHELP.  Assume tab stops of 8
	     * wide.
	     */
	    ntabs= (T_CPHELP+6) >> 3;	/* tabs between col 0 and help margin
									   add 6 instead of 7 for columns start
									   with number 1 */
	    len= (10-ntabs) << 3;	/* len after help margin;  assume
								   10 tabs/screen (80 col) */
	    sp= mp->me_help;

	    for(lcnt= 0; lcnt < T_MENUMAX; lcnt++) {

			if(*sp == '\0') break;		/* done */
			cnt= ntabs;			/* tab over to margin */
			while(cnt--) putchar('\t');
			cnt= len;

			/*
			 * If line is too long for len after margin, split it
			 * and continue on next line.
			 */
			while(cnt--) {
				if(*sp == '\0') break;	/* done */
				if(*sp == '\n') {	/* new line */
					sp++;
					break;
				}
				putchar(*sp++);
			}
			putchar('\n');
	    }
	    ttop_;
	}

	/*
	 * Print menu entries.
	 */
	for(vlp= mp->me_vlp, line= 0; vlp->vl_name[0] != NULL; vlp++) {

		if(line >= T_MENUMAX) {
			rxerr("pr_menu(): Too many items in menu");
			return(-1);
		}
		if(echo) pr_line(mp, vlp, line, stdout);
		line++;
	}

	if(echo) tflush_;
	return(line);
}

/*
 * Print single menu line.  Called both to print on tty and to write
 * to root file.  Cursor must already be properly positioned when printing
 * on tty; no cursor positioning is done.  When done a flush is NOT
 * performed.  When writing to root file index number is not printed.
 */
int
pr_line(MENU *mp, VLIST *vlp, int num, FILE *file)
{
	int cnt, lflag, fflag;
	char *p;
	char *format, *namep;
	unsign vadd;
	int ctmp;

	if(file == stdout) {

		/*
		 * Erase current line and old value.
		 */
		tcup(-1, T_CPHELP-2);	/* advance cursor to help position-1;
								   sub 2 because cols start with 1 */
		teltcur_;		/* erase line to cursor */
		putchar('\r');		/* return cursor to beginning of line */
		printf("%d.", num);
		putchar(' ');
		if(num < 10) putchar(' ');
	}

	namep= vlp->vl_name;
	fputs(namep, file);

	/*
	 * Get len of variable name to determine how many tabs needed
	 * between name and value.
	 */
	p= namep;
	while(*p++ != '\0');

	/*
	 * Correct count for number field if present;  subtract 1 for
	 * null termination that was counted.
	 */
	cnt= (p - namep) + (file == stdout ? 3 : -1);
	cnt= (T_CPVALUE - cnt) + 6;	/* first col is 1 not 0 */
	cnt >>= 3;		/* num of tabs */
	while(cnt--) putc('\t', file);

	/*
	 * Get address of value and print according to type.
	 */
	if(vlp->vl_flag != ME_SUBMENU) {
		vadd= (u_int)vlp->vl_add;
		if(vlp->vl_flag & ME_GB) vadd += mp->me_basep;
		if(vlp->vl_flag & ME_LB) vadd += (u_int)vlp->vl_basep;
		if(vlp->vl_flag & ME_ILB) vadd += *(unsign *)vlp->vl_basep;
		if(vlp->vl_flag & ME_IGB) vadd += *(unsign *)mp->me_basep;
	}

	/*
	 * Print value.
	 */
	switch(vlp->vl_type) {
	case ME_ACHAR:
		lflag= 0;
		fflag= 0;
		format= "%1c\n";
		break;

	case ME_UDCHAR:
		lflag= 0;
		fflag= 0;
		ctmp= *(char *)vadd;
		ctmp &= 0377;
		vadd= (u_int)&ctmp;
		format= "%d\n";
		break;

	case ME_STR:
		lflag= 0;
		fflag= 0;
		ctmp= vadd;
		vadd= (u_int)&ctmp;
		format= "%s\n";
		break;

	case ME_OCT:
		lflag= 0;
		fflag= 0;
		format= "%#o\n";
		break;

	case ME_DEC:
		lflag= 0;
		fflag= 0;
		format= "%d\n";
		break;

	case ME_HEX:
		lflag= 0;
		fflag= 0;
		format= "%#x\n";
		break;

	case ME_LOCT:
		lflag= 1;
		fflag= 0;
		format= "%#lo\n";
		break;

	case ME_LDEC:
		lflag= 1;
		fflag= 0;
		format= "%ld\n";
		break;

	case ME_LHEX:
		lflag= 1;
		fflag= 0;
		format= "%#lx\n";
		break;

	case ME_NVALO:
		if(*(int *)vadd == NULLI) {
			fputs("-\n", file);
			return(0);
		}
		lflag= 0;
		fflag= 0;
		format= "%#o\n";
		break;

	case ME_NVALD:
		if(*(int *)vadd == NULLI) {
			fputs("-\n", file);
			return(0);
		}
		lflag= 0;
		fflag= 0;
		format= "%d\n";
		break;

	case ME_NVALH:
		if(*(int *)vadd == NULLI) {
			fputs("-\n", file);
			return(0);
		}
		lflag= 0;
		fflag= 0;
		format= "%#x\n";
		break;

	case ME_NVALLO:
		if(*(long *)vadd == NULLI_L) {
			fputs("-\n", file);
			return(0);
		}
		lflag= 1;
		fflag= 0;
		format= "%#lo\n";
		break;

	case ME_NVALLD:
		if(*(long *)vadd == NULLI_L) {
			fputs("-\n", file);
			return(0);
		}
		lflag= 1;
		fflag= 0;
		format= "%ld\n";
		break;

	case ME_NVALLH:
	case ME_NVALACTARG:
		if(*(long *)vadd == NULLI_L) {
			fputs("-\n", file);
			return(0);
		}
		lflag= 1;
		fflag= 0;
		format= "%#lx\n";
		break;

	case ME_FLOAT:
		lflag= 0;
		fflag= 1;
		format= "%.2f\n";
		break;
	case ME_SUBMENU:
		fputs("~Submenu\n", file);
		return(0);

	}

	if(lflag == 1) fprintf(file, format, *(unsigned long *)vadd);
	else if(fflag == 1) fprintf(file, format, *(float *)vadd);
	else fprintf(file, format, *(unsigned int *)vadd);

	fflush(file);
	return(0);
}

/*
 * Access parameter struct for normal menu accesses.
 */
ME_ACCP me_accp = {
	pr_menu, pr_line, ME_MENU,
};

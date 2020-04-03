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
 *	Routines to access and change variables associated
 * with states.
 */

#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <sys/types.h>
#include "rexhdrs.h"
#include "tty.h"
#include "int.h"

/*
 * State variable changing uses the menu access() function.  Following
 * table drives this function specifing differences necessary when
 * changing state variables rather then menu variables.
 */
ME_ACCP st_ap = {
	pr_state, pr_state, ME_SLIST,
};

/*
 * Access functions for state VLIST.
 */
#pragma off (unreferenced)
int
flag_af(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd)
{
#pragma on (unreferenced)
	if(*tvadd < 0 || *tvadd > 3) {
		rxerr("flag_af(): Flag must be > 0 and < 3");
		return(-1);
	}
	return(0);
}

/*
 * VLIST struct for state.
 * Note: names below are not printed on screen (state is printed by
 * pr_state()), however they are used by access() to select each variable
 * for changing.  State variables are selected by name only, not by
 * index number or name as for regular menu variables.
 */
VLIST st_var[] = {
"preset", 		&((STATE *)NP)->preset, NP, NP, ME_GB, ME_LDEC,
"random",		&((STATE *)NP)->random, NP, NP, ME_GB, ME_DEC,
"type_flag_rl",		&((STATE *)NP)->rlflag, NP, flag_af, ME_BEF|ME_GB,
			    ME_UDCHAR,
"value_rl",		&((STATE *)NP)->rlval, NP, NP, ME_GB, ME_UDCHAR,
"a_arg",		&((STATE *)NP)->device.act_code, NP, NP, ME_GB,
			    ME_NVALACTARG,
"b_arg",		&((STATE *)NP)->device.act_2code, NP, NP, ME_GB,
			    ME_NVALACTARG,
"c_arg",		&((STATE *)NP)->device.act_3code, NP, NP, ME_GB,
			    ME_NVALACTARG,
"d_arg",		&((STATE *)NP)->device.act_4code, NP, NP, ME_GB,
			    ME_NVALACTARG,
"e_arg",		&((STATE *)NP)->device.act_5code, NP, NP, ME_GB,
			    ME_NVALACTARG,
"f_arg",		&((STATE *)NP)->device.act_6code, NP, NP, ME_GB,
			    ME_NVALACTARG,
"g_arg",		&((STATE *)NP)->device.act_7code, NP, NP, ME_GB,
			    ME_NVALACTARG,
"h_arg",		&((STATE *)NP)->device.act_8code, NP, NP, ME_GB,
			    ME_NVALACTARG,
"i_arg",		&((STATE *)NP)->device.act_9code, NP, NP, ME_GB,
			    ME_NVALACTARG,
"j_arg",		&((STATE *)NP)->device.act_10code, NP, NP, ME_GB,
			    ME_NVALACTARG,
NS,
};

/*
 * MENU struct for changing state variables.
 * Name is not used and is a dummy string.
 */
MENU stmenu = {
"x",			&st_var, NP, NP, 0, NP, NP,
};

/*
 * Print state names, select state, call access() to change
 * variables in state.
 */
int
s_access(STATE *bsp[], char *vstr, char *astr)
{
	STATE **es, **sp;
	ME_RECUR recur;
	char *pbuf[P_NARG];
	char is[P_ISLEN];
	int pcnt, error= 0, sx, snum;
	int st_page;			/* states per page */
	int curstate;			/* beg state on current page */
	int last_state;			/* state number that begins last
					   page */
	int nstate;			/* total states */

	if(*vstr == 't')
	   st_page= (2 * ((T_SBOT - T_SCTOP) + 1)); /* max states displayed
					               per page, 2 columns */
	else
	   st_page= (5 * ((T_SBOT - T_SCTOP) + 1)); /* max states displayed
						       per page */
	for(es= bsp, nstate= 0; *es != 0; es++, nstate++);
	last_state= nstate - st_page;
	if(last_state < 0) last_state= 0;

	if(echo) {
		if(astr != '\0') curstate= atoi(astr) * st_page;
		else curstate= 0;
		pr_snames(bsp, vstr, curstate, st_page, last_state);
	}

	/*
 	 * Main loop to select state name and change its variables.
	 */
	for(;;) {
	    if(echo) {
		tcup(T_SPROMPT, 1);
		tecline_;
	    	if(last_state == 0) fputs(": ", stdout);
	    	else if(curstate > 0 && curstate < last_state)
	    		fputs("< More states > : ", stdout);
	    	else if(curstate == 0)
	    		fputs("More states > : ", stdout);
	    	else fputs("< More states : ", stdout);
		tflush_;
	    }

	    if( (tgetl(is, &is[P_ISLEN]) <= 0)
	    ||  (pcnt= parse(1, is, pbuf, P_NARG)) < 0 ) {
		if(infd == 0) continue;
		error= -1;
		goto out;
	    }
	    if(pcnt == 0) break;		/* done */

	    /*
	     * Look for commands to print forward and backward pages
	     * of state name list.
	     */
	    if(echo) {
	    	if(pbuf[0][0] == '>') {
	    	
	    		/*
	    		 * Print next page of states.
	    		 */
	    		curstate += st_page;
	    		pr_snames(bsp, vstr, curstate, st_page, last_state);
	    		continue;
	    	}
	    	if(pbuf[0][0] == '<') {
	    		curstate -= st_page;
	    		pr_snames(bsp, vstr, curstate, st_page, last_state);
	    		continue;
	    	}
	    }
	    	    
	    /*
	     * Search for state name.
	     */
	    for(sp= bsp, snum= 0; *sp != 0; sp++, snum++);
	    if(isdigit(pbuf[0][0])) {
		sx= atoi(pbuf[0]);
		if(sx >= snum) goto badname;
		sp= &bsp[sx];
	    } else {
		for(sp= bsp; *sp != 0; sp++)
		    if(match(pbuf[0], (*sp)->statename)) break;
		if(*sp == 0) {

		badname:
		    if(infd == 0) {
			tcup(T_SERROR, 1);
			tecline_;
			fputs("Bad state name", stdout);
			tflush_;
			continue;
		    } else {
			rxerr("s_access(): Bad state name");
			error= -1;
			goto out;
		    }
		}
	    }
	    stmenu.me_basep= (unsign)*sp;

	    /*
	     * Call menu access function to change state variables.
	     */
	    recur.me_rec_bp= NP;
	    recur.me_rec_menup= &stmenu;
	    recur.me_rec_astr= NS;
	    if(access_m(&st_ap, &recur) != 0) {
		error= 1;
		goto out;
	    }

	    /*
	     * Erase old state and possibly select another.
	     */
	    if(echo) {
		tcup(T_SPRINT, 1);
		tebcur_;
		tflush_;
	    }
	}

out:
	if(echo) {
		tescroll_;
		tflush_;
	}
	return(error);
}

/*
 * Print names of all states on screen.
 */
#pragma off (unreferenced)
int
pr_snames(STATE *bsp[], char *vstr, int curstate, int st_page, int last_state)
{
#pragma on (unreferenced)

	int line, tcnt;
	int scnt, toff;
	char os[11];		/* max width of state name */

	/*
	 * Get starting state.
	 */
	if(curstate < 0) curstate= 0;
	if(curstate > last_state) curstate= last_state;
	scnt= curstate, bsp += curstate;

	ttop_;
	tebcur_;
	if(*vstr == 's') {
	  for(toff= 0; toff <= 8; toff += 2) {
	    for(line= T_SCTOP; line <= T_SBOT; line++) {
		    if(*bsp == 0) goto done;
		    tcnt= toff;
		    while(tcnt--) putchar('\t');
		    printf("%d. ", scnt);
		    if(scnt < 10) putchar(' ');
		    
		    /*
		     * Limit string to size of column.
		     */
		    stufs((*bsp++)->statename, os, os + sizeof(os));
		    fputs(os, stdout);
		    putchar('\n');
		    scnt++;
	    }
	    ttop_;
	  }
	} else if(*vstr == 't') {
	  for(toff= 0; toff <= 8; toff += 4) {
	    for(line= T_SCTOP; line <= T_SBOT; line++) {
		    if(*bsp == 0) goto done;
		    tcnt= toff;

		   /*
		    * For the 't' verb, print out states that have
		    * non-zero times, along with the times (from meg).
		    */
		   if ((*bsp)->preset != 0) {
			tcnt= toff;
			while(tcnt--) putchar('\t');
			printf("%d. ", scnt);
			if(scnt < 10) putchar(' ');
			printf("%-12s %6d %6d\n", (*bsp)->statename,
			       (*bsp)->preset, (*bsp)->random);
		   } else line--;	    /* allow full column in t s */
		   bsp++;
		   scnt++;
	    }
	    ttop_;
	  }
	}
done:
	return(0);
}

/*
 * Called to write state info to root files.
 */
int
mksroot(void)
{
	STATE **sp= snames;
	extern FILE *rtout;

	fputs("~\n~State set variables:\n", rtout);
	fputs("set statelist\n", rtout);
	for(; *sp != 0; sp++) {
		fputs((*sp)->statename, rtout);
		fputc('\n', rtout);
		s_print(*sp, rtout);
		fputs("~\n", rtout);
	}
	fputs("~\n", rtout);
	return(0);
}

/*
 * Interface function to allow access() to call s_print() to
 * print state info.  Necessary because access() does not supply
 * proper arguments.
 */
int
pr_state(MENU *mp)
{
	if(echo) s_print((STATE *)mp->me_basep, stdout);

	/*
	 * The first time this function is called by access() it
	 * must return the number of lines in VLIST.  Make sure this
	 * is more than enough for a state.
	 */
	return(50);
}

/*
 * Print state.  Called both to print on tty and to write to root file.
 */
int
s_print(STATE *sp, FILE *file)
{
	int value, flag;
	long a_code, b_code, c_code;
	long d_code, e_code, f_code;
	long g_code, h_code, i_code, j_code;

	if(file == stdout) {		/* not writing root */
	    tcup(T_SPRINT, 1);
	    tecline_;
	    putchar('\n');
	    tecline_;
	    trindex_;
	    putchar('\"');
	    fputs(sp->statename, file);
	    fputs("\"  ", file);
	}
	flag= sp->rlflag;
	value= sp->rlval & 0377;
	fprintf(file, "preset %ld  random %d  type_flag_rl %d  value_rl %d\n",
			sp->preset, sp->random, flag, value);

	a_code= sp->device.act_code;
	b_code= sp->device.act_2code;
	c_code= sp->device.act_3code;
	d_code= sp->device.act_4code;
	e_code= sp->device.act_5code;
	f_code= sp->device.act_6code;
	g_code= sp->device.act_7code;
	h_code= sp->device.act_8code;
	i_code= sp->device.act_9code;
	j_code= sp->device.act_10code;

	/*
	 * If not writing a root print hex and dec.  Else just print
	 * hex.
	 */
	if(file == stdout) {
		printf("a_arg %#lx %ld b_arg %#lx %ld c_arg %#lx %ld\n",
			   a_code, a_code,
			   b_code, b_code,
			   c_code, c_code);
		printf("d_arg %#lx %ld e_arg %#lx %ld f_arg %#lx %ld\n",
			   d_code, d_code,
			   e_code, e_code,
			   f_code, f_code);
		printf("g_arg %#lx %ld h_arg %#lx %ld i_arg %#lx %ld j_arg %#lx %ld\n",
			   g_code, g_code,
			   h_code, h_code,
			   i_code, i_code,
			   j_code, j_code);
	} else {
		fprintf(file, "a_arg %#lx  b_arg %#lx  c_arg %#lx\n",
				a_code, b_code, c_code);
		fprintf(file, "d_arg %#lx  e_arg %#lx  f_arg %#lx\n",
				d_code, e_code, f_code);
		fprintf(file, "g_arg %#lx  h_arg %#lx  i_arg %#lx j_arg %#lx\n",
				g_code, h_code, i_code, j_code);
	}
	return(0);
}

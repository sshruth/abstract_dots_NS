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

%token  STATE CODE TIME RANDOM TO MINUS PLUS RUNL FIRST
%token ABORT NAME STATUS ACTION ON PNUM
%token RESTART

%union {
	int ival;
	char cval;
	char *cpval;
}

%token <cpval> STRING
%token <cpval> CAPS
%token <cpval> NUMBER
%token <cpval> ADDRESS


%type <cpval> newnum
%type <cpval> newstring
%type <cpval> address
%type <cpval> sign

%%

statelist:	PNUM newnum restart all
			{
				pnum = $2;
			}
		| statelist all
		;

restart:	/* empty */
		| RESTART ':' newstring
			{
				*fnptr++ = $3;
			}
		| RESTART newstring
			{
				*fnptr++ = $2;
			}
		;

all:		newstring '{' status states abort '}'
			{
				if(!first_seen) {
				    fprintf(stderr,
				      "CHAIN '%s' has no 'begin' state\n",
					    $1);
				    exit(1);
				}
				*pnames++ = $1;
				chain_cnt++;
				first_seen= 0;
				abcount++;
			}
		| newstring '{' status states '}'
			{
				if(!first_seen) {
				  fprintf(stderr,
				    "CHAIN '%s' has no 'begin' state\n",
					    $1);
				  exit(1);
				}
				*pnames++ = $1;
				chain_cnt++;
				first_seen= 0;
				abcount++;
			}
		;

status:		STATUS newnum
		{
			*pstat++ = $2;
		}
		;

states:		shortstate
			{
				sptr++;
				index = 0;
			}
		| states shortstate
			{
				sptr++;
				index = 0;
			}
		;

shortstate:	newstring ':' rest
			{
				sptr->sn = $1;
				if (index >= MAXESC) {
					fprintf(stderr,
						"STATE %s: too many escapes\n",
						sptr->sn);
					exit(1);
				}
			}
		| FIRST newstring ':' rest
			{
				if(strncmp(zero, sptr->sa, sizeof(zero)) !=
				   0) {
				   fprintf(stderr,
				     "STATE '%s': 'begin' state cannot contain an action\n",
					   $2);
			           exit(1);
				}
				if(first_seen) {
				    fprintf(stderr,
			"STATE '%s': only one 'begin' state per chain allowed\n",
					$2);
				    exit(1);
				} else first_seen= 1;
				*begstack++ = $2;
				sptr->sn = $2;
				if (index >= MAXESC) {
					fprintf(stderr,
						"STATE %s: too many escapes\n",
						sptr->sn);
					exit(1);
				}
			}
		;

rest:		CODE newnum ta ahead
			{
				sptr->sc = $2;
			}
		| CODE newnum ahead
			{
				sptr->sc = $2;
			}
		| ta ahead
		| ahead
		;

ta:		action runl time
		| action time runl
		| runl time action
		| runl action time
		| time runl action
		| time action runl
		;

action:		/* empty */
		| lact newstring '(' ')'
		{
			sptr->sa = $2;
		}
		| lact newstring '('  newstring ')'
			{
				sptr->sa = $2;
				sptr->sa1 = $4;
			}
		| lact newstring '(' newstring ','  newstring ')'
			{
				sptr->sa = $2;
				sptr->sa1 = $4;
				sptr->sa2 = $6;
			}
		| lact newstring '(' newstring ',' newstring ',' newstring ')'
			{
				sptr->sa = $2;
				sptr->sa1 = $4;
				sptr->sa2 = $6;
				sptr->sa3 = $8;
			}
		| lact newstring '(' newstring ',' newstring ',' newstring 
		  ',' newstring ')'
			{
				sptr->sa = $2;
				sptr->sa1 = $4;
				sptr->sa2 = $6;
				sptr->sa3 = $8;
				sptr->sa4 = $10;
			}
		| lact newstring '(' newstring ',' newstring ',' newstring 
		  ',' newstring ',' newstring ')'
			{
				sptr->sa = $2;
				sptr->sa1 = $4;
				sptr->sa2 = $6;
				sptr->sa3 = $8;
				sptr->sa4 = $10;
				sptr->sa5 = $12;
			}
		| lact newstring '(' newstring ',' newstring ',' newstring 
		  ',' newstring ',' newstring ',' newstring ')'
			{
				sptr->sa = $2;
				sptr->sa1 = $4;
				sptr->sa2 = $6;
				sptr->sa3 = $8;
				sptr->sa4 = $10;
				sptr->sa5 = $12;
				sptr->sa6 = $14;
			}
		| lact newstring '(' newstring ',' newstring ',' newstring 
		  ',' newstring ',' newstring ',' newstring ',' newstring ')'
			{
				sptr->sa = $2;
				sptr->sa1 = $4;
				sptr->sa2 = $6;
				sptr->sa3 = $8;
				sptr->sa4 = $10;
				sptr->sa5 = $12;
				sptr->sa6 = $14;
				sptr->sa7 = $16;
			}
		| lact newstring '(' newstring ',' newstring ',' newstring 
		  ',' newstring ',' newstring ',' newstring ',' newstring
	          ',' newstring ')'
			{
				sptr->sa = $2;
				sptr->sa1 = $4;
				sptr->sa2 = $6;
				sptr->sa3 = $8;
				sptr->sa4 = $10;
				sptr->sa5 = $12;
				sptr->sa6 = $14;
				sptr->sa7 = $16;
				sptr->sa8 = $18;
			}
		| lact newstring '(' newstring ',' newstring ',' newstring 
		  ',' newstring ',' newstring ',' newstring ',' newstring
	          ',' newstring ',' newstring ')'
			{
				sptr->sa = $2;
				sptr->sa1 = $4;
				sptr->sa2 = $6;
				sptr->sa3 = $8;
				sptr->sa4 = $10;
				sptr->sa5 = $12;
				sptr->sa6 = $14;
				sptr->sa7 = $16;
				sptr->sa8 = $18;
				sptr->sa9 = $20;
			}
		| lact newstring '(' newstring ',' newstring ',' newstring 
		  ',' newstring ',' newstring ',' newstring ',' newstring
	          ',' newstring ',' newstring ',' newstring ')'
			{
				sptr->sa = $2;
				sptr->sa1 = $4;
				sptr->sa2 = $6;
				sptr->sa3 = $8;
				sptr->sa4 = $10;
				sptr->sa5 = $12;
				sptr->sa6 = $14;
				sptr->sa7 = $16;
				sptr->sa8 = $18;
				sptr->sa9 = $20;
				sptr->sa10 = $22;
			}
		;
lact:		/* empty */
		| ACTION
		;
time:		/* empty */
		| TIME newnum
			{
				sptr->st = $2;
			}
		| RANDOM newnum
			{
				sptr->sr = $2;
			}
		| TIME newnum RANDOM newnum
			{
				sptr->st = $2;
				sptr->sr = $4;
			}
		| RANDOM newnum TIME newnum
			{
				sptr->sr = $2;
				sptr->st = $4;
			}
		;

runl:		/* empty */
		| RUNL newnum
			{
				sptr->sz = one;
				sptr->sl = $2;
			}
		| RUNL PLUS newnum
			{
				sptr->sz = two;
				sptr->sl = $3;
			}
		| RUNL MINUS newnum
			{
				sptr->sz = three;
				sptr->sl = $3;
			}
		;

ahead:		esc
			{
			}
		| ahead esc
			{
			}
		;

esc:		escape
		| esc escape
			{
			}
		;

escape:		TO newstring ON newnum '=' address
		{
			sptr->es[index].se = $2;
			sptr->es[index].sf = equal;
			sptr->es[index].sad = $6;
			sptr->es[index].sm = $4;
			index++;
		}
		| TO newstring ON sign newnum '=' address
		{
			char *tmp;

			/* this will work because a leading ' ' exists */
			if ($4 == bitoff) {
				$5 = $5 - 1;
				tmp = $5;
				*tmp = '-';
			}
			sptr->es[index].se = $2;
			sptr->es[index].sf = equal;
			sptr->es[index].sad = $7;
			sptr->es[index].sm = $5;
			index++;
		}
		| TO newstring ON sign newnum '&' address
		{
			sptr->es[index].se = $2;
			sptr->es[index].sf = $4;
			sptr->es[index].sad = $7;
			sptr->es[index].sm = $5;
			index++;
		}
		| TO newstring ON newnum '>' address
		{
			sptr->es[index].se = $2;
			sptr->es[index].sf = greater;
			sptr->es[index].sad = $6;
			sptr->es[index].sm = $4;
			index++;
		}
		| TO newstring ON sign newnum '>' address
		{
			char *tmp;

			/* this will work because a leading ' ' exists */
			if ($4 == bitoff) {
				$5 = $5 - 1;
				tmp = $5;
				*tmp = '-';
			}
			sptr->es[index].se = $2;
			sptr->es[index].sf = greater;
			sptr->es[index].sad = $7;
			sptr->es[index].sm = $5;
			index++;
		}
	        | TO newstring ON newnum '<' address
		{
			sptr->es[index].se = $2;
			sptr->es[index].sf = less;
			sptr->es[index].sad = $6;
			sptr->es[index].sm = $4;
			index++;
		}
		| TO newstring ON sign newnum '<' address
		{
			char *tmp;

			/* this will work because a leading ' ' exists */
			if ($4 == bitoff) {
				$5 = $5 - 1;
				tmp = $5;
				*tmp = '-';
			}
			sptr->es[index].se = $2;
			sptr->es[index].sf = less;
			sptr->es[index].sad = $7;
			sptr->es[index].sm = $5;
			index++;
		}
		| TO newstring ON newnum '?' address
		{
			sptr->es[index].se = $2;
			sptr->es[index].sf = question;
			sptr->es[index].sad = $6;
			sptr->es[index].sm = $4;
			index++;
		}
		| TO newstring ON sign newnum '?' address
		{
			char *tmp;

			/* this will work because a leading ' ' exists */
			if ($4 == bitoff) {
				$5 = $5 - 1;
				tmp = $5;
				*tmp = '-';
			}
			sptr->es[index].se = $2;
			sptr->es[index].sf = question;
			sptr->es[index].sad = $7;
			sptr->es[index].sm = $5;
			index++;
		}
		| TO newstring ON newnum '%' address
		{
			sptr->es[index].se = $2;
			sptr->es[index].sf = function;
			sptr->es[index].sad = $6;
			sptr->es[index].sm = $4;
			index++;
		}
		| TO newstring ON sign newnum '%' address
		{
			char *tmp;

			/* this will work because a leading ' ' exists */
			if ($4 == bitoff) {
				$5 = $5 - 1;
				tmp = $5;
				*tmp = '-';
			}
			sptr->es[index].se = $2;
			sptr->es[index].sf = function;
			sptr->es[index].sad = $7;
			sptr->es[index].sm = $5;
			index++;
		}
		| TO newstring
		{
			sptr->es[index].se = $2;
			sptr->es[index].sf = time;
			index++;
		}
		| TO newstring ON TIME
		{
			sptr->es[index].se = $2;
			sptr->es[index].sf = time;
			index++;
		}

address:	newnum
			{
				$$ = $1;
			}
		| newstring
			{
				$$ = $1;
			}

sign:		PLUS
			{
				$$ = biton;
			}
		| MINUS
			{
				$$ = bitoff;
			}
		

abort:		ABORT newstring ':' ab1
		| ABORT ':' ab1
		;

ab1:		/* empty */
		| newstring
			{
				ablist[abcount++] = $1;
			}
		| ab1 newstring
			{
				ablist[abcount++] = $2;
			}
		;

newstring:	STRING
			{
			$$ = $1;
			}
		;

newnum:		NUMBER
			{
				$$ = $1;
			}
		| CAPS
			{
				$$ = $1;
			}
		| ADDRESS
			{
				$$ = $1;
			}
		;

%%
#include <stdio.h>
#include "lex.yy.c"
#include <ctype.h>
#include "sp.h"

/* 
 * Commonly used strings.
 */
char *time = {"TIME"};
char *bitoff = {"BITOFF"};
char *biton = {"BITON"};
char *function = {"FUNC"};
char *equal = {"EQUAL"};
char *less = {"LESSOR"};
char *greater = {"GREATER"};
char *question = {"QUERY"};
char *zero = {"0"};
char *one = {"1"};
char *two = {"2"};
char *three = {"3"};

struct names {
	char *sn;	/* state name */
	char *sc;	/* ecode */
	char *sa,*sa1,*sa2,*sa3,*sa4,*sa5,*sa6,*sa7,*sa8,*sa9,*sa10;    /* action and args */
	char *sl;	/* running line flag */
	char *sz;	/* running line value */
	char *st,*sr;	/* preset time, random time */
	struct escp {	/* escapes */
		char *se,*sf,*sad,*sm;
	} es[MAXESC];
	struct names *nptr;
};
struct names list[MAXSTATE] = {0};  /* list of states */
struct names *sptr = list;	/* pointer to current state yacc is processing */
char buffer[MAXLEXBUF] = {0};	/* buffer for lex */
char *lptr = buffer - 1;	/* pointer to null term of last string */
char *begbuf[MAXCHAIN] = {0};	/* pointers to start (FIRST) state of each
				   chain */
char **begstack = begbuf;
char *names[MAXCHAIN] = {0};	/* names of chains */
char **pnames = names;
char *stat[MAXCHAIN] = {0};	/* status of chains (on or off) at init time */
char **pstat = stat;
char *ablist[MAXCHAIN] = {0};	/* aborts of each chain; term by null ptr */
int abcount = 0;		/* number of aborts */
char ab = 'a';
char *fnlist[MAXCHAIN] = {0};	/* restart function names */
char **fnptr = fnlist;
char *pnum = {0};		/* paradigm number */
int i = {0};
int index = {0};
int chain_cnt = {0};		/* total number of chains */
extern FILE *outfd;
int first_seen= 0;

/* 
 * Put out the required #includes.
 */
void
includes(void)
{
	fprintf(stdout,"#include <stdio.h>\n");
	fprintf(stdout,"#include <sys/types.h>\n");
	fprintf(stdout,"#include \"../hdr/sys.h\"\n");
	fprintf(stdout,"#include \"../hdr/cnf.h\"\n");
	fprintf(stdout,"#include \"../hdr/proc.h\"\n");
	fprintf(stdout,"#include \"../hdr/cdsp.h\"\n");
	fprintf(stdout,"#include \"../hdr/idsp.h\"\n");
	fprintf(stdout,"#include \"../hdr/buf.h\"\n");
	fprintf(stdout,"#include \"../hdr/menu.h\"\n");
	fprintf(stdout,"#include \"../hdr/state.h\"\n");
	fprintf(stdout,"#include \"../hdr/ecode.h\"\n");
	fprintf(stdout,"#include \"../hdr/device.h\"\n");
	fprintf(stdout,"#include \"../hdr/int.h\"\n");
	fprintf(stdout,"\n\n");
}

/* 
 * Put out the states.
 */
void
states_names(void)
{
	register struct names *fptr;
	register struct escp *eptr;
	register int i;

	for (fptr = list; fptr < sptr; fptr++) {
		fprintf(stdout,"STATE s%s = {\n",fptr->sn);
		if(fptr->sc[0] == '&') fprintf(stdout,"\t{\"%s\"},%s,%s\n",fptr->sn,zero,fptr->sc);
		else fprintf(stdout,"\t{\"%s\"},%s,%s\n",fptr->sn,fptr->sc,zero);
		fprintf(stdout,"\t,%s,%s\n",fptr->sz,fptr->sl);
		fprintf(stdout,"\t,{%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s}\n",
				fptr->sa,fptr->sa1,fptr->sa2,fptr->sa3,fptr->sa4,fptr->sa5,
				fptr->sa6,fptr->sa7, fptr->sa8, fptr->sa9, fptr->sa10);
		fprintf(stdout,"\t,%s,%s\n",fptr->st,fptr->sr);

		eptr = fptr->es;
		for (i = 0; i < MAXESC; i++) {
			if(i == 0) fprintf(stdout, "\t,{\n");
			if (*eptr->se == '0') {
				fprintf(stdout,"\t{0,0,%s,%s,",eptr->se,eptr->sf);
				fprintf(stdout,"%s,%s}",eptr->sad,eptr->sm);
			}
			else {
				fprintf(stdout,"\t{0,0,&s%s,%s,",eptr->se,eptr->sf);
				if (islower(*eptr->sad))
					fprintf(stdout,"&%s,%s}",eptr->sad,eptr->sm);
				else fprintf(stdout,"%s,%s}",eptr->sad,eptr->sm);
			}
			if(i != MAXESC-1) fprintf(stdout, ",\n");
			else fprintf(stdout, "\n\t}\n};\n");
			eptr++;
		}
	}
	fprintf(stdout,"\n\n\n");
}

/* 
 * Put out all needed external declarations.
 */
void
globals_names(void)
{
	register int i;
	register struct names *fptr;
	register struct escp *eptr;

	printf("\n\n");
	for (fptr = list; fptr < sptr; fptr++) {
		if (islower(*fptr->sa)) {
			fprintf(stdout,"extern int %s();\n",fptr->sa);
		}
		eptr = fptr->es;
		for (i = 0; i < MAXESC; i++) {
 		  if(islower(*eptr->sad)) {
		    if(eptr->sf == function) {
		      fprintf(stdout,"extern int %s();\n",eptr->sad);
		    } else {
		      char *cp;

		      cp= eptr->sad;

		      /*
		       * Don't output extern declaration if variable is
		       * a struct or array.  Let user do it explicitly.
		       */
		      while(*cp != '\0') {
			if(*cp == '[' || *cp == '.') break;
			cp++;
		      }
		      if(*cp == '\0')
	      		 fprintf(stdout,"extern int %s;\n",eptr->sad);
		    }
		  }
		  eptr++;
		}
	}
	fprintf(stdout,"\n\n\n");

	for (fptr = list; fptr < sptr; fptr++) {
		fprintf(stdout,"STATE s%s;\n",fptr->sn);
	}
	fprintf(stdout,"\n\n\n");
	fprintf(stdout,"STATE *snames[] = {\n");
	for (fptr = list; fptr < sptr; fptr++) {
		fprintf(stdout,"&s%s,\n",fptr->sn);
	}
	fprintf(stdout,"0};\n\n");
}

/*
 * Zero state tables.
 */
void
ilist(void)
{
	register struct names *kptr;
	register int i;

	for (kptr = list; kptr < &list[MAXSTATE]; kptr++) {
			kptr->sn = zero;
			kptr->sc = zero;
			kptr->sl = zero;
			kptr->sz = zero;
			kptr->sa = zero;
			kptr->sa1 = zero;
			kptr->sa2 = zero;
			kptr->sa3 = zero;
			kptr->sa4 = zero;
			kptr->sa5 = zero;
			kptr->sa6 = zero;
			kptr->sa7 = zero;
			kptr->sa8 = zero;
			kptr->sa9 = zero;
			kptr->sa10 = zero;
			kptr->st = zero;
			kptr->sr = zero;
			for (i = 0; i < MAXESC; i++) {
				kptr->es[i].sf = zero;
				kptr->es[i].sad = zero;
				kptr->es[i].se = zero;
				kptr->es[i].sm = zero;
			}
	}
}

/*
 * Get out the structures which are used by the state processor.
 */
void
inittable(void)
{
	register char **fptr;
	int i;

	fprintf(stdout,"int sf_init();\n\n");
	fprintf(stdout,"STATE sps_state;\n\n");
	fprintf(stdout,"AWAKE init_state[] = {\n");
	fprintf(stdout,"\t{0,ON,ON,&sps_state,&sps_state,0,(STATE *)&init_state[1],0,\n\
			&sps_state.escape}\n");
	fprintf(stdout,"\t,{0,0,0,0,0,0,0,0,&sps_state.escape[1]}\n};\n");

	fprintf(stdout,"STATE sps_state = {\n");
	fprintf(stdout, "\t{\"spec\"},1,0\n\t,1,0\n");
	fprintf(stdout, "\t,{sf_init,%s,0,0,0,0,0,0,0,0,0}\n", pnum);
	fprintf(stdout,"\t,0,0\n\t,{\n\t{0,init_state,&sps_state,TIME,0,0}\n");
	fprintf(stdout,"\t,{0,0,0,0,0,0}\n\t}\n};\n");

	fprintf(stdout,"AWAKE nowstate[] = {\n\t");
	for (i = 0; i < chain_cnt; i++) {
		fprintf(stdout,"{0,%s,%s,&s%s,&s%s,abort%c,0,0,0}\n\t,",stat[i],stat[i],begbuf[i],
			begbuf[i],'a' + i);
	}

	fprintf(stdout,"{0,0,0,0,0,0,0,0,0}");
	fprintf(stdout,"\n};\n\n");


	/* the following is the kludge for init routines to be called */
	for (fptr = fnlist; *fptr; fptr++) {
		fprintf(stdout,"void %s();\n",*fptr);
	}

	fprintf(stdout,"int (*init_list[])() = {\n");
	for (fptr = fnlist; *fptr; fptr++) {
		fprintf(stdout,"%s,\n",*fptr);
	}

	fprintf(stdout,"0};\n");
	fflush(stdout);
}

/*
 * Put out the abort lists for the different chains.
 */
void
aborts(void)
{
	int i;
	register char **fptr;

	fptr = ablist;

	for (i = 0; i < chain_cnt; i++) {
		fprintf(stdout,"STATE *abort%c[] = {\n",ab++);
		for (; *fptr; fptr++) {
			fprintf(stdout,"&s%s,\n",*fptr);
		}

		fprintf(stdout,"0};\n\n");
		fptr++;
	}
}

/*
 * Only error is return line number of error.
 */
void
yyerror(char *s)
{
	extern int lineno;

	fprintf(stderr,"error %s: line %d\n", s, lineno);
}

void
spot(void)
{
	go();		/* set up lex to be in the proper state */
	ilist();
	includes();
	yyparse();
	globals_names();
	usercode();
	states_names();
	aborts();
	inittable();
	varlist();
}


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
%{
#include <stdio.h>
#include <ctype.h>
#include "spot.h"
#include "lex.yy.c"

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

struct stateStruct {
	char *sn;		/* state name */
	char *sc;		/* ecode */
	char *pc;		/* pointer to variable holding ecode */
	char *sact;		/* action name */
	char *saa1;		/* action arguments */
	char *saa2;
	char *saa3;
	char *saa4;
	char *saa5;
	char *saa6;
	char *saa7;
	char *saa8;
	char *saa9;
	char *saa10;
	char *srlf;		/* running line flag */
	char *srlv;		/* running line value */
	char *spt;		/* preset time */
	char *srt;		/* random time */
	struct escp {	/* escapes */
		char *se;
		char *sf;
		char *sad;
		char *sm;
	} es[MAXESC];
	struct stateStruct *nptr;
};

struct stateStruct stateList[MAXSTATE] = {0};	/* list of states */
struct stateStruct *statePtr = stateList;		/* pointer to current state yacc is processing */
char buffer[MAXLEXBUF] = {0};					/* buffer for lex */
char *lptr = buffer - 1;						/* pointer to null term of last string */
char *firstStateList[MAXCHAIN] = {0};			/* pointers to start (FIRST) state of each chain */
char **firstStatePtr = firstStateList;
char *chainNames[MAXCHAIN] = {0};				/* names of chains */
char **chainPtr = chainNames;
char *chainStatusList[MAXCHAIN] = {0};			/* status of chains (on or off) at init time */
char **chainStatusPtr = chainStatusList;
char *abortList[MAXCHAIN] = {0};				/* aborts of each chain; term by null ptr */
int abortCount = 0;								/* number of aborts */
char ab = 'a';
char *restartFuncList[MAXCHAIN] = {0};			/* restart function names */
char **restartFuncPtr = restartFuncList;
char *pnum = {0};								/* paradigm number */
int i = {0};
int escpIndx = {0};
int chain_cnt = {0};							/* total number of chains */
int first_seen = 0;

extern FILE *outfd;
%}

%token STATE CODE TIME RANDOM TO MINUS PLUS RUNL FIRST
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
				*restartFuncPtr++ = $3;
			}
		| RESTART newstring
			{
				*restartFuncPtr++ = $2;
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
				*chainPtr++ = $1;
				chain_cnt++;
				first_seen= 0;
				abortCount++;
			}
		| newstring '{' status states '}'
			{
				if(!first_seen) {
					fprintf(stderr,
							"CHAIN '%s' has no 'begin' state\n",
							$1);
					exit(1);
				}
				*chainPtr++ = $1;
				chain_cnt++;
				first_seen= 0;
				abortCount++;
			}
		;

status:		STATUS newnum
        {
			*chainStatusPtr++ = $2;
		}
		;

states:		shortstate
			{
				statePtr++;
				escpIndx = 0;
			}
		| states shortstate
			{
				statePtr++;
				escpIndx = 0;
			}
		;

shortstate:	newstring ':' rest
			{
				if(strlen($1) > 30) {
					yyerror("State name too long");
				}
				statePtr->sn = $1;

				if (escpIndx >= MAXESC) {
					yyerror("Too many escapes");
				}
			}
		| FIRST newstring ':' rest
			{
				if(strncmp(zero, statePtr->sact, sizeof(zero)) != 0) {
					yyerror("\'begin\' state cannot contain an action");
				}
				if(first_seen) {
					yyerror("Only one \'begin\' state per chain allowed");
				}
				else first_seen= 1;

				if(strlen($2) > 30) {
					yyerror("State name too long");
				}
				*firstStatePtr++ = $2;
				statePtr->sn = $2;

				if(escpIndx >= MAXESC) {
					yyerror("Too many escapes");
				}
			}
		;

rest:		CODE newnum ta ahead
			{
				if($2[0] == '&') statePtr->pc = $2;
				else statePtr->sc = $2;
			}
		| CODE newnum ahead
			{
				if($2[0] == '&') statePtr->pc = $2;
				else statePtr->sc = $2;
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
			statePtr->sact = $2;
		}
		| lact newstring '(' newstring ')'
			{
				statePtr->sact = $2;
				statePtr->saa1 = $4;
			}
		| lact newstring '(' newstring ','  newstring ')'
			{
				statePtr->sact = $2;
				statePtr->saa1 = $4;
				statePtr->saa2 = $6;
			}
		| lact newstring '(' newstring ',' newstring ',' newstring ')'
			{
				statePtr->sact = $2;
				statePtr->saa1 = $4;
				statePtr->saa2 = $6;
				statePtr->saa3 = $8;
			}
		| lact newstring '(' newstring ',' newstring ',' newstring ','
		  newstring ')'
			{
				statePtr->sact = $2;
				statePtr->saa1 = $4;
				statePtr->saa2 = $6;
				statePtr->saa3 = $8;
				statePtr->saa4 = $10;
			}
		| lact newstring '(' newstring ',' newstring ',' newstring ','
		  newstring ',' newstring ')'
			{
				statePtr->sact = $2;
				statePtr->saa1 = $4;
				statePtr->saa2 = $6;
				statePtr->saa3 = $8;
				statePtr->saa4 = $10;
				statePtr->saa5 = $12;
			}
		| lact newstring '(' newstring ',' newstring ',' newstring ','
		  newstring ',' newstring ',' newstring ')'
			{
				statePtr->sact = $2;
				statePtr->saa1 = $4;
				statePtr->saa2 = $6;
				statePtr->saa3 = $8;
				statePtr->saa4 = $10;
				statePtr->saa5 = $12;
				statePtr->saa6 = $14;
			}
		| lact newstring '(' newstring ',' newstring ',' newstring ','
		  newstring ',' newstring ',' newstring ',' newstring ')'
			{
				statePtr->sact = $2;
				statePtr->saa1 = $4;
				statePtr->saa2 = $6;
				statePtr->saa3 = $8;
				statePtr->saa4 = $10;
				statePtr->saa5 = $12;
				statePtr->saa6 = $14;
				statePtr->saa7 = $16;
			}
		| lact newstring '(' newstring ',' newstring ',' newstring ','
		  newstring ',' newstring ',' newstring ',' newstring ','
		  newstring ')'
			{
				statePtr->sact = $2;
				statePtr->saa1 = $4;
				statePtr->saa2 = $6;
				statePtr->saa3 = $8;
				statePtr->saa4 = $10;
				statePtr->saa5 = $12;
				statePtr->saa6 = $14;
				statePtr->saa7 = $16;
				statePtr->saa8 = $18;
			}
		| lact newstring '(' newstring ',' newstring ',' newstring ','
		  newstring ',' newstring ',' newstring ',' newstring ','
		  newstring ',' newstring ')'
			{
				statePtr->sact = $2;
				statePtr->saa1 = $4;
				statePtr->saa2 = $6;
				statePtr->saa3 = $8;
				statePtr->saa4 = $10;
				statePtr->saa5 = $12;
				statePtr->saa6 = $14;
				statePtr->saa7 = $16;
				statePtr->saa8 = $18;
				statePtr->saa9 = $20;
			}
		| lact newstring '(' newstring ',' newstring ',' newstring ','
		  newstring ',' newstring ',' newstring ',' newstring ','
		  newstring ',' newstring ',' newstring ')'
			{
				statePtr->sact = $2;
				statePtr->saa1 = $4;
				statePtr->saa2 = $6;
				statePtr->saa3 = $8;
				statePtr->saa4 = $10;
				statePtr->saa5 = $12;
				statePtr->saa6 = $14;
				statePtr->saa7 = $16;
				statePtr->saa8 = $18;
				statePtr->saa9 = $20;
				statePtr->saa10 = $22;
			}
		;
lact:		/* empty */
		| ACTION
		;
time:		/* empty */
		| TIME newnum
			{
				statePtr->spt = $2;
			}
		| RANDOM newnum
			{
				statePtr->srt = $2;
			}
		| TIME newnum RANDOM newnum
			{
				statePtr->spt = $2;
				statePtr->srt = $4;
			}
		| RANDOM newnum TIME newnum
			{
				statePtr->srt = $2;
				statePtr->spt = $4;
			}
		;

runl:		/* empty */
		| RUNL newnum
			{
				statePtr->srlv = one;
				statePtr->srlf = $2;
			}
		| RUNL PLUS newnum
			{
				statePtr->srlv = two;
				statePtr->srlf = $3;
			}
		| RUNL MINUS newnum
			{
				statePtr->srlv = three;
				statePtr->srlf = $3;
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
			statePtr->es[escpIndx].se = $2;
			statePtr->es[escpIndx].sf = equal;
			statePtr->es[escpIndx].sad = $6;
			statePtr->es[escpIndx].sm = $4;
			escpIndx++;
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
			statePtr->es[escpIndx].se = $2;
			statePtr->es[escpIndx].sf = equal;
			statePtr->es[escpIndx].sad = $7;
			statePtr->es[escpIndx].sm = $5;
			escpIndx++;
		}
		| TO newstring ON sign newnum '&' address
		{
			statePtr->es[escpIndx].se = $2;
			statePtr->es[escpIndx].sf = $4;
			statePtr->es[escpIndx].sad = $7;
			statePtr->es[escpIndx].sm = $5;
			escpIndx++;
		}
		| TO newstring ON newnum '>' address
		{
			statePtr->es[escpIndx].se = $2;
			statePtr->es[escpIndx].sf = greater;
			statePtr->es[escpIndx].sad = $6;
			statePtr->es[escpIndx].sm = $4;
			escpIndx++;
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
			statePtr->es[escpIndx].se = $2;
			statePtr->es[escpIndx].sf = greater;
			statePtr->es[escpIndx].sad = $7;
			statePtr->es[escpIndx].sm = $5;
			escpIndx++;
		}
	        | TO newstring ON newnum '<' address
		{
			statePtr->es[escpIndx].se = $2;
			statePtr->es[escpIndx].sf = less;
			statePtr->es[escpIndx].sad = $6;
			statePtr->es[escpIndx].sm = $4;
			escpIndx++;
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
			statePtr->es[escpIndx].se = $2;
			statePtr->es[escpIndx].sf = less;
			statePtr->es[escpIndx].sad = $7;
			statePtr->es[escpIndx].sm = $5;
			escpIndx++;
		}
		| TO newstring ON newnum '?' address
		{
			statePtr->es[escpIndx].se = $2;
			statePtr->es[escpIndx].sf = question;
			statePtr->es[escpIndx].sad = $6;
			statePtr->es[escpIndx].sm = $4;
			escpIndx++;
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
			statePtr->es[escpIndx].se = $2;
			statePtr->es[escpIndx].sf = question;
			statePtr->es[escpIndx].sad = $7;
			statePtr->es[escpIndx].sm = $5;
			escpIndx++;
		}
		| TO newstring ON newnum '%' address
		{
			statePtr->es[escpIndx].se = $2;
			statePtr->es[escpIndx].sf = function;
			statePtr->es[escpIndx].sad = $6;
			statePtr->es[escpIndx].sm = $4;
			escpIndx++;
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
			statePtr->es[escpIndx].se = $2;
			statePtr->es[escpIndx].sf = function;
			statePtr->es[escpIndx].sad = $7;
			statePtr->es[escpIndx].sm = $5;
			escpIndx++;
		}
		| TO newstring
		{
			statePtr->es[escpIndx].se = $2;
			statePtr->es[escpIndx].sf = time;
			escpIndx++;
		}
		| TO newstring ON TIME
		{
			statePtr->es[escpIndx].se = $2;
			statePtr->es[escpIndx].sf = time;
			escpIndx++;
		}
;
address:	newnum
			{
				$$ = $1;
			}
		| newstring
			{
				$$ = $1;
			}
;
sign:		PLUS
			{
				$$ = biton;
			}
		| MINUS
			{
				$$ = bitoff;
			}
;		

abort:		ABORT newstring ':' ab1
		| ABORT ':' ab1
		;

ab1:		/* empty */
		| newstring
			{
				abortList[abortCount++] = $1;
			}
		| ab1 newstring
			{
				abortList[abortCount++] = $2;
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

/*
 * Zero state tables.
 */
void initList(void)
{
	struct stateStruct *sptr;
	int i;

	for(sptr = stateList; sptr < &stateList[MAXSTATE]; sptr++) {
			sptr->sn = zero;
			sptr->sc = zero;
			sptr->pc = zero;
			sptr->srlf = zero;
			sptr->srlv = zero;
			sptr->sact = zero;
			sptr->saa1 = zero;
			sptr->saa2 = zero;
			sptr->saa3 = zero;
			sptr->saa4 = zero;
			sptr->saa5 = zero;
			sptr->saa6 = zero;
			sptr->saa7 = zero;
			sptr->saa8 = zero;
			sptr->saa9 = zero;
			sptr->saa10 = zero;
			sptr->spt = zero;
			sptr->srt = zero;
			for (i = 0; i < MAXESC; i++) {
				sptr->es[i].sf = zero;
				sptr->es[i].sad = zero;
				sptr->es[i].se = zero;
				sptr->es[i].sm = zero;
			}
	}
}

/* 
 * Put out the required #includes.
 */
void printIncludes(void)
{
	fprintf(stdout,"#include <stdio.h>\n");
	fprintf(stdout,"#include <sys/types.h>\n");
	fprintf(stdout,"#include \"rexhdrs.h\"\n");
	fprintf(stdout,"#include \"ecode.h\"\n");
	fprintf(stdout,"#include \"int.h\"\n");
	fprintf(stdout,"\n\n");
}

/* 
 * Put out the states.
 */
void printStateStructs(void)
{
	register struct stateStruct *sptr;
	register struct escp *eptr;
	register int i;

	for(sptr = stateList; sptr < statePtr; sptr++) {
		fprintf(stdout,"STATE s%s = {\n",sptr->sn);
		fprintf(stdout, "\t{\"%s\"},%s,%s\n", sptr->sn, sptr->sc, sptr->pc);
		fprintf(stdout,"\t,%s,%s\n",sptr->srlv,sptr->srlf);
		fprintf(stdout,"\t,{%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s}\n",
				sptr->sact, sptr->saa1, sptr->saa2, sptr->saa3, sptr->saa4,
				sptr->saa5, sptr->saa6, sptr->saa7, sptr->saa8, sptr->saa9,
				sptr->saa10);
		fprintf(stdout,"\t,%s,%s\n",sptr->spt,sptr->srt);

		eptr = sptr->es;
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
void printExterns(void)
{
	int i;
	struct stateStruct *sptr;
	struct escp *eptr;

	printf("\n\n");
	for(sptr = stateList; sptr < statePtr; sptr++) {
		if(islower(*sptr->sact)) {
			fprintf(stdout,"extern int %s();\n", sptr->sact);
		}

		eptr = sptr->es;
		for(i = 0; i < MAXESC; i++) {
			if(islower(*eptr->sad)) {
				if(eptr->sf == function) {
					fprintf(stdout,"extern int %s();\n", eptr->sad);
				}
				else {
					char *cp;

					cp = eptr->sad;

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

	for(sptr = stateList; sptr < statePtr; sptr++) {
		fprintf(stdout,"STATE s%s;\n", sptr->sn);
	}

	fprintf(stdout,"\n\n\n");
	fprintf(stdout,"STATE *snames[] = {\n");

	for (sptr = stateList; sptr < statePtr; sptr++) {
		fprintf(stdout,"&s%s,\n", sptr->sn);
	}
	fprintf(stdout,"0};\n\n");
}

/*
 * Get out the structures which are used by the state processor.
 */
void printTransitionStructs(void)
{
	char **funcNamePtr;
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
		fprintf(stdout,"{0,%s,%s,&s%s,&s%s,abort%c,0,0,0}\n\t,",
				chainStatusList[i], chainStatusList[i], firstStateList[i], firstStateList[i],'a' + i);
	}

	fprintf(stdout,"{0,0,0,0,0,0,0,0,0}");
	fprintf(stdout,"\n};\n\n");


	/* the following is the kludge for init routines to be called */
	for (funcNamePtr = restartFuncList; *funcNamePtr; funcNamePtr++) {
		fprintf(stdout,"void %s();\n",*funcNamePtr);
	}

	fprintf(stdout,"int (*init_list[])() = {\n");
	for (funcNamePtr = restartFuncList; *funcNamePtr; funcNamePtr++) {
		fprintf(stdout,"%s,\n",*funcNamePtr);
	}

	fprintf(stdout,"0};\n");
	fflush(stdout);
}

/*
 * Put out the abort lists for the different chains.
 */
void printAbortStructs(void)
{
	int i;
	char **abortPtr;

	abortPtr = abortList;

	for (i = 0; i < chain_cnt; i++) {
		fprintf(stdout,"STATE *abort%c[] = {\n",ab++);
		for (; *abortPtr; abortPtr++) {
			fprintf(stdout,"&s%s,\n",*abortPtr);
		}

		fprintf(stdout,"0};\n\n");
		abortPtr++;
	}
}

/*
 * Only error is return line number of error.
 */
yyerror(char *s)
{
	extern int lineno;
	extern char infile[50];

	int i;
	char buf[1024];

	fseek(stdin, 0L, 0);

	fprintf(stderr,"error: %s: spot file %s, line %d, before \"%s\"\n", s, infile, lineno, yytext);

	// scan spot file to location of error
	for(i = 0; i < lineno - 2; ++i) {
		fgets(buf, 1024, stdin);
	}

	// get the line before offending line
	fgets(buf, 1024, stdin);
	fprintf(stderr, "%d%s", lineno - 1, buf);

	// get the offending line
	fgets(buf, 1024, stdin);
	fprintf(stderr, "%d%s\n", lineno, buf);

	exit(-1);
}

void spot(void)
{
	go();		/* set up lex to be in the proper state */
	initList();
	printIncludes();
	yyparse();
	printExterns();
	printUserCode();
	printStateStructs();
	printAbortStructs();
	printTransitionStructs();
	printNullStructs();
}

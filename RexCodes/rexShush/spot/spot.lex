%{
#include <stdio.h>
#include "spot.tab.h"

static int com_cnt = 0;
int lineno = 1;
long sect1 = 0;
int svarflag = 0;
int unounflag = 0;
int umenuflag = 0;
int ufuncflag = 0;
int rtvarflag = 0;
int hmflag = 0;

extern char *lptr;
extern char ifile[];

void go();
char *copyToken(char *s1, char *s2);
%}

%option noyywrap
%x USER_CODE CHAIN_DSCRPT C_COMMENT CPP_COMMENT ESCAPE INCLUDE RUNNING_LINE ACTION_ARG

%%
<USER_CODE>{
	/* process the user code at the beginning of the spot file */
	\n {
		lineno++;
		sect1++;
	}

	"state_vl" {					/* description of state variables structure */
		sect1 += yyleng;
		if(!svarflag) {
			svarflag = 1;
		}
	}

	"unouns" {						/* description of user nouns structure */
		sect1 += yyleng;
		if (!unounflag) {
			unounflag = 1;
		}
	}

	"rtvars" {						/* description of real time variables structure */
		sect1 += yyleng;
		if (!rtvarflag) {
			rtvarflag = 1;
		}
	}

	"ufuncs" {						/* description of user functions structure */
		sect1 += yyleng;
		if (!ufuncflag) {
			ufuncflag = 1;
		}
	}

	"umenus" {						/* description of user menus structure */
		sect1 += yyleng;
		if (!umenuflag) {
			umenuflag = 1;
		}
	}

	"hm_sv_vl" {					/* menu help menu */
		sect1 += yyleng;
		if (!hmflag) {
			hmflag = 1;
		}
	}

	. sect1 += yyleng;

	"%%" BEGIN(CHAIN_DSCRPT);		/* beginning of the state description */
}

<CHAIN_DSCRPT>{
	/* process the description of a chain */
	"{" return('{');							/* chain opening brace */
	"}"	return('}');							/* chain closing brace */
	"(" {										/* begin argument to action */
		BEGIN(ACTION_ARG);
		return('(');
	}
	")" return(')');							/* end argument to action */
	"," return(',');							/* comma separating arguments */
	: return(':');								/* colon ending state name */
	id(ent)? return(PNUM);						/* process ID */
	begin return(FIRST);						/* first state in a chain */
	stat(us)? return(STATUS);					/* status of the chain */
	code return(CODE);							/* an ecode to drop into E file */
	time return(TIME);							/* the time to wait in a state */
	rand(om)? return(RANDOM);					/* random amount to add to time */
	restart return(RESTART);					/* restart function */
	do return(ACTION);							/* action */
	rl {										/* running line value */
		BEGIN(RUNNING_LINE);
		return(RUNL);
	}											/* abort list for a chain */
	abort return(ABORT);
	to return(TO);								/* name of state to escape to */
	"&" return('&');							/* escape conditions */
	"=" return('=');
	"<" return('<');
	">" return('>');
	"?" return('?');
	"%" return('%');
	on {										/* list escape conditions */
		BEGIN(ESCAPE);
		return(ON);
	}
	[[:upper:]][-+ _[:digit:][:upper:]]* {		/* a string with all caps */
		++lptr;									/* and possibly digits */
		*lptr++ = ' ';
		yylval.cpval = lptr;
		lptr = copyToken(lptr, yytext);
		return(CAPS);
	}
	[_[:alpha:]][_[:alnum:]\[\].]* {			/* an alphanumeric string with leading letter */
		++lptr;									/* and containing an array index */
		*lptr++ = ' ';
		yylval.cpval = lptr;
		lptr = copyToken(lptr, yytext);
		return(STRING);
	}
	&[[:alnum:]\[\].]+	{						/*an address containing an array index */
		++lptr;
		*lptr++ = ' ';
		yylval.cpval = lptr;
		lptr = copyToken(lptr,yytext);
		return(ADDRESS);
	}
	[-/+]?[[:digit:]][-+ [:digit:]]* {			/* a number */
		++lptr;
		*lptr++ = ' ';
		yylval.cpval = lptr;
		lptr = copyToken(lptr, yytext);
		return(NUMBER);
	}
	\n lineno++;								/* increment line number */
	"/*" {										/* begin a C style comment */
		com_cnt = 1;
		BEGIN(C_COMMENT);
	}
	"//" {										/* begin a C++ style comment */
		com_cnt = 1;
		BEGIN(CPP_COMMENT);
	}
	^# {										/* a #include statement */
		fprintf(stdout,"#");
		BEGIN(INCLUDE);
	}
	. {}										/* eat character */
}

<ACTION_ARG>{
	/* process the arguments to an action */
    ")" {								/* closing parenthesis */
		BEGIN(CHAIN_DSCRPT);
		return(')');
	}
    "," return(',');					/* comma separating arguments */
	" " {}								/* eat spaces */
    \n lineno++;						/* increment line counter on new line */
	[[:alpha:]][_[:alnum:]\[\].]*	{	/* argument is alphanumeric */
		++lptr;
		*lptr++ = ' ';
		yylval.cpval = lptr;
		lptr = copyToken(lptr, yytext);
		return(STRING);
	}
	&[[:alpha:]_?[:alnum:]\[\].]+	{	/* argument is an address */
		++lptr;
		*lptr++ = ' ';
		yylval.cpval = lptr;
		lptr = copyToken(lptr,yytext);
		return(STRING);
	}
	[-/+]?[[:digit:]][-+ [:digit:]]* { /* argument is a number */
		++lptr;
		*lptr++ = ' ';
		yylval.cpval = lptr;
		lptr = copyToken(lptr, yytext);
		return(STRING);
	}
}

<ESCAPE>{
	/* process the arguments for an escape description */
	"+" {									/* escape if the bit is set */
		BEGIN(CHAIN_DSCRPT);
		return(PLUS);
	}
	"-" {									/* escape if the bit is not set */
		BEGIN(CHAIN_DSCRPT);
		return(MINUS);
	}
	"time" {								/* escape on time */
		BEGIN(CHAIN_DSCRPT);
		return(TIME);
	}
	[[:digit:]][-+ [:digit:]]* {
		++lptr;
		*lptr++ = ' ';
		yylval.cpval = lptr;
		lptr = copyToken(lptr, yytext);

		BEGIN(CHAIN_DSCRPT);
		return(NUMBER);
	}
	[[:upper:]][-+ _[:digit:][:upper:]]* {	/* name of flag variable */
		++lptr;
		*lptr++ = ' ';
		yylval.cpval = lptr;
		lptr = copyToken(lptr, yytext);

		BEGIN(CHAIN_DSCRPT);
		return(CAPS);
	}
	. {}
}

<RUNNING_LINE>{
	/* process the arguments for a running line description */
	" " {}									/* eat spaces */
	"+" {									/* raise running line by following value */
		BEGIN(CHAIN_DSCRPT);
		return(PLUS);
	}
	"-" {									/* lower running line by following value */
		BEGIN(CHAIN_DSCRPT);
		return(MINUS);
	}
	[[:digit:]][-+ [:digit:]]* {			/* a fixed running line value */
		++lptr;
		*lptr++ = ' ';
		yylval.cpval = lptr;
		lptr = copyToken(lptr, yytext);

		BEGIN(CHAIN_DSCRPT);
		return(NUMBER);
	}
	[[:upper:]][-+ _[:digit:][:upper:]]* {	/* a constant holding a running line value */
		++lptr;
		*lptr++ = ' ';
		yylval.cpval = lptr;
		lptr = copyToken(lptr, yytext);

		BEGIN(CHAIN_DSCRPT);
		return(CAPS);
	}
}

<C_COMMENT>{
	/* process a C style comment */
	"/*" com_cnt++;						/* check for nested C comments */
	. {}								/* eat all characters in the comment */
	\n lineno++;						/* increment line counter */
	"*/" {								/* close C comment */
		if(--com_cnt == 0) {
			BEGIN(CHAIN_DSCRPT);
		}
	}
}

<CPP_COMMENT>{
	/* process a C++ style comment */
	\n {
		--com_cnt;
		lineno++;
		BEGIN(CHAIN_DSCRPT);
	}
	. {}								/* eat all characters in the comment */
}

<INCLUDE>{
	/* process a #include statement */
	[\040-~]* fprintf(stdout,"%s ",yytext);
	[" "\t]* {}							/* eat white space */
	\n {
		lineno++;
		fprintf(stdout,"\n");

		BEGIN(CHAIN_DSCRPT);
	}
}
%%

void go()
{
	BEGIN(USER_CODE);
}

char *copyToken(char *s1, char *s2)
{
	extern char buffer[MAXLEXBUF];
	extern char infile[50];

	if((s1 + strlen(s2)) > &buffer[MAXLEXBUF]) {
		fprintf(stderr, "Error in spot file %s, at line %d\n", infile, lineno);
		fprintf(stderr, "Total length of all tokens is too long.\n");
		fprintf(stderr, "Spot lexical analysis buffer will overflow.\n");
		fprintf(stderr, "You can reduce the length of tokens by\n");
		fprintf(stderr, "shortening the names and/or reducing the number\n");
		fprintf(stderr, "of those variables, actions, macros, and states\n");
		fprintf(stderr, "which are used after the \'%%%%\' symbol in the spot file\n\n");

		exit(-1);
	}

	while(*s1++ = *s2++);

	return(s1 - 1);
}

/* print out the user code at the beginning of the
 * spot file
 */
void printUserCode()
{
	long i;
	char tbuf;

	fseek(stdin, 0L, 0);

	for(i = 0; i < sect1; i++) {
		fread(&tbuf, 1, 1, stdin);
		fwrite(&tbuf, 1, 1, stdout);
	}
}

/*
 * Put the null structures into the accessory file if they have not been
 * initialized by the user.
 */
void printNullStructs()
{
	if(!svarflag) {
		fprintf(stdout, "VLIST state_vl[] = {\n");
		fprintf(stdout, "NS, };\n");
		fflush(stdout);
	}
	if(!unounflag) {
		fprintf(stdout, "NOUN unouns[] = {\n");
		fprintf(stdout, "\"\",\n");
		fprintf(stdout, "};\n");
	}
	if(!rtvarflag) {
		fprintf(stdout, "RTVAR rtvars[] = {\n");
		fprintf(stdout, "{\"\"},\n");
		fprintf(stdout, "};\n");
	}
	if(!ufuncflag) {
		fprintf(stdout, "USER_FUNC ufuncs[] = {\n");
		fprintf(stdout, "{\"\"},\n");
		fprintf(stdout, "};\n");
	}
	if(!umenuflag) {
		fprintf(stdout, "MENU umenus[] = {\n");
	  	if(!svarflag || !hmflag) {
            fprintf(stdout, "\"state_vars\",\t&state_vl, NP, NP, 0, NP, NS,\n");
        }
		else {
            fprintf(stdout, "\"state_vars\",\t&state_vl, NP, NP, 0, NP, hm_sv_vl,\n");
        }
		fprintf(stdout, "NS,\n");
		fprintf(stdout, "};\n");
	}
}

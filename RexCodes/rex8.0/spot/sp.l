%{
#include <stdio.h>
static int com_cnt = 0;
int lineno = 1;
long sect1 = 0;
int svarflag = 0;
int rtvarflag= 0;
int ufuncflag = 0;
int umenuflag= 0;
int hmflag= 0;
extern char *lptr;
extern char ifile[];
extern char *copystr(char *, char *);

void go();
%}
%start GO STOP AA BB INCLUDE KK LINE CC

%%
<KK>"%%"	{
			BEGIN GO;
		}
<KK>\n	{
		lineno++;
		sect1++;
	}
<KK>"state_vl"		{
				sect1 += yyleng;
				if (!svarflag) {
					svarflag = 1;
				}
			}
<KK>"umenus"		{
				sect1 += yyleng;
				if (!umenuflag) {
					umenuflag = 1;
				}
			}
<KK>"rtvars"	{
				sect1 += yyleng;
				if (!rtvarflag) {
					rtvarflag = 1;
				}
			}
<KK>"ufuncs"		{
				sect1 += yyleng;
				if (!ufuncflag) {
					ufuncflag = 1;
				}
			}
<KK>"hm_sv_vl"		{
				sect1 += yyleng;
				if (!hmflag) {
					hmflag = 1;
				}
			}
<KK>.		{
			sect1 += yyleng;
		}
<GO>"{"		return('{');
<GO>"}"		return('}');
<GO>"("		{
		BEGIN CC;
			return('(');
		}
<GO>")"		return(')');
<GO>","		return(',');
<GO>:		return(':');
<GO>id(ent)?	return(PNUM);
<GO>begin	return(FIRST);
<GO>stat(us)?	return(STATUS);
<GO>code	return(CODE);
<GO>time	return(TIME);
<GO>rand(om)?	return(RANDOM);
<GO>restart	return(RESTART);
<GO>do		return(ACTION);
<GO>rl		{BEGIN LINE;
		return(RUNL);
		}
<GO>abort	return(ABORT);
<GO>to		{
			return(TO);
		}
<GO>"&"		return('&');
<GO>"="		return('=');
<GO>"<"		return('<');
<GO>">"		return('>');
<GO>"?"		return('?');
<GO>"%"		return('%');
<GO>on	{
		BEGIN AA;
		return(ON);
	}
<GO>[A-Z][-+ _0-9A-Z]*	{
				++lptr;
				*lptr++ = ' ';
				yylval.cpval = lptr;
				lptr = copystr(lptr,yytext);
				return(CAPS);
			}
<GO>[A-Z]?[_a-z][_a-zA-Z0-9\[\].]*	{
				++lptr;
				*lptr++ = ' ';
				yylval.cpval = lptr;
				lptr = copystr(lptr,yytext);
				return(STRING);
			}
<GO>&[a-zA-Z0-9\[\].]+	{
				++lptr;
				*lptr++ = ' ';
				yylval.cpval = lptr;
				lptr = copystr(lptr,yytext);
				return(ADDRESS);
			}
<GO>[-/+]?[0-9][-+ 0-9]*	{
				++lptr;
				*lptr++ = ' ';
				yylval.cpval = lptr;
				lptr = copystr(lptr,yytext);
				return(NUMBER);
			}
<GO>\n		{
			lineno++;
		}
<CC>")"		{
			BEGIN GO;
			return(')');
		}
<CC>","		return(',');
<CC>\n		{
			lineno++;
		}
<CC>[^),\n]*		{
				++lptr;
				*lptr++ = ' ';
				yylval.cpval = lptr;
				lptr = copystr(lptr,yytext);
				return(STRING);
			}
<CC>[0-9A-Z][-+ _0-9A-Z]*	{
				++lptr;
				*lptr++ = ' ';
				yylval.cpval = lptr;
				lptr = copystr(lptr,yytext);
				return(NUMBER);
			}
<CC>.		{}
<AA>"+"		{
			BEGIN GO;
			return(PLUS);
		}
<AA>"-"		{
			BEGIN GO;
			return(MINUS);
		}
<AA>"time"	{
			BEGIN GO;
			return(TIME);
		}
<AA>[0-9][-+ 0-9]*	{
				++lptr;
				*lptr++ = ' ';
				yylval.cpval = lptr;
				lptr = copystr(lptr,yytext);
				BEGIN GO;
				return(NUMBER);
			}
<AA>[A-Z][-+ _0-9A-Z]*	{
				++lptr;
				*lptr++ = ' ';
				yylval.cpval = lptr;
				lptr = copystr(lptr,yytext);
				BEGIN GO;
				return(CAPS);
			}
<AA>.		{}
<LINE>" "	{}
<LINE>"+"		{
			BEGIN GO;
			return(PLUS);
		}
<LINE>"-"		{
			BEGIN GO;
			return(MINUS);
		}
<LINE>[0-9][-+ 0-9]*	{
				++lptr;
				*lptr++ = ' ';
				yylval.cpval = lptr;
				lptr = copystr(lptr,yytext);
				BEGIN GO;
				return(NUMBER);
			}
<LINE>[A-Z][-+ _0-9A-Z]*	{
				++lptr;
				*lptr++ = ' ';
				yylval.cpval = lptr;
				lptr = copystr(lptr,yytext);
				BEGIN GO;
				return(CAPS);
			}
<GO>"/*"	{
			com_cnt = 1;
			BEGIN STOP;
		}
<STOP>"/*"	{
			com_cnt++;
		}
<STOP>"*/"	{
			if (--com_cnt == 0)
				BEGIN GO;
		}
<STOP>\n	{
			lineno ++;
		}
<GO>^#	{
		fprintf(stdout,"#");
		BEGIN INCLUDE;
	}
<INCLUDE>[\040-~]*	{
			fprintf(stdout,"%s ",yytext);
		}
<INCLUDE>[" "\t]*	{}
<INCLUDE>\n	{
			BEGIN GO;
			lineno++;
			fprintf(stdout,"\n");
		}
<GO>.	{}
<STOP>.		{}
%%
void
go()
{
	BEGIN KK;
}

void
usercode()
{
	long i;
	char tbuf;

	fseek(stdin,0L,0);

	for (i=0; i < sect1; i++) {
		fread(&tbuf,1,1,stdin);
		fwrite(&tbuf,1,1,stdout);
	}
}

/*
 * Put the null variable table into the accessory file if it has not been
 *	initialized by the user.
 */
void
varlist()
{
	if (!svarflag) {
		fprintf(stdout, "VLIST state_vl[] = {\n");
		fprintf(stdout, "NS, };\n");
		fflush(stdout);
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
	  	if(!svarflag || !hmflag) 
		  fprintf(stdout, "\"state_vars\",\t&state_vl, NP, NP, 0, NP, NS,\n");
		else fprintf(stdout, "\"state_vars\",\t&state_vl, NP, NP, 0, NP, hm_sv_vl,\n");
		fprintf(stdout, "NS,\n");
		fprintf(stdout, "};\n");
	}
	
}

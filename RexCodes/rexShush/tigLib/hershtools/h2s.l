	int lowh = 0;
	int hnum  = 0;
	int started = 0;
	int ascchar = 32;
	int ix;

%%

[0-9]+/-	{	lowh	=	atoi(yytext);
		}

[0-9]+		{	if (!started) printf("charset\n");
			started = 1;
			hnum	=	atoi(yytext);
			if (lowh) ix = lowh;
				else ix = hnum;
			lowh  =  0;
			for (; ix <= hnum; ix++,ascchar++)
			   if (ascchar > ' ' && ascchar < 127)
				printf("%d,%c\n", ix, ascchar);
			   else
				printf("%d,\\%o\n", ix, ascchar);
		}

.	;
\n	;


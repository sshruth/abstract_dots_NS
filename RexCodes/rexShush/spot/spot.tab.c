
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 12 "spot.y"

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


/* Line 189 of yacc.c  */
#line 148 "spot.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     STATE = 258,
     CODE = 259,
     TIME = 260,
     RANDOM = 261,
     TO = 262,
     MINUS = 263,
     PLUS = 264,
     RUNL = 265,
     FIRST = 266,
     ABORT = 267,
     NAME = 268,
     STATUS = 269,
     ACTION = 270,
     ON = 271,
     PNUM = 272,
     RESTART = 273,
     STRING = 274,
     CAPS = 275,
     NUMBER = 276,
     ADDRESS = 277
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 90 "spot.y"

	int ival;
	char cval;
	char *cpval;



/* Line 214 of yacc.c  */
#line 214 "spot.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 226 "spot.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  7
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   174

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  35
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  22
/* YYNRULES -- Number of rules.  */
#define YYNRULES  76
/* YYNRULES -- Number of states.  */
#define YYNSTATES  150

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   277

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    34,    30,     2,
      26,    27,     2,     2,    28,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    23,     2,
      32,    29,    31,    33,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    24,     2,    25,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     8,    11,    12,    16,    19,    26,    32,
      35,    37,    40,    44,    49,    54,    58,    61,    63,    67,
      71,    75,    79,    83,    87,    88,    93,    99,   107,   117,
     129,   143,   159,   177,   197,   219,   243,   244,   246,   247,
     250,   253,   258,   263,   264,   267,   271,   275,   277,   280,
     282,   285,   292,   300,   308,   315,   323,   330,   338,   345,
     353,   360,   368,   371,   376,   378,   380,   382,   384,   389,
     393,   394,   396,   399,   401,   403,   405
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      36,     0,    -1,    17,    56,    37,    38,    -1,    36,    38,
      -1,    -1,    18,    23,    55,    -1,    18,    55,    -1,    55,
      24,    39,    40,    53,    25,    -1,    55,    24,    39,    40,
      25,    -1,    14,    56,    -1,    41,    -1,    40,    41,    -1,
      55,    23,    42,    -1,    11,    55,    23,    42,    -1,     4,
      56,    43,    48,    -1,     4,    56,    48,    -1,    43,    48,
      -1,    48,    -1,    44,    47,    46,    -1,    44,    46,    47,
      -1,    47,    46,    44,    -1,    47,    44,    46,    -1,    46,
      47,    44,    -1,    46,    44,    47,    -1,    -1,    45,    55,
      26,    27,    -1,    45,    55,    26,    55,    27,    -1,    45,
      55,    26,    55,    28,    55,    27,    -1,    45,    55,    26,
      55,    28,    55,    28,    55,    27,    -1,    45,    55,    26,
      55,    28,    55,    28,    55,    28,    55,    27,    -1,    45,
      55,    26,    55,    28,    55,    28,    55,    28,    55,    28,
      55,    27,    -1,    45,    55,    26,    55,    28,    55,    28,
      55,    28,    55,    28,    55,    28,    55,    27,    -1,    45,
      55,    26,    55,    28,    55,    28,    55,    28,    55,    28,
      55,    28,    55,    28,    55,    27,    -1,    45,    55,    26,
      55,    28,    55,    28,    55,    28,    55,    28,    55,    28,
      55,    28,    55,    28,    55,    27,    -1,    45,    55,    26,
      55,    28,    55,    28,    55,    28,    55,    28,    55,    28,
      55,    28,    55,    28,    55,    28,    55,    27,    -1,    45,
      55,    26,    55,    28,    55,    28,    55,    28,    55,    28,
      55,    28,    55,    28,    55,    28,    55,    28,    55,    28,
      55,    27,    -1,    -1,    15,    -1,    -1,     5,    56,    -1,
       6,    56,    -1,     5,    56,     6,    56,    -1,     6,    56,
       5,    56,    -1,    -1,    10,    56,    -1,    10,     9,    56,
      -1,    10,     8,    56,    -1,    49,    -1,    48,    49,    -1,
      50,    -1,    49,    50,    -1,     7,    55,    16,    56,    29,
      51,    -1,     7,    55,    16,    52,    56,    29,    51,    -1,
       7,    55,    16,    52,    56,    30,    51,    -1,     7,    55,
      16,    56,    31,    51,    -1,     7,    55,    16,    52,    56,
      31,    51,    -1,     7,    55,    16,    56,    32,    51,    -1,
       7,    55,    16,    52,    56,    32,    51,    -1,     7,    55,
      16,    56,    33,    51,    -1,     7,    55,    16,    52,    56,
      33,    51,    -1,     7,    55,    16,    56,    34,    51,    -1,
       7,    55,    16,    52,    56,    34,    51,    -1,     7,    55,
      -1,     7,    55,    16,     5,    -1,    56,    -1,    55,    -1,
       9,    -1,     8,    -1,    12,    55,    23,    54,    -1,    12,
      23,    54,    -1,    -1,    55,    -1,    54,    55,    -1,    19,
      -1,    21,    -1,    20,    -1,    22,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   108,   108,   112,   115,   116,   120,   126,   139,   154,
     160,   165,   172,   183,   205,   210,   215,   216,   219,   220,
     221,   222,   223,   224,   227,   228,   232,   237,   243,   250,
     259,   269,   280,   292,   306,   321,   338,   339,   341,   342,
     346,   350,   355,   362,   363,   368,   373,   380,   383,   388,
     389,   394,   402,   418,   426,   434,   450,   458,   474,   482,
     498,   506,   522,   528,   535,   539,   544,   548,   554,   555,
     558,   559,   563,   569,   575,   579,   583
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "STATE", "CODE", "TIME", "RANDOM", "TO",
  "MINUS", "PLUS", "RUNL", "FIRST", "ABORT", "NAME", "STATUS", "ACTION",
  "ON", "PNUM", "RESTART", "STRING", "CAPS", "NUMBER", "ADDRESS", "':'",
  "'{'", "'}'", "'('", "')'", "','", "'='", "'&'", "'>'", "'<'", "'?'",
  "'%'", "$accept", "statelist", "restart", "all", "status", "states",
  "shortstate", "rest", "ta", "action", "lact", "time", "runl", "ahead",
  "esc", "escape", "address", "sign", "abort", "ab1", "newstring",
  "newnum", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,    58,   123,   125,    40,    41,    44,    61,
      38,    62,    60,    63,    37
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    35,    36,    36,    37,    37,    37,    38,    38,    39,
      40,    40,    41,    41,    42,    42,    42,    42,    43,    43,
      43,    43,    43,    43,    44,    44,    44,    44,    44,    44,
      44,    44,    44,    44,    44,    44,    45,    45,    46,    46,
      46,    46,    46,    47,    47,    47,    47,    48,    48,    49,
      49,    50,    50,    50,    50,    50,    50,    50,    50,    50,
      50,    50,    50,    50,    51,    51,    52,    52,    53,    53,
      54,    54,    54,    55,    56,    56,    56
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     4,     2,     0,     3,     2,     6,     5,     2,
       1,     2,     3,     4,     4,     3,     2,     1,     3,     3,
       3,     3,     3,     3,     0,     4,     5,     7,     9,    11,
      13,    15,    17,    19,    21,    23,     0,     1,     0,     2,
       2,     4,     4,     0,     2,     3,     3,     1,     2,     1,
       2,     6,     7,     7,     6,     7,     6,     7,     6,     7,
       6,     7,     2,     4,     1,     1,     1,     1,     4,     3,
       0,     1,     2,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,    75,    74,    76,     4,     1,    73,     3,
       0,     0,     0,     0,     0,     6,     2,     0,     0,     5,
       9,     0,     0,    10,     0,     0,     0,     8,    11,     0,
      36,    36,    70,     0,     7,     0,     0,     0,     0,     0,
      37,    12,     0,    38,     0,    24,    24,    17,    47,    49,
      13,    69,    71,    70,    36,    39,    40,    62,     0,     0,
      44,    16,    43,    38,     0,    43,    24,    38,    24,    48,
      50,    72,    68,     0,    15,     0,     0,     0,    46,    45,
      19,    18,     0,    23,    22,    21,    20,    14,    41,    42,
      63,    67,    66,     0,     0,    25,     0,     0,     0,     0,
       0,     0,     0,    26,     0,     0,     0,     0,     0,     0,
       0,    51,    65,    64,    54,    56,    58,    60,     0,    52,
      53,    55,    57,    59,    61,    27,     0,     0,    28,     0,
       0,    29,     0,     0,    30,     0,     0,    31,     0,     0,
      32,     0,     0,    33,     0,     0,    34,     0,     0,    35
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     2,    12,     9,    18,    22,    23,    41,    42,    43,
      44,    45,    46,    47,    48,    49,   111,    93,    29,    51,
     112,   113
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -47
static const yytype_int16 yypact[] =
{
      15,   138,     7,   -47,   -47,   -47,    19,   -47,   -47,   -47,
      37,    12,    48,    55,    48,   -47,   -47,   138,     2,   -47,
     -47,    48,    -8,   -47,    80,   106,    27,   -47,   -47,    65,
     107,   107,    48,   111,   -47,   138,   138,   138,    48,    63,
     -47,   -47,   114,    53,    48,    29,    76,   114,   114,   -47,
     -47,    48,   -47,    48,   113,   129,   135,   125,   138,   138,
     -47,   114,   143,    68,   142,   143,    49,    68,    49,   114,
     -47,   -47,    48,   114,   114,   138,   138,    57,   -47,   -47,
     -47,   -47,    28,   -47,   -47,   -47,   -47,   114,   -47,   -47,
     -47,   -47,   -47,   138,   123,   -47,    69,   117,    67,    67,
      67,    67,    67,   -47,    48,    67,    67,    67,    67,    67,
      67,   -47,   -47,   -47,   -47,   -47,   -47,   -47,    88,   -47,
     -47,   -47,   -47,   -47,   -47,   -47,    48,    98,   -47,    48,
     104,   -47,    48,   110,   -47,    48,   116,   -47,    48,   134,
     -47,    48,   136,   -47,    48,   139,   -47,    48,   144,   -47
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -47,   -47,   -47,   153,   -47,   -47,   147,   141,   119,   -23,
     -47,   -38,    -5,   -40,   -46,   -42,    -1,   -47,   -47,   121,
      -2,    17
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -37
static const yytype_int16 yytable[] =
{
      10,    69,    61,    21,    26,    62,    70,     7,    68,    15,
      10,     8,    19,    21,    74,    69,    24,    27,     6,    25,
      24,     8,    65,    67,    33,    81,     8,    70,    69,    85,
      52,     8,     1,    87,    20,    14,    57,    11,    63,    39,
      66,    69,    64,    84,    40,    86,     8,     8,   -36,    71,
      32,    52,    54,    55,    56,    95,    60,    80,    36,    37,
      83,    13,    90,    39,    40,    91,    92,     8,   -36,    17,
      71,    58,    59,    36,    37,    78,    79,     3,     4,     5,
      96,    36,    37,     3,     4,     5,     8,     3,     4,     5,
      34,    40,    88,    89,    94,   -36,   103,   104,   114,   115,
     116,   117,   118,    30,   119,   120,   121,   122,   123,   124,
      97,    35,    36,    37,    38,   125,   126,    39,    36,    37,
      38,    38,    40,    39,   127,   128,   129,   130,    40,    31,
     133,   131,   132,   136,    53,    75,   139,   134,   135,   142,
      76,    77,   145,   137,   138,   148,   105,   106,   107,   108,
     109,   110,    98,    39,    99,   100,   101,   102,     3,     4,
       5,   140,   141,   143,   144,    16,   146,   147,    82,    28,
       0,   149,    50,    73,    72
};

static const yytype_int16 yycheck[] =
{
       2,    47,    42,    11,    12,    43,    48,     0,    46,    11,
      12,    19,    14,    11,    54,    61,    18,    25,     1,    21,
      22,    19,    45,    46,    26,    63,    19,    69,    74,    67,
      32,    19,    17,    73,    17,    23,    38,    18,    43,    10,
      45,    87,    44,    66,    15,    68,    19,    19,    19,    51,
      23,    53,    35,    36,    37,    27,    39,    62,     5,     6,
      65,    24,     5,    10,    15,     8,     9,    19,    19,    14,
      72,     8,     9,     5,     6,    58,    59,    20,    21,    22,
      82,     5,     6,    20,    21,    22,    19,    20,    21,    22,
      25,    15,    75,    76,    77,    19,    27,    28,    99,   100,
     101,   102,   104,    23,   105,   106,   107,   108,   109,   110,
      93,     4,     5,     6,     7,    27,    28,    10,     5,     6,
       7,     7,    15,    10,   126,    27,    28,   129,    15,    23,
     132,    27,    28,   135,    23,     6,   138,    27,    28,   141,
       5,    16,   144,    27,    28,   147,    29,    30,    31,    32,
      33,    34,    29,    10,    31,    32,    33,    34,    20,    21,
      22,    27,    28,    27,    28,    12,    27,    28,    26,    22,
      -1,    27,    31,    54,    53
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    17,    36,    20,    21,    22,    56,     0,    19,    38,
      55,    18,    37,    24,    23,    55,    38,    14,    39,    55,
      56,    11,    40,    41,    55,    55,    12,    25,    41,    53,
      23,    23,    23,    55,    25,     4,     5,     6,     7,    10,
      15,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      42,    54,    55,    23,    56,    56,    56,    55,     8,     9,
      56,    48,    46,    47,    55,    44,    47,    44,    46,    49,
      50,    55,    54,    43,    48,     6,     5,    16,    56,    56,
      47,    46,    26,    47,    44,    46,    44,    48,    56,    56,
       5,     8,     9,    52,    56,    27,    55,    56,    29,    31,
      32,    33,    34,    27,    28,    29,    30,    31,    32,    33,
      34,    51,    55,    56,    51,    51,    51,    51,    55,    51,
      51,    51,    51,    51,    51,    27,    28,    55,    27,    28,
      55,    27,    28,    55,    27,    28,    55,    27,    28,    55,
      27,    28,    55,    27,    28,    55,    27,    28,    55,    27
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1455 of yacc.c  */
#line 109 "spot.y"
    {
				pnum = (yyvsp[(2) - (4)].cpval);
			;}
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 117 "spot.y"
    {
				*restartFuncPtr++ = (yyvsp[(3) - (3)].cpval);
			;}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 121 "spot.y"
    {
				*restartFuncPtr++ = (yyvsp[(2) - (2)].cpval);
			;}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 127 "spot.y"
    {
				if(!first_seen) {
				    fprintf(stderr,
							"CHAIN '%s' has no 'begin' state\n",
							(yyvsp[(1) - (6)].cpval));
				    exit(1);
				}
				*chainPtr++ = (yyvsp[(1) - (6)].cpval);
				chain_cnt++;
				first_seen= 0;
				abortCount++;
			;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 140 "spot.y"
    {
				if(!first_seen) {
					fprintf(stderr,
							"CHAIN '%s' has no 'begin' state\n",
							(yyvsp[(1) - (5)].cpval));
					exit(1);
				}
				*chainPtr++ = (yyvsp[(1) - (5)].cpval);
				chain_cnt++;
				first_seen= 0;
				abortCount++;
			;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 155 "spot.y"
    {
			*chainStatusPtr++ = (yyvsp[(2) - (2)].cpval);
		;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 161 "spot.y"
    {
				statePtr++;
				escpIndx = 0;
			;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 166 "spot.y"
    {
				statePtr++;
				escpIndx = 0;
			;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 173 "spot.y"
    {
				if(strlen((yyvsp[(1) - (3)].cpval)) > 30) {
					yyerror("State name too long");
				}
				statePtr->sn = (yyvsp[(1) - (3)].cpval);

				if (escpIndx >= MAXESC) {
					yyerror("Too many escapes");
				}
			;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 184 "spot.y"
    {
				if(strncmp(zero, statePtr->sact, sizeof(zero)) != 0) {
					yyerror("\'begin\' state cannot contain an action");
				}
				if(first_seen) {
					yyerror("Only one \'begin\' state per chain allowed");
				}
				else first_seen= 1;

				if(strlen((yyvsp[(2) - (4)].cpval)) > 30) {
					yyerror("State name too long");
				}
				*firstStatePtr++ = (yyvsp[(2) - (4)].cpval);
				statePtr->sn = (yyvsp[(2) - (4)].cpval);

				if(escpIndx >= MAXESC) {
					yyerror("Too many escapes");
				}
			;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 206 "spot.y"
    {
				if((yyvsp[(2) - (4)].cpval)[0] == '&') statePtr->pc = (yyvsp[(2) - (4)].cpval);
				else statePtr->sc = (yyvsp[(2) - (4)].cpval);
			;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 211 "spot.y"
    {
				if((yyvsp[(2) - (3)].cpval)[0] == '&') statePtr->pc = (yyvsp[(2) - (3)].cpval);
				else statePtr->sc = (yyvsp[(2) - (3)].cpval);
			;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 229 "spot.y"
    {
			statePtr->sact = (yyvsp[(2) - (4)].cpval);
		;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 233 "spot.y"
    {
				statePtr->sact = (yyvsp[(2) - (5)].cpval);
				statePtr->saa1 = (yyvsp[(4) - (5)].cpval);
			;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 238 "spot.y"
    {
				statePtr->sact = (yyvsp[(2) - (7)].cpval);
				statePtr->saa1 = (yyvsp[(4) - (7)].cpval);
				statePtr->saa2 = (yyvsp[(6) - (7)].cpval);
			;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 244 "spot.y"
    {
				statePtr->sact = (yyvsp[(2) - (9)].cpval);
				statePtr->saa1 = (yyvsp[(4) - (9)].cpval);
				statePtr->saa2 = (yyvsp[(6) - (9)].cpval);
				statePtr->saa3 = (yyvsp[(8) - (9)].cpval);
			;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 252 "spot.y"
    {
				statePtr->sact = (yyvsp[(2) - (11)].cpval);
				statePtr->saa1 = (yyvsp[(4) - (11)].cpval);
				statePtr->saa2 = (yyvsp[(6) - (11)].cpval);
				statePtr->saa3 = (yyvsp[(8) - (11)].cpval);
				statePtr->saa4 = (yyvsp[(10) - (11)].cpval);
			;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 261 "spot.y"
    {
				statePtr->sact = (yyvsp[(2) - (13)].cpval);
				statePtr->saa1 = (yyvsp[(4) - (13)].cpval);
				statePtr->saa2 = (yyvsp[(6) - (13)].cpval);
				statePtr->saa3 = (yyvsp[(8) - (13)].cpval);
				statePtr->saa4 = (yyvsp[(10) - (13)].cpval);
				statePtr->saa5 = (yyvsp[(12) - (13)].cpval);
			;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 271 "spot.y"
    {
				statePtr->sact = (yyvsp[(2) - (15)].cpval);
				statePtr->saa1 = (yyvsp[(4) - (15)].cpval);
				statePtr->saa2 = (yyvsp[(6) - (15)].cpval);
				statePtr->saa3 = (yyvsp[(8) - (15)].cpval);
				statePtr->saa4 = (yyvsp[(10) - (15)].cpval);
				statePtr->saa5 = (yyvsp[(12) - (15)].cpval);
				statePtr->saa6 = (yyvsp[(14) - (15)].cpval);
			;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 282 "spot.y"
    {
				statePtr->sact = (yyvsp[(2) - (17)].cpval);
				statePtr->saa1 = (yyvsp[(4) - (17)].cpval);
				statePtr->saa2 = (yyvsp[(6) - (17)].cpval);
				statePtr->saa3 = (yyvsp[(8) - (17)].cpval);
				statePtr->saa4 = (yyvsp[(10) - (17)].cpval);
				statePtr->saa5 = (yyvsp[(12) - (17)].cpval);
				statePtr->saa6 = (yyvsp[(14) - (17)].cpval);
				statePtr->saa7 = (yyvsp[(16) - (17)].cpval);
			;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 295 "spot.y"
    {
				statePtr->sact = (yyvsp[(2) - (19)].cpval);
				statePtr->saa1 = (yyvsp[(4) - (19)].cpval);
				statePtr->saa2 = (yyvsp[(6) - (19)].cpval);
				statePtr->saa3 = (yyvsp[(8) - (19)].cpval);
				statePtr->saa4 = (yyvsp[(10) - (19)].cpval);
				statePtr->saa5 = (yyvsp[(12) - (19)].cpval);
				statePtr->saa6 = (yyvsp[(14) - (19)].cpval);
				statePtr->saa7 = (yyvsp[(16) - (19)].cpval);
				statePtr->saa8 = (yyvsp[(18) - (19)].cpval);
			;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 309 "spot.y"
    {
				statePtr->sact = (yyvsp[(2) - (21)].cpval);
				statePtr->saa1 = (yyvsp[(4) - (21)].cpval);
				statePtr->saa2 = (yyvsp[(6) - (21)].cpval);
				statePtr->saa3 = (yyvsp[(8) - (21)].cpval);
				statePtr->saa4 = (yyvsp[(10) - (21)].cpval);
				statePtr->saa5 = (yyvsp[(12) - (21)].cpval);
				statePtr->saa6 = (yyvsp[(14) - (21)].cpval);
				statePtr->saa7 = (yyvsp[(16) - (21)].cpval);
				statePtr->saa8 = (yyvsp[(18) - (21)].cpval);
				statePtr->saa9 = (yyvsp[(20) - (21)].cpval);
			;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 324 "spot.y"
    {
				statePtr->sact = (yyvsp[(2) - (23)].cpval);
				statePtr->saa1 = (yyvsp[(4) - (23)].cpval);
				statePtr->saa2 = (yyvsp[(6) - (23)].cpval);
				statePtr->saa3 = (yyvsp[(8) - (23)].cpval);
				statePtr->saa4 = (yyvsp[(10) - (23)].cpval);
				statePtr->saa5 = (yyvsp[(12) - (23)].cpval);
				statePtr->saa6 = (yyvsp[(14) - (23)].cpval);
				statePtr->saa7 = (yyvsp[(16) - (23)].cpval);
				statePtr->saa8 = (yyvsp[(18) - (23)].cpval);
				statePtr->saa9 = (yyvsp[(20) - (23)].cpval);
				statePtr->saa10 = (yyvsp[(22) - (23)].cpval);
			;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 343 "spot.y"
    {
				statePtr->spt = (yyvsp[(2) - (2)].cpval);
			;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 347 "spot.y"
    {
				statePtr->srt = (yyvsp[(2) - (2)].cpval);
			;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 351 "spot.y"
    {
				statePtr->spt = (yyvsp[(2) - (4)].cpval);
				statePtr->srt = (yyvsp[(4) - (4)].cpval);
			;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 356 "spot.y"
    {
				statePtr->srt = (yyvsp[(2) - (4)].cpval);
				statePtr->spt = (yyvsp[(4) - (4)].cpval);
			;}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 364 "spot.y"
    {
				statePtr->srlv = one;
				statePtr->srlf = (yyvsp[(2) - (2)].cpval);
			;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 369 "spot.y"
    {
				statePtr->srlv = two;
				statePtr->srlf = (yyvsp[(3) - (3)].cpval);
			;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 374 "spot.y"
    {
				statePtr->srlv = three;
				statePtr->srlf = (yyvsp[(3) - (3)].cpval);
			;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 381 "spot.y"
    {
			;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 384 "spot.y"
    {
			;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 390 "spot.y"
    {
			;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 395 "spot.y"
    {
			statePtr->es[escpIndx].se = (yyvsp[(2) - (6)].cpval);
			statePtr->es[escpIndx].sf = equal;
			statePtr->es[escpIndx].sad = (yyvsp[(6) - (6)].cpval);
			statePtr->es[escpIndx].sm = (yyvsp[(4) - (6)].cpval);
			escpIndx++;
		;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 403 "spot.y"
    {
			char *tmp;

			/* this will work because a leading ' ' exists */
			if ((yyvsp[(4) - (7)].cpval) == bitoff) {
				(yyvsp[(5) - (7)].cpval) = (yyvsp[(5) - (7)].cpval) - 1;
				tmp = (yyvsp[(5) - (7)].cpval);
				*tmp = '-';
			}
			statePtr->es[escpIndx].se = (yyvsp[(2) - (7)].cpval);
			statePtr->es[escpIndx].sf = equal;
			statePtr->es[escpIndx].sad = (yyvsp[(7) - (7)].cpval);
			statePtr->es[escpIndx].sm = (yyvsp[(5) - (7)].cpval);
			escpIndx++;
		;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 419 "spot.y"
    {
			statePtr->es[escpIndx].se = (yyvsp[(2) - (7)].cpval);
			statePtr->es[escpIndx].sf = (yyvsp[(4) - (7)].cpval);
			statePtr->es[escpIndx].sad = (yyvsp[(7) - (7)].cpval);
			statePtr->es[escpIndx].sm = (yyvsp[(5) - (7)].cpval);
			escpIndx++;
		;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 427 "spot.y"
    {
			statePtr->es[escpIndx].se = (yyvsp[(2) - (6)].cpval);
			statePtr->es[escpIndx].sf = greater;
			statePtr->es[escpIndx].sad = (yyvsp[(6) - (6)].cpval);
			statePtr->es[escpIndx].sm = (yyvsp[(4) - (6)].cpval);
			escpIndx++;
		;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 435 "spot.y"
    {
			char *tmp;

			/* this will work because a leading ' ' exists */
			if ((yyvsp[(4) - (7)].cpval) == bitoff) {
				(yyvsp[(5) - (7)].cpval) = (yyvsp[(5) - (7)].cpval) - 1;
				tmp = (yyvsp[(5) - (7)].cpval);
				*tmp = '-';
			}
			statePtr->es[escpIndx].se = (yyvsp[(2) - (7)].cpval);
			statePtr->es[escpIndx].sf = greater;
			statePtr->es[escpIndx].sad = (yyvsp[(7) - (7)].cpval);
			statePtr->es[escpIndx].sm = (yyvsp[(5) - (7)].cpval);
			escpIndx++;
		;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 451 "spot.y"
    {
			statePtr->es[escpIndx].se = (yyvsp[(2) - (6)].cpval);
			statePtr->es[escpIndx].sf = less;
			statePtr->es[escpIndx].sad = (yyvsp[(6) - (6)].cpval);
			statePtr->es[escpIndx].sm = (yyvsp[(4) - (6)].cpval);
			escpIndx++;
		;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 459 "spot.y"
    {
			char *tmp;

			/* this will work because a leading ' ' exists */
			if ((yyvsp[(4) - (7)].cpval) == bitoff) {
				(yyvsp[(5) - (7)].cpval) = (yyvsp[(5) - (7)].cpval) - 1;
				tmp = (yyvsp[(5) - (7)].cpval);
				*tmp = '-';
			}
			statePtr->es[escpIndx].se = (yyvsp[(2) - (7)].cpval);
			statePtr->es[escpIndx].sf = less;
			statePtr->es[escpIndx].sad = (yyvsp[(7) - (7)].cpval);
			statePtr->es[escpIndx].sm = (yyvsp[(5) - (7)].cpval);
			escpIndx++;
		;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 475 "spot.y"
    {
			statePtr->es[escpIndx].se = (yyvsp[(2) - (6)].cpval);
			statePtr->es[escpIndx].sf = question;
			statePtr->es[escpIndx].sad = (yyvsp[(6) - (6)].cpval);
			statePtr->es[escpIndx].sm = (yyvsp[(4) - (6)].cpval);
			escpIndx++;
		;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 483 "spot.y"
    {
			char *tmp;

			/* this will work because a leading ' ' exists */
			if ((yyvsp[(4) - (7)].cpval) == bitoff) {
				(yyvsp[(5) - (7)].cpval) = (yyvsp[(5) - (7)].cpval) - 1;
				tmp = (yyvsp[(5) - (7)].cpval);
				*tmp = '-';
			}
			statePtr->es[escpIndx].se = (yyvsp[(2) - (7)].cpval);
			statePtr->es[escpIndx].sf = question;
			statePtr->es[escpIndx].sad = (yyvsp[(7) - (7)].cpval);
			statePtr->es[escpIndx].sm = (yyvsp[(5) - (7)].cpval);
			escpIndx++;
		;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 499 "spot.y"
    {
			statePtr->es[escpIndx].se = (yyvsp[(2) - (6)].cpval);
			statePtr->es[escpIndx].sf = function;
			statePtr->es[escpIndx].sad = (yyvsp[(6) - (6)].cpval);
			statePtr->es[escpIndx].sm = (yyvsp[(4) - (6)].cpval);
			escpIndx++;
		;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 507 "spot.y"
    {
			char *tmp;

			/* this will work because a leading ' ' exists */
			if ((yyvsp[(4) - (7)].cpval) == bitoff) {
				(yyvsp[(5) - (7)].cpval) = (yyvsp[(5) - (7)].cpval) - 1;
				tmp = (yyvsp[(5) - (7)].cpval);
				*tmp = '-';
			}
			statePtr->es[escpIndx].se = (yyvsp[(2) - (7)].cpval);
			statePtr->es[escpIndx].sf = function;
			statePtr->es[escpIndx].sad = (yyvsp[(7) - (7)].cpval);
			statePtr->es[escpIndx].sm = (yyvsp[(5) - (7)].cpval);
			escpIndx++;
		;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 523 "spot.y"
    {
			statePtr->es[escpIndx].se = (yyvsp[(2) - (2)].cpval);
			statePtr->es[escpIndx].sf = time;
			escpIndx++;
		;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 529 "spot.y"
    {
			statePtr->es[escpIndx].se = (yyvsp[(2) - (4)].cpval);
			statePtr->es[escpIndx].sf = time;
			escpIndx++;
		;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 536 "spot.y"
    {
				(yyval.cpval) = (yyvsp[(1) - (1)].cpval);
			;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 540 "spot.y"
    {
				(yyval.cpval) = (yyvsp[(1) - (1)].cpval);
			;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 545 "spot.y"
    {
				(yyval.cpval) = biton;
			;}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 549 "spot.y"
    {
				(yyval.cpval) = bitoff;
			;}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 560 "spot.y"
    {
				abortList[abortCount++] = (yyvsp[(1) - (1)].cpval);
			;}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 564 "spot.y"
    {
				abortList[abortCount++] = (yyvsp[(2) - (2)].cpval);
			;}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 570 "spot.y"
    {
				(yyval.cpval) = (yyvsp[(1) - (1)].cpval);
			;}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 576 "spot.y"
    {
				(yyval.cpval) = (yyvsp[(1) - (1)].cpval);
			;}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 580 "spot.y"
    {
				(yyval.cpval) = (yyvsp[(1) - (1)].cpval);
			;}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 584 "spot.y"
    {
				(yyval.cpval) = (yyvsp[(1) - (1)].cpval);
			;}
    break;



/* Line 1455 of yacc.c  */
#line 2256 "spot.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 589 "spot.y"


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


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
 *	System wide definitions, typedefs.  Every REX source file 
 * includes this header first.
 */

/*
 * Version identification.
 */
#define RHEADER "NEI/LSR REX 7.1"

/*
 * interrupt blocked flag bit
 */
#define INTR_IF 0x0200

/*
 * Names of shared memory areas.
 */
#define I_NAME "INT_BLOCK"
#define R_NAME "RAW_BLOCK"

/*
 *	General typedefs.
 */
typedef short int DATUM;		/* type of data */
typedef long	AVG_DATUM;		/* longs for average accumulator */
typedef unsigned	unsign;

/*
 *	System wide definitions.
 */
#define REX		1
#define WARN		0	/* tells cprint to print a warning msg */
#define PRINT		1	/* tells cprint to print normal msg */
#define HIGHBIT		0100000
#define MAXCAL		10	/* max range of internal calibration nums */
#define MAXMAG		6	/* max range of window disp magnifications */
#define MAXUNSIGN	65535
#define MAXINT		32767	    /* max signed int */
#define MININT		-32768	    /* min signed int */
#define MAXINT_L	2147483647	/* max long signed int */
#define MININT_L	-2147483648	/* mix long signed int */
#define EINTR		4	/* interrupted sys call error */
#define PT	(i_b->ptbl)	/* shorthand for accessing ptbl */
#define NULLI		MININT	/* -32768, null int argument */
#define NULLI_L		MININT_L    /* null long argument */
#define NP		0	/* null return of things returning pointers.
				   Under Unix this is ok since no data address
				   is allowed to be 0.  This might have to
				   be made a real address under other circum-
				   stances. */
extern char NS[];		/* pointer to null string */

/*
 * Following pointers used to access PROCTBL entries for main REX procs.
 */
#define COMM		(&(i_b->ptbl[0]))	/* comm is always ptbl[0] */
#define SCRB		(&(i_b->ptbl[i_b->scrb_pi]))
#define INT			(&(i_b->ptbl[i_b->int_pi]))
#define DISP		(&(i_b->ptbl[i_b->disp_pi]))

/*
 *	General macros.
 */
#ifdef GOO
#define abs(x)		((x) < 0 ? -(x) : (x))
#define min(x, y)	(((x) < (y)) ? (x) : (y))
#define max(x, y)	(((x) < (y)) ? (y) : (x))
#endif

#define sgn(x)		((x) < 0 ? -1 : 1)
#define release_(adr)	(*(adr)= HIGHBIT)   /* release a semaphore */
#define s_(msg)		(01 << msg)	    /* used to get octal const from
					       msg number */
 
/*
 * On the pdp11 compiler, one could shift by a negative amount.
 * Now, one cant.  This macro is for code that relied on this
 * feature.  Macro is constructed so that args can be of the
 * form *ptr++ and still work (arg only evaluated once).  Must
 * declare "int shift_tmp" in any function which uses this.
 */
#define shift_(amount, value)	((shift_tmp=(amount)) >= 0 ? \
				 ((value) << (shift_tmp)) : \
				 ((value) >> -(shift_tmp)))

/*
 *	Access parts of ints, longs.
 */
#define longhi_(a)	(short)((a >> 16) & 0xffff)
#define longlo_(a)	(short)(a & 0xffff)
#define shorthi_(a)	(short)((a >> 8) & 0xff)
#define shortlo_(a)	(short)(a & 0xff)

/*
 * Debugging support.
 */
extern int doutput_exit, doutput_rot, doutput_inmem; /* see sys/rlib/dputs.c */
extern int doutput_tofile;
extern char *doutput_filename;

/*
 * Function prototypes.
 */
/* char far *f_itoa_RL(int num, char flag, char far *to, char far *ep); */
/* char far *f_ltoa_RL(long num, char flag, char far *to, char far *ep); */
char *itoa_RL(int num, char flag, char *to, char *ep);
char *ltoa_RL(long num, char flag, char *to, char *ep);
int btoi_RL(char *string, int base);
long btol_RL(char *string, int base);
/* void rxerr(char far *p); */
void rxerr(char *p);
int sindex(char *mstr, char **ptr, int size);
int rt_write(char *i_rtname);
void sleep_comm(void);
void wakeup_comm(void);
int match(char *str1, char *str2);
int parse(int ntflag, char *p, char *argv[], int nargs);
int nullf(void);
void badverb(void);
int rt_close(void);
char *fills(char c, int num, char *p, char *ep);
volatile void * map_shared(char *name, int size);
int rt_read(void);
char *stufs(char *from, char *to, char *endp);
char *index_RL(char c, char *str);
void dputchar(char c);
/* void dputs(char far *p); */
void dputs(char *p);
void dprintf(char *fmt, ...);
void doutput(int sig);
/* char far *_fstufs(char far *from, char far *to, char far *endp); */
/* char far *_findex_RL(char c, char far *str); */
void protect(u_int *p);

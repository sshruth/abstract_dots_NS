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
 *	comm.h:  Local header for comm process.
 */

#define ERASECNT	2	/* num of input command lines to wait before
				   erasing previous error status output */

/*
 * Flags local to comm kept in sysflags.
 */
#define C_QUIT		01	/* terminate REX */
#define C_SENDNAME	02	/* child is ready to send names */
#define C_BADCHILD	04	/* child forked by comm has failed */
#define C_NAMERDY	010	/* comm is ready to receive names from child */
#define C_AFFIRM	020	/* process successfully responds to a comm
				   message (ususally run or stop) */
#define C_NEG		040	/* process cannot repsond successfully to a
				   comm message */
#define clrack_		(sysflags &= ~(C_AFFIRM|C_NEG))

/*
 *	Local variables.
 */
extern int sysflags;		/* local flags for comm */
extern int nproc;		/* number of currently exectued procs */
extern int nnoun;		/* current number of nouns */
extern int nmenu;		/* current number of menus */
extern NAME nountbl[];		/* system noun table */
extern NAME menutbl[];		/* system menu table */
extern char nullst[];		/* null char string containing only '\0' */
extern PROCTBL zerop;		/* used to zero entries in proctbl */
extern PROCTBL_P getptp();
extern char outbuf[];		/* to buffer stdout */
extern int rtflag;		/* root flag */
extern int rtlevel;		/* root read nesting level */
extern RTSAV rtsav[];		/* stack to store state of nested roots */
extern int errnum;		/* current error number accum by rxerr() */
extern int rerr_erase;		/* this var is cleared by rxerr() and inc'd
				   for each command input line;  when count
				   == ERASECNT the error status line is
				   erased */
extern sigset_t	alert_set;
extern int mxmode_st;		/* non-zero when console has been switched
				   to graphics mode */

/*
 * Prototypes.
 */
void alert(int sig);
void p_rxerr(char *s, int ptx);
int status(int flag);
void getnames(PROCTBL_P p);
int rsproc(PROCTBL_P p, char state);
int rtreof(void);
int
sendnm(PROCTBL_P p, char *nmstr, char *vstr, char *argstr, int noerrprt);
void endproc(PROCTBL_P p);
void n_tty(char *vstr, char *astr);
void n_name(char *vstr, char *astr);
int n_rex(char *vstr, char *astr);
void n_trials(char *vstr, char *astr);
void n_debug(char *vstr, char *astr);
int n_root(char *vstr, char *astr);
int rtalert(PROCTBL_P pp);
void n_proc(char *vstr, char *astr);
PROCTBL_P newproc(char *name);
void tproc(void);
PROCTBL_P getptp(char *name);
int crproc(PROCTBL_P p);
void pproc(void);
void tname(char what);
int waitack(int timeout, int signal);
void oncntrlb(int sig);

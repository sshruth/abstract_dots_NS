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
 *	scribe.h:  Local header for scribe process.
 */

/*
 *	E and A file descriptors.
 */
extern int analog_fd;
extern int event_fd;

#define FLUSH_E_WIN_CLOSE   /* define to flush E and A files on each window
			     * close;  useful if one is trying to analyze
			     * data from growing E and A files from another
			     * program (e.g. Matlab) as REX is running.  Will
			     * reduce efficiency and may impact performance if
			     * window opens are very short */


/*
 * Dump error codes, kept in 'derror'.
 */
extern int derror;
extern char outbuf[];		/* to buffer stdout */

#define E_ANERR		01	/* analog error */
#define E_EVERR		02	/* event overflow error */
#define E_ABADW		04	/* error writing A file to disk */
#define E_EBADW		010	/* error writing E file to disk */
#define E_CNTLB		020	/* cntl b received by scribe */
#define E_ADERR		040	/* a/d converter error */

/*
 * Function prototypes.
 */
void alert(int sig);
void dumperr(int error);
void dumpe(int flush);
void dumpa(void);
void n_file(char *vstr, char *astr);
void n_adata(char *vstr, char *astr);
void n_edata(char *vstr, char *astr);
void oncntrlb(int sig);
void n_bdata(char *vstr, char *astr);
int filecr(char *astr);
int stufhdr(CHANNEL *pchan, int fd, int size);


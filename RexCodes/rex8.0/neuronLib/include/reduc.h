/* reduc.h -- header for reducing firing rates */
#undef DEBUG
#define LATENCY		80		/* msec from onset to response */
#define INTERPOLATE	0
#define OUT_SIZE	64

#define OUT_WINDOW	(64*6)		/* number of milliseconds after epoch */

extern int sysflags;

extern char hdr[];
extern char rname[];
extern char filt_name[];
extern char out_name[];
extern FILE *ostrm, *istrm, *instrm;
extern int no_plot;
extern int no_filt;

extern int n_dat;		/* length of data record */
extern int on_k;	/* offset to stimulus onset */
extern int off_k;	/* offset to stimulus offset */
extern int trig;	/* trigger number */
extern int lo_trig, hi_trig;	/* threshold for ignoring responses in average */
extern int epoch;	/* entropy - determined epoch */
extern int latency;
extern int interp;
extern int start;

extern int nproc;
extern int nmean;
extern long key_nmean;
extern int npts;

extern int out_size;
extern int out_window;

extern DFILTER *in_buf;
extern DFILTER *f_buf, *pf, *pfl;
extern float out_buf[OUT_SIZE];
extern float out_bar[OUT_SIZE];
extern short int *tme;

extern DIFFER filt;

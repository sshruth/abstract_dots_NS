#define MAXFILES	128	/* maximum number of filenames */
#define MAXVAR		256
extern int dim;

extern int sysflags;

extern char hdr[512];
extern FILE *ostrm;
extern FILE *instrm;
extern char out_name[];
extern char *rname[];
extern int file_num;
extern int fcount;
extern char *varname[];
extern int n_var;

extern int *codes;		/* code for each trial */
extern float **vars;		/* variables for each trial */
extern int ltimes;		/* length of times */
extern int ntimes;		/* number of times used */
extern int ltrials;		/* length of trials */
extern int ntrials;		/* number of trials */

extern MATRIX *sigma;	/* data domain covariance matrix */
extern VECTOR *z;
extern VECTOR *zbar;	/* average response */
extern MATRIX *T;	/* the forward transform:  y = T * x */
extern MATRIX *Tp;	/* the inverse transform:  x = Tp * y */
extern VECTOR *lambda;	/* eigenvalues */

extern int n_dat;		/* length of data record */
extern int trig;	/* trigger number */

extern int nproc;	/* number of records processed */
extern int nmean;	/* number of records in averaged response */
extern int in_size;

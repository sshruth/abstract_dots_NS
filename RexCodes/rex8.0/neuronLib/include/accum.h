#define DIM	64	/* number of points in a data vector */

extern int dim;

extern int sysflags;

extern char hdr[512];
extern FILE *ostrm;
extern FILE *instrm;
extern char out_name[];
extern char rname[20][50];

extern int file_num;
extern int fcount;

extern MATRIX sigma[DIM][DIM];	/* data domain covariance matrix */
extern VECTOR z[DIM];
extern VECTOR zbar[DIM];	/* average response */

extern int n_dat;		/* length of data record */
extern int trig;	/* trigger number */

extern int nproc;	/* number of records processed */
extern int nmean;	/* number of records in averaged response */
extern int in_size;

#define DIM	64	/* number of points in a data vector */

#define NAMESZ	256

extern int sysflags;

extern VECTOR lambda[DIM];	/* eigenvalues fo data covariance matrix */

extern int dim;
extern int nproc;

extern char hdr[512];
extern FILE *ostrm, *instrm;
extern char out_name[], rname[];

extern MATRIX sigma[DIM][DIM];	/* data domain covariance matrix */

extern MATRIX nu[DIM][DIM];	/* Eigenvector matrix */
extern MATRIX *T;	/* pointer to transformation matrix */

#define DIM	64	/* number of points in a data vector */

/*
 * Plot space for Variance
 */
#define LVAR	8000
#define HVAR	7000
#define X0_VAR	1000
#define Y0_VAR	1000

extern float lambda[];	/* eigenvalues fo data covariance matrix */

extern int dim;
extern char *out_name;
extern long rbeg;

extern MATRIX sigma[DIM][DIM];	/* data domain covariance matrix */
extern VECTOR z[DIM];
extern VECTOR zbar[DIM];	/* average response */

extern MATRIX nu[DIM][DIM];	/* Eigenvector matrix */
extern MATRIX *T;	/* pointer to transformation matrix */

extern char hdr[512];
extern FILE *ostrm;

extern int n_dat;		/* length of data record */
extern int trig;	/* trigger number */

extern int nproc;	/* number of records processed */

extern int in_size;	/* number of points in input vector */
extern short int tme[DIM];

extern short int evnum[];
extern float norm[], cum[];
extern int x_off, y_off;

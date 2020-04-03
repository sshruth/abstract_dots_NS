/* xform.h */
#define DIM	64

extern int dim;
extern int sysflags;

extern char out_name[], e_name[], rname[];

extern MATRIX T[DIM][DIM];	/* KLT matrix, with eigenvectors as columns */
extern VECTOR z[DIM];		/* response vector */
extern VECTOR y[DIM];		/* transformed vector */
extern VECTOR zbar[DIM];	/* average response */

extern FILE *ostrm;
extern FILE *estrm;
extern FILE *instrm;

extern int trig;	/* trigger number */

extern int nproc;	/* number of records processed */
extern int nmean;	/* number of records in average */
extern long key_nproc;

extern int in_size;	/* number of points in input vector */

/* xfilt.h */
#define DIM	64
#define NCOMP	3	/* number of components of KLE to keep */

extern int dim;
extern int ncomp, scomp, lcomp;
extern int sysflags;
extern int nmean;

extern char out_name[], e_name[], rname[];
extern char x_name[];

extern MATRIX T[DIM][DIM];	/* KLT matrix */
extern VECTOR x[DIM];		/* response vector */
extern VECTOR y[DIM];		/* transformed vector */
extern VECTOR zbar[DIM];	/* average response */

extern FILE *ostrm;
extern FILE *estrm, *xstrm;
extern FILE *instrm;

extern int trig;	/* trigger number */

extern int nproc;	/* number of records processed */

extern int in_size;	/* number of points in input vector */

#define DIM	64	/* number of points in a data vector */
#define NCOMP	6	/* number of components to keep */
#define NCL	75	/* number of classes */

typedef struct {
	int c_n;	/* number in class */
	float c_scat;	/* scatter */
	VECTOR c_w[NCOMP];	/* component weights */
} CLASS;

/*
 * Plot space for Variance
 */
#define LVAR	8000
#define HVAR	7000
#define X0_VAR	1000
#define Y0_VAR	1000

extern float lambda[];	/* eigenvalues of data covariance matrix */

extern int dim;
extern char *out_name;
extern long rbeg;

extern VECTOR z[DIM];
extern VECTOR zt[DIM];	/* average response */

extern MATRIX T[DIM][DIM];

extern char hdr[512];
extern FILE *ostrm;

extern int trig;	/* trigger number */

extern int nproc;	/* number of records processed */
extern int nclass;

extern int in_size;	/* number of points in input vector */
extern int tme[DIM];

extern float cum[DIM];
extern VECTOR vmin[NCOMP];
extern VECTOR vmax[NCOMP];
extern CLASS class[NCL];

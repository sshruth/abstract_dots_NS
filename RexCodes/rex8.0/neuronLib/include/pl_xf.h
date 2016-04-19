/* pl_xf.h */

#define DIM	64

#define TRBASE	-1		/* trigger base */

extern int dim;
extern int sysflags;
#define F_XF	01	/* filtered data */

extern char rname[];
extern char *nmext;

extern VECTOR x[DIM];		/* response vector */
extern VECTOR y[DIM];		/* transformed vector */
extern VECTOR zbar[DIM];	/* average response */

extern FILE *xstrm;

extern int trig;	/* trigger number */
extern int trbase;
extern double avg;

extern int nproc;	/* number of records processed */

extern short int tme[DIM];

extern int x_off, y_off;

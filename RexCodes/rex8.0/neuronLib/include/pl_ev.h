/* pl_ev.h */

#define DIM	64
#define MAXEXT	5

#define GD_WD		9500
#define GD_HT		7500
#define GD_MIN		5		/* min step size in thousands inch */
#define CONF_INTERV	1.96		/* 95% confidence interval */

extern int sysflags;
extern int no_label;

/*
 *	FLAGS
 */
#define F_THICK		01		/* request thick lines */
#define F_BASELINE	02		/* request base lines */

extern FILE *instrm;

extern char rname[];
extern char rootnm[];
extern char *avglbl;
extern char nmext[MAXEXT];
extern int next;
extern int dim;
extern int nproc;
extern int nmean;

extern VECTOR zbar[DIM];
extern VECTOR lambda[MAXEXT][DIM];	/* eigenvalues of data covariance matrix */
extern MATRIX T[DIM][DIM];	/* Transpose of Eigenvector matrix */

extern short int tme[DIM];
extern int npc;	/* number of significant eigenvalues */
extern int nplot; /* number of traces to plot */

extern short int evnum[DIM];		/* index */
extern float norm[DIM];	/* normalized eigenvalues */
extern float cum[DIM];	/* cumulative eigenvalues */
extern float dci[DIM]; /* confidence interval */
extern int x_off;
extern int y_off;

extern char *title[];

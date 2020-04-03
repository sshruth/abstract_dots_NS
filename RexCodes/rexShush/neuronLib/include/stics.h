/* stics.h -- definitions for statistics programs */

/* means and standard deviations */
typedef struct deviations {
	int n;
	double mean;
	double stddev;
	double sterr;
	double sumi;
} MEANSD;

/* linear regression intermediate quantities */
typedef struct regrquant {
	int n;
	double xsum;
	double x2sum;
	double ysum;
	double y2sum;
	double xysum;

	double xsumsq;
	double ysumsq;
	double xysumsq;

	double q3;	/* the mysterious "Q3" of Sokal & Rohlf */
	double nxysum;
	double sx2;
	double sy2;
	double sxy;
	double ctx;
	double cty;
} REGRQ ;

/* linear regression coefficients */
typedef struct regrcoef {
	double slope;
	double intcpt;
	double seslope;
	double seintcpt;
	double explss;
	double unexplss;
	double rxy;	/* correlation coefficient */
	double sstotal;
	double ssgroups;
	int nu1;
	int nu2;
	double br_s12;
	double br_s22;
	double br_s32;
	double Flin;
	double Fzero;
	double plin;
	double pzero;
} REGRC ;

/* multiple Y-values in linear regression */
typedef struct {
	int ny;	/* number of Y-values for this X-value */
	double *yp;	/* pointer to the first Y-value */
	double sumy;	/* sub of the Y-values */
} MULY ;

double ftestQ();

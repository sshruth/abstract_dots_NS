/* sd_est.h
 * header for spike-density estimation subroutines
 */
#define FIX_H		3.0		/* default fixed-width */
#define ALPHA		0.5		/* default exponent for lambda */

#define CORR_H		1.0		/* correlation width */
#define CORR_LO		-20		/* max leftward shift for corr */
#define CORR_HI		20		/* max rightward shift for corr */

#define HISTBINS	50

typedef struct histo {
	float h_x;
	float h_y;
} HISTO;

#define EPSILON_LAMBDA	1e-6		/* smallest value for adaptive lambda */

#ifndef PI
#define	PI	3.14159265358979323846264338
#endif

#define TWOPI	(2 * PI)

#define abs_(x)	((x) < 0 ? -(x) : (x))
#define sqr_(x)	((x) * (x))

/*
 * types
 */
typedef struct sd_est {
	float *psd_avg;		/* average spike density waveform */
	float **psd_ind;	/* individual waveforms */
	float *psd_se;		/* std err waveform */
	int *psh_left;		/* shift-left lag times */
	float *psh_corr;	/* shift correlations */
	int h_lo;		/* lo h width */
	int h_hi;		/* hi h width */
	int h_avg;		/* average h width */
	float *psd_shifthx;	/* histogram of shift values */
	float *psd_shifthy;	/* histogram of shift values */
	int psd_shifthn;	/* number of points */
	float *psd_corrhx;	/* histogram of corr values */
	float *psd_corrhy;	/* histogram of corr values */
	int psd_corrhn;		/* number of points */
	float *psd_sigmahx;	/* histogram of adaptive sigma values */
	float *psd_sigmahy;	/* histogram of adaptive sigma values */
	int psd_sigmahn;	/* number of points */
} SD_EST;

SD_EST *sd_est();
float *fast_sd_est();
SD_EST *adap_k_est();

/*
 * Macros
 */
#define dyn_free_(x) {dyn_free(x); x = 0;}
#define dyn_pfree_(x) {dyn_pfree(x); x = 0;}

#ifndef DIFF_H
#define DIFF_H
/* DIFF.H
 *	Header for Differentiators and other FIR filters
 */
#define SHURE			/* cope with edge effects */

#define FILT_OFF	101
#define FILT_MAX	(2 * FILT_OFF)

#define TEMPSIZE	256	/* MUST BE A POWER OF 2! this determines
				 * the size of data buffer converted in one
				 * step taken by fftconv()
				 * Cannot be smaller than filter size
				 */

/* TYPEDEFS */

typedef float DFILTER;

/*
 *	Standard filter format is dptr[0] is furthest back in time!
 */
typedef struct {
	char *dname;	/* file name for FIR values */
	double dscale;	/* scale factor */
	int dnum;	/* number of points in half a filter,
				stored in dptr[] */
	int dlen;	/* length of whole filter */
	int dneg;	/* dneg == 1 ? antisymmetric : symmetric */
	double *dptr;	/* pointer to raw FIR values */
} DIFFER;
#endif

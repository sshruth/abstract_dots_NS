/* sd_alloc.c
 * functions to allocate data space for kernel estimates
 * HISTORY:
 *	17oct 92	LMO	remove from sd_est.c
 */
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <sys/types.h>
#include <malloc.h>
#include "neuron.h"
#include "diff.h"
#include "sd_est.h"

/*
 * pointers to signal buffers, offset to start of signal
 */
DFILTER **sd_ind = 0;		/* individual sd waveforms */
DFILTER *sd_avg = 0;		/* mean sd */
DFILTER *sd_se = 0;		/* std err of mean sd */
DFILTER **sd_lambda = 0;	/* individual gaussian widths */
DFILTER *tmbuf = 0;		/* temporary signal buffer */
int *sh_left = 0;		/* list of left-shift times */
float *sh_corr = 0;		/* correlation for this trial */
int msec = 0;			/* number of msec allocated */
int trials = 0;			/* number of trials allocated */

int sd_alloc(int n_msec, int n_trials)
{
	static int old_msec = 0;
	static int old_trials = 0;
	static int first = 1;

	register int i, len;

	msec = n_msec;
	trials = n_trials;
	if (first || (msec > old_msec) || (trials > old_trials)) {
		old_msec = n_msec * 1;		/* make extra */
		old_trials = n_trials;

		if (first) first = 0;
		else sd_free();
	}
	else {
		return(0);				/* already big enough */
	}

	/*
	 * allocate big enough to do fir filtering
	 */
	len = old_msec + (FILT_OFF * 2);

	if((sd_avg = (DFILTER *) dyn_alloc(len, sizeof(*sd_avg), "sd_alloc(): sd_avg")) == (char *)NULL) {
		return(-1);
	}
	else {
		memset(sd_avg, 0, (len * sizeof(*sd_avg)));
	}
	if((sd_se =  (DFILTER *) dyn_alloc(len, sizeof(*sd_se), "sd_alloc(): sd_se")) == (char *)NULL) {
		return(-1);
	}
	if((sh_left = (int *)   dyn_alloc(old_trials, sizeof(*sh_left), "sd_alloc(): sh_left")) == (char *)NULL) {
		return(-1);
	}
	if((sh_corr = (float *) dyn_alloc(old_trials, sizeof(*sh_corr), "sd_alloc(): sh_corr")) == (char *)NULL) {
		return(-1);
	}
	if((sd_ind =(DFILTER **) dyn_palloc(old_trials, sizeof(*sd_ind), len, sizeof(**sd_ind),
										"sd_alloc(): sd_ind")) == (char *)NULL) {
		return(-1);
	}

	/*
	 * offset pointers to beginning of data
	 */
	sd_avg = &sd_avg[FILT_OFF];
	sd_se =  &sd_se[FILT_OFF];
	for (i = 0; sd_ind[i] != NULL; i++) {
		sd_ind[i] = &sd_ind[i][FILT_OFF];
	}
}

int alloc_lambda(int n_trials, int *n_times)
{
	static int old_n_trials = 0;
	static int old_nmax = 0;
	int nmax;
	register int i, n;

	nmax = old_nmax;
	for (i = 0; i < n_trials; i++) {
		n = n_times[i];
		if (n > nmax) nmax = n;
	}

	if (sd_lambda && (n_trials <= old_n_trials) && (nmax <= old_nmax)) {
		return(0);
	}

	if (n_trials > old_n_trials) old_n_trials = n_trials;
	if (nmax > old_nmax) old_nmax = nmax;

	if (sd_lambda) dyn_pfree_(sd_lambda);	/* note:  use macro to zero pointer */

	if((sd_lambda = (DFILTER **)dyn_palloc(old_n_trials, sizeof(*sd_lambda), old_nmax, sizeof(**sd_lambda),
										   "sd_alloc(): sd_lambda")) == (char *)NULL) {
		return(-1);
	}

	return(0);
}

/* SD_FREE()
 *	user calls this function to force release of dynamically
 * allocated storage space
 */
void sd_free()
{
	register int i;

	/*
	 * normal pointers
	 */
	dyn_pfree_(sd_lambda);
	dyn_free_(sh_left);
	dyn_free_(sh_corr);

	/*
	 * offset pointers to beginning of buffers
	 */
	if (sd_avg) {
		sd_avg = &sd_avg[- FILT_OFF];
		dyn_free_(sd_avg);
	}
	if (sd_se) {
		sd_se = &sd_se[- FILT_OFF];
		dyn_free_(sd_se);
	}
	if (sd_ind) {
		for (i = 0; sd_ind[i] != NULL; i++) {
			sd_ind[i] = &sd_ind[i][- FILT_OFF];
		}
		dyn_pfree_(sd_ind);
	}
}

/* ADAP_K_EST.C
 * functions for performing adaptive kernel estimation
 *
 * NOTE:  buffer scheme allocates enough space to hold filtered data.
 *	Hence, the pointer to a buffer references the first valid data
 * point.  There is guaranteed to be FILT_OFF points ahead and behind
 * the buffer.  The number of data points is NMSEC.  Last time is MAX_TIME
 *
 * HISTORY:
 *	19jan88	LMO	derive from dcomp test program
 *	11dec91	LMO	try to improve efficiency via pixie:  sqrt_() and
 *			peak_corr()
 */
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <sys/types.h>
#include <malloc.h>
#include "neuron.h"
#include "diff.h"
#include "sd_est.h"
#include "mk_gauss.h"

#define DEBUG

#define sqrt_(x) ( (x) <= 0.0 ? 0.0 : sqrt(x) )

/*
 * private variables used by adap_k_est programs
 */
/*
 * pointers to signal buffers, offset to start of signal
 */
extern DFILTER **sd_ind;		/* individual sd waveforms */
extern DFILTER *sd_avg;			/* mean sd */
extern DFILTER *sd_se;			/* std err of mean sd */
extern DFILTER **sd_lambda;		/* individual gaussian widths */
extern DFILTER *tmbuf;			/* temporary signal buffer */
extern int *sh_left;			/* list of left-shift times */
extern float *sh_corr;			/* correlation for this trial */
extern int msec;			/* number of msec allocated */
extern int trials;			/* number of trials allocated */

/* ADAP_K_EST()
 *	spike density estimation control program
 *
 * INPUT ARGS:
 *	sp_times	= pointer to array of spike times
 *	sp_vals		= pointer to array of spike values
 *	n_trials	= number of trials in this class
 *	n_times		= number of times in sp_times in each trial
 *	n_msec		= number of msec in 
 *	h_width		= 0 ==> adaptive kernel
 *			< 0 ==> adaptive kernel with pilot width = -h_width
 *			> 0 ==> use as width for fixed kernel
 * OUTPUT:
 *	returns & SD_EST structure
 *	
 */
SD_EST *
adap_k_est(sp_times, sp_vals, n_trials, n_times, n_msec, h_width)
		int **sp_times;
		float **sp_vals;
		int n_trials, *n_times, n_msec;
		double h_width;
{
	int adapt;
	double hw, hlo, havg, hhi;
	static SD_EST sd;

	if (n_trials < 1) {
		fprintf(stderr, "adap_k_est(): n_trials = %d\n", n_trials);
		exit(1);
	}

/*
fprintf(stderr, "n_msec = %d, n_trials = %d\n", n_msec, n_trials);
*/
	sd_alloc(n_msec, n_trials);	/* get space */

	/*
	 * get fixed estimate of individual densities,
	 * either with caller's width,
	 * or default width for adaptive estimate
	 */
	if (h_width > 0.0) {
		hw = h_width;
		adapt = 0;
	}
	else if (h_width < 0.0) {
		hw = - h_width;
		adapt = 1;
	}
	else {
		hw = FIX_H;
		adapt = 1;
	}
	do_fixed(n_trials, sp_times, sp_vals, n_times, sd_ind, n_msec, hw);

	/*
	 * get average sd
     	 */
	avg_sd(n_trials, sd_avg, sd_se, sd_ind, n_msec);

	/*
	 * if adaptive, redo estimates
	 */
	if (adapt) {
		alloc_lambda(n_trials, n_times);
		get_lambda(sp_times, n_times, sd_avg, n_msec, hw, n_trials,
			sd_lambda, &hlo, &havg, &hhi, &sd);
		do_adapt(n_trials, sp_times, sp_vals, n_times, sd_ind, n_msec, sd_lambda);
		avg_sd(n_trials, sd_avg, sd_se, sd_ind, n_msec);	/* get avgs */
	}

	/*
	 * return pointers to waveform arrays
	 */
	sd.psd_avg = sd_avg;
	sd.psd_ind = sd_ind;
	sd.psd_se = sd_se;
	sd.h_lo = hlo;
	sd.h_avg = havg;
	sd.h_hi = hhi;

	return(&sd);
}

/* SD_KERNEL
 * spike density estimation functions ----------------
 */

/* DO_FIXED
 *	get efr average using fixed estimate of sigma
 *	and calculate mse
 */
do_fixed(nt, sp_times, sp_vals, n_times, sd_ind, msec, h_width)
	int nt, msec, *n_times;
	int **sp_times;
	float **sp_vals;
	double h_width;
	DFILTER **sd_ind;
{
	register int i;

	mk_gauss(h_width);
	for (i = 0; i < nt; i++) {
		do_fixed_efr(sp_times[i], sp_vals[i], n_times[i], sd_ind[i], msec);
	}
}

/* DO_ADAPT
 *	get efr average using adaptive estimate of sigma
 */
do_adapt(n_trials, sp_times, sp_vals, n_times, sd_ind, n_msec, lambda)
	int n_trials, n_msec, *n_times;
	int **sp_times;
	float **sp_vals;
	DFILTER **sd_ind, **lambda;
{
	register int i;

	for (i = 0; i < n_trials; i++) {
		do_adap_efr(sp_times[i], sp_vals[i], n_times[i], sd_ind[i], n_msec, lambda[i]);
	}
}

/*
 * make single ensemble firing rate from spike interval list
 */
do_fixed_efr(time, val, nt, efr, msec)
	register int *time;
	register float *val;
	register DFILTER *efr;
	int nt, msec;
{
	register DFILTER *gptr, *eptr;
	register DFILTER *gtop, *etop;
	register int filt_off;
	register int i;
	int max_time;

	max_time = msec + FILT_OFF - 1;
	filt_off = ngauss / 2;

	/* make sure the output buffer is empty */
	etop = &efr[max_time];
	for (eptr = efr; eptr < etop; *eptr++ = 0.0);

	gtop = &gauss[ngauss];

	for (i = 0; i < nt; i++, time++, val++) {
		if (*time < 0) continue;
		if (*time > max_time) break;
		eptr = &efr[*time - filt_off];
		for (gptr = gauss; gptr < gtop; ) {
			*eptr++ += *gptr++ * *val;
		}
	}
}

/*
 * make single ensemble firing rate from spike interval list
 */
do_adap_efr(time, val, nt, sd, msec, lambda)
	int *time, nt, msec;
	float *val;
	DFILTER *sd, *lambda;
{
	register DFILTER *gptr, *eptr;
	register DFILTER *gtop, *etop;
	register int filt_off;
	register int i, j;
	int max_time;

	max_time = msec + FILT_OFF;

	/* make sure the output buffer is empty */
	etop = &sd[max_time];
	for (eptr = &sd[-FILT_OFF]; eptr < etop; ) *eptr++ = 0.0;

	for (j = 0; j < nt; j++, time++, val++) {
		if (*time < 0) continue;
		mk_gauss((double) lambda[j]);
		filt_off = ngauss / 2;
		if ((*time + filt_off) >= max_time) break;
		eptr = &sd[*time - filt_off];
		for (gptr = gauss, i = 0; i < ngauss; i++) {
			*eptr++ += *gptr++ * *val;
		}
	}
}

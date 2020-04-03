/* sd_est.c
 * functions for performing spike-density estimation
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
 *	12jul94	LMO	check for nsigmas < 2 before doing histo
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

#undef DEBUG

#define sqrt_(x) ( (x) <= 0.0 ? 0.0 : sqrt(x) )

/*
 * private variables used by sd_est programs
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

static HISTO histo[HISTBINS];		/* histogram for save_histo */
static float shiftx[HISTBINS];
static float shifty[HISTBINS];
static float corrx[HISTBINS];
static float corry[HISTBINS];
static float sigmax[HISTBINS];
static float sigmay[HISTBINS];

/* SD_EST()
 *	spike density estimation control program
 *
 * INPUT ARGS:
 *	sp_times	= pointer to array of spike times
 *	n_trials	= number of trials in this class
 *	n_times		= number of times in sp_times in each trial
 *	n_msec		= number of msec in 
 *	h_width		= 0 ==> adaptive kernel
 *			< 0 ==> adaptive kernel with pilot width = -h_width
 *			> 0 ==> use as width for fixed kernel
 *	lag_comp	= controls lag compensation
 *	save_histo	= 1 for loading of shift & sigma & corr histograms 
 * OUTPUT:
 *	returns & SD_EST structure
 *	
 */
SD_EST *sd_est(int **sp_times, int n_trials, int *n_times, int n_msec, double h_width,
			   int lag_comp, int save_histo)
{
	int adapt;
	double hw, hlo, havg, hhi;
	static SD_EST sd;

	if (n_trials < 1) {
		rxerr("sd_est() warning: n_trials < 0");
		return((SD_EST *)NULL);
	}

	if(sd_alloc(n_msec, n_trials) == -1) {
		return((SD_EST *)NULL);
	}
		
	if(lag_comp) {	/* correct lags */
		if(align_data(sp_times, n_trials, n_times, n_msec, sd_avg, sd_ind, sh_left,
					  sh_corr, save_histo, &sd) == -1) {
			return((SD_EST *)NULL);
		}
	}

	/*
	 * get fixed estimate of individual spike densities,
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

	if(get_fixed(n_trials, sp_times, n_times, sd_ind, n_msec, hw) == -1) {
		return((SD_EST *)NULL);
	}

	/*
	 * get average sd
	 */
	avg_sd(n_trials, sd_avg, sd_se, sd_ind, n_msec);

	/*
	 * if adaptive, redo estimates
	 */
	if(adapt) {
		if(alloc_lambda(n_trials, n_times) == -1) {
			return((SD_EST *)NULL);
		}

		get_lambda(sp_times, n_times, sd_avg, n_msec, hw, n_trials,
				   sd_lambda, &hlo, &havg, &hhi, save_histo, &sd);

		get_adapt(n_trials, sp_times, n_times, sd_ind, n_msec, sd_lambda);

		avg_sd(n_trials, sd_avg, sd_se, sd_ind, n_msec);	/* get avgs */
	}

	/*
	 * return pointers to waveform arrays
	 */
	sd.psd_avg = sd_avg;
	sd.psd_ind = sd_ind;
	sd.psd_se = sd_se;
	sd.psh_left = sh_left;
	sd.psh_corr = sh_corr;
	sd.h_lo = hlo;
	sd.h_avg = havg;
	sd.h_hi = hhi;

	return(&sd);
}

/*
 * fast_sd_est()
 * This function builds a running spike density average, one trial at a time
 * It does not keep the individual spike density functions or compute the
 * standard error, shift histogram, or shift correlation. It does not do lag
 * compensation or use an adaptive kernel
 * As long as the n_msec argument is the same for each call, it will
 * accumulate the average. Calling fast_sd_est with sp_times, n_times, or
 * n_msec set to 0 will clear the accumulated average.
 *
 * INPUT ARGS:
 * sp_times			= pointer to array of spike times
 * n_trials			= the number of trials accumulated so far
 * n_times			= the number of times in the sp_times array
 * m_msec			= length of the interval in msec
 * h_width			= width of the gaussian kernel
 */

float *fast_sd_est(int *sp_times, int n_trials, int n_times, int n_msec, double h_width)
{
	double hw;

	if((sp_times == (long *)NULL) || (n_times == 0) || (n_msec == 0)) {
		sd_free();
		return((float *)NULL);
	}

	if(sd_alloc(n_msec, 1) == -1) {
		return((float *)NULL);
	}

	/*
	 * get fixed estimate of individual spike densities,
	 * either with caller's width,
	 * or default width for adaptive estimate
	 */
	if (h_width > 0.0) {
		hw = h_width;
	}
	else if (h_width < 0.0) {
		hw = -h_width;
	}
	else {
		hw = FIX_H;
	}

	if(!mk_gauss(hw)) {
		if(fixed_efr(sp_times, n_times, sd_ind[0], msec) == -1) {
			return((float *)NULL);
		}
	}
	else {
		return((float *)NULL);
	}

	// add the current trial into the average
	fast_avg_sd(n_trials, sd_avg, sd_ind[0], n_msec);

	return(sd_avg);
}

/* GET_SSD
 * get sample std dev
 */
double get_ssd(float *buf, int np)
{
	register int i;
	double x, x2, z;

	x = x2 = 0;
	for (i = 0; i < np; i++) {
		z = buf[i];
		x += z;
		x2 += z * z;
	}
	
	z = ( x2 - ((x * x) / np) ) / (np-1);
	z = sqrt_( z );

	return(z);
}		

int conv_data(int n, int **sp_times, int *n_times, DFILTER **sd_ind, int msec, double h)
{
	register int i;

	if(!mk_gauss(h)) {
		for (i = 0; i < n; i++) {
			fixed_efr(sp_times[i], n_times[i], sd_ind[i], msec);
		}
		return(0);
	}
	else {
		return(-1);
	}
}

/* AVG_SD()
 * get the average sd waveform.  remember that
 * buffers have guard bands of FILT_OFF time
 */
void avg_sd(int nt, DFILTER *sd_avg, DFILTER *sd_se, DFILTER **sd_ind, int msec)
{
	register DFILTER *p;
	register int j, i, n;
	double z;

	n = msec + FILT_OFF;
	for (p = &sd_avg[-FILT_OFF], i = -FILT_OFF; i < n; i++) {
		*p++ = 0; 
		sd_se[i] = 0;
	}

	for (i = 0; i < nt; i++) {
		for (j = -FILT_OFF, p = &sd_ind[i][j]; j < n; j++){
			sd_se[j] += *p * *p;
			sd_avg[j] += *p++;
		}
	}
	if(nt > 1) {
		for (p = &sd_avg[-FILT_OFF], i = -FILT_OFF; i < n; i++) {
			z = (sd_se[i] - ((*p * *p) / nt)) / (nt * (nt - 1));
			sd_se[i] = sqrt_(z);
			*p++ /= nt;
		}
	}
	else {
		for(p = &sd_avg[-FILT_OFF], i = -FILT_OFF; i < n; i++) {
			sd_se[i] = 0.0;
		}
	}
}

/* fast_avg_sd()
 * get a running average, one trial at a time
 *
 */
void fast_avg_sd(int nt, DFILTER *sd_avg, DFILTER *sd_ind, int msec)
{
	DFILTER *p;
	int n;
	int i;
	int j;

	n = msec + FILT_OFF;

	for(j = -FILT_OFF, p = &sd_ind[j]; j < n; j++) {
		sd_avg[j] = (sd_avg[j] * nt) + *p++;
		sd_avg[j] /= (nt + 1);
	}
}

/* SD_ALIGN
 * spike density alignment functions ----------------------
 */

/* ALIGN_DATA
 *	1. convert data times to discrete waveform
 *	2. make average data set
 *	3. correlate each spike train with average
 *	4. offset data to lag of maximum correlation
 */
int align_data(int **sp_times, int n_trials, int *n_times, int n_msec, float *sd_avg, float **sd_ind,
			   int *sh_left, int *sh_corr, int save_histo, SD_EST *psd)
{
	static int ltmbuf = 0;

	register int i;
	int len;

	len = FILT_MAX + n_trials;
	if(len > ltmbuf) {
		if(tmbuf) {
			tmbuf = &tmbuf[- FILT_OFF];
			dyn_free_(tmbuf);
		}
		
		if((tmbuf = (DFILTER *) dyn_alloc(len, sizeof(*tmbuf), "sd_alloc(): tmbuf")) == (DFILTER *)NULL) {
			return(-1);
		}

		tmbuf = &tmbuf[FILT_OFF];
	}

	/*
	 * reset shift times
	 */
	for (i = 0; i < n_trials; i++) sh_left[i] = 0;

	/*
	 * convert times to individual waveforms
	 */
	if(conv_data(n_trials, sp_times, n_times, sd_ind, n_msec, CORR_H) == -1) {
		return(-1);
	}

	/*
	 * get average waveform, then normalize it to have unit power
	 */
	avg_sd(n_trials, sd_avg, sd_se, sd_ind, n_msec);
	norm_pow(sd_avg, n_msec);

	/*
	 * do correlation to determine shift
	 */
	do_corr(n_trials, sd_avg, sd_ind, n_msec, sh_left, sh_corr);

	/*
	 * do the shift
	 */
	do_lag(n_trials, sp_times, n_times, sh_left);

	if (save_histo) {
		for (i = 0; i < n_trials; i++) tmbuf[i] = sh_left[i];
		sv_histo(tmbuf, n_trials, shiftx, shifty);
		psd->psd_shifthx = shiftx;		
		psd->psd_shifthy = shifty;
		psd->psd_shifthn = HISTBINS;

		for (i = 0; i < n_trials; i++) tmbuf[i] = sh_corr[i];
		sv_histo(tmbuf, n_trials, corrx, corry);
		psd->psd_corrhx = corrx;		
		psd->psd_corrhy = corry;
		psd->psd_corrhn = HISTBINS;
	}
	else {
		psd->psd_shifthn = 0;
		psd->psd_corrhn = 0;
	}

	return(0);
}

/* DO_LAG()
 * actually align the data set by
 * correcting the times
 */
void do_lag(int nt, int **sp_times, int *n_times, int *sh_left)
{
	register int *time;
	register int j, tau, i, n;

	for (i = 0; i < nt; i++) {
		n = n_times[i];
		time = sp_times[i];
		tau = sh_left[i];
		for (j = 0; j < n; j++) *time++ -= tau;
	}
}

/*
 * normalize power in signal to 1.0
 */
void norm_pow(DFILTER *p, int n)
{
	register int j;
	double mn, sum2, z;

	n += FILT_OFF;	/* correct for buffer ends */
	for (mn = 0, j = -FILT_OFF; j < n; j++) mn += p[j];
	mn /= n;

	for (sum2 = 0, j = -FILT_OFF; j < n; j++) {
		p[j] -= mn;
		z = p[j];
		sum2 += sqr_(z);
	}
	z = sqrt_(sum2);

	if (z != 0.0) for (j = -FILT_OFF; j < n; j++) {
		p[j] /= z;
	}
}

/*
 * correlate individual to average waveforms
 */
void do_corr(int n, DFILTER *sd_avg, DFILTER **sd_ind, int msec, int *sh_left, float *sh_corr)
{
	register int i;

	for (i = 0; i < n; i++) {	/* for each trial */
		sh_left[i] =
			peak_corr(CORR_LO, CORR_HI, sd_avg, msec,
				sd_ind[i], msec, &sh_corr[i]);
	}
}

/* PEAK_CORR
 * returns the index by which y must be shifted left to have the
 * peak correlation with tmplt.
 */
int peak_corr(int lo, int hi, DFILTER *tmplt, int ntmplt, DFILTER *y, int ny, float *corcoef)
{
	register DFILTER *yi;
	register int i, k, tau;
	double corr, best, z, mn;

	tau = 0;
	best = 0;
	for (i = lo; i <= hi; i++) {
		yi = &y[i];

		mn = 0;
		for (k = 0; k < ntmplt; k++) mn += yi[k];
		mn /= ntmplt;

		corr = 0;
		for (k = 0; k < ntmplt; k++) {
			z = yi[k] - mn;
			corr += z * tmplt[k];
		}

		if (corr> best) {
			best = corr;
			tau = i;
		}
	}
	*corcoef = best;

	return(tau);
}

void shift_left(int tau, DFILTER *p, int msec)
{
	register int i, n;
	register DFILTER *q;

	if (tau == 0) return;

	if (tau > 0) {		/* shift left */
		n = msec + FILT_OFF - tau;
		p = &p[-FILT_OFF];	/* set to beginning */
		q = &p[tau];
		for (i = -FILT_OFF; i < n; i++) *p++ = *q++;
		for (i = 0; i < tau; i++) *p++ = 0;		/* fill in tail */
	}
	else {			/* shift right */
		n = msec + FILT_OFF;
		p = &p[n - 1];	/* set to end */
		q = &p[tau];
		for (i = -(FILT_OFF + tau); i < n; i++) *p-- = *q--;
		tau = -tau;
		for (i = 0; i < tau; i++) *p-- = 0;		/* fill in head */
		
	}
}

/* SD_KERNEL
 * spike density estimation functions ----------------
 */

/* GET_FIXED
 *	get efr average using fixed estimate of sigma
 *	and calculate mse
 */
int get_fixed(int nt, int **sp_times, int *n_times, DFILTER **sd_ind, int msec, double h_width)
{
	register int i;

	if(!mk_gauss(h_width)) {
		for (i = 0; i < nt; i++) {
			fixed_efr(sp_times[i], n_times[i], sd_ind[i], msec);
		}

		return(0);
	}
	else {
		return(-1);
	}
}

/* GET_ADAPT
 *	get efr average using adaptive estimate of sigma
 */
void get_adapt(int n_trials, int **sp_times, int *n_times, DFILTER **sd_ind, int n_msec, DFILTER **lambda)
{
	register int i;

	for (i = 0; i < n_trials; i++) {
		adap_efr(sp_times[i], n_times[i], sd_ind[i], n_msec, lambda[i]);
	}
}

/* GET_LAMBDA
 *	define local bandwidth factor, lambda, at each data point
 *  according to Silverman, eq. 5.7.
 *	spt = pointer array of spike times
 *	ntime = number of times
 *	yfix = buffer of floats holding current estimate
 *	nt = number of trials
 */
void get_lambda(int **spt, int *ntime, DFILTER *yfix, int msec, double h_width, int nt, DFILTER **lambda,
				double *plo, double *pavg, double *phi, int save_histo, SD_EST *psd)
{
	register int i, j;
	register int *time;
	float *pl;
	int n, np, max_time;
	double g, sum;
	double alpha = ALPHA;
	double z;
	double h_lo, h_avg, h_hi;

	max_time = msec + FILT_OFF - 1;

	/* first, get geometric mean */
	sum = 0;
	for(i = np = 0; i < nt; i++) {
		time = spt[i];
		n = ntime[i];
		for (j =  0; j < n; j++, time++) {
			if (*time < 0) continue;
			if (*time >= max_time) break;
			z = yfix[*time];
			if (z < EPSILON_LAMBDA) z = EPSILON_LAMBDA;
			sum += log(z);
			np++;
		}
	}
	g = exp(sum / (double) np);

	/* determine local bandwidth factors, store with h_width factored in */
	h_lo = 1e6;
	h_hi = 0;
	h_avg = 0;
	for(i = 0; i < nt; i++) {
		time = spt[i];
		n = ntime[i];
#ifdef DEBUG
		fprintf(stderr, "get_lambda():  lambda = %X nt = %d, i = %d\n", lambda, nt, i);
#endif
		pl = lambda[i];
		for (j = 0; j < n; j++, time++, pl++) {
			if (*time < 0) continue;
			if (*time >= max_time) break;
			*pl = h_width * pow(( yfix[*time] / g), -alpha);
			if (*pl < h_lo) h_lo = *pl;
			else if (*pl > h_hi) h_hi = *pl;
			h_avg += *pl;
		}
	}
	h_avg /= np;

#ifdef DEBUG
	for(i = 0; i < 10; i++) {
		pl = lambda[i];
		for (j = 0; j < 2; j++, pl++) fprintf(stderr, "lambda[%d][%d] = %g\n", i, j, *pl);
	}
#endif
	*plo = h_lo;
	*pavg = h_avg;
	*phi = h_hi;

	if(save_histo) {
		int nsigmas;
		DFILTER *sigbuf;

		for (i = 0, nsigmas = 0; i < nt; i++) nsigmas += ntime[i];

		if (nsigmas > 1) {
			sigbuf = (DFILTER *) dyn_alloc(nsigmas, sizeof(*sigbuf), "get_lambda(): sigbuf");

			for (i = nsigmas = 0; i < nt; i++) {
				for (j = 0; j < ntime[i]; j++) {
					sigbuf[nsigmas++] = lambda[i][j];
				}
			}
			sv_histo(sigbuf, nsigmas, sigmax, sigmay);
			psd->psd_sigmahx = sigmax;		
			psd->psd_sigmahy = sigmay;
			psd->psd_sigmahn = HISTBINS;
			dyn_free(sigbuf);	/* note, does not alter buffer pointer */
		}
	}
}

void sv_histo(float *buf, int n, float *px, float *py)
{
	register int i;

	bld_hist(buf, n, histo, HISTBINS);
	for (i = 0; i < HISTBINS; i++) {
		*px++ = histo[i].h_x;
		*py++ = histo[i].h_y;	
	}
}

void bld_hist(float *buf, int nb, struct histo *hist, int nh)
{
	register int i, j;
	float lo, hi, z, step, hstep;
	double sum;

	lo = hi = buf[0];
	for (i = 1; i < nb; i++) {
		z = buf[i];
		if (z < lo) lo = z;
		else if (z > hi) hi = z;
	}
	step = (hi - lo) / (nh - 1);
	hstep = step/2;

	for (z = lo, i = 0; i < nh; i++) {
		hist[i].h_x = z;
		hist[i].h_y = 0;
		z += step;
	}

	for (i = 0; i < nb; i++) {
		z = buf[i];
		j = (z - lo + hstep) / step;
		hist[j].h_y++;
	}
	for (i = sum = 0; i < nh; i++) sum += hist[i].h_y;
	for (i = 0; i < nh; i++) hist[i].h_y /= sum;
}

/*
 * make single ensemble firing rate from spike interval list
 */
void fixed_efr(int *time, int nt, DFILTER *efr, int msec)
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

	for (i = 0; i < nt; i++, time++) {
		if (*time < 0) continue;
		if (*time > max_time) break;
		eptr = &efr[*time - filt_off];
		for (gptr = gauss; gptr < gtop; ) {
			*eptr++ += *gptr++;
		}
	}
}

/*
 * make single ensemble firing rate from spike interval list
 */
int adap_efr(int *time, int nt, DFILTER *sd, int msec, DFILTER *lambda)
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

	for (j = 0; j < nt; j++, time++) {
		if(*time < 0) {
			continue;
		}
		if(!mk_gauss((double) lambda[j])) {
			filt_off = ngauss / 2;
			if ((*time + filt_off) >= max_time) {
				break;
			}
			eptr = &sd[*time - filt_off];
			for (gptr = gauss, i = 0; i < ngauss; i++) {
				*eptr++ += *gptr++;
			}
		}
		else {
			return(-1);
		}
	}
	return(0);
}

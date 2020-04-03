#include <stdio.h>
#include <math.h>
#include "mk_gauss.h"

/*
 * public vars
 */
int ngauss = 0;
DFILTER *gauss = 0;


/*
 * make a gaussian suitable for convolution
 * with delta functions.  Note that sigma is a function of the
 * number of trials
 */
int mk_gauss(double width)
{
	register int i, mid;
	register DFILTER *pg;
	double x, z, sigma;
	int no_sig;
	static double old_width = 0;
	static int first = 1;

	if(width == old_width) {
		return(0);
	}
	else {
		old_width = width;
		sigma = width;

		for(no_sig = 6; no_sig < 12; no_sig++) {
			if ((no_sig * sigma + 1) > 36) {
				break;
			}
		}

		ngauss = no_sig * sigma + 1;
		if (ngauss > FILT_MAX) ngauss = FILT_MAX;
		if ((ngauss % 2) == 0) --ngauss;
		mid = ngauss / 2;

		if(first) {
			first = 0;
			i = FILT_MAX * sizeof(*gauss);
			if(gauss) free(gauss);
			gauss = (DFILTER *) malloc(i);
			if(gauss == NULL) {
				rxerr("failure to realloc gaussian");
				return(-1);
			}
		}

		/*
		 * create filter.  Remember, sigma is in milliseconds
		 */
		z = sigma * sqrt(TWOPI);
		pg = &gauss[mid];
		for (i = 0; i <= mid; i++) {
			x = (DFILTER) i  / sigma;
			x *= x;
			x /= -2;
			pg[i] = pg[-i] = exp(x) / z;
		}
		/*
		 * normalize gaussian pulse to have  area = 1000.0.
		 */
		for (z = 0, i = 0; i < ngauss; i++) {
			z += gauss[i];
		}
		z = 1000 / z;
		for (i = 0; i < ngauss; i++) {
			gauss[i] *= z;
		}
	}

	return(0);
}

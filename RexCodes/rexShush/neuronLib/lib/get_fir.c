#include <stdio.h>
#include <math.h>
#include "diff.h"

#define	PI	3.14159265358979323846264338
#define twopi	(2 * PI)
#define pi	(PI)


/* GET_FIR
 *	read in a standard format FIR filter.  Allocate space for
 * it, and scale it.  Fill in DIFFER table with num and pointer
 *	If scale is zero, a scale of 1 is used.
 *	Multiply by 2pi to convert units
 */
get_fir(pf)
	register DIFFER *pf;
{
	register int i;
	double h[68];
	int nf;
	int nfilt;
	int neg;
	register double *f;
	int fd;
	double scale;

	if ((fd = open(pf->dname, 0)) == -1) {
		fprintf(stderr,"Can't open filter file (%s)\n", pf->dname);
		exit(-1);
	}
	if ((nf = read(fd,h,sizeof(h))) < sizeof(h)) {
		fprintf(stderr,"Read on filter file %s returns %d\n",
			pf->dname, nf);
		exit(-1);
	}

	nf = h[66];	/* size is stored in array */
	nfilt = h[67];	/* number of points in filter */
	if (nfilt < 0) {
		nfilt = -nfilt;
		neg = 1;
	}
	else neg = 0;
	f = (double *) malloc(nf * sizeof(*f));
	if (f == 0) {
		fprintf(stderr,"Can't malloc %d spaces for %s\n",
			nf, pf->dname);
		exit(-1);
	}

	if (pf->dscale == 0) scale = (neg ? twopi : 1);
	else scale = pf->dscale * (neg ? twopi : 1);

	for (i = 0; i < nf; i++) f[i] = scale * h[i];

	pf->dnum = nf;
	pf->dlen = nfilt;
	pf->dneg = neg;
	pf->dptr = f;
}

#include "diff.h"

/*
 * FILTER
 *	N point Finite Impulse Filter
 *
 * INPUT:
 *	x = pointer to buffer of type DFILTER
 *	xl = pointer to one past data
 *	xout = pointer to buffer to hold output
 *	pxoutl = pointer to pointer to one beyond output.
 *		*PXOUTL MUST BE SET BY USER TO POINT TO MAX AVAILABLE SPACE,
 *		i.e., *pxoutl = &xout[MAXSIZE]
 *	filt = pointer to filter structure
 *		This filter is in the order put out by the FIR program,
 *		so that f[0] = the most distant negative time,
 *		and f[n] = time zero
 * OUTPUT:
 *	xout[] is filled with the calculated data
 *		at the beginning and end, where the filter output
 *		is undefined, the raw data is used.
 *	pxoutl is set to point to last calculated data
 *
 * REV
 *	30jul87	LMO	correct count for even low-pass filters
 *	29jul92	LMO	fix buffer ends
 */
filter(x, xl, xout, pxoutl, filt)
	register DFILTER *x;
	DFILTER *xl;
	DFILTER *xout;
	DFILTER **pxoutl;
	DIFFER *filt;
{
	register int i;
	register double *h;
	static double z;
	DFILTER w;
	int num, odd;

	num = filt->dnum;
	odd = (filt->dlen & 01 ? 1 : 0);

	if ((xl - x) > (*pxoutl - xout)) xl = x + (*pxoutl - xout); /* get last x */

	fix_edges(x, xl, num);

	/*
	 * compute by convolving with filter
	 */
	for (; x < xl ; x++, xout++) {
		/* at each point compute sum */
		h = &filt->dptr[num-1];
		if (odd) {
			z = *h-- * x[0];
			for (i = 1; i < num; i++, h--)
				z += *h * (x[i] + x[-i]);
		}
		else {
			z = 0;
			for (i = 0; i < num; h--) {
				w = x[-i];
				w += x[++i];
				z += *h * w;
			}
		}
		*xout = z;
	}

	*pxoutl = xout;
}

/*
 * DIFF
 *	2N + 1 point differentiator
 *
 * INPUT:
 *	x = pointer to buffer of type DFILTER
 *	xl = pointer to one past data
 *	xdot = pointer to buffer to hold output
 *	pxdotl = pointer to pointer to one beyond output.
 *		*PXDOTL MUST BE SET BY USER TO POINT TO MAX AVAILABLE SPACE,
 *		i.e., *pxdotl = &xdot[MAXSIZE]
 *	filter = pointer to filter structure
 *		This filter is in the order put out by the FIR program,
 *		so that f[0] = the most distant negative time,
 *		and f[n] = time zero
 * OUTPUT:
 *	xdot[] is filled with the calculated derivative
 *	pxdotl is set to point to last calculated derivative
 *
 * REV:
 *	30jul87	LMO	eliminate 0 pass in differentiator loop
 */
diff(x, xl, xdot, pxdotl, filter)
	register DFILTER *x;
	DFILTER *xl;
	DFILTER *xdot;
	DFILTER **pxdotl;
	DIFFER *filter;
{
	register int i;
	register double *h;
	static double z;
	int num;

	num = filter->dnum;

	if ((xl - x) > (*pxdotl - xdot)) xl = x + (*pxdotl - xdot); /* get last x */


	fix_edges(x, xl, num);

	/*
	 * compute derivative by convolving with filter
	 */
	for (; x < xl ; x++, xdot++) {
		/* at each point compute sum */
		for (z = 0, i = 1, h = &filter->dptr[num-1];
			i <= num; i++, h--) z += *h * (x[i] - x[-i]);
		*xdot = z;
	}

	*pxdotl = xdot;	/* set up pointer to last */
}


/* FIX_EDGES
 * To avoid edge effects, use Shure's method of reflecting
 * data back at edges before fir filtering
 */
fix_edges(x, xl, num)
	DFILTER *x, *xl;
	int num;
{
	int i;
	double z;

#ifdef SHURE
	for (i = 1, z = 2 * x[0];   i < num; i++) x[-i] = z - x[i];
	for (i = 0, z = 2 * xl[-1]; i < num; i++) xl[i] = z - xl[-i-1];
#else
	for (i = 1; i < num; i++) x[-i] = x[0];		/* fix beginning */
	for (i = 0; i < num; i++) xl[i] = xl[-1];	/* fix ending */
#endif
}


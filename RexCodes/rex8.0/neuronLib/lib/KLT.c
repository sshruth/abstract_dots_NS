#include "matrix.h"

/* KLT
 * carry out KLT transform.  First, subtract mean, then
 * apply transition matrix.
 * transformed value is returned in place of the input value
 */
KLT(T, dat, pc, zbar, dim)
	MATRIX *T;
	VECTOR *dat;
	register VECTOR *zbar, *pc;
	int dim;
{
	register VECTOR *p, *d;
	register int n;

	p = pc;
	d = dat;
	for (n = 0; n < dim; n++) *p++ = *d++ - *zbar++;
	vecmpy(T, pc, dat, dim, dim);
}

/* iKLT
 * carry out inverse KLT transform.
 * First, apply transition matrix, then add mean
 */
iKLT(Tp, dat, pc, zbar, dim)
	MATRIX *Tp;
	VECTOR *dat, *pc;
	register VECTOR *zbar;
	int dim;
{
	register int n;
	register VECTOR *d, *p;

	d = dat;
	p = pc;
	vecmpy(Tp, d, p, dim, dim);
	for (n = 0; n < dim; n++) *d++ = *p++ + *zbar++;
}

/* iKLT_delta
 * carry out inverse KLT transform as relative offset, i.e.,
 * don't correct by offset zbar.
 */
iKLT_delta(Tp, dat, pc, dim)
	MATRIX *Tp;
	VECTOR *dat, *pc;
	int dim;
{
	register int n;
	register VECTOR *d, *p;

	d = dat;
	p = pc;
	vecmpy(Tp, d, p, dim, dim);
	for (n = 0; n < dim; n++) *d++ = *p++;
}

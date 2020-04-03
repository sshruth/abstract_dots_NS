#include <stdio.h>
#include "matrix.h"

/* GET_KLTRANS
 *	Get Karhunen-Loeve Transform
 *
 * The KL transform is an orthogonal rotation that maps a data set {x} into a
 * transform set {y} such that y's covariance matrix is diagonal:
 *	y = T  * x	(forward transform)(usage comment corrected 28nov90)
 *	x = Tp * y	(inverse transform)(usage comment corrected 28nov90)
 *
 * INPUT:
 * sigma = data domain covariance matrix on input.
 * dim = dimension, i.e., sigma[dim][dim].
 * OUTPUT:
 * sigma = diagonal is eigenvalues
 * Tp = columns are eigenvectors of sigma
 * T = rows are eigenvectors of sigma
 */
get_KLtrans(sigma, T, Tp, dim)
	int dim;
	MATRIX *sigma, *T, *Tp;
{
	if (symmQR2(dim, sigma, Tp) < 0) {
		fprintf(stderr,"symmQR2 fails\n");
		exit(1);
	}
	mattran(Tp, T, dim, dim);	/* get transpose */
}


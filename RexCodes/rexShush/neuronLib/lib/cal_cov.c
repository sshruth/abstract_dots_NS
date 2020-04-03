#include "matrix.h"

/* CAL_COV
 *	Calculate the data domain covariance matrix:
 * sigma(Z) = E{ZZ'} - ZbarZbar'
 * where Zbar = E{Z} is the average value of Z.
 *
 *	INPUT:
 * Z = input data vector
 * n = length of Z
 * flag = 0 --> begin
 *      > 0 --> accumulate ZZ' and Zbar
 *      < 0 --> calculate sigma(Z) and Zbar
 *
 *	OUTPUT:
 * sigma = n x n output matrix
 * Zbar = n x 1 output vector
 * na = pointer to number of Z's accumulated
 *
 *	RETURN:
 * 1 = success
 * -1 = failure
 *
 *	USAGE:
 * The first time, call cal_cov() with flag = 0.
 * Then call cal_cov() with flag > 0 until all Z's are used.
 * Finally, call cal_cov() with flag < 0 to perform final calculations.
 *
 * CHANGES:
 * 4-1-87	LMO	Change covariance measure to divide by N-1
 *			to correct variance estimate for small samples.
 *			This is the method used by BMDP, and appears to
 *			be necessary when cal_cov() is used by scatter3D
 *			program.  Note that for large N, this is a change
 *			of no consequence.
 *			Note: var = [sum(x^2) - (sum(x)^2 / n)] / (n-1)
 */

cal_cov(Z, n, sigma, Zbar, na, flag)
	VECTOR *Z, *Zbar;
	MATRIX *sigma;
	int n, *na, flag;
{
	register int r, c, k;
	register MATRIX *pr;
	double Zr, num, nm1;

	if (flag < 0) {
		if ((num = *na) < 2) return(-1);
		nm1 = num - 1;
		/*
		 * get sample variance, but remember matrix
		 * is symmetric, so just do lower triangle first
		 */
		for (pr = sigma, r = 0; r < n; r++, pr += n) {
			Zr = Zbar[r] / num;
			for (c = 0; c <= r; c++) {
				pr[c] -= Zr * Zbar[c];
				pr[c] /= nm1;
			}
		} /* next row */
		/*
		 * fill in upper triangle of symmetric matrix
		 * and get mean response
		 */
		for (pr = sigma, r = 0, k = n; r < n; r++, pr += n) {
			k = r + (r + 1) * n;
			for (c = (r + 1); c < n; c++, k += n) {
				pr[c] = sigma[k];
			}
			Zbar[r] /= num;
		}
		return(1);
	}

	if (flag == 0) {
		for (pr = sigma, c = n * n, r = 0; r < c; r++) *pr++ = 0;
		*na = 0;
		for (r = 0; r < n; r++) Zbar[r] = 0;
	}

	/*
	 * fill in only lower triangle
	 */
	(*na)++;
	for (pr = sigma, r = 0; r < n; r++, pr += n) {
		Zr = Z[r];
		Zbar[r] += Zr;
		for (c = 0; c <= r; c++) pr[c] += Zr * Z[c];
	}
	return(1);
}

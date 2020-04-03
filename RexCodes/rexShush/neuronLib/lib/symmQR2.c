#include <math.h>
#ifdef sgi
#include <malloc.h>
#endif
#include "matrix.h"

/*
 *   See comments at beginning of symmQR2() concerning use, references,
 * and open questions.
 *
 *	HISTORY:
 *	19mar90	LMO	change to double precision for g[][]
 */

/*
 *   The foll'g macro delivers x[p][q], assuming that x's
 * kth row ( k = 1,2,... ) has indices k,1 k,2 ... k,n.
 */
#define X1(p,q) *(x+(p-1)*n + q-1)

/*
 *   The foll'g macro delivers g[p][q], assuming that g's
 * kth row ( k = 1,2,... ) has indices k,1 k,2 ... k,n.
 */
#define G1(p,q) (*(g+(p-1)*n + q-1) )

#define max(a,b) ( (a) > (b) ? (a) : (b) )

/*  Call is symmQR( n, &g[0][0], &s[0][0] ),
 * where MATRIX g[n][n] contains lower 
 * triangle ( including diagonal ) of a real symmetric matrix.  The computed
 * eigenvalues returned in g[0][0], g[1][1],..., g[n-1][n-1], and the corresp.
 * eigenvectors are returned in the corresponding columns of MATRIX x[n][n].
 * The entry contents of the lower triangle of g[][] are lost; the strict
 * upper triangle of g[][] is not changed by this function.
 *
 *   Returns 1 if all goes well, -1 if malloc() trouble occurs.
 *
 *  This is ALGORITHM 254 from _COLLECTED ALGORITHMS FROM CACM_, adapted
 * to the C language from the Algol in which it is published.  The change
 * described in "CERTIFICATION OF ALGORITHM 254", which immediately follows
 * ALGORITHM 254, is incorporated in this file.* ALGORITHM 254.
 */

/*   RELPREC is used only in setting eps after call to Householder().
 *   Lance used 1e-8 in a run made before I changed several "abs()"
 * calls to "fabs()" calls and coerced fabs()'s arg everywhere needed.
 *   I have changed it to 1e-8 ( .5^23 ) because 1e-8 < .5^26,
 * and .5^23 or .5^24
 * seems appropriate, as 1.5e-11 was used in the publised algorithm and was
 * said to be appropriate for a 36-bit mantissa ( .5^36 = 1.45519e-11 ).
 */
#if MATRIX_IS_DOUBLE
#undef SINGLE_PRECISION
#else
#define SINGLE_PRECISION
#endif

#ifdef SINGLE_PRECISION
#define RELPREC 1.19e-7 /* 1 / 2^23 */
#else
/*#define RELPREC 1.0e-12 */
#define RELPREC 1.0e-14
#endif

symmQR2( n, g, x )
	int n;
	MATRIX *g, *x;
{
	int i, j, k, m, m1, jobdone;
	double t, norm, eps, sine, cosine, lambda, mu, a0, a1, b0, beta,
		x0, x1;
	/*
	 * double a[1:n], b[0:n], c[0:n-1], cs[1:n-1], sn[1:n-1]
	 */
	double *a, *b, *c, *cs, *sn;

	a = (double *)  calloc(n+1, sizeof(*a));
	b = (double *)  calloc(n+1, sizeof(*b));
	c = (double *)  calloc(n,   sizeof(*c));
	cs = (double *) calloc(n,   sizeof(*cs));
	sn = (double *) calloc(n,   sizeof(*sn));
	jobdone = 1;
	if( a==0 || b==0 || c==0 || cs==0 || sn==0 ){
		jobdone = -1; goto final;
	}

	/*
	 *  Set x[][] to the n x n identity matrix.
	 */
	for( i=1; i<=n; i++)
		for( j=1; j<=n; j++)
			X1(i,j) = ( i==j ? 1. : 0.);

	if(Householder( n, g, a, b, x, &norm ) < 0){jobdone= -1; goto final;};
	eps = norm * RELPREC;
	/*
	 *  Tolerance is set to product of the infinity norm of the re-
	 * duced matrix and the relative machine precision.
	 *   RELPREC is defined and commented upon just above the body
	 * of this function.
	 */

	b[0] = mu = 0;
	m = n;

	while( m != 0 ){
		i = k = m1 = m-1;
		if( -eps <= b[k] && b[k] <= eps ){
			G1(m,m) = a[m];
			m = k;
		}
		else{
			for( i--; fabs( (double)b[i] ) > eps; i--) k = i;
			/* find eigenvalues of lower 2 x 2 */
			b0 = b[m1] * b[m1];
			a1 = a[m1] - a[m];
			a1 = sqrt( a1*a1 + 4. * b0);
			t = a[m1] * a[m] - b0;
			a0 = a[m1] + a[m];
			lambda = .5 * ( a0 >= 0. ? a0+a1 : a0-a1 );
			t = t / lambda; /* compute the shift */
			if( fabs(t - mu) < .5 * fabs((double)t) )
				mu = lambda = t;
			else if( fabs(lambda - mu) < .5 * fabs( (double)lambda) )
				mu = lambda;
			else{
				mu = t;
				lambda = 0.;
			}
			a[k] = a[k] - lambda;
			beta = b[k];
			for( j = k; j <= m1; j++){ /* transf. on left */
				a0 = a[j]; a1 = a[j+1] - lambda; b0 = b[j];
				t = sqrt( a0*a0 + beta*beta );
				cosine = cs[j] = a0 / t;
				sine = sn[j] = beta / t;
				a[j] = cosine * a0 + sine * beta;
				a[j+1] = - sine * b0 + cosine * a1;
				b[j] = cosine * b0 + sine * a1;
				beta = b[ j+1 ];
				b[j+1] = cosine * beta;
				c[j] = sine * beta;
			} /* end j */
			b[ k-1 ] = c[ k-1 ] = 0.;
			for( j=k; j <=m1; j++ ){ /* transf. on right */
				sine = sn[j]; cosine = cs[j];
				a0 = a[j]; b0 = b[j];
				b[ j-1 ] = b[ j-1 ] * cosine + c[j-1] * sine;
				a[j] = a0 * cosine + b0 * sine + lambda;
				b[j] = - a0 * sine + b0 * cosine;
				a[j+1] *= cosine;
				for( i=1; i<=n; i++){
					x0 = X1(i,j);
					x1 = X1(i,j+1);
					X1(i,j) = x0 * cosine + x1 * sine;
					X1(i,j+1) = -x0 * sine + x1 * cosine;
				}
			} /* end j */
			a[m] = a[m] + lambda;
		}
	}

	final: /* jump here iff a malloc() fails at start of this function
		* or if Householder() fails.
		*/
	if(a != 0 )free(a);
	if(b != 0 )free(b);
	if(c != 0 )free(c);
	if(cs != 0 )free(cs);
	if(sn != 0 )free(sn);
	return(jobdone);
}



/*  Call: Householder( n, &g[0][0], &a[0], &b[0], &x[0][0], &norm );
 *
 *   Returns -1 if malloc() trouble, 1 otherwise.
 *
 * Reduces the real symm. n x n matrix defined by the lower triangle of
 * g[][] to tridiagonal form using n-2 elementary orthogonal transfor-
 * mations ( I - 2ww' ) = ( I - gamma uu' ).  Only the lower triangular
 * part of g[][] need be given.  The computed diagonal and subdiagonal
 * elts of the reduced matrix are stored in double a[0:n-1], b[0:n-2]
 * respectively.  The transformations on the right are also applied to the
 * n x n matrix x[][].  The cols of the strict lower triangle of g[][]
 * are replaced by the non-zero portion of the vectors u.  norm is set
 * to the infinity norm of the reduced matrix.
 *
 */

Householder( n, g, a, b, x, norm )
	int n;
	MATRIX *g, *x;
	double *a, *b, *norm;
{
	int i, j, k;
	double t, sigma, alpha, beta, gamma, absb, sum;
	double *p;	/* double p[2:n]; */

	if( !( p = (double *) calloc(n+1, sizeof *p)) ) return(-1);

	for(i=1; i<=n; i++)
		for(j=1; j<=n; j++)
	*norm = absb = 0.;
	for( k=1; k<= n-2; k++){
		a[k] = G1(k, k);
		sigma = 0.;
		for( i=k+1; i<=n; i++)
			sigma += G1(i,k) * G1(i,k);
		t = absb + fabs( (double)a[k] ); absb = sqrt((double)sigma);
		*norm = max(*norm, t+absb);
		alpha = G1(k+1,k); /* g[k+1][k] */
		b[k] = beta = ( alpha < 0. ? absb : -absb );
		if( sigma != 0. ){
			gamma = 1. / (sigma - alpha*beta);
			G1(k+1,k) = alpha - beta; /* g[k+1, k] */
			for( i=k+1; i<=n; i++){
				sum = 0.;
				for( j=k+1; j<=i; j++)
					sum += G1(i,j) * G1(j,k);
				for(j=i+1; j<=n; j++)
					sum += G1(j,i) * G1(j,k);
				p[i] = gamma * sum;
			}
			t = 0.;
			for( i=k+1; i<=n; i++) t += G1(i,k) * p[i];
			t = .5 * gamma * t;
			for( i=k+1; i<=n; i++ ) p[i] -= t * G1(i,k);
			for( i=k+1; i<=n; i++ )
				for( j=k+1; j<=i; j++ )
					G1(i,j) -= G1(i,k) * p[j]
						   + p[i] * G1(j,k);
			for( i=2; i<=n; i++ ){
				t = 0.;
				for( j=k+1; j<=n; j++)
					t += X1(i,j) * G1(j,k);
				p[i] = gamma * t;
			}
			for( i=2; i<=n; i++ )
				for( j=k+1; j<=n; j++)
					X1(i,j) -= p[i] * G1(j,k);
		}
	} /* end k */
	a[n-1] = G1(n-1,n-1);
	a[n] = G1(n,n);
	b[n-1] = G1(n,n-1);
	t = fabs((double)b[n-1]);
	*norm = max( *norm, absb + fabs(a[n-1] + t ) );
	*norm = max( *norm, t + fabs((double)a[n]) );
	free(p);
	 return(1);
} /* end of Householder */

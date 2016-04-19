#define MODULUS	((1L << 31) - 1L)	/* 2^31 - 1 */
#define SIGN_BIT (1L << 31)		/* 32nd bit */

/* MCUNIF
 *	Multiplicative congruential number generator
 *	INPUT:  seed > 0 ==> reseed
 *
 * $Header: /b/usr/people/neuron/src/info/RCS/ivunif.c,v 6.0 90/06/04 14:14:48 lmo Exp Locker: lmo $
 * $Log:	ivunif.c,v $
 * Revision 6.0  90/06/04  14:14:48  lmo
 * 
 * 
 *
 */
long
mcunif(seed)
	int seed;
{
	static long x = 3;

	if (seed > 0) x = 2 * seed + 1;	/* seed must be odd */

/*	x *= 1226874159L; /* new */ 
	x *= 29903947L;  /* old multiplier */
	x &= ~ SIGN_BIT;

	return(x);
}

/* IFUNIF
 *	fast interval uniform random number generator.
 * This generates integers between 0 and n, but uses a a less
 * precise technique than ivuniv(), and hence can
 * lead to under-representation of numbers near n.  However,
 * it is suitable for some uses, such as shuffling.
 * INPUT:  n = max integer of interval
 *
 * OUTPUT: integer in range [0, n], inclusive
 */
long
ifuniv(n)
	long int n;
{
	register long z, x;

	x = mcunif(0);		/* don't reseed */
	n++;			/* correct range */
	if ( (z = x / (MODULUS / n)) < 0) return (0);
	return(z);
}

/* IVUNIF
 *	interval uniform random number generator.  From Bratley, et al.,
 * pg. 209.  Inversion algorithm
 * This uses an infinite precision technique to generate integers
 * between 0 and n without suffering from truncation errors, which can
 * lead to under-representation of numbers near n.
 *
 * INPUT:  seed > 0 ==> reseed
 *	   n	max integer of interval
 *
 * OUTPUT: integer in range [0, n], inclusive
 */
ivunif(seed, n)
	int seed, n;
{
	register int jtry;
	long mcunif(), x;
	long i, j, k, l, iji, ijr, kli, klr;

	x = mcunif(seed);
	n++;	/* correct range */

	jtry = x / (MODULUS / n);
	if (jtry <= 0) return(0);

	for ( ; ; jtry--) {
		/*
		 * check if m/ix <= n/jtry
		 */
		i = MODULUS;
		j = x;
		k = n;
		l = jtry;
		/*
		 * is i/j <= k/l, where i >= j, k >= l?
		 */
		for ( ; ; ) {
			iji = i / j;
			kli = k / l;
			if (iji < kli) return(jtry);
			if (iji > kli) break;
			/*
			 * look at remainders
			 */
			ijr = i % j;
			if (ijr <= 0) return(jtry);
			klr = k % l;
			if (klr <= 0) break;
			/*
			 * still not resolved, rephrase in standard
			 * form, i.e., is l/klr <= j/ijr?
			 */
			i = l;
			k = j;
			j = klr;
			l = ijr;
		}
	}
}

/* SHUFFLE
 *	shuffles a list of (int) indices
 *
 * ARGS:
 *	p	= pointer to array of ints
 *	n	= number of elements in list
 */
shuffle(p, n)
	register int *p, n;
{
	register int i, tmp;

	while(--n) {
		i = ifuniv(n);
		if (i != n) {
			tmp = p[n];
			p[n] = p[i];
			p[i] = tmp;
		}
	}
}

/* SSHUFFLE
 *	shuffles a list of (short) indices
 *
 * ARGS:
 *	p	= pointer to array of ints
 *	n	= number of elements in list
 */
sshuffle(p, n)
	register short int *p;
	register int n;
{
	register int i;
	register short int tmp;

	while(--n) {
		i = ifuniv(n);
		if (i != n) {
			tmp = p[n];
			p[n] = p[i];
			p[i] = tmp;
		}
	}
}


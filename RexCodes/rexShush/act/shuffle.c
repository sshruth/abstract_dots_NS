/*
 * SHUFFLE
 *	Shuffles a list by permuting it.  The algorithm is based on
 * the method of Moses and Oakford, as explained in Knuth, vol. 2,
 * pg 125, in Algorithm P
 *	This uses a very fast, but poor, random number generator, based
 * on the additive number generator of Green, Smith and Klem, as
 * explained in Knuth, vol. 2, pg. 26 (Algorithm A).
 *
 *
 * INPUT
 *	k	The number of items to be shuffled
 *	shift	The number of shifts for the random generator
 *	table	The address of the ints to be shuffled
 */

static unsigned r_Y[17] = {
	2749,
	12767,
	9084,
	12060,
	32225,
	17543,
	25089,
	21183,
	25137,
	43217,
	26966,
	4978,
	20495,
	10311,
	11367,
	30054,
	17031,
};

static unsigned r_Z = 2749;
static unsigned * r_pY = & r_Y[16];	/* pointer to current Y */
static unsigned * r_pYl = & r_Y[16];	/* pointer to last valid Y */

int shuffle(unsigned k, int shift, int *table)
{
	register unsigned *x;
	register unsigned j;

	for (j = k-1; j > 0; j--) {

		/*
		 * get a random number between 0 and j
		 */
		x = r_pY;
		if (x < r_Y) x = r_pYl;
	
		k = *x;
		k += r_Z;
	
		/*
		 * save the new value
		 */
		*x = k;
		r_Z = k;
		r_pY = --x;
	
		/*
		 * produce a number between 0 and j
		 */
		k >>= shift;
		k %= j+1;

		/*
		 * exchange these elements
		 */
		x = (unsigned *) table[k];
		table[k] = table[j];
		table[j] = (int) x;
	}
	return(0);
}

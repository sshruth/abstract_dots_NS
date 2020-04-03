/*
 * function to determine sign of double argument
 *
 */
double fsgn(x)
	double x;
{
	if (x < 0) return(-1.0);
	if (x > 0) return(1.0);
	return(0.0);
}

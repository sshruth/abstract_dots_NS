#include "diff.h"

#ifndef PI
#define	PI	3.14159265358979323846264338
#endif
#ifndef TWOPI
#define TWOPI	(2 * PI)
#endif

/*
 * public vars
 */
extern int ngauss;
extern DFILTER *gauss;

int mk_guass(double width);

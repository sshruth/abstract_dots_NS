#ifndef _TOYS_H_
#define _TOYS_H_

#include <string.h>
#include <math.h>
#include "safe_alloc.h"

/* PUBLIC CONSTANTS */
#define DEG2RAD	0.0174533
#define PI			3.141592654
#define MET 		1
#define ANYMET		0xff
#define ONES 		65535

/* PUBLIC MACROS */
#define TOY_RT_TO_X(x,r,t)	(int) ((double)(x) + ((double)(r)*cos(DEG2RAD*(double)(t))))
#define TOY_RT_TO_Y(y,r,t)	(int) ((double)(y) + ((double)(r)*sin(DEG2RAD*(double)(t))))
#define TOY_ADD_STRING(f1,f2) ((f1) ? (f2) ? (char *) strcat((char *) \
									   SAFE_REALLOC((f1), (strlen(f1) + strlen(f2) + 2)), (f2)) \
									 : (f1) : NULL)
#define TOY_CARBONCOPY(s) 	 ((s) ? strcpy(SAFE_MALLOC(strlen(s)+1),s) : NULL )
#define TOY_BOUND(x,b,t)	 (x <= t ? (x >= b ? x : b) : t)

#define TOY_PCT(c,i)			 ((c)+(i)?100.*(float)(c)/((float)(i)+(float)(c)):0.)

	/* random integer between 0 and r-1 */
#define TOY_RAND(r)			 ((int) ((r) * rand() / (RAND_MAX+1.0)))

/* PUBLIC ROUTINE PROTOTYPES */
int 	toy_atan		(int, int);
int 	toy_mag		(int, int);
long 	toy_exp		(long, long, long);
int	toy_unique	(int, int *, int *);
void toy_sort (float*, int);
float toy_median (float*, int);

#endif /* _TOYS_H_ */

/* lance.h:  lance's commonly used definitions */

/* macros */
#define sqr(x)	((x) * (x))
#define sgn(x)	((x) < 0 ? -1 : 1)
#define xswap(a,b) a ^= b, b ^= a, a ^= b
#define swap(a,b,c) c = a, a = b, b = c

#define max(x,y)	((x) >= (y) ? (x) : (y))
#define min(x,y)	((x) <= (y) ? (x) : (y))

#define BLKSIZE 512
#define UPDATE 2

#ifdef pdp
/* struct to define low and high bytes of a word. */
struct lohi {
	char lo, hi;
} ;

/* struct to define low and high words of a long int */
struct llohi {
	int lhi, llo;
} ;
#endif
#ifdef vax
typedef struct bytes {
	char lobyte;
	char hibyte;
} * BYTEP;

typedef struct words {
	short int loword;
	short int hiword;
} * WORDP;
#endif

/* high precision constants */
#define LN2	0.69314718055994530941
#define LN10	2.30258509299404568401
#define LOG10E	0.43429448190325182765
#define EULER	2.71828182845904523536
#define SQRT2	1.41421356237309504880
#define HALFSQRT2	.70710678118654752440
#define HALFPI	1.57079632679489661923
#define TWOPI	6.28318530717958647692
#define BIGPI	(32766*PI)	/* must be an even multiple of PI */
#define RADPDEG	0.01745329251994329576
#define	PI	3.141592653589793238462643

/*
 *-----------------------------------------------------------------------*
 * NOTICE:  This code was developed by the US Government.  The original
 * versions, REX 1.0-3.12, were developed for the pdp11 architecture and
 * distributed without restrictions.  This version, REX 4.0, is a port of
 * the original version to the Intel 80x86 architecture.  This version is
 * distributed only under license agreement from the National Institutes 
 * of Health, Laboratory of Sensorimotor Research, Bldg 10 Rm 10C101, 
 * 9000 Rockville Pike, Bethesda, MD, 20892, (301) 496-9375.
 *-----------------------------------------------------------------------*
 */

/*
 *	sac.h:  Header for the Position-Velocity saccade detection
 * algorithm.
 */

/*
 * Possible states for algorithm.  At interrupt time the algorithm
 * executes in one and only one state.
 */
#define S_SEARCH	0	/* look for point that exceeds postion range */
#define S_P1		1	/* Accumulate four more points that exceed */
#define S_P2		2	/* the position range.  If four consecutive */
#define S_P3		3	/* points aren't seen, go back to S_SEARCH */
#define S_EVAL		4	/* determine if saccade start seen */
#define S_ACCEPT	5	/* determine if saccade end seen */
#define S_DELAY		6	/* delay before looking for next saccade */

/*
 * Flags
 */
#define	SF_HOR		01	/* If 1: following saccade on horizontal
				 * component; if 0 on vertical
				 */
#define SF_ULIM		02	/* Determines which limit (upper or lower)
				 * being used in comparisons.
				 */
#define SF_OFF		04	/* saccade detection on/off */
#define SF_ONSET	010	/* saccade start seen */
#define SF_GOOD		020	/* saccade accepted */

/*
 * Parameters for position detection part of algorithm.
 */
extern struct lim {
	int	rpoint;		/* midpoint of range */
	int	range;		/* movements within rpoint-range to
				 * rpoint+range are ignored
				 */
	int	ulim;		/* equal to rpoint+range */
	int	blim;		/* equal to rpoint-range */
} limh, limv;

/*
 * Defaults for saccade recognition
 */
#define STSIZE		300		/* size of stacksack */
#define DELAY		40		/* Time in msec to delay after good
					 * saccade recognized.
					 */
#define RANGE		15		/* initial range */
#define VELSTART	6		/* Velocities are all times 6 due to */
#define VELEND		2		/* method of calculation. */
#define VELMAX		30000
#define VELMIN		15
#define DURMAX		100		/* These times dependent on interrupt
					 * rate.  At 1kHz, 200= 200msec e.g.
					 */
#define DURMIN		10

/*
 * Variables
 */
extern int sacstate;		/* current state in algorithm */
extern int sacflags;
extern int duration;		/* duration of saccade times interrupt rate */
extern long sacontime;		/* saccade start time */
extern int timeout;		/* delay variable */
extern int delay_time;		/* actual delay */
extern int *aval, *bval, *newval;
extern struct lim *limp;
extern int *sacfillp;		/* pointer for saccade stack */
extern int sacstack[STSIZE];	/* This stack begins filling when a point
				 * exceeds range; used in vel computations.
				 * Assume no saccade > 300ms (@1khz int rate).
				 */
extern int peakvel;		/* peak velocity of saccade */
extern int velstart;		/* threshold velocity for saccade start */
extern int velend;		/* threshold for saccade end */
extern int velmax;		/* criteria for saccade acceptance */
extern int velmin;		/* peakvel must fall between velmax,min */
extern int durmax;		/* duration must fall between these */
extern int durmin;
extern int h_sacinit;		/* Initial position of possible saccade */
extern int v_sacinit;
extern int h_sacend;
extern int v_sacend;
extern int h_sacsize;		/* Size of the most recent accepted saccade */
extern int v_sacsize;
extern int aeyeflag;		/* absolute position eye flags */


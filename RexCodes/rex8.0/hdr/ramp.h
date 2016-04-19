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
 *	Header for ramp generator.
 */
/*
 * Parameters used in ramp computation.
 */
typedef struct {
	int	ra_mode;	/* mode of algorithm; see defines */
	int	ra_err;		/* error term */
	int	ra_rem;		/* remainder term */
	int	ra_inc;		/* increment for remainder slope */
	unsign	ra_d;		/* distance to cover */
	unsign	ra_de;		/* epsilon distance added for start */
	unsign	ra_dtime;	/* time to cover it in */
	int	ra_quot;	/* quotient if slope > 1 */
	int	ra_pos;		/* current position: 2s comp, 40 steps/deg */
	int	ra_valid;	/* output valid flag;  if not set d/a will
				   not be updated even if it is connected */
} RA_PARAM;

/*
 * One of the structs is needed for each ramp.  Includes two instances
 * of the RA_PARAM struct for x and y.
 */
struct ra_struct {
    int	m_ra_menu;	/* menu override flag */
    int	ra_type;	/* ramp type; see defines below */
    int	m_ra_type;
    int	ra_len;		/* parameters specifing ramp; length in tenths
			   of degree */
    int	m_ra_len;	/* menu prototype */
    int	ra_ang;		/* angle (polar coordinates) in degrees */
    int	m_ra_ang;
    int	ra_vel;		/* velocity in deg/sec */
    int	m_ra_vel;
    int	ra_xoff;	/* xoffset, yoffset (cart coord) of either
			   begin, middle, or end point of ramp */
    int m_ra_xoff;
    int	ra_yoff;
    int m_ra_yoff;
    int	ra_ecode;	/* store ramp event code */
    int	m_ra_ecode;
    unsign ra_duration;	/* duration of ramp less 24ms startup time */
    unsign ra_timeleft;	/* time remaining on current ramp */
    int	ra_state;	/* state of ramp generator */
    int	ra_urate;	/* update rate for ramp */
    int	ra_rampflag;	/* current ramp state */
    int	ra_wtime;	/* timer for start delays */
    int	ra_xflag;	/* flag used in ramp output section */
    int	ra_vfactor;	/* velocity compensation factor for ramp
			   initial position.  See comment in ramp.c */
    DIO_ID ra_device;	/* if non-zero, ramp generator turns off
			   this device at end of ramp */
    DIO_ID ra_phidev;	/* phi device */
    int	ra_phiflag;	/* phi flag */
    int ra_pontime;	/* phi on time in msec */
    int	ra_pofftime;	/* phi off time in ms */
    int ra_phistate;
    int ra_phicount;
    RA_PARAM ra_x;	/* parameters for x ramp */
    RA_PARAM ra_y;	/* parameters for y ramp */
    struct ra_struct *ra_for;	/* forward pointer to next active ramp */
    struct ra_struct *ra_back;	/* back pointer to previous active ramp */
};
typedef struct ra_struct RAMP;
extern RAMP ramp[RA_MAXNUM];
extern RAMP ra_first, ra_last;

/*
 * Defines for ra_mode.
 */
#define RA_NOMOVE	0	/* either a vert or horiz line */
#define RA_EQINT	1	/* slope == integer value */
#define RA_LESS1	2	/* slope of time vs. distance is < 1 */
#define RA_GREAT1	3	/* slope of time vs. dist is > 1 
				   and not an integer value */

/*
 * Defines for ra_state.
 */
#define RA_OFF		0
#define RA_DELST	1	/* start ramp with initial 40ms delay for mirs
				   to settle after moving to start position */
#define RA_DELWAIT	2	/* wait for 40msec to elapse */
#define RA_NODEL	3	/* start ramp with no 40msec delay */
#define RA_GO		4	/* begin ramp */
#define RA_25WAIT	5	/* wait for 25msec accel delay to elapse if
				   NO25MS is not set */
#define RA_RUN		6	/* output ramp */

/*
 * Defines for ra_rampflag.
 */
#define RA_STARTED	01	/* ramp has started */
#define RA_CDONE	02	/* ramp has been computed */
#define RA_TIMEDONE	010	/* set by function ramptd when times set
				   by function pre2set have elapsed;  used
				   in timing duration of ramps */
#define RA_TIMESTART	0100000	/* used to synchronize state set that times
				   ramp durations when setting RA_TIMEDONE
				   flag;  see a ramp paradigm for info */

/*
 *	Defines for ramp type.  Must fit in a char.
 */
#define RA_NO25MS	01	/* dont prepend 25ms acceleration time to
				   ramp  */
#define RA_CENPT	02	/* ramp xoffset, yoffset is center of ramp */
#define RA_BEGINPT	04	/* x, yoffset is begining of ramp */
#define RA_ENDPT	010	/* x, yoffset is end of ramp */

/*
 *	Defines for ra_phiflag (phi tracking flag).
 */
#define RA_PHIGO	0100000		/* phi tracking on/off */

/*
 * Return argument for 'ra_compute_time()' function.
 */
typedef struct {
    long    ra_ramp_time_preset;	/* preset */
    int	    ra_ramp_time_random;	/* random */
} RA_RAMP_TIME;
   
/*
 * Prototypes from act/ramp_act.c
 */
int ra_new(int rnum, int len, int ang, int vel, int xoff,
	   int yoff, int ecode, int type);
int Pra_new(int *rnum, int *len, int *ang, int *vel,
			int *xoff, int *yoff, int *ecode, int *type);
int ra_start(long rnum, long flag, DIO_ID device);
int Pra_start(long *rnum, long *flag, DIO_ID *device);
int ra_stop(long rnum);
int Pra_stop(long *rnum);
int ra_tostart(long rnum);
int Pra_tostart(long *rnum);
int ra_phistart(long rnum);
int Pra_phistart(long *rnum);
int ra_phiend(long rnum);
int Pra_phiend(long *rnum);
int ramptset(long rnum, long presetper, long randper);
void ra_unlink(RAMP *rp);
RA_RAMP_TIME ra_compute_time(long rnum, long presetpre, long randper);
int Pra_compute_time(long *rnum, long *presetper, long *randper, RA_RAMP_TIME *ra_time);
int ramptd(long rnum);
int Pramptd(long *rnum);
int ra_init(void);
/* act/ramp.c */
void ra_compute(int num);

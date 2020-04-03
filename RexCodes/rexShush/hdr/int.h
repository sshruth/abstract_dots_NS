/*
 *-----------------------------------------------------------------------*
 * This code was developed by the US Government.  The original
 * versions, REX 1.0-3.12, were developed for the pdp11 architecture and
 * distributed without restrictions.  This version, REX 4.0, is a port of
 * the original version to the Intel 80x86 architecture.  This version is
 * distributed only under license agreement from the National Institutes 
 * of Health, Laboratory of Sensorimotor Research, Bldg 10 Rm 10C101, 
 * 9000 Rockville Pike, Bethesda, MD, 20892, (301) 496-9375.
 *-----------------------------------------------------------------------*
 */

/*
 * int.h:  Local header for interrupt process. Should not
 * place anything in here that requires previous inclusion of other
 * headers.
 */
#define TEN_TO_IC	2		/* converts TENs of deg to internal
					   calibration (40 st/deg) */

/*
 * Defs and macros for interprocess communication used in lower level of
 * int process.
 */
#define to_scrb_(msg)	(flag_msg |= (scrb_msg |= s_(msg)))
#define to_disp_(msg)	(flag_msg |= (disp_msg |= s_(msg)))
#define to_comm_(msg)	(flag_msg |= (comm_msg |= s_(msg)))
extern int scrb_msg, disp_msg, comm_msg, flag_msg;

#define MEX_TIMER		200		/* times in milliseconds at which to send mex
								 * rex clock information
								 */
#define TIMER_DEFAULT	10	    /* default for system timer in
								 * msec;  should be 10msec */

/*
 * Flags controlling data storage windows.  User should only SET first three
 * bits; never CLEAR any bits.
 */
#define W_CANCEL	01		/* cancel an open window without writing
							 * on disk */
#define W_OPEN		02		/* open data storage window */
#define W_CLOSE		04		/* close data window */

#define W_ISOPEN	010		/* window currently open */
#define W_WAITCLOSE	020		/* window has been closed; waiting until post
							 * time w_post elapses */
#define W_NULLOPEN	040		/* window opened when keeping not on */
#define W_ERR		0100	/* err condition, causes window to close */

#define LFP_CANCEL	0x100		/* cancel saving of local field potential */
#define LFP_OPEN	0x200		/* start saving local field potential */
#define LFP_CLOSE	0x400		/* stop saving local field potential */

extern int (*p_mksroot)();

/*
 * This struct is used to hold variables from the signal menu.
 */
typedef struct {
#define SIG_OFF 0
#define SIG_AD_SOURCE 1
#define SIG_MEM_SOURCE 2
#define SIG_COMP_SOURCE 3
    int	sig_enable;		/* if NULLI, this signal is disabled */
    int sig_ad_chan;	/* a/d channel for this signal */
    char sig_ad_mem_var[P_LNAME];   /* memory variable for a/d channel */
    int sig_ad_rate;	/* a/d sample rate in hz */
    int	sig_ad_calib;	/* a/d scale factor */
    int	sig_ad_gain;	/* channel gain for a/d's that have channel-gain lists */
    int sig_ad_delay;	/* delay in msec for a/d channel filters */
    char sig_mem_source[P_LNAME];   /* memory variable for non-a/d source */
    char sig_title[P_LNAME];	/* user specified title of signal */
    int sig_store_rate;	/* rate at which signal is stored to A-file */
} SIG;
extern SIG sig[];

/*
 * Holds names and addresses of signals that can be specified in the
 * signal menu.  Initialized in i.c.
 */
typedef struct {
    int	*gv_ptr;	/* pointer to global variable */
    char *gv_name;	/* ascii name of variable */
} GVPTR;
extern GVPTR gv_ad_mem[], gv_mem_src[];
extern int cursTyp[WDI_NDATACURS];

extern int prim_dio_base;	/* base address of primary digital i/o board */
extern int *sa_gvptr[];  	/* pointers to internal global variables
							   for each signal, e.g. 'eyeh',
							   'eyev', etc.  These pointers are
							   currently local to int's address space;
							   when bit 'SA_MEMSIG' is set, this
							   is address of incore variable that is
							   source instead of a/d channel */
extern int ad_ov_gain;		/* overall gain for a/d's without individual
							   gains */
extern int ad_max_rate;		/* max acquisition rate and a/d interrupt rate */
extern int ad_min_rate;		/* lowest support a/d acquisition rate */
extern int w_pre;			/* time in msec before window opening that data will
							   be saved */
extern int w_post;			/* time in msec after window closing that data will
							   be kept */
extern int m_pre, m_post;	/* menu prototypes of above */
extern int adumpinc;		/* initialization for high water mark */
extern unsign w_flags;		/* contains window flags */
extern int inside_int;		/* flag is true when int routine is running */
extern int inside_clock;	/* flag is true when inside clock routine */
extern short *sa_frame;		/* pointer to current position in frame array */
extern int sa_mfrbeglx;		/* most recent load index pointer to beginning
							   of master sample frame */
extern long sa_mfrbegtime;  /* time of most recent master frame */
extern int sa_1msec_ctd;    /* countdown timer for 1msec intervals */
extern int sa_int_P_ms;	    /* number of ticks of current a/d interrupt
							   rate to equal 1msec;  used to initialize
							   sa_1msec_ctd */
extern int fillcheck;		/* used to check for abuf overflow */
extern int eyeh, eyev;		/* globals to store current eye pos samples */
extern int otherh, otherv;	/* other channels stored in h and v buffers */
extern int oeyeh, oeyev;	/* other eye channel */
extern int addh, addv;		/* additional channels */
extern int joyh, joyv;		/* joystick inputs */
extern int winCntrx, winCntry;
extern int vergh, vergv;		/* computed vergence values (eye[hv] - oeye[hv])*/
extern int cycloph, cyclopv;	/* computed cyclopian view values ((eye[hv] + oeye[hv]) / 2) */
extern int gazeh, gazev;		/* computed eye-in-head values (eye[hv] - other[hv]) */
extern int ogazeh, ogazev;		/* computed other eye-in-head (oeye[hv] - other[hv]) */
extern int vgazeh, vgazev;		/* computed e-i-h vergence (gaze[hv] - ogaze[hv]) */
extern int cgazeh, cgazev;		/* computed e-i-h cyclopian view ((gaze[hv] + ogaze[hv]) / 2) */
extern RT_CURPTR curPtrs[WDI_NDATACURS]; /* array of pointers to eye window cursor data */
#ifdef SMI_TRACK
extern int leyex, leyey, reyex, reyey;	/* SMI variables */
extern int lpup, rpup;
extern int smi_icnt;		/* interrupt count */
extern int smi_old, smi_new;	/* save toggle bit status */
extern int smi_late, smi_debtog, smi_debtog2, smi_deblval, smi_debhval;
extern int smi_overrun;
extern int smi_counts[SMI_MAXCOUNTS];
extern int smi_offset, smi_gain, smi_on;
#endif
extern unsigned r4_clicks , r5_clicks;
extern unsigned ib4_clicks, ib5_clicks;
extern int sdctl;				/* saccade detection control */
extern int Ctraceon;			/* C running line trace on/off */
extern int rl_wrap;				/* when true, running line wraps */
extern int rl_sig, rl_caddr;	/* variables for assigning C trace */
extern int dina, dinb;			/* holds digital input words */
extern int tyes, tno;			/* trial counts */
extern int noflag;				/* flag set by score() on bad trials */
extern int m_calnum;			/* temp storage for variables that live in
								   int_block */
extern int t_wrate;				/* number of ms to skip between eye window
								   checks */
extern int w_rate;				/* refresh rate for wind display */
extern int int_request;			/* interrupt services request flag */
extern int m_sdctl;				/* saccade detector control */
extern int m_uwind, uwind;		/* unit window control flags */
extern int st_rate;				/* state processor execution rate */
extern int sp_lock, ds_lock;	/* locks to prevent re-entering state
								   set proc and display */
extern int Ctcount, Cstate;
extern long c_count;			/* continuation record count */
extern int a_count;				/* analog record write count */
extern long awtime;				/* analog write time */
extern int winrate;				/* rate for eye window check */
extern int rurate;				/* ramp update rate */
extern int eyeint;				/* alternates each int */
extern int samcnt;				/* determines which interrupts data is saved */
extern int hiwat;				/* high water point for writing to disk */
extern int windcnt;				/* fill count of unwritten data for
								   the current window opening */
extern int postcnt;				/* count of samples to save after window
								   closes */
extern int lowx;				/* low index for next write of analog buffers */
extern int lasthighx;			/* last high index of write, or open index */
extern int skip_cnt;			/* skip count for sections of int that may
								   not be executed every interrupt */
extern int mrxout, mryout;
extern int st_count;
#ifdef DMRBLOCK
extern int rlevel;				/* re-interrupt level */
#endif
extern int m_bitdio;			/* default device for noun 'bit' */
extern int null_int;			/* null location */

/* array holding state debugging flags */
typedef struct {
	char color;
	long time;
} DEBUG_STATE;

extern DEBUG_STATE stateDebug[];

/* array of state pointers */
extern STATE *snames[];

/*
 * Defines for backward compatibility.
 */
#define drinput		dina

/*
 * Software switches for PSTOP and RLSTOP.
 */
extern int softswitch;

extern WD *wd_wind_cur_p;  		/* pointers to window cursors */
extern WD *wd_owind_cur_p;
extern int eyeflag;				/* eye position checking flag */
extern int m_wd_windcur_wd_num;
extern int m_wd_owindcur_wd_num;
extern int m_wd_center_cur_num;
extern int m_wddisp;		/* window display flags */

/*
 * Values for wd_source.
 */
#define WD_DIRPOS	0	/* window is not connected;
						   positioned via wd_dir_pos() action */
#define WD_RAMP_X	1	/* window position is connected to ramp */
#define WD_RAMP_Y	2
#define WD_JOY_X	3	/* window position is connected to joystick */
#define WD_JOY_Y	4
#define WD_DA		5	/* window position is connected to d/a */
#define WD_SIGNAL	6	/* window position determined by a signal */
#define WD_MEM		7	/* window position determined by mem array */

/*
 * Values for wd_cntrl.
 */
#define WD_OFF		0	/* window is off;  flags report that checked
				   value is inside window */
#define WD_ON		1	/* window checking is on */

/*
 * Values for eyeflag.  These definitions must correspond to the
 * initialization of wd_xflag and wd_yflag in 'wd_init()'.
 */
#define WD0_X		0x1	/* window 0 horiz */
#define WD0_Y		0x2	/* window 0 vert */
#define WD0_XY		0x3	/* window 0 both */

#define WD1_X		(WD0_X << 2)
#define WD1_Y		(WD0_Y << 2)
#define WD1_XY		(WD0_XY << 2)

#define WD2_X		(WD0_X << 4)
#define WD2_Y		(WD0_Y << 4)
#define WD2_XY		(WD0_XY << 4)

#define WD3_X		(WD0_X << 6)
#define WD3_Y		(WD0_Y << 6)
#define WD3_XY		(WD0_XY << 6)

#define WD4_X		(WD0_X << 8)
#define WD4_Y		(WD0_Y << 8)
#define WD4_XY		(WD0_XY << 8)

#define WD5_X		(WD0_X << 10)
#define WD5_Y		(WD0_Y << 10)
#define WD5_XY		(WD0_XY << 10)

#define WD6_X		(WD0_X << 12)
#define WD6_Y		(WD0_Y << 12)
#define WD6_XY		(WD0_XY << 12)

#define WD7_X		(WD0_X << 14)
#define WD7_Y		(WD0_Y << 14)
#define WD7_XY		(WD0_XY << 14)

#define WD0_1_XY	((WD0_XY) | (WD1_XY))

/*
 * Memory arrays.  This struct controls the memory array sources.
 */
struct ma_struct {
    int ma_status;		/* status flag */
    int ma_repcnt;		/* current number of repeats */
    int m_ma_menu;		/* menu override flag */
    int *ma_bap;		/* pointer to beginning of array */
    int *m_ma_bap;
    int ma_count;		/* number of points to output */
    int m_ma_count;
    int ma_rate;		/* msec per point */
    int m_ma_rate;
    int ma_repeat;
    int	m_ma_repeat;		/* repeat count, -1 = continuous */
    int	*ma_cur_bap;		/* current output pointer */
    int ma_cur_val;		/* current output value */
    int ma_cur_cnt;		/* current count */
    int ma_cur_time;		/* time in msec until next point is output */
    int ma_cur_repeat;		/* current repeat count */
    int ma_valid;		/* output valid flag */
    struct ma_struct *ma_for;	/* forward pointer to next active array */
    struct ma_struct *ma_back;	/* backward pointer to previous active array */
};
typedef struct ma_struct MA;
extern MA ma[MA_MAXNUM];
extern MA ma_first, ma_last;

/*
 * Flags for 'ma_status'.
 */
#define MA_RUN	    0x1		/* memory array is running */

/*
 * Eye channel offsets.
 */
extern int m_offmenu;		/* menu overides enabled */
extern int m_eho, m_evo;	/* menu variables for eye offsets */
extern int m_oeho, m_oevo;	/* menu variables for other eye offsets */
extern int eho, evo, oeho, oevo;

/*
 * Stabilization control.
 */
#define ST_OFF		0	/* note that stctl contains bit flags and is
				   not a scalar value, as other 'ctls */
#define ST_HOR		01	/* horiz stab */
#define ST_VERT		02	/* vert stab */

extern int m_stmenu;
extern int m_stxoff, m_styoff;	/* menu copies of stab offsets */
extern int m_stctl;
extern int stctl;		/* actual control values */
extern int stxoff, styoff;
extern int stx_pos, sty_pos;	/* final eye+offset values to output */
extern int c_dim;	/* dim device */
extern int c_curp;	/* index to current eye position structure */
extern int c_xhold;	/* temp holds eye positions */
extern int c_yhold;
extern int c_oxhold;
extern int c_oyhold;

/*
 * 	Values for eye positions are accumulated in this struct.
 */
#define C_KEEPMAX	10	/* save 10 previous values */
extern struct c_eyekeep {
	int c_xsav;
	int c_ysav;
	int c_oxsav;
	int c_oysav;
} c_eyekeep[C_KEEPMAX];

#ifdef MEXWEX_UNITS
extern int mex1_sync;
extern int mexwex_units;

/*
 * This struct must be initialized in cnf.c.
 */
typedef struct {
    int	    mex2_port;		/* input port */
    int	    mex2_ecode_base;	/* base ecode of codes carried in this byte */
	int		mex2_unit_base;
    int	    mex2_seq;		/* MEX communication sequence number */
#ifdef MEX2_TEST
    int	    mex2_cur;		/* current test number */
#endif
} MEX2;

#ifdef MEX62_UNITS
typedef struct {
	long rexClock;
	short int rexFlag;
	unsigned short int code; 
} MEX_TIME;
extern MEX_TIME mexTime;
extern int mexClockSendCtr;
#endif

extern MEX2 mex2[];
extern int mex2_sync;
#endif

#ifdef TC_UNITS
/*
 * This struct must be initialized in cnf.c.  Code for acquiring units
 * with 9513 counter/timer chips.  One of these structs exists for
 * each 9513 chip.
 */
typedef struct {
    int	    tc_port;		/* address of control port */
    int	    tc_num_inuse;	/* there are 5 counter/timers per chip;
				   they are used sequentially from the
				   first- this tells how many of the 5
				   are in use */
    int	    tc_ecode_base;	/* ecode base for unit codes on this chip */
    unsigned short tc_lcnt[5];	/* last count recorded from chip */
    int	    tc_inuse_bits;	/* bit field for inuse counters; computed
				   in prego.c */
} TC;
extern TC tc[];
extern int tc_units;
#endif

/*
 * Prototypes.
 */
void clk_stop(void);
void clk_set(int msec);
void clk_start(void);
void ad_start(void);
void ad_stop(void);
int ad_ds_init(void);
int ad_ds_uninit(void);
void ad_ds_begin(void);
void ad_ds_end(void);
void oncntrlb(int sig);

/* prototype for alert depends on whether
 * photon compatable signals are used
 */
#ifdef PHOTON_SIG_COMPAT
int alert(int sig, void *data);
#else
void alert(int sig);
#endif

int flag_af(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd);
void again(void);
/* variable access functions */
int ra_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd);
int ras0_rate_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd);
int cp_chan_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd);
int cp_rate_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd);
int cp_cal_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd);
int sd_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd);
int wd_sc_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd);
int wd_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd);
int mr_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd);
int cp_aw_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd);
int cp_uw_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd);
int off_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd);
int st_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd);
int rl_chan_maf(int flag, MENU *mp, char *astr, ME_RECUR *rp);
int rl_chan_agf(int call_cnt, MENU *mp, char *astr);
int rl_chan_sig_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd);

int prego(void);
const struct sigevent *ad_int_hand(void *arg, int id);
void msec_clock(int sig);

/* action prototypes */
/* act/awind.c */
int awind(long flag);
int pre_post(long pre, long post);
int Ppre_post(long *pre, long *post);
int mwind(long flag);
int tst_mx_new( void );

/* act/cal.c */
int hold_eye(void);
int keep_eye(void);

/* act/da_set.c */
int da_cntrl_1(long danum, long source, long src_num);
int Pda_cntrl_1(long *danum, long *source, long *src_num);
int da_cntrl_2(long danum0, long source0, long src_num0,
			   long danum1, long source1, long src_num1);
int Pda_cntrl_2(long *danum0, long *source0, long *src_num0,
				long *danum1, long *source1, long *src_num1);
int da_mode(long danum, long mode);
int Pda_mode(long *danum, long *mode);
int da_set_1(long danum, long val);
int Pda_set_1(long *danum, long *val);
int da_set_2(long danum0, long val0, long danum1, long val1);
int Pda_set_2(long *danum0, long *val0, long *danum1, long *val1);
int da_offset(long danum, long val);
int Pda_offset(long *danum, long *val);
int da_cursor(long da_x, long da_y, long cursor);
int Pda_cursor(long *da_x, long *da_y, long *cursor);
int da_init(void);
/* act/dout.c */
int dio_on(DIO_ID id);
int Pdio_on(DIO_ID *id);
int dio_off(DIO_ID id);
int Pdio_off(DIO_ID *id);
int dio_onoff(DIO_ID id_on, DIO_ID id_off);
int Pdio_onoff(DIO_ID *id_on, DIO_ID *id_off);
int dio_out(DIO_ID id, long value);
int Pdio_out(DIO_ID *id, long *value);
/* act/ma_set.c */
int ma_cntrl(int manum, int *bap, int count, int rate, int repeat);
int Pma_cntrl(int *manum, int *bap, int *count, int *rate, int *repeat);
int ma_reset(int manum);
int Pma_reset(int *manum);
int ma_start(int manum);
int Pma_start(int *manum);
int ma_stop(int manum);
int Pma_stop(int *manum);
void ma_unlink(MA *map);
int ma_init(void);
/* act/off_eye.c */
int off_eye(long hoff, long voff);
int Poff_eye(long *hoff, long *voff);
/* act/off_oeye.c */
int off_oeye(long hoff, long voff);
int Poff_oeye(long *hoff, long *voff);
/* act/rl_set.c */
int rl_setspike(long spike_num);
int rl_setspike_bits(long spike_bits);
int rl_setbar(long val);
int rl_addbar(long val);
int rl_trig(void);
int rl_stop(void);
int rl_erase(void);
/* act/score.c */
int score(long x);
/* act/sd_set.c */
int sd_set(long ctlval);
int sd_mark(long chan, long flag);
void sd_mark_sig(char *sig_name, int mark);
/* act/shuffle.c */
int shuffle(unsigned k, int shift, int *table);
/* act/stateTime.c */
int set_times(char *name, long preset, long random);
int Pset_times(char *name, long *preset, long *random);
long getClockTime(void);
int PgetClockTime(long *time);
int PsetFlag(int *flag, int val);
/* act/ustatus.c */
int ustatus(char *sp);
/* act/uw_set.c */
int uw_set(long flag);
/* act/wd_set.c */
int wd_cntrl(long wdnum, long flag);
int Pwd_cntrl(long *wdnum, long *flag);
int wd_src_pos(long wdnum, long x_src, long x_src_num, long y_src, long y_src_num);
int Pwd_src_pos(long *wdnum, long *x_src, long *x_src_num, long *y_src, long *y_src_num);
int wd_src_check(long wdnum, long x_src, long x_src_num, long y_src, long y_src_num);
int Pwd_src_check(long *wdnum, long *x_src, long *x_src_num, long *y_src, long *y_src_num);
int wd_siz(long wdnum, long xsiz, long ysiz);
int Pwd_siz(long *wdnum, long *xsiz, long *ysiz);
int wd_pos(long wdnum, long xpos, long ypos);
int Pwd_pos(long *wdnum, long *xpos, long *ypos);
int wd_test(long flag);
int Pwd_test(long *flag);
int wd_init(void);
int wd_disp(long disp);
int wd_cursor(long wdnum, long cursor);
int wd_center(long cursor);

int mr_mov(long xpos, long ypos);
int stab_set(long ctlval, long xoff, long yoff);
/* int/prego.c */
int sig_init(void);
int sig_ck_sig_enable(int snum, int enable);
int sig_ck_chan(int snum, int chan);
int sig_ck_ad_mem_var(int snum, char *sp);
int sig_ck_ad_acq_rate(int snum, int rate);
int sig_ck_ad_max_rate(int snum, int rate);
void sig_compute_ad_rates(void);
int sig_ck_ad_calib(int snum, int calib);
int sig_ck_ad_gain(int snum, int gain);
int sig_ck_mem_src_var(int snum, char *sp);
int sig_ck_sig_store_rate(int snum, int store_rate);
int sig_ck_sig_title_var(int snum, char *sp);
int sig_check_str(char *sp);
GVPTR *gv_look(GVPTR *gvp, char *sp);
int get_rates_index(int snum, int rate);
void sig_e(int snum, char *sp);
void null_samp_arrays(void);
void null_sig_arrays(void);
void n_soft(char *vstr, char *astr);

/* prototypes for vex */
/* functions */
int tst_rx_new(void);
int tst_tx_rdy(void);
int to_vex(unsigned short len_of_msg);
unsigned char *vex_message(void);
unsigned char vex_code(unsigned char *msgfvex);
int vex_location(unsigned char *msgfvex);
unsigned char *float2byte(float value);
unsigned char hibyte(short sval);
unsigned char lobyte(short sval);

/* actions */
int PvexEraseMethod(int *method);
int PvexVideoSync(int *sync);
int PvexDigitalSync(int *sync);
int PvexSetRexScale(int *distance, int *width);
int PvexSetBackLum(int *r, int *g, int *b);
int PvexSetFixColors(int *nr, int *ng, int *nb, int *dr, int *dg, int *db);
int PvexSetFixSiz(int *size);
int PvexSetStimLuminances(int *nObjects, int *objList, int *fgList, int *bgList);
int PvexSetStimColors(int *nObjects, int *objList,
					  int *fgrList, int *fggList, int *fgbList,
					  int *bgrList, int *bggList, int *bgbList);
int PvexSetColorMask(int *nObjects, int *objList, int *redList,
					 int *greenList, int *blueList, int *alphaList);
int PvexSetGrayScale(int *start, int *length);
int PvexSetObjectGrayScale(int *nObjects, int *objList, int *startList, int *lengthList);
int PvexSetLutEntryClr(int *nEntries, int *entryList, int *rList, int *gList, int *bList);
int PvexSetObjectLutEntryClr(int *nObjects, int *objList, int *nEntries,
							 int *entryList, int *rList, int *gList, int *bList);
int PvexAllOff(void);
int PvexSwitchFix(int *flag);
int PvexDimFix(void);
int PvexPreloadStim(int *nObjects, int *objList, int *swtchList);
int PvexSwapBuffers(void);
int PvexSwitchStim(int *nObjects, int *objList, int *swtchList);
int PvexSetStimSwitch(int *nObjects, int *objList, int *swtchList);
int PvexTimeStim(int *nObjects, int *objList, int *nFields);
int PvexSequenceStim(int *nFrstObj, int *frstList, int *frstFields, int *gapFields,
					 int *nScndObj, int *scndList, int *scndFields);
int PvexSetFixLocation(float *x, float *y);
int PvexStimLocation(int *nObjects, int *objList, float *xList, float *yList);
int PvexStimFromFixPoint(int *nObjects, int *objList, float *xList, float *yList);
int PvexShiftLocation(int *nObjects, int *objList, float *xList, float *yList);
int PvexReportLocation(int *obj);
int PvexMessage(int *code, int *x, int *y);
int PvexSetActiveObject(int *obj);
int PvexClipRectSet(int *nObjects, int *objList, int *widthList,
					int *heightList, int *xList, int *yList);
int PvexClipRectFromFixPoint(int *nObjects, int *objList, int *widthList,
							 int *heightList, int *xList, int *yList);
int PvexClearClipRect(int *nObjects, int *objList);
int PvexDrawWalsh(int *nObjects, int *objList, int *patList, int *sizList, int *cntrstList);
int PvexDrawHaar(int *nObjects, int *objList, int *patList, int *sizList, int *cntrstList);
int PvexDrawRandom(int *nObjects, int *objList, int *colList, int *rowList, int *sizList, int *pwhiteList);
int PvexDrawAnnulus(int *nObjects, int *objList, int *outrList, int *innrList, int *cntrstList);
int PvexDrawBar(int *nObjects, int *objList, int *anglList, int *widList, int *lenList, int *cntrstList);
int PvexDrawFlowField(int *nObjects, int *objList, int *widthList, int *heightList,
					  int *nearList, int *farList, int *covList, int *sizeList);
int PvexDrawEllipticalFlowField(int *nObjects, int *objList, int *radList, float *widthList,
								float *heightList, int *nearList, int *farList, int *covList, int *sizeList);
int PvexMaskFlowField(int *nObjects, int *objList, int *widthList, int *heightList,
					  int *xList, int *yList);
int PvexDrawUserPattern(int *nObjects, int *objList, int *patList, int *sizeList, int *cntrstList);
int PvexDrawRgbUserPattern(int *nObjects, int *objList, int *patList, int *sizeList);
int PvexDrawTiffImage(int *nObjects, int *objList, int *patList);
int PvexDrawPngImage(int *nObjects, int *objList, int *patList);
int PvexDrawOknGrating(int *nObjects, int *objList, int *dirList, int *speedList, int *widList,
					   int *heightList, int *barwidList);
int PvexDrawSineGrating(int *nObjects, int *objList, int *orList, float *spatFreqList, float *tempFreqList, 
						int *cntrPhaseList, float *redList, float *greenList, float *blueList, int *sizList);
int PvexDrawHalfSineGrating(int *nObjects, int *objList, int *orList, float *spatFreqList, float *tempFreqList, 
							int *cntrPhaseList, float *redList, float *greenList, float *blueList, int *sizList);
int PvexDrawSquareGrating(int *nObjects, int *objList, int *orList, float *spatFreqList, float *tempFreqList,
						  int *cntrPhaseList, float *redList, float *greenList, float *blueList, int *sizList);
int PvexDrawHalfSquareGrating(int *nObjects, int *objList, int *orList, float *spatFreqList, float *tempFreqList,
							  int *cntrPhaseList, float *redList, float *greenList, float *blueList, int *sizList);
int PvexLoadPatterns(int *nObjects, int *objList, int *stimList, int *sizList, int *rowList,
					 int *colList, int *checkList);
int PvexLoadPointArray(int *obj, int *nPoints, int *siz, int *pointArray);
int PvexCopyObject(int *source, int *destination, float *xscale, float *yscale);
int PvexSetAliasing(int *nObjects, int *objList, int *aliasList);
int PvexSetTransparency(int *nObjects, int *objList, float *transList);
int PvexRotateObject(int *nObjects, int *objList, float *Xlist, float *Ylist, float *Zlist);
int PvexNewRamp(int *nRamps, int *rampList, int *lenList,
				int *dirList, int *velList, int *xList,
				int *yList, int *typeList, int *actList);
int PvexNewRampFromFixPoint(int *nRamps, int *rampList, int *lenList,
							int *dirList, int *velList, int *xList,
							int *yList, int *typeList, int *actList);
int PvexLoadRamp(int *nRamps, int *rampList, int *actList, int *stpList,
				 float *xList, float *yList);
int PvexLoadSound(int *nObjects, int *objList, int *sndList);
int PvexToRampStart(int *nObjects, int *objList, int *rmpList);
int PvexStartRamp(int *nRamps, int *rmpList, int *objList, int *cycleList);
int PvexResetRamps(void);
int PvexStartSine(int *nObjects, int *objList);
int PvexStartSquare(int *nObjects, int *objList);
int PvexTimeGrating(int *nObjects, int *objList, int *cycles);
int PvexStartGratingRamp(int *nObjects, int *gratingList, int *rmpList);
int PvexStopSine(void);
int PvexStopSquare(void);
int PvexStartOkn(int *nObjects, int *objList);
int PvexStopOkn(void);
int PvexFlowDirectionRange(int *nObjects, int *objList, int *rangeList);
int PvexFlowSpeedRange(int *nObjects, int *objList, int *speedList);
int PvexNewFlow(int *nObjects, int *objList, int *xyList, int *zList, int *velList,
				int *rollList, int *pitchList, int *yawList, int *spanList, int *coherList);
int PvexShiftFlow(int *nObjects, int *objList, int *xyList, int *zList, int *velList,
				  int *rollList, int *pitchList, int *yawList, int *spanList, int *coherList);
int PvexMakeFlowMovie(int *nObjects, int *nFrames, int *objList);
int PvexToFlowMovieStart(int *nObjects, int *startFrame, int *objList);
int PvexStartFlow(int *nObjects, int *objList);
int PvexTimeFlow(int *nObjects, int *fields, int *objList);
int PvexShowFlowMovie(int *nObjects, int *startFrame, int *lastFrame, int *objList);
int PvexStartFlowRamp(int *nFlows, int *flwList, int *nRamps, int *rmpList, int *objList);
int PvexStopFlowRamp(void);
int PvexShowMovieClip(int *nClips, int *objList, int *nFrames, int *interval, int *cycles);
int PvexStopMovie(void);
int PvexSetTriggers(int *nTrigs, int *rmpList, int *trigList, int *frameList);

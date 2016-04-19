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
 *	Initializations for int process.
 *
 */
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include "rexhdrs.h"
#include "ecode.h"
#include "sac.h"
#include "ramp.h"
#include "int.h"

/*
 * Version identifier for int process.
 */
char version[P_LVERSION]= "7.5 Jul 2002";

INT_BLOCK_P i_b;
RAW_BLOCK_P r_b;
int myptx= -1;
PROCTBL_P myptp= NP;
char NS[] = "";
sigset_t alert_set;
int errprt;
int null_int= 0;
int mx_photon_on= 0;
DEBUG_STATE stateDebug[MAXSTATE] = { 0 };

/*
 * Used in interrupt routine proper.
 */
int prim_dio_base;
SAMP samp;
int *sa_gvptr[SA_MAXSIG];
int ad_cglist[AD_CHANNELS];
int ad_max_rate= AD_RATE;
int cursTyp[WDI_NDATACURS];
int ad_min_rate;
int ad_ov_gain= 1;
short *sa_frame;
int sa_mfrbeglx;
long sa_mfrbegtime;
int sa_1msec_ctd;
int sa_int_P_ms;
int adumpinc= ADUMPINC;
int fillcheck;
int st_rate= ST_RATE;
int eyeh, eyev;		/* space for adc values */
int otherh, otherv;	/* place where other channels are stored */
int oeyeh, oeyev;	/* other eye values */
int addh, addv;		/* additional channels to make up to 8 */
int joyh, joyv;		/* joystick channels */

int vergh, vergv;		/* computed vergence values (eye[hv] - oeye[hv])*/
int cycloph, cyclopv;	/* computed cyclopian view values ((eye[hv] + oeye[hv]) / 2) */
int gazeh, gazev;		/* computed eye-in-head values (eye[hv] - other[hv]) */
int ogazeh, ogazev;		/* computed other eye-in-head (oeye[hv] - other[hv]) */
int vgazeh, vgazev;		/* computed e-i-h vergence (gaze[hv] - ogaze[hv]) */
int cgazeh, cgazev;		/* computed e-i-h cyclopian view ((gaze[hv] + ogaze[hv]) / 2) */
int winCntrx = 0;		/* center of the eye window */
int winCntry = 0;
RT_CURPTR curPtrs[WDI_NDATACURS]; /* array of pointers to eye window cursor data */

int sp_lock, ds_lock;	/* locks to prevent re-entering state
						   set proc and display */

int Ctcount, Cstate;
long c_count;			/* continuation record count */
long awtime;			/* analog write time */
int winrate;			/* rate for eye window check */
int rurate;			/* ramp update rate */
int hiwat;			/* high water point for writing to disk */
int windcnt;			/* fill count of unwritten data for
				   the current window opening */
int postcnt;			/* count of samples to save after window
				   closes */
int lowx;			/* low index for next write of analog buffers */
int lasthighx;			/* last high index of write, or open index */
int st_count;
int m_bitdio= BIT_DEFAULT;	/* default device for noun 'bit' */
sigset_t sig_state;
int int_request;
unsigned w_flags;		/* flags for write processing */
int scrb_msg, disp_msg, comm_msg, flag_msg;
int w_pre= W_PRE;
int w_post= W_POST;
int m_pre= NULLI, m_post= NULLI;
int m_uwind= NULLI, uwind= 1;	/* unit window control */
int dina, dinb;			/* digital input words */
int softswitch= 0;		/* software switches for PSTOP and RLSTOP */
int noflag;
int tyes, tno;
ESCAPE *firstesc;
RAMP ramp[RA_MAXNUM];

#ifdef MEXWEX_UNITS
int mex1_sync;
int mex2_sync;
int mexwex_units= 2;
#endif

#ifdef TC_UNITS
int tc_units= 0;
#endif

#ifdef SMI_TRACK
int leyex, leyey, reyex, reyey;	/* SMI variables */
int lpup, rpup;
int smi_icnt;			/* interrupt count */
int smi_old, smi_new;		/* save toggle bit status */
int smi_late;			/* statistics for clock sync */
int smi_overrun;
int smi_debtog, smi_debtog2, smi_deblval, smi_debhval;
int smi_counts[SMI_MAXCOUNTS];
int smi_offset= 0, smi_gain= 1, smi_on= 0;
#endif

/*
 * Running line.  These initializations are copied to the INT_BLOCK
 * when 'int' begins.
 */
RL rl_init = {
	{
		/*
		 * Channel initializations.
		 */
		{0,	    /* signal */
		 0},		/* mark request */

		{1,	    /* signal */
		 0},		/* mark request */

		{2,	    /* signal */
		 0},		/* mark request */

		{3,	    /* signal */
		 0},		/* mark request */

		{8,	    /* signal */
		 0},		/* mark request */

		{9,	    /* signal */
		 0},		/* mark request */

		{-1,	    /* signal */
		 0},		/* mark request */

		{-1,	    /* signal */
		 0},		/* mark request */
	},

	{
		/*
		 * Spike initializations.
		 */
#ifdef LATCH_UNITS
		{4,	    /* offset */
		 10},	/* color */
#endif
#ifdef MEX62_UNITS
		{8,		/* offset */
		 6},	/* color */
		{16,	/* offset */
		 3},	/* color */
		{24,   	/* offset */
		 4},	/* color */
		{32,	/* offset */
		 6},	/* color */
		{40,	/* offset */
		 3},	/* color */
		{48,	/* offset */
		 4},	/* color */
		{56,	/* offset */
		 6},	/* color */
		{64,	/* offset */
		 3},	/* color */
		{72,	/* offset */
		 4},	/* color */
		{80,	/* offset */
		 6},	/* color */
		{88,	/* offset */
		 3},	/* color */
		{96,	/* offset */
		 4},	/* color */
		{104,	/* offset */
		 6},	/* color */
		{112,	/* offset */
		 3},	/* color */
		{120,  	/* offset */
		 4},	/* color */
		{128,	/* offset */
		 6},	/* color */
		{136,	/* offset */
		 3},	/* color */
		{144,	/* offset */
		 4},	/* color */
		{152,	/* offset */
		 6},	/* color */
		{160,	/* offset */
		 3},	/* color */
		{168,	/* offset */
		 4},	/* color */
		{176,	/* offset */
		 6},	/* color */
		{184,	/* offset */
		 3},	/* color */
		{192,	/* offset */
		 4},	/* color */
#else
		{4,		/* offset */
		 11},	/* color */
		{6,		/* offset */
		 12},	/* color */
		{8,	   	/* offset */
		 13},	/* color */
		{10,	/* offset */
		 14},	/* color */
		{12,	/* offset */
		 15},	/* color */
		{14,	/* offset */
		 11},	/* color */
		{16,	/* offset */
		 12},	/* color */
		{18,	/* offset */
		 13},	/* color */
		{20,	/* offset */
		 14},	/* color */
		{22,	/* offset */
		 15},	/* color */
		{24,	/* offset */
		 11},	/* color */
		{26,	/* offset */
		 12},	/* color */
		{28,	/* offset */
		 13},	/* color */
		{30,	/* offset */
		 14},	/* color */
		{32,	/* offset */
		 15},	/* color */
		{34,	/* offset */
		 11},	/* color */
		{36,	/* offset */
		 12},	/* color */
		{38,	/* offset */
		 13},	/* color */
		{40,	/* offset */
		 14},	/* color */
		{42,	/* offset */
		 15},	/* color */
		{44,	/* offset */
		 11},	/* color */
		{46,	/* offset */
		 12},	/* color */
		{48,	/* offset */
		 13},	/* color */
		{50,	/* offset */
		 14},	/* color */
#endif
		{52,	/* offset */
		 15},	/* color */
		{54,	/* offset */
		 11},	/* color */
		{56,	/* offset */
		 12},	/* color */
		{58,	/* offset */
		 13},	/* color */
		{60,	/* offset */
		 14},	/* color */
		{62,	/* offset */
		 15},	/* color */
		{64,	/* offset */
		 11},	/* color */
		{66,	/* offset */
		 12},	/* color */
		{68,	/* offset */
		 11},	/* color */
		{70,	/* offset */
		 12},	/* color */
		{72,   	/* offset */
		 13},	/* color */
		{74,	/* offset */
		 14},	/* color */
		{76,	/* offset */
		 15},	/* color */
		{78,	/* offset */
		 11},	/* color */
		{80,	/* offset */
		 12},	/* color */
		{82,	/* offset */
		 13},	/* color */
		{84,	/* offset */
		 14},	/* color */
		{86,	/* offset */
		 15},	/* color */
		{88,	/* offset */
		 11},	/* color */
		{90,	/* offset */
		 12},	/* color */
		{92,	/* offset */
		 13},	/* color */
		{94,	/* offset */
		 14},	/* color */
		{96,	/* offset */
		 15},	/* color */
		{98,	/* offset */
		 11},	/* color */
		{100,	/* offset */
		 12},	/* color */
		{102,	/* offset */
		 13},	/* color */
		{104,	/* offset */
		 14},	/* color */
		{106,	/* offset */
		 15},	/* color */
		{108,	/* offset */
		 11},	/* color */
		{110,	/* offset */
		 12},	/* color */
		{112,	/* offset */
		 13},	/* color */
		{114,	/* offset */
		 14},	/* color */
		{116,	/* offset */
		 15},	/* color */
		{118,	/* offset */
		 11},	/* color */
		{120,	/* offset */
		 12},	/* color */
		{122,	/* offset */
		 13},	/* color */
		{124,	/* offset */
		 14},	/* color */
		{126,	/* offset */
		 15},	/* color */
		{128,	/* offset */
		 11},	/* color */
		{130,	/* offset */
		 12},	/* color */
		{132,	/* offset */
		 11},	/* color */
		{134,	/* offset */
		 12},	/* color */
		{136,  	/* offset */
		 13},	/* color */
		{138,	/* offset */
		 14},	/* color */
		{140,	/* offset */
		 15},	/* color */
		{142,	/* offset */
		 11},	/* color */
		{144,	/* offset */
		 12},	/* color */
		{146,	/* offset */
		 13},	/* color */
		{148,	/* offset */
		 14},	/* color */
		{150,	/* offset */
		 15},	/* color */
		{152,	/* offset */
		 11},	/* color */
		{154,	/* offset */
		 12},	/* color */
		{156,	/* offset */
		 13},	/* color */
		{158,	/* offset */
		 14},	/* color */
		{160,	/* offset */
		 15},	/* color */
		{162,	/* offset */
		 11},	/* color */
		{164,	/* offset */
		 12},	/* color */
		{166,	/* offset */
		 13},	/* color */
		{168,	/* offset */
		 14},	/* color */
		{170,	/* offset */
		 15},	/* color */
		{172,	/* offset */
		 11},	/* color */
		{174,	/* offset */
		 12},	/* color */
		{176,	/* offset */
		 13},	/* color */
		{178,	/* offset */
		 14},	/* color */
		{180,	/* offset */
		 15},	/* color */
		{182,	/* offset */
		 11},	/* color */
		{184,	/* offset */
		 12},	/* color */
		{186,	/* offset */
		 13},	/* color */
		{188,	/* offset */
		 14},	/* color */
		{190,	/* offset */
		 15},	/* color */
		{192,	/* offset */
		 11},	/* color */
	},
};

WIN_DATCURS cursTyp_init = {
	{
		{CHAN_CENTER},
		{CHAN_EYE},
		{CHAN_OEYE},
		{WDI_NCHANTYPE},
		{WDI_NCHANTYPE},
		{WDI_NCHANTYPE},
		{WDI_NCHANTYPE},
		{WDI_NCHANTYPE},
		{WDI_NCHANTYPE},
		{WDI_NCHANTYPE},
		{WDI_NCHANTYPE},
		{WDI_NCHANTYPE},
		{WDI_NCHANTYPE},
		{WDI_NCHANTYPE},
		{WDI_NCHANTYPE},
		{WDI_NCHANTYPE},
	}
};

WIN_DATCURS localCursTyp;
RL localRl;

int wdchange;
int m_wddisp= NULLI, m_wd_windcur_wd_num= NULLI, m_wd_owindcur_wd_num= NULLI;
int m_da_cur_xda= NULLI, m_da_cur_yda= NULLI, m_oda_cur_xda= NULLI;
int m_oda_cur_yda= NULLI, m_wd_center_cur_num= NULLI;
DA *da_cur_xda_p= &da[0], *da_cur_yda_p= &da[1];
DA *oda_cur_xda_p= &da[2], *oda_cur_yda_p= &da[3];
DA da_first, da_last;
RAMP ra_first, ra_last;
MA ma_first, ma_last;
MA ma[MA_MAXNUM];

/*
 * Eye offsets.
 */
int m_offmenu;
int m_eho= NULLI, m_evo= NULLI;
int m_oeho= NULLI, m_oevo= NULLI;
int eho, evo, oeho, oevo;

/*
 * Saccade detector.
 */
struct lim limh= {			/* horizontal defaults */
	0, RANGE, 0 + RANGE, 0 - RANGE
};
struct lim limv= {			/* vertical */
	0, RANGE, 0 + RANGE, 0 - RANGE
};

int m_sdctl= NULLI;
int sdctl;
int sacstate, sacflags, duration;
long sacontime;
int timeout, delay_time=DELAY;
int *aval, *bval, *newval, *sacfillp;
struct lim *limp;
int sacstack[STSIZE] = {0};
int peakvel;
int velstart= VELSTART, velend= VELEND, velmax= VELMAX, durmax= DURMAX;
int velmin= VELMIN;
int durmin= DURMIN, h_sacinit, h_sacend, h_sacsize;
int v_sacinit, v_sacend, v_sacsize;
int eyeflag;

/*
 * Calibration paradigms.
 */
int c_dim, c_curp, c_xhold, c_yhold, c_oxhold, c_oyhold;
struct c_eyekeep c_eyekeep[C_KEEPMAX] = {0};

/*
 * Dummy p_rxerr to resolve reference from rxerr().
 */
p_rxerr()
{
}

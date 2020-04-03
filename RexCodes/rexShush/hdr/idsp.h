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
 * idsp.h: Header for displays updated by int (running line and window).
 */

/*
 *-----------------------------------------------------------------------
 * Running line display.
 *-----------------------------------------------------------------------
 */
#define RL_MAXCHAN  8	    /* max number of running line channels */
#define RL_MAXSPIKE 96	    /* max number of spikes; don't define to be
							 * more than necessary- takes additional run
							 * time */
#define RL_MARKMAX  8	    /* max number of distinct marks allowed on
							 * channel traces */

#define RT_DISP_LEN 20

#define CU_DA_ONE 3
#define CU_DA_TWO 4

enum windCursor {
	CU_DATA0,
	CU_DATA1,
	CU_DATA2,
	CU_DATA3,
	CU_DATA4,
	CU_DATA5,
	CU_DATA6,
	CU_DATA7,
	WDI_NDATACURS,
	CU_WIND0 = WDI_NDATACURS,
	CU_WIND1,
	CU_WIND2,
	CU_WIND3,
	CU_WIND4,
	CU_WIND5,
	CU_WIND6,
	CU_WIND7,
	WDI_NCURS
};

enum windChanType {
	CHAN_CENTER,
	CHAN_EYE,
	CHAN_OEYE,
	CHAN_OTHER,
	CHAN_ADD,
	CHAN_JOY,
	CHAN_DA0,
	CHAN_DA1,
	CHAN_RAMP0,
	CHAN_RAMP1,
	CHAN_VERG,
	CHAN_CYCLOP,
	CHAN_GAZE,
	CHAN_OGAZE,
	CHAN_VGAZE,
	CHAN_CGAZE,
	WDI_NCHANTYPE
};

typedef struct {
	short level;
	unsigned char mark;
} RT_CHAN;

typedef struct {
	short x;
	short y;
} RT_CURSOR;

typedef struct {
	int *x;
	int *y;
} RT_CURPTR;

/* structure used to hold data for the real-time displays ewind and rline */
typedef struct {
	RT_CHAN rt_chans[RL_MAXCHAN];
	RT_CURSOR rt_cursors[WDI_NCURS];
	unsigned long rt_spikes[3];
	unsigned short rt_bar;
} RT_DISP;

/*
 * Parameters for running line display.
 */
typedef struct {
    int	    rl_signal;	    /* source signal number */
    int	    rl_mark_req;	/* request to put up a mark */
    void    *rl_addr;	    /* pointer to data to be displayed */
    char    *rl_sig_gvname; /* pointer to name of signal's global variable */
    char    *rl_sig_title;  /* pointer to name of signal's title */
} RL_CHAN;

typedef struct {
    int	    rl_spike_os;    /* offset for spike */
    int	    rl_spike_color; /* color for spike */
    int	    rl_spike_req;	/* request to put up a spike */
} RL_SPIKE;
    
typedef struct {
    RL_CHAN		rl_chan[RL_MAXCHAN];
    RL_SPIKE	rl_spike[RL_MAXSPIKE];
    int		    rl_bar;		    /* new value of bar */
} RL;
extern RL rl_init;	    /* inits for RL struct in shared area */

typedef struct {
	int type;
} WIN_CURSOR;

typedef struct {
	WIN_CURSOR	datCurs[WDI_NDATACURS];
} WIN_DATCURS;
extern WIN_DATCURS cursTyp_init;

/*
 * Commonly used marks.
 */
#define SAC_MARK_START	    1	/* saccade start */
#define SAC_MARK_GOOD	    2	/* good saccade */

extern int eyeh, eyev;		/* eye values acquired from A/D */
extern int oeyeh, oeyev;	/* other eye */
extern int wdchange;		/* flag is true when cursor changes have
				   occured during window display updates */

/*
 *-----------------------------------------------------------------------
 * Window display.
 *-----------------------------------------------------------------------
 */
/*
 * Windows.  Currently limited to supporting max of 8 windows.  Each
 * window requires two bits to test;  'eyeflag' is a 16 bit int.
 */
struct wd_struct {
    int	m_wd_menu;		/* menu override flag */
    int	wd_cntrl;		/* control flag */
    int	m_wd_cntrl;
    int wd_xpos_source;		/* source for x position of window */
    int	m_wd_xpos_source;
    int wd_xpos_src_num;	/* source number for x position */
    int m_wd_xpos_src_num;
    int wd_ypos_source;		/* source for y position of window */
    int	m_wd_ypos_source;
    int wd_ypos_src_num;	/* source number for y position */
    int m_wd_ypos_src_num;
    int wd_xcheck_source;	/* x source for what to check in window */
    int m_wd_xcheck_source;
    int wd_xcheck_src_num;
    int m_wd_xcheck_src_num;
    int wd_ycheck_source;	/* y source for what to check in window */
    int m_wd_ycheck_source;
    int wd_ycheck_src_num;
    int m_wd_ycheck_src_num;
    int wd_xdirpos;		/* x direct position of window */
    int m_wd_xdirpos;
    int wd_ydirpos;		/* y direct position of window */
    int m_wd_ydirpos;
    int	wd_xsiz;		/* xsize of window */
    int m_wd_xsiz;
    int wd_ysiz;		/* ysize of window */
    int m_wd_ysiz;

    int	*wd_xposp;		/* pointer to x position of window */
    int	*wd_yposp;		/* pointer to y position of window */

    int wd_xcurpos;		/* current position of window- only valid */
	int wd_ycurpos;     /* when window is on */

    int *wd_xcheckp;	/* pointer to value window is checking */
	int *wd_ycheckp;    /* (or testing) */

    int	wd_xflag;		/* flag bit to set in eyeflags when checked */
    int	wd_yflag;	 	/* value is in this window */

    struct wd_struct *wd_for;	/* forward pointer to next active window */
    struct wd_struct *wd_back;	/* backward pointer to previous active wind */
};
typedef struct wd_struct WD;

/* 
 * Defines for wd_flag.
 */
#define WDI_NEWSIZ	01	/* cursor has changed size (e.g. the window
				   cursor) */
#define WDI_ON		02	/* cursor is on and being plotted for wind
				   display */
#define WDI_SWOFF	04	/* request to turn off cursor when next
				   plotted */
#define WDI_OLDINVALID	010	/* old cursor add is invalid; dont erase it
				   when plotting new cursor */
#define WDI_CHANGE	020	/* when set, cursor was changed during
				   window display update */
#define WDI_SWON	040	/* request to turn on cursor */


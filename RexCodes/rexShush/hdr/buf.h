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

#include <pthread.h>
#include <sys/neutrino.h>

#include "pci.h"

/*
 *	buf.h:  Shared data buffers, A and E file formats.
 */
#define LARGE_BUFS		/* define if you have 16MB of memory;
				   undef if you have 4MB of memory */

typedef short int	    snt;
typedef unsigned short int  u_snt;

/*
 *	Buffer size defines.  These definitions are critical.  Note that
 * raw_block and int_block MUST be <= 65536 bytes each.  This is the maximum
 * size segment that can be currently allocated under QNX running in 16 bit
 * protected mode.  RAWSIZE and EBUFNUM are set to largest number possible
 * without exceeding this limit.
 * 	In addition, EBUFNUM must meet the additional restriction of
 * being defined so that bevent[] is an integral multiple of 512 bytes.
 */
#define SA_MAXSIG	16	/* max number of stored signals in ADATACD
				   (-112) A-file records; includes a/d channel
				   and signals from incore memory variables */
#define SA_AD_INT_RATE_MIN  1000    /* minimum a/d interrupt rate- the a/d
				       interrupt rate must currently be at
				       least 1msec;  max rate is determined
				       by particular a/d card */
#define SA_SPREAD_LOG2	3	/* log base 2 of sampling rate spread (spread
				   is ratio of highest sampling rate to lowest
				   sampling rate) e.g. 2^3 = 8 would
			           result in ratio of max to min of 8; for
				   a 1000Hz max rate and a spread of 8 the min
				   rate would be 1000/8 or 125Hz */
#define SA_MAX_MFRAME_CNT 32	/* max number of frames in a master frame */
#define SA_MAXCAL	20	/* max number of a/d calibrations */
#define EVSIZE		8	/* size of an event in bytes;  cannot be
				   changed */
#define EV_BLOCK	(512/EVSIZE)	/* events per block */

#ifdef LARGE_BUFS
#define EBUFNUM		2048000	/* size of event buffer in EVENTs;
				   EBUFNUM*EVSIZE must be integral
				   multiple of 512 */
//#define EDUMPINC	256000	/* high water mark for dumping ev buf in
//				   EVENTS;  chosen to be on a block boundary */
#define EDUMPINC	1024	/* high water mark for dumping ev buf in
				   EVENTS;  chosen to be on a block boundary */
#define RAWSIZE		8192000	/* size of analog buffer area in DATUMs */
#else
#define EBUFNUM		7680	/* size of event buffer in EVENTs;
				   EBUFNUM*EVSIZE must be integral
				   multiple of 512 */
#define EDUMPINC	1024	/* high water mark for dumping ev buf in
				   EVENTS;  chosen to be on a block boundary */
#define RAWSIZE		31000	/* size of analog buffer area in DATUMs */
#endif

#define	ADUMPINC	8000	/* High water mark for dumping analog
				   records in DATUMS.  Long A-file records
				   are written in pieces termed
				   continuation records.  This provides
				   chance for other disk accesses (such as
				   writing the E-file) to occur
				   while a data keeping window is open.
				   Continuation records will be app. ADUMPINC
				   datums in size.  Note that A-file
				   dumping is done in multiples of 512 byte
				   blocks for efficiency; this
				   number is just a high water mark and
				   doesn't determine size of writes. */
#define NUMWST		25	/* number of entries in analog write stack */

/*
 * The type of DATUM is defined in sys.h.  Changing this from 'int' is
 * not guarenteed to work properly- it has never been tested.  The analog
 * header, currently 20 bytes, must be an integral multiple of DATUMs for
 * Afile disk writing to work properly (write sizes are constrained to be
 * multiples of 512 bytes except for the last write of a window).  An
 * integral number of DATUMs must also fit in 512 bytes.
 */
#define AHDRSZ_IN_DATUMS	((sizeof(ANALOGHDR)+(sizeof(DATUM)-1)) / \
						sizeof(DATUM))
#define DAT_P_BLOCK		(512 / sizeof(DATUM))

/*
 *	MAGIC is a long bit pattern placed in analog headers in
 * A-file.  It is unique by either a byte or word search!
 *
 *	The bytes appear in the file in the order:
 *	MAGIC_B1, MAGIC_B2, MAGIC_B3, MAGIC_B4.
 *	It as absolutely necessary they be defined as character
 *	constants.
 *	The words appear in the file in the order:
 *	MAGIC_W1, MAGIC_W2
 */
#define MAGIC		1210832817L
#define MAGIC_B1	'\053'
#define MAGIC_B2	'\110'
#define MAGIC_B3	'\261'
#define MAGIC_B4	'\327'
#define MAGIC_W1	0044053
#define MAGIC_W2	0153661

/*
 *	Info about currently open data files.  One
 * is allocated to the current E file and A file.
 */
typedef struct {
	int filedes;	/* file descriptor for this channel */
	long fkey;	/* file offset key */
} CHANNEL;

/*
 *	All data generated by REX is stored in only two types of files:
 * the E (Event) file and the A (Analog) file.  They are closely related-
 * the A file is indexed by the E file.  Information is also incorporated
 * into the files to verify their integrity and aid in recovery
 * if corrupted.
 *	The E file is a fixed record length file composed of events
 * with the following structure.  An event has a fixed length of 8
 * bytes.
 * 
 * 	Note:  The event buffering/saving depends on the sizeof an event 
 * evenly dividing 512 bytes.  The sizeof and event must be 8 bytes and
 * cannot be easily changed.
 */
typedef struct {
	u_snt	e_seqnum;	/* event sequence number in E file */
	snt	e_code;		/* event code; see event.h */
	long	e_key;		/* a long time; or if this event references
				   an analog record in the A file, a long
				   address */
} EVENT;
typedef EVENT * EVENT_P;

/*
 *	The A file is composed of variable length records each
 * preceeded by an instance of this structure.  All A file records
 * are indexed by an E file event.
 * Information to verify file integrity is placed in the A file
 * and E file.  Both have record sequence numbers;  the A file also
 * has a long magic number to search for to locate headers in case
 * of damage.  (See documentation on file consistency checkers for
 * further info).
*/
typedef struct {
	long	amagic;		/* magic number for bit search */
	u_snt	aseqnum;	/* A file record sequence number */
	snt	acode;			/* event code for this record */
	long	atime;		/* time of start of data record; note: this
						 * field is null for continuation records */
	long	acount;		/* zero if this is not a continuation record;
						 * increments for each successive
						 * continution record in current record */
	snt	acontinue;		/* flag: 1 if record is a continuation
						 * record */
	u_snt	alength;	/* byte count of following data */
} ANALOGHDR;

/*
 *	Sample header.  Contains info about sampled signals and sampling rates.
 * This struct is initialized by the int process before the clock is begun,
 * and a copy is placed in the shared memory area and written into the A-file.
 * Once an A-file is opened, this struct cannot be changed until the file
 * is closed.
 *
 *	Some terminology:  REX stores multiple 'signals' in the A-file.
 * A signal may have various sources:  the a/d converter, an incore memory
 * variable, etc.  Signals are identified by an integer signal number from
 * 0 to SA_MAXSIG-1.  Each signal also includes the name of any associated
 * global memory variable, and an ascii 'title' that can be specified by
 * the user.  Signals are composed of multiple 'samples' of type 'DATUM'.
 *
 *	REX stores up to SA_MAXSIG signals in the ADATACD (-112) type
 * analog file records.  The sampling rates can be independently set for
 * each signal.  Possible rates range from the maximum sample rate supported
 * by the particular a/d to lower rates obtained from dividing down the
 * max rate by 2.
 *
 *	Data is stored in ADATACD (-112) type analog records in logical
 * groupings termed "frames".  Each frame is composed of "subframes",
 * the number of which depends on the ratio of the max to min sampling rate
 * (the sampling 'spread').  As an example, suppose the following signals
 * were being stored witha max sample rate of 1KHz, and a minimum sample
 * rate of 125Hz:
 *
 *	sig_0, rate 500Hz
 *	sig_1, rate 125Hz
 *	sig_2, rate 500Hz
 *	sig_3, rate 1KHz
 *	sig_6, rate 250Hz
 *	sig_7, rate 125Hz
 *	
 *	The ratio of max sampling rate to min sampling rate
 * (the sampling 'spread') is 1000/125 or 8.  Therefore, there are 8
 * subframes as follows:
 *
 *	subframe 0: sig_0, sig_1, sig_2, sig_3, sig_6, sig_7
 *	subframe 1: sig_3
 *	subframe 2: sig_0, sig_2, sig_3
 *	subframe 3: sig_3
 *	subframe 4: sig_0, sig_2, sig_3, sig_6
 *	subframe 5: sig_3
 *	subframe 6: sig_0, sig_2, sig_3
 *	subframe 7: sig_3
 *
 *	Note that a subframe may be empty!  If sig_3 were being sampled
 * at 500Hz, the odd subframes above would be empty.  Within a subframe,
 * signals are guaranteed to be stored in order from lowest to highest.
 *
 *	The Rex A-file record header currently supports storing times only
 * at 1msec resolution.  However, data may be sampled at rates higher than
 * 1msec (1Khz).  Rex must therefore guarantee that the data in an A-file
 * record actually begin at 1msec boundaries.  A problem arises at
 * higher sampling rates because frame boundaries might not be at 1msec
 * intervals (e.g., assume an sample rate of 5kHz, with 8 subframes to
 * a frame;  frame duration is 8 * 200us= 1.6msec).  Therefore, a
 * 'master frame' is composed of the smallest number of 'frames' such that
 * the master frame duration is an integral number of msecs.  In the example
 * just given, the master frame would be composed of 5 frames
 * (5 * 1.6usec= 8msec).
 *
 *	Rex supports 16 bit a/d converters.  Therefore, there is no room
 * in a 16 bit DATUM to include a signal identifier.  In order to successfully
 * unpack analog records composed of multiple signals sampled at independent
 * rates, Rex guarantees the following constraint- an A-file analog record
 * starts on a *master* frame boundary.  Note,
 * however, continuation records can begin anywhere (even inside a subframe),
 * and the last record is not guaranteed to end on a master frame boundary.
 */
/*
 * Don't change following.
 */
#define SA_SUBFRNUM	(01 << SA_SPREAD_LOG2)	    /* number of subframes */

/*
 * Max size of sa_frame array.  Each subframe 'row' (see below) has
 * SA_MAXSIG possible entries, plus two extra- one for the count of
 * samples stored, the other for the termination value.  In addition, one
 * more entry is added to the end of the array to provide an array termination
 * value, and a last entry to make array size an even number of bytes
 * (SA_SUBFRNUM should always be even).
 */
#define SA_FR_ARRAY_SZ	(((SA_MAXSIG+2) * SA_SUBFRNUM)+2)

/*
 * This struct must be read by data analysis programs on other architectures.
 * In order to make unpacking possible, the struct is composed of only
 * short ints and arrays of short ints or chars.  Note that pointers
 * should not be placed in this struct- this greatly complicates
 * unpacking on other architectures.
 */
typedef struct {
	snt sa_maxsig;	    /* max number of possible signals */
	snt sa_fr_array_sz; /* size of sa_frame array */
	snt sa_maxcal;	    /* max number of calibrations */
	snt sa_lname;	    /* length of name string */
	snt sa_signum;	    /* actual number of signals */
	snt sa_maxrate;	    /* max sample rate */
	snt sa_minrate;	    /* min sample rate */
	snt sa_subfr_num;   /* number of subframes in a frame */
	snt sa_mfr_num;	    /* number of frames in a master frame */
	snt sa_mfr_dur;	    /* duration of master frame in msec */
	snt sa_fr_sa_cnt;   /* number of 'stored' signals in a frame */
	snt sa_mfr_sa_cnt;  /* number of 'stored' signals in a master frame */
	snt sa_ad_channels; /* number of channels on a/d converter */
	snt sa_ad_res;	    /* a/d resolution in bits, usually 12 or 16 */
	snt sa_ad_rcomp;    /* radix compensation- amount to subtract from
			       a sample to convert to 2's complement */
	snt sa_ad_ov_gain;  /* if a/d does not have a channel-gain list */
	snt sa_datumsz;	    /* size of a sample datum in bytes */

	/*
	 * From this point on, items in struct have variable size
	 * depending on defined constants.  The following entries
	 * are byte offsets from the variable 'sa_var_data_begin' to
	 * the variable size arrays that follow.  These offsets are
	 * used by analysis programs to locate the arrays.
	 */
	snt sa_ad_rate_bo;
	snt sa_store_rate_bo;
	snt sa_ad_calib_bo;
	snt sa_shift_bo;
	snt sa_gain_bo;
	snt sa_ad_delay_bo;
	snt sa_ad_chan_bo;
	snt sa_frame_bo;
	snt sa_gvname_bo;
	snt sa_title_bo;
	snt sa_calibst_bo;
	snt sa_var_data_begin;	/* marker for beginning of variable data
				   for use by anaylsis programs */
#ifdef __QNX__
	snt sa_ad_rate[SA_MAXSIG];   /* a/d acquisition rate for each signal */
	snt sa_store_rate[SA_MAXSIG];/* A-file store rate for each signal */
	snt sa_ad_calib[SA_MAXSIG];  /* a/d calibration number for each sig */
	snt sa_shift[SA_MAXSIG]; /* shift factor per physical signal to
				    convert to Rex internal calibration of
				    40st/deg */
	snt sa_gain[SA_MAXSIG];	 /* gains per channel if a/d has a channel-
				    gain list;  will be NULLI otherwise */
	snt sa_ad_delay[SA_MAXSIG]; /* filter delay in msec */
	snt sa_ad_chan[SA_MAXSIG];  /* physical a/d channel for each a/d
				       type signal */

	/*
	 * The 'sa_frame' array is organized as follows:
	 *
	 * Subframe0: #_of_stored_samples, samp_spec, samp_spec, ... -1
	 * Subframe1: #_of_stored_samples, samp_spec, samp_spec, ... -1
	 *	    ...
	 * SubframeN: #_of_stored_samples, samp_spec, samp_spec, ... -1
	 * -2 terminates array.
	 *
	 * There is one terminated 'row' for each subframe.  The first
	 * entry in the row is the number of samples that are to be 'stored'
	 * in the A-file buffer for this frame.  Note that a sample can
	 * be 'acquired' and 'stored'.  An 'acquired' sample is one that is
	 * acquired by the a/d converter (or possibly some other means)
	 * and placed in a global variable for use by REX internally, e.g.
	 * the saccade detector.  A 'stored' sample is one that is not only
	 * 'acquired' but also 'stored' in the A-file.  REX supports individual
	 * sampling rates for both 'acquired' and 'stored' samples, with the
	 * restriction that the 'acquired' rate is at least as high as the
	 * 'stored' rate.  The 'samp_spec' gives the signal
	 * number in the low byte, and various flags in the high byte.
	 * The entire array of subframe rows is terminated with -1.  Note
	 * that a row for a subframe that specifies no samples to be taken
	 * would still have two entries: 0, 0.  Note also that this array
	 * is densely packed- rows vary in size depending on the number
	 * of samples that are to be read or stored for that subframe.
	 */
#endif
#define SA_ACQUIRE_ONLY	0x4000  /* this sample is 'acquired', but not 'stored';
				   never use bit 15- these flags must not cause
				   value to be negative! */
#define SA_MEMSIG	0x2000	/* the source of this sample is not an a/d
				   converter channel, but an incore variable */
#define SA_SIGMASK	0x00ff  /* signal identifier stored in low byte */
#define SA_SUBFR_TERM	-1	/* row termination */
#define SA_ARRAY_TERM	-2	/* array termination */
#ifdef __QNX__
	snt sa_frame[SA_FR_ARRAY_SZ];
	char sa_gvname[SA_MAXSIG][P_LNAME]; /* name of global variable
						corresponding to each sig */
	char sa_title[SA_MAXSIG][P_LNAME];  /* title specified by
					        user for each signal */
	char sa_calibst[SA_MAXCAL][P_LNAME];/* asci strings for each
					       calibration factor */
#endif
} SAMP;
extern SAMP samp;

/*
 *	Header for REX interrupt process.  Only one interrupt
 * process can be running at a time.
 */
typedef struct {
	int i_correct;	/* number of correct trials */
	int i_wrong;	/* number of wrong trials */
	char i_name[P_LFNAME];	/* A, E file name without A, E extensions */
#ifdef NIH_ACUC
	long i_lastCorrect;
#endif
} INTHDR;

/*
 *	REX has two data areas sharable by all REX processes.
 * The INT_BLOCK contains the circular event buffer and
 * all inter-process communication and syncronization info.  The
 * RAW_BLOCK contains the circular analog buffers. 
 */
typedef struct {
	PROCTBL	ptbl[P_NPROC];		/* process table */
	RL	rl;						/* running line data */
	RT_DISP rtDisp[RT_DISP_LEN];/* real time display data */
	WD wd[WD_MAXNUM];			/* array of window structs */
	WD wd_last;
	WD wd_first;
	int rtDspPshDwnCntr;		/* real time display stack is increment counter */
	int eyeHoff;				/* eye horizontal offset */
	int eyeVoff;				/* eye vertical offset */
	int oEyeHoff;				/* other eye horizontal offset */
	int oEyeVoff;				/* other eye vertical offset */
	WIN_DATCURS cursTyp;		/* types of window display cursors */
	char i_rxver[P_LVERSION];	/* Rex version number */
	long i_time;				/* system time in msec */
	int	i_ptblx;				/* passed to new REX process to let it know it's ptbl index */

	/* Indices to ptbl for main REX procs;
	 * loaded by the process when it is set to
	 * run state.  Cannot be pointers since would
	 * differ for each process.  Comm is always
	 * ptbl[0].  The indices for photon processes
	 * have a ph_ prefix
	 */
	int int_pi;
	int	scrb_pi;
	int ph_switch_pi;				/* proc table index for photon process switcher */
	int ph_int_pi;					/* proc table index for photon int process */
	int	ph_rline_pi;				/* proc table index for photon running line display process */
	int	ph_ewind_pi;				/* proc table index for photon window display process */
	int	ph_ras_pi;					/* proc table index for photon raster process */
	int ph_xcorr_pi;				/* proc table index for photon xcorr process */
	int	ph_test_pi;
	pthread_mutex_t i_mutex;  		/* int process switching mutex */

	u_int	c_flags;				/* system wide flags for comm process */
	u_int	i_flags;				/* system wide flags for int process */
	u_int	d_flags;				/* system wide flags for display process */
	int	d_rwakecd;					/* wake up display to update rasters when
									 * this ecode is loaded by state processor */
	u_int	d_rl_disp;				/* running line disp flags */
	u_int	d_wd_disp;				/* window disp flags */
	u_int	d_mwd_disp;				/* menu window disp flags */
	int	d_emag;						/* window disp eye cursors magnification */
	int	d_omag;						/* window disp offset cursors magnification */
	int t_wrate;					/* window check rate  */
	int w_rate;						/* window display rate */
	int d_tails;					/* eyemovement tails flag */
	int	d_ras_fg_color;				/* foreground color of raster displays */
	int	d_ras_bg_color;				/* background color of raster displays */
	int	d_win_fg_color;				/* foreground color of win displays */
	int	d_win_bg_color;				/* background color of win-rl displays */
	int	i_nindex;					/* name index of proc's noun or menu
									 * table sent from comm on noun, menu */
	int	i_nuser;					/* true if index is for a user noun or menu */
	char i_verbs[P_ISLEN];			/* verb string of noun, verb */
	char i_args[P_ISLEN];			/* argument string of command */
	int	i_rtflag;					/* root control flag */
	int	i_rtcx;						/* root file current read index */
	long i_rtseekp; 				/* root file seek pointer to end of block
									 * currently in buffer */
	int	i_rtcln;					/* current line number of root file */
	int i_rtappend;         		/* whether to append or over write root file */
	char i_rtname[P_ISLEN];			/* name of root file */
	/*	char i_rtmenus[P_ISLEN];	 names of menus to dump when writing
									 * subset of a root file */
	char i_rtbuf[512];				/* root file read input buffer */
	char i_errstr[P_ISLEN];			/* error string for rxerr() */
	char i_userstr[P_LUSER];		/* user status string */
	int	i_errptx;					/* proc tbl index for proc generating error */
	u_int	evlx;					/* load index for event buf;  points to next
									 * location to load */
	u_int	evdx;					/* points to next location to be dumped */
	u_int	evdump;					/* event dump index;  when evlx == evdump
									 * scribe will dump event buffer */
	u_int	evdumpinc;				/* the amount to increment the dump index */
	u_int	seqnum_a;				/* A-file sequence number */
	u_int	seqnum_e;				/* E-file sequence number */
	pid_t	int_msg_pid;			/* if non-zero, pid of process to deliver
									 * S_ALERT signal;  requested by int routine */
	pid_t	data_proxy;				/* proxy registered by data process */
#ifdef PLEX_ECODES
	int	pl_hipri[PL_MAXCODES];		/* hi priority ecode buffer area */
	int	pl_lopri[PL_MAXCODES];		/* lo priority ecode buffer area */
	int	pl_hilx, pl_hidx; 			/* load and dump indices */
	int	pl_lolx, pl_lodx;
#endif	
   	PCI_DEV	i_PciDev[PCI_DEV_MAX];	/* results of pci_attach_device() */
	int	i_PciDiscoveryDone;			/* flag is true when PCI discovery has 
									 * been done, and all one-time init functions called */
   	int	i_prim_dio_base;			/* copy of primary dio base address needed by procs other than int */
	EVENT	rev;					/* remote event storage */
	EVENT	i_iev;					/* initial event from BEHAVE */
	INTHDR	inthdr;					/* interrupt header */
	ANALOGHDR ahdr;					/* analog header for input file */
	EVENT bevent[EBUFNUM];			/* actual event buffer */
} INT_BLOCK;

/*
 *	Write to disk requests for scribe process are queued with this
 * struct.  For REX V1 through V4.1 there are two incore buffers rotating
 * in sync (and two only).  These have ecodes -110 and -111.  Each may have
 * more than one channel stored in them, usually divided into horizontal
 * data in the -110 buffer and vertical in the -111 buffer.  One wstack
 * entry causes both buffers to be written to disk.
 *
 *	Starting with REX V4.2 there is only one incore buffer containing
 * samples from all signals.  A-file records have been given a new ecode
 * of -112 to distinguish them from those written by previous versions of REX.
 */
typedef struct {
	int	ws_flag;	/* type of entry */
	int	ws_code;	/* ecode for this entry */
	int	ws_cont;	/* continuation count */
	long	ws_acount;	/* acount */
	long	ws_time;	/* time for this write */
	int	ws_lowx;	/* low and high indexes of write */
	int	ws_highx;
} WSTACK;

/*
 * Bits for ws_flag.
 */
#define WS_OPEN		01	/* a window has been opened;  ws_cont if non-
				   zero contains an adjustment to be added to
				   r_fillcnt for gaps, w_pre backups in analog
				   buffer */
#define WS_WRITE	02	/* entry specifies a write */


/*
 *	Structure of RAW_BLOCK.
 */
typedef struct {
	int	r_rawlx;	/* load index for cir analog buffers;  points
				   to next location to load */
	int	r_cendx;	/* current end index of rotating buffer */
	int	r_wrapx;	/* target wrap index */
	int	r_bufendx;	/* absolute end of rotating buffer */
	int	r_fillcnt;	/* current undumped count of buffer;  used
				   to check for buffer overflow */
	int	r_wslx;		/* load index of write stack;  points to next
				   location to load */
	int	r_wsdx;		/* dump index;  points to next location to
				   dump */
	int	r_sam_swtch;	/* controls sampling;  when set to MININT,
				   sampling is disabled */
	int	r_bufinc;	/* index distance between same index location in
				   consecutive raw buffers;  takes into account
				   space for AHDR between buffers */
	SAMP	r_samp;		/* sample header */
	WSTACK	wstack[NUMWST];	/* write stack */
	char guard[2*sizeof(ANALOGHDR)];/* scribe plops AHDRS inline with data
					    when writing to disk;
					    see dumpa() (2 used for safety,
					    only 1 is needed) */
	DATUM raw[RAWSIZE];
} RAW_BLOCK;

/*
 *	Extern definitions used system wide.
 */
typedef volatile INT_BLOCK * INT_BLOCK_P;
typedef volatile RAW_BLOCK * RAW_BLOCK_P;
extern INT_BLOCK_P i_b;
extern RAW_BLOCK_P r_b;

/*
 * Function prototypes.
 */
void ldevent(EVENT *evp);
#ifdef PLEX_ECODES
void ldevent_plexon_low(EVENT *evp, int flag);
#endif

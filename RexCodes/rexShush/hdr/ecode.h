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
 *	System wide ecode defines.  Paradigm specific ecode definitions
 * should be kept in a header file in rex/sset.
 *
 *	The following ecode mappings exist:
 * ecode < 0:	Event indexes an entry in A file. (e.g. eye movement)
 * ecode > 0:	Event is simple epoch. (e.g. unit firing)
 *	Bits 14 and 13 are special bits used as masks.
 * In addition, all events with absolute value less than 1500 are
 * reserved for system use.  Therefore, the permissable range
 * of user event codes currently is:
 *	 1501 to  8191 for events that are simple epochs
 */

/*
 *-----------------------------------------------------------------------*
 *		Ecode mask bits.
 *-----------------------------------------------------------------------*
 */
#define INIT_MASK	020000	/* When set, the lower bits contain the
				   paradigm number (this is specified in
				   the Spot file with keyword 'id');  this
				   code is stored whenever a 'reset statelist'
				   command is executed.  'disp' uses this
				   code to decide how far back to display
				   units */
#define CANCEL_MASK	040000	/* can be set when an error condition
				   aborts a trial; currently only used
				   in definition of 'ERR1CD' */

/*
 *-----------------------------------------------------------------------*
 *		Reserved negative Codes:  Index A file
 *-----------------------------------------------------------------------*
 */
#define STDHDRCD	-1	/* pre REX 4.2: standard header code */
#define SAMPHDRCD	-3	/* sample header code */
#define EYEHCD		-110	/* pre REX 4.2: horizontal eye */
#define EYEVCD		-111	/* pre REX 4.2: vertical eye */
#define ADATACD		-112	/* combined a/d data records of version 4.2 */

/*
 *-----------------------------------------------------------------------*
 *		Reserved positive codes.
 *-----------------------------------------------------------------------*
 */
#define SACONHCD	1	/* horiz saccade onset */
#define SACONVCD	2
#define ESTOPCD		10	/* someone stopped the event writing out */

/*
 *	Spot calibration codes.
 */
#define TARGETCD	510	/* target jump: location and time */
#define EHCALCD		520	/* calibration point for horizontal eye */
#define EVCALCD		521	/* calibration point for vertical eye */

/*
 *	Single unit codes.
 */
#define UNIT1CD		601	/* channel 1 units */
#define UNIT2CD		602
#define UNIT3CD		603
#define UNIT4CD		604
#define UNIT5CD		605
#define UNIT6CD		606

#ifdef MEXWEX_UNITS
#define MEX1_BASE_CODE	610	/* MEX units start with this base code */
#endif

/*
 *	Window manipulation codes.
 */
#define WOPENCD		800
#define WCLOSECD	801
#define WCANCELCD	802
#define WERRCD		803	/* error aborted current window */
#define UWOPENCD	804	/* unit window */
#define UWCLOSECD	805
#define WOPENERRCD	806	/* enetered on attempt to open a window while
				   window is already open, e.g. during post
				   time */

/*
 *	Standard reserved codes.
 */
#define ENABLECD	1001
#define PAUSECD		1003	/* paradigm stop code */
#define STARTCD		1005	/* trial has started */
#define LISTDONECD	1035	/* ramp list has completed */
#define RDONECD		1037	/* ramp over */
#define RTDONECD	1039	/* ramp timing sequence completed */
#define ERR1CD		(ENABLECD|CANCEL_MASK)
#define PHIONCD		1125	/* phi blink on */
#define PHISTARTCD	1127	/* phi sequence started */
#define PHIENDCD	1129	/* phi sequence ended */


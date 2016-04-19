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
 modified cb Jun 13, 1994 - addition of T_LOCK and backward compatibility
 */

/*
*	time = 0 synchronization code
*/
#define	T_LOCK		3232

/*
 *	Event codes specific per laboratory.
 */
#define FPONCD		1010
#define FPONBLINKCD	1012
#define FPOFFBLINKCD	1014
#define	DISP_STEP	1015
#define	RSIMSAC		1016
#define FPDIMCD		1020
#define FPOFFCD		1025
#define REWCD		1030
#define STIMCD		1100
#define STOFFCD		1101
#define STONBLINKCD	1120	/* stim blink */
#define STOFFBLINKCD	1122	/* stim blink off */
#define PHIONCD		1125	/* phi blink on */
#define PHISTARTCD	1127	/* phi sequence started */
#define PHIENDCD	1129	/* phi sequence ended */
#define BASECD		1500	/* Code that can be modified by an
				   action return.  Leave with 300 consecutive
				   unused numbers.  */

/*
 *	Paradigm init codes.
 */
#define EP_1	101		/* fix pt with stim on */
#define EP_2	102		/* fix pt with ramp stim */
#define EP_3	103		/* ramp fix pt with stationary stim */

#define E_D0	2000		/* ramp direction series */
#define E_D45	2001
#define E_D90	2002
#define E_D135	2003
#define E_D180	2004
#define E_D225	2005
#define E_D270	2006
#define E_D315	2007

#define E_VT5	2031
#define E_VT10	2032
#define E_VT15	2033
#define E_VT25	2034
#define E_VT40	2035

#define E_VT2	2040
#define E_VT4   2041
#define E_VT8   2042
#define E_VT16  2043
#define E_VT32  2044
#define E_VT64  2045
#define E_VT128 2046

#define E_VS5	2050		/* velocity stimulation series */
#define E_VS10	2051
#define E_VS15	2052
#define E_VS25	2053
#define E_VS50	2054
#define E_VS100	2055
#define E_VS200	2056
#define E_VS400 2057

#define E_VS1	2070
#define E_VS2	2071
#define E_VS4   2072
#define E_VS8   2073
#define E_VS16  2074
#define E_VS32  2075
#define E_VS64  2076
#define E_VS128 2077

/* LED on & off codes */
#define LED0OFF	2800
#define LED0ON	2801
#define LED1OFF	2802
#define LED1ON	2803
#define LED2OFF	2804
#define LED2ON	2806
#define LED3OFF	2809
#define LED3ON	2810
#define LIGHTS1OFF	2820
#define LIGHTS1ON	2821
#define LIGHTS2OFF	2830
#define LIGHTS2ON	2831

/* Ramp codes		*/
#define RAMPBEG		2501
#define RAMPEND		2502

#define TRIGGERON	2511
#define TRIGGEROFF	2512

/* Saccade codes	*/
#define SACBEG		2601
#define SACEND		2602


/*
 * Stabilization codes.  The code is determined by ORing the
 * base code, STBONCD or STBOFFCD with the stabilization flag
 * values, STB_H, STB_V, STB_B = 01, 02, 03.
 */
#define STBONCD		2048
#define STBOFFCD	2064

/*
 * Acuity answer codes
 *	ANSWERCD | 01 = yes, ANSWERCD = no
 */
#define ANSWERCD	2100

/*
 * when the mirror is flicked to blank the image, this code goes out
 */
#define BLANKCD		2150

/* local paradigm EVENT codes */
#define TARGONCD	3000
#define TARGOFFCD	3002
#define TARGON1CD	3100
#define TARGON2CD	3101
#define TARG1OFF	3102
#define TARG2OFF	3103
#define BRANCHCD	2300
#define ONSTABCD	2100
#define ONSLIPCD	2400
#define ONSTPCD		2500
#define ONRMPCD		2600
#define OFFSTABC	2101
#define DARK		2700
#define OKNSTART	2702
#define OKNSAT		2703
#define OKAN		2704

/*
 * defines for rampflags
 */
#define RS_ENDLST	01
#define RS_CDONE	02
#define RS_ADONE	04
#define RS_MONITOR	010
#define RS_TDONE	020
#define RS_SAC		040
#define RS_STAB		0100
#define RS_OFF		0200
#define RS_SLIP		0400
#define RS_STP		01000
#define RS_RMP		02000
#define RS_RAMP		04000

/*
 * RASHBASS TABLE
 *		Contains fixation position, ramp length, polar
 *	coordinates for ramp, and step dimensions for generalized
 *	step-ramp tracking paradigm.
 */
typedef struct {
	int rs_xw, rs_yw;	/* eye window dimensions, in 10ths of deg */
	int rs_vel, rs_ang;	/* polar velocity, in deg/sec and deg */
	int rs_xoff, rs_yoff;	/* step dimensions */
	int rs_ecode;		/* event code for this stimulus */
	int rs_good;		/* number of good trials */
	int rs_count;		/* number of successful trials */
} RASHTBL ;

typedef struct {
	int rs_xw;		/* eye window dimensions, in 10ths of deg */
	int rs_len;		/* 1/2 total ramp length in 10ths of deg */
	int rs_vel, rs_ang;	/* polar velocity, in deg/sec and deg */
	int rs_xoff, rs_yoff;	/* step dimensions */
	int rs_ecode;		/* event code for this stimulus */
	int rs_good;		/* number of good trials */
	int rs_count;		/* number of successful trials */
} RAMPTBL ;

/*Condition used in branches in paradigms*/

#define MET	1

/*
 *Paradigm Ecodes
 */
#define EYINWD 	1006
#define SACMAD	1007
#define PURSCD  1008
#define BCKDRK	1026
#define BCKLIT	1027
#define TGBLCD	1028
#define TGBLOF	1029
#define TARGON	1040
#define TARGOF	1041
#define STABON	1050
#define STABOF	1051
#define TTGBLC	1052
#define TSTABO	1053
#define TSTIMC	1054
#define TTRGON  1055
#define TFPNBK  1056
#define TVDIS	1057
#define TVOFF	1058
#define ELESTM	1059
#define TVACC	1060
#define BCKNOIS	1061
#define TELESTM	1062
#define ELEOFF  1063

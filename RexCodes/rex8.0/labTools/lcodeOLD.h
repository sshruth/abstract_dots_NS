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

#ifndef _LCODE_H_
#define _LCODE_H_

/* index codes for parameters.  This allows all parameters
** to use the same ecode space 7000-7999
** but to be identified uniquely by the immediately
** preceding 8000 ecode.
*/

#define HEADCD			8000	/* marks beginning of a series
							 	 * and should be followed by a series
							 	 * of index 8000+n ECODE pairs.
							 	 */
#define I_FIXXCD		8001	/* fix x */
#define I_FIXYCD		8002	/* fix y */
#define I_STXCD		    8003	/* stim center x */
#define I_STYCD		    8004	/* stim center y */
#define I_STDIACD		8005	/* stim (aperture) diameter */
#define I_SPDCD		    8006	/* speed */
#define I_VNVCD		    8007	/* var novar */
#define I_TRG1XCD		8008	/* target iposition X */
#define I_TRG1YCD		8009	/* targ position Y */
#define I_DOTDIRCD	    8010	/* pref dir */
#define I_COHCD		    8011	/* correlation */
#define I_TRG2XCD		8012	/* target 2 X */
#define I_TRG2YCD		8013	/* target 2 Y */
#define I_SEEDCD		8014	/* seed range */
#define CHMADECD		8015	/* code to indicate choice was made */
#define EYEHLDCD		8016	/* code to indicate eye was held in wind. */
#define I_TRIALIDCD	    8017	/* trial id number */
#define I_STIMDUR		8018	/* stimulus duration, new to para 502  */
#define I_DWCD			8019	/* DataWaves interrupt codes */
#define I_QXCD			8020	/* Attention Q */
#define I_QYCD			8021
#define I_GOXCD		    8022	/* Go target */
#define I_GOYCD		    8023
#define I_GOLUMCD		8024    /* luminance of Go target */
#define I_TRG1LCD		8025	/* Target 1 luminance */
#define I_TRG2LCD		8026	/* Target 2 luminance */
#define I_SEEDBASECD	8027	/* seed_base 	*/
#define I_SEEDVARCD	    8028	/* seed_var		*/
#define I_FPJERKXCD		8051	/*RK: the following codes are for FP and TA displacement*/
#define I_FPJERKYCD		8052	
#define I_TAJERKXCD		8053
#define I_TAJERKYCD		8054
#define I_PLSSIZE		8055
#define I_PLSDUR		8056
#define I_TRG3XCD		8057 	/*RK: ts*/
#define I_TRG3YCD		8058
#define I_TRG3LCD		8059

#define I_REWSIZE_COR	8060	/*RK: size of reward for the correct and sure targets*/
#define I_REWSIZE_SBET	8061

#define I_ELESTMCD		8031	/* Marks the beginning of electrical stimulation */
#define I_ELESTMSTCD	8062	/* RK: Marks the end of electrical stimulation */
#define I_CORRTARGCD	8034

/* special event codes for dot paradigms */
#define GORANDCD	      4900		/* turn on dots */
#define GOCOHCD 	      4901		/* turn on coherence */
#define ENDCD		      4902		/* turn off dots */
#define ALLONCD	          4903
#define ALLOFFCD	      4904
#define CORRECTCD	      4905
#define WRONGCD           4906
#define NOCHCD		      4907
#define REW1CD		      4908
#define LOOPCD		      4909
#define PREFCD		      4910
#define NULLCD		      4911
#define SBETCD		      4953	/* sure bet is chosen */	/*RK: ts*/
#define SBETSHOWCD	      4954	/* sure bet is shown on the trial */	/* ts */
#define FIXBREAKCD        4955    /* broken eye fixation */
#define FIXHANDBREAKCD    4956    /* broken hand fixation */


/* not sure what this was at stanford... but at UW it is... */
#define ASKOFFCD	 4912
#define FIX1CD 	     4913  /* added for Matt, para 512 */
#define BIGREWCD 	 4914  /* added for Matt, para 512 */ 
#define CHGFPCD 	 4915  /* added for Jamie, para 602 */
#define QONCD		 4916  /* added for Mark, para 422 */
#define QOFFCD		 4917  /*  same */
#define GOTRGONCD    4918  /*  same */
#define TRGACQUIRECD 4919  /*  same */
#define QACQUIRECD   4920
#define GOTRGOFFCD   4921
#define DTMVBGNCD	  4922	/* added for Josh, para 702exp */
#define DTMVENDCD	  4923	/* added for Josh, para 702exp */
#define ACCEPTCAL     4924  /* added by JD, para 730 */
#define LASTCD        4925      /* Added by MM for 65* series of paras */
#define FPJERKCD	  4950	/* RK: to jerk FP around */
#define TAJERKCD	  4951	/* RK: to jerk TA around */


/*
*	time = 0 synchronization code
*/
#define	T_LOCK		3232

/*
 *	Event codes specific per laboratory.
 */
#define FPONCD		    1010
#define FPONBLINKCD	    1012
#define FPOFFBLINKCD	1014
#define	DISP_STEP	    1015
#define	RSIMSAC		    1016
#define FPDIMCD		    1020
#define FPOFFCD		    1025
#define REWCD		    1030
#define STIMCD		    1100
#define STOFFCD		    1101
#define PLSONCD		    1102	/* RK: added for the pulse experiment */
#define PLSOFFCD	    1103	/* RK: added for the pulse experiment */
#define STONBLINKCD	    1120	/* stim blink */
#define STOFFBLINKCD	1122	/* stim blink off */
#define PHIONCD		    1125	/* phi blink on */
#define PHISTARTCD	    1127	/* phi sequence started */
#define PHIENDCD	    1129	/* phi sequence ended */
#define BASECD		    1500	/* Code that can be modified by an
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
#define TARGONCD 	3000
#define TARGC1CD	3001
#define TARGC2CD	3002
#define TARGC3CD	3003
#define TARGC4CD	3004

/* OLD CODES 
#define TARGONCD	3000
#define TARGOFFCD	3002
#define TARGON1CD	3100
#define TARGON2CD	3101
#define TARG1OFF	3102
#define TARG2OFF	3103
#define BRANCHCD	2300
#define ONSTABCD	2100
#define ONSLIPCD	2400
#define ONSTPCD	2500
#define ONRMPCD	2600
#define OFFSTABC	2101
#define DARK		2700
#define OKNSTART	2702
#define OKNSAT		2703
#define OKAN		2704
*/

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

#endif

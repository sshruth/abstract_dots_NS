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

#define I_FIX2XCD       8063    /* For hand fixation spot */
#define I_FIX2YCD       8064    /* For hand fixation spot */
#define I_MONITORDISTCD 8065    /* Monitor distance to calculate degrees to cm */

/* Codes to store extent of acceptance regions (eye and hand) */

#define I_EFIX_ACCEPTHX  8066
#define I_EFIX_ACCEPTHY  8067
#define I_ETARG_ACCEPTHX 8068
#define I_ETARG_ACCEPTHY 8069

#define I_HFIX_ACCEPTHX  8070
#define I_HFIX_ACCEPTHY  8071
#define I_HFIX_ACCEPTHZ  8072
#define I_HTARG_ACCEPTHX 8073
#define I_HTARG_ACCEPTHY 8074
#define I_HTARG_ACCEPTHZ 8075

/* Fix spot and target diameters */
#define I_EFIXDIAMCD     8076
#define I_HFIXDIAMCD     8077
#define I_TRG1DIAMCD     8078
#define I_TRG2DIAMCD     8079

#define I_MAXDOTSDUR     8080   /* max dots duration in RT dots task */

#define I_RESPONSE       8091   /* LAST ONE!!! */


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
#define FIXBREAKCD        4955  /* broken eye fixation */
#define FIXHANDBREAKCD    4956  /* broken hand fixation */
#define EFIXACQ           4957  /* eye fixation acquired */
#define HFIXACQ           4958  /* hand fixation acquired */


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


/* local paradigm EVENT codes */
#define TARGONCD 	3000
#define TARGC1CD	3001
#define TARGC2CD	3002
#define TARGC3CD	3003
#define TARGC4CD	3004

#define TARGC1OFFCD	3005

#define EYINWD 	    1006
#define SACMADCD	1007
#define REACHMADCD  1008
#define ELESTM	    1059

#endif

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

 *	Event codes specific per laboratory.

 */

/* spot file code; 9000 series */

#define MASK			9000

#define FIXSPOT			9001

#define SEARCH			9002

#define LUMIN			9003

#define LANDOLTC		9004



/* computer controlled ecodes; 1000 series */

#define TRLSTARTCD		1000

#define ENABLECD		1001

#define VIDBEGIN		1002

#define PAUSECD			1003

#define FPONCD			1004

#define FPDIMCD			1005

#define CUECD			1005

#define FPOFFCD			1006

#define ISCATCHCD		1007

#define TARGONCD		1008

#define TARGOFFCD		1009

#define MASKONCD		1010

#define MASKOFFCD		1011

#define ALLOFFCD 		1012

#define ABORTCD			1013

#define ERRCD			1014

#define REWCD			1015

#define EOT				1016

#define HOLDTIME		1017

#define CORRECTCD		1018

#define ISNOGOCD		1019

#define ISRELEASECD		1019

#define GAPTASKCD		1020

#define OVERLAPTASKCD	1021

#define FLASHTARGCD		1022

#define RTTASKCD		1023



#define SEARCHONCD		1010	/* used in LandoltC.d to indicate when search array appears; same codes a maskon maskoff */

#define SEARCHOFFCD		1011



/* electrical stimulation ecodes; 1100 series */

#define ZAPONCD			1100

#define ZAPOFFCD		1101



/* behavioral ecodes; 2000 series */

#define TOUCHCD			2000

#define RELEASECD		2001

#define LEVERTURN		2002

#define	LEVINWD			2003

#define FIXATECD		2004

#define SACMAD			2005

#define EYINWD 			2006

#define CORRCTVSAC		2007

#define BROKEFIX		2008





/* performance score; 3000 series */

#define PERFBASE		3000

/* trialtype definitions for scoring */

#define HIT 0				/* Hits, correct */

#define MISLOC 1			/* Mislocalized target trials, error */

#define FA 2				/* False alarms, error */

#define MISS 3				/* Misses, error */

#define CR 4				/* Correct Rejections, correct */



/*Valid trial = target or mask appeared; Invalid trial = trial abort before target or mask appeared or timeout */

#define VALID			6000

#define INVALID 		6666



/* unknown video event */

#define UNKNOWN			9999





/*

 * Information Codes, can have up to 22.  Plexon accepts 15 bits = 32768.

 */

#define CODECOUNT		18	/* number of information codes */

#define INFOBASE		10000	/* infos base code	*/

#define TASK_NUM		0000	/* 100s = tasklevel; 1s = num (number of stimulus locations)*/

#define ECCEN 			1000

#define SHIFT			2000

#define PCTCATCH		3000

#define FX_STM1_STM2	4000	/*0 or 1; 100s = fixation cue; 10s = target; 1s =  mask (or 2nd stim) */

#define TPOS			5000

#define SOA_MIN_MAX		6000	/*100s = currentsoa; 10s = minsoa; 1s = maxsoa*/

#define STEP_TYPE		7000	/*10s = step amount; 1s = steptype */

#define TRLSPERSOA		8000

#define LEVER			9000

#define TARGX			10000

#define TARGY			11000

#define TARGR			12000

#define TARGG			13000

#define TARGB			14000

#define FIXX			15000

#define FIXY			16000

#define FIXSIZE			17000

#define DISTR			18000

#define DISTG			19000

#define DISTB			20000

#define LUM_MIN			21000

#define TODDPOS			22000	/* location of oddball stimulus in LandoltC.d TL2 */

#define XYOFFSET	 	500


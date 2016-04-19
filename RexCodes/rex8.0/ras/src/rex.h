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
 * REX.H -- functions for REX A- and E-file data unpacking
 *
 * HISTORY
 *	24feb93	LMO	Create
 * $Log: rex.h,v $
 * Revision 1.8  1999/09/28 18:34:24  lmo
 * add comment to adRate
 *
 * Revision 1.7  1996/03/17 19:20:55  lmo
 * rexunit support
 *
 * Revision 1.6  1995/11/02  16:26:28  lmo
 * allow up to 32 unit codes
 *
 * Revision 1.5  1993/04/07  18:26:33  lmo
 * -112 codes
 *
 * Revision 1.4  1993/04/03  22:16:26  lmo
 * samp header
 *
 * Revision 1.3  1993/03/05  14:23:02  lmo
 * include continue flag in AIX structure
 *
 * Revision 1.2  1993/03/03  23:41:00  lmo
 * fix event error macro
 *
 * Revision 1.1  1993/02/25  22:54:24  lmo
 * Initial revision
 *
 */

#ifndef __REX_H__
#define __REX_H__

/*
 * include necessary rex headers
 */
#ifdef GOO
#include "sys.h"
#include "proc.h"
#include "buf.h"
#include "ecode.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define PRIVATE	static

#define rexNchannels	3
#define rexMaxSignals	32
#define rexMaxUnits	99	/* units = 601 to 699 */

#ifndef INIT_MASK
#define INIT_MASK	020000
#define CANCEL_MASK	040000
#endif

#define TIMECHECK   1000	/* time in msec for check of drastic
				   reduction in event times */

/*
 * REX -110 and -111 channel masks for h- and v-buffers
 */
#define MASK_N		4
#define MASK_0		000000
#define MASK_1		010000
#define MASK_2		020000
#define MASK_3		040000
#define MASK_12		007777
#define MASK_ALL	(MASK_1 | MASK_2 | MASK_3)

/*
 * macros
 */
#define rexSignalRound(z)	((int)((z) + 0.5))	/* rounding for Signal type */
#define rexSignalLoop(S)	for (S = signalList; S; S = S->next)

/*
 * Rex Tools Types
 */

typedef struct unitinfo {
	long *nTimes;		/* number of times for each unit */
	long **unitTimes;	/* time of occurrence for each spike */
}  RexUnits;

/*
 * trial list structure
 */
typedef struct {
	int events;		/* index of start code event */
	int nEvents;		/* number of events in this trial */

	RexUnits rexunits;	/* rex units when split out */

	long tStartTime;	/* time of first event in trial */
	long tEndTime;		/* time of last event in trial */

	int recNum;		/* corresponding analog records */
	long aStartTime;	/* time of first point in signal */
	long aEndTime;		/* time of last point in signal */
} Trial;

typedef short int RexSignal;

typedef struct siglist {
	RexSignal *signal;	/* pointer to Signal array */
	int npts;		/* number of points in signal */
	int count;		/* number of this signal */
	int index;		/* signal array index for this signal */
	int sigChan;		/* rex channel number */
	char *sigName;		/* name of signal channel */
	char *sigLabel;		/* label for signal */
	int adRate;		/* a/d rate in Hz of this signal, even after interpolation */
	int storeRate;		/* REX store rate in Hz */
	float fscale;		/* a/d fullscale */
	float scale;		/* takes a/d levels to fullscale */
	int sigGain;		/* rex gain table index */
	struct siglist *next;	/* pointer to next element in list */
} SignalList;

typedef struct rexinfo {
	int nSignals;		/* number of signals */
	SignalList *signalList;	/* pointer to list of signals */
	int nPoints;		/* max number of points in signals */
	int ad_res;		/* a/d resolution in bits */
	long aStartTime;	/* time of first point in signal */
	long aEndTime;		/* time of last point in signal */

	EVENT *events;		/* pointer to event array for this trial */
	int nEvents;		/* number in event array for this trial*/

	RexUnits *rexunits;	/* rex unit structure for this trial if splitting events */
	int nUnits;		/* number of different units between 601 and 699 */
	int *unitCodes;		/* codes for each unit */

	long tStartTime;	/* time of first point in trial */
	long tEndTime;		/* time of last point in trial */

	int numTrials;		/* number of trial periods */
	int numRec;		/* number of analog records */
	char *ehdr;		/* E-file header */
	char *ahdr;		/* A-file header */
	int maxSampRate;	/* maximum sample rate of any stored signal */
	int startCode;		/* trial start code */
} RexInfo;

/*
 * analog record index structure
 */
typedef struct {
	long key;	/* A-file offset */
	int loEv;	/* event buffer pointer */
	int nEv;	/* number of related events */
	long strtTime;	/* time of record start */
	long endTime;	/* approximate time of record end */
	int cont;	/* continue flag */
	int more;	/* more records coming */
	int strt;	/* start record for this count */
} AIX;


typedef struct {
	char hi;
	char lo;
} * BYTEP;

typedef struct {
	short int hiword;
	short int loword;
} *WORDP;

/*
 * macros  * * * * * * * * * * * * * * * * * * * * * * * *
 */
#define error_M  \
	{err= 1;\
	 fprintf(outstrm, "\n[1m-- ERROR: event %lu -- [0m", evnum);\
	}


/*
 * PUBLIC FUNCTION PROTOTYPES  * * * * * * * * * * * * * * * * * * * * * * * *
 */

/* rex signal functions */
void rexSplitEvents(void);
int rexFileOpen(int maxSampRate, int startCode, int preTime);
RexInfo *rexGetTrial(int trialNumber, int interpolateFlag);
RexInfo *rexGetEvents(int trialNum);
RexInfo *rexGetAnalog(int interpFlag);
RexInfo *rexGetSignals(int recordNumber, int interpolateFlag);
int rexGetUnitCount(int unitCode);
int rexGetAnalogHeader(EVENT *ev, ANALOGHDR *ahdr);
void rexSetAWinCodes(int openCd, int closeCd, int cancelCd);

/* printing functions */
char *rexToolsVersion(void);
char *rexTimeConv(long time);
void rexTimePrint(long time, FILE *strm);
void rexHeaderPrint(FILE *strm);
void rexTotalsPrint(FILE *strm);
void rexEprint(EVENT *ev, FILE *strm);
void rexAprint(EVENT *ev, FILE *strm, int color);
void rexInfoPrint(RexInfo *ri, FILE *strm);
void rexSampHdrPrint(FILE *strm);
#ifdef __cplusplus
}
#endif
#endif /* !__REX_H__ */

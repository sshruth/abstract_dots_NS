/* Header "ph_ras.h" for ras Application */

#ifndef __PH_RAS_H_INCLUDED
#define __PH_RAS_H_INCLUDED

#ifndef __PT_BASIC_H_INCLUDED
#include <photon/PtBasic.h>
#endif

#ifndef __REX_H_INCLUDED
#define __REX_H_INCLUDED
#include "rexhdrs.h"
#include "tty.h"
#include "ecode.h"
#endif

#include "sd_est.h"
#include "lsrTig.h"
#include "raster.h"
#include "rex.h"
#include "dex.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * If the following is defined, then signal handling is compatable with Photon.
 * This means that Photon functions can be called from the signal handler.
 * Photon accomplishes this by catching the signal first, and then calling the
 * signal handler from Photon's main loop.  Defining this removes some of
 * the REX 'alert()' functionality, however-  when processing a noun,verb preemption
 * by another message isnt possible.
 */
#define PHOTON_SIG_COMPAT

#define ROOT_FILE_READ 1
#define ROOT_FILE_WRITE 2
#define RASTER_FILE_SAVE 3

#define ECODE_LIST_MAX 32
#define MAX_OR_CODES 16
#define MAX_CLASSES 1024
#define BLOCK 500
#define TRIAL_BLOCK 25
#define S_IGS 1
#define S_IGD 2

/* This structure holds the beginning and end of an ecode range */
typedef struct {
	int beg;
	int end;
	int list[MAX_OR_CODES];
} ANYOFRANGE;

/* This structure holds the ecodes that determine which trials will
 * be selected for analysis, and which ecodes will be used to
 * align trials.
 */
typedef struct {
	int nAllOf;			/* number of inclusive ecodes */
	int allOfList[ECODE_LIST_MAX];	/* List of ecodes which must all be present */
	int nNonOf;			/* number of exclusive ecodes */
	int nonOfList[ECODE_LIST_MAX];	/* List of ecodes which must not be present */
	int nAnyOf;			/* number of ranges */
	ANYOFRANGE anyOfList[ECODE_LIST_MAX];	/* List of ranges of ecodes to search for */
} SELECT;

/* This structure holds the main and Markov selection structures */
typedef struct {
	SELECT select;			/* pointer to main selection set */
	int alignCode;			/* ecode on which to align data */
	int alignOffsetTime;		/* data alignment offset */
} SELECTSET;

/* This structure holds the index into the event file */
typedef struct {
	int new;		/* flag to indicate that this is a new trial */
	int copy;		/* flag to indicate that di is a copy */
	int setNum;		/* number of the selection set */
	int c_code;		/* rex event code defining the stimulus condition */
	int trlNum;		/* rex trial number */
	long algnTime;		/* time stamp for aligning data */
	DexInfo *di;		/* pointer to rex information structure, modified for dex */
} TRIALS;

/* This structure holds the spike density function and spike trains for a class of trials */
typedef struct {
	int npts;		/* number of points in the spike density functions */
	int **ppSpikeTimes;	/* array of spike times for each trial in the class */
	int *pSpikeCounts;	/* array of spike counts for each trial in the class */
	SD_EST spkDen;		/* structure to hold the spike density function */
} SPKDEN;

/* This structure holds the classes of selected trials */
typedef struct {
	int new;		/* count of the number of new trials in this class */
	int setNum;		/* selection set number */
	int c_code;		/* e_code defining experimental condition */
	int first;		/* classes's first element in the dataSet array */
	int last;		/* classes's last element in the dataSet array */
	int nCcodes;		/* number of codes (trials) in this class */
	int maxCcodes;		/* maximum number of codes in this class */
	int classStart;		/* largest interval between alignment time and start time for this class of trials */
	int classEnd;		/* largest interval between alignment time and end time for this class of trials */
	int nTimes;		/* largest number of units in this class */
	int maxTimes;		/* maximum number of units in this class */
	int maxPts;		/* maximum number of points in the spike density functions */
	SPKDEN spikeDen;	/* array to hold the spike densities for this class */
} CLASSES;

/* This structure holds the selection sets */
typedef struct {
	SELECTSET *selectSets;		/* pointer to selection set */
	int nSelectSets;		/* number of selections sets defined */
	int newSelects;			/* flag indicating new select sets are being defined */
	int curSetIndx;			/* index of the current selection set */
	TRIALS *dataSet;		/* pointer to data set */
	int nTrials;			/* total number of trials in the data set */
	CLASSES *classSet;		/* pointer to class set */
	int nClasses;			/* total number of classes */
	int maxClasses;			/* maximum number of classes */
	int maxTrials;			/* maximum number of trials in a class */
	int maxSampRate;		/* maximum REX analog sampling rate */
	int preTime;			/* REX pre time */
	int startCode;			/* ecode indicating start of trial */
	int curTrial;			/* trial being drawn */
	int iallRas;			/* flag inidicating re-initialization of rasters */
} SELECTIONS;

#ifdef __cplusplus
};
#endif
#endif /* PH_RAS_H */

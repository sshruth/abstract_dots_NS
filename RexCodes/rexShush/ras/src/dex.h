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
#ifndef DEX_H
#define DEX_H
typedef unsigned short int uShort;

#define ONEMEGABYTE 1048576
#define MISS_TIME 32000
typedef struct dexinfo {
	int nEvents;		/* number in event array for this trial */
	EVENT *events;		/* pointer to event array for this trial */
	int nUnits;		/* number of different units between 601 and 699 */
	int *unitCodes;		/* codes for each unit */
	RexUnits *rexunits;	/* rex unit structure for this trial if splitting events */
	long tStartTime;	/* time of first point in trial */
	long tEndTime;		/* time of last point in trial */
	char *ehdr;		/* E-file header */
	int numTrials;		/* number of trial periods */
	int startCode;		/* trial start code */
} DexInfo;

/* rex signal functions */
void rexSplitEvents(void);
int rexFileOpen(int maxSampRate, int startCode, int preTime);
void rexFileClose(void);
RexInfo *rexGetTrial(int trialNumber, int interpolateFlag);
RexInfo *rexGetEvents(int trialNum);
RexInfo *rexGetAnalog(int interpFlag);
RexInfo *rexGetSignals(int recordNumber, int interpolateFlag);
int rexGetUnitCount(int unitCode);

#endif /* DEX_H */

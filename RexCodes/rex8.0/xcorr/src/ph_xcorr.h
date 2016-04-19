/* Header "ph_xcorr.h" for xcorr Application */

#ifndef __PH_XCORR_H_INCLUDED
#define __PH_XCORR_H_INCLUDED

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

#define MAX_LINES 200
#define THIN 1
#define THICK 10
#define CLSFILE		0
#define CLSFLTR		0
#define MARGINS		100

/* make typdefs for tig structures to make defining easier */
typedef struct TIGvec TIGVEC;
typedef struct darray DARRAY;
typedef struct space TIGSPACE;
typedef struct cinfo CINFO;
typedef struct TIGIO TIGIO;

/* this structure holds the definition of the graph label */
typedef struct Dlbl {
	char label[128];	/* text of label */
	double x;		/* relative X coordinate of label */
	double y;		/* relative Y coordinate of label */
	int sz;			/* size of the text */
	int font;		/* font for text */
} LABEL;

/* This structure holds the definition of a raster graph */
typedef struct Rgrph {
	int start;		/* start of data display relative to alignment mark (msec) */
	int interval;	/* amount of data to plot (msec) */
	int maxFreq;	/* maximum expected unit firing rate */
	int timeMark;	/* interval between X axis tick marks in msec */
	int Nfont;		/* font to use for the numbers */
	int Nsz;		/* size of the numbers */
	char xFormat[16];
} GRAPH;

#ifdef __cplusplus
};
#endif

#endif /* PH_XCORR_H_INCLUDED */

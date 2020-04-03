/* Header "ph_rline.h" for rline Application */

#ifndef __PH_RLINE_H_INCLUDED
#define __PH_RLINE_H_INCLUDED

#ifndef __PT_BASIC_H_INCLUDED
#include <photon/PtBasic.h>
#endif

#ifndef __REX_H_INCLUDED
#define __REX_H_INCLUDED
#include "rexhdrs.h"
#include "tty.h"
#endif
 
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

#define Pt_ARG_OSCOPE_RECT			Pt_RESOURCE( Pt_USER(1), 0)
#define Pt_ARG_OSCOPE_DATA			Pt_RESOURCE( Pt_USER(1), 1)
#define Pt_ARG_OSCOPE_TIME_BASE		Pt_RESOURCE( Pt_USER(1), 2)
#define Pt_ARG_OSCOPE_SCALE			Pt_RESOURCE( Pt_USER(1), 3)
#define Pt_ARG_OSCOPE_TRIGGER		Pt_RESOURCE( Pt_USER(1), 4)
#define Pt_ARG_OSCOPE_TRIG_LEVEL	Pt_RESOURCE( Pt_USER(1), 5)
#define Pt_ARG_OSCOPE_DISPLAY		Pt_RESOURCE( Pt_USER(1), 6)
#define Pt_ARG_OSCOPE_DISP_RESET	Pt_RESOURCE( Pt_USER(1), 7)
#define Pt_ARG_OSCOPE_TRACE_COLORS	Pt_RESOURCE( Pt_USER(1), 8)

#define MAX_TRACES       8
#define MAX_COLORS      32
#define MAX_SIGNALS     32
#define MAX_LABEL_LEN   20
#define ERASE_WIDTH     3
#define LINE_THICK      1
#define RL_BAR_INDX		RL_MAXCHAN
#define RL_SPK_INDX		RL_MAXCHAN + 1
#define RL_TIME_INTERVAL 6
typedef struct {
	int rl_on;
	int rl_scale;
	int rl_offset;
} TRACE;

/* oscilloscope widget instance structure */
typedef struct oscilloscope_widget {
	PgColor_t   fill_color;
	PgColor_t   color;
	PgColor_t	traceColors[MAX_COLORS];
	PhRect_t	canvas;
	PhArea_t	area;
	PhPoint_t	drawStart;
	PhPoint_t	drawEnd;
	unsigned	n_data;
	short int	timeBase;
	short int	scaleFactor;
	short int	trigger;
	short int	triggerLevel;
	short int	display;
	short int	modeValue;
	short int   mode;
	short int	dispReset;
	short int	min;
	short int	max;
	short int	range;
	short int	eraseWidth;
	short int	timeCounter;
	short int	timeStamp;
	short int	timeInterval;
} lsrRunLine;

#ifdef __cplusplus
};
#endif
#endif

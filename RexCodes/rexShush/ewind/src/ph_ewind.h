/* Header "ph_ewind.h" for ewind Application */

#ifndef __PH_EWIND_H_INCLUDED
#define __PH_EWIND_H_INCLUDED

#ifndef __PT_BASIC_H_INCLUDED
#include <photon/PtBasic.h>
#endif

#ifndef __REX_H_INCLUDED
#define __REX_H_INCLUDED
#include "pcsSocket.h"
#include "rexhdrs.h"
#include "tty.h"
#endif

#include "PtScale.h"
#include "timerFunction.h"

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

#define MAXCHARS             256
#define MAX_COLORS		      32
#define DFLT_EYE_MAG	       1
#define NUM_SCALES		       6
#define NUM_TICKS		      11
#define WD_ON			       1
#define WD_OFF			       0
#define MAX_LABEL_LEN	      20
#define GLUT_DOWN              0
#define GLUT_UP                1
#define GLUT_KEY_F1            1
#define GLUT_KEY_F2            2
#define GLUT_KEY_F3            3
#define GLUT_KEY_F4            4
#define GLUT_KEY_F5            5
#define GLUT_KEY_F6            6
#define GLUT_KEY_F7            7
#define GLUT_KEY_F8            8
#define GLUT_KEY_F9            9
#define GLUT_KEY_F10          10
#define GLUT_KEY_F11          11
#define GLUT_KEY_F12          12
#define GLUT_KEY_LEFT        100
#define GLUT_KEY_UP          101
#define GLUT_KEY_RIGHT       102
#define GLUT_KEY_DOWN        103
#define GLUT_KEY_PAGE_UP     104
#define GLUT_KEY_PAGE_DOWN   105
#define GLUT_KEY_HOME        106
#define GLUT_KEY_END         107
#define GLUT_KEY_INSERT		 108

enum cursorType {
	CU_BOX,
	CU_DIAMOND,
	CU_OCTAGON,
	CU_JOY,
	CU_UTRIANGLE,
	CU_DTRIANGLE,
	CU_LTRIANGLE,
	CU_RTRIANGLE,
	CU_X,
	CU_PLUS,
	CU_STAR,
	CU_EYEWIN,
	CU_NONE
};

typedef struct cursDes {
	int on;
	int type;
} CURS_DES;

typedef struct eyeWindow {
	int index;
	PhDim_t dim;
	PhPoint_t bnd[4];	/* 4 points of the corners of the window */
} WND_SIZ;

/* eye window widget instance structure */
typedef struct dispWindow {
	PgColor_t   fill_color;
	PgColor_t   color;
	PgColor_t	traceColors[MAX_COLORS];
	PhPoint_t	cursPos[RT_DISP_LEN][WDI_NCURS];
	PhRect_t	canvas;
	PhArea_t	area;
	PhPoint_t   lastPoint;
	WND_SIZ		windSiz[WD_MAXNUM];
	CURS_DES	cursors[WDI_NCURS];
	unsigned	n_data;
	unsigned	refreshRate;
	unsigned	grid;
	short int	scaleFactor;
	short int	trigger;
	short int	triggerLevel;
	short int	autoRefresh;
	short int 	autoInterval;
	short int	refresh;
	short int	numCurs;
	short int	eyeMin;
	short int	eyeMax;
	short int	eyeRange;
	short int	refreshCounter;
	short int	intervalCounter;
} lsrDispWindow;

#ifdef __cplusplus
};
#endif
#endif

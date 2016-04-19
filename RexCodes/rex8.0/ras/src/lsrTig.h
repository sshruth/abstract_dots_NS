/* lsrTig.h */

#ifndef __PT_TIG_H_INCLUDED
#define __PT_TIG_H_INCLUDED

#include "../../hdr/tig.h"
#include "../../hdr/tigio.h"

#ifdef __cplusplus
extern "C" {
#endif

#define UP		1
#define DOWN		0
#define W_MARGX		100L	/* margin as TIG pixels (.001 inch) */
#define W_MARGY		100L
#define RES		200L
#define MAXDEV_X	MAX_X
#define MAXDEV_Y	MAX_Y
#define FMAXDEV_X	((double) MAX_X)
#define FMAXDEV_Y	((double) MAX_Y)
#define MAX_ARGS	30
#define NUM_COLORS	12
#define NPAGES		1024

/* eye window widget instance structure */
typedef struct tig_widget {
	char		*tigList;		/* buffer of tig commands */
	long		tigListPtr;		/* current buffer index */
	long		lastTigListPtr;		/* last buffer index */
	long		tigListLen;		/* length of the buffer */
	long		*tigListEnd;		/* pointer to buffer end index */
	long		*lastTigListEnd;	/* last pointer to buffer end index */
	long		pageStart[NPAGES];	/* start of page */
	long		currentPage;		/* current page */
	long		lastPage;		/* number of the last page */
	int			formFeed;		/* form feed flag */
	PhPoint_t	currentPoint;		/* current location */
	PhPoint_t	lastPoint;		/* last location */
	int			pen;			/* state of the pen (up or down) */
	PgColor_t	tigColors[NUM_COLORS];	/* array of tig colors */
	PhRect_t	canvas;		/* drawing window rectangle */
	PhArea_t	area;		/* area of drawing window */
	PhRect_t	tiles[64];		/* raster drawing locations */
	int			tileCount;		/* number of tiles to draw */
	int			clear;
	double		xscale;			/* tig to photon conversion variables */
	double		yscale;
	double		xoff;
	double		yoff;
	double		xmax;
	double		ymax;
	double		wyb;
	double		wym;
} LsrTig;

#ifdef __cplusplus
};
#endif
#endif

/* 
 * PtScale contributed widget
*/
#ifndef SCALE_H
#define SCALE_H

#include <photon/PtBasic.h>

#include <photon/PhPack.h>

#ifdef __cplusplus
extern "C" {
#endif

/* widget resources */
#define Pt_ARG_SCALE_FLAGS			Pt_RESOURCE( Pt_CONTRIB(6), 0 )
#define Pt_ARG_SCALE_MAJOR_TICK_COLOR		Pt_RESOURCE( Pt_CONTRIB(6), 1 )
#define Pt_ARG_SCALE_MAJOR_TICK_DIVISION	Pt_RESOURCE( Pt_CONTRIB(6), 2 )
#define Pt_ARG_SCALE_MAJOR_TICK_LENGTH		Pt_RESOURCE( Pt_CONTRIB(6), 3 )
#define Pt_ARG_SCALE_MINOR_TICK_COLOR		Pt_RESOURCE( Pt_CONTRIB(6), 4 )
#define Pt_ARG_SCALE_MINOR_TICK_DIVISION	Pt_RESOURCE( Pt_CONTRIB(6), 5 )
#define Pt_ARG_SCALE_MINOR_TICK_LENGTH		Pt_RESOURCE( Pt_CONTRIB(6), 6 )
#define Pt_ARG_SCALE_FONT			Pt_RESOURCE( Pt_CONTRIB(6), 7 )
#define Pt_ARG_SCALE_TAGS			Pt_RESOURCE( Pt_CONTRIB(6), 8 )

/* widget instance structure */
typedef struct Pt_scale_widget{
	PtBasicWidget_t	basic;
	#ifdef __QNXNTO__
		short padding1;
	#endif
	long			scale_flags;
	PgColor_t		major_ticks_color;		// Major tics defined in terms of color,
	short			major_ticks_division;	// count and
	short			major_ticks_length;		// size
	PgColor_t		minor_ticks_color;		// Minor tics defined in terms of color,
	short			minor_ticks_division;   // count and                            
	short			minor_ticks_length;	    // size                                 
	char*			font;					// Font for major tick labels.
	char**			labels;					// Labels for major ticks
	short			labels_size;			// size of the labels array.
}	PtScaleWidget_t;

/* widget class pointer */
extern PtWidgetClassRef_t *PtScale;

/* widget flags settings	*/
#define	Pt_SCALE_HORIZONTAL		0x01
#define	Pt_SCALE_VERTICAL		0x02
#define Pt_SCALE_REVERSE		0x04
#define Pt_SCALE_DRAW_TOP_LINE	0x08
#define Pt_SCALE_LABEL			0x10

#ifdef __cplusplus
};
#endif

#include <photon/PhPackPop.h>

#endif

/* Scale.c - widget source file */

/* Include Files	*/
#include <strings.h>
#include "PtScale.h"

/* prototype declarations */
PtWidgetClass_t *CreateScaleClass( void );
static void scale_draw_horizontal( PtWidget_t *widget );
static void scale_draw_vertical( PtWidget_t *widget );
static void scale_set_major_ticks( PtWidget_t *widget, PtArg_t const *argt);

/* widget class pointer - class structure, create function */
PtWidgetClassRef_t WScale = { NULL, CreateScaleClass };
PtWidgetClassRef_t *PtScale = &WScale; 

//
// Scale defaults function
//
static void scale_dflts( PtWidget_t *widget )
{
	PtScaleWidget_t	*sb = ( PtScaleWidget_t * ) widget;
//	PtBasicWidget_t		*basic = ( PtBasicWidget_t * ) widget;

	sb->scale_flags = Pt_SCALE_DRAW_TOP_LINE | Pt_SCALE_HORIZONTAL;
	sb->major_ticks_color = Pg_BLACK;
	sb->major_ticks_division = 5;
	sb->major_ticks_length = 10;
	sb->minor_ticks_color = Pg_BLACK;
	sb->minor_ticks_division = 3;
	sb->minor_ticks_length = 5;
	sb->font = strdup( "helv12" );
	
	// Allocate storage for labels
	sb->labels_size = sb->major_ticks_division + 1;
	sb->labels = ( char ** ) calloc( sb->labels_size, sizeof( char * ) );
}
//
// Scale draw function
//
static void scale_draw( PtWidget_t *widget, PhTile_t *damage )
{
	PtScaleWidget_t	*sb = ( PtScaleWidget_t * ) widget;
//	PtBasicWidget_t		*basic = ( PtBasicWidget_t * ) widget;
	PhRect_t 			rect;
//	PhRect_t 			shadow_rect;

	PtSuperClassDraw( PtBasic, widget, damage );

	// we don't want to draw outside our canvas! So we clip.
	PtBasicWidgetCanvas( widget, &rect );
	PtClipAdd( widget, &rect );

	if( sb->scale_flags & Pt_SCALE_VERTICAL )
	{
		scale_draw_vertical( widget );
	}
	else
	{
		scale_draw_horizontal( widget );
	}

	/* remove the clipping */
	PtClipRemove();
}

static void scale_draw_vertical( PtWidget_t *widget )
{
	PtScaleWidget_t	*sb = ( PtScaleWidget_t * ) widget;
	PhRect_t 			rect;
	PhPoint_t			from;
	PhPoint_t			to;
	PhPoint_t			text_loc;
	PhPoint_t			from2;
	PhPoint_t			to2;
	short				seg_offset = 0;
	short				old_seg_offset;
	short				height;
	short				height2;
	short				text_x_offset;
	int 				i;
	int					j;
	int					length1, length2;
	int					text_flags;

	// Get the size of our canvas
	PtBasicWidgetCanvas( widget, &rect );

	// Set our text colors now (won't change)
	PgSetStrokeColor( sb->major_ticks_color );
	PgSetFont( sb->font );
	PgSetTextColor( sb->major_ticks_color );

	// Change setting if drawing in reverse
	if( sb->scale_flags & Pt_SCALE_REVERSE )
	{
		from = rect.lr;
		to.x = rect.lr.x;
		to.y = rect.ul.y;
		if( sb->scale_flags & Pt_SCALE_LABEL )
		{
			text_flags = Pg_TEXT_LEFT;
			text_x_offset = 0;
		}
		else
		{
			text_flags = Pg_TEXT_RIGHT;
			text_x_offset = -5;
		}
	}
	else
	{
		from.x = rect.ul.x;
		from.y = rect.lr.y;
		to = rect.ul;
		if( sb->scale_flags & Pt_SCALE_LABEL )
		{
			text_flags = Pg_TEXT_RIGHT;
			text_x_offset = 0;
		}
		else
		{
			text_flags = Pg_TEXT_LEFT;
			text_x_offset = 5;
		}
	}
	
	// Draw major line, if required.
	if( sb->scale_flags & Pt_SCALE_DRAW_TOP_LINE )
	{
		PgDrawLine( &from, &to ); 
	}

	// Set the lenght of major/minor ticks once (use - to reverse draw direction)
	length1 = sb->major_ticks_length;
	length2 = sb->minor_ticks_length;
	if( sb->scale_flags & Pt_SCALE_REVERSE )
	{
		length1 = -length1;
		length2 = -length2;
	}
	
	// Calculate the total height of the scale and constant drawing positions
	height = rect.lr.y - rect.ul.y;
	to.x = to.x + length1;

	// Loop, drawing major ticsk from BOTTOM to TOP	
	for( i = 0; i < ( sb->major_ticks_division + 1 ); i++ )
	{
		// calculate Y position of line and draw it (X remains constants)
		from.y = rect.lr.y - seg_offset;
		to.y = from.y;
		PgDrawLine( &from, &to );
		
		// Draw labels, if one exist for this tick
		if( sb->labels[i] != NULL )
		{
			text_loc.x = to.x + text_x_offset;
			text_loc.y = to.y; 
			if( sb->scale_flags & Pt_SCALE_LABEL )
			{
				if( sb->scale_flags & Pt_SCALE_REVERSE )
				{
					text_loc.x = rect.ul.x;
				}
				else
				{
					text_loc.x = rect.lr.x;
				}
			}
			
			// Draw text, making sure limits are drawn within the container.				
			if( i == 0 )
			{
				PgDrawText( sb->labels[i], strlen( sb->labels[i]), &text_loc, text_flags | Pg_TEXT_BOTTOM );
			}
			else if( i == sb->major_ticks_division )
			{
				PgDrawText( sb->labels[i], strlen( sb->labels[i]), &text_loc, text_flags | Pg_TEXT_TOP );
			}
			else
			{
				PgDrawText( sb->labels[i], strlen( sb->labels[i]), &text_loc, text_flags | Pg_TEXT_MIDDLE );
			}
		}
		
		// Needed for later calcs, so we do it now
		old_seg_offset = seg_offset;
		seg_offset = ( ( i + 1) * height ) /  (sb->major_ticks_division ? sb->major_ticks_division : 1);
		
		// Now do minor ticks, if required
		if( sb->minor_ticks_division > 0)
		{
			height2 = seg_offset - old_seg_offset;
			from2 = from;
			
			// Don't overwrite the top line, inc the y a bit.  This
			// should incremented by the width of the line eventually.
			if( sb->scale_flags & Pt_SCALE_REVERSE )
			{
				from2.x--;
			}
			else
			{
				from2.x++;
			}
			to2.x = from2.x + length2;
		
			// Don't need to overlap the major tick, so we loop for the 
			// internal tick marks only.
			for( j = 1; j < sb->minor_ticks_division; j++ )
			{
				from2.y = from.y + ( j * height2 ) / sb->minor_ticks_division;
				to2.y = from2.y;
				PgSetStrokeColor( sb->minor_ticks_color );
				PgDrawLine( &from2, &to2 );
			}
			// Reset stroke color to major (optimization)
			PgSetStrokeColor( sb->major_ticks_color );
		}
	}	
}

static void scale_draw_horizontal( PtWidget_t *widget )
{
	PtScaleWidget_t	*sb = ( PtScaleWidget_t * ) widget;
	PhRect_t 			rect;
	PhPoint_t			from;
	PhPoint_t			to;
	PhPoint_t			from2;
	PhPoint_t			to2;
	PhPoint_t			text_loc;
	short				seg_offset;
	short				old_seg_offset;
	short				width;
	short				width2;
	int 				i;
	int					j;
	int					length1, length2;
	int					text_flags;

	PtBasicWidgetCanvas( widget, &rect );

	// Set our text colors now (won't change)
	PgSetFont( sb->font );
	PgSetTextColor( sb->major_ticks_color );
	PgSetStrokeColor( sb->major_ticks_color );

	if( sb->scale_flags & Pt_SCALE_REVERSE )
	{	
		from.x = rect.ul.x;
		from.y = rect.lr.y;
		to = rect.lr;
		if( sb->scale_flags & Pt_SCALE_LABEL )
		{
			text_flags = Pg_TEXT_TOP;
		}
		else
		{
			text_flags = Pg_TEXT_BOTTOM;
		}
	}
	else
	{
		from = rect.ul;
		to.y = rect.ul.y;
		to.x = rect.lr.x;
		if( sb->scale_flags & Pt_SCALE_LABEL )
		{
			text_flags = Pg_TEXT_BOTTOM;
		}
		else
		{
			text_flags = Pg_TEXT_TOP;
		}
	}
	if( sb->scale_flags & Pt_SCALE_DRAW_TOP_LINE )
	{
		PgDrawLine( &from, &to ); 
	}
	
	length1 = sb->major_ticks_length;
	length2 = sb->minor_ticks_length;
	if( sb->scale_flags & Pt_SCALE_REVERSE )
	{
		length1 = -length1;
		length2 = -length2;
	}
	
	width = rect.lr.x - rect.ul.x;
	to.y = to.y + length1;
	seg_offset = 0;
	for( i = 0; i < ( sb->major_ticks_division + 1 ); i++ )
	{
		// calculate X position of line
		from.x = rect.ul.x + seg_offset;
		to.x = from.x;
		PgDrawLine( &from, &to );
		
		// Draw labels, if one exist for this tick
		if( sb->labels[i] != NULL )
		{
			text_loc = to;
			
			if( sb->scale_flags & Pt_SCALE_LABEL )
			{
				if( sb->scale_flags & Pt_SCALE_REVERSE )
				{
					text_loc.y = rect.ul.y;
				}
				else
				{
					text_loc.y = rect.lr.y;
				}
			}

			// Draw text, making sure limits are drawn within the container.				
			if( i == 0 )
			{
				PgDrawText( sb->labels[i], strlen( sb->labels[i]), &text_loc, text_flags | Pg_TEXT_LEFT );
			}
			else if( i == sb->major_ticks_division )
			{
				PgDrawText( sb->labels[i], strlen( sb->labels[i]), &text_loc, text_flags | Pg_TEXT_RIGHT );
			}
			else
			{
				PgDrawText( sb->labels[i], strlen( sb->labels[i]), &text_loc, text_flags | Pg_TEXT_CENTER );
			}
		}
		
		// Needed for later calcs, so do it now
		old_seg_offset = seg_offset;
		seg_offset = ( ( i + 1) * width ) /  ( sb->major_ticks_division ? sb->major_ticks_division : 1 );
		
		// Do minor ticks, if required
		if( sb->minor_ticks_division > 0)
		{
			width2 = seg_offset - old_seg_offset;
			from2 = from;
			
			// Don't overwrite the top line, inc the y a bit.  This
			// should incremented by the line with eventually.
			if( sb->scale_flags & Pt_SCALE_REVERSE )
			{
				from2.y--;
			}
			else
			{
				from2.y++;
			}
			
			to2.y = from2.y + length2;
		
			for( j = 1; j < sb->minor_ticks_division; j++ )
			{
				from2.x = from.x + ( j * width2 ) / sb->minor_ticks_division;
				to2.x = from2.x;
				PgSetStrokeColor( sb->minor_ticks_color );
				PgDrawLine( &from2, &to2 );
			}
			// Reset stroke color to major (optimization)
			PgSetStrokeColor( sb->major_ticks_color );
		}
	}	
}

// Set the major scale.  If needed, allocate a new temporary array
// to store
static void scale_set_major_ticks( PtWidget_t *widget, PtArg_t const *argt)
{
	PtScaleWidget_t *sc = (PtScaleWidget_t *) widget;
	char**			str_array;
	int				i;
	
	// If needed, allocate new array of pointers to store the labels into
	if( ( argt->value + 1 ) > sc->labels_size )
	{
		str_array = calloc( ( argt->value + 1 ), sizeof( char * ) );

		if( NULL != str_array )
		{
			for( i = 0; i < sc->labels_size; i++ )
			{
				str_array[i] = sc->labels[i];
			}
			
			// No longer need the old table, release the memory.
			free( sc->labels );
			
			// Set the widgets labels table to correspond to the new size.
			sc->labels = str_array;
			sc->labels_size = argt->value + 1;
		}
	}
	
	// Set the major division and mark widget damaged.
	sc->major_ticks_division = argt->value;
	PtDamageWidget( widget );

}

// Assumes if Major Tick size is changes, the old array is reallocated properly.
void scale_set_labels( PtWidget_t *widget, PtArg_t const *argt)
{
	PtScaleWidget_t *sc = (PtScaleWidget_t *) widget;
	int				i;
	int				loop;
	char**			str_array = (char**)argt->value;


	// Make sure we only set what we can
	if( argt->len > sc->labels_size )
	{
		loop = sc->labels_size;
	}
	else
	{
		loop = argt->len;
	}
	
	// Loop here, freeing/setting string as required.
	for( i = 0; i < loop; i++ )
	{
		// If label is alreay set, release storage space for it
		if( NULL != sc->labels[i] )
		{
			free( sc->labels[i] );
		}
		sc->labels[i] = strdup( str_array[i] );
	}
	PtDamageWidget( widget );

}

// int scale_get_labels( PtWidget_t const *widget, PtArg_t *argt)
int scale_get_labels( PtWidget_t *widget, PtArg_t *argt)
{
	PtScaleWidget_t	*sc = (PtScaleWidget_t *) widget;
	
	argt->value = (long)(&sc->labels);
	
	return( Pt_CONTINUE );
}

static int scale_destroy( PtWidget_t *widget )
{
	PtScaleWidget_t *sc = (PtScaleWidget_t *) widget;
	int				i;

	// Iterate the array, freeing all the resources allocated, if any
	for( i = 0; i < sc->labels_size; i++ )
	{
		if( NULL != sc->labels[i] )	
		{
			free( sc->labels[i] );
		}
	}
	
	return( Pt_CONTINUE );
}
//
// Scale class creation function
//
PtWidgetClass_t *CreateScaleClass( void )
{
	// define our resources
	static PtResourceRec_t resources[] = {
		{ Pt_ARG_SCALE_FLAGS, Pt_CHANGE_REDRAW, 0, 
			Pt_ARG_IS_FLAGS ( PtScaleWidget_t, scale_flags ), 0 },
		{ Pt_ARG_SCALE_MAJOR_TICK_COLOR, Pt_CHANGE_REDRAW, 0, 
			Pt_ARG_IS_NUMBER ( PtScaleWidget_t, major_ticks_color ) , 0},
		{ Pt_ARG_SCALE_MAJOR_TICK_DIVISION, scale_set_major_ticks, 0,
			Pt_ARG_IS_NUMBER ( PtScaleWidget_t, major_ticks_division ), 0 },
		{ Pt_ARG_SCALE_MAJOR_TICK_LENGTH, Pt_CHANGE_REDRAW, 0,
			Pt_ARG_IS_NUMBER ( PtScaleWidget_t, major_ticks_length ), 0 },
		{ Pt_ARG_SCALE_MINOR_TICK_COLOR, Pt_CHANGE_REDRAW, 0, 
			Pt_ARG_IS_NUMBER ( PtScaleWidget_t, minor_ticks_color ), 0 },
		{ Pt_ARG_SCALE_MINOR_TICK_DIVISION, Pt_CHANGE_REDRAW, 0,
			Pt_ARG_IS_NUMBER ( PtScaleWidget_t, minor_ticks_division ), 0 },
		{ Pt_ARG_SCALE_MINOR_TICK_LENGTH, Pt_CHANGE_REDRAW, 0,
			Pt_ARG_IS_NUMBER ( PtScaleWidget_t, minor_ticks_length ), 0 }, 
		{ Pt_ARG_SCALE_FONT, Pt_CHANGE_REDRAW, 0,
			Pt_ARG_IS_STRING ( PtScaleWidget_t, font ), 0 },
		{ Pt_ARG_SCALE_TAGS, scale_set_labels, scale_get_labels, 
			Pt_ARG_IS_ARRAY ( PtScaleWidget_t, labels ), 0 }
		};
#if defined( VARIANT_so )
	#define CONST_BUG
#else
	#define CONST_BUG const
#endif
	// set up our class member values
	static CONST_BUG PtArg_t args[] = {
		{ Pt_SET_VERSION, 110},
		{ Pt_SET_STATE_LEN, sizeof( PtScaleWidget_t ) },
		{ Pt_SET_DFLTS_F, (long)scale_dflts },
		{ Pt_SET_DRAW_F, (long)scale_draw },
		{ Pt_SET_DESTROY_F, (long)scale_destroy },
		{ Pt_SET_FLAGS, 0 },
		{ Pt_SET_NUM_RESOURCES, sizeof( resources ) / sizeof( resources[0] ) },
		{ Pt_SET_RESOURCES, (long)resources, sizeof( resources ) / sizeof( resources[0] ) },
		};

	// create the widget class
	return( PtScale->wclass = PtCreateWidgetClass(
		PtBasic, 0, sizeof( args )/sizeof( args[0] ), args ) );
}

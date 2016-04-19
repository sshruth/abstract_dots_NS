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
 * Macros for manipulating circular buffers.
 * These can only be called from interrupt level.
 */

/*
 * Move load index backward in circular buffer.  Note that the index cannot
 * be moved forward because the current end index is always changing.
 */
#define backlx_(newx, fromx, amount) \
	{ (newx) = (fromx) - (amount); \
	if( (newx) < 0) (newx) += r_b->r_cendx; }

/*
 * Compute length between a high and low index to circular buffer.
 */
#define len_(highx, lowx) \
	(highx >= lowx ? highx - lowx : highx + (r_b->r_cendx - lowx))

/*
 * Macro to load events into event buffer.
 * For non-interrupt level requests for event loading
 * routine "ldevent()" is used.  This routine must always
 * be run at high priority.
 */
#define int_ldevent_(pev) { \
\
	InterruptDisable();\
	i_b->bevent[i_b->evlx] = *(pev); \
\
	/* \
	 * Advance load pointer and check for overflow (evdx is valid only \
	 * when keeping E data). \
	 */ \
	if (++i_b->evlx >= EBUFNUM) i_b->evlx = 0; \
	if(i_b->evlx == i_b->evdx)	/* check for overflow */ \
			to_scrb_(SC_EVERR); \
\
	/* \
	 * Various tradeoffs have been made so that loadevents are fast and \
	 * small.  evdump rotates continuously even when not keeping E data. \
	 * It is not adjusted by current value of evdx. Given that stacking \
	 * loadevent requests is too expensive, it is necessary that a \
	 * loadevent never be blocked from loading during an interrupt. \
	 * Thus there can be no critical sections in scribe that \
	 * require protection from loadevents. \
	 */ \
	if (i_b->evlx == i_b->evdump) { \
\
		i_b->evdump += EDUMPINC;	/* set new dump index */ \
		if(i_b->evdump >= EBUFNUM) i_b->evdump -= EBUFNUM; \
\
		/* \
		 * If writing to disk, alert scribe. \
		 */ \
		if(i_b->i_flags & I_EOUT) to_scrb_(SC_EDUMP); \
	} \
	InterruptEnable();\
}

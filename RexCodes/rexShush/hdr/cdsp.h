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
 * cdsp.h: Common header for displays included by both int and disp.
 */

/*
 *	Flags for type of window display. Kept in i_b->d_wd_disp.
 */
#define D_W_EYE_X	01	/* x eye cursor is on */
#define D_W_EYE_O	02	/* o eye cursor is on */
#define D_W_EOFF	04	/* eye offset cursors are on */
#define D_W_OEOFF	010	/* other eye offset cursors are on */
#define D_W_JOY		020	/* joystick cursor on */
#define D_W_GRID	040	/* grid on */
#define D_W_FRAME	0100	/* frame on */
#define D_W_DA_DIAMOND	0200	/* diamond d/a cursor on */
#define D_W_DA_PLUS	0400	/* plus d/a cursor on */
#define D_W_ALLCUR	(D_W_EYE_X|D_W_EYE_O|D_W_EOFF|D_W_OEOFF|D_W_JOY\
			 |D_W_DA_DIAMOND|D_W_DA_PLUS)

/*
 * Flags that determine which eye windows are on
 */
#define D_W_WND0	01		/* window 0 is on */
#define D_W_WND1	02		/* window 1 is on */
#define D_W_WND2	04		/* window 2 is on */
#define D_W_WND3	010		/* window 3 is on */
#define D_W_WND4	020		/* window 4 is on */
#define D_W_WND5	040		/* window 5 is on */
#define D_W_WND6	0100	/* window 6 is on */
#define D_W_WND7	0200	/* window 7 is on */

/*
 * These constants are expressed in actual coordinates used for addressing
 * screen: offset binary, with coordinates 0,0 in lower left corner.
 * Matrox display is 512x512, VGA 640x480.
 */
#define WDI_LEFT	0	/* left coordinate of display */
#define WDI_RIGHT	639	/* right coordinate of display */
#define WDI_BOT		0	/* bottom coordinate of display */
#define WDI_TOP		479	/* top coordinate of display */
#define WDI_XWRAP	0x1ff	/* if 'mxwrap' is true, this constant
				   is anded with x coord before plotting */
#define WDI_YWRAP	0x1ff

#define WDI_HTXT	 8	/* horiz spacing for a txt char */
#define WDI_VTXT	10	/* vertical spacing for a txt char */
#define WDI_CSIZ	 8	/* width of eye cursors */

#define WDI_LAXIS	(WDI_LEFT+8)	/* left axis */
#define WDI_RAXIS	(WDI_RIGHT-40)
#define WDI_BAXIS	(WDI_BOT+8)
#define WDI_TAXIS	(WDI_TOP-27)

#define WDI_LENTIC	 5	/* number of dots for tic mark */
#define WDI_SPTIC	 3	/* number of dots to leave as space between
				   tic marks and ascii labels */

/*
 * Many values that are plotted on matrox (such as eye position) are
 * stored internally in REX in 40 st/deg, 2's complement.  This yields
 * a range of -4096 to +4097 for 204.8 deg FS.  Following shift factor
 * is used to convert from internal calibration to range of matrox.
 */
#define IC_TO_MAT	4	/* shift factor to convert internal 
				   calibration (40 st/deg) to full scale
				   range of matrox (512x512) */
#define TO_OFFB_512	255	/* converts full scale 512 to offset
					   binary */

/*
 * Following are various constants constructed from above parameters.
 */
#define WDI_hcen_(xright, xleft)	(((xright)+(xleft))>>1)
#define WDI_vcen_(ytop, ybottom)	(((ytop)+(ybottom))>>1)

#define WDI_XCEN	(WDI_hcen_(WDI_RAXIS, WDI_LAXIS))
#define WDI_YCEN	(WDI_vcen_(WDI_TAXIS, WDI_BAXIS))
#define WDI_XOFF	(WDI_XCEN-((WDI_RIGHT-WDI_LEFT)>>1))
#define WDI_YOFF	(WDI_YCEN-((WDI_TOP-WDI_BOT)>>1))

/*
 * Cursor travel limits, expressed in matrox coordinates: 0-511 range.
 */
#define WDI_CLEFT	(WDI_LAXIS-WDI_HTXT)
#define WDI_CRIGHT	(WDI_RAXIS+WDI_HTXT)
#define WDI_CBOT	(WDI_BAXIS-WDI_VTXT)
#define WDI_CTOP	(WDI_TAXIS+WDI_VTXT)

/*
 * Locations of offsets cursors, expressed in Rex internal calibration.
 */
#define WDI_ORIGHT	(WDI_RAXIS-WDI_CSIZ)
#define WDI_OTOP	(WDI_TAXIS-WDI_CSIZ)
#define WDI_OVERT	((WDI_ORIGHT+(WDI_CSIZ>>1)-WDI_XCEN) << IC_TO_MAT)
#define WDI_OHOR	((WDI_OTOP+(WDI_CSIZ>>1)-WDI_YCEN) << IC_TO_MAT)

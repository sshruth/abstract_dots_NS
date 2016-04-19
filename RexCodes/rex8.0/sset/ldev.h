/*
 *-----------------------------------------------------------------------*
 * NOTICE:  This code was developed by the US Government.  The original
 * versions, REX 1.0-3.12, were developed for the pdp11 architecture and
 * are in the public domain.  This version, REX 4.0, is a port of the
 * original version to the Intel 80x86 architecture.  This version is
 * copyright (C) 1992 by the National Institutes of Health, Laboratory
 * of Sensorimotor Research, Bldg 10 Rm 10C101, 9000 Rockville Pike,
 * Bethesda, MD, 20892, (301) 496-9375.  All rights reserved.
 *-----------------------------------------------------------------------*
 */

/*
 * This header contains the device definitions that are specific
 * to each laboratory. 
 */

/*
 *-----------------------------------------------------------------------*
 *		    Laboratory Specific Device Definitions
 *-----------------------------------------------------------------------*
 */
 
 
/* udp IP addresses and port numbers */

#define rexIP     "192.168.0.5"
#define macIP     "192.168.0.4"
#define polarisIP "192.168.1.13"
#define udpPORT   6665

/* screen limits in degrees */
#define SCR_LIM_HOR 614 
#define SCR_LIM_VER 348

/* Polaris camera screen center in mm*10 */
#define POLARISX_CENTER -50 // ~1 cm in "front" of screen
#define POLARISY_CENTER 540
#define POLARISZ_CENTER -14010

/* Capacitative touch sensor center in Polaris coordinate in mm*10 */
#define CAPSENSOR_XCENTER 0
#define CAPSENSOR_YCENTER 0
#define CAPSENSOR_ZCENTER 0

/* Distance from monk to monitor in mm*10 (for Polaris) */
#define VIEW_DIST_CM 27
#define VIEW_DIST 27

/* for Polaris conversions */
#define PI 3.1415926535897931f
#define DEG_PER_CM (2.*atan(1./(2.*VIEW_DIST_CM))*180./PI)
#define CM_PER_DEG (1./DEG_PER_CM)

/* Conversion from joystick voltages to position
 * used by int/int.c
 * if you change gain and offset you must recompile int directory to make the 
 * changes effective 
 * all values MUST be integer 
 * if both MULT and DIV are defined the actual gain will be MULT/DIV. useful for creating non-integer gain. 
 */ 
#define JOY_TRANSF			/* must be defined to use the gain and offset */
#define JOYH_OFFS 	300/*512*/
#define JOYH_MULT 	-4	    
#undef JOYH_DIV		
#define JOYV_OFFS 	300/*480*/
#define JOYV_MULT 	4
#undef JOYV_DIV 		

/* Joystick button */
#define JOYBUT		Dio_id(PCDIO_DIO, 6, 0x01) /* Luke was here */

/* Capacitative touch sensor */
#define TOUCH		Dio_id(PCDIO_DIO, 6, 0x02) /* Luke was here */

/* Photodetector */
#define PHOTODETECTOR  Dio_id(PCDIO_DIO, 6, 0x04) /* Luke was here */

/*
 * Digital output.
 */
/* port configuration,
     it seems in newsome lab, output ports are 4, 5 and 6 and input port is 1
     in Shadlen lab output ports were 2 and 5, input ports were 0 and 6
*/

#define _RIG 5

/* in case rigs are different... */
#define RIG1 1
#define RIG2 2
#define RIG3 3
#define RIG4 4
#define RIG5 5

/*
 * LED
 */
#define LED1	    Dio_id(PCDIO_DIO, 2, 0x01)
#define LED2	    Dio_id(PCDIO_DIO, 2, 0x02)
#define LED3	    Dio_id(PCDIO_DIO, 2, 0x04)
#define LED4	    Dio_id(PCDIO_DIO, 2, 0x08)

/* 
 * Lamps for Backlight in Lab-g
 */
/*
 #define BACKLT	    Dio_id(PCDIO_DIO, 5, 0x08)
 #define BACKLT3	 	Dio_id(PCDIO_DIO, 5, 0x20)
 */

/* 
 * Slide Projector
 */
/*
 #define PROJ	    Dio_id(PCDIO_DIO, 5, 0x10)
 */

/* 
 * Valve and Beeper for reward.  The second reward is a switch
 * definition that we check to go to the second reward state.
 * 6/6/96 defined DIMSWITCH as the 7th switch on port 0.  Para 502st3 
 * (and subsequent paras) use this switch to override the value of 
 * of the dim target luminance so that the target is simply turned off.  
 * This allows the experimenter to switch between overlap and memory saccades
 * by the flip of a switch
 */
#define SWITCH4 	Dio_id(PCDIO_DIO, 0, 0x80)
#define SWITCH3     Dio_id(PCDIO_DIO, 0, 0x40)
#define SWITCH1     Dio_id(PCDIO_DIO, 0, 0x10)
#define SWITCH2     Dio_id(PCDIO_DIO, 0, 0x20)
//#define TAG0SWITCH	Dio_id(PCDIO_DIO, 0, 0x10)

/* JD: All rigs use only a single bit now for reward & beep. */
#define REW	   	 	Dio_id(PCDIO_DIO, 5, 0x1)
#define BEEP	    Dio_id(PCDIO_DIO, 5, 0x1)
#define REWBEEP	  	Dio_id(PCDIO_DIO, 5, 0x1)

/* Conversion from joystick voltages to position
 * (used by /rex/int/int.c)
 * JOYH_OFFS and JOYV_OFFS (the offsets) MUST be defined
 * when JOY_TRANSF is defined; scaling is optional;
 * you can either define a multiplication (JOYH_MULT and
 * JOYV_MULT) or a division (JOYH_DIV and JOYV_DIV), but
 * you MUST use integers!
 */
#if _RIG == RIG2
#define JOY_TRANSF
#define JOYH_OFFS	1024
#define JOYV_OFFS	1024
#endif

#if _RIG == RIG4
#define JOY_TRANSF
#define JOYH_OFFS	-200
#define JOYH_DIV	2
#define JOYV_OFFS	-500
#define JOYV_DIV	2
#endif

#if _RIG == RIG5
#define JOY_TRANSF
#define JOYH_OFFS	400
#define JOYH_DIV	2
#define JOYV_OFFS	-900
#define JOYV_DIV	2
#endif

/* Joystick button */
#if _RIG == RIG5
#define JOYBUT		Dio_id(PCDIO_DIO, 6, 0x1)
#endif

/* Shadlen lab Fancy TTL  */
#define FTTL1		Dio_id(PCDIO_DIO, 4, 0x1)
#define FTTL2		Dio_id(PCDIO_DIO, 4, 0x2)
#define FTTL1_2		Dio_id(PCDIO_DIO, 4, 0x3)
#define FTTL8		Dio_id(PCDIO_DIO, 4, 0x80)
#define ALL_FTTL	Dio_id(PCDIO_DIO, 4, 0xFF)

/*
 * GRASS electrical stimulator
 * Although there is none in lab-g, the following
 * lines are added to compile fix_702.
 */
#define ELECTR1	    Dio_id(PCDIO_DIO, 4, 0x1)
#define ELECTR2	    Dio_id(PCDIO_DIO, 4, 0x2)
#define ELECTR3	    Dio_id(PCDIO_DIO, 4, 0x4)

/* DataWaves ALL BITS */
#define DWALL			 Dio_id(PCDIO_DIO, 9, 0xff)


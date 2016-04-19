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
/*
 * Digital output.
 */
#define LED1	    Dio_id(PCDIO_DIO, 6, 0x40)
#define LED1_D	    Dio_id(PCDIO_DIO, 6, 0xc0)
#define LED2	    Dio_id(PCDIO_DIO, 6, 0x10)
#define LED2_D	    Dio_id(PCDIO_DIO, 6, 0x30)
#define LED3	    Dio_id(PCDIO_DIO, 6, 0x4)
#define LED3_D	    Dio_id(PCDIO_DIO, 6, 0xc)
#define LED4	    Dio_id(PCDIO_DIO, 6, 0x1)
#define LED4_D	    Dio_id(PCDIO_DIO, 6, 0x3)
#define LED5	    Dio_id(PCDIO_DIO, 5, 0x80)
#define LED6	    Dio_id(PCDIO_DIO, 5, 0x40)
#define LED7	    Dio_id(PCDIO_DIO, 5, 0x20)
#define LED8	    Dio_id(PCDIO_DIO, 5, 0x10)
#define LED9	    Dio_id(PCDIO_DIO, 5, 0x8)
#define LED10	    Dio_id(PCDIO_DIO, 5, 0x4)
#define BACKLT	    Dio_id(PCDIO_DIO, 5, 0x1)
#define ELECTR1	    Dio_id(PCDIO_DIO, 4, 0x1)
#define ELECTR2	    Dio_id(PCDIO_DIO, 4, 0x2)
#define ELECTR3	    Dio_id(PCDIO_DIO, 4, 0x4)
#define REW	    	Dio_id(PCDIO_DIO, 2, 0x80)
#define BEEP	    Dio_id(PCDIO_DIO, 4, 0x40)
#define PROJ1	    Dio_id(PCDIO_DIO, 2, 0x20)
#define PROJ2	    Dio_id(PCDIO_DIO, 2, 0x10)


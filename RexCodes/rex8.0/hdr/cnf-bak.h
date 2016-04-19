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
 * Configuration header.
 * NOTE:  This file is also auotmatically edited by
 * running configRex.
 */

/*
 *-----------------------------------------------------------------------*
 *	Device defines:  #define to include, #undef to exclude.
 *-----------------------------------------------------------------------*
 */
/* BEGIN_USER_DEFINES -- do not delete, move, or change this line */

/*
 * PCI defines.  #define or #undef the following depending on whether devices
 * are on the PCI bus.  To add new PCI devices, one must also edit tables in int/pci.c
 * which guide the PCI bus discovery process.  See comments in hdr/pci.h for more
 * info about this.
 */

/*
 * Defines for PCI bus devices.
 */
#define  PCI_AtoD	/* define if a/d on PCI bus */
#undef  PCI_DtoA	/* define if primary d/a on PCI bus */
#define  PCI_DIO	/* define if primary digital i/o on PCI bus */

/*
 * Defines for ISA devices.
 */
#undef NIATMIOX	/* if a/d not on PCI bus: select National Instruments ATMIO16X 16 bit */ 
#undef  ANDAS12		/* if a/d not on PCI bus: select Analogics ANDAS12 */
#define AD_BASE 0x240	/* ISA a/d base address */ 
#define AD_VECT 10	/* ISA a/d interrupt vector */ 
#define CIO_DA		/* Computer boards (now Measurement Computing) ISA d/a */
#define PRIM_ISA_DA_BASE 0x180	/* base address of primary ISA d/a */
#define PCDIO		/* Kontron PCDIO120 ISA digital i/o */
#undef  KMPIO12		/* Keithly-Metrabyte digitial i/o */
#define PRIM_ISA_DIO_BASE 0x280	/* base address of primary ISA digital i/o */

/*
 * Misc defines.
 */
#define DIO_TRUE_LOW	/* #define if a low voltage on digital input should be interpreted as a 1
						 * and if a 1 sent to a digital out should produce a low voltage
						 * this is the default for using the Grayhill panels
						 * #undef if a low voltage on digital input should be interpreted as a 0
						 * and if a 1 sent to a digital out should produce a high voltage */
					   
#undef  DIN_8		/* define if using only 8 bits of digital input on PCDIO
					 * board port 0, otherwise 16 bits assumed */
#define NIH_ACUC		 /* enforce the use of the score action to track performance */
#define MAX_TIME	7200000	/* maximum time with out a score or correct trial in msec */
#undef  LATCH_UNITS	/* single units on latch inputs */ 
#define MEXWEX_UNITS	/* receive MEX or WEX units */
#define MEX62_UNITS		/* receive new mex units */
#define MEXWEX_PORT 3   /* digital i/o port on PCDIO card used for MEX/WEX 
						 * communication, range 2-4 */
#define MEX62_PORT 3	/* digital i/o port on PCDIO card used for MOMENTICS MEX
						 * communication, range 2-4 */
#undef  TC_UNITS	/* units acquired with 9513 timer chips from Plexon box */ 
#define TC_NUM 8	/* number of 9513 chips in use.  Each chip can input 5 unit channels,
					 * there are 4 chips per card, max 2 cards possible */
#define TC_CARD0_BASE 0x340 /* base address of counter timer card 0; note PCI version is not
							 * detected plug and play- you must put it's base address here */
#define TC_CARD1_BASE 0x348 /* base address of counter timer card 1 */
#undef  TC_OLD_VER      /* define if each card handles 16 units, undef if
						 * each card handles 20 units */
#undef  FAKE_UNITS	/* generate fake units for testing */ 
#undef  PCMESSAGE	/* PC to PC messaging code */
#define PCMESSAGE_PORT 3 /* digital i/o port on PCDIO card used for PC Messaging, 
						  * range is from 2-4 */
#define PCS_SOCKET	/* PC to PC messaging code with sockets */ 
#undef  HARDWARE_SWITCH	/* enable hardware PSTOP and RLSTOP switches */ 
#undef  SMI_TRACK	/* SMI Eye tracker */ 
#undef  PLEX_ECODES	/* REX sends ecodes to Plexon box */
#define PLEX_PORT 4     /* which digital i/o port on the PCDIO board connected to the
						 * Plexon box, range is from 2-4 */

/* END_USER_DEFINES -- do not delete, move, or change this line */

/*
 *-----------------------------------------------------------------------*
 *	Configuration parameters;  note: other sampling related
 * defines are in buf.h
 *-----------------------------------------------------------------------*
 */
#undef SCOPE		/* enable hardware debugging through digitial
			 * I/O using oscilloscope- leave off for normal use */
#undef SOFT_TIMER	/* enable software timer profiling */
#undef EXT_CTR		/* external counter in use keep copy of i_time */
#undef HARD_ABORT	/* include code for hardware initiated abort */
/*
 * Hardware Abort defines.  This will permit a safe abort of any REX
 * hang and cause any open files to be written out and closed.
 */
#ifdef HARD_ABORT
#define ABORT_BIT	0x20		/* Hard abort bit */
#define ABORT_PORT	PCDIO0_PORT	/* address to test for ABORT bit */
#endif

#define CLOCK_RATE 1	/* clock interrupt rate in msec */
#define ST_RATE 1	/* processing rate of state set in msec */
#define W_PRE 100	/* default size of pre time in msec */
#define W_POST 100	/* default size of post time in msec */
#define DA_MAXNUM 16	/* max number of d/a converters- set to 6, 8, 16, or 32 */
#define RA_MAXNUM 10	/* max number of simultaneous ramps */
#define WD_MAXNUM 8	/* max number of signal windows; don't make this > 8 */
#define MA_MAXNUM 16	/* max number of memory array sources */

/*
 *-----------------------------------------------------------------------*
 *	SMI Eye Tracker
 *-----------------------------------------------------------------------*
 */
#ifdef SMI_TRACK
#define SMI_DIO_BASE 0x2a0
#define SMI_CHIP_NUM 5	/* number of 8255 chips in use */
#define SMI_NUM_VAL 6		/* number of 16 bit values sent by SMI */
#define SMI_MAXCOUNTS 12	/* number of stat bins for counts */
#define SMI_TOGGLE_BASE (SMI_DIO_BASE+(4*4)+2)	/* toggle bit on chip 5 port C */
#define SMI_TOGGLE_MASK (0x4)						/* toggle bit */
typedef struct {
    int	    *smi_var;	/* pointer to global variable */
    int	    smi_offl;	/* offset to low byte from SMI_DIO_BASE */
    int	    smi_offh;	/* offset to high byte from SMI_DIO_BASE */
} SMI;
extern SMI smi[];
#endif

/*
 *-----------------------------------------------------------------------*
 * Plexon support to transmit ecodes to Plexon box.  Assumes that Plexon
 * digital input is operated in mode 3, 15 bit strobed word.  Unit ecodes
 * and negative ecodes are not sent.  Low order byte of ecode is transmitted on port A
 * of 8255, high order byte is transmitted on port B.  Bit 0 of port C
 * is connected to Plexon strobe bit, and bit 4 of Port C is connected to
 * Plexon busy bit.
 *
 * Plexon box can receive a strobed word about every 120usec.  This implementation
 * sends ecodes automatically from the load event routines (int_ldevent() and ldevent())
 * when the box is not busy.  Otherwise the ecodes are buffered and sent
 * at the end of the interrupt and clock routines.  Given that the interrupt
 * routine runs every 500usec and the clock routine every msec this will give,
 * on average, 3 or more opportunities every msec to send an ecode.  This
 * is below the maximum of 8.3 (1msec / 120usec).  To achieve the max thruput
 * would require having the Plexon busy line generate an interrupt.
 *
 * Borrowing an idea from Jochen Ditterich there are two buffers for
 * ecodes- a high priority buffer and a low priority buffer.  The high
 * priority ecodes are sent first.  The user can specifically queue
 * low priority ecodes by calling 'ldevent_plexon_low(EVENT *evp, int flag)'.
 * This routine has a flag argument.  If it is true, then the ecode is not
 * entered into the internel REX event buffer, but rather only sent to the
 * Plexon box.  This permits a full 15 bits to be passed to the Plexon box, which
 * would not be possible if the code were entered into the REX data stream since
 * REX reserves some of the high order bits of the ecode.
 *-----------------------------------------------------------------------*
 */
#ifdef PLEX_ECODES
#define PL_DIOBASE  ((i_b->i_prim_dio_base)+(PLEX_PORT*4))   /* base address of 8255 connected to Plexon box */
#define PL_BUSY	    0x10    /* Plexon busy bit */
#define PL_STROBE   0x1	    /* Plexon strobe bit */
#define	PL_MAXCODES 100	    /* max number of ecodes buffered */
#define PL_UNIT_CODE_LOW    600	/* range of ecodes exluded from being sent to Plexon
								   by int_ldevent() */
#define PL_UNIT_CODE_HIGH   799
#define pl_box_ready_	    (!(in8(PL_DIOBASE+2) & PL_BUSY))

/*
 * The following macro is used to send the ecode to the Plexon box.  Note that
 * the strobe bit is sent separately from the data in Port C.  This is to
 * give time for cable deskew, and 8255 settling.
 */
#define pl_send_code_(code)  { \
		out8(PL_DIOBASE+2, 0); /* clear strobe */ \
		out8(PL_DIOBASE, code & 0xff); \
		out8(PL_DIOBASE+1, (code >> 8)); \
		out8(PL_DIOBASE+2, PL_STROBE); /* set strobe */ \
	}
#endif

/*
 *-----------------------------------------------------------------------*
 * MEX configuration, comm version 1.
 *-----------------------------------------------------------------------*
 */
#ifdef MEXWEX_UNITS
/*
 * Defines for receiving units from MEX.
 * MEX communicates to REX via an 8 bit parallel port.  This port
 * should be declared and initialized in int/cnf.c, however REX
 * doesnt use 'dio_in()' to access this port for efficiency reasons.
 * It's acutal bus address must be decleared below.
 */
#define MEX1_LOW_DIR (PCDIO0_PORT+(MEXWEX_PORT*4))

/*
 *-----------------------------------------------------------------------*
 * MEX configuration, comm version 2.
 *-----------------------------------------------------------------------*
 */
/*
 * Defines for receiving units from MEX.
 * MEX communicates to REX via an 8 bit parallel ports.  The ports
 * should be declared and initialized in int/cnf.c, however REX
 * doesnt use 'dio_in()' to access this port for efficiency reasons.
 * It's acutal bus address must be decleared below.
 */
#ifdef MEX62_UNITS
#define MEX2_BYTE_NUM 2		/* number of bytes use to communicate from MEX to REX */
#define MEX_CODE_PORT (MEX1_LOW_DIR + 2)
#else
#define MEX2_BYTE_NUM 3	/* number of bytes use to communicate from MEX to REX */
#endif		// MEX62_UNITS

#define MEX2_UNITS_P_BYTE 6			/* number of bits devoted to MEX units */
#define MEX2_SEQ_P_BYTE (8 - MEX2_UNITS_P_BYTE)	/* number of bits devoted to seq number */
#define MEX2_SEQ_MASK (0xff >> MEX2_UNITS_P_BYTE)
#endif 		// MEXWEX_UNITS

/*
 *-----------------------------------------------------------------------*
 * Timing parameters via oscilloscope.
 *-----------------------------------------------------------------------*
 */
#ifdef SCOPE
#define SCOPE_L1 Dio_id(PCDIO_DIO, 2, 0x1)
#define SCOPE_L2 Dio_id(PCDIO_DIO, 2, 0x2)
#define SCOPE_L3 Dio_id(PCDIO_DIO, 2, 0x4)
#define SCOPE_L4 Dio_id(PCDIO_DIO, 2, 0x8)
#define SCOPE_L5 Dio_id(PCDIO_DIO, 2, 0x10)
#define SCOPE_L6 Dio_id(PCDIO_DIO, 2, 0x20)
#define SCOPE_L7 Dio_id(PCDIO_DIO, 2, 0x40)
#define SCOPE_L8 Dio_id(PCDIO_DIO, 2, 0x80)
#endif

#ifdef EXT_CTR
#define EXT_CTR_COUNT	Dio_id(PCDIO_DIO, 2, 0x1)
#define EXT_CTR_RESET	Dio_id(PCDIO_DIO, 2, 0x2)
#endif

/*
 *-----------------------------------------------------------------------*
 * Device id assignments.  Each d/a or digital i/o device must have
 * a UNIQUE device id.  Range: 1-0xff.  These should not be changed.
 *-----------------------------------------------------------------------*
 */
#ifdef ANDAS12
    #define ANDAS12_DA	    0x1
#endif
#ifdef CIO_DA
    #define CIO_DA_ISA	    0x2
#endif
#ifdef PCI_DtoA
    #define PCI_DtoA_PRIM   0x3     	/* primary d/a on PCI bus */
    #define CIO_DA_DA PCI_DtoA_PRIM
#else
    #define CIO_DA_DA CIO_DA_ISA    	/* for backwards compatibility for ISA */
#endif
#ifdef KMPIO12
    #define KMPIO120_DIO    0x4
    #define KMPIO121_DIO    0x5
#endif
#ifdef PCDIO
    #define PCDIO_DIO_ISA   0x6
#endif    
#ifdef PCI_DIO
    #define PCI_DIO_PRIM    0x7   	/* primary digital i/o on PCI bus */
    #define PCDIO_DIO PCI_DIO_PRIM
#else
    #define PCDIO_DIO PCDIO_DIO_ISA	/* for backwards compatibility for ISA */
#endif

/*
 * These defines are for backwards compatibility with pre-PCI versions that
 * had port base addresses as compile time constants.
 * ---------------------------------------------------------------------
 */
#define PCDIO0_PORT	prim_dio_base		/* base address */
#define KMPIO120_PORT	prim_dio_base		/* base address */
#define KMPIO121_PORT	(prim_dio_base + 4)	/* base address */

/*
 *-----------------------------------------------------------------------*
 * Default device specifications.  Make sure devices used below
 * have been #defined above.  
 *-----------------------------------------------------------------------*
 */
/*
 * Device to pulse to reset unit latch.
 */
#define LATCH_RESET	Dio_id(PCDIO_DIO, 2, 0x1)

/*
 * Device ID definitions for d/as.
 */
#define DAX_ID	    Dio_id(CIO_DA_DA, 0, 0xfff)
#define DAY_ID	    Dio_id(CIO_DA_DA, 1, 0xfff)

/*
 * First card
 */
#define DA0_ID	    Dio_id(CIO_DA_DA, 0, 0xfff)
#define DA1_ID	    Dio_id(CIO_DA_DA, 1, 0xfff)
#define DA2_ID	    Dio_id(CIO_DA_DA, 2, 0xfff)
#define DA3_ID	    Dio_id(CIO_DA_DA, 3, 0xfff)
#define DA4_ID	    Dio_id(CIO_DA_DA, 4, 0xfff)
#define DA5_ID	    Dio_id(CIO_DA_DA, 5, 0xfff)
#define DA6_ID	    Dio_id(CIO_DA_DA, 6, 0xfff)
#define DA7_ID	    Dio_id(CIO_DA_DA, 7, 0xfff)
#define DA8_ID	    Dio_id(CIO_DA_DA, 8, 0xfff)
#define DA9_ID	    Dio_id(CIO_DA_DA, 9, 0xfff)
#define DA10_ID	    Dio_id(CIO_DA_DA, 10, 0xfff)
#define DA11_ID	    Dio_id(CIO_DA_DA, 11, 0xfff)
#define DA12_ID	    Dio_id(CIO_DA_DA, 12, 0xfff)
#define DA13_ID	    Dio_id(CIO_DA_DA, 13, 0xfff)
#define DA14_ID	    Dio_id(CIO_DA_DA, 14, 0xfff)
#define DA15_ID	    Dio_id(CIO_DA_DA, 15, 0xfff)

/*
 * Second card
 */
#define DA16_ID	    Dio_id(CIO_DA_DA, 16, 0xfff)
#define DA17_ID	    Dio_id(CIO_DA_DA, 17, 0xfff)
#define DA18_ID	    Dio_id(CIO_DA_DA, 18, 0xfff)
#define DA19_ID	    Dio_id(CIO_DA_DA, 19, 0xfff)
#define DA20_ID	    Dio_id(CIO_DA_DA, 20, 0xfff)
#define DA21_ID	    Dio_id(CIO_DA_DA, 21, 0xfff)
#define DA22_ID	    Dio_id(CIO_DA_DA, 22, 0xfff)
#define DA23_ID	    Dio_id(CIO_DA_DA, 23, 0xfff)
#define DA24_ID	    Dio_id(CIO_DA_DA, 24, 0xfff)
#define DA25_ID	    Dio_id(CIO_DA_DA, 25, 0xfff)
#define DA26_ID	    Dio_id(CIO_DA_DA, 26, 0xfff)
#define DA27_ID	    Dio_id(CIO_DA_DA, 27, 0xfff)
#define DA28_ID	    Dio_id(CIO_DA_DA, 28, 0xfff)
#define DA29_ID	    Dio_id(CIO_DA_DA, 29, 0xfff)
#define DA30_ID	    Dio_id(CIO_DA_DA, 30, 0xfff)
#define DA31_ID	    Dio_id(CIO_DA_DA, 31, 0xfff)

/*
 * Default device for 'bit' noun.  This device is used for commands
 * 'set bit' and 'clr bit'.
 */
#define BIT_DEFAULT PCDIO_DIO

/*
 * REX reads 16 bits of digital input every interrupt (msec).  Following
 * defines are args for function 'dio_in()', used to pick up this input.
 * The result is stored in a variable named 'dina' (a #define
 * also allows this variable to be accessed as 'drinput' for backward
 * compatability).  Bits in this variable can then be tested for on/off
 * from Spot files, e.g. "to nextstate on +BAR & drinput".
 */
#define DIN_LOW	    Dio_id(PCDIO_DIO, 0, 0xff)	/* low 8 bits of digital in */
#define DIN_HI	    Dio_id(PCDIO_DIO, 1, 0xff)	/* hi 8 bits of digital in */

/*
 * Standard definitions for digital input bits.
 */
#define CBAR		0x1	/* chair's bar */
#define EXBAR		0x2	/* experimenter's bar */
#define GOBAR		0x10
#define SOL		0x40	/* free reward button */
#define UNIT1		0x4000	/* units channel 1 */
#define UNIT0		0x8000	/* units channel 0 */

#ifdef HARDWARE_SWITCH	/* if hardware switches present */
#define PSTOP		0x4	/* stop paradigm */
#define RLSTOP	0x8	/* freeze displays */
#endif
#ifndef HARDWARE_SWITCH	/* if hardware switches NOT present */
#define PSTOP		0x1	/* stop paradigm */
#define RLSTOP	0x2	/* freeze displays */
#endif

/*
 *-----------------------------------------------------------------------*
 * Device addresses, dma channels, vectors.
 * Note:  a/d sampling rate options are configured in int/cnf.c.
 *-----------------------------------------------------------------------*
 */

/*
 * National Instruments AT-MIO-16X: 16 bit a/d, 2 12 bit d/a, 8 bits dio.
 * ---------------------------------------------------------------------
 */
#ifdef NIATMIOX
/*
 * ATMIO-16X is operated in dual dma mode, with interval scanning.
 * Channels must be 6 and 7.  Don't change.
 */
#define AD_RATE		2000	    /* default interrupt rate */
#define AD_CHANNELS	8	    /* 8 DI channels */
#define AD_RES		16	    /* 16 bit */
#define AD_RCOMP	0	    /* radix compensation- constant to subtract
				       from sample to produce 2's complement */
#define AD_VECT		10	    /* vector */
#define AD_CGLIST	1	    /* defined if a/d has a channel-gain
				       list */
#undef	AD_SINGLEDMA		    /* defined if a/d supports only
				       single-dma */
#define AD_DUALDMA	1	    /* defined if a/d supports dual-dma */
#define NI1_DMACHAN1	6	    /* DMA channel */
#define NI1_DMACHAN2	7	    /* DMA channel */
#define NI1_PORT	AD_BASE	    /* base address */
#endif

/*
 * Analogic DAS-12 12 bit a/d with 2 12 bit d/a converters.
 * ---------------------------------------------------------------------
 */
#ifdef ANDAS12
/*
 * Uses a single 16 bit DMA channel.
 */
#define AD_RATE		2000	    /* default interrupt rate */
#define AD_CHANNELS	8	    /* 8 DI channels */
#define AD_RES		12	    /* 12 bit */
#define AD_RCOMP	04000	    /* radix compensation- constant to subtract
				       from sample to produce 2's complement */
#define AD_VECT		10	    /* vector */
#undef	AD_CGLIST		    /* defined if a/d has a channel-gain
				       list */
#define	AD_SINGLEDMA	1	    /* defined if a/d supports only
				       single-dma */
#undef  AD_DUALDMA		    /* defined if a/d supports dual-dma */
#define AN1_DMACHAN	6	    /* DMA channel- a/d */
#define AN1_PORT	AD_BASE	    /* base address */
#endif

/*
 * Support for PCI a/d.
 * ---------------------------------------------------------------------
 */
#ifdef PCI_AtoD

/*
 * Doesnt use DMA.
 */
#define AD_RATE		2000	    /* default interrupt rate */
#define AD_CHANNELS	8	    /* 8 DI channels */
#define AD_RES		12	    /* 12 bit */
#define AD_RCOMP	0	    /* radix compensation- constant to subtract */
#undef	AD_SINGLEDMA
#undef  AD_DUALDMA
#undef  AD_CGLIST

#endif


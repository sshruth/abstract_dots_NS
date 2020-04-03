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
 * Configuration initializations.
 */
#include <stdio.h>
#include <sys/types.h>
#include "rexhdrs.h"
#include "ramp.h"
#include "int.h"

/*
 * Device description initializations.  Port configurations (in or out)
 * are also established here.
 */
void i8255_init(DIO_DEV *dp);
void pciPrimDtoAInit(DIO_DEV *dp);

DIO_DEV dio_dev[] = {

#ifdef ANDAS12
    {ANDAS12_DA,	    /* device id for this device; array terminates
			       when this field is zero */
    0,			    /* flags */
    0,			    /* port base address */
    0,			    /* pci rexuse field */
    0,			    /* pci vendor id */
    0,			    /* pci device id */
    {D_DA, D_DA},	    /* port configuration */
    {0},		    /* software state */
    0,			    /* function to call for initialization */
    0},			    /* function to call for uninitialization */
#endif

#ifdef KMPIO12		    /* Keithley-Metrabyte PIO-12, card 0 */
    {KMPIO120_DIO,	    /* device id for this device; array terminates
			       when this field is zero */
    0,			    /* flags */
    0,			    /* port base address */
    0,			    /* pci rexuse field */
    0,			    /* pci vendor id */
    0,			    /* pci device id */
    {D_IN|D_REV, D_IN|D_REV, D_IN|D_REV, D_CTRL,
    {0},		    /* software state */
    i8255_init,		    /* function to call for initialization */
    0},			    /* function to call for uninitialization */

    {KMPIO121_DIO,	    /* device id for this device; array terminates
			       when this field is zero */
    0,			    /* flags */
    0,			    /* port base address */
    0,			    /* pci rexuse field */
    0,			    /* pci vendor id */
    0,			    /* pci device id */
    D_OUT|D_REV, D_OUT|D_REV, D_OUT|D_REV, D_CTRL},
    {0},		    /* software state */
    i8255_init,		    /* function to call for initialization */
    0},			    /* function to call for uninitialization */
#endif

#ifdef PCDIO
    {PCDIO_DIO_ISA,	    /* device id for this device; array terminates
			       when this field is zero */
    0,			    /* flags */
    0,			    /* port base address */
    0,			    /* pci rexuse field */
    0,			    /* pci vendor id */
    0,			    /* pci device id */
#ifdef DIN_8
    {D_IN|D_REV, D_OUT|D_REV, D_OUT|D_REV, D_CTRL,   /* Port 0: i/o panel */
#else
    {D_IN|D_REV, D_IN|D_REV, D_OUT|D_REV, D_CTRL,   /* Port 0: i/o panel */
#endif
    D_OUT|D_REV, D_OUT|D_REV, D_OUT|D_REV, D_CTRL,  /* Port 1: i/o panel */
/*
 * Ports 2-4 on the 5 port card are initialized in dio.c:dio_init().
 */
    D_RES, D_RES, D_RES, D_RES,
    D_RES, D_RES, D_RES, D_RES,
    D_RES, D_RES, D_RES, D_RES
    },
    {0},		    /* software state */
    i8255_init,		    /* function to call for initialization */
    0},			    /* function to call for uninitialization */
#endif

#ifdef PCI_DIO
    {PCI_DIO_PRIM,	    /* device id for this device; array terminates
			       when this field is zero */
    0,			    /* flags */
    0xdc40,			    /* port base address */
    PCI_PRIMARY_DIO,	    /* pci rexuse field */
    0x494f,			    /* pci vendor id */
    0xc78,			    /* pci device id */
#ifdef DIN_8
    {D_IN|D_REV, D_OUT|D_REV, D_OUT|D_REV, D_CTRL,    /* Port 0: i/o panel */
#else
    {D_IN|D_REV, D_IN|D_REV, D_OUT|D_REV, D_CTRL,     /* Port 0: Switch panel & bricks */
#endif
    D_OUT|D_REV, D_OUT|D_REV, D_IN|D_REV, D_CTRL,     /* Port 1: Fancy TTL panel */

/*
 * Ports 2-4 on the 5 port card are initialized in dio.c:dio_init().
 */
		  
    D_IN|D_REV, D_OUT|D_REV, D_OUT|D_REV, D_CTRL,     /* Port 2: Unit Latch */
	D_OUT|D_REV, D_OUT|D_REV, D_IN_OUT|D_REV, D_CTRL, /* Port 3: Plexon */
	D_OUT|D_REV, D_OUT|D_REV, D_IN_OUT|D_REV, D_CTRL  /* Port 4: Plexon polaris */
    },
    {0},		    /* software state */
    i8255_init,		/* function to call for initialization */
    0},			    /* function to call for uninitialization */
#endif

#ifdef CIO_DA
    {CIO_DA_ISA,	    /* device id for this device; array terminates
			       when this field is zero */
    0,			    /* flags */
    PRIM_ISA_DA_BASE,	    /* port base address */
    0,			    /* pci rexuse field */
    0,			    /* pci vendor id */
    0,			    /* pci device id */
    {D_DA, D_DA, D_DA, D_DA,	/* up to 32 on 2 cards */
    D_DA, D_DA, D_DA, D_DA,
    D_DA, D_DA, D_DA, D_DA,
    D_DA, D_DA, D_DA, D_DA,
    D_DA, D_DA, D_DA, D_DA,
    D_DA, D_DA, D_DA, D_DA,
    D_DA, D_DA, D_DA, D_DA,
    D_DA, D_DA, D_DA, D_DA},
    {0},		    /* software state */
    0,			    /* function to call for initialization */
    0},			    /* function to call for uninitialization */
#endif

#ifdef PCI_DtoA
    {PCI_DtoA_PRIM,	    /* device id for this device; array terminates
			       when this field is zero */
    0,			    /* flags */
    0,			    /* port base address */
    PCI_PRIMARY_DtoA,	    /* pci rexuse field */
    0,			    /* pci vendor id */
    0,			    /* pci device id */
    {D_DA, D_DA, D_DA, D_DA,	/* up to 32 on 2 cards */
    D_DA, D_DA, D_DA, D_DA,
    D_DA, D_DA, D_DA, D_DA,
    D_DA, D_DA, D_DA, D_DA,
    D_DA, D_DA, D_DA, D_DA,
    D_DA, D_DA, D_DA, D_DA,
    D_DA, D_DA, D_DA, D_DA,
    D_DA, D_DA, D_DA, D_DA},
    {0},		    /* software state */
    pciPrimDtoAInit,	    /* function to call for initialization */
    0},			    /* function to call for uninitialization */
#endif

    {0},		    /* DONT DELETE!  Null termination for array */
};

/*
 * Possible max rates for each a/d with number of channels that
 * can be sampled at that rate.  Must be null terminated.
 * Currently REX is limited to max rates that produce periods that
 * evenly divide 1msec- 1,2,4,5,8,10Khz, etc.
 */
AD_SAMP ad_samp[]= {
#ifdef ANDAS12
    2000,8,	    /* a/d sampling config; MUST be null terminated */
    1000,8,
#endif
#ifdef PCI_AtoD
    2000,8,	    /* a/d sampling config; MUST be null terminated */
    1000,8,
#endif
#ifdef NIATMIOX
    2000,8,
    1000,8,
#endif
#ifdef DT2821
    1000,8,
#endif    
    0,0
};

/*
 * Possible sampling rates.  Filled in by sig_compute_ad_rates().
 */
AD_ACQ_RATES ad_acq_rates[SA_SPREAD_LOG2+1];

/*
 * Gains supported by various a/ds
 */
AD_GAIN ad_gain[] = {
#ifdef ANDAS12
    1, 0x0,
    2, 0x1,
    4, 0x2,
    8, 0x3,
#endif
#ifdef PCI_AtoD
    1, 0x0,
    2, 0x1,
    4, 0x2,
    8, 0x3,
#endif
#ifdef NIATMIOX
    1, 0x1,
    2, 0x2,
    5, 0x3,
    10, 0x4,
    20, 0x5,
    50, 0x6,
    100, 0x7,
#endif     
#ifdef DT2821
    1, 0x0,
    2, 0x1,
    4, 0x2,
    8, 0x3,
#endif
    -1, 0,
};

/*
 * Names and addresses of variables that can be specified as memory locations
 * for storing a/d values.
 */
GVPTR gv_ad_mem[] = {
    &eyeh, "eyeh",
    &eyev, "eyev",
    &oeyeh, "oeyeh",
    &oeyev, "oeyev",
    &otherh, "otherh",
    &otherv, "otherv",
    &addh, "addh",
    &addv, "addv",
    &joyh, "joyh",
    &joyv, "joyv",
    0, NS,
};

/*
 * Names and addresses of variables that can be specified as non-a/d sources
 * for signals.
 */
GVPTR gv_mem_src[] = {
    &da[0].da_curval, "da_0",
    &da[1].da_curval, "da_1",
    &da[2].da_curval, "da_2",
    &da[3].da_curval, "da_3",
    &ramp[0].ra_x.ra_pos, "ramp_0_x",
    &ramp[0].ra_y.ra_pos, "ramp_0_y",
    &ramp[1].ra_x.ra_pos, "ramp_1_x",
    &ramp[1].ra_y.ra_pos, "ramp_1_y",
/* because the wd array is now in the shared memory space
 * the pointers have to be assigned in main so just assign
 * dummy pointers here to build the array
 */
	0, "wd_0_x",
	0, "wd_0_y",
    0, "wd_1_x",
    0, "wd_1_y",

/* don't assign pointers here
    &i_b->wd[0].wd_xcurpos, "wd_0_x",
    &i_b->wd[0].wd_ycurpos, "wd_0_y",
    &i_b->wd[1].wd_xcurpos, "wd_1_x",
    &i_b->wd[1].wd_ycurpos, "wd_1_y",
*/
	/* computed variables */
	&vergh, "vergh",
	&vergv, "vergv",
	&cycloph, "cycloph",
	&cyclopv, "cyclopv",
	&gazeh, "gazeh",
	&gazev, "gazev",
	&ogazeh, "ogazeh",
	&ogazev, "ogazev",
	&vgazeh, "vgazeh",
	&vgazev, "vgazev",
	&cgazeh, "cgazeh",
	&cgazev, "cgazev",

#ifdef SMI_TRACK
    &leyex, "leyex",
    &leyey, "leyey",
    &lpup, "lpup",
    &reyex, "reyex",
    &reyey, "reyey",
    &rpup, "rpup",
#endif
    0, NS,
};

#ifdef SMI_TRACK
SMI smi[] = {
    {&leyex, 0x0, 0x1},
    {&leyey, 0x4, 0x5},
    {&lpup, 0x8, 0x9},
    {&reyex, 0xc, 0xd},
    {&reyey, 0x10, 0x11},
    {&rpup, 0x2, 0x6},
};
#endif

#ifdef NIATMIO
u_int ni0_com1;
u_int ni0_com2;
#endif

#ifdef NIATMIOX
u_int ni1_com1;
u_int ni1_com2;
u_int ni1_com3;
u_int ni1_com4;
#endif

#ifdef ANLSDAS16
u_int an0_ictrl;
u_int an0_ctrl;
#endif

/*
 * SIG struct initializations.
 */
SIG sig[SA_MAXSIG]= {
    
    /* signal 0 */
    {SIG_AD_SOURCE,	/* enable */
    0,			/* a/d channel */
    "eyeh",		/* a/d memory variable */
    2000,		/* a/d rate */
    1,			/* a/d calib */
    1,			/* a/d gain */
    23,			/* a/d filter delay */
    "-",		/* memory source */
    "horiz_eye",	/* title */
    1000},		/* store rate */

    /* signal 1 */
    {SIG_AD_SOURCE,	/* enable */
    1,			/* a/d channel */
    "eyev",		/* a/d memory variable */
    2000,		/* a/d rate */
    1,			/* a/d calib */
    1,			/* a/d gain */
    23,			/* a/d filter delay */
    "-",		/* memory source */
    "vert_eye",		/* title */
    1000},		/* store rate */

    /* signal 2 */
    {SIG_AD_SOURCE,	/* enable */
    2,			/* a/d channel */
    "oeyeh",		/* a/d memory variable */
    2000,		/* a/d rate */
    1,			/* a/d calib */
    1,			/* a/d gain */
    23,			/* a/d filter delay */
    "-",		/* memory source */
    "other_eyeh",	/* title */
    1000},		/* store rate */

    /* signal 3 */
    {SIG_AD_SOURCE,	/* enable */
    3,			/* a/d channel */
    "oeyev",		/* a/d memory variable */
    2000,		/* a/d rate */
    1,			/* a/d calib */
    1,			/* a/d gain */
    23,			/* a/d filter delay */
    "-",		/* memory source */
    "other_eyev",	/* title */
    1000},		/* store rate */

    /* signal 4 */
    {SIG_AD_SOURCE,	/* enable */
    4,			/* a/d channel */
    "addh",		/* a/d memory variable */
    2000,		/* a/d rate */
    1,			/* a/d calib */
    1,			/* a/d gain */
    23,			/* a/d filter delay */
    "-",		/* memory source */
    "add_h",		/* title */
    1000},		/* store rate */

    /* signal 5 */
    {SIG_AD_SOURCE,	/* enable */
    5,			/* a/d channel */
    "addv",		/* a/d memory variable */
    2000,		/* a/d rate */
    1,			/* a/d calib */
    1,			/* a/d gain */
    23,			/* a/d filter delay */
    "-",		/* memory source */
    "add_v",		/* title */
    1000},		/* store rate */

    /* signal 6 */
    {SIG_AD_SOURCE,	/* enable */
    6,			/* a/d channel */
    "joyh",		/* a/d memory variable */
    2000,		/* a/d rate */
    1,			/* a/d calib */
    1,			/* a/d gain */
    23,			/* a/d filter delay */
    "-",		/* memory source */
    "horiz_joy",	/* title */
    250},		/* store rate */

    /* signal 7 */
    {SIG_AD_SOURCE,	/* enable */
    7,			/* a/d channel */
    "joyv",		/* a/d memory variable */
    2000,		/* a/d rate */
    1,			/* a/d calib */
    1,			/* a/d gain */
    23,			/* a/d filter delay */
    "-",		/* memory source */
    "vert_joy",		/* title */
    250},		/* store rate */

    /* signal 8 */
    {SIG_MEM_SOURCE,	/* enable */
    NULLI,		/* a/d channel */
    "-",		/* a/d memory variable */
    NULLI,		/* a/d rate */
    NULLI,		/* a/d calib */
    NULLI,		/* a/d gain */
    NULLI,		/* a/d filter delay */
    "da_0",		/* memory source */
    "horiz_mirror",	/* title */
    500},		/* store rate */

    /* signal 9 */
    {SIG_MEM_SOURCE,	/* enable */
    NULLI,		/* a/d channel */
    "-",		/* a/d memory variable */
    NULLI,		/* a/d rate */
    NULLI,		/* a/d calib */
    NULLI,		/* a/d gain */
    NULLI,		/* a/d filter delay */
    "da_1",		/* memory source */
    "vert_mirror",	/* title */
    500},		/* store rate */

    {0},
};

/*
 * This struct maps scale factor above to proper shift factors
 * used in rex for 12 and 16 bit a/d converters.
 */
AD_CALIB ad_calib[SA_MAXCAL] = {
    {0,
#if AD_RES == 12
     -1,
#elif AD_RES == 16
     3,
#endif
     "0: 204.8 deg FS",
    },
    {1,
#if AD_RES == 12
     0,
#elif AD_RES == 16
     4,
#endif
     "1: 102.4 deg FS",
    },
    {2,
#if AD_RES == 12
     1,
#elif AD_RES == 16
     5,
#endif
     "2: 51.2 deg FS",
    },
    {3,
#if AD_RES == 12
     2,
#elif AD_RES == 16
     6,
#endif
     "3: 25.6 deg FS",
    },
    {4,
#if AD_RES == 12
     3,
#elif AD_RES == 16
     7,
#endif
     "4: 12.8 deg FS",
    },
    {5,
#if AD_RES == 12
     4,
#elif AD_RES == 16
     8,
#endif
     "5: 6.4 deg FS",
    },
    {6,
     0,
     "6: 16 bit test",
    },
    {-1},	/* -1 terminates array */
};

/*
 * D/A converter struct initialization.  This struct determines the
 * correspondence between the da index number and the physical da
 * converter.  Also, flag 'da_clear' if initialized to 1 will result
 * in d/a being cleared when int process is first run.
 */
DA da[DA_MAXNUM] = {
    {DA0_ID, 1},
    {DA1_ID, 1},
    {DA2_ID, 1},
    {DA3_ID, 1},
    {DA4_ID, 1},
    {DA5_ID, 1},
#if DA_MAXNUM > 6
    {DA6_ID, 1},
    {DA7_ID, 1},
#endif
#if DA_MAXNUM > 8
    {DA8_ID, 1},
    {DA9_ID, 1},
    {DA10_ID, 1},
    {DA11_ID, 1},
    {DA12_ID, 1},
    {DA13_ID, 1},
    {DA14_ID, 1},
    {DA15_ID, 1},
#endif
#if DA_MAXNUM > 16
    {DA16_ID, 1},
    {DA17_ID, 1},
    {DA18_ID, 1},
    {DA19_ID, 1},
    {DA20_ID, 1},
    {DA21_ID, 1},
    {DA22_ID, 1},
    {DA23_ID, 1},
    {DA24_ID, 1},
    {DA25_ID, 1},
    {DA26_ID, 1},
    {DA27_ID, 1},
    {DA28_ID, 1},
    {DA29_ID, 1},
    {DA30_ID, 1},
    {DA31_ID, 1},
#endif
};

#ifdef MEXWEX_UNITS
/*
 * Ports are initialized in dio.c:dio_init().
 */
#ifdef MEX62_UNITS
MEX2 mex2[MEX2_BYTE_NUM] = {
    {0,             	/* port */
	 610,  				/* ecode base */
	 0},				/* unit base */
    {0,             	/* port */
	 616,  				/* ecode base */
	 6},				/* unit base */
};
#else
MEX2 mex2[MEX2_BYTE_NUM]= {
    {0,             	/* port */
	 610,  				/* ecode base */
	 0},				/* unit base */
    {0,             	/* port */
	 616,  				/* ecode base */
	 6},				/* unit base */
    {0,             	/* port */
	 622,  				/* ecode base */
	 12},				/* unit base */
};
#endif		// MEX62_UNITS
#endif		// MEXWEX_UNITS

/*
 * Initialization of structs that control 9513 timer/counter
 * chips for collecting units.  The ICS cards have 4 9513 chips per card.
 * Each 9513 chip contains 5 counter/timers.  This setup is for the
 * Spectrum Scientific box and handles 32 units using 2 cards.
 */
#ifdef TC_UNITS
TC tc[TC_NUM]= {

#ifdef TC_OLD_VER
    /* CARD 1 */
    {TC_CARD0_BASE, /* chip port */
    5,		    /* number of counters in use on chip */
    611,	    /* ecode base */
    {0},	    /* last count array */
    },
    {TC_CARD0_BASE+2,	    /* chip port */
    5,		    /* number of counters in use on chip */
    616,	    /* ecode base */
    {0},	    /* last count array */
    },
    {TC_CARD0_BASE+4,	    /* chip port */
    5,		    /* number of counters in use on chip */
    621,	    /* ecode base */
    {0},	    /* last count array */
    },
    {TC_CARD0_BASE+6,	    /* chip port */
    1,		    /* number of counters in use on chip */
    626,	    /* ecode base */
    {0},	    /* last count array */
    },

    /* CARD 2 */
    {TC_CARD1_BASE, /* chip port */
    5,		    /* number of counters in use on chip */
    627,	    /* ecode base */
    {0},	    /* last count array */
    },
    {TC_CARD1_BASE+2,	    /* chip port */
    5,		    /* number of counters in use on chip */
    632,	    /* ecode base */
    {0},	    /* last count array */
    },
    {TC_CARD1_BASE+4,	    /* chip port */
    5,		    /* number of counters in use on chip */
    637,	    /* ecode base */
    {0},	    /* last count array */
    },
    {TC_CARD1_BASE+6,	    /* chip port */
    1,		    /* number of counters in use on chip */
    642,	    /* ecode base */
    {0},	    /* last count array */
    },

#else

    /* CARD 1 */
    {TC_CARD0_BASE, /* chip port */
    5,		    /* number of counters in use on chip */
    611,	    /* ecode base */
    {0},	    /* last count array */
    },
    {TC_CARD0_BASE+2,	    /* chip port */
    5,		    /* number of counters in use on chip */
    616,	    /* ecode base */
    {0},	    /* last count array */
    },
    {TC_CARD0_BASE+4,	    /* chip port */
    5,		    /* number of counters in use on chip */
    621,	    /* ecode base */
    {0},	    /* last count array */
    },
    {TC_CARD0_BASE+6,	    /* chip port */
    5,		    /* number of counters in use on chip */
    626,	    /* ecode base */
    {0},	    /* last count array */
    },

    /* CARD 2 */
    {TC_CARD1_BASE, /* chip port */
    5,		    /* number of counters in use on chip */
    631,	    /* ecode base */
    {0},	    /* last count array */
    },
    {TC_CARD1_BASE+2,	    /* chip port */
    5,		    /* number of counters in use on chip */
    636,	    /* ecode base */
    {0},	    /* last count array */
    },
    {TC_CARD1_BASE+4,	    /* chip port */
    5,		    /* number of counters in use on chip */
    641,	    /* ecode base */
    {0},	    /* last count array */
    },
    {TC_CARD1_BASE+6,	    /* chip port */
    5,		    /* number of counters in use on chip */
    646,	    /* ecode base */
    {0},	    /* last count array */
    },
#endif
};
#endif    

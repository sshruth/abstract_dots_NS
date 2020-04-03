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
 *	Device specific code for int process.  Conditionally compiled
 * from defines in ../hdr/cnf.h.
 */
#include <stdio.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/neutrino.h>
#include <hw/inout.h>
#include "rexhdrs.h"
#include "dma.h"
#include "int.h"

/*
 *----------------------------------------------------------------------*
 * A/D Support.
 *----------------------------------------------------------------------*
 */
unsigned counter;
int int_id;
const struct sigevent sigEvent;

void
ad_start(void)
{

    /*
     * Init a/d board.
     */
    if(ad_ds_init() == -1) return;

    /*
     * Attach to a/d interrupt.
     */
    if(int_id == 0) {
#ifdef PCI_AtoD
		if((int_id = InterruptAttach(ni_adp->ni_irq, ad_int_hand, &sigEvent, sizeof(sigEvent), 0)) == -1) {
		    rxerr("Cannot attach to a/d interrupt");
		    int_id= 0;
		    return;
		}
#else
		if((int_id = InterruptAttach(AD_VECT, ad_int_hand, &sigEvent, sizeof(sigEvent), 0)) == -1) {
		    rxerr("Cannot attach to a/d interrupt");
		    int_id= 0;
		    return;
		}
#endif
    }

    /*
     * Start a/d.
     */
    ad_ds_begin();
}

void
ad_stop(void)
{
    /*
     * Stop a/d.
     */
    ad_ds_end();

    /*
     * Detach interrupt.
     */
    if(int_id) {
		if(InterruptDetach(int_id)) {
			rxerr("Cannot detach a/d interrupt");
			return;
		}
    }
    int_id= 0;

    /*
     * <Clean up.
     */
    ad_ds_uninit();
}

/*
 *----------------------------------------------------------------------*
 * Device specific routines for a/d support.
 *----------------------------------------------------------------------*
 */
#ifdef PCI_AtoD

/*
 * Setup DMA or shared memory areas needed by a/d.
 */
int
ad_ds_init(void)
{
   ad_ds_end();
   return(0);
}

int
ad_ds_uninit(void)
{
   return(0);
}

/*
 * Reset the board.
 */
void
ad_ds_end(void)
{
   
	/*
	 * Critical section- turn off interrupts.
	 */
	InterruptDisable();

	NI_WriteDaqWin(ni_adp, INT_CTL_WO, 0x0, Interrupt_A_Enable, 0x0); /* clear Interrupt_A_Enable */
	NI_WriteDaqWin(ni_adp, INT_A_ENA_WO, 0x0, 0x3ff, 0x0); /* clear all interrupt enables */
	NI_StrobeDaqWin(ni_adp, ADC_FIFO_CLEAR_WO, 0x1); /* adc fifo clear */
	NI_StrobeDaqWin(ni_adp, JOINT_RESET_WO, AI_Reset_STRB); /* reset board registers */
	InterruptEnable();
   
	NI_StrobeDaqWin(ni_adp, JOINT_RESET_WO, AI_Configuration_Start_STRB); /* prevent glitches when clearing ACK's */
	NI_StrobeDaqDir(ni_adp, INT_A_ACK_WO_DIR, 0xffe0); /* clear all ACKs */
	NI_StrobeDaqWin(ni_adp, JOINT_RESET_WO, AI_Configuration_End_STRB);
   
	/*
	 * Board should now be in a stable state, and not active.
	 */
}

/*
 * Begin data acquisitions.
 */
void
ad_ds_begin(void)
{
	int i, dither, gain, range;

	ad_ds_end(); /* make sure board is stable */
	NI_ClearSoftRegisters(ni_adp); /* clear software register copies */
#ifdef GOO
	NI_Calibrate(ni_adp); /* load calibration constants into DACs */
#endif
   
	/*
	 * Load configuration memory.  We load all 8 differential channels.  These form a scan, which
	 * is peformed every 1msec or .5msec.
	 */
	NI_StrobeDaqWin(ni_adp, CONFIG_MEM_CLEAR_WO, 0x1); /* clear config memory */
	for(i=0; i < 8; i++) {
		NI_StrobeDaqDir(ni_adp, CONFIG_MEM_HI_WO_DIR, Diff_Channel_Type | i);
#ifdef GOO /* not implemented yet */
		switch(ni_adp->ni_type) {
		case NI_PCI6032E: /* dither cannot be set to 0 on these cards */
		case NI_PCI6033E:
			dither= DitherEn;
			break;
		default:
			dither= 0x0;
			break;
		}
#endif
		dither= DitherEn;
		range= 0x0;         /* bipolar */
		gain= 0x1;
		NI_StrobeDaqDir(ni_adp, CONFIG_MEM_LOW_WO_DIR, (i == 7 ? LastChan : 0x0)
						| dither | range | gain);
	}
	NI_WriteDaqWin(ni_adp, CLOCK_FOUT_WO, Slow_Internal_Time_Divide_By_2 | Slow_Internal_Timebase
				   | Clock_To_Board_Divide_By_2 | Clock_To_Board, 0x0, 0x0); /* setup timing */

	NI_StrobeDaqWin(ni_adp, JOINT_RESET_WO, AI_Configuration_Start_STRB); /* prevent glitches */
	NI_WriteDaqWin(ni_adp, AI_MODE_1_WO, Reserved_One|AI_Start_Stop, 0x0, 0x0);
	NI_WriteDaqWin(ni_adp, CLOCK_FOUT_WO, AI_Output_Divide_By_2, 0x0, 0x0);
	NI_WriteDaqWin(ni_adp, AI_PERSONAL_WO, AI_LOCALMUX_CLK_Pulse_Width | AI_Overrun_Mode |
				   AI_CONVERT_Pulse_Width | AI_SOC_Polarity |
				   AI_SHIFTIN_Pulse_Width, 0x0, 0x0);
	NI_WriteDaqWin(ni_adp, AI_OUTPUT_CONTROL_WO, 0x32e, 0x0, 0x0); /* AI_SCAN_IN_PROG_Output_Select= 3,
																	* AI_LOCALMUX_CLK_Output_Select= 2,
																	* AI_SC_TC_Output_Select= 3,
																	* AI_CONVERT_Output_Select= 2 */
	NI_StrobeDaqWin(ni_adp, JOINT_RESET_WO, AI_Configuration_End_STRB);

	NI_WriteDaqWin(ni_adp, AI_COM_1_WO, 0x0, 0x0, AI_CONVERT_Pulse_STRB); /* prime config memory */

	NI_StrobeDaqWin(ni_adp, JOINT_RESET_WO, AI_Configuration_Start_STRB); /* prevent glitches */
	NI_WriteDaqWin(ni_adp, AI_TRIG_SEL_WO, AI_START1_Sync | AI_START1_Edge, 0x0, 0x0);
	NI_WriteDaqWin(ni_adp, AI_MODE_1_WO, AI_Continuous, 0x0, 0x0); /* continuous scans */
	NI_StrobeDaqWin(ni_adp, AI_SC_LD_A_WO_PR, 0x0); /* set to 1 scan */
	NI_StrobeDaqWin(ni_adp, AI_SC_LD_A_WO_PR+1, 0x0); /* set to 1 scan */
	NI_WriteDaqWin(ni_adp, AI_COM_1_WO, 0x0, 0x0, AI_SC_Load_STRB); /* load SC */
	NI_WriteDaqWin(ni_adp, AI_START_STOP_SEL_WO, AI_START_Edge | AI_START_Sync |
				   AI_STOP_Sync | (19 << 7), 0x0, 0x0); /* 19 = AI_STOP_IN pin */
	NI_StrobeDaqWin(ni_adp, AI_SI_LD_A_WO_PR, 0x0); /* time from START1 to first START */
	NI_StrobeDaqWin(ni_adp, AI_SI_LD_A_WO_PR+1, 0x1);
	NI_WriteDaqWin(ni_adp, AI_COM_1_WO, 0x0, 0x0, AI_SI_Load_STRB); /* load SI */
	NI_StrobeDaqWin(ni_adp, AI_SI_LD_A_WO_PR, 0x0);
	NI_StrobeDaqWin(ni_adp, AI_SI_LD_A_WO_PR+1, 9999); /* scan interval of .5 msec, 20000 counts = 1msec */
	NI_StrobeDaqWin(ni_adp, AI_SI2_LD_A_WO, 599); /* convert interval of 30usec */
	NI_StrobeDaqWin(ni_adp, AI_SI2_LD_B_WO, 599); /* convert interval of 30usec */
	NI_WriteDaqWin(ni_adp, AI_MODE_2_WO, AI_SI2_Reload_Mode, 0x0, 0x0);
	NI_WriteDaqWin(ni_adp, AI_COM_1_WO, 0x0, 0x0, AI_SI2_Load_STRB);
	NI_WriteDaqWin(ni_adp, AI_MODE_2_WO, AI_SI2_Initial_Load_Source, 0x0, 0x0);
	NI_StrobeDaqWin(ni_adp, JOINT_RESET_WO, AI_Configuration_End_STRB);

	NI_StrobeDaqWin(ni_adp, ADC_FIFO_CLEAR_WO, 0x1); /* adc fifo clear */
	NI_WriteDaqWin(ni_adp, INT_CTL_WO, Interrupt_Output_Polarity, 0x0, 0x0); /* select interrupt stuff */
	NI_WriteDaqWin(ni_adp, INT_A_ENA_WO, AI_START_Interrupt_Enable, 0x0, 0x0); /* select interrupt */
	NI_WriteDaqWin(ni_adp, INT_CTL_WO, Interrupt_A_Enable, 0x0, 0x0); /* enable interrupts */
	NI_WriteDaqWin(ni_adp, AI_COM_1_WO, 0x0, 0x0, AI_SC_Arm_STRB | AI_SI_Arm_STRB | AI_SI2_Arm_STRB); /* arm */
	NI_WriteDaqDir(ni_adp, AI_COM_2_WO_DIR, 0x0, 0x0, AI_START1_Pulse_STRB); /* begin acquisitions */
}
#endif


/*
 * National Instruments ATMIO-16X.  16 bit A/D, 16 bit AT interface, DMA,
 * 2 D/A channels, 8 bits of digital I/O.  Operated in dual
 * dma mode.
 * ----------------------------------------------------------------------
 */
#ifdef NIATMIOX

/* u_int ad_sel= -1; */
DMA dma_a= {0}, dma_b= {0};
int int_a;		/* signifies which dma buffer was loaded
				 * with dual dma */
/* int shm_fd; */

int
ad_ds_init(void)
{
	/*     struct _seginfo sbuf; */
    int rval;
	off64_t physAddr;
    
    /*
     * Setup DMA controller.  Operate in dual-dma mode
     * using 2 16 bit channels.  All 8 channels are sampled and stored
     * each msec.
     */

	if((dma_a.dma_lbufp = mmap(0, 64, PROT_READ | PROT_WRITE | PROT_NOCACHE,
							   MAP_PHYS | MAP_ANON | MAP_BELOW16M | MAP_NOX64K, NOFD, 0)) == MAP_FAILED) {
		rxerr("ad_ds_init(): cannot mmap dma buffer");
	}
	mem_offset(dma_a.dma_lbufp, NOFD, 64, &physAddr, 0);
    /*
     * Get segment for dma buffer.
     */
	/***************************************************************
    if(ad_sel == -1) {
	if((ad_sel= qnx_segment_alloc_flags(64, _PMF_DMA_SAFE)) == -1) {
	    rxerr("Cannot alloc a/d DMA segment");
	    return(-1);
	}
    }
    if(qnx_segment_info(0, 0, ad_sel, &sbuf) == -1) {
	    rxerr("Cannot 'qnx_segment_info' on a/d DMA segment");
	    return(-1);
    }
	****************************************************************/

    /*
     * Map segment into process address space.
     */
	/***************************************************************
    if((shm_fd= shm_open("Physical", O_RDWR, 0)) == -1) {
	rxerr("ad_ds_init(): cannot open PHYSICAL_MEMORY");
	return(-1);
    }
    if((dma_a.dma_lbufp= mmap(0, 64, PROT_READ | PROT_WRITE, MAP_SHARED,
							  shm_fd, sbuf.addr)) == (void *)-1) {
		rxerr("ad_ds_init(): cannot mmap dma buffer");
		return(-1);
    }
	****************************************************************/

    dma_a.dma_pbufp.dma_phys= physAddr;
    dma_a.dma_chan= NI1_DMACHAN1;
    dma_a.dma_mode= (DMA_M_WRITE | DMA_M_AUTOINIT | DMA_M_SINGLE);
    dma_a.dma_bcount= 16;
    dma_a.dma_inuse= 1;
    if((rval= dma_init(&dma_a)) != 0) return(rval); 

    dma_b.dma_lbufp= dma_a.dma_lbufp + 16;
    dma_b.dma_pbufp.dma_phys= physAddr + 16;
    dma_b.dma_chan= NI1_DMACHAN2;
    dma_b.dma_mode= (DMA_M_WRITE | DMA_M_AUTOINIT | DMA_M_SINGLE);
    dma_b.dma_bcount= 16;
    dma_b.dma_inuse= 1;
    if((rval= dma_init(&dma_b)) != 0) return(rval); 
    int_a= 0;
    return(0);
}

int
ad_ds_uninit(void)
{

    dma_uninit(&dma_a); dma_uninit(&dma_b);
    dma_a.dma_inuse= 0, dma_b.dma_inuse= 0;

    if(munmap(dma_a.dma_lbufp, 64) == -1) {
		rxerr("ad_ds_uninit(): Cannot unmap a/d DMA segment");
		return(-1);
    }
	/***********************************************************
	close(shm_fd);
    if(ad_sel != -1) {
		if(qnx_segment_free(ad_sel) == -1) {
			rxerr("ad_ds_uninit(): Cannot free a/d DMA segment");
			return(-1);
		}
		ad_sel= -1;
    }
	************************************************************/
    return(0);
}

void
ad_ds_begin(void)
{
    int i;
    u_int ad_vect, utmp;

    /*
     * Init the board.
     */
    ad_ds_end();

    /*
     * Load channel-gain list.
     */
    in8(NI1_CMEMCLR_ROST_8);
    for(i= 0; i < AD_CHANNELS; i++) {
	out16(NI1_CFGMEM_WO, (((ad_cglist[i] & 07) << 3)
			       | (i << 6)
			       | NI1_CHAN_BIP
			       | (i == (AD_CHANNELS-1) ? NI1_CHAN_LAST :
							    0x0)));
    }
    out8(NI1_CMEMLD_WOST_8, 0x0);

    /*
     * Program counter 4- sample counter.
     * Want sampling to continue indefinitely, so program counter 4
     * to have a high output (a falling edge on OUT4 disables counter
     * 3).  Do this by setting up counter to generate a low TC pulse,
     * but dont arm counting!  Output will stay high indefinetly.
     */
    out16(NI1_TCOM_WO, 0xff04);
    out16(NI1_TDATA_RW, 0x5);    /* active low TC */
    out16(NI1_TCOM_WO, 0xff0c);
    out16(NI1_TDATA_RW, 0x10);   /* some positive count */
    out16(NI1_TCOM_WO, 0xff48);  /* load counter 4 */
    out16(NI1_TCOM_WO, 0xfff4);  /* dec counter 4 */

    /*
     * Program counter 3- sample interval counter.
     * Set it to do conversions at a 20Khz rate.  Sample 8 channels
     * every msec.
     */
    out16(NI1_TCOM_WO, 0xff03);
    out16(NI1_TDATA_RW, 0x8c25); /* 100Khz clock input */
    out16(NI1_TCOM_WO, 0xff0b);
    out16(NI1_TDATA_RW, 0x2);
    out16(NI1_TCOM_WO, 0xff44);
    out16(NI1_TCOM_WO, 0xfff3);
    out16(NI1_TDATA_RW, 5);    /* 5 counts @ 100Khz = 20Khz */
    out16(NI1_TCOM_WO, 0xff24);  /* arm counter 3 */

    /*
     * Program counter 2- scan interval counter.
     * Set it to do a scan of all 8 channels.
     */
    out16(NI1_TCOM_WO, 0xff02);
    out16(NI1_TDATA_RW, 0x8d25); /* 10Khz clock input */
    out16(NI1_TCOM_WO, 0xff0a);
    out16(NI1_TDATA_RW, 0x2);
    out16(NI1_TCOM_WO, 0xff42);
    out16(NI1_TCOM_WO, 0xfff2);
    if(ad_max_rate == 1000) {
	out16(NI1_TDATA_RW, 10);    /* 10 counts @ 10Khz = 1Khz */
    } else if(ad_max_rate == 2000) {
	out16(NI1_TDATA_RW, 5);     /* 5 counts @ 10Khz = 2Khz */
    } else {
	rxerr("ad_ds_begin(): Illegal ad_max_rate");
	return;
    }
    out16(NI1_TCOM_WO, 0xff22);  /* arm counter 2 */

    /*
     * Set up a/d.
     */
    Wsetport_WO(NI1_COM2_WO_P, ni1_com2,
		NI1_DMACHAN1 | (NI1_DMACHAN2 << 3), 0x0);   /* dma */
    utmp= AD_VECT;
    switch(utmp) {
	case 3:
		ad_vect= 0;
		break;
	case 4:
		ad_vect= 1;
		break;
	case 5:
		ad_vect= 2;
		break;
	case 7:
		ad_vect= 3;
		break;
	case 10:
		ad_vect= 4;
		break;
	case 11:
		ad_vect= 5;
		break;
	case 12:
		ad_vect= 6;
		break;
	case 15:
		ad_vect= 7;
		break;
	default:
		rxerr("ad_ds_begin(): Illegal value for a/d interrupt.");
    }
    Wsetport_WO(NI1_COM3_WO_P, ni1_com3, ad_vect, 0x0);	/* interrupt level */
    Wsetport_WO(NI1_COM3_WO_P, ni1_com3,
		NI1_ADCREQ | NI1_DMACHB | NI1_DMACHA | NI1_DMATCINT,
		0x0);
    Wsetport_WO(NI1_COM4_WO_P, ni1_com4, NI1_EXTTRIG_DIS, 0x0);
    Wsetport_WO(NI1_COM1_WO_P, ni1_com1, NI1_SCN2 | NI1_SCANEN | NI1_DAQEN,
		0x0);
    in8(NI1_DAQST_ROST_8);	    /* start a/d */

#ifdef GOO
doutput_rot= 0;
{
    int stat_sav= 0;
    int stat_new;

    while(1) {
	stat_new= in16(NI1_STAT1_RO);
	if(stat_new != stat_sav) {
	    dprintf(" %x ", stat_new);
	    stat_sav= stat_new;
	}
    }
}
#endif
}

void
ad_ds_end(void)
{
    int i;
    
    /*
     * Init the board.  This sequence is detailed in the manual on page
     * 5-2 (June 1992 edition, p/n 320488-01).
     */
    Wsetport_WO(NI1_COM1_WO_P, ni1_com1, 0x0, 0xffff);
    Wsetport_WO(NI1_COM2_WO_P, ni1_com2, 0x0, 0xffff);
    Wsetport_WO(NI1_COM3_WO_P, ni1_com3, 0x0, 0xffff);
    Wsetport_WO(NI1_COM4_WO_P, ni1_com4, 0x0, 0xffff);
    in8(NI1_CMEMCLR_ROST_8);
    in8(NI1_DAQCLR_ROST_8);
    out8(NI1_DMATCACL_WOST_8, 0x0);
    in8(NI1_DMATCBCL_ROST_8);
    in8(NI1_DMACCLR_ROST_8);
    in8(NI1_DACCLR_ROST_8);
    in8(NI1_TMRCLR_ROST_8);
    out8(NI1_ADCCAL_WOST_8, 0x0);
    while((in16(NI1_STAT2_RO) & NI1_ADC_BUSY) == 0);

    /*
     * 9513 timer.  No delay needed on this board between accesses.
     */
    out16(NI1_TCOM_WO, 0xffff);
    out16(NI1_TCOM_WO, 0xffef);
    out16(NI1_TCOM_WO, 0xff17);
    out16(NI1_TDATA_RW, 0xf000);
    for(i=1; i <= 5; i++) {
	out16(NI1_TCOM_WO, 0xff00 + i);

	/*
	 * Disable counters 4, 5 (the sample counters- we dont use).
	 */
	if(i == 4  || i == 5) out16(NI1_TDATA_RW, 0x0);
	else out16(NI1_TDATA_RW, 0x4);
	out16(NI1_TCOM_WO, 0xff08 + i);
	out16(NI1_TDATA_RW, 0x3);
    }
    out16(NI1_TCOM_WO, 0xff5f);
}

#endif

/*
 * Analogic DAS12 12 bit a/d converter with 2 12 bit d/as.  Uses
 * a single 16 bit DMA channel.  Also has 16 bits dio.
 * ----------------------------------------------------------------------
 */
#ifdef ANDAS12

/* u_int ad_sel= -1; */
DMA dma_a= {0};
int shm_fd;

int
ad_ds_init(void)
{
/*     struct _seginfo sbuf; */
    int rval;
	long physAddr;
    
    /*
     * Setup DMA controller.  Operate in single DMA mode
     * using 1 16 bit channel.  All 8 channels are sampled and stored
     * each msec.
     */
	if((dma_a.dma_lbufp = mmap(0, 64, PROT_READ | PROT_WRITE | PROT_NOCACHE,
							   MAP_PHYS | MAP_ANON | MAP_BELOW16M | MAP_NOX64K, NOFD, 0)) == MAP_FAILED) {
		rxerr("ad_ds_init(): cannot mmap dma buffer");
	}
	mem_offset(dma_a.dma_lbufp, NOFD, 64, &physAddr, 0);
    /*
     * Get segment for dma buffer.
     */
	/********************************************************************
    if(ad_sel == -1) {
		if((ad_sel= qnx_segment_alloc_flags(64, _PMF_DMA_SAFE)) == -1) {
			rxerr("Cannot alloc a/d DMA segment");
			return(-1);
		}
    }
    if(qnx_segment_info(0, 0, ad_sel, &sbuf) == -1) {
	    rxerr("Cannot 'qnx_segment_info' on a/d DMA segment");
	    return(-1);
    }
	*******************************************************************/
    /*
     * Map segment into process address space.
     */
	/*******************************************************************
    if((shm_fd= shm_open("Physical", O_RDWR, 0)) == -1) {
		rxerr("ad_ds_init(): cannot open PHYSICAL_MEMORY");
		return(-1);
    }
    if((dma_a.dma_lbufp= mmap(0, 64, PROT_READ | PROT_WRITE, MAP_SHARED,
			      shm_fd, sbuf.addr)) == (void *)-1) {
		rxerr("ad_ds_init(): cannot mmap dma buffer");
		close(shm_fd);
	return(-1);
    }
	********************************************************************/
	
    dma_a.dma_pbufp.dma_phys= physAddr;
    dma_a.dma_chan= AN1_DMACHAN;
    dma_a.dma_mode= (DMA_M_WRITE | DMA_M_AUTOINIT | DMA_M_SINGLE);
    dma_a.dma_bcount= 16;
    dma_a.dma_inuse= 1;
    if((rval= dma_init(&dma_a)) != 0) return(rval); 

    return(0);
}

int
ad_ds_uninit(void)
{
    dma_uninit(&dma_a);
    dma_a.dma_inuse= 0;

    if(munmap(dma_a.dma_lbufp, 64) == -1) {
		rxerr("ad_ds_uninit(): Cannot unmap a/d DMA segment");
		return(-1);
    }
	/***********************************************************
	close(shm_fd);
    if(ad_sel != -1) {
		if(qnx_segment_free(ad_sel) == -1) {
			rxerr("ad_ds_uninit(): Cannot free a/d DMA segment");
			return(-1);
		}
		ad_sel= -1;
    }
	***********************************************************/
    return(0);
}

void
ad_ds_begin(void)
{
    u_int ad_vect= 0, ad_dma= 0, utmp;

    ad_ds_end();    /* init board */
    out16(AN1_BDCTRL_RW, AN1_SRESET);	/* enable board */

    /*
     * Program timers 1,2- a/d conversion pacer clock.
     * In burst mode, each tick of timer results in all 8 differential
     * channels being sampled at the max rate of 50Khz.  Therefore,
     * set timer to tick at either 1 or 2Khz rate.  This results in all 8
     * channels being sampled.  On board clock is 1MHz.
     * Board cascades timer 1 and 2 together to get 32 bit counter.
     * Timer 2 is MS word.
     */
    out16(AN1_TCTRL_RW, 0x94);	/* counter 2, LSB write, mode 2, binary */
    out16(AN1_CNTR2_RW, 100);
    out16(AN1_TCTRL_RW, 0x54);	/* counter 1, LSB write, mode 2, binary */
    if(ad_max_rate == 1000) out16(AN1_CNTR1_RW, 10);
    else if(ad_max_rate == 2000) out16(AN1_CNTR1_RW, 5);
    else {
	rxerr("ad_ds_begin(): Illegal ad_max_rate");
	return;
    }
    out16(AN1_INCHCTRL_RW, 0x70);   /* start channel 0, end channel 7 */
    utmp= AD_VECT;
    switch(utmp) {
	case 5:
	    ad_vect= 0x40;
	    break;
	case 7:
	    ad_vect= 0x80;
	    break;
	case 10:
	    ad_vect= 0x100;
	    break;
	case 11:
	    ad_vect= 0x200;
	    break;
	case 15:
	    ad_vect= 0x400;
	    break;
	default:
	    rxerr("ad_ds_begin(): Illegal value for a/d interrupt.");
    }
    utmp= AN1_DMACHAN;
    switch(utmp) {
	case 5:
	    ad_dma= 0x800;
	    break;
	case 6:
	    ad_dma= 0x1000;
	    break;
	case 7:
	    ad_dma= 0x2000;
	    break;
	default:
	    rxerr("ad_ds_begin(): Illegal value for a/d dma chan.");
    }
    Wsetport_RW(AN1_BDCTRL_RW, (ad_dma | ad_vect), 0x0);
    AN1_delay;  /* must initialize dma and vector a little before enabling
		   interrupts */
    out16(AN1_ADCTRL_RW, (AN1_ERRINTENA | AN1_TCINTENA | AN1_BURST
			  | (ad_cglist[0] & 03) << 6));
    Wsetport_RW(AN1_BDCTRL_RW, (AN1_CLADERR | AN1_CLTCDONE | AN1_MINTENA
				| AN1_ADDMAENA | AN1_GO), 0x0);

#ifdef GOO
doutput_rot= 0;
{
    int stat_sav0= 0, stat_sav1= 0;
    int stat_new0, stat_new1;

    while(1) {
	stat_new0= in16(AN1_STATUS_RO);
	if(stat_new0 != stat_sav0) {
	    dprintf(" 0:%x ", stat_new0);
	    stat_sav0= stat_new0;
	}
    }
}
#endif
}

void
ad_ds_end(void)
{
    /*
     * Reset board.
     */
    out16(AN1_BDCTRL_RW, 0x0);
    AN1_delay;
}
#endif


/*
 * Functions for accessing registers on NI E-series cards.  The write-only registers
 * require software copies (see NI E-Series Register Level Programming Manual for
 * more info on programming the registers).  The software copy is in an array indexed by the
 * register's offset.  
 *
 * Note that some routines have critical sections given that these fucntions
 * are called in both the upper and lower levels.  For example, in windowed access mode a critical
 * section exists between loading the address and accessing the data register.  If an interrupt came
 * in this interval and the interrupt routine attempted a windowed access it would interfere with the original
 * access.  Another critical section exists when manipulating the software
 * copies of registers.  We dont protect these routines, however, because it would be very
 * unusual for them to be called simultaneously from the upper and lower levels.  Until the a/d interrupt
 * is enabled, they would not be called from the lower level.  Once the a/d interrupt is enabled it would
 * be rare for them to be called from the upper level (except when turning off the a/d interrupt).
 * However, if one of these routines is called anywhere in the upper level when a/d interrupts are enabled 
 * it must be protected by locking out interrupts.
 * 
 * Some registers must be accessed as a byte, others as a short int.  All functions are inlined for speed.
 */
inline uchar NI_ReadDaqDirByte(NI_ATOD *_ad_p, int _offset) {
   return( *((uchar *)(_ad_p->ni_DaqVar + _offset)) );
}
inline usint NI_ReadDaqDir(NI_ATOD *_ad_p, int _offset) {
   return( *((usint *)(_ad_p->ni_DaqVar + _offset)) );
}
inline usint NI_ReadDaqWin(NI_ATOD *_ad_p, int _offset) {
   *((usint *)(_ad_p->ni_DaqVar))= _offset; /* load window address */
   return( *((usint *)(_ad_p->ni_DaqVar + 2)) );
}
inline void NI_WriteDaqDirByte(NI_ATOD *_ad_p, int _offset, uint _set, uint _clear, uint _strobe) {
   _ad_p->ni_DirScopy[_offset] &= ~(_clear);
   _ad_p->ni_DirScopy[_offset] |= (_set);
   *((uchar *)(_ad_p->ni_DaqVar + _offset))= (uchar)((_ad_p->ni_DirScopy[_offset] | _strobe) & 0xff);
}
inline void NI_StrobeDaqDirByte(NI_ATOD *_ad_p, int _offset, uint _strobe) {
   *((uchar *)(_ad_p->ni_DaqVar + _offset))= (uchar)(_strobe & 0xff);
}
inline void NI_WriteDaqDir(NI_ATOD *_ad_p, int _offset, uint _set, uint _clear, uint _strobe) {
   _ad_p->ni_DirScopy[_offset] &= ~(_clear);
   _ad_p->ni_DirScopy[_offset] |= (_set);
   *((usint *)(_ad_p->ni_DaqVar + _offset))= (usint)((_ad_p->ni_DirScopy[_offset] | _strobe) & 0xffff);
}
inline void NI_StrobeDaqDir(NI_ATOD *_ad_p, int _offset, uint _strobe) {
   *((usint *)(_ad_p->ni_DaqVar + _offset))= (usint)(_strobe & 0xffff);
}
inline void NI_WriteDaqWin(NI_ATOD *_ad_p, int _offset, uint _set, uint _clear, uint _strobe) {
   _ad_p->ni_WinScopy[_offset] &= ~(_clear);
   _ad_p->ni_WinScopy[_offset] |= (_set);
   *((usint *)(_ad_p->ni_DaqVar))= _offset;
   *((usint *)(_ad_p->ni_DaqVar + 2))= (usint)((_ad_p->ni_WinScopy[_offset] | _strobe) & 0xffff);
}
inline void NI_StrobeDaqWin(NI_ATOD *_ad_p, int _offset, uint _strobe) {
   *((usint *)(_ad_p->ni_DaqVar))= _offset;
   *((usint *)(_ad_p->ni_DaqVar + 2))= (usint)(_strobe & 0xffff);
}


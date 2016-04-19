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
 * DMA support using the 8237 controller.
 */
#include <stdio.h>
#include <x86/inline.h>
#include <sys/types.h>
#include "rexhdrs.h"
#include "dma.h"
#include "int.h"

/*
 * Register addresses indexed by channel number.
 */
const DMA_8237 dma_8237[]= {
{0x000, 0x001, 0x087, 0x008, 0x009, 0x00a, 0x00b, 0x00c, 0x00d, 0x00e, 0x00f},
{0x002, 0x003, 0x083, 0x008, 0x009, 0x00a, 0x00b, 0x00c, 0x00d, 0x00e, 0x00f},
{0x004, 0x005, 0x081, 0x008, 0x009, 0x00a, 0x00b, 0x00c, 0x00d, 0x00e, 0x00f},
{0x006, 0x007, 0x082, 0x008, 0x009, 0x00a, 0x00b, 0x00c, 0x00d, 0x00e, 0x00f},
{0x0c0, 0x0c2, 0x08f, 0x0d0, 0x0d2, 0x0d4, 0x0d6, 0x0d8, 0x0da, 0x0dc, 0x0de},
{0x0c4, 0x0c6, 0x08b, 0x0d0, 0x0d2, 0x0d4, 0x0d6, 0x0d8, 0x0da, 0x0dc, 0x0de},
{0x0c8, 0x0ca, 0x089, 0x0d0, 0x0d2, 0x0d4, 0x0d6, 0x0d8, 0x0da, 0x0dc, 0x0de},
{0x0cc, 0x0ce, 0x08a, 0x0d0, 0x0d2, 0x0d4, 0x0d6, 0x0d8, 0x0da, 0x0dc, 0x0de}
};

/*
 * Initialize DMA registers for specified channel.  Channels 0-3
 * are byte channels, channels 5-7 are word channels.  Channel 4
 * cannot be used, it is the cascade input from the first 8237.
 */
int
dma_init(DMA *dma_p)
{
    DMA_8237 *dma_reg;
    short int count;
    DMA_PADDR add_tmp;
    unsigned eflags;

    if(dma_p->dma_inuse == 0) {
	rxerr("dma_init(): unitialized DMA buf passed as arg");
	return(-1);
    }

    if(dma_p->dma_chan >= 0 && dma_p->dma_chan <= 7 && dma_p->dma_chan != 4) {
	dma_reg= &dma_8237[dma_p->dma_chan];

	/*
	 * For 16 bit channels, the count must be a word count.
	 * and the low 16 bits of the address is a word address.
	 */
	count= dma_p->dma_bcount;
	add_tmp.dma_phys= dma_p->dma_pbufp.dma_phys;
	if(dma_p->dma_chan >= 5) {
	    count >>= 1;
	    add_tmp.dma_phys >>= 1;
	}
	count--;
	
	if((eflags = pswget() & INTR_IF)) InterruptDisable();
	out8(dma_reg->dma_wmode, (dma_p->dma_chan & 0x3) | dma_p->dma_mode);
	out8(dma_reg->dma_bptr, 0);	    /* clear byte pointer */
	out8(dma_reg->dma_base, add_tmp.dma_phys_b[0]);
	out8(dma_reg->dma_base, add_tmp.dma_phys_b[1]);
	out8(dma_reg->dma_wc, count);
	out8(dma_reg->dma_wc, (count >> 8) & 0xff);
	out8(dma_reg->dma_page, dma_p->dma_pbufp.dma_phys_b[2]);
	out8(dma_reg->dma_wsmask, (dma_p->dma_chan & 0x3));
	if(eflags & INTR_IF) InterruptEnable();

    } else {
	rxerr("dma_init(): Illegal DMA channel");
	return(-1);
    }
    return(0);
}

int
dma_uninit(DMA *dma_p)
{
    DMA_8237 *dma_reg;

    if(dma_p->dma_inuse == 0) return(-1);
    
    if(dma_p->dma_chan >= 0 && dma_p->dma_chan <= 3) {
	dma_reg= &dma_8237[dma_p->dma_chan];
	out8(dma_reg->dma_wsmask, DMA_SM_RESET | dma_p->dma_chan);

    } else if(dma_p->dma_chan >= 5 && dma_p->dma_chan <= 7) {
	dma_reg= &dma_8237[dma_p->dma_chan];
	out16(dma_reg->dma_wsmask, DMA_SM_RESET | dma_p->dma_chan);

    } else {
	rxerr("dma_init(): Illegal DMA channel");
	return(-1);
    }
    return(0);
}

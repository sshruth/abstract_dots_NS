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
 * DMA support using the 8237 DMA controller.
 */

/*
 * Union to access physical 32 bit address in various ways.
 */
 typedef union {
    paddr_t	dma_phys;		/* physical address of dma buffer */
    u_char	dma_phys_b[4];		/* to access individual bytes */
} DMA_PADDR;

/*
 * This struct contains info necessary to set up a DMA channel.
 * Used by 'dma_init()' in initializing DMA channel.  Read comments
 * carefully.
 */
typedef struct {
    int		dma_inuse;	/* true if struct has been inited */
    caddr_t 	dma_lbufp;	/* logical address of dma buffer */
    DMA_PADDR	dma_pbufp;	/* physical address of dma buffer; this is
				   a byte address, even for channels 5-7;
				   code in dma_init() handles differecnes for
				   16 bit channels */
    int		dma_chan;	/* channel, 0-7 */
    int		dma_bcount;	/* byte count;  place a byte count (not
				   word count) here, even for channels 5-7.
				   code in dma_init() handles subtracting
				   1 as required */
    int		dma_mode;	/* mode; dont put channel in here too, the
				   code in dma_init() handles this */
} DMA;

/*
 * 8237 registers.
 */
typedef struct {
    int	    dma_base;	    /* base address, low order 16 bits */
    int	    dma_wc;	    /* word count */
    int	    dma_page;	    /* upper 4 bits of address */
    int	    dma_csr;	    /* control/status register */
    int	    dma_wreq;	    /* write request */
    int	    dma_wsmask;	    /* write single mask register */
    int	    dma_wmode;	    /* write mode */
    int	    dma_bptr;	    /* byte pointer register */
    int	    dma_mclear;	    /* master clear */
    int	    dma_cmask;	    /* clear mask */
    int	    dma_wamask;	    /* write all mask */
} DMA_8237;

/*
 * Control bits.
 */
#define DMA_M_WRITE	    bit_(2)	/* write to memory */
#define DMA_M_READ	    bit_(3)	/* read from memory */
#define DMA_M_AUTOINIT	    bit_(4)	
#define DMA_M_ADDR	    bit_(5)	/* if set, decrement */
#define DMA_M_SINGLE	    bit_(6)
#define DMA_M_BLOCK	    bit_(7)
#define DMA_M_CASCADE	    (bit_(7)|bit_(6))
#define DMA_M_DEMAND	    0x0

#define DMA_SM_RESET	    bit_(2)	/* reset mask bit */

/*
 * Function prototypes.
 */
int dma_init(DMA *dma_p);
int dma_uninit(DMA *dma_p);


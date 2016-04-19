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
 * Definitions for devices such as a/d, d/a, and digital I/O.
 */

/*
 * The 386 architecture accesses I/O registers via special instructions
 * (in and out).  One therefore cannot change individual bits by
 * or'ing in as on the pdp11 architecture.
 *
 * These macros are for read/write registers.  These are registers which
 * keep the state of bits that are written, and can be read back to determine
 * bits that have been set.
 
 * NOTE:  It is not safe to use the forms 'port++', etc.
 * for 'port'- it is evaluated twice.
 */
#define Wsetport_RW(port, bit_set, bit_clr) \
	(out16(port, (in16(port) & (~bit_clr)) | (bit_set)))
#define Bsetport_RW(port, bit_set, bit_clr) \
	(out8(port, (in8(port) & (~bit_clr)) | (bit_set)))

/*
 * These macros are for registers which are write-only.
 * These are registers that read back bits other than those that were
 * set on a write.
 *
 * In the register definitions for various devices, suffix '_RW' signifies
 * a read/write register which reads back any bits that are currently set.
 * Registers that are write only are designated '_WO', read only '_RO'.
 * A register that needs to have a software prototype is designated with
 * suffix '_P'.  It should only be accessed with the following macros.
 */
#define Wsetport_WO(port, save_val, bit_set, bit_clr) \
	save_val &= ~(bit_clr); save_val |= (bit_set); out16(port, save_val)
#define Bsetport_WO(port, save_val, bit_set, bit_clr) \
	save_val &= ~(bit_clr); save_val |= (bit_set); out8(port, save_val)
#define bit_(number)	(0x1 << number)

/*
 * Prototypes for digital I/O functions.
 */
typedef unsigned long DIO_ID;
void dio_init(void);			/* initialize index list */
int dio_in(DIO_ID);			/* input from digital I/O port */
int dio_on(DIO_ID);			/* turn on bit */
int dio_off(DIO_ID);			/* turn off bit */  
int dio_onoff(DIO_ID, DIO_ID);		/* turn on/off bits */	
int dio_out(DIO_ID, long value);	/* output value to port */
int Pdio_on(DIO_ID *id);
int Pdio_off(DIO_ID *id);
int Pdio_onoff(DIO_ID *id_on, DIO_ID *id_off);
int Pdio_out(DIO_ID *id, long *value);
#define D_ON	1			/* defines for 'flag' below */
#define D_OFF	2
#define D_VAL	3
int dio_set(DIO_ID, int value, int flag);   /* routine that is called to
					       do the ouptut */
/*
 * Argument definition for function that outputs to d/a converters.
 */
typedef struct {
    DIO_ID  dio_da_id;			/* dio id for d/a converter */
    int     dio_da_value;		/* value */
    int	    dio_da_flag;		/* flag */
    int	    *dio_da_curvalp;		/* pointer to place to store current
					   value of d/a after it has been
					   updated */
} DIO_DA_ARG;
/* int dio_da(int cnt, DIO_DA_ARG far *ap); */
int dio_da(int cnt, DIO_DA_ARG *ap);

#define D_NOMAP	0x1			/* don't map from internal calib
					   when outputting to d/a */
/*
 *-----------------------------------------------------------------------*
 *			D/A CONVERTERS
 *-----------------------------------------------------------------------*
 */
/*
 * Each digital-to-analog converter is controlled by an instance
 * of the following structure.  d/a's are named by their index
 * in the following array of structs; the 'da_id' is specified in "int/cnf.c".
 * Note: this array of structs is initialized in 'da_init()', which is
 * called from main.c.  Any items added to this struct should also
 * be added to 'da_init()' to be initialized.
 */
struct da_struct {
    DIO_ID  da_id;	/* DIO_ID of this converter */
    int	    da_clear;	/* if non-zero, clear the outputs of this d/a when
			   first init'ed;  this is set in cnf.c */
    int	    m_da_menu;	/* menu override flag */
    int	    da_source;	/* source */
    int	    m_da_source;
    int	    da_src_num;	/* source number specification */
    int	    m_da_src_num;
    int	    da_mode;	/* output mode */
    int	    m_da_mode;
    int	    m_da_val;
    int	    da_offset;	/* offset to be applied to output */
    int	    m_da_offset;
    int	    da_curval;	/* current position of d/a */
    void    *da_validp;	/* pointer to valid flag- when what is pointed to is
			   0 output is not valid; does not apply when source
			   is DA_STBY */
    void    *da_posp;	/* pointer to source */
    struct da_struct *da_back;	/* backward pointer to previous active d/a */
    struct da_struct *da_for;	/* forward pointer to next active d/a */
};
typedef struct da_struct DA;
extern DA da[DA_MAXNUM];
extern DA da_clr;
extern DA da_first, da_last;
extern int m_da_cur_xda, m_da_cur_yda;
extern int da_cur_xda, da_cur_yda;					   
extern DA *da_cur_xda_p, *da_cur_yda_p;
extern int m_oda_cur_xda, m_oda_cur_yda;
extern int oda_cur_xda, oda_cur_yda;					   
extern DA *oda_cur_xda_p, *oda_cur_yda_p;

/*
 * Values for da_source.
 */
#define DA_STBY		0	/* d/a is not currently connected; can still
				   be driven directly by 'da_set()' actions */
#define DA_RAMP_X	1
#define DA_RAMP_Y	2
#define DA_JOY_X	3
#define DA_JOY_Y	4
#define DA_WIND_X	5
#define DA_WIND_Y	6
#define DA_SIGNAL	7	/* d/a connected to specified signal */
#define DA_MEM		8

/*
 * Values for da_mode.
 */
#define DA_DIRECT	0x0	/* direct output */
#define DA_OFFSET_CONN	0x1	/* apply offset to each output value for
				   all connected sources (i.e. everything but
				   DA_STBY) */
#define DA_OFFSET_STBY	0x2	/* apply offset when source is DA_STBY; note
				   that both DA_OFFSET_CONN and DA_OFFSET_STBY
				   can be set simultaneously */

/*
 * Argument structs for d/a functions.
 */
typedef struct {
    int	    da_cntrl_num;		/* d/a num */
    int	    da_cntrl_src;		/* source */
    int	    da_cntrl_src_num;		/* source num */
} DA_CNTRL_ARG;
/* int da_cntrl(int cnt, DA_CNTRL_ARG far *ap); */
int da_cntrl(int cnt, DA_CNTRL_ARG *ap);

typedef struct {
    int	    da_set_num;			/* d/a num */
    int	    da_set_val;			/* output value */
} DA_SET_ARG;
/* int da_set(int cnt, DA_SET_ARG far *ap); */
int da_set(int cnt, DA_SET_ARG *ap);

/*
 *-----------------------------------------------------------------------*
 *			A/D CONVERTERS
 *-----------------------------------------------------------------------*
 */


/*
 * National Instruments AT-MIO-16X: 16 bit a/d, 2 12 bit d/a, 8 bits dio.
 * ---------------------------------------------------------------------
 */
#ifdef NIATMIOX
#define NI1_COM1_WO_P	(NI1_PORT)
extern u_int ni1_com1;			/* software prototype */
#define NI1_CNT32	bit_(5)	/* sample count, 32 or 16 bits */
#define NI1_SCN2	bit_(6)	/* interval scanning mode */
#define NI1_SCANEN	bit_(7)	/* scan enable */
#define NI1_DAQEN	bit_(8)	/* enable timer controlled acquisitions */

#define NI1_COM2_WO_P	(NI1_PORT+0x2)	/* write only */
extern u_int ni1_com2;		/* software prototype */

#define NI1_COM3_WO_P	(NI1_PORT+0x4)	/* write only */
extern u_int ni1_com3;		/* software prototype */
#define NI1_DRVAIS	bit_(3)	/* drive analog input sense */
#define NI1_ADCREQ	bit_(6)	/* ADC request enable */
#define NI1_DMACHB	bit_(7)	/* dma channel B enable */
#define NI1_DMACHA	bit_(8)	/* dma channel A enable */
#define NI1_IO_INT	bit_(9)	/* input/Output interrupt enable */
#define NI1_DMATCINT	bit_(12)/* DMA T/C interrupt enable */
#define NI1_DIOPAEN	bit_(13)/* dio port A set to output */
#define NI1_DIOPBEN	bit_(14)/* dio port B set to output */

#define NI1_COM4_WO_P	(NI1_PORT+0x6)	/* write only */
extern u_int ni1_com4;		/* software prototype */
#define NI1_EXTTRIG_DIS	bit_(0)	/* disable external triggers */
#define NI1_ADCFIFOREQ	bit_(4)

#define NI1_STAT1_RO	(NI1_PORT+0x18)	/* read only */
#define NI1_OVERRUN	bit_(8)	/* data overrun */
#define NI1_OVERFLOW	bit_(9)	/* FIFO overflow */
#define NI1_DMATCB	bit_(10)/* dma T/C channel B */
#define NI1_DMATCA	bit_(11)/* dma T/C channel A */
#define NI1_DAQCOMP	bit_(15)/* data acquisition copmlete */

#define NI1_STAT2_RO	(NI1_PORT+0x1a)	/* read only */
#define NI1_ADC_BUSY	bit_(0)

#define NI1_ADCFIFO_RO	(NI1_PORT+0x0)	/* read only */

#define NI1_CFGMEM_WO	(NI1_PORT+0x8)	/* write only */
#define NI1_CHAN_LAST	bit_(2)	/* set for last chan of config mem */
#define NI1_CHAN_BIP	bit_(12)/* bipolar */

#define NI1_DAC0_WO	(NI1_PORT+0x10)	/* write only */
#define NI1_DAC1_WO	(NI1_PORT+0x12)	/* write only */

#define NI1_CMEMCLR_ROST_8  (NI1_PORT+0x1b) /* read-only strobe, 8 bits */
#define NI1_CMEMLD_WOST_8   (NI1_PORT+0x1b) /* write-only strobe, 8 bits */
#define NI1_DAQCLR_ROST_8   (NI1_PORT+0x19) /* read-only strobe, 8 bits */
#define NI1_DAQST_ROST_8    (NI1_PORT+0x1d) /* read-only strobe, 8 bits */
#define NI1_SINGLE_WOST_8   (NI1_PORT+0x1d) /* write-only strobe, 8 bits */
#define NI1_ADCCAL_WOST_8   (NI1_PORT+0x1f) /* write-only strobe, 8 bits */
#define NI1_TMRCLR_ROST_8   (NI1_PORT+0x1f) /* read-only strobe, 8 bits */
#define NI1_DACUPD_WOST_8   (NI1_PORT+0x18) /* write-only strobe, 8 bits */
#define NI1_DACCLR_ROST_8   (NI1_PORT+0x1e) /* read-only strobe, 8 bits */
#define NI1_DMACCLR_ROST_8  (NI1_PORT+0x0b) /* read-only strobe, 8 bits */
#define NI1_DMATCACL_WOST_8 (NI1_PORT+0x19) /* write-only strobe, 8 bits */
#define NI1_DMATCBCL_ROST_8 (NI1_PORT+0x09) /* read-only strobe, 8 bits */
#define NI1_EXTSTB_WOST_8   (NI1_PORT+0x1e) /* write-only strobe, 8 bits */
#define NI1_DAC0CAL_WOST_8  (NI1_PORT+0x0a) /* write-only strobe, 8 bits */
#define NI1_DAC1CAL_WOST_8  (NI1_PORT+0x1a) /* write-only strobe, 8 bits */

#define NI1_TDATA_RW    (NI1_PORT+0x14) /* read and write */
#define NI1_TCOM_WO	(NI1_PORT+0x16)	/* timer command on writes */
#define NI1_TSTAT_RO	(NI1_PORT+0x16)

#define NI1_DI_RO	(NI1_PORT+0x1c)	/* digital input */
#define NI1_DO_WO	(NI1_PORT+0x1c)	/* digital out */

#define NI1_RTSISW_WO_8		(NI1_PORT+0x0c)	/* 8 bit port- write only */
#define NI1_RTSIST_WOST_8	(NI1_PORT+0x0e)	/* 8 bit port- write only */
#endif 


/*
 * Analogic DAS-12 12 bit a/d with 2 12 bit d/a converters.
 * ---------------------------------------------------------------------
 */
#ifdef ANDAS12
#define AN1_BDCTRL_RW	(AN1_PORT+0x0)	/* board control */
#define AN1_SRESET	bit_(0)		/* software reset */
#define AN1_CLADERR	bit_(1)		/* clear a/d error */
#define AN1_CLTCDONE	bit_(2)		/* clear tc done */
#define AN1_MINTENA	bit_(4)		/* master interrupt enable */
#define AN1_ADDMAENA	bit_(5)		/* a/d dma enable */
#define AN1_GO		bit_(15)	/* go bit */

#define AN1_INCHCTRL_RW	(AN1_PORT+0x2)	/* input channel control reg */

#define AN1_ADCTRL_RW	(AN1_PORT+0x4)	/* a/d control register */
#define AN1_ERRINTENA	bit_(10)
#define AN1_TCINTENA	bit_(11)	/* tc interrupt enable */
#define AN1_DIO0	bit_(12)	/* dio0 direction */
#define AN1_DIO1	bit_(13)	/* dio1 direction */
#define AN1_BURST	0xc000		/* burst mode bits */

#define AN1_ADDATA_RO	(AN1_PORT+0x6)	/* a/d data */
#define AN1_DAC0_WO	(AN1_PORT+0x8)	/* dac0 output data */
#define AN1_DAC1_WO	(AN1_PORT+0xa)	/* dac1 output data */
#define AN1_DIO_RW	(AN1_PORT+0xc)	/* dio port */

#define AN1_STATUS_RO	(AN1_PORT+0xe)	/* board status register */
#define AN1_TCDONE	bit_(5)		/* tc reached */
#define AN1_ADERR	bit_(4)		/* a/d error */

#define AN1_CNTR0_RW	(AN1_PORT+0x10)	/* counter 0 port */
#define AN1_CNTR1_RW	(AN1_PORT+0x12)	/* counter 1 port */
#define AN1_CNTR2_RW	(AN1_PORT+0x14)	/* counter 2 port */
#define AN1_TCTRL_RW	(AN1_PORT+0x16)	/* counter control port */

/*
 * Short delay.
 */
#define AN1_delay \
        in16(AN1_STATUS_RO),in16(AN1_STATUS_RO),in16(AN1_STATUS_RO),\
	in16(AN1_STATUS_RO)
#endif

/*
 * National Instruments E-series PCI a/d converters.
 * ---------------------------------------------------------------------
 */

/*
 * Header for National Instruments PCI E-series a/d cards.  This code
 * supports multiple cards on the same machine.
 */

#define NI_MAX_NUMBER		 3 	/* max number of NI a/d's on machine */
#define NI_DIR_SCOPY_NUM	 40 	/* size of array of shorts for software copies
	        			 * of registers.  Must be greater than largest
	  			         * offset */
#define NI_WIN_SCOPY_NUM	 100

/*
 * There is one instance of this structure for each National Instruments
 * a/d converter.
 */
typedef struct {
   int	 ni_type;		/* type of NI a/d */
   int	 ni_irq;		/* assigned irq */
   uint  ni_MiteVar;		/* mite virtual address */
   uint	 ni_MiteBar;		/* mite bar */
   uint  ni_DaqVar;		/* daq virtual address */
   uint  ni_DaqBar;		/* daq bar */
   usint ni_DirScopy[NI_DIR_SCOPY_NUM];	/* software copies for direct access */
   usint ni_WinScopy[NI_WIN_SCOPY_NUM];	/* software copies for windowed access */
} NI_ATOD;

extern NI_ATOD *ni_adp;

/*
 * Supported NI a/d cards, put in 'ni_type' above.
 */
#define NI_PCIMIO16E1	1
#define NI_PCI6032E	2
#define NI_PCI6033E	3
#define NI_PCI6023E	4

/*
 * Functions for accessing registers.  The write-only registers
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
inline uchar NI_ReadDaqDirByte(NI_ATOD *_ad_p, int _offset);
inline usint NI_ReadDaqDir(NI_ATOD *_ad_p, int _offset);
inline usint NI_ReadDaqWin(NI_ATOD *_ad_p, int _offset);
inline void NI_WriteDaqDirByte(NI_ATOD *_ad_p, int _offset, uint _set, uint _clear, uint _strobe);
inline void NI_StrobeDaqDirByte(NI_ATOD *_ad_p, int _offset, uint _strobe);
inline void NI_WriteDaqDir(NI_ATOD *_ad_p, int _offset, uint _set, uint _clear, uint _strobe);
inline void NI_StrobeDaqDir(NI_ATOD *_ad_p, int _offset, uint _strobe);
inline void NI_WriteDaqWin(NI_ATOD *_ad_p, int _offset, uint _set, uint _clear, uint _strobe);
inline void NI_StrobeDaqWin(NI_ATOD *_ad_p, int _offset, uint _strobe);
   
/*
 * DAQ registers, mapped by BAR1.
 * Key- RW: read/write, RO: read only, WO: write only, DIR: directly accessible
 * register, 8: byte access only, PR: pair of registers.  Assumed if no 
 * extension: 16 bit access, windowed mode.
 */

/*
 * Offsets to directly accessible registers.  These offsets are all decimal byte offsets,
 * and are used in reading directly from the PCI memory space.  Listed in order as shown
 * in RLPM.
 */
#define SER_COM_WO_8_DIR	13
#define MISC_COM_WO_8_DIR	15
#define STATUS_RO_8_DIR		1
#define ADC_FIFO_DATA_RO_DIR	28
#define CONFIG_MEM_LOW_WO_DIR	16
#define		Unip_Bip			0x100
#define		DitherEn			0x200
#define		LastChan			0x8000
#define CONFIG_MEM_HI_WO_DIR	18
#define 	Diff_Channel_Type		0x1000
#define AO_CONFIG_WO_DIR	22
#define DAC_FIFO_DATA_WO_DIR	30
#define DAC0_DIR_DATA_WO_DIR	24
#define DAC1_DIR_DATA_WO_DIR	26
#define AI_AO_SEL_WO_8_DIR	9
#define G0_G1_SEL_WO_8_DIR	11
#define WIN_ADD_RW_DIR		0
#define WIN_DATA_RW_DIR		2
#define INT_A_ACK_WO_DIR	4
#define 	AI_SC_TC_Error_Confirm_STRB	0x80
#define 	AI_SC_TC_Interrupt_Ack_STRB	0x100
#define		AI_START_Interrupt_Ack_STRB	0x800
#define 	AI_STOP_Interrupt_Ack_STRB	0x1000
#define INT_B_ACK_WO_DIR	6
#define AI_COM_2_WO_DIR		8
#define 	AI_START1_Pulse_STRB		0x1
#define AO_COM_2_WO_DIR		10
#define G0_COM_WO_DIR		12
#define G1_COM_WO_DIR		14
#define AI_STATUS_1_RO_DIR	4
#define 	AI_FIFO_Request_St		0x1
#define 	AI_STOP_St			0x10
#define 	AI_START_St			0x20
#define 	AI_SC_TC_St			0x40
#define 	AI_SC_TC_Error_St		0x200
#define		AI_Overflow_St			0x400
#define 	AI_Overrun_St			0x800
#define 	AI_FIFO_Empty_St		0x1000
#define 	AI_FIFO_Half_Full_St		0x2000
#define 	AI_FIFO_Full_St			0x4000
#define 	Interrupt_A_St			0x8000
#define AO_STATUS_1_RO_DIR	6
#define G_STATUS_RO_DIR		8
#define AI_STATUS_2_RO_DIR	10
#define AO_STATUS_2_RO_DIR	12
#define DIO_PAR_IN_RO_DIR	14

/*
 * Offsets to registers accessed by windowed mode.  These offsets are all
 * decimal short word offsets.  Listed in order as shown in DAQ-STC Reference
 * Manual, appendix B.  The WO STC registers that are also available by direct
 * access to the PCI memory space are not repeated here.  This is so that there
 * are not two ways to access these registers and therefore software copies that could
 * get out of sync.
 */
#define AI_COM_1_WO		8
#define 	AI_CONVERT_Pulse_STRB		0x1
#define 	AI_SC_TC_Pulse			0x2
#define 	AI_SC_Load_STRB			0x20
#define		AI_SC_Arm_STRB			0x40
#define		AI_SI_Load_STRB			0x200
#define		AI_SI_Arm_STRB			0x400
#define		AI_SI2_Load_STRB		0x800
#define		AI_SI2_Arm_STRB			0x1000
#define AI_DIV_LD_A_WO		64
#define AI_DIV_SAV_RO		26
#define AI_MODE_1_WO		12
#define 	AI_Trigger_Once			0x1
#define		AI_Continuous			0x2
#define 	Reserved_One			0x4
#define		AI_Start_Stop			0x8
#define AI_MODE_2_WO		13
#define		AI_SI2_Reload_Mode		0x100
#define		AI_SI2_Initial_Load_Source	0x200
#define AI_MODE_3_WO		87
#define AI_OUTPUT_CONTROL_WO	60
#define AI_PERSONAL_WO		77
#define		AI_LOCALMUX_CLK_Pulse_Width	0x20
#define 	AI_Overrun_Mode			0x80
#define 	AI_CONVERT_Pulse_Width		0x400
#define		AI_SOC_Polarity			0x2000
#define		AI_SHIFTIN_Pulse_Width		0x8000
#define AI_SC_LD_A_WO_PR	18
#define AI_SC_LD_B_WO_PR	20
#define AI_SC_SAV_RO_PR		66
#define AI_SI_LD_A_WO_PR	14
#define AI_SI_LD_B_WO_PR	16
#define AI_SI_SAV_RO_PR		64
#define AI_SI2_LD_A_WO		23
#define AI_SI2_LD_B_WO		25
#define AI_SI2_SAV_RO		25
#define AI_START_STOP_SEL_WO	62
#define 	AI_START_Edge			0x20
#define		AI_START_Sync			0x40
#define		AI_STOP_Sync			0x2000
#define AI_TRIG_SEL_WO		63
#define 	AI_START1_Edge			0x20
#define 	AI_START1_Sync			0x40
#define AN_TRIG_ETC_WO		61
#define CLOCK_FOUT_WO		56
#define 	AI_Output_Divide_By_2		0x80
#define		Clock_To_Board			0x100
#define		Clock_To_Board_Divide_By_2	0x200
#define 	Slow_Internal_Timebase		0x800
#define 	Slow_Internal_Time_Divide_By_2	0x1000
#define GENERIC_CTL_WO		71
#define INT_A_ENA_WO		73
#define 	AI_SC_TC_Interrupt_Enable	0x1
#define 	AI_START1_Interrupt_Enable	0x2
#define 	AI_START_Interrupt_Enable	0x8
#define		AI_STOP_Interrupt_Enable	0x10
#define		AI_Error_Interrupt_Enable	0x20
#define INT_B_ENA_WO		75
#define INT_CTL_WO		59
#define 	Interrupt_Output_Polarity	0x1
#define		Interrupt_A_Enable		0x800
#define IO_BIDIR_PIN_WO		57
#define JOINT_RESET_WO		72
#define 	AI_Reset_STRB			0x1
#define 	AI_Configuration_Start_STRB	0x10
#define		AI_Configuration_End_STRB	0x100
#define JOINT_STAT_1_RO		27
#define JOINT_STAT_2_RO		29
#define SEC_IRQ_A_ENA_WO	74
#define SEC_IRQ_B_ENA_WO	76
#define CONFIG_MEM_CLEAR_WO	82
#define ADC_FIFO_CLEAR_WO	83
#define DAC_FIFO_CLEAR_WO	84
#define WRITE_STB_3_WO		85

/*
 *-----------------------------------------------------------------------*
 *			DIGITAL I/O
 *-----------------------------------------------------------------------*
 */

/*
 * Output to digitial I/O devices is handled by routines that use
 * a 'DIO_ID' as an argument.  'DIO_ID' is a 32 bit long with
 * following description:
 *
 *       31		    HIGH WORD                 16
 *	 ___________-___________-___________-___________
 *	|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|
 *		  Device       	|    Port or d/a num
 *
 *       15		     LOW WORD                  0
 *	 ___________-___________-___________-___________
 *	|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|
 *			bit pattern or mask
 *
 * This programming interface yields 8 bits for 'device', which is
 * typically a card in the PC, and 8 bits to select the 'port' for cards
 * that implement multiple ports.  For the functions 'dio_on', 'dio_off',
 * and 'dio_onoff', the low 16 bits hold a bit pattern
 * which is output to the specified port.  For the functions 'dio_in' and
 * 'dio_out', the lower 16 bits are a bit mask.  For 'dio_in', this mask
 * is bitwise-anded with the input before being returned.  For 'dio_out',
 * this mask is bitwise-anded with the value before being output.
 *
 * At the beginning of a Spot file, one typically includes a header that
 * defines the 'DIO_ID' of devices in the laboratory by using the
 * following macro:
 *
 *	Dio_id(device, port, bit_pattern)
 *
 * As an example, suppose an LED is connected in one laboratory to
 * the digital I/O port on a Data Translation DT2821 a/d converter
 * as bit 6.  This LED would be defined as:
 *
 *  #define LED_FP0	Dio_id(DT2821_DIO, 0, 0x40)
 * 
 * In another laboratory, this LED might be connected to bit 2 on port B
 * on Metrabyte PIO-12 card number 0.  It would then be defined as:
 *
 *  #define LED_FP0	Dio_id(KMPIO120_DIO, 1, 0x4)
 *
 * These definitions could be used in a Spot file with the following
 * three actions:
 *
 *	    dio_on(LED_FP0)		* turn on LED_FP0) *
 *	    dio_off(LED_FP0)		* turn off LED_FP0 *
 *	    dio_onoff(LED_FP0, LED_FP1)	* turns FP0 on, FP1 off *
 *
 * The above functions keep a software copy of the state of the port.
 * Turning on or off a bit does not affect the state of other bits in
 * the port.
 *
 * In addition, an bit pattern can be sent directly to a port by
 * using the following action:
 *
 *	    dio_out(DIO_ID, output_data)	* output 'value' *
 * 
 * The above function does not keep a software copy of the state of the
 * port.  The 'output_data' is written as is directly to the port.
 * The functions 'dio_on', dio_off', and 'dio_onoff' all call this
 * function to do the actual output.
 *
 * The following example is for 2 12 bit d/a converters.
 *
 *  #define DA_X	Dio_id(DIO_DT2821_DA, 0, 0xfff)
 *  #define DA_Y	Dio_id(DIO_DT2821_DA, 1, 0xfff)
 *
 * This programming interface allows the body of Spot files to remain
 * unchanged when used in different laboratories.  Only the definitions
 * for the device ids needs to be changed in the headers.
 *
 * Digital input is not currently handled with such a lab-independent
 * programming interface.  Every msec, the interrupt routine reads 16 bits of
 * digital input and stores the result in variable 'dina' (a #define
 * also allows this variable to be accessed as 'drinput' for backward
 * compatability).  Bits in this variable can then be tested for on/off
 * from Spot files, e.g. "to nextstate on +BAR & drinput".
 */
#define DIO_DEV_DEVMAX		256		/* max number of devices */
#define DIO_PORT_PORTMAX	256		/* max number of ports */
#define DIO_DEV_MASK	(0xff000000)    /* device id mask and shift factor */
#define DIO_DEV_SHIFT	24
#define DIO_PORT_MASK	(0xff0000)	/* port id mask and shift factor */
#define DIO_PORT_SHIFT	16
#define DIO_DATA_MASK	(0xffff)
#define DIO_DATA_SHIFT	0

/*
 * Macros.
 */
#define Dio_mk_dev(x)	((((long)x) << DIO_DEV_SHIFT) & DIO_DEV_MASK)
#define Dio_mk_port(x)	((((long)x) << DIO_PORT_SHIFT) & DIO_PORT_MASK)
#define Dio_id(dev, port, bit_pat) \
		((Dio_mk_dev(dev)) | (Dio_mk_port(port)) | (bit_pat))
#define Dio_dev(id)	((int)((id & DIO_DEV_MASK) >> DIO_DEV_SHIFT))
#define Dio_port(id)	((int)((id & DIO_PORT_MASK) >> DIO_PORT_SHIFT))
#define Dio_data(id)	((int)((id & DIO_DATA_MASK) >> DIO_DATA_SHIFT))

/*
 * The following array of unsigned chars gives specific info about
 * each port.
 *		     ______________-________
 *		    |__|__|__|__|__|__|__|__|
 *			  Flags      Type     	
 *
 * First three bits encoded as type of port.
 */
#define Dio_type(x)	((x) & 0x7)
#define D_NUL		0	/* terminates port list */
#define D_IN		1	/* input */
#define D_OUT		2	/* output */
#define D_CTRL		3	/* control port */
#define D_DA		4	/* d/a port */
#define D_RES		5	/* port reserved for other uses- don't
			             * initialize (e.g. pcmessage) */
#define D_IN_OUT	6	/* port C-hi in, port C-lo out */
#define D_OUT_IN	7	/* port C-hi out, port C-lo in */

/*
 * Remaining five bits are flags.
 */
#define D_WORD		0x8	/* access port via word access */

#ifdef DIO_TRUE_LOW
#define D_REV		0x10
#else
#define D_REV		0x0
#endif

#define D_NOCLR		0x20	/* if an output port, dont set to off
				   state when device is initialized or
				   during a 'reset bit' command
				   (default case is to set to off) */

/*
 * Flags for 'dio_flags'.  These flags apply to device as a whole, i.e.
 * all the ports.
 */
#define D_INIT		0x1	/* set when device has been inited */
typedef struct {
    int	    dio_devid;	    /* device id for this device; array terminates
			       when this field is zero */
    int	    dio_flags;	    /* flags, see above */
    int	    dio_port_base;  /* port base address */
    int	    dio_pciRexUse;  /* rexuse field from PCI struct if this is a PCI device */
    int	    dio_vend;	    /* if PCI device, vendor id */
    int	    dio_device;	    /* if PCI device, device id */
    u_char  dio_pinit[DIO_PORT_PORTMAX];  /* for cards whose ports need to
					     be configured for input or
					     output */
    u_int   dio_state[DIO_PORT_PORTMAX];  /* software copy of current state
					     of port */
    void    (*dio_init_f)();  /* function to call for initialization */
    void    (*dio_uninit)();/* function to call for uninitialization */
} DIO_DEV;
extern DIO_DEV dio_dev[];

/*
 * An a/d converter may support different max sampling rates depending
 * on how many channels are sampled.  This array specifies various
 * workable configurations for an individual converter.  For each possible
 * maximum sampling rate, a max number of channels that can be acquired
 * is specified.
 *
 * Note: REX currently supports only one a/d at a time.  To support
 * multiple a/ds, this array should be expanded to include all the
 * configuration info now stored in hdr/cnf.h.
 */
typedef struct {
    int	    ad_samp_max;    /* max sampling rate */
    int	    ad_samp_num;    /* number of channels at above max sampling rate */
} AD_SAMP;
extern AD_SAMP ad_samp[];

/*
 * Gain lists.  Some a/d converters have channel-gain lists,
 * which allow a different gain for each channel.
 * Different a/d's have provide different gains, e.g.
 * the ANDAS12 has gains of 1, 2, 4, 8 and the ATMIO16-X has gains
 * of 1, 2, 5, 10, 20, etc.
 */
typedef struct {
    int	ad_gain_factor;	/* gain, -1 terminates array */
    int ad_gain_bits;	/* bits to load into register for this gain */
} AD_GAIN;
extern AD_GAIN ad_gain[];

/*
 * This array contains ad_gain_bits indexed by channel for converters
 * with channel-gain lists.  For converters with one global gain,
 * only first element is used.
 */
extern int ad_cglist[AD_CHANNELS];

/*
 * A/D calibration struct.  Specifies shift factors used to scale
 * a/d inputs for various full scale calibrations.
 */
typedef struct {
    int	ad_ca_num;	/* calibration number; -1 terminates array */
    int ad_ca_shift;	/* shift factor */
    char ad_ca_fullscale[P_LNAME];    /* full scale range */
} AD_CALIB;
extern AD_CALIB ad_calib[SA_MAXCAL];

/*
 * Array holds all valid sampling rates.  Initialized by
 * sig_compute_ad_rates().
 */
typedef struct {
    int	ad_acq_rate;	    /* possible sampling rate */
    int ad_acq_bin_const;   /* constant used in sig_init() to compute sa_frame
			       array */
} AD_ACQ_RATES;
extern AD_ACQ_RATES ad_acq_rates[SA_SPREAD_LOG2+1];

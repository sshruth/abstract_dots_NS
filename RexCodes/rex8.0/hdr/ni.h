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


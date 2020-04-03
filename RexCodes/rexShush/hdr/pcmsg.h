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
 *	pcmsg.h: PC to PC Messaging
 */

/*
 *---------------------------------------------------------------------------*
 *			User configurable options			     *
 *---------------------------------------------------------------------------*
 */
/*
 * Define one (and only one) of the following to describe the
 * C compiler and operating system in use.
 */
#define GCC_RTP_REX     /* Gnu C compiler under QNX Neutrino */
#undef  WCC_QNX_REX	    /* Watcom C under QNX compiled with REX */
#undef  WCC_QNX_SA	    /* Watcom C under QNX compiled standalone
			       (not under REX) */
#undef  BORLAND_DOS	    /* Borland under DOS */
#undef  MICROSOFT_DOS	    /* Microsoft under DOS */
#undef  MWC_PPC		    /* Metrowerks C on Power PC */

/*
 * PCM_DEBUG can be defined to 1, 2, or 3 for increasing levels
 * of debugging info.  At level 1, the times for printing/receiving
 * messages are printed under REX (but not on the DOS side).
 */
#define PCM_DEBUG 0
#define PCM_NUMCHAN 1       /* Max number of communications channels.
			       Each full duplex hardware connection is
			       one channel.  It is possible to have multiple
			       channels to multiple PCs, or to have more than
			       one channel between just two PCs.  Dont define
			       higher than the actual number of existing
			       hardware connections- additional cycles are
			       needed for each channel. */
#define PCM_RBUFSIZ 500     /* Default max size of channel receive buffers.
			       This value determines the max size of an 
			       incoming message.  Each channel has its own
			       receive buffer.  Buffer sizes are independent-
			       if one channel receives large messages it 
			       can be set to a different size than the
			       default.  Declarations are in pcmsg.c */
#define PCM_TESTMAX 10      /* number of times to test a handshake bit before
			       giving up and waiting until next interrupt;
			       each test consumes 800nsec since it requires
			       an (E)ISA bus I/O instruction */
#define PCM_TRANSMAX 64     /* Max number of bytes transfered per channel per
			       interrupt;  if -1 than unlimited.  For example,
			       assume two channels are active on both send and
			       receive.  If PCM_TRANSMAX is defined as 64 then
			       *each* channel will transmit *and* receive 64
			       bytes before activity is be suspended until the
			       next interrupt. This prevents long messages from
			       locking out other processing. */

/*
 *---------------------------------------------------------------------------*
 * End of user configurable options.					     *
 *---------------------------------------------------------------------------*
 */
#ifdef GCC_RTP_REX
#define GCC_RTP
#define UNDER_REX
#endif
#ifdef WCC_QNX_REX
#define WCC_QNX
#define UNDER_REX
#endif
#ifdef WCC_QNX_SA
#define WCC_QNX
#undef UNDER_REX
#endif
#ifdef MWC_PPC
typedef char *ADDR;
#else
typedef int ADDR;
#endif
typedef unsigned short int uns_sh;
#define PCM_ushorthi(a)	(unsigned short)((a >> 8) & 0xff)
#define PCM_ushortlo(a)	(unsigned short)(a & 0xff)
#define PCM_setflag(p, bit_set, bit_clr) \
	p->pcm_flag_save &= ~(bit_clr); p->pcm_flag_save |= (bit_set); \
	OUTB(p->pcm_flag_port, p->pcm_flag_save)

/*
 * Algorithm is stateful, and uses 4 handshake lines- two for each direction.
 * Suppose that the TX_PC is sending to the RX_PC.  In this case the
 * DPRESENT handshake line is an output signal from the TX_PC, and the ACK
 * handshake line is an output signal from the RX_PC:
 *
 *		TX_PC				RX_PC
 *
 *	Current state: TX_RDY_WAIT
 *	  TX_PC waits until ACK is clear
 *	  if RECEIVE_ENABLE is true,
 *	    TX_PC loads data to be sent in OUT port
 *	  else error is reported
 *	  TX_PC sets DPRESENT
 *	New state: TX_RPLY_WAIT
 *	  TX_PC waits until ACK or ERROR is set
 *				    Current state: RX_RDY_WAIT
 *				      RX_PC waits until DPRESENT is set,
 *				      RX_PC reads IN port
 *				      if no errors RX_PC sets ACK 
 *				      else RX_PC sets ACK and ERROR
 *				    New state: RX_RPLY_WAIT
 *				      RX_PC waits until DPRESENT is clear
 *	  TX_PC clears DPRESENT
 *	  Error reported if ERROR was set
 *	New state: TX_RDY_WAIT
 *	Goto top
 *				      RX_PC clears ACK and/or ERROR
 *				    New state: RX_RDY_WAIT
 *				    Goto top
 *
 * This communication algorithm is fully handshaked and has no timing
 * dependencies *whatsoever*.  The code on either the TX_PC side or the
 * RX_PC side can be interrupted indefinitely at any point and the algorithm
 * will still function properly when execution is resumed.  The algorithm
 * would be simpler if the handshaking lines were latched.  However, the
 * 8255 was not designed to talk directly to another 8255.  It does not
 * permit an efficient algorithm without external glue logic to latch
 * the handshake lines.
 *
 * If the receiving PC encounters an error the ERROR line is set.
 * The transmitting PC then aborts the transfer and returns an error to
 * the caller.  Error conditions are therefore passed from the receiving
 * end back to the transmitting end.  Errors are not sent from the
 * transmitting end to the receiving end.  However, the transmitting
 * end reports an error to the caller if message receiving is not enabled
 * on the receiving end.
 *
 * This algorithm was designed so that an 8255 could be directly
 * connected to another 8255- 24 bits to 24 bits.  If the two interfaces
 * have the same connectors, then a simple straight-through flat cable
 * should be sufficient to connect them.  Note, however, if +5 (or other
 * voltages) are also on the connecter these lines should be cut on the
 * cable so that the power supplies of the two PCs are not connected together-
 * ground *should* be connected between the PCs, but not power supply
 * voltages.  This algorithm yields a full-duplex 8 bit wide channel.
 *
 * For each communications channel, one PC must be designated the MALE end,
 * the other the FEMALE end.  This is necessary so each end will read and
 * write the proper ports/bits.  It doesn't matter which end is the
 * MALE or FEMALE, as long as both ends aren't the same.  If a single
 * PC has multiple connections, they can be mixed MALE and FEMALE.
 *
 * The MALE end uses port A for send, port B for receive.  Port C is
 * divided into two 4 bit ports.  C-hi is send from the MALE end, C-lo is
 * receive. These designations are reversed on the FEMALE end.  Port C
 * carries the handshaking signals between the PCs.
 *
 * Following defines are bit numbers in C port.
 */
#define PCM_DPRES	0
#define PCM_ACK		1
#define PCM_ENABLE	2
#define PCM_ERROR	3

/*
 * Software status word bits.
 */
#define PCM_TX_BUSY	    0x1	    /* when set, a message is in process of
				       of being transmitted */
#define PCM_RX_ENAB	    0x2	    /* when set, message receiving enabled */
#define PCM_MSG_WAITING	    0x4	    /* when set, an incoming message has been
				       received */
#define PCM_OTHER_PC_RDY    0x8     /* when set, the other PC is enabled to
				       receive a message */
#define PCM_RX_BUSY	    0x10    /* when set, a message is in process
				       of being received */
#define PCM_RX_ERROR	    0x20    /* checksum or other error on receive */
#define PCM_RX_CLRBUSY	    0x40

/*
 * States.
 */
#define PCM_TX_RDY_WAIT		    0
#define PCM_TX_RPLY_WAIT	    1
#define PCM_RX_RDY_WAIT		    0
#define PCM_RX_RPLY_WAIT	    1

/*
 * Messages are composed of a sequence of 8-bit bytes.
 * 
 * Message format:  There are 3 types of messages.  The first byte of a 
 * message is the 'control' byte.  Message type is specified
 * in the control byte.
 *
 * Note that Type 0, 1 and 2 messages are given priority in sending/receiving.
 * The code tries to completely send or receive these message types before
 * processing messages on other channels.  Type 2 messages *block* on sending
 * in 'pcm_msg_process()'.
 *
 * Type 3 messages allow interleaving of processing of other channels, and
 * support full-duplex tranmission.  However, they are slower.
 *
 * Max thruput between two 486/33 machines is app. 25usec/byte with Type 0,
 * 1, or 2 messages.
 * 
 * 		_________________________________
 * 		|   |   |   |   |   |   |   |   |  Control byte
 * 		---------------------------------
 * 		  7   6   5   4   3   2   1   0
 * 
 * 	Bits 7,6: 00-	Type 0 message.  Message is contained in lower
 * 			6 bits of control byte.  This is a 1 byte
 * 			message composed only of the control byte.
 * 	Bits 7,6: 01-	Type 1 message.  Lower 6 bits of control byte
 * 			contain message length in bytes.  Message follows
 * 			control byte, and can be a max of 64 bytes.  A
 *			one byte checksum is transmitted at the end.
 * 	Bits 7,6: 10-	Type 2 message.  Lower 6 bits of control byte
 * 			contain message length in bytes.  Message follows
 * 			control byte, and can be a max of 64 bytes.  A
 *			one byte checksum is transmitted at the end.
 *			This is just like a Type 1 message, except that
 *			it causes 'pcm_msg_process()' to *block* until
 *			the message has been sent.  Use this type cautiously,
 *			for it displaces processing of all other messages
 *			and hangs BOTH SENDING AND RECEIVING computers
 *		        until the message has been sent.
 * 	Bits 7,6: 11-	Type 3 message.  Lower 6 bits are undefined.
 * 			Byte count follows in first two bytes after control
 * 			word, low order byte first.  Max length of message
 * 			is 65,535 bytes.  A two byte checksum is transmitted
 *		        at the end.
 */
#define PCM_TYPE0   0
#define PCM_TYPE1   1
#define PCM_TYPE2   2
#define PCM_TYPE3   3

typedef volatile struct {
    int     pcm_8255;	    /* I/O page address of first port of 8255 */
    int	    pcm_male;	    /* if true, this end is the male end */
    char    *pcm_rbufp;	    /* pointer to start of receive buffer */
    uns_sh  pcm_rbuflen;    /* length in bytes of receive buffer */
    uns_sh  pcm_msg_cnt;    /* length in bytes of received message */
    int	    pcm_status;	    /* status word; note- user code can read this
			       word, but should NEVER clear any bits! */
    int	    pcm_chan_num;   /* channel number */
    int	    pcm_msg_type;   /* type of current received message */
    int	    pcm_tx_msg_type;/* type of current transmitted message */
    int	    pcm_tx_state;   /* transmit state */
    uns_sh  pcm_tx_cnt;	    /* count remaining to transmit */
    uns_sh  pcm_tx_cksum;   /* transmit checksum */
    char    *pcm_tx_cur;    /* pointer to current place in transmit buffer */
    char    pcm_tx_head[3]; /* space for message header */
    char    *pcm_tx_hd_cur; /* pointer to current place in trans head buffer */
    int	    pcm_tx_hd_cnt;  /* count remaining to transmit on header */
    int	    pcm_tx_ck_cnt;  /* count remaining to transmit on checksum */
    char    *pcm_tx_ck_cur; /* pointer to checksum */
    int	    pcm_rx_state;   /* receive state */
    uns_sh  pcm_rx_cnt;	    /* count remaining to receive */
    uns_sh  pcm_rx_calcksum;/* receive checksum as calculated from rec msg */
    uns_sh  pcm_rx_cksum;   /* cksum transmitted at end of rec msg */
    char    *pcm_rx_cur;    /* pointer to current place in receive buffer */
    char    pcm_rx_head[3]; /* space for message header */
    char    *pcm_rx_hd_cur; /* pointer to current place in rec head buffer */
    int	    pcm_rx_hd_cnt;  /* count remaining to receive on header */
    int	    pcm_rx_ck_cnt;  /* count remaining to receive on checksum */
    char    *pcm_rx_ck_cur; /* pointer to checksum */
    int	    pcm_rx_block;   /* block on receiving a Type 3 message */
    ADDR    pcm_tx_port;    /* I/O page address of send port */
    ADDR    pcm_rx_port;    /* I/O page address of rec port */
    ADDR    pcm_flag_port;  /* I/O page address of handshake flags port */
    ADDR    pcm_ctrl_port;  /* I/O page address of control port */
    int	    pcm_flag_save;  /* copy of current flag port bits */
    int	    pcm_dpres_rx;   /* bit pattern to set/clear handshake line */
    int	    pcm_ack_rx;
    int	    pcm_enable_rx;
    int	    pcm_error_rx;
    int	    pcm_dpres_tx;
    int	    pcm_ack_tx;
    int	    pcm_enable_tx;
    int	    pcm_error_tx;
} PCM;
extern PCM pcm[PCM_NUMCHAN];

/*
 * Function prototypes.
 */
void pcm_init(int chan_num);
int pcm_rec_reset(int chan_num);
int pcm_rec_enable(int chan_num);
int pcm_rec_disable(int chan_num);
int pcm_tx_reset(int chan_num);
int pcm_send(int chan_num, char *buf, int msg_type, uns_sh msg_len);
int pcm_ack(int chan_num);
void pcm_msg_process(void);
int pcm_test(PCM *p, int bit, int on_off);
void swap_short(uns_sh *p);
 
#ifndef UNDER_REX
void rxerr(char *p);
#endif

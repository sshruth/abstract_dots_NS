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
 * PC to PC messaging.
 *
 * REMEMBER TO CONFIGURE 8255 ADDRESS AND MASTER/SLAVE DIRECTION
 * IN PCM STRUCT BELOW.
 *
 */
#undef SCOPE_DEBUG	/* don't define!!! - used for low level testing */
#include "pcmsg.h"

#ifdef UNDER_REX
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <hw/inout.h>
#include "rexhdrs.h"
#if (PCM_DEBUG >= 1)
#include "tty.h"
#include "int.h"
#endif
#else	/* UNDER_REX */
#define PCMESSAGE
#include <stdio.h>
#include <stdlib.h>
#ifndef MWC_PPC
#include <conio.h>
#include <sys/types.h>
#else
#include <types.h>
#endif
#endif	/* UNDER_REX */

#ifdef PCMESSAGE

/*
 * Define the functions used by your compiler for outputing and inputing
 * one *byte* (not word), register spacing, byte order.
 */
#ifdef GCC_RTP
#define OUTB out8
#define INB in8
#endif
#if (defined WCC_QNX || defined MICROSOFT_DOS)
#define OUTB	outp
#define INB	inp
#endif
#ifdef BORLAND_DOS
#define OUTB	outportb
#define INB	inportb
#endif
#ifdef MWC_PPC
#define OUTB(A,B)	(*(A)) = (B)
#define INB(A)		( *(A) )
char *itoa(int value, char *buf, int radix);  /* function prototype */
#endif
#ifdef MWC_PPC
#define REG_SPACING 4
#else
#define REG_SPACING 1
#endif
#ifdef MWC_PPC
#define SWAP_BYTES
#else
#undef SWAP_BYTES
#endif
#ifndef UNDER_REX
#if (PCM_DEBUG >= 1)
void dputchar(char c);
void doutput(int sig);
void dputs(char *p);
#endif
#endif

/*
 * Receive buffers.  These are individually allocated by declaration here.
 * Buffers can be different sizes depending on communication requirements
 * of particular PC.
 */
char pcm_rbuf0[PCM_RBUFSIZ];
/*char pcm_rbuf1[PCM_RBUFSIZ];*/
/*char pcm_rbuf2[PCM_RBUFSIZ];*/
/*char pcm_rbuf3[PCM_RBUFSIZ];*/

/*
 * Initialization of PCM struct.  This struct must be initialized for
 * each PC to PC channel.  It contains the addresses of the 8255 and the
 * receive buffer.  The rest of the struct is initialized at runtime
 * by a call to pcm_init().
 *
 * Make sure that both sides of the communications link are not set for
 * the same gender.  One side must be male, the other female.  It
 * doesn't matter which one is which, as long as they are different.
 *
 * NOTE: Under REX, don't use either of the first two 8255 ports (0x280
 * or 0x284).  Even if you are not using these ports under REX for digital
 * I/O, conflicts will result.  Under DOS, it is fine to use these addresses.
 */
PCM pcm[PCM_NUMCHAN] = {
#ifdef MWC_PPC
#define SLOT 0xC
    {((long) (0xF000 | (SLOT<<8) | (SLOT<<4))) <<16L, /* I/O page address
							 of 8255 */
#else
    {0x288,		    /* I/O page address of 8255 */
#endif
    1,			    /* 1 for male, 0 for female */
    &pcm_rbuf0[0],	    /* pointer to receive buffer */
    sizeof(pcm_rbuf0)},	    /* length of receive buffer */
};

/*
 * Initialization routine.  Must be called before communication can occur.
 * If argument is -1, all links are inited.
 */
void
pcm_init(int chan_num)
{
    PCM *p, *begin, *end;

    if(chan_num == -1) {
	begin= &pcm[0], end= &pcm[PCM_NUMCHAN];
	chan_num= 0;
    } else {
	begin= &pcm[chan_num], end= &pcm[chan_num+1];
    }
    for(p= begin; p < end; p++, chan_num++) {
	p->pcm_status= 0;
	p->pcm_chan_num= chan_num;
	p->pcm_tx_state= PCM_TX_RDY_WAIT;
	p->pcm_rx_state= PCM_RX_RDY_WAIT;
	p->pcm_flag_port= (ADDR)(p->pcm_8255 + (2 * REG_SPACING));
	p->pcm_ctrl_port= (ADDR)(p->pcm_8255 + (3 * REG_SPACING));
	p->pcm_flag_save= 0;
	p->pcm_rx_block= 0;
	if(p->pcm_male) {
	    p->pcm_dpres_rx= (01 << PCM_DPRES);
	    p->pcm_ack_rx= (01 << PCM_ACK);
	    p->pcm_enable_rx= (01 << PCM_ENABLE);
	    p->pcm_error_rx= (01 << PCM_ERROR);
	    p->pcm_dpres_tx= (01 << (PCM_DPRES + 4));
	    p->pcm_ack_tx= (01 << (PCM_ACK + 4));
	    p->pcm_enable_tx= (01 << (PCM_ENABLE + 4));
	    p->pcm_error_tx= (01 << (PCM_ERROR + 4));

	    /*
	     * Init 8255 for MALE end: port A out, port B in, port
	     * CHI out, port CLO in.  Mode 0.
	     */
	    p->pcm_tx_port= (ADDR)p->pcm_8255;
	    p->pcm_rx_port= (ADDR)(p->pcm_8255 + (1 * REG_SPACING));
	    OUTB(p->pcm_ctrl_port, 0x83);   /* control word init */

	} else {
	    p->pcm_dpres_tx= (01 << PCM_DPRES);
	    p->pcm_ack_tx= (01 << PCM_ACK);
	    p->pcm_enable_tx= (01 << PCM_ENABLE);
	    p->pcm_error_tx= (01 << PCM_ERROR);
	    p->pcm_dpres_rx= (01 << (PCM_DPRES + 4));
	    p->pcm_ack_rx= (01 << (PCM_ACK + 4));
	    p->pcm_enable_rx= (01 << (PCM_ENABLE + 4));
	    p->pcm_error_rx= (01 << (PCM_ERROR + 4));

 	    /*
	     * Init 8255 for FEMALE end: port A in, port B out, port
	     * CHI in, port CLO out.  Mode 0.
	     */
	    p->pcm_tx_port= (ADDR)(p->pcm_8255 + (1 * REG_SPACING));
	    p->pcm_rx_port= (ADDR)p->pcm_8255;
	    OUTB(p->pcm_ctrl_port, 0x98);   /* control word init */
	}
	OUTB(p->pcm_tx_port, 0x0);	/* clear tx port */
	OUTB(p->pcm_flag_port, 0x0);    /* clear handshake flags */
    }
#ifdef SCOPE_DEBUG
    OUTB(0x283, 0x83);
    OUTB(0x280, 0x0);
#endif
}

/*
 * Reset the receive link.  This function can be called to reset a
 * receive error.  Then, 'pcm_rec_enable' must be called to enable message
 * receiving again.
 */
int
pcm_rec_reset(int chan_num)
{
    PCM *p= &pcm[chan_num];

    p->pcm_status &= ~(PCM_RX_ENAB|PCM_MSG_WAITING
		       |PCM_RX_BUSY|PCM_RX_ERROR|PCM_RX_CLRBUSY);
    PCM_setflag(p, 0x0, p->pcm_ack_tx|p->pcm_error_tx|p->pcm_enable_tx);
    return(0);
}

/*
 * Enable message receiving from other PC.  Must be called before messages
 * can be received.
 */
int
pcm_rec_enable(int chan_num)
{
    PCM *p= &pcm[chan_num];

    if(p->pcm_status & PCM_RX_ENAB) {
	rxerr("pcm_rec_enable(): Called when PCM_RX_ENAB is true");
	return(-1);
    }
    pcm_rec_reset(chan_num);
    PCM_setflag(p, p->pcm_enable_tx, 0x0);
    p->pcm_status |= PCM_RX_ENAB;
    return(0);
}

/*
 * Disable message receiving.
 */
int
pcm_rec_disable(int chan_num)
{
    PCM *p= &pcm[chan_num];

    if(p->pcm_status & PCM_RX_BUSY) {
	rxerr("pcm_rec_disable(): Called when PCM_RX_BUSY is true");
	return(-1);
    }
    PCM_setflag(p, 0x0, p->pcm_enable_tx);
    p->pcm_status &= ~PCM_RX_ENAB;
    return(0);
}

/*
 * Reset the transmit link.
 */
int
pcm_tx_reset(int chan_num)
{
    PCM *p= &pcm[chan_num];

    p->pcm_status &= ~PCM_TX_BUSY;
    PCM_setflag(p, 0x0, p->pcm_dpres_tx);
    p->pcm_tx_state= PCM_TX_RDY_WAIT;
    return(0);
}

/*
 * Send message.
 */
int
pcm_send(int chan_num, char *buf, int msg_type, uns_sh msg_len)
{
    PCM *p= &pcm[chan_num];

    if(p->pcm_status & PCM_TX_BUSY) {
	rxerr("pcm_send(): called when PCM_TX_BUSY set");
	return(-1);
    }
    if(!(p->pcm_status & PCM_OTHER_PC_RDY)) {
	rxerr("pcm_send(): receiving not enabled on other PC");
	return(-1);
    }
    switch(msg_type) {
    case PCM_TYPE0:
	if(msg_len != 0) {
	    rxerr("pcm_send(): message length non-zero for type 0 message");
	    return(-1);
	}
	p->pcm_tx_head[0]= (PCM_TYPE0 << 6) | (buf[0] & 0x3f);
	p->pcm_tx_cnt= 0;
	p->pcm_tx_hd_cnt= 1;
	p->pcm_tx_hd_cur= (char *)&p->pcm_tx_head[0];
	p->pcm_tx_ck_cnt= 0;
	p->pcm_tx_msg_type= msg_type;
	p->pcm_status |= PCM_TX_BUSY;
	return(0);

    case PCM_TYPE1:
    case PCM_TYPE2:
	if(msg_len > 64) {
	    rxerr("pcm_send(): bad message length for type 1 or 2 message");
	    return(-1);
	}
	p->pcm_tx_head[0]= (msg_type == PCM_TYPE1 ?
			   (PCM_TYPE1 << 6) | (msg_len & 0x3f) :
			   (PCM_TYPE2 << 6) | (msg_len & 0x3f));
	p->pcm_tx_hd_cnt= 1;
	p->pcm_tx_hd_cur= (char *)&p->pcm_tx_head[0];
	p->pcm_tx_ck_cnt= 1;
	p->pcm_tx_ck_cur= (char *)&p->pcm_tx_cksum;
	p->pcm_tx_cnt= msg_len;
	p->pcm_tx_cur= buf;
	p->pcm_tx_cksum= 0;
	p->pcm_tx_msg_type= msg_type;
	p->pcm_status |= PCM_TX_BUSY;
	return(0);

    case PCM_TYPE3:
	p->pcm_tx_head[0]= PCM_TYPE3 << 6;
	p->pcm_tx_head[1]= PCM_ushortlo(msg_len);
	p->pcm_tx_head[2]= PCM_ushorthi(msg_len);
	p->pcm_tx_hd_cnt= 3;
	p->pcm_tx_hd_cur= (char *)&p->pcm_tx_head[0];
	p->pcm_tx_ck_cnt= 2;
	p->pcm_tx_ck_cur= (char *)&p->pcm_tx_cksum;
	p->pcm_tx_cnt= msg_len;
	p->pcm_tx_cur= buf;
	p->pcm_tx_cksum= 0;
	p->pcm_tx_msg_type= msg_type;
	p->pcm_status |= PCM_TX_BUSY;
	return(0);

    default:
	rxerr("pcm_send(): bad message type");
	return(-1);
    }
}
       
int
pcm_ack(int chan_num)
{
    PCM *p= &pcm[chan_num];

/* if pcm_ack is called for the vex channel when sockets
 * are being used for vex communication, just return
 */
#ifdef PCS_SOCKET
    if(!chan_num) return(0);
#endif

    if(!(p->pcm_status & PCM_MSG_WAITING)) {
	rxerr("pcm_ack(): called when PCM_MSG_WAITING not set");
	return(-1);
    }
    p->pcm_rx_block= 0;
    p->pcm_status &= ~PCM_MSG_WAITING;
    return(0);
}

/*
 * This routine must be called periodically to do the sending/receiving
 * of messages.  This can be accomplished either via a clock interrupt
 * or as part of a background loop.  This routine is not reentrant.
 * The other functions above (pcm_rec_reset, pcm_rec_enable, pcm_rec_disable,
 * pcm_tx_reset, pcm_send, and pcm_ack) must not be called while this function
 * is executing.  If this function is called from an interrupt routine,
 * however, it can safely interrupt the above functions at any time.
 */
void
pcm_msg_process(void)
{
  PCM *p;
  int active, transmax;
  int i;
#if (PCM_DEBUG >= 1)
  char st[100];
#endif
#if (PCM_DEBUG >= 2)
  int j;
#endif  
  unsigned volatile save;
  int other_done;
  int tx_tmax, rx_tmax;
  int tx_run, rx_run;
  int tx_done, rx_done;

  /*
   * Type 3 messages are full duplex.  Type 3 messages can be sent and
   * received simultaneously.  Also for Type 3 messages the algorithm
   * interleaves sending/receiving among channels.  For example, if
   * a 100 byte message is being sent/received on chan 0 and a 2 byte message
   * on chan 1, then one byte is sent/received on chan 0, one byte on chan 1,
   * then another byte on chan 0 and another on chan 1.  In this way small
   * messages are not blocked by larger ones.
   *
   * Type 0 and 1 messages are given priority.  The algorithm trys to
   * completely transmit or receive a Type 0 or 1 message before moving
   * on to other messages.
   * 
   * Type 2 messages *block* on send in pcm_msg_process()!
   *
   * Debouncing, skew issues:  each I/O instruction consumes
   * 800nsec.  This should be enough delay to handle bounce and skew for
   * cable lengths under 15 feet.
   *
   * Speed between two 486/33's is app. 25usec/byte.
   */
#if (PCM_DEBUG >= 3)
  if(pcm[0].pcm_status & (PCM_TX_BUSY|PCM_RX_BUSY)) dputs(" ^ ");
#endif
  transmax= 0;
  other_done= 0;

  do {
#if (PCM_DEBUG >= 3)
    if(pcm[0].pcm_status & (PCM_TX_BUSY|PCM_RX_BUSY)) dputchar('+');
#endif

    active= tx_done= rx_done= 0;

    /*
     * Loop for each channel.
     */
    for(p= &pcm[0]; p < &pcm[PCM_NUMCHAN]; p++) {

	/*
	 * Reset constants that determine how long to wait
	 * for other PC before giving up and returning.
	 */
	tx_tmax= PCM_TESTMAX;

	/*
	 * Don't waste time looking for a message PCM_TESTMAX times
	 * if not currently receiving one.
	 */
	if(p->pcm_status & PCM_RX_BUSY) rx_tmax= PCM_TESTMAX;
	else rx_tmax= 1;
	tx_run= rx_run= -2;
        if(!other_done) {

	  /*
	   * This check only needs to be done once per call to
	   * pcm_msg_process().  Lot faster to check 'other_done'
	   * than to do an inp().
	   */
	  if(INB(p->pcm_flag_port) & p->pcm_enable_rx) {
	    p->pcm_status |= PCM_OTHER_PC_RDY;	    /* update status bit */
	  } else {
	    p->pcm_status &= ~PCM_OTHER_PC_RDY;
	  }
	  if(p == &pcm[PCM_NUMCHAN-1]) other_done= 1;
	}


    tx_start:

	if(p->pcm_status & PCM_TX_BUSY) {
#if (PCM_DEBUG >= 2)
	    itoa(p->pcm_chan_num, st, 10);
	    dputs("(ch:");
	    dputs(st);
	    dputchar(')');
#endif

	    /*
	     * Transmitting.
	     */
	    switch(p->pcm_tx_state) {
	    case PCM_TX_RDY_WAIT:
#if (PCM_DEBUG >= 2)
		for(i= 0, j= 0;;) {
#else
		for(i= 0;;) {
#endif
		  save= INB(p->pcm_flag_port);
		  if(!(save & p->pcm_enable_rx)) {
		      rxerr("pcm_process_msg(): no RECEIVE_ENABLE on tx");
		      goto tx_reset;
		  }
		  if(!(save & p->pcm_ack_rx)) break;
		  if(++i == tx_tmax) {
		    if(p->pcm_tx_msg_type == PCM_TYPE2) {

			/*
			 * Block on send for TYPE2.
			 */
			i= 0;
#if (PCM_DEBUG >= 2)
			j += tx_tmax;
#endif			
			continue;
		    }
#if (PCM_DEBUG >= 2)
		    {
			i += j;
			itoa(i, st, 10);
			dputs(st);
		    }
#endif
		    goto rx_start;	/* ack still set */
		  }
		}

#if (PCM_DEBUG >= 2)
		{
		    i += j;
		    itoa(i, st, 10);
		    dputs(st);
		}
#endif
		/*
		 * Double tx_tmax if we are acutally communicating with
		 * the other machine at this moment (i.e. we have gone
		 * through two handshakes- tx_run is init'ed to -2).
		 */
		if(++tx_run == 0) tx_tmax <<= 1;
		if(p->pcm_tx_hd_cnt) {
#if (PCM_DEBUG >= 1)
		  if( (p->pcm_tx_hd_cnt == 3)
		  ||  ((p->pcm_tx_hd_cnt == 1)
		      && (p->pcm_tx_msg_type != PCM_TYPE3))) {
		    dputs("\n(send start:");
#ifdef UNDER_REX
		    {
			long time;
			time= i_b->i_time;
			itoa(time, st, 10);
			dputs(st);
		    }
#endif
		    dputs(",len:");
		    itoa(p->pcm_tx_cnt, st, 10);
		    dputs(st);
		    dputs(",ty:");
		    itoa(p->pcm_tx_msg_type, st, 10);
		    dputs(st);
		    itoa(p->pcm_chan_num, st, 10);
		    dputs(",ch:");
		    dputs(st);
		    dputchar(')');
		  }
#endif
		    /*
		     * Currently transmitting message header.
		     */
		    p->pcm_tx_cksum += *p->pcm_tx_hd_cur & 0xff;
		    OUTB(p->pcm_tx_port, *p->pcm_tx_hd_cur++);
		    p->pcm_tx_hd_cnt--;
		    PCM_setflag(p, p->pcm_dpres_tx, 0x0);
		    p->pcm_tx_state= PCM_TX_RPLY_WAIT;
		    active++;
		    /* fall thru to next state */

		} else if(p->pcm_tx_cnt) {
#if (PCM_DEBUG >= 3)
		    dputchar('b');
#endif
		    /*
		     * Currently transmitting type 1,2, or 3 message.
		     */
		    p->pcm_tx_cksum += *p->pcm_tx_cur & 0xff;
		    OUTB(p->pcm_tx_port, *p->pcm_tx_cur++);
		    p->pcm_tx_cnt--;
		    PCM_setflag(p, p->pcm_dpres_tx, 0x0);
		    p->pcm_tx_state= PCM_TX_RPLY_WAIT;
		    active++;
		    /* fall thru to next state */

		} else if(p->pcm_tx_ck_cnt) {
#if (PCM_DEBUG >= 3)
		    dputchar('c');
#endif
		    /*
		     * Currently transmitting checksum.
		     */
#ifdef SWAP_BYTES
		    if(p->pcm_tx_msg_type == PCM_TYPE3) {
		       if(p->pcm_tx_ck_cnt == 2) swap_short(&p->pcm_tx_cksum);
		    } else {
			swap_short(&p->pcm_tx_cksum);
		    }
#endif		    
		    OUTB(p->pcm_tx_port, *p->pcm_tx_ck_cur++);
		    p->pcm_tx_ck_cnt--;
		    PCM_setflag(p, p->pcm_dpres_tx, 0x0);
		    p->pcm_tx_state= PCM_TX_RPLY_WAIT;
		    active++;
		    /* fall thru to next state */

		} else {
#if (PCM_DEBUG >= 3)
		    dputchar('d');
#endif
		    /*
		     * Done transmitting.
		     */
		    p->pcm_status &= ~PCM_TX_BUSY;
#if (PCM_DEBUG >= 1)
		    dputs("\n(send done:");
#ifdef UNDER_REX
		    {
			long time;
			time= i_b->i_time;
			itoa(time, st, 10);
			dputs(st);
		    }
#endif
		    itoa(p->pcm_chan_num, st, 10);
		    dputs(",ch:");
		    dputs(st);
		    dputchar(')');
#endif
		    break;	/* next for() iteration */
		}
		
	    case PCM_TX_RPLY_WAIT:
#if (PCM_DEBUG >= 2)
		for(i= 0, j= 0;;) {
#else
		for(i= 0;;) {
#endif
		  save= INB(p->pcm_flag_port);
		  if(save & p->pcm_error_rx) {
		      rxerr("pcm_process_msg(): transmission error");
		      goto tx_reset;
		  }
		  if(save & p->pcm_ack_rx) break;
		  if(++i == tx_tmax) {
		    if(p->pcm_tx_msg_type == PCM_TYPE2) {

			/*
			 * Block on send for TYPE2.
			 */
			i= 0;
#if (PCM_DEBUG >= 2)
			j += tx_tmax;
#endif			
			continue;
		    }
#if (PCM_DEBUG >= 2)
		    {
			i += j;
			itoa(i, st, 10);
			dputs(st);
		    }
#endif
		    goto rx_start;	/* ack not set */
		  }
		}
		PCM_setflag(p, 0x0, p->pcm_dpres_tx);
#if (PCM_DEBUG >= 2)
		{
		    i += j;
		    itoa(i, st, 10);
		    dputs(st);
		}
#endif
		/*
		 * Double tx_tmax if we are acutally communicating with
		 * the other machine at this moment (i.e. we have gone
		 * through two handshakes- tx_run is init'ed to -2).
		 */
		if(++tx_run == 0) tx_tmax <<= 1;
		p->pcm_tx_state= PCM_TX_RDY_WAIT;
#if (PCM_DEBUG >= 3)
		dputchar('@');
		{
		    dputs("(tm:");
		    itoa(transmax, st, 10);
		    dputs(st);
		    dputchar(')');
		}
#endif
		active++;
		tx_done++;

		/*
		 * Give Type 0, 1, and 2 messages priority.  Try
		 * to process them as fast as possible, without
		 * interleaving other messages.
		 */
		if(p->pcm_tx_msg_type != PCM_TYPE3) goto tx_start;
		break;

	    default:

		/*
		 * Should never happen.  Cannot recover cleanly from here.
		 */
		rxerr("pcm_msg_process(): bad state reached in tx");
		break;

	    tx_reset:

		PCM_setflag(p, 0x0, p->pcm_dpres_tx);
	        p->pcm_tx_state= PCM_TX_RDY_WAIT;
		p->pcm_status &= ~PCM_TX_BUSY;
	        break;
	    }
	}	    /* end if(): transmitting */

    rx_start:
    
	if(p->pcm_status & PCM_RX_ENAB) {

	    /*
	     * Message receiving.  Note that PCM_MSG_WAITING is set when
	     * complete message has been received, but *before* handshake
	     * is finished for the final byte.  This is to provide fastest
	     * response for one byte (TYPE0) messages.
	     */
	    switch(p->pcm_rx_state) {
	    case PCM_RX_RDY_WAIT:
		if(p->pcm_status & PCM_MSG_WAITING) break;
#if (PCM_DEBUG >= 2)
		for(i= 0, j= 0;;) {
#else
		for(i= 0;;) {
#endif
		    if(INB(p->pcm_flag_port) & p->pcm_dpres_rx) break;
		    if(++i == rx_tmax) {
			if(p->pcm_rx_block) {

			    /*
			     * Block on receive for TYPE2.
			     */
			    i= 0;
#if (PCM_DEBUG >= 2)
			    j += rx_tmax;
#endif			
			    continue;
			}
#if (PCM_DEBUG >= 2)
			if(p->pcm_status & PCM_RX_BUSY) {
			    i += j;
			    itoa(i, st, 10);
			    dputs(st);
			}
#endif
			goto rx_done;
		    }
		}
#if (PCM_DEBUG >= 2)
		{
		    i += j;
		    itoa(i, st, 10);
		    dputs(st);
		}
#endif
#ifdef SCOPE_DEBUG
		OUTB(0x280, 0x1);
#endif
		/*
		 * Double rx_tmax if we are acutally communicating with
		 * the other machine at this moment (i.e. we have gone
		 * through two handshakes- rx_run is init'ed to -2).
		 */
		if(rx_tmax == 1) rx_tmax= PCM_TESTMAX;
		if(++rx_run == 0) rx_tmax <<= 1;
		if(!(p->pcm_status & PCM_RX_BUSY)) {
#if (PCM_DEBUG >= 1)
		    dputs("\n(rec start:");
#ifdef UNDER_REX
		    {
			long time;
			time= i_b->i_time;
			itoa(time, st, 10);
			dputs(st);
		    }
#endif
#endif
		    /*
		     * Beginning of new message.  Initialize rx_tmax
		     * (it was 1).
		     */
		    p->pcm_rx_head[0]= INB(p->pcm_rx_port);
#if (PCM_DEBUG >= 1)
		    dputs(",ty:");
		    itoa(((p->pcm_rx_head[0] & 0xc0) >> 6), st, 10);
		    dputs(st);
		    itoa(p->pcm_chan_num, st, 10);
		    dputs(",ch:");
		    dputs(st);
		    dputchar(')');
#endif
		    switch((p->pcm_rx_head[0] & 0xc0) >> 6) {
		    case PCM_TYPE0:
			p->pcm_rbufp[0]= p->pcm_rx_head[0] & 0x3f;
			p->pcm_msg_type= PCM_TYPE0;
			p->pcm_msg_cnt= 1;
			p->pcm_status |= PCM_MSG_WAITING|PCM_RX_CLRBUSY;
			break;
		    case PCM_TYPE1:
			p->pcm_rx_cnt= p->pcm_msg_cnt=
			    p->pcm_rx_head[0] & 0x3f;
			if(p->pcm_rx_cnt == 0) {
			  rxerr("pcm_msg_process(): 0 length TYPE1 message");
			  goto rx_reset;
			}
			if(p->pcm_rx_cnt > p->pcm_rbuflen) {
			  rxerr("pcm_msg_process(): T1 msg size > rec buf");
			  goto rx_reset;
			}
			p->pcm_msg_type= PCM_TYPE1;
			p->pcm_rx_cur= p->pcm_rbufp;
			p->pcm_rx_hd_cnt= 0;
			p->pcm_rx_ck_cnt= 1;
			p->pcm_rx_ck_cur= (char *)&p->pcm_rx_cksum;
			p->pcm_rx_calcksum= p->pcm_rx_head[0] & 0xff;
			break;
		    case PCM_TYPE2:
			p->pcm_rx_cnt= p->pcm_msg_cnt=
			    p->pcm_rx_head[0] & 0x3f;
			if(p->pcm_rx_cnt == 0) {
			  rxerr("pcm_msg_process(): 0 length TYPE2 message");
			  goto rx_reset;
			}
			if(p->pcm_rx_cnt > p->pcm_rbuflen) {
			  rxerr("pcm_msg_process(): T2 msg size > rec buf");
			  goto rx_reset;
			}
			p->pcm_msg_type= PCM_TYPE2;
			p->pcm_rx_cur= p->pcm_rbufp;
			p->pcm_rx_hd_cnt= 0;
			p->pcm_rx_ck_cnt= 1;
			p->pcm_rx_ck_cur= (char *)&p->pcm_rx_cksum;
			p->pcm_rx_calcksum= p->pcm_rx_head[0] & 0xff;
			p->pcm_rx_block= 1;
			break;
		    case PCM_TYPE3:
			p->pcm_msg_type= PCM_TYPE3;
			p->pcm_rx_cur= p->pcm_rbufp;
			p->pcm_rx_hd_cnt= 2;
			p->pcm_rx_hd_cur= (char *)&p->pcm_rx_head[1];
			p->pcm_rx_ck_cnt= 2;
			p->pcm_rx_ck_cur= (char *)&p->pcm_rx_cksum;
			p->pcm_rx_calcksum= p->pcm_rx_head[0] & 0xff;
			break;
		    default:
			rxerr("pcm_msg_process(): unknown msg type, rx-1");
			goto rx_reset;
		    }
		    /* fall thru to next state */

		} else if(p->pcm_rx_hd_cnt) {
#if (PCM_DEBUG >= 3)
		    dputchar('B');
#endif
		    /*
		     * Currently receiving rest of header.
		     */
		    p->pcm_rx_calcksum +=
			((*p->pcm_rx_hd_cur++ = INB(p->pcm_rx_port)) & 0xff);
		    if(--p->pcm_rx_hd_cnt == 0) {

			/*
			 * Complete header received.  Compute length
			 * of message that follows.
			 */
			switch((p->pcm_rx_head[0] & 0xc0) >> 6) {
			case PCM_TYPE0:
			case PCM_TYPE1:
			case PCM_TYPE2:
			  rxerr("pcm_msg_process(): out of sync");
			  goto rx_reset;
			case PCM_TYPE3:
			  p->pcm_msg_cnt=
				p->pcm_rx_cnt= (p->pcm_rx_head[1] & 0xff)
					 | ((p->pcm_rx_head[2] << 8) & 0xff00);
			  if(p->pcm_rx_cnt == 0) {
			    rxerr("pcm_msg_process(): 0 length TYPE3 message");
			    goto rx_reset;
			  }
			  if(p->pcm_rx_cnt > p->pcm_rbuflen) {
			    rxerr("pcm_msg_process(): T3 msg size > rec buf");
			    goto rx_reset;
			  }
			  break;
			default:
			  rxerr("pcm_msg_process(): unknown msg type, rx-2");
			  goto rx_reset;
			}
		    }
		    /* fall thru to next state */

		} else if(p->pcm_rx_cnt) {
#if (PCM_DEBUG >= 3)
		    dputchar('C');
#endif
		    /*
		     * Currently receiving body of message.
		     */
		    p->pcm_rx_calcksum +=
			((*p->pcm_rx_cur++ = INB(p->pcm_rx_port)) & 0xff);
		    p->pcm_rx_cnt--;
		    /* fall thru to next state */

		} else if(p->pcm_rx_ck_cnt) {
#if (PCM_DEBUG >= 3)
		    dputchar('D');
#endif
		    /*
		     * Currently receiving checksum.
		     */
		    *p->pcm_rx_ck_cur++ = INB(p->pcm_rx_port);
		    if(--p->pcm_rx_ck_cnt == 0) {

			/*
			 * Compare checksums.
			 */
			switch(p->pcm_msg_type) {
			case PCM_TYPE0:
			    rxerr("pcm_msg_process(): TYPE0 has cksum");
			    goto rx_reset;
			case PCM_TYPE1:
			case PCM_TYPE2:
			    p->pcm_rx_calcksum &= 0xff;
#ifdef SWAP_BYTES
			    swap_short(&p->pcm_rx_cksum);
#endif			    
			    p->pcm_rx_cksum &= 0xff;
			    goto ck_check;
			case PCM_TYPE3:
#ifdef SWAP_BYTES
			    swap_short(&p->pcm_rx_cksum);
#endif			    

			ck_check:
			    if(p->pcm_rx_cksum != p->pcm_rx_calcksum) {
			      rxerr("pcm_msg_process(): cksums don't compare");
			      goto rx_reset;
			    }
			    break;
			default:
			    rxerr("pcm_msg_process(): bad type in cksum-rx");
			    goto rx_reset;
			}

			/*
			 * Message has been received.
			 */
#if (PCM_DEBUG >= 1)
			dputs("\n(rec done:");
#ifdef UNDER_REX
			{
			    long time;
			    time= i_b->i_time;
			    itoa(time, st, 10);
			    dputs(st);
			}
#endif
			    dputs(",len:");
			    itoa(p->pcm_msg_cnt, st, 10);
			    dputs(st);
			    itoa(p->pcm_chan_num, st, 10);
			    dputs(",ch:");
			    dputs(st);
			    dputchar(')');
#endif
			p->pcm_status |= (PCM_MSG_WAITING|PCM_RX_CLRBUSY);
		    }
		    /* fall thru to next state */

		} else {

		    /*
		     * Errors.
		     */
		    rxerr("pcm_msg_process(): out of sync in rx");

		rx_reset:
		    active++;
		    p->pcm_status |= (PCM_RX_ERROR|
				     PCM_MSG_WAITING|PCM_RX_CLRBUSY);
		    PCM_setflag(p, p->pcm_error_tx|p->pcm_ack_tx, 0x0);
		}
		active++;
		p->pcm_status |= PCM_RX_BUSY;
		PCM_setflag(p, p->pcm_ack_tx, 0x0);
		p->pcm_rx_state= PCM_RX_RPLY_WAIT;
		/* intenionally fall thru to next state */
		    
	    case PCM_RX_RPLY_WAIT:
#ifdef SCOPE_DEBUG
		OUTB(0x280, 0x0);
#endif
#if (PCM_DEBUG >= 2)
		for(i= 0, j= 0;;) {
#else
		for(i= 0;;) {
#endif
		    if(!(INB(p->pcm_flag_port) & p->pcm_dpres_rx)) break;
		    if(++i == rx_tmax) {
			if(p->pcm_rx_block) {

			    /*
			     * Block on receive for TYPE2.
			     */
			    i= 0;
#if (PCM_DEBUG >= 2)
			    j += rx_tmax;
#endif			
			    continue;
			}
#if (PCM_DEBUG >= 2)
			{
			    i += j;
			    itoa(i, st, 10);
			    dputs(st);
			}
#endif
			goto rx_done;
		    }
		}
		PCM_setflag(p, 0x0, p->pcm_ack_tx);
#if (PCM_DEBUG >= 2)
		{
		    i += j;
		    itoa(i, st, 10);
		    dputs(st);
		}
#endif

		/*
		 * Double rx_tmax if we are acutally communicating with
		 * the other machine at this moment (i.e. we have gone
		 * through two handshakes- rx_run is init'ed to -2).
		 */
		if(++rx_run == 0) rx_tmax <<= 1;
		if(p->pcm_status & PCM_RX_ERROR)
			    PCM_setflag(p, 0x0, p->pcm_error_tx);
		if(p->pcm_status & PCM_RX_CLRBUSY)
			    p->pcm_status &= ~(PCM_RX_BUSY|PCM_RX_CLRBUSY);
		p->pcm_rx_state= PCM_RX_RDY_WAIT;
#if (PCM_DEBUG >= 3)
		dputchar('#');
#endif
		active++;
		rx_done++;

		/*
		 * Give Type 0, 1 and 2 messages priority.  Try
		 * to process them as fast as possible, without
		 * interleaving other messages.
		 */
		if(p->pcm_msg_type != PCM_TYPE3) goto rx_start;
		break;

	    default:

		/*
		 * Should never happen.  Cannot recover cleanly from here.
		 */
		rxerr("pcm_msg_process(): bad state reached in rx");
		break;
	    }
	}	    /* end if(): receiving */

    rx_done:
	continue;
	
      }		    /* end for(): loop for each channel */
      if(rx_done || tx_done) transmax++;
  } while((transmax < PCM_TRANSMAX) && (active != 0));
}

#ifndef UNDER_REX

void
rxerr(char *msg)
{
    printf("PCMSG_ERR: %s\n", msg);
}

#if (PCM_DEBUG >= 1)

#define DB_BUF_SIZ  20000
char db_buf[DB_BUF_SIZ]= {'$', {0}};
char *db_fill= &db_buf[1];

/*
 * Print string.
 */
void
dputs(char *p)
{
	while(*p != '\0') {
	    dputchar(*p++);
	}
}

/*
 * If 'doutput_inmem' is true, results are stored in a memory
 * buffer instead of printing on screen.  If 'doutput_rot' is true,
 * the contents of the circular buffer rotate;  otherwise when it
 * is filled it no longer stores new data.
 */
int doutput_rot= 0, doutput_inmem=0 , doutput_tofile= 1;

void
dputchar(char c)
{
    if(!doutput_inmem) {
	putchar(c);
	return;
    }
    *db_fill++ = c;
    if(db_fill >= &db_buf[DB_BUF_SIZ]) {
	if(doutput_rot) db_fill= &db_buf[0];
	else db_fill--;
    }
    *db_fill= '\0';
}

/*
 * Called to print out contents of memory buffer.  If 'doutput_exit'
 * is true, will also exit program after completion of printout.
 */
int doutput_exit= 0;

#pragma off (unreferenced)
void
doutput(int sig)
{
#pragma on (unreferenced)
    int tmp= sig;
    char *p= db_buf;
    FILE *dout;

    tmp++;
    
    /*
     * Find beginning of circular buffer.
     */
    p= db_fill;
    if(*p != '\0') {
	fprintf(stderr, "doutput: circular buffer scrambled\n");
	fflush(stderr);
	return;
    }
    while(*p == '\0') {
	if(++p >= &db_buf[DB_BUF_SIZ]) p= &db_buf[0];
    }
    if(doutput_tofile) {
	if((dout= fopen("/tmp/doutput_file", "w")) == NULL) {
	    fprintf(stderr, "doutput: cannot create file in /tmp");
	}
    } else dout= NULL;
    while(*p != '\0') {
	    putc(*p, stderr);
	    if(dout != NULL) fputc(*p, dout);
	    if(++p >= &db_buf[DB_BUF_SIZ]) p= &db_buf[0];
    }
    fflush(stderr);
    if(dout != NULL) {
	fflush(dout);
	fclose(dout);
    }
    if(doutput_exit) exit(1);
}

#endif
#endif
#endif

#ifdef MWC_PPC
char *
itoa(int value, char *buf, int radix)
{
	sprintf(buf, "%d", value);
	return(buf);
}
#endif

void
swap_short(unsigned short *p)
{
    uns_sh hold;

    hold= ((*p & 0xff00) >> 8) & 0xff;
    hold |= ((*p & 0xff) << 8) & 0xff00;
    *p= hold;
}

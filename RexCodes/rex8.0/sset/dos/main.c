/*
 * Program to test PC to PC messaging from the non-QNX side.
 */
#define PCMESSAGE
#define TYPE1		/* undef to send a Type 2 message */

#include "pcmsg.h"
#include <stdio.h>
#ifdef WCC_QNX
#include <unistd.h>
#include <signal.h>
#endif
#include <stdlib.h>

char msg[]=
"No one comes to the Father but by me.";

#ifdef WCC_QNX
void do_interrupt(int sig_num);
#endif
#ifdef BORLAND_DOS
int do_interrupt(void);
#endif

main()
{
    PCM *p= &pcm[0];
    
#ifdef WCC_QNX
    signal(SIGQUIT, do_interrupt);
#endif
#ifdef BORLAND_DOS
    ctrlbrk(do_interrupt);
#endif
    setbuf(stdout, NULL);	/* turn off output buffering */
    pcm_init(0);		/* init the channel */
    pcm_rec_enable(0);		/* enable receiving */

    for(;;) {
      while(1) {
	pcm_msg_process();	/* wait for message from REX */
	if(p->pcm_status & PCM_MSG_WAITING) break;
      }
      printf("rec: status %x, type %d, count %d\n",
	   p->pcm_status, p->pcm_msg_type, p->pcm_msg_cnt);
      printf("rec: %s\n", p->pcm_rbufp);
      pcm_ack(0);		/* acknowledge message */

#ifdef TYPE1
      pcm_send(0, msg, PCM_TYPE1, sizeof(msg));	/* send reply */

      /*
       * Repeatedly call 'pcm_msg_process()' until message is sent.
       */
      while(pcm[0].pcm_status & PCM_TX_BUSY) pcm_msg_process();
#else
      pcm_send(0, msg, PCM_TYPE2, sizeof(msg));	/* send reply */

      /*
       * 'pcm_msg_process()' will block on send for a Type 2 message,
       * and does not have to be called repeatedly.
       */
      pcm_msg_process();
#endif
    }
}

#if (PCM_DEBUG >= 1)
extern void doutput(int sig);
#endif

#ifdef WCC_QNX
#pragma off (unreferenced)
void
do_interrupt(int sig_num)
#endif
#if (defined BORLAND_DOS || defined MICROSOFT_DOS || defined MWC_PPC)
int
do_interrupt(void)
#endif
{
#ifdef WCC_QNX
#pragma on (unreferenced)
#endif
#if (PCM_DEBUG >= 1)
    doutput(0);
#endif
    
    printf("\nQuit? ");
#ifdef WCC_QNX
    if(getchar() == 'y') exit(1);
#endif
#ifdef BORLAND_DOS
    if(getchar() == 'y') return(0);
    else return(1);
#endif
}

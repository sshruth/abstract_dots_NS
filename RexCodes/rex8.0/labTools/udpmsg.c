
#include <stdlib.h>
#include <string.h>
#include <x86/inline.h>
#include <sys/types.h>
#include "../hdr/sys.h"
#include "../sset/ldev.h"
#include "udpmsg.h"
#include "polaris.h"

char last_recv[MAXMSGLENGTH+10] = "";

// This function is specifically designed to send packets to Matlab, do not
// use for Polaris stuff
int udpmsg_send(udpmsg *packet)
{
	char send_buf[MAXMSGLENGTH+10];
	
	if(strlen(packet->id)!=9)
	  ultoa((unsigned long)((drand48()*0.9+0.1)*1e9), packet->id, 10);
	sprintf(send_buf, "%s %s", packet->id, packet->msg);
	//printf("\nin");
    udp_send(macIP, send_buf);
    //printf("\nout\n");
		
	packet->sent = 1;
	packet->received = packet->executed = 0;
	
	return 1;
}


// This is a very weird function, look at udpCheckReceiveFork() in matlab.c for the "why"
int udpmsg_receive(udpmsg *packet)
 {
	 char recv_buf[MAXMSGLENGTH+10];
	 char senderIP[16]; // "xxx.xxx.xxx.xxx\0"
	 int ret = 0;

	 if(udp_check(0)) {

		 udp_read(recv_buf, sizeof(recv_buf), senderIP);
		 //	printf("%s\n", senderIP);
		 // First if clause determines whether message is from Psychtoolbox
		 if (!strcmp(senderIP, macIP)) {
			 if(strlen(recv_buf)>10 && recv_buf[9]==' ' && strcmp(recv_buf,last_recv)!=0) {
				 strcpy(last_recv, recv_buf);
				 strncpy(packet->id, recv_buf, 9);
				 packet->id[9] = 0;
				 strcpy(packet->msg, recv_buf+10);
				 if (strcmp(packet->msg,"RECEIVED")==0)
				   packet->received = 1;
				 else if (strcmp(packet->msg,"EXECUTED")==0)
				   packet->executed = 1;
				 else {
					 /* if it is a new message confirm its receipt */
					 strcpy(recv_buf+10, "RECEIVED");
					 udp_send(recv_buf, macIP);
				 }
				 
				 ret = 1;
			 }       
		 }
	 }
	 return ret;
 }



	
	




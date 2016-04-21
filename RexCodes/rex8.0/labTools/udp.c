/* UDP.C
*
*	UDP.c, contains a few c-routines
*	to be called from REX to communicate with another
*	machine via the UDP/IP protocol
*
*  NOTE: after connecting this machine with the Mac,
*		 directly, we still had problems until we
*		 removed the entry for "Default gateway"
*		 in the network (TCP/IP) configuration panel
*
*        JIG 7/6/06
*
*  BSH 6 Jan 2006
* 
*  LW Oct 2011 - modified to allow simult. communication
*                with Polaris Host computer. this required
*                parsing of sender and recipient addresses.
* 
* 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>
#include "udp.h"
#include "../sset/ldev.h" // for addresses and ports

/* PRIVATE DATA TYPES */
typedef struct _UDPinfo_struct *_UDPinfo;

struct _UDPinfo_struct {
	int	sockfd;			// Descriptor of UDP socket
	int	address_length;	// sizeof sockadd struct
	
	struct sockaddr_in rex_address;
	struct sockaddr_in sender_address;
	struct sockaddr_in recipient_address;
};

/* PRIVATE VARIABLES */
static struct _UDPinfo_struct gl_info = 
{0,sizeof(struct sockaddr)};

/* PUBLIC ROUTINE: udp_open
**
**	Initialize UDP socket
**
**	Arguments:
**		rexIP		... string address (xxx.xxx.xxx.xxx) 
**							of this machine
**		port		... usu 6665 (?)
*/

void udp_open(const char *thisIP,const int port)
{
	
	/* SET UP Rex MACHINE */
	gl_info.rex_address.sin_family 		  = AF_INET; 		   // Host byte order
	gl_info.rex_address.sin_port 		  = htons(port); 	   // Network byte order (short)
	gl_info.rex_address.sin_addr.s_addr   = inet_addr(thisIP); // Rex IP address
	memset(&(gl_info.rex_address.sin_zero), '\0', 8); 		   // Zero the rest
	
	/* CREATE SOCKET */
	if ((gl_info.sockfd=socket(PF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("udp_open: Could not create UDP socket");
		exit(1);
	}
	
	/* BIND SOCKET */
	if (bind(gl_info.sockfd, 
		(struct sockaddr *)&(gl_info.rex_address), 
		gl_info.address_length) == -1) {
		perror("udp_open: Could not bind UDP socket to REX address\n");
		exit(1);
	}
}

/* PUBLIC ROUTINE: udp_send
**
**	Send a string.
**
**	Arguments:
**      recipientIP ... "xxx.xxx.xxx.xxx"
**		buf         ... '\0'-terminated C-string
*/
void udp_send(const char *recipientIP, char *buf)
{
	gl_info.recipient_address.sin_family 		  = AF_INET; 				// Host byte order
	gl_info.recipient_address.sin_port 			  = htons(6665); 		    // Network byte order (short)
	gl_info.recipient_address.sin_addr.s_addr     = inet_addr(recipientIP); // Recipient IP address
	memset(&(gl_info.recipient_address.sin_zero), '\0', 8); 				// Zero the rest
	
	//printf("\n %s \n", recipientIP); 
	
	if(sendto(gl_info.sockfd, buf, strlen(buf), 0,
			  (struct sockaddr *)&(gl_info.recipient_address),
			  gl_info.address_length) == -1) {
		
		perror("udp_send: Could not send string");
		fflush(stdout);
		exit(1);
	}
	
}

/* PUBLIC ROUTINE: udp_check
**
** Is a return message available?
**
** timeout_ms is -- duh -- timeout in ms
*/

int udp_check(int timeout_ms)
{
	static struct timeval timeout;
	static fd_set readfds;

	FD_ZERO(&readfds);
	FD_SET(gl_info.sockfd,&readfds);
	if(timeout_ms < 0) {
		/* block until got something */
		select(gl_info.sockfd+1,&readfds,NULL,NULL,NULL);
	} else if(timeout_ms == 0) {
		/* check once */
		timeout.tv_sec  = 0;
		timeout.tv_usec = 0;
		select(gl_info.sockfd+1,&readfds,NULL,NULL,&timeout);
	} else {
		/* check until timeout_ms */
		timeout.tv_sec  = timeout_ms/1000L;
		timeout.tv_usec = (timeout_ms%1000L)*1000L;
		select(gl_info.sockfd+1,&readfds,NULL,NULL,&timeout);
	}	
	return(FD_ISSET(gl_info.sockfd,&readfds));
}

/* PUBLIC ROUTINE: udp_read
**
**	Read any available message and indicate senderIP address
*/

void udp_read(char buf[], int len, char senderIP[])
{
	
	if((len=recvfrom(gl_info.sockfd, buf, len, 0,
					 (struct sockaddr *)&(gl_info.sender_address), 
					 &(gl_info.address_length))) == -1) {
		
		perror("udp_read: Did not read correct number of bytes from remote machine");
		exit(1);
	}
	
	buf[len] = '\0';

	strcpy(senderIP, inet_ntoa(gl_info.sender_address.sin_addr));
	len = strlen(senderIP);
	senderIP[len] = '\0';
	
	/*
	 **	printf("Got packet from %s\n",inet_ntoa(gl_info.other_address.sin_addr));
	 **	printf("packet is %d bytes long\n",len);
	 **	printf("packet contains \"%s\"\n",buf);
	 */
}

/* PUBLIC ROUTINE: udp_close
**
**	Cleanup UDP socket
*/
void udp_close (void)
{
//	printf("Sock exists = %d\n", gl_info.sockfd); 
	if(gl_info.sockfd)
		close(gl_info.sockfd);
}

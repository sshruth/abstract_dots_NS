#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "rexhdrs.h"
#include "pcsSocket.h"
#include "timerFunction.h"

/* private global variables */
char *peerAddress;
int peerAddrLen;
int senderAddrLen;
int recvMsgLen;
int sendMsgLen;
char errorMsg[64];
char msgRecvd[MSG_BUFFER_LEN];
char *msg2send;
unsigned long nonBlock;
struct sockaddr_in peerAddr;
struct sockaddr_in senderAddr;
struct sockaddr_in currentAddr;
SOCKET pcsSock;

/* private function declarations */
int sendtoPackets(char *buf, int bufLength);
int recvfromPackets(char *buf, int bufLength);
int sendPackets(char *buf, int bufLength);
int recvPackets(char *buf, int bufLength);
SOCKET passiveUDP(const char *subnet, const char *service);
SOCKET passiveSock(const char *subnet, const char *service, const char *transport, int qlen);

/* Public function definitions */
// This function initializes private global variables
void pcsSocket()
{
	// initialize the lengths of the receive and send buffers
	recvMsgLen = 0;
	sendMsgLen = 0;

	// initialize the socket descriptor
	pcsSock = 0;

	// initialize the received message buffer to 0
	memset(msgRecvd, 0, sizeof(msgRecvd));

	// clear the address structure of the rex machine
	memset(&peerAddr, 0, sizeof(peerAddr));

	// initialize the address string of the peer machine
	peerAddress = (char *)NULL;
}

// This function closes down the socket
void pcsClose(void)
{
	if(pcsSock != 0) {
		close(pcsSock);
	}
	pcsSock = 0;
}

// This function is the public interface to socket allocation
int pcsAllocPassiveSocket(const char *subnet, const char *port)
{
	int sockFlags;

	// if the socket has already been allocated, just return
	if(pcsSock) return(0);
	
	// allocate an unconnected UDP socket
	pcsSock = passiveUDP(subnet, port);

	// if no socket could be allocated, print an error and exit
	if(pcsSock == SOCKET_ERROR) {
		rxerr("allocPassiveSocket: couldn't allocate socket");
		return(-1);
	}

	// set the socket to non-blocking
	if((sockFlags = fcntl(pcsSock, F_GETFL), 0) == SOCKET_ERROR) {
		rxerr("allocPassiveSocket: couldn't get socket flags");
		return(-1);
	}
	if(fcntl(pcsSock, F_SETFL, (sockFlags | O_NONBLOCK)) == SOCKET_ERROR) {
		rxerr("allocPassiveSocket: couldn't set socket to non-blocking");
		return(-1);
	}

	return(0);
}

// This function must be called periodically to send messages and to
// check the socket for incoming messages
void pcsScktProcess(void)
{
	int lenMsgSent;
	
	if(!pcsSock) return;
	
	// check to see if there is a message to send
	if(sendMsgLen) {	// if there is a message to send
		lenMsgSent = sendtoPackets(msg2send, sendMsgLen);
		if(lenMsgSent < 0) {	// if sendtoPackets returned an error
			sprintf(errorMsg, "pcsScktProcess(): error %d when sending message",
					-lenMsgSent);
			rxerr(errorMsg);
		}
		if(lenMsgSent != sendMsgLen) {	// if the entire message was not sent
			sprintf(errorMsg, "pcsScktProcess(): error in sending %d length message",
					sendMsgLen);
			rxerr(errorMsg);
		}
		sendMsgLen = 0;                   // reset send msg length to 0
	}

	// check to see if a message is in the queue
	recvMsgLen = recvfromPackets(msgRecvd, sizeof(msgRecvd)); // for an unconnected socket
	if(recvMsgLen < 0) {
		sprintf(errorMsg, "pcsScktProcess(): error %d in receiving packets",
				-recvMsgLen);
		rxerr(errorMsg);
		recvMsgLen = 0;
	}
	msgRecvd[recvMsgLen] = 0;				// null terminate the received string
}

// This function returns a pointer to the last message taken from the socket
char *pcsRecvdMsg(void)
{
	if(recvMsgLen) return(msgRecvd);
	else return((char *)NULL);
}

// This function returns the length of the last message taken from the socket
int pcsRecvdMsgLen(void)
{
	return(recvMsgLen);
}

// This funtion sets the pointer to a message to send and its length
void pcsSend(char *buf, int buflen)
{
	int lenMsgSent;

	msg2send = buf;
	sendMsgLen = buflen;

	if(sendMsgLen) {	// if there is a message to send
		lenMsgSent = sendtoPackets(msg2send, sendMsgLen);
		if(lenMsgSent < 0) {	// if sendtoPackets returned an error
			sprintf(errorMsg, "pcsSend(): error %d when sending message",
					-lenMsgSent);
			rxerr(errorMsg);
		}
		if(lenMsgSent != sendMsgLen) {	// if the entire message was not sent
			sprintf(errorMsg, "pcsSend(): error in sending %d length message",
					sendMsgLen);
			rxerr(errorMsg);
		}
		sendMsgLen = 0;                   // reset send msg length to 0
	}
}

// This function sets the address of the peer
// with which this machine will communicate
int pcsSetPeerAddr(const char *host, const char *port)
{
	struct hostent *phe;

    // set the socket family
	peerAddr.sin_family = AF_INET;

	// set the socket address to rex machine's address
	if((phe = gethostbyname(host)) == NULL) {           // if host name is unknown
		rxerr("can't get host name");
		return(-1);
	}

	// copy the host name into the peer address structure
	memcpy(&peerAddr.sin_addr, phe->h_addr, phe->h_length);

	// set the port number of the socket
	if((peerAddr.sin_port = htons((u_short)atoi(port))) == 0) { // if the port can't be assigned
		rxerr("can't get port entry");
		return(-1);
	}

	// set the length of the peerAddr structure
	peerAddrLen = sizeof(peerAddr);

	// get the peer IP address string
	if(!inet_ntoa(peerAddr.sin_addr)){
		rxerr("can't get peer IP address");
		return(-1);
	}

	// copy the IP address of the peer
	peerAddress = (char *)malloc((strlen((const char*)inet_ntoa(peerAddr.sin_addr)) + 1) * sizeof(char));
	if(!peerAddress) {
		rxerr("can't allocate peer IP address string");
		return(-1);
	}
	strcpy(peerAddress, (const char *)inet_ntoa(peerAddr.sin_addr));
	
	return(0);
}

/* private function definitions */
// wrapper function for passiveSock
SOCKET passiveUDP(const char *subnet, const char *port)
{
	return(passiveSock(subnet, port, "udp", 0));
}

// This function allocates a TCP server socket or an unconnected UDP socket
SOCKET passiveSock(const char *subnet, const char *port, const char *transport, int qlen)
{
	struct protoent *ppe;   /* pointer to protocol information entry */
	struct sockaddr_in sin; /* an Internet endpoint address  */
	SOCKET s;                               /* socket descriptor */
	int type;                               /* socket type (SOCK_STREAM, SOCK_DGRAM) */
	char errmsg[64];

	// initialize the socket address structure
	memset(&sin, 0, sizeof(sin));

	// set the socket family type to internet
	sin.sin_family = AF_INET;

	// if the user selected a subnet, set the socket
	// to accept connections only from that subnet
	if(subnet && strlen(subnet) && inet_addr(subnet)) sin.sin_addr.s_addr = inet_addr(subnet);

	// otherwise accept messages from any subnet
	else sin.sin_addr.s_addr = INADDR_ANY;

	/* Map port name to port number */
	if((sin.sin_port = htons((u_short)atoi(port))) == 0) {
		rxerr("can't get port entry");
		s = SOCKET_ERROR;
	}

	/* Map protocol name to protocol number */
	if((ppe = getprotobyname(transport)) == 0) {
		rxerr("can't get protocol entry");
		s = SOCKET_ERROR;
	}

	/* Use protocol to choose a socket type */
	if (strcmp(transport, "udp") == 0) type = SOCK_DGRAM;
	else type = SOCK_STREAM;

	/* Allocate a socket */
	if((s = socket(AF_INET, type, ppe->p_proto)) == SOCKET_ERROR){
		rxerr("can't create socket");
	}

	/* Bind the socket */
	if(bind(s, (struct sockaddr *)&sin, sizeof(sin)) == SOCKET_ERROR) {
		sprintf(errmsg, "can't bind to %s port", port);
		rxerr(errmsg);
	}

	// if this is a stream socket, start listening on the socket
	if(type == SOCK_STREAM && listen(s, qlen) == SOCKET_ERROR) {
		rxerr("can't listen on port");
		s = SOCKET_ERROR;
	}
	return s;
}

// This function gets one message, if any are available, out of an unconnected UDP buffer
int recvfromPackets(char *buf, int bufLength)
{
	int retcode;
	int errorcode;

	senderAddrLen = sizeof(senderAddr);
	/* 	startTimer(1); */
	retcode = recvfrom(pcsSock, buf, bufLength, 0, (struct sockaddr *)&senderAddr, &senderAddrLen);
	/* 	stopTimer(1); */
	
	// check to see if recvfrom returned an error
	if(retcode == SOCKET_ERROR) {
		errorcode = errno;

		// if the error just indicates that nothing was received,
		// return zero
		if(errorcode == EWOULDBLOCK) return(0);

		// if there was any other kind of error, print it out and exit
		else return(-errorcode);
	}

	// if the user specified a client machine, check to see if the message came from that client
	else if(strcmp(peerAddress, (const char *)inet_ntoa(senderAddr.sin_addr))) {
		// if the message came from some other machine, print a warning and return 0 bytes
		sprintf(errorMsg, "Rejecting message from machine %s\n", inet_ntoa(senderAddr.sin_addr));
		rxerr(errorMsg);
		return(0);
	}
        
	// if the message came from the specified client, return the number of bytes in the message
	return(retcode);
}

// This function sends a message to the selected peer through an unconnected UDP socket
int sendtoPackets(char *buf, int bufLength)
{
	int retcode;
	int errorcode;

	// send the message to the peer specified on the command line
	/* 	startTimer(2); */
	retcode = sendto(pcsSock, buf, bufLength, 0, (struct sockaddr *)&peerAddr, peerAddrLen);
	/* 	stopTimer(2); */

	if(retcode == SOCKET_ERROR) {
		errorcode = errno;
		return(-errorcode);
	}
	return(retcode);
}

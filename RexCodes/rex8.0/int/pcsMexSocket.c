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

#ifdef PCS_SOCKET
#ifdef MEX62_UNITS

/* private global variables */
char *mexPeerAddress;
int mexPeerAddrLen;
int mexSenderAddrLen;
int recvMexMsgLen;
int sendMexMsgLen;
char mexMsgRecvd[MSG_BUFFER_LEN];
char *msg2sendMex;
struct sockaddr_in mexPeerAddr;
struct sockaddr_in mexSenderAddr;
struct sockaddr_in currentAddr;
SOCKET pcsMexSock;

/* private function declarations */
int sendtoMexPackets(char *buf, int bufLength);
int recvfromMexPackets(char *buf, int bufLength);
SOCKET passiveMexUDP(const char *subnet, const char *service);
SOCKET passiveMexSock(const char *subnet, const char *service, const char *transport, int qlen);

/* Public function definitions */
// This function initializes private global variables
void pcsMexSocket()
{

	// initialize the lengths of the receive and send buffers
	recvMexMsgLen = 0;
	sendMexMsgLen = 0;

	// initialize the socket descriptor
	pcsMexSock = 0;

	// initialize the received message buffer to 0
	memset(mexMsgRecvd, 0, sizeof(mexMsgRecvd));

	// clear the address structure of the rex machine
	memset(&mexPeerAddr, 0, sizeof(mexPeerAddr));

	// initialize the address string of the mexPeer machine
	mexPeerAddress = (char *)NULL;
}

// This function closes down the socket
void pcsCloseMex(void)
{
	if(pcsMexSock > 0) {
		close(pcsMexSock);
		pcsMexSock = 0;
	}
}

void pcsConnectMex(char *host)
{
	char *port = MEX_PORT_STR;
	char *subnet = 0;

	pcsSetMexPeerAddr(host, port);
	pcsAllocPassiveMexSocket(subnet, port);
}

int pcsMexConnected(void)
{
	return(pcsMexSock);
}

// This function sets the address of the mexPeer
// with which this machine will communicate
void pcsSetMexPeerAddr(const char *host, const char *port)
{
	struct hostent *phe;

	// set the socket family
	mexPeerAddr.sin_family = AF_INET;

	// set the socket address to rex machine's address
	if((phe = gethostbyname(host)) == NULL) {           // if host name is unknown
		rxerr("pcsSetMexPeerAddr() can't get host name");
		exit(1);
	}

	// copy the host name into the mexPeer address structure
	memcpy(&mexPeerAddr.sin_addr, phe->h_addr, phe->h_length);

	// set the port number of the socket
	if((mexPeerAddr.sin_port = htons((u_short)atoi(port))) == 0) { // if the port can't be assigned
		rxerr("pcsSetMexPeerAddr() can't get port entry");
		exit(1);
	}

	// set the length of the mexPeerAddr structure
	mexPeerAddrLen = sizeof(mexPeerAddr);

	// get the mexPeer IP address string
	if(!inet_ntoa(mexPeerAddr.sin_addr)){
		rxerr("pcsSetMexPeerAddr() can't get peer IP address");
		exit(1);
	}

	// copy the IP address of the mexPeer
	mexPeerAddress = (char *)malloc((strlen((const char*)inet_ntoa(mexPeerAddr.sin_addr)) + 1) * sizeof(char));
	if(!mexPeerAddress) {
		rxerr("pcsSetMexPeerAddr() can't allocate peer IP address string");
		exit(1);
	}
	strcpy(mexPeerAddress, (const char *)inet_ntoa(mexPeerAddr.sin_addr));
}

// This function is the public interface to socket allocation
void pcsAllocPassiveMexSocket(const char *subnet, const char *port)
{
	int sockFlags;

	// if the socket has already been allocated, just return
	if(pcsMexSock) return;
	
	// allocate an unconnected UDP socket
	pcsMexSock = passiveMexUDP(subnet, port);

	// if no socket could be allocated, print an error and exit
	if(pcsMexSock == SOCKET_ERROR) {
		rxerr("pcsAllocPassiveMexSocket() couldn't allocate socket");
	}

	// set the socket to non-blocking
	if((sockFlags = fcntl(pcsMexSock, F_GETFL), 0) == SOCKET_ERROR) {
		rxerr("pcsAllocPassiveMexSocket() couldn't get socket flags");
	}
	if(fcntl(pcsMexSock, F_SETFL, (sockFlags | O_NONBLOCK)) == SOCKET_ERROR) {
		rxerr("pcsAllocPassiveMexSocket() couldn't set socket to non-blocking");
	}
}

// This function must be called periodically to send messages and to
// check the socket for incoming messages
void pcsScktProcessMex(void)
{
	int lenMsgSentMex;
	char errorMsg[64];
	
	if(!pcsMexSock) return;

	// check to see if there is a message to send
	if(sendMexMsgLen) {	// if there is a message to send
		lenMsgSentMex = sendtoMexPackets(msg2sendMex, sendMexMsgLen);
		if(lenMsgSentMex < 0) {	// if sendtoMexPackets returned an error
			sprintf(errorMsg, "pcsScktProcessMex() error %d when sending message",
					-lenMsgSentMex);
			rxerr(errorMsg);
		}
		if(lenMsgSentMex != sendMexMsgLen) {	// if the entire message was not sent
			sprintf(errorMsg, "pcsScktProcessMex() error in sending %d length message",
					sendMexMsgLen);
			rxerr(errorMsg);
		}
		sendMexMsgLen = 0;                   // reset send msg length to 0
	}

	// check to see if a message is in the queue
	recvMexMsgLen = recvfromMexPackets(mexMsgRecvd, sizeof(mexMsgRecvd)); // for an unconnected socket
	if(recvMexMsgLen < 0) {
		sprintf(errorMsg, "pcsScktProcessMex() error %d in receiving packets",
				-recvMexMsgLen);
		rxerr(errorMsg);
		recvMexMsgLen = 0;
	}
	mexMsgRecvd[recvMexMsgLen] = 0;				// null terminate the received string
}

// This funtion sets the pointer to a message to send and its length
void pcsSendMex(char *buf, int buflen)
{
	msg2sendMex = buf;
	sendMexMsgLen = buflen;
}

// This function returns the length of the last message taken from the socket
int pcsRecvdMexMsgLen(void)
{
	return(recvMexMsgLen);
}

// This function returns a pointer to the last message taken from the socket
char *pcsRecvdMexMsg(void)
{
	if(recvMexMsgLen) return(mexMsgRecvd);
	else return((char *)NULL);
}


/* private function definitions */
// wrapper function for passiveMexSock
SOCKET passiveMexUDP(const char *subnet, const char *port)
{
	return(passiveMexSock(subnet, port, "udp", 0));
}

// This function allocates a TCP server socket or an unconnected UDP socket
SOCKET passiveMexSock(const char *subnet, const char *port, const char *transport, int qlen)
{
	struct protoent *ppe;   /* pointer to protocol information entry */
	struct sockaddr_in sin; /* an Internet endpoint address  */
	SOCKET s;                               /* socket descriptor */
	int type;                               /* socket type (SOCK_STREAM, SOCK_DGRAM) */
	int reuse;
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
		rxerr("passiveMexSock() can't get port entry");
		s = SOCKET_ERROR;
	}

	/* Map protocol name to protocol number */
	if((ppe = getprotobyname(transport)) == 0) {
		rxerr("passiveMexSock() can't get protocol entry");
		s = SOCKET_ERROR;
	}

	/* Use protocol to choose a socket type */
	if (strcmp(transport, "udp") == 0) type = SOCK_DGRAM;
	else type = SOCK_STREAM;

	/* Allocate a socket */
	if((s = socket(AF_INET, type, ppe->p_proto)) == SOCKET_ERROR){
		rxerr("passiveMexSock() can't create socket");
	}

	if(s != SOCKET_ERROR) {
		/* set the reuse port option */
		reuse = 1;
		setsockopt(s, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse));
		
		/* Bind the socket */
		if(bind(s, (struct sockaddr *)&sin, sizeof(sin)) == SOCKET_ERROR) {
			sprintf(errmsg, "passiveMexSock() can't bind to %s port", port);
			rxerr(errmsg);
		}
	}

	// if this is a stream socket, start listening on the socket
	if(type == SOCK_STREAM && listen(s, qlen) == SOCKET_ERROR) {
		rxerr("passiveMexSock() can't listen on port");
		s = SOCKET_ERROR;
	}
	return s;
}

// This function gets one message, if any are available, out of an unconnected UDP buffer
int recvfromMexPackets(char *buf, int bufLength)
{
	int retcode;
	int errorcode;
	char errorMsg[64];

	mexSenderAddrLen = sizeof(mexSenderAddr);
	/* 	startTimer(1); */
	retcode = recvfrom(pcsMexSock, buf, bufLength, 0, (struct sockaddr *)&mexSenderAddr, &mexSenderAddrLen);
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
	else if(strcmp(mexPeerAddress, (const char *)inet_ntoa(mexSenderAddr.sin_addr))) {
		// if the message came from some other machine, print a warning and return 0 bytes
		sprintf(errorMsg, "recvfromMexPackets() Rejecting message from machine %s\n", inet_ntoa(mexSenderAddr.sin_addr));
		rxerr(errorMsg);
		return(0);
	}
        
	// if the message came from the specified client, return the number of bytes in the message
	return(retcode);
}

// This function sends a message to the selected mexPeer through an unconnected UDP socket
int sendtoMexPackets(char *buf, int bufLength)
{
	int retcode;
	int errorcode;

	// send the message to the mexPeer specified on the command line
	/* 	startTimer(2); */
	retcode = sendto(pcsMexSock, buf, bufLength, 0, (struct sockaddr *)&mexPeerAddr, mexPeerAddrLen);
	/* 	stopTimer(2); */

	if(retcode == SOCKET_ERROR) {
		errorcode = errno;
		return(-errorcode);
	}
	return(retcode);
}

#endif
#endif

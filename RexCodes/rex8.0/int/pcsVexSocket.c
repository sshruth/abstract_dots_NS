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

/* private global variables */
char *vexPeerAddress;
int vexPeerAddrLen;
int vexSenderAddrLen;
int recvVexMsgLen;
int sendVexMsgLen;
char vexMsgRecvd[MSG_BUFFER_LEN];
char *msg2sendVex;
struct sockaddr_in vexPeerAddr;
struct sockaddr_in vexSenderAddr;
struct sockaddr_in currentAddr;
SOCKET pcsVexSock;

/* private function declarations for UDP sockets*/
int sendtoVexPackets(char *buf, int bufLength);
int recvfromVexPackets(char *buf, int bufLength);
SOCKET passiveVexUDP(const char *subnet, const char *service);
SOCKET passiveVexSock(const char *subnet, const char *service, const char *transport, int qlen);

/* private function declarations for TCP sockets */
int writePackets(char *buf, int bufLength);
int readPackets(char *buf, int bufLength);
SOCKET connectTCP(const char *subnet, const char *service);
SOCKET connectSock(const char *subnet, const char *service, const char *transport);

/* pointers to socket read and write functions */
int (*writeSocket)(char *buf, int bufLength);
int (*readSocket)(char *buf, int bufLength);

/* Public function definitions */
// This function initializes private global variables
void pcsVexSocket()
{
	// initialize the lengths of the receive and send buffers
	recvVexMsgLen = 0;
	sendVexMsgLen = 0;

	// initialize the socket descriptor
	pcsVexSock = 0;

	// initialize the received message buffer to 0
	memset(vexMsgRecvd, 0, sizeof(vexMsgRecvd));

	// clear the address structure of the rex machine
	memset(&vexPeerAddr, 0, sizeof(vexPeerAddr));

	// initialize the address string of the vexPeer machine
	vexPeerAddress = (char *)NULL;
}

// This function closes down the socket
void pcsCloseVex(void)
{
	if(pcsVexSock > 0) {
		close(pcsVexSock);
		pcsVexSock = 0;
	}
}

void pcsConnectVex(char *host)
{
	char *port = GLVEX_PORT_STR;
	char *subnet = 0;

	pcsSetVexPeerAddr(host, port);
	pcsAllocPassiveVexSocket(subnet, port);
}

void pcsConnectTcp(char *host)
{
	char *port = GLVEX_PORT_STR;
	char *subnet = 0;

	pcsAllocConnectVexSocket(host, port);
}

int pcsVexConnected(void)
{
	return(pcsVexSock);
}

// This function sets the address of the vexPeer
// with which this machine will communicate
void pcsSetVexPeerAddr(const char *host, const char *port)
{
	struct hostent *phe;

	// set the socket family
	vexPeerAddr.sin_family = AF_INET;

	// set the socket address to rex machine's address
	if((phe = gethostbyname(host)) == NULL) {           // if host name is unknown
		rxerr("pcsSetVexPeerAddr() can't get host name");
		exit(1);
	}

	// copy the host name into the vexPeer address structure
	memcpy(&vexPeerAddr.sin_addr, phe->h_addr, phe->h_length);

	// set the port number of the socket
	if((vexPeerAddr.sin_port = htons((u_short)atoi(port))) == 0) { // if the port can't be assigned
		rxerr("pcsSetVexPeerAddr() can't get port entry");
		exit(1);
	}

	// set the length of the vexPeerAddr structure
	vexPeerAddrLen = sizeof(vexPeerAddr);

	// get the vexPeer IP address string
	if(!inet_ntoa(vexPeerAddr.sin_addr)){
		rxerr("pcsSetVexPeerAddr() can't get peer IP address");
		exit(1);
	}

	// copy the IP address of the vexPeer
	vexPeerAddress = (char *)malloc((strlen((const char*)inet_ntoa(vexPeerAddr.sin_addr)) + 1) * sizeof(char));
	if(!vexPeerAddress) {
		rxerr("pcsSetVexPeerAddr() can't allocate peer IP address string");
		exit(1);
	}
	strcpy(vexPeerAddress, (const char *)inet_ntoa(vexPeerAddr.sin_addr));
}

// This function is the public interface to socket allocation
void pcsAllocConnectVexSocket(const char *host, const char *port)
{
	int sockFlags;

	// if the socket has already been allocated, just return
	if(pcsVexSock) return;
	
	// allocate a TCP socket
	pcsVexSock = connectTCP(host, port);

	// if no socket could be allocated, print an error and exit
	if(pcsVexSock == SOCKET_ERROR) {
		rxerr("pcsAllocConnectVexSocket: couldn't allocate socket");
	}

	// set the socket to non-blocking
	if((sockFlags = fcntl(pcsVexSock, F_GETFL), 0) == SOCKET_ERROR) {
		rxerr("pcsAllocConnectVexSocket: couldn't get socket flags");
	}
	if(fcntl(pcsVexSock, F_SETFL, (sockFlags | O_NONBLOCK)) == SOCKET_ERROR) {
		rxerr("pcsAllocConnectVexSocket: couldn't set socket to non-blocking");
	}

	// set the read and write functions
	writeSocket = writePackets;
	readSocket = readPackets;
}

// This function is the public interface to socket allocation
void pcsAllocPassiveVexSocket(const char *subnet, const char *port)
{
	int sockFlags;

	// if the socket has already been allocated, just return
	if(pcsVexSock) return;
	
	// allocate an unconnected UDP socket
	pcsVexSock = passiveVexUDP(subnet, port);

	// if no socket could be allocated, print an error and exit
	if(pcsVexSock == SOCKET_ERROR) {
		rxerr("pcsAllocPassiveVexSocket() couldn't allocate socket");
	}

	// set the socket to non-blocking
	if((sockFlags = fcntl(pcsVexSock, F_GETFL), 0) == SOCKET_ERROR) {
		rxerr("pcsAllocPassiveVexSocket() couldn't get socket flags");
	}
	if(fcntl(pcsVexSock, F_SETFL, (sockFlags | O_NONBLOCK)) == SOCKET_ERROR) {
		rxerr("pcsAllocPassiveVexSocket() couldn't set socket to non-blocking");
	}

	// set the read and write functions
	writeSocket = sendtoVexPackets;
	readSocket = recvfromVexPackets;
}

// This function must be called periodically to send messages and to
// check the socket for incoming messages
void pcsScktProcessVex(void)
{
	int lenMsgSentVex;
	char errorMsg[64];
	
	if(!pcsVexSock) return;
	
	// check to see if there is a message to send
	if(sendVexMsgLen) {	// if there is a message to send
		lenMsgSentVex = writeSocket(msg2sendVex, sendVexMsgLen);
		if(lenMsgSentVex < 0) {	// if sendtoVexPackets returned an error
			sprintf(errorMsg, "pcsScktProcessVex() error %d when sending message",
					-lenMsgSentVex);
			rxerr(errorMsg);
		}
		if(lenMsgSentVex != sendVexMsgLen) {	// if the entire message was not sent
			sprintf(errorMsg, "pcsScktProcessVex() error in sending %d length message",
					sendVexMsgLen);
			rxerr(errorMsg);
		}
		sendVexMsgLen = 0;                   // reset send msg length to 0
	}

	// check to see if a message is in the queue
	recvVexMsgLen = readSocket(vexMsgRecvd, sizeof(vexMsgRecvd)); // for an unconnected socket
	if(recvVexMsgLen < 0) {
		sprintf(errorMsg, "pcsScktProcessVex() error %d in receiving packets",
				-recvVexMsgLen);
		rxerr(errorMsg);
		recvVexMsgLen = 0;
	}
	vexMsgRecvd[recvVexMsgLen] = 0;				// null terminate the received string
}

// This funtion sets the pointer to a message to send and its length
void pcsSendVex(char *buf, int buflen)
{
	msg2sendVex = buf;
	sendVexMsgLen = buflen;
}

// This function returns the length of the last message taken from the socket
int pcsRecvdVexMsgLen(void)
{
	return(recvVexMsgLen);
}

// This function returns a pointer to the last message taken from the socket
char *pcsRecvdVexMsg(void)
{
	if(recvVexMsgLen) return(vexMsgRecvd);
	else return((char *)NULL);
}


/* private function definitions */
// wrapper function for passiveVexSock
SOCKET passiveVexUDP(const char *subnet, const char *port)
{
	return(passiveVexSock(subnet, port, "udp", 0));
}

// This function allocates a TCP server socket or an unconnected UDP socket
SOCKET passiveVexSock(const char *subnet, const char *port, const char *transport, int qlen)
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
		rxerr("passiveVexSock() can't get port entry");
		s = SOCKET_ERROR;
	}

	/* Map protocol name to protocol number */
	if((ppe = getprotobyname(transport)) == 0) {
		rxerr("passiveVexSock() can't get protocol entry");
		s = SOCKET_ERROR;
	}

	/* Use protocol to choose a socket type */
	if (strcmp(transport, "udp") == 0) type = SOCK_DGRAM;
	else type = SOCK_STREAM;

	/* Allocate a socket */
	if((s = socket(AF_INET, type, ppe->p_proto)) == SOCKET_ERROR){
		rxerr("passiveVexSock() can't create socket");
	}

	if(s != SOCKET_ERROR) {
		/* set the reuse port option */
		reuse = 1;
		setsockopt(s, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse));

		/* Bind the socket */
		if(bind(s, (struct sockaddr *)&sin, sizeof(sin)) == SOCKET_ERROR) {
			sprintf(errmsg, "passiveVexSock() can't bind to %s port", port);
			rxerr(errmsg);
		}
	}

	// if this is a stream socket, start listening on the socket
	if(type == SOCK_STREAM && listen(s, qlen) == SOCKET_ERROR) {
		rxerr("passiveVexSock() can't listen on port");
		s = SOCKET_ERROR;
	}
	return s;
}

// wrapper function for connectSock
SOCKET connectTCP(const char *host, const char *port)
{
	return(connectSock(host, port, "tcp"));
}

// This function allocates a TCP client socket
SOCKET connectSock(const char *host, const char *port, const char *transport)
{
	struct protoent *ppe;   /* pointer to protocol information entry */
	struct hostent *phe;	/* pointer to host name information */
	struct sockaddr_in sin; /* an Internet endpoint address  */
	SOCKET s;                               /* socket descriptor */
	int type;                               /* socket type (SOCK_STREAM, SOCK_DGRAM) */
	char errmsg[64];

	// initialize the socket address structure
	memset(&sin, 0, sizeof(sin));

	// set the socket family type to internet
	sin.sin_family = AF_INET;

	// set the socket address to rex machine's address
	if((phe = gethostbyname(host)) != NULL) {           // if host name is unknown
		// copy the host name into the peer address structure
		memcpy(&sin.sin_addr, phe->h_addr, phe->h_length);
	}
	else {
		rxerr("can't get host name");
		exit(1);
	}

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

	/* Connect the socket */
	if(connect(s, (struct sockaddr *)&sin, sizeof(sin)) == SOCKET_ERROR) {
		sprintf(errmsg, "can't bind to %s port", port);
		rxerr(errmsg);
	}

	return(s);
}

// This function gets one message, if any are available, out of an unconnected UDP buffer
int recvfromVexPackets(char *buf, int bufLength)
{
	int retcode;
	int errorcode;
	char errorMsg[64];

	vexSenderAddrLen = sizeof(vexSenderAddr);
	/* 	startTimer(1); */
	retcode = recvfrom(pcsVexSock, buf, bufLength, 0, (struct sockaddr *)&vexSenderAddr, &vexSenderAddrLen);
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
	else if(strcmp(vexPeerAddress, (const char *)inet_ntoa(vexSenderAddr.sin_addr))) {
		// if the message came from some other machine, print a warning and return 0 bytes
		sprintf(errorMsg, "recvFromVexPackets() Rejecting message from machine %s\n", inet_ntoa(vexSenderAddr.sin_addr));
		rxerr(errorMsg);
		return(0);
	}
        
	// if the message came from the specified client, return the number of bytes in the message
	return(retcode);
}

// This function sends a message to the selected vexPeer through an unconnected UDP socket
int sendtoVexPackets(char *buf, int bufLength)
{
	int retcode;
	int errorcode;

	// send the message to the vexPeer specified on the command line
	/* 	startTimer(2); */
	retcode = sendto(pcsVexSock, buf, bufLength, 0, (struct sockaddr *)&vexPeerAddr, vexPeerAddrLen);
	/* 	stopTimer(2); */

	if(retcode == SOCKET_ERROR) {
		errorcode = errno;
		return(-errorcode);
	}
	return(retcode);
}

// This function gets one message, if any are available, out of a connected TCP buffer
int readPackets(char *buf, int bufLength)
{
	int retcode;
	int errorcode;
	int n;
	char errorMsg[64];

	/* 	startTimer(1); */
	retcode = 0;
	while((n = read(pcsVexSock, &buf[retcode], bufLength)) > 0) {
		retcode += n;
		bufLength -= n;
	}
	/* 	stopTimer(1); */
	
	// check to see if recvfrom returned an error
	if(retcode == SOCKET_ERROR) {
		errorcode = errno;

		// if the error just indicates that nothing was received,
		// return zero
		if(errorcode == EAGAIN) return(retcode);

		// if there was any other kind of error, print it out and exit
		else return(-errorcode);
	}
        
	// return the number of bytes in the message
	return(retcode);
}

// This function sends a message to the selected peer through an unconnected UDP or TCP socket
int writePackets(char *buf, int bufLength)
{
	int retcode;
	int errorcode;

	// send the message to the peer specified on the command line
	/* 	startTimer(2); */
	retcode = write(pcsVexSock, buf, bufLength);
	/* 	stopTimer(2); */

	if(retcode == SOCKET_ERROR) {
		errorcode = errno;
		return(-errorcode);
	}
	return(retcode);
}

#endif

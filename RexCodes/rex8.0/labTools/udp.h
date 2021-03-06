/* UDP.H
*
*	header file for UDP.c, which contains a few c-routines
*	to be called from REX to communicate to another
*	machine via udp/ip protocols
*
*	BSH 6 Jan 2006, LW Oct 2011
*/

#ifndef UDP_H_
#define UDP_H_

/* PUBLIC ROUTINE PROTOTYPES */

void udp_open		(const char *, int);	// Initialize UDP socket
void udp_send		(const char *, char *);	// Send a string to specific address
int  udp_check		(int);					// Is incoming message available?
void udp_read		(char *, int, char *);	// Read any incoming message and store sender address
void udp_close		(void);					// Cleanup UDP socket 

#endif /* UDP_H_ */

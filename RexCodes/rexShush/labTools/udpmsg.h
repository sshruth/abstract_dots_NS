

#ifndef _UDPMSG_H_
#define _UDPMSG_H_

#include "udp.h"

#define MAXMSGLENGTH (256-10)

typedef struct udpmsg_struct {
   char id[10];
   char msg[MAXMSGLENGTH];
   long sent, received, executed;
} udpmsg;

#define IS_RECEIVED	1
#define IS_EXECUTED	2

int udpmsg_send 				(udpmsg *packet);
int udpmsg_receive 			    (udpmsg *packet);
void udpmsg_parsePolaris         (char *buf);

#endif

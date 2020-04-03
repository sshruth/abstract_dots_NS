// pcsSocket.h: interface for the vexSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PCSSOCKET_H__1C56E603_F1C8_11D2_B2BE_00104B93918B__INCLUDED_)
#define AFX_PCSSOCKET_H__1C56E603_F1C8_11D2_B2BE_00104B93918B__INCLUDED_

#define GLVEX_PORT      9998
#define GLVEX_PORT_STR "9998"
#define MSG_BUFFER_LEN 16384
#define SOCKET_ERROR	-1

typedef int SOCKET;

// public function declarations
void pcsSocket(void);
void pcsClose(void);
int pcsSetPeerAddr(const char *host, const char *port);
int pcsAllocPassiveSocket(const char *subnet, const char *port);
void pcsScktProcess(void);
void pcsSend(char *buf, int buflen);
int pcsRecvdMsgLen(void);
char *pcsRecvdMsg(void);

#endif // !defined(AFX_PCSSOCKET_H__1C56E603_F1C8_11D2_B2BE_00104B93918B__INCLUDED_)

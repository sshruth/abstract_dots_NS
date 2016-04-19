// pcsSocket.h: interface for the vexSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PCSSOCKET_H__1C56E603_F1C8_11D2_B2BE_00104B93918B__INCLUDED_)
#define AFX_PCSSOCKET_H__1C56E603_F1C8_11D2_B2BE_00104B93918B__INCLUDED_

#define GLVEX_PORT		9999
#define GLVEX_PORT_STR	"9999"

#define MEX_PORT		9997
#define MEX_PORT_STR	"9997"

#define MSG_BUFFER_LEN	16384
#define SOCKET_ERROR	-1

typedef int SOCKET;

// vex public function declarations
void pcsVexSocket(void);
void pcsCloseVex(void);
void pcsConnectVex(char *host);
void pcsConnectTcp(char *host);
int pcsVexConnected(void);
void pcsSetVexPeerAddr(const char *host, const char *port);
void pcsAllocConnectVexSocket(const char *host, const char *port);
void pcsAllocPassiveVexSocket(const char *subnet, const char *port);
void pcsScktProcessVex(void);
void pcsSendVex(char *buf, int buflen);
int pcsRecvdVexMsgLen(void);
char *pcsRecvdVexMsg(void);

// mex public function declarations
void pcsMexSocket(void);
void pcsCloseMex(void);
void pcsConnectMex(char *host);
int pcsMexConnected(void);
void pcsSetMexPeerAddr(const char *host, const char *port);
void pcsAllocPassiveMexSocket(const char *subnet, const char *port);
void pcsScktProcessMex(void);
void pcsSendMex(char *buf, int buflen);
int pcsRecvdMexMsgLen(void);
char *pcsRecvdMexMsg(void);

#endif // !defined(AFX_PCSSOCKET_H__1C56E603_F1C8_11D2_B2BE_00104B93918B__INCLUDED_)

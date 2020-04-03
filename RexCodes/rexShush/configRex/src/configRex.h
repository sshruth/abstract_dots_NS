/* Header "configRex.h" for configRex Application */
#include <sys/stat.h>
#define DT2821 	    1
#define NIATMIO	    2
#define NIATMIOX	3
#define AC5508HR	4
#define AC5525MF	5
#define ANLSDAS16	6
#define ANDAS12	7
#define PCI_AtoD    8
#define ISA_DIO     9
#define PCI_DIO    10
#define ISA_DtoA   11
#define PCI_DtoA   12
#define ISA_CT     13
#define PCI_CT     14

#define LATCH_UNIT  1
#define MEXWEX_UNIT	2
#define PLEXON_UNIT	3
#define FAKE_UNIT 	4

#define PCMSG 	1
#define SOCKET	2

#define NO		0
#define YES		1
#define NDEF    2

char *rexlinetok(char *);

extern char *cnfHeader;
extern int a2dboard;
extern int a2dvect;
extern int a2dbase;
extern int d2aboard;
extern int d2abase;
extern int dioboard;
extern int diobase;
extern int ctboard;
extern int unit;
extern int pcmessageSwitch;
extern int socketSwitch;
extern int hardwareSwitch;
extern int smiSwitch;
extern int plexEcodeSwitch;

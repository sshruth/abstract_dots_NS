
/* ../abmain.c */

/* ../eyeWindow.c */
int initWinOptionsDialog ( PtWidget_t *link_instance , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int windowScale ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int refreshRate ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int refreshTrig ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int refreshTrigLevel ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int autoRefresh ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int winRefresh ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int autoInterval ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int initDataChan ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int dataChan ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int windowGrid ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int cursorChoice ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
void setOSContainer ( void );

/* ../init.c */
int init ( PtWidget_t *link_instance , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
void writeRoot ( void );
void readRoot ( char *root );

/* ../initArgs.c */
int initArgs ( int argc , char *argv []);

/* ../manageWin.c */
int resizeWin ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int manageWin ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int exitEwind ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );

/* ../ph_ewind.c */
void dispWindDraw ( PtWidget_t *widget , PhTile_t *damage );
void dispWindExtent ( PtWidget_t *widget );
void calcCursPos ( int nData );
void calcEyeScale ( int scale );
void changeScales ( int magIndex );
void calcWndSiz ( int i , PhDim_t siz );
void drawPlus ( PhPoint_t *cntr );
void drawX ( PhPoint_t *cntr );
void drawStar ( PhPoint_t *cntr );

/* ../timer.c */
int windTimer ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
void updateWinSizes ( void );
void updateEyeOffsets ( void );
void updateLabels ( void );

/* ../vexControl.c */
int initPcsSocket ( PtWidget_t *link_instance , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int getVexName ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int connectVex ( PtWidget_t *link_instance , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int checkVexConnect ( PtWidget_t *link_instance , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int closeSocket ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int hideDialog ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int vexMouse ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int keyboard ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int character ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int initVexDebug ( PtWidget_t *link_instance , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int closeDebugDialog ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
void sendVexString ( char *string );
void sendVexFuncKey ( int key );
void sendVexMouse ( int action );
void sendVexMotion ( PhPoint_t loc );
void vexPrint ( char *text );
void printHelp ( void );
int listen2Vex ( void *data );

/* ../PtScale.c */
void scale_set_labels ( PtWidget_t *widget , PtArg_t const *argt );
int scale_get_labels ( PtWidget_t *widget , PtArg_t *argt );
PtWidgetClass_t *CreateScaleClass ( void );

/* ../rex_main.c */
void rex_main ( void );

/* ../rex_alert.c */
int alert ( int sig , void *data );
void oncntrlb ( int sig );

/* ../pcsSocket.c */
void pcsSocket ( void );
void pcsClose ( void );
int pcsAllocPassiveSocket ( const char *subnet , const char *port );
void pcsScktProcess ( void );
char *pcsRecvdMsg ( void );
int pcsRecvdMsgLen ( void );
void pcsSend ( char *buf , int buflen );
int pcsSetPeerAddr ( const char *host , const char *port );
SOCKET passiveUDP ( const char *subnet , const char *port );
SOCKET passiveSock ( const char *subnet , const char *port , const char *transport , int qlen );
int recvfromPackets ( char *buf , int bufLength );
int sendtoPackets ( char *buf , int bufLength );

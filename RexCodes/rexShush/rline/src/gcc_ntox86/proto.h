
/* ../abmain.c */

/* ../init.c */
int init ( PtWidget_t *link_instance , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
void readRoot ( char *buf );

/* ../initArgs.c */
int initArgs ( int argc , char *argv []);

/* ../manageWin.c */
int resizeWin ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int manageWin ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int exitRline ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );

/* ../ph_rline.c */
void runLineDraw ( PtWidget_t *widget , PhTile_t *damage );
void runLineExtent ( PtWidget_t *widget );
void scaleData ( int nData );

/* ../runLine.c */
int signalScale ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int signalWrap ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int runBarOff ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int rlineTrig ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int rlTrigLevel ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int rlineDisplay ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int rlineMode ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int rlDisplayReset ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int runDispRate ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int initRunChan ( PtWidget_t *link_instance , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int runChanSig ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int runChanScale ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int runChanOffset ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int signalChoice ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );

/* ../timer.c */
int rlineTimer ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
void updateLabels ( void );

/* ../rex_main.c */
void rex_main ( void );

/* ../rex_alert.c */
int alert ( int sig , void *data );
void oncntrlb ( int sig );

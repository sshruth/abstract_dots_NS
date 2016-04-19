
/* ../abmain.c */

/* ../initArgs.c */
int initArgs ( int argc , char *argv []);
void initTig ( void );

/* ../initXcorr.c */
int initXcorr ( PtWidget_t *link_instance , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
PtWidget_t *inflateToggleBalloon ( PtWidget_t *window , PtWidget_t *widget , int position , char *string , char *font , PgColor_t fill , PgColor_t text );
void readRoot ( char *buf );

/* ../lsrTig.c */
void setTigList ( char *buffer );
void setTigListLength ( long length );
char getTigChar ( void );
short getTigWord ( void );
void wait_ff ( void );
void setTigListEnd ( void );
void getPointXY ( void );
void calcScale ( void );
void tigExtent ( PtWidget_t *widget );
void tigDraw ( PtWidget_t *widget , PhTile_t *damage );
void resetData ( void );
void loadData ( int nCorrUnits , long *corrUnitTimes );
void drawXcorr ( void );
void labelPage ( void );
int setTrialVSpace ( int *count , int ysiz , double *trialLineHeight );
void initTigCell ( TIGSPACE *dataSpace );
void drawTigAxes ( TIGSPACE *dataSpace );
void drawTigXaxis ( TIGSPACE *dataSpace );
void drawRasterAlign ( TIGSPACE *dataSpace );
void plotSpkRas ( int **spikeTrains , int nTrials , int maxLines , int tikht , double lineHeight , TIGSPACE *dataSpace );
void plotSpkDen ( float *pspkDenAvg , TIGSPACE *dataSpace );

/* ../manageWin.c */
int resizeWin ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int manageWin ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int exposeWin ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int exitXcorr ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );

/* ../setData.c */
int switchSampling ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int clearData ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int setGain ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int setTrigUnit ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int setCorrelatedUnit ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int setPreTime ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int setInterval ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int setSigma ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int getData ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );

/* ../rex_main.c */
void rex_main ( void );

/* ../rex_alert.c */
int alert ( int sig , void *data );
void oncntrlb ( int sig );

/* ../timerFunction.c */
void timerFunction ( int number );
void clearTimerFunction ( void );
void initThisTimerHisto ( int index , char *label , int bins , int width );
void startTimer ( int index );
void stopTimer ( int index );
void printTimes ( void );

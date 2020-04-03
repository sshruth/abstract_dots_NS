
/* ../abmain.c */

/* ../fileTools.c */
int initRootSave ( PtWidget_t *link_instance , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int initRootRead ( PtWidget_t *link_instance , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int initRasterSave ( PtWidget_t *link_instance , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int setDir ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int getFileName ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int openFile ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
void writeRoot ( char *fileName );
void readRoot ( char *fileName );
void printSelect ( SELECT *select , FILE *outFile );
void saveRaster ( char *fileName );

/* ../inRex.c */
int startRexData ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int stopRexData ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int loadRexTrial ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int reInitRasters ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int selectTrials ( RexInfo *ri , int trialNum );
int loadDataSetStructure ( RexInfo *ri , TRIALS *thisTrial , int trialNum );
int countClasses ( void );
int iallRas ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );

/* ../initArgs.c */
int initArgs ( int argc , char *argv []);
void initSelections ( SELECTIONS *selections );
SELECTIONS *getSelections ( void );
SELECTSET *addSelectSet ( SELECTSET *selectSets , int n );
SELECTSET *getSelectSet ( int indx );
TRIALS *getTrialData ( int indx );
void initRasters ( RASTER *rasters );
void initTig ( void );
RASTER *getRasters ( void );
PAGE *addRasterPage ( PAGE *pages , int n );
PAGE *getRasterPage ( int indx );
CELL *getRasterCell ( CELL *cellsInfo , int indx );
SIGNAL *getRasterSignal ( SIGNAL *sigInfo , int indx );

/* ../initRasterDemo.c */
int initRasterDemo ( PtWidget_t *link_instance , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );

/* ../lsrTig.c */
void tigDraw ( PtWidget_t *widget , PhTile_t *damage );
void setTigList ( char *buffer );
void setTigListLength ( long length );
char getTigChar ( void );
short getTigWord ( void );
void wait_ff ( void );
void setTigListEnd ( void );
void getPointXY ( void );
void calcScale ( void );
void setTile ( PhRect_t *tilep );
void clearTiles ( void );
void tigClear ( void );
void tigExtent ( PtWidget_t *widget );

/* ../manageWin.c */
int resizeWin ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int manageWin ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int exposeWin ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int exitRas ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );

/* ../tigControls.c */
int nextPageProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int priorPageProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );

/* ../toolGraph.c */
void initGraphWidgets ( PtWidget_t *thisModule , GRAPH *thisGraph );
int graphStartProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int graphDurationProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int graphRateProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int graphTicProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int graphYaxisProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int graphNumberProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int graphAxisFormatProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int graphXaxisLabelTextProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int graphYaxisLabelTextProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int graphAxisFontProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int graphSizeProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int globalStartProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int globalDurationProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int globalRateProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int globalTicProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int globalGraphYaxisProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int globalGraphNumberProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int globalGraphAxisFormatProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int globalGraphAxisFontProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int globalGraphSizeProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );

/* ../toolPage.c */
int buildPage ( PtWidget_t *link_instance , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int newPage ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int showRasterPage ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int resetButtons ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int rowColProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int pageLabelTextProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int pageLabelFontProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int pageLabelPositionProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int pageLabelSizeProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int cellButtonProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int clearRasters ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
void initFirstPage ( PAGE *thisPage );
void initSubsequentPage ( PAGE *thisPage , PAGE *lastPage );
void copyPlot ( PLOT *thisPlot , PLOT *lastPlot );
void initPlot ( PAGE *thisPage , PLOT *thisPlot , PLOT *lastPlot );
void initPageWidgets ( PtWidget_t *link_instance , PAGE *thisPage , int newRasters );
void initCellButtons ( PtWidget_t *thisModule , PAGE *thisPage , int newRasters );
int getCellButtonName ( int index );

/* ../toolPlot.c */
int cellSetProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int cellEcodeProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int cellSigmaProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int cellRasterProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int cellDensityProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int cellUnitsProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int plotMarkEcodeProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int plotMarkSymbolProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int plotMarkSizeProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int plotLabelTextProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int plotLabelFontProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int plotLabelSizeProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int plotLabelPositionProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
void initPlotWidgets ( PtWidget_t *thisModule , PLOT *thisPlot );
void setButtonLabel ( PtWidget_t *thisModule , CELL *thisCell );
char decodeSymbolString ( char *symbolString );
int globalCellUnitsProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int globalCellRasterProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int globalCellDensityProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int globalPlotMarkEcodeProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int globalPlotMarkSymbolProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int globalPlotMarkSizeProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int globalPlotLabelFontProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int globalPlotLabelPositionProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int globalPlotLabelSizeProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );

/* ../toolSelect.c */
int buildSelect ( PtWidget_t *link_instance , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int newSelectSet ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int showSelectSet ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int startCodeProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int preTimeProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int alignCodeProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int alignOffsetProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int allOfProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int nonOfProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int anyOfProc ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int clearSelections ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
void initFirstSet ( SELECTSET *thisSet );
void initSubsequentSet ( SELECTSET *thisSet , SELECTSET *lastSet );
void initSetWidgets ( PtWidget_t *link_instance , SELECTSET *thisSet , int newSelects );

/* ../utilDex.c */
void copyRexInfo ( DexInfo *di , RexInfo *ri );
void copyRexEvents ( EVENT *dev , EVENT *sev , int n );
void copyRexUnits ( RexUnits *du , RexUnits *su , int nUnits );
int trialSort ( TRIALS *dataArray , int n );
int compareTrials ( TRIALS *element1 , TRIALS *element2 );
char *getDexUnitLabel ( DexInfo *di , int unitIndex );
int getDexUnitIndex ( DexInfo *di , char *unitLabel );
int getDexUnitCode ( DexInfo *di , int unitIndx );
long alignOnEcode ( DexInfo *di , long eventTime , int thisCode );
int getDexEcodeTime ( DexInfo *di , int ecode );
int getDexEcodeRelTime ( DexInfo *di , float relAlgnTime , int ecode );
void getDexSigNumbs ( SIGNAL *sigInfo , int nSignals );

/* ../viewRasters.c */
void drawRasters ( void );
int getSpikeDensity ( CLASSES *pci , TRIALS *pti , int start , int end , int unitCode , double sigma );
int getSpikeTimes ( CLASSES *pci , TRIALS *pti , int unitCode , int start , int interval );
void copySpikeDensityFunction ( SD_EST *spsd , SD_EST *dpsd , int nTrials , int interval );
int getMarkTimes ( TRIALS *dataSet , int nTrials , int rMark , int **markTrains , int *markCounts , int start , int interval );

/* ../initRasters.c */
void labelPage ( int pageWidth , int pageHeight , LABEL *thisLabel );
void initTigCell ( TIGSPACE *dataSpace , PLOT *thisPlot , int cellWidth , int cellHeight );
void labelCell ( TIGSPACE *dataSpace , LABEL *thisLabel , int cellWidth , int cellHeight );
void drawTigAxes ( TIGSPACE *dataSpace , GRAPH *graph );
void drawTigXaxis ( TIGSPACE *dataSpace , GRAPH *graph );
void drawTigYaxis ( TIGSPACE *dataSpace , GRAPH *graph );
void drawRasterAlign ( TIGSPACE *dataSpace , GRAPH *graph );
int setTrialVSpace ( int *count , int ysiz , double *trialLineHeight );

/* ../drawRasterCell.c */
void plotSpkRas ( int **spikeTrains , int *spikeCounts , int nTrials , int maxLines , int color , int tikht , double lineHeight , GRAPH *graph , TIGSPACE *dataSpace );
void plotSpkDen ( float *psd_avg , int color , GRAPH *graph , TIGSPACE *dataSpace );
void plotEcodeMarks ( int **ecodeTrains , int *ecodeCounts , int nTrials , int maxLines , char symbol , int size , double lineHeight , GRAPH *graph , TIGSPACE *dataSpace );

/* ../rex_main.c */
void rex_main ( void );

/* ../rex_alert.c */
int alert ( int sig , void *data );
void oncntrlb ( int sig );

/* ../rex_ebuf.c */
void rexSplitEvents ( void );
int rexFileOpen ( int rexmaxSampRate , int rexstartCode , int rexpreTime );
void rexFileClose ( void );
RexInfo *rexGetEvents ( int trialNum );
PRIVATE void *rexAlloc ( void *ptr , long size , long number , char *err_msg );
PRIVATE void percolate ( register EVENT *buf , register EVENT *a , register EVENT *b );
PRIVATE void esort ( EVENT *buf , int n );
PRIVATE int loadEbuf ( EVENT *ev );
PRIVATE void loadUbuf ( int code , int time );
PRIVATE void rexloadbufs ( EVENT_P sEp , EVENT_P eEp );

/* Import (extern) header for application - AppBuilder 2.03  */

#include "abdefine.h"

extern ApWindowLink_t base;
extern ApWindowLink_t classesSelect;
extern ApWindowLink_t rastersPage;
extern ApDialogLink_t fileSelectorDialog;
extern ApWindowLink_t globalSettings;
extern ApWidget_t AbWidgets[ 172 ];

extern ApMenuLink_t filesMenu;
extern ApMenuLink_t classesMenu;
extern ApMenuLink_t rastersMenu;

#ifdef __cplusplus
extern "C" {
#endif
int initArgs( int argc, char **argv );
int initRootSave( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int initRasterDemo( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int buildSelect( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int startCodeProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int alignCodeProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int alignOffsetProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int allOfProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int nonOfProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int anyOfProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int cellButtonProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int rowColProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int pageLabelPositionProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int pageLabelTextProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int pageLabelSizeProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int graphStartProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int graphDurationProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int graphRateProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int graphTicProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int graphYaxisProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int graphNumberProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int graphAxisFormatProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int pageLabelFontProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int graphSizeProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int graphXaxisLabelTextProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int graphYaxisLabelTextProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int graphAxisFontProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int resetButtons( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int cellSetProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int cellEcodeProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int cellRasterProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int cellDensityProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int cellUnitsProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int plotMarkSymbolProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int plotMarkSizeProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int plotLabelTextProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int plotLabelFontProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int plotLabelSizeProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int plotLabelPositionProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int plotMarkEcodeProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int cellSigmaProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int initRootRead( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int clearSelections( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int clearRasters( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int loadRexTrial( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int preTimeProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int showRasterPage( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int buildPage( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int newPage( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int showSelectSet( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int newSelectSet( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int startRexData( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int stopRexData( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int iallRas( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int priorPageProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int nextPageProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int setDir( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int getFileName( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int openFile( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int manageWin( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int initRasterSave( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
void tigDraw( PtWidget_t *widget, PhTile_t *damage ) 

;
void tigExtent( PtWidget_t *widget ) 

;
int exposeWin( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int globalDurationProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int globalRateProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int globalTicProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int globalStartProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int globalCellUnitsProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int globalCellRasterProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int globalCellDensityProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int globalPlotMarkEcodeProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int globalPlotMarkSymbolProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int globalPlotMarkSizeProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int globalPlotLabelFontProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int globalPlotLabelPositionProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int globalGraphYaxisProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int globalGraphNumberProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int globalGraphAxisFormatProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int globalGraphAxisFontProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int globalPlotLabelSizeProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int globalGraphSizeProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int resizeWin( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int exitRas( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
#ifdef __cplusplus
}
#endif

/* Import (extern) header for application - AppBuilder 2.03  */

#include "abdefine.h"

extern ApEventLink_t AbInternalLinks[];

extern ApWindowLink_t base;
extern ApDialogLink_t processSelectorDialog;
extern ApDialogLink_t rootSelectorDialog;
extern ApDialogLink_t dataSelectorDialog;
extern ApDialogLink_t rexErrorDialog;
extern ApWidget_t AbWidgets[ 30 ];

extern ApMenuLink_t displayMenu;
extern ApMenuLink_t filesMenu;

#ifdef __cplusplus
extern "C" {
#endif
int initArgs( int argc, char **argv );
int init( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int setProcDir( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int getProcFileName( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int runProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int loadProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int setRootDir( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int getRootFileName( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int writeAllRoots( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int writeSysUserRoots( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int writeSysRoots( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int writeUserRoots( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int writeStateRoots( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int readRoots( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int setDataDir( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int getDataFileName( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int openData( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int exitRex( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int startWindow( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int startRaster( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int startRunLine( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int waitProc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int onCntrlB( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int initProcSelDialog( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int initDataSelDialog( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int closeDataFile( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int initRootSelDialog( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int closeErrorDialog( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int startXcorr( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int initRexError( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int switchData( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
#ifdef __cplusplus
}
#endif

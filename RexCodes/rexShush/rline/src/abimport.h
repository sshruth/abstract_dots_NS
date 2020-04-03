/* Import (extern) header for application - AppBuilder 2.03  */

#include "abdefine.h"

extern ApWindowLink_t base;
extern ApDialogLink_t runLineDialog;
extern ApWidget_t AbWidgets[ 42 ];


#ifdef __cplusplus
extern "C" {
#endif
int initArgs( int argc, char **argv );
int init( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int rlineTimer( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int runBarOff( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int initRunChan( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int runChanSig( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int runChanScale( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int runChanOffset( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int runDispRate( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int manageWin( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int resizeWin( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int rlineTrig( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int rlineDisplay( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int rlTrigLevel( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int rlDisplayReset( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
void runLineDraw( PtWidget_t *widget, PhTile_t *damage ) 

;
void runLineExtent( PtWidget_t *widget ) 

;
int rlineMode( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int signalChoice( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int signalScale( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int signalWrap( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int exitRline( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
#ifdef __cplusplus
}
#endif

/* Import (extern) header for application - AppBuilder 2.03  */

#include "abdefine.h"

extern ApEventLink_t AbInternalLinks[];

extern ApWindowLink_t base;
extern ApDialogLink_t windowOptionsDialog;
extern ApDialogLink_t vexName;
extern ApDialogLink_t vexDebugDialog;
extern ApWidget_t AbWidgets[ 61 ];

extern ApMenuLink_t ewinMenu;

#ifdef __cplusplus
extern "C" {
#endif
int initArgs( int argc, char **argv );
int init( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int windTimer( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int manageWin( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int windowScale( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int resizeWin( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int refreshRate( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int refreshTrig( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int refreshTrigLevel( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int autoRefresh( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int winRefresh( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int cursorChoice( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int autoInterval( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int initDataChan( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int dataChan( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
void dispWindDraw( PtWidget_t *widget, PhTile_t *damage ) 

;
void dispWindExtent( PtWidget_t *widget ) 

;
int initWinOptionsDialog( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int initPcsSocket( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int connectVex( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int hideDialog( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int getVexName( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int keyboard( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int closeSocket( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int vexMouse( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int windowGrid( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int character( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int initVexDebug( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int closeDebugDialog( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int exitEwind( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
#ifdef __cplusplus
}
#endif

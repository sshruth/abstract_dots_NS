/* Import (extern) header for application - AppBuilder 2.03  */

#include "abdefine.h"

extern ApWindowLink_t base;
extern ApWidget_t AbWidgets[ 12 ];


#ifdef __cplusplus
extern "C" {
#endif
int initArgs( int argc, char **argv );
int initXcorr( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
void tigDraw( PtWidget_t *widget, PhTile_t *damage ) 

;
void tigExtent( PtWidget_t *widget ) 

;
int manageWin( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int exposeWin( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int switchSampling( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int clearData( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int setSigma( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int getData( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int setTrigUnit( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int setCorrelatedUnit( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int setPreTime( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int setInterval( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int setGain( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int resizeWin( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int exitXcorr( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
#ifdef __cplusplus
}
#endif

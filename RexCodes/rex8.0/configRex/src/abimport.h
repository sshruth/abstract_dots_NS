/* Import (extern) header for application - AppBuilder 2.01  */

#include "abdefine.h"

extern ApWindowLink_t base;
extern ApWidget_t AbWidgets[ 38 ];


#ifdef __cplusplus
extern "C" {
#endif
int initArgs( int argc, char **argv );
int initDialog( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int atodChoice( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int writeCnfFile( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int vectChoice( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int unitChoice( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int pstopSwitchChoice( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int messageChoice( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int basePortChoice( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int plexEcodeChoice( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int atodChoice( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int smiChoice( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int smiChoice( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int dioChoice( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int dioChoice( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int timerChoice( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int timerChoice( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int d2aChoice( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int d2aChoice( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
#ifdef __cplusplus
}
#endif

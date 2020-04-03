/* Import (extern) header for application - AppBuilder 2.03  */

#if defined(__cplusplus)
extern "C" {
#endif

#include "abdefine.h"

extern ApEventLink_t AbInternalLinks[];

extern ApWindowLink_t rebelBaseWindow;
extern ApWindowLink_t rebelWindow;
extern ApPictureLink_t rebelPictures;
extern ApDialogLink_t rebelOpenSelector;
extern ApDialogLink_t rebelSaveAsSelector;
extern ApDialogLink_t rebelResourceDialog;
extern ApDialogLink_t rebelWidgetDialog;
extern ApDialogLink_t rebelActionDialog;
extern ApDialogLink_t rebelGlobalsDialog;
extern ApDialogLink_t rebelEscapeDialog;
extern ApDialogLink_t rebelAbortDialog;
extern ApDialogLink_t rebelLabActionsDialog;
extern ApWidget_t AbWidgets[ 144 ];

extern ApMenuLink_t rebelFileMenu;
extern ApMenuLink_t rebelEditMenu;
extern ApMenuLink_t rebelAppMenu;
extern ApMenuLink_t rebelViewMenu;

#if defined(__cplusplus)
}
#endif


#ifdef __cplusplus
int initArgs( int argc, char **argv );
int initGui( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int resize( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
#endif

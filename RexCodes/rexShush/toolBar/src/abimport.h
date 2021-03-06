/* Import (extern) header for application - AppBuilder 2.03  */

#include "abdefine.h"

extern ApEventLink_t AbInternalLinks[];

extern ApWindowLink_t base;
extern ApDialogLink_t signalDialog;
extern ApDialogLink_t sigSumDialog;
extern ApDialogLink_t eyeWinSumDialog;
extern ApDialogLink_t stateListDialog;
extern ApDialogLink_t eyeWinDialog;
extern ApDialogLink_t processDialog;
extern ApDialogLink_t rampDialog;
extern ApDialogLink_t sacDecDialog;
extern ApDialogLink_t daConvDialog;
extern ApDialogLink_t daSumDialog;
extern ApDialogLink_t memArrayDialog;
extern ApDialogLink_t memArraySumDialog;
extern ApDialogLink_t eyeOffDialog;
extern ApDialogLink_t bitsDialog;
extern ApPictureLink_t sigSumPictures;
extern ApPictureLink_t eyeWinSumPictures;
extern ApPictureLink_t daSumPictures;
extern ApPictureLink_t memArraySumPictures;
extern ApDialogLink_t rampSumDialog;
extern ApPictureLink_t rampSumPictures;
extern ApPictureLink_t userMenuPictures;
extern ApDialogLink_t userFunctionDialog;
extern ApPictureLink_t userFunctionPictures;
extern ApWindowLink_t userMenuWindow;
extern ApDialogLink_t smiDialog;
extern ApPictureLink_t stateDebugPictures;
extern ApDialogLink_t userRtVarsDialog;
extern ApPictureLink_t userRtVarsPictures;
extern ApDialogLink_t stateDebugDialog;
extern ApDialogLink_t stateDebugWindow;
extern ApDialogLink_t rexMessageDialog;
extern ApDialogLink_t scoreAlertDialog;
extern ApWidget_t AbWidgets[ 616 ];

extern ApMenuLink_t statesMenu;
extern ApMenuLink_t controlsMenu;
extern ApMenuLink_t signalsMenu;
extern ApMenuLink_t eyeWindowMenu;
extern ApMenuLink_t userMenu;

#ifdef __cplusplus
extern "C" {
#endif
int initArgs( int argc, char **argv );
int initUserMenus( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int init( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int initAllStates( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int initTimedStates( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int switchClock( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int switchParadigm( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int switchWindows( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int initProcessDialog( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int initRamps( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int initDADialog( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int initDASummary( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int initMemArrayDialog( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int initMemArraySummary( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int setSigType( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int initSignalDialog( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int initEyeWinDialog( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int eyeWinMenuOverRide( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int eyeWinSourceName( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int setSigName( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int setADrates( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int setADcal( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int setADchannel( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int setADdelay( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int setStateTime( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int setStateRunLineVal( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int rampParameters( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int processADgain( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int processADrate( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int processDebugState( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int processPreTime( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int processPostTime( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int processStateRate( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int processMenuOvrRide( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int sacDecParams( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int initRampSum( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int resetStates( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int initSigSummary( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int initEyeWinSum( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int toggleBit( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int bitsIODevice( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int bitsStartPortNumber( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int setBits( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int setPulseMode( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int setToggleMode( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int resetBits( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int initSacDec( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int initBitsDialog( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int rampDialogSet( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int rampAnchor( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int setStateActionArg( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int setStateActionArgFormat( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int setStateRunLineFlag( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int processUnitControl( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int initEyeOffDialog( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int eyeOffMenuOverRide( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int eyeOff( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int eyeWinDialogSet( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int eyeOffDialogSet( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int rampMenuOverRide( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int setDASource( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int daMenuOverRide( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int setDAParameters( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int daConvDialogSet( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int memArrayDialogSet( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int memArrayMenuOverRide( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int memArrayParameters( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int initStateList( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int callUserFunction( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int manageWin( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int initUserMenuWindow( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int eyeWinSwitch( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int eyeWinDirPos( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int eyeWinDirSiz( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int eyeWinSourceNum( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int setUserMenuVal( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int closeMenuWindow( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int setBatchUserMenuVal( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int initUserFunctionDialog( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int sacDecSet( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int rampPreTime( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int smiOffset( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int smiGain( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int smiOverrun( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int smiDebtog( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int smiDebtog2( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int smiDeblval( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int smiDebhval( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int smiV0( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int smiV1( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int smiV2( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int smiV3( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int smiV4( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int smiV5( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int smiV6( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int smiV7( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int smiV8( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int smiV9( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int smiLater( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int initSmi( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int switchDebug( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int swapColors( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int initUserRtVarsDialog( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int updateVars( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int resetVars( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int closeDebugWindow( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int closeDebugDialog( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int eyeWndChkRate( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int initRexMessage( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int closeMessageDialog( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int setSigTitle( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int printDebug( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int notifyUserMenuVal( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int notifyBatchUserMenuVal( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int notifyUserFunction( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int setDAMode( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int monkeyAlert( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int cancelAlert( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int initScoreAlertDialog( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int exitSpot( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int initStatePanel( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int initDebugPanel( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int showStateDebugDialog( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int initStateDebugWindow( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
#ifdef __cplusplus
}
#endif

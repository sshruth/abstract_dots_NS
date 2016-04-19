/* Y o u r   D e s c r i p t i o n                       */
/*                            AppBuilder Photon Code Lib */
/*                                         Version 1.14C */

/* Standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

/* Toolkit headers */
#include <Ph.h>
#include <Pt.h>
#include <Ap.h>

/* Local headers */
#include "ph_toolBar.h"
#include "abimport.h"
#include "proto.h"

/* Application Options string */
const char ApOptions[] =
	AB_OPTIONS ""; /* Add your options in the "" */

PgColor_t stateButtonColors[3];

int portLabels[6];
int portBits[6][8];
int portControls[6][4];
int stateArguments[20][3];
int eyeWinSrcNames[4][6];
int actEyeWinSrcNames[4];
int sigTypeToggles[4];
int adVarToggles[10];
int memSrcToggles[12];
int computedToggles[12];
int aqrRtToggles[4];
int strRtToggles[4];
int adCalToggles[7];
int daSourceToggles[9];
char toolBarTitle[128];
int clockSwitched;
int clockState;

int initArgs( int argc, char *argv[] )
{
	int i;

	/* eliminate 'unreferenced' warnings */
	argc = argc, argv = argv;

	/* get the title for the toolbar from the argument list */
	strncpy(toolBarTitle, argv[0], 128);

	/* set up the rex interface */
	rex_main();

	/* initialize the debug colors of the state buttons */
	stateButtonColors[0] = PgRGB(127, 252, 254);
	stateButtonColors[1] = PgRGB(254, 254, 127);
	stateButtonColors[2] = PgRGB(239, 189, 255);

	/* initialize widget arrays */
	portLabels[0] = ABN_bitsPort0Label;
	portLabels[1] = ABN_bitsPort1Label;
	portLabels[2] = ABN_bitsPort2Label;
	portLabels[3] = ABN_bitsPort4Label;
	portLabels[4] = ABN_bitsPort5Label;
	portLabels[5] = ABN_bitsPort6Label;

	portBits[0][0] = ABN_bitsP0B0Toggle;
	portBits[0][1] = ABN_bitsP0B1Toggle;
	portBits[0][2] = ABN_bitsP0B2Toggle;
	portBits[0][3] = ABN_bitsP0B3Toggle;
	portBits[0][4] = ABN_bitsP0B4Toggle;
	portBits[0][5] = ABN_bitsP0B5Toggle;
	portBits[0][6] = ABN_bitsP0B6Toggle;
	portBits[0][7] = ABN_bitsP0B7Toggle;

	portBits[1][0] = ABN_bitsP1B0Toggle;
	portBits[1][1] = ABN_bitsP1B1Toggle;
	portBits[1][2] = ABN_bitsP1B2Toggle;
	portBits[1][3] = ABN_bitsP1B3Toggle;
	portBits[1][4] = ABN_bitsP1B4Toggle;
	portBits[1][5] = ABN_bitsP1B5Toggle;
	portBits[1][6] = ABN_bitsP1B6Toggle;
	portBits[1][7] = ABN_bitsP1B7Toggle;

	portBits[2][0] = ABN_bitsP2B0Toggle;
	portBits[2][1] = ABN_bitsP2B1Toggle;
	portBits[2][2] = ABN_bitsP2B2Toggle;
	portBits[2][3] = ABN_bitsP2B3Toggle;
	portBits[2][4] = ABN_bitsP2B4Toggle;
	portBits[2][5] = ABN_bitsP2B5Toggle;
	portBits[2][6] = ABN_bitsP2B6Toggle;
	portBits[2][7] = ABN_bitsP2B7Toggle;

	portBits[3][0] = ABN_bitsP4B0Toggle;
	portBits[3][1] = ABN_bitsP4B1Toggle;
	portBits[3][2] = ABN_bitsP4B2Toggle;
	portBits[3][3] = ABN_bitsP4B3Toggle;
	portBits[3][4] = ABN_bitsP4B4Toggle;
	portBits[3][5] = ABN_bitsP4B5Toggle;
	portBits[3][6] = ABN_bitsP4B6Toggle;
	portBits[3][7] = ABN_bitsP4B7Toggle;

	portBits[4][0] = ABN_bitsP5B0Toggle;
	portBits[4][1] = ABN_bitsP5B1Toggle;
	portBits[4][2] = ABN_bitsP5B2Toggle;
	portBits[4][3] = ABN_bitsP5B3Toggle;
	portBits[4][4] = ABN_bitsP5B4Toggle;
	portBits[4][5] = ABN_bitsP5B5Toggle;
	portBits[4][6] = ABN_bitsP5B6Toggle;
	portBits[4][7] = ABN_bitsP5B7Toggle;

	portBits[5][0] = ABN_bitsP6B0Toggle;
	portBits[5][1] = ABN_bitsP6B1Toggle;
	portBits[5][2] = ABN_bitsP6B2Toggle;
	portBits[5][3] = ABN_bitsP6B3Toggle;
	portBits[5][4] = ABN_bitsP6B4Toggle;
	portBits[5][5] = ABN_bitsP6B5Toggle;
	portBits[5][6] = ABN_bitsP6B6Toggle;
	portBits[5][7] = ABN_bitsP6B7Toggle;

	portControls[0][0] = ABN_bitsPort0ToggleToggle;
	portControls[0][1] = ABN_bitsPort0PulseToggle;
	portControls[0][2] = ABN_bitsPort0SetToggle;
	portControls[0][3] = ABN_bitsPort0ResetToggle;

	portControls[1][0] = ABN_bitsPort1ToggleToggle;
	portControls[1][1] = ABN_bitsPort1PulseToggle;
	portControls[1][2] = ABN_bitsPort1SetToggle;
	portControls[1][3] = ABN_bitsPort1ResetToggle;

	portControls[2][0] = ABN_bitsPort2ToggleToggle;
	portControls[2][1] = ABN_bitsPort2PulseToggle;
	portControls[2][2] = ABN_bitsPort2SetToggle;
	portControls[2][3] = ABN_bitsPort2ResetToggle;

	portControls[3][0] = ABN_bitsPort4ToggleToggle;
	portControls[3][1] = ABN_bitsPort4PulseToggle;
	portControls[3][2] = ABN_bitsPort4SetToggle;
	portControls[3][3] = ABN_bitsPort4ResetToggle;

	portControls[4][0] = ABN_bitsPort5ToggleToggle;
	portControls[4][1] = ABN_bitsPort5PulseToggle;
	portControls[4][2] = ABN_bitsPort5SetToggle;
	portControls[4][3] = ABN_bitsPort5ResetToggle;

	portControls[5][0] = ABN_bitsPort6ToggleToggle;
	portControls[5][1] = ABN_bitsPort6PulseToggle;
	portControls[5][2] = ABN_bitsPort6SetToggle;
	portControls[5][3] = ABN_bitsPort6ResetToggle;
	
	stateArguments[0][0] = ABN_stateArg1DecToggle;
	stateArguments[0][1] = ABN_stateArg1HexToggle;
	stateArguments[0][2] = ABN_stateArg1Val;

	stateArguments[1][0] = ABN_stateArg2DecToggle;
	stateArguments[1][1] = ABN_stateArg2HexToggle;
	stateArguments[1][2] = ABN_stateArg2Val;

	stateArguments[2][0] = ABN_stateArg3DecToggle;
	stateArguments[2][1] = ABN_stateArg3HexToggle;
	stateArguments[2][2] = ABN_stateArg3Val;

	stateArguments[3][0] = ABN_stateArg4DecToggle;
	stateArguments[3][1] = ABN_stateArg4HexToggle;
	stateArguments[3][2] = ABN_stateArg4Val;

	stateArguments[4][0] = ABN_stateArg5DecToggle;
	stateArguments[4][1] = ABN_stateArg5HexToggle;
	stateArguments[4][2] = ABN_stateArg5Val;

	stateArguments[5][0] = ABN_stateArg6DecToggle;
	stateArguments[5][1] = ABN_stateArg6HexToggle;
	stateArguments[5][2] = ABN_stateArg6Val;

	stateArguments[6][0] = ABN_stateArg7DecToggle;
	stateArguments[6][1] = ABN_stateArg7HexToggle;
	stateArguments[6][2] = ABN_stateArg7Val;

	stateArguments[7][0] = ABN_stateArg8DecToggle;
	stateArguments[7][1] = ABN_stateArg8HexToggle;
	stateArguments[7][2] = ABN_stateArg8Val;

	stateArguments[8][0] = ABN_stateArg9DecToggle;
	stateArguments[8][1] = ABN_stateArg9HexToggle;
	stateArguments[8][2] = ABN_stateArg9Val;

	stateArguments[9][0] = ABN_stateArg10DecToggle;
	stateArguments[9][1] = ABN_stateArg10HexToggle;
	stateArguments[9][2] = ABN_stateArg10;

	stateArguments[10][0] = ABN_stateArg1DecFormat;
	stateArguments[10][1] = ABN_stateArg1HexFormat;
	stateArguments[10][2] = ABN_stateArg1;

	stateArguments[11][0] = ABN_stateArg2DecFormat;
	stateArguments[11][1] = ABN_stateArg2HexFormat;
	stateArguments[11][2] = ABN_stateArg2;

	stateArguments[12][0] = ABN_stateArg3DecFormat;
	stateArguments[12][1] = ABN_stateArg3HexFormat;
	stateArguments[12][2] = ABN_stateArg3;

	stateArguments[13][0] = ABN_stateArg4DecFormat;
	stateArguments[13][1] = ABN_stateArg4HexFormat;
	stateArguments[13][2] = ABN_stateArg4;

	stateArguments[14][0] = ABN_stateArg5DecFormat;
	stateArguments[14][1] = ABN_stateArg5HexFormat;
	stateArguments[14][2] = ABN_stateArg5;

	stateArguments[15][0] = ABN_stateArg6DecFormat;
	stateArguments[15][1] = ABN_stateArg6HexFormat;
	stateArguments[15][2] = ABN_stateArg6;

	stateArguments[16][0] = ABN_stateArg7DecFormat;
	stateArguments[16][1] = ABN_stateArg7HexFormat;
	stateArguments[16][2] = ABN_stateArg7;

	stateArguments[17][0] = ABN_stateArg8DecFormat;
	stateArguments[17][1] = ABN_stateArg8HexFormat;
	stateArguments[17][2] = ABN_stateArg8;

	stateArguments[18][0] = ABN_stateArg9DecFormat;
	stateArguments[18][1] = ABN_stateArg9HexFormat;
	stateArguments[18][2] = ABN_stateArg9;

	stateArguments[19][0] = ABN_stateArg10DecFormat;
	stateArguments[19][1] = ABN_stateArg10HexFormat;
	stateArguments[19][2] = ABN_stateArg10;

	eyeWinSrcNames[0][0] = ABN_eyeWinXposDPtoggle;
	eyeWinSrcNames[0][1] = ABN_eyeWinXposRXtoggle;
	eyeWinSrcNames[0][2] = ABN_eyeWinXposJXtoggle;
	eyeWinSrcNames[0][3] = ABN_eyeWinXposDAtoggle;
	eyeWinSrcNames[0][4] = ABN_eyeWinXposSigtoggle;
	eyeWinSrcNames[0][5] = ABN_eyeWinXposMemtoggle;

	eyeWinSrcNames[1][0] = ABN_eyeWinYposDPtoggle;
	eyeWinSrcNames[1][1] = ABN_eyeWinYposRYtoggle;
	eyeWinSrcNames[1][2] = ABN_eyeWinYposJYtoggle;
	eyeWinSrcNames[1][3] = ABN_eyeWinYposDAtoggle;
	eyeWinSrcNames[1][4] = ABN_eyeWinYposSigtoggle;
	eyeWinSrcNames[1][5] = ABN_eyeWinYposMemtoggle;

	eyeWinSrcNames[2][0] = ABN_eyeWinXchkDPtoggle;
	eyeWinSrcNames[2][1] = ABN_eyeWinXchkRXtoggle;
	eyeWinSrcNames[2][2] = ABN_eyeWinXchkJXtoggle;
	eyeWinSrcNames[2][3] = ABN_eyeWinXchkDAtoggle;
	eyeWinSrcNames[2][4] = ABN_eyeWinXchkSigtoggle;
	eyeWinSrcNames[2][5] = ABN_eyeWinXchkMemtoggle;

	eyeWinSrcNames[3][0] = ABN_eyeWinYchkDPtoggle;
	eyeWinSrcNames[3][1] = ABN_eyeWinYchkRYtoggle;
	eyeWinSrcNames[3][2] = ABN_eyeWinYchkJYtoggle;
	eyeWinSrcNames[3][3] = ABN_eyeWinYchkDAtoggle;
	eyeWinSrcNames[3][4] = ABN_eyeWinYchkSigtoggle;
	eyeWinSrcNames[3][5] = ABN_eyeWinYchkMemtoggle;

	actEyeWinSrcNames[0] = ABN_eyeWinXposNamAct;
	actEyeWinSrcNames[1] = ABN_eyeWinYposNamAct;
	actEyeWinSrcNames[2] = ABN_eyeWinXchkNamAct;
	actEyeWinSrcNames[3] = ABN_eyeWinYchkNamAct;

	sigTypeToggles[0] = ABN_asSigOffToggle;
	sigTypeToggles[1] = ABN_asADToggle;
	sigTypeToggles[2] = ABN_asMemoryToggle;
	sigTypeToggles[3] = ABN_asComputeToggle;

	adVarToggles[0] = ABN_asEyehToggle;
	adVarToggles[1] = ABN_asEyevToggle;
	adVarToggles[2] = ABN_asOeyehToggle;
	adVarToggles[3] = ABN_asOeyevToggle;
	adVarToggles[4] = ABN_asOtherhToggle;
	adVarToggles[5] = ABN_asOthervToggle;
	adVarToggles[6] = ABN_asAddhToggle;
	adVarToggles[7] = ABN_asAddvToggle;
	adVarToggles[8] = ABN_asJoyhToggle;
	adVarToggles[9] = ABN_asJoyvToggle;

	memSrcToggles[0] = ABN_asDA0Toggle;
	memSrcToggles[1] = ABN_asDA1Toggle;
	memSrcToggles[2] = ABN_asDA2Toggle;
	memSrcToggles[3] = ABN_asDA3Toggle;
	memSrcToggles[4] = ABN_asRamp0XToggle;
	memSrcToggles[5] = ABN_asRamp0YToggle;
	memSrcToggles[6] = ABN_asRamp1XToggle;
	memSrcToggles[7] = ABN_asRamp1YToggle;
	memSrcToggles[8] = ABN_asWD0XToggle;
	memSrcToggles[9] = ABN_asWD0YToggle;
	memSrcToggles[10] = ABN_asWD1XToggle;
	memSrcToggles[11] = ABN_asWD1YToggle;

	computedToggles[0] = ABN_asVerghToggle;
	computedToggles[1] = ABN_asVergvToggle;
	computedToggles[2] = ABN_asCyclophToggle;
	computedToggles[3] = ABN_asCyclopvToggle;
	computedToggles[4] = ABN_asGazehToggle;
	computedToggles[5] = ABN_asGazevToggle;
	computedToggles[6] = ABN_asOgazehToggle;
	computedToggles[7] = ABN_asOgazevToggle;
	computedToggles[8] = ABN_asVgazehToggle;
	computedToggles[9] = ABN_asVgazevToggle;
	computedToggles[10] = ABN_asCgazehToggle;
	computedToggles[11] = ABN_asCgazevToggle;

	aqrRtToggles[0] = ABN_asAquire2000Toggle;
	aqrRtToggles[1] = ABN_asAquire1000Toggle;
	aqrRtToggles[2] = ABN_asAquire500Toggle;
	aqrRtToggles[3] = ABN_asAquire250Toggle;

	strRtToggles[0] = ABN_asStore2000Toggle;
	strRtToggles[1] = ABN_asStore1000Toggle;
	strRtToggles[2] = ABN_asStore500Toggle;
	strRtToggles[3] = ABN_asStore250Toggle;

	adCalToggles[0] = ABN_asCal204Toggle;
	adCalToggles[1] = ABN_asCal102Toggle;
	adCalToggles[2] = ABN_asCal51Toggle;
	adCalToggles[3] = ABN_asCal25Toggle;
	adCalToggles[4] = ABN_asCal12Toggle;
	adCalToggles[5] = ABN_asCal6Toggle;
	adCalToggles[6] = ABN_asCalNoiseToggle;

	daSourceToggles[0] = ABN_daStandbyToggle;
	daSourceToggles[1] = ABN_daRampXToggle;
	daSourceToggles[2] = ABN_daRampYToggle;
	daSourceToggles[3] = ABN_daJoyXToggle;
	daSourceToggles[4] = ABN_daJoyYToggle;
	daSourceToggles[5] = ABN_daSignalToggle;
	daSourceToggles[6] = ABN_daWinXToggle;
	daSourceToggles[7] = ABN_daWinYToggle;
	daSourceToggles[8] = ABN_daMemToggle;

	return( Pt_CONTINUE );
}

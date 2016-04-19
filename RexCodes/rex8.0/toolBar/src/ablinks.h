/* Link header for application - AppBuilder 2.03  */

extern ApContext_t AbContext;

ApWindowLink_t base = {
	"base.wgtw",
	&AbContext,
	AbLinks_base, 0, 22
	};

ApDialogLink_t signalDialog = {
	"signalDialog.wgtd",
	&AbContext,
	AbLinks_signalDialog, 14, 57
	};

ApDialogLink_t sigSumDialog = {
	"sigSumDialog.wgtd",
	&AbContext,
	NULL, 79, 0
	};

ApDialogLink_t eyeWinSumDialog = {
	"eyeWinSumDialog.wgtd",
	&AbContext,
	NULL, 81, 0
	};

ApDialogLink_t stateListDialog = {
	"stateListDialog.wgtd",
	&AbContext,
	AbLinks_stateListDialog, 83, 48
	};

ApDialogLink_t eyeWinDialog = {
	"eyeWinDialog.wgtd",
	&AbContext,
	AbLinks_eyeWinDialog, 142, 75
	};

ApDialogLink_t processDialog = {
	"processDialog.wgtd",
	&AbContext,
	AbLinks_processDialog, 210, 38
	};

ApDialogLink_t rampDialog = {
	"rampDialog.wgtd",
	&AbContext,
	AbLinks_rampDialog, 245, 35
	};

ApDialogLink_t sacDecDialog = {
	"sacDecDialog.wgtd",
	&AbContext,
	AbLinks_sacDecDialog, 283, 14
	};

ApDialogLink_t daConvDialog = {
	"daConvDialog.wgtd",
	&AbContext,
	AbLinks_daConvDialog, 296, 32
	};

ApDialogLink_t daSumDialog = {
	"daSumDialog.wgtd",
	&AbContext,
	NULL, 325, 0
	};

ApDialogLink_t memArrayDialog = {
	"memArrayDialog.wgtd",
	&AbContext,
	AbLinks_memArrayDialog, 327, 14
	};

ApDialogLink_t memArraySumDialog = {
	"memArraySumDialog.wgtd",
	&AbContext,
	NULL, 343, 0
	};

ApDialogLink_t eyeOffDialog = {
	"eyeOffDialog.wgtd",
	&AbContext,
	AbLinks_eyeOffDialog, 345, 13
	};

ApDialogLink_t bitsDialog = {
	"bitsDialog.wgtd",
	&AbContext,
	AbLinks_bitsDialog, 359, 122
	};

ApPictureLink_t sigSumPictures = {
	"sigSumPictures.wgtp",
	&AbContext,
	NULL,
	453, 0
	};

ApPictureLink_t eyeWinSumPictures = {
	"eyeWinSumPictures.wgtp",
	&AbContext,
	NULL,
	464, 0
	};

ApPictureLink_t daSumPictures = {
	"daSumPictures.wgtp",
	&AbContext,
	NULL,
	478, 0
	};

ApPictureLink_t memArraySumPictures = {
	"memArraySumPictures.wgtp",
	&AbContext,
	NULL,
	484, 0
	};

ApDialogLink_t rampSumDialog = {
	"rampSumDialog.wgtd",
	&AbContext,
	NULL, 490, 0
	};

ApPictureLink_t rampSumPictures = {
	"rampSumPictures.wgtp",
	&AbContext,
	NULL,
	492, 0
	};

ApPictureLink_t userMenuPictures = {
	"userMenuPictures.wgtp",
	&AbContext,
	AbLinks_userMenuPictures,
	504, 3
	};

ApDialogLink_t userFunctionDialog = {
	"userFunctionDialog.wgtd",
	&AbContext,
	NULL, 507, 0
	};

ApPictureLink_t userFunctionPictures = {
	"userFunctionPictures.wgtp",
	&AbContext,
	AbLinks_userFunctionPictures,
	509, 3
	};

ApWindowLink_t userMenuWindow = {
	"userMenuWindow.wgtw",
	&AbContext,
	AbLinks_userMenuWindow, 514, 4
	};

ApDialogLink_t smiDialog = {
	"smiDialog.wgtd",
	&AbContext,
	AbLinks_smiDialog, 520, 18
	};

ApPictureLink_t stateDebugPictures = {
	"stateDebugPictures.wgtp",
	&AbContext,
	AbLinks_stateDebugPictures,
	539, 1
	};

ApDialogLink_t userRtVarsDialog = {
	"userRtVarsDialog.wgtd",
	&AbContext,
	AbLinks_userRtVarsDialog, 541, 2
	};

ApPictureLink_t userRtVarsPictures = {
	"userRtVarsPictures.wgtp",
	&AbContext,
	NULL,
	547, 0
	};

ApDialogLink_t stateDebugDialog = {
	"stateDebugDialog.wgtd",
	&AbContext,
	AbLinks_stateDebugDialog, 551, 48
	};

ApDialogLink_t stateDebugWindow = {
	"stateDebugWindow.wgtd",
	&AbContext,
	AbLinks_stateDebugWindow, 608, 1
	};

ApDialogLink_t rexMessageDialog = {
	"rexMessageDialog.wgtd",
	&AbContext,
	AbLinks_rexMessageDialog, 610, 1
	};

ApDialogLink_t scoreAlertDialog = {
	"scoreAlertDialog.wgtd",
	&AbContext,
	AbLinks_scoreAlertDialog, 612, 2
	};

static ApItem_t ApItems_statesMenu[ 3 ] = {
	{ 1, 1, 0, NULL, 0, "statesAll", "Show All States", NULL },
	{ 1, 1, 0, NULL, 0, "statesTime", "Show Timed States", NULL },
	{ 0, 0, NULL, NULL, 0, NULL, NULL, NULL } };

ApMenuLink_t statesMenu = {
	"statesMenu",
	"",
	NULL,
	"Helvetica09ba",
	2,
	ApItems_statesMenu,
	& AbContext,
	AbLinks_statesMenu,
	616, 2, 2
	};

static ApItem_t ApItems_controlsMenu[ 9 ] = {
	{ 1, 1, 0, NULL, 0, "controlsProcess", "Process Controls", NULL },
	{ 1, 1, 0, NULL, 0, "controlsRamps", "Ramp Controls", NULL },
	{ 1, 1, 0, NULL, 0, "controlsRampSum", "Ramps Summary", NULL },
	{ 1, 1, 0, NULL, 0, "controlsDA", "D to A Controls", NULL },
	{ 1, 1, 0, NULL, 0, "controlsDAsum", "D to A Summary", NULL },
	{ 1, 1, 0, NULL, 0, "controlsMemArray", "Mem Array Controls", NULL },
	{ 1, 1, 0, NULL, 0, "controlsMemArraySum", "Mem Array Summary", NULL },
	{ 1, 1, 0, NULL, 0, "smiControls", "Smi Controls", NULL },
	{ 0, 0, NULL, NULL, 0, NULL, NULL, NULL } };

ApMenuLink_t controlsMenu = {
	"controlsMenu",
	"",
	NULL,
	"Helvetica09ba",
	2,
	ApItems_controlsMenu,
	& AbContext,
	AbLinks_controlsMenu,
	619, 8, 8
	};

static ApItem_t ApItems_signalsMenu[ 3 ] = {
	{ 1, 1, 0, NULL, 0, "signalsMenuControls", "Analog Signal Controls", NULL },
	{ 1, 1, 0, NULL, 0, "signalsMenuSummary", "Analog Signal Summary", NULL },
	{ 0, 0, NULL, NULL, 0, NULL, NULL, NULL } };

ApMenuLink_t signalsMenu = {
	"signalsMenu",
	"",
	NULL,
	"Helvetica09ba",
	2,
	ApItems_signalsMenu,
	& AbContext,
	AbLinks_signalsMenu,
	628, 2, 2
	};

static ApItem_t ApItems_eyeWindowMenu[ 4 ] = {
	{ 1, 1, 0, NULL, 0, "eyeWinMenuControls", "Eye Window Controls", NULL },
	{ 1, 1, 0, NULL, 0, "eyeWinMenuOffsets", "Eye Offset Controls", NULL },
	{ 1, 1, 0, NULL, 0, "eyeWinMenuSummary", "Eye Window Summary", NULL },
	{ 0, 0, NULL, NULL, 0, NULL, NULL, NULL } };

ApMenuLink_t eyeWindowMenu = {
	"eyeWindowMenu",
	"",
	NULL,
	"Helvetica09ba",
	2,
	ApItems_eyeWindowMenu,
	& AbContext,
	AbLinks_eyeWindowMenu,
	631, 3, 3
	};

static ApItem_t ApItems_userMenu[ 5 ] = {
	{ 1, 2, 0, NULL, 0, "userMenuItem", "Menus", NULL },
	{ 2, 8, 0, NULL, 0, "", (char*) initUserMenus, NULL },
	{ 1, 1, 0, NULL, 0, "userFunctionItem", "Functions", NULL },
	{ 1, 1, 0, NULL, 0, "userMenuRtVars", "Real-timeVariables", NULL },
	{ 0, 0, NULL, NULL, 0, NULL, NULL, NULL } };

ApMenuLink_t userMenu = {
	"userMenu",
	"",
	NULL,
	"Helvetica09ba",
	2,
	ApItems_userMenu,
	& AbContext,
	AbLinks_userMenu,
	635, 2, 4
	};


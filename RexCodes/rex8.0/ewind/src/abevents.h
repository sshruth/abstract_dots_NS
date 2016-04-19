/* Event header for application - AppBuilder 2.03  */

static const ApEventLink_t AbApplLinks[] = {
	{ 3, 0, 0L, 0L, 0L, &base, NULL, NULL, 0, init, 2, 0, 0, 0, },
	{ 0 }
	};

ApEventLink_t AbInternalLinks[] = {
	{ 4, 0, 0L, 0L, 0L, &vexDebugDialog, NULL, NULL, 0, initVexDebug, 2, 9, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_base[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "base", 18017, manageWin, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "base", 10003, resizeWin, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "base", 18024, closeSocket, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "base", 18024, exitEwind, 0, 0, 0, 0, },
	{ 5, 0, 0L, 0L, 0L, &ewinMenu, NULL, "windowOptionsButton", 2007, NULL, 0, 1, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "windowRefreshButton", 2009, winRefresh, 0, 0, 0, 0, },
	{ 8, 1, 0L, 0L, 22L, NULL, NULL, "eyeWndPane", 1026, vexMouse, 0, 0, 0, 0, },
	{ 8, 3, 0L, 0L, 0L, NULL, NULL, "displayWindow", 24000, (int(*)(PtWidget_t*,ApInfo_t*,PtCallbackInfo_t*)) dispWindDraw, 0, 0, 0, 0, },
	{ 8, 3, 0L, 0L, 0L, NULL, NULL, "displayWindow", 24002, (int(*)(PtWidget_t*,ApInfo_t*,PtCallbackInfo_t*)) dispWindExtent, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "vexCommand", 2009, keyboard, 0, 0, 0, 0, },
	{ 8, 1, 0L, 0L, 1L, NULL, NULL, "vexCommand", 1026, character, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "curs0Btn", 2007, cursorChoice, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "curs0Btn", 2008, cursorChoice, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "curs1Btn", 2007, cursorChoice, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "curs1Btn", 2008, cursorChoice, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "curs2Btn", 2007, cursorChoice, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "curs2Btn", 2008, cursorChoice, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "curs3Btn", 2007, cursorChoice, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "curs3Btn", 2008, cursorChoice, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "curs4Btn", 2007, cursorChoice, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "curs4Btn", 2008, cursorChoice, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "curs5Btn", 2007, cursorChoice, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "curs5Btn", 2008, cursorChoice, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "curs6Btn", 2007, cursorChoice, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "curs6Btn", 2008, cursorChoice, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "curs7Btn", 2007, cursorChoice, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "curs7Btn", 2008, cursorChoice, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "wnd0Btn", 2007, cursorChoice, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "wnd0Btn", 2008, cursorChoice, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "wnd1Btn", 2007, cursorChoice, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "wnd1Btn", 2008, cursorChoice, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "wnd2Btn", 2007, cursorChoice, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "wnd2Btn", 2008, cursorChoice, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "wnd3Btn", 2007, cursorChoice, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "wnd3Btn", 2008, cursorChoice, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "wnd4Btn", 2007, cursorChoice, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "wnd4Btn", 2008, cursorChoice, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "wnd5Btn", 2007, cursorChoice, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "wnd5Btn", 2008, cursorChoice, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "wnd6Btn", 2007, cursorChoice, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "wnd6Btn", 2008, cursorChoice, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "wnd7Btn", 2007, cursorChoice, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "wnd7Btn", 2008, cursorChoice, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "gridToggleButton", 2009, windowGrid, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "wndTimer", 41002, windTimer, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_windowOptionsDialog[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "scale0Btn", 2007, windowScale, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "scale1Btn", 2007, windowScale, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "scale2Btn", 2007, windowScale, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "scale3Btn", 2007, windowScale, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "scale4Btn", 2007, windowScale, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "scale5Btn", 2007, windowScale, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "winRefreshScrollBar", 29010, refreshRate, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "winRefreshTrigButton", 2009, refreshTrig, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "winRefreshLevel", 53015, refreshTrigLevel, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "autoRefreshButton", 2009, autoRefresh, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "autoRefreshInterval", 53015, autoInterval, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "channelList", 23010, initDataChan, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "signalList", 23010, dataChan, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_vexName[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "vexNameText", 2009, connectVex, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "vexNameText", 4010, getVexName, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "vexCancelButton", 2009, hideDialog, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "vexSetButton", 2009, connectVex, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_vexDebugDialog[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "vexDebugDialog", 18017, closeDebugDialog, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_ewinMenu[] = {
	{ 4, 0, 0L, 0L, 0L, &windowOptionsDialog, NULL, "ewinOptions", 2009, initWinOptionsDialog, 1, 1, 0, 0, },
	{ 4, 0, 0L, 0L, 0L, &vexName, NULL, "vexConnect", 2009, initPcsSocket, 0, 1, 0, 0, },
	{ 0 }
	};


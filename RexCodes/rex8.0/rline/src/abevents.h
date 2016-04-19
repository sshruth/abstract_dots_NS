/* Event header for application - AppBuilder 2.03  */

static const ApEventLink_t AbApplLinks[] = {
	{ 3, 0, 0L, 0L, 0L, &base, NULL, NULL, 0, init, 2, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_base[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "base", 18017, manageWin, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "base", 10003, resizeWin, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "base", 18024, exitRline, 0, 0, 0, 0, },
	{ 8, 3, 0L, 0L, 0L, NULL, NULL, "runLineWindow", 24000, (int(*)(PtWidget_t*,ApInfo_t*,PtCallbackInfo_t*)) runLineDraw, 0, 0, 0, 0, },
	{ 8, 3, 0L, 0L, 0L, NULL, NULL, "runLineWindow", 24002, (int(*)(PtWidget_t*,ApInfo_t*,PtCallbackInfo_t*)) runLineExtent, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rlTrigButton", 2009, rlineTrig, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rlRepeatButton", 2009, rlineDisplay, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rlModeButton", 2009, rlineMode, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rlTrigLevel", 53015, rlTrigLevel, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "chan0Toggle", 2009, signalChoice, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "chan1Toggle", 2009, signalChoice, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "chan2Toggle", 2009, signalChoice, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "chan3Toggle", 2009, signalChoice, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "unitsToggle", 2009, signalChoice, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "chan4Toggle", 2009, signalChoice, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "chan5Toggle", 2009, signalChoice, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "chan6Toggle", 2009, signalChoice, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "chan7Toggle", 2009, signalChoice, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rlineToggle", 2009, signalChoice, 0, 0, 0, 0, },
	{ 4, 0, 0L, 0L, 0L, &runLineDialog, NULL, "runLineOptions", 2007, initRunChan, 0, 11, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "dispResetButton", 2009, rlDisplayReset, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rlTimer", 41002, rlineTimer, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_runLineDialog[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "runDispBarOffScroll", 29010, runBarOff, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "runDispRateScroll", 29010, runDispRate, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "scale0Btn", 2009, signalScale, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "scale1Btn", 2009, signalScale, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "scale2Btn", 2009, signalScale, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "scale3Btn", 2009, signalScale, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "scale4Btn", 2009, signalScale, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "scale5Btn", 2009, signalScale, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "wrapBtn", 2009, signalWrap, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "runChanList", 23010, initRunChan, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "runChanSigNumb", 53015, runChanSig, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "runChanSigTitle", 2009, NULL, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "runChanOffsetScroll", 29010, runChanOffset, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "runChanScaleScroll", 29010, runChanScale, 0, 0, 0, 0, },
	{ 0 }
	};


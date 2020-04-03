/* Event header for application - AppBuilder 2.03  */

static const ApEventLink_t AbApplLinks[] = {
	{ 3, 0, 0L, 0L, 0L, &base, NULL, NULL, 0, initXcorr, 2, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_base[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "base", 18017, manageWin, 0, 0, 0, 0, },
	{ 8, 1, 0L, 0L, 64L, NULL, NULL, "base", 1011, exposeWin, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "base", 10003, resizeWin, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "base", 18024, exitXcorr, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "xcorrTimer", 41002, getData, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "sampleToggleButton", 2009, switchSampling, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "clearButton", 2009, clearData, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "trigUnitInteger", 53015, setTrigUnit, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "xcorrUnitInteger", 53015, setCorrelatedUnit, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "preInteger", 53015, setPreTime, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "intervalInteger", 53015, setInterval, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "sigmaFloat", 2009, setSigma, 0, 0, 0, 0, },
	{ 8, 3, 0L, 0L, 0L, NULL, NULL, "tigWindow", 24000, (int(*)(PtWidget_t*,ApInfo_t*,PtCallbackInfo_t*)) tigDraw, 0, 0, 0, 0, },
	{ 8, 3, 0L, 0L, 0L, NULL, NULL, "tigWindow", 24002, (int(*)(PtWidget_t*,ApInfo_t*,PtCallbackInfo_t*)) tigExtent, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "gainScrollbar", 29010, setGain, 0, 0, 0, 0, },
	{ 0 }
	};


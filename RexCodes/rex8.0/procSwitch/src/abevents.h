/* Event header for application - AppBuilder 2.03  */

static const ApEventLink_t AbApplLinks[] = {
	{ 3, 0, 0L, 0L, 0L, &base, NULL, NULL, 0, init, 1, 0, 0, 0, },
	{ 0 }
	};

ApEventLink_t AbInternalLinks[] = {
	{ 4, 0, 0L, 0L, 0L, &rexErrorDialog, NULL, NULL, 0, initRexError, 0, 11, 0, 100, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_base[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "base", 18024, exitRex, 0, 0, 0, 0, },
	{ 5, 0, 0L, 0L, 0L, &filesMenu, NULL, "fileMenuButton", 2007, NULL, 0, 1, 0, 0, },
	{ 5, 2, 102L, 2L, 0L, &filesMenu, NULL, "fileMenuButton", 1010, NULL, 0, 1, 0, 0, },
	{ 5, 0, 0L, 0L, 0L, &displayMenu, NULL, "displayMenuButton", 2007, NULL, 0, 1, 0, 0, },
	{ 5, 2, 100L, 2L, 0L, &displayMenu, NULL, "displayMenuButton", 1010, NULL, 0, 1, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "cntrlBMenuButton", 2007, onCntrlB, 0, 0, 0, 0, },
	{ 8, 2, 98L, 2L, 0L, NULL, NULL, "cntrlBMenuButton", 1010, onCntrlB, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "dataKeepBothToggle", 2009, switchData, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "dataKeepEventsToggle", 2009, switchData, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "procWaitTimer", 41002, waitProc, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_processSelectorDialog[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "procFileDirText", 2009, setProcDir, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "procFileSelList", 47007, getProcFileName, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "procFileSelList", 47013, getProcFileName, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "procSelChangeBtn", 2007, runProc, 0, 0, 0, 0, },
	{ 8, 2, 114L, 2L, 0L, NULL, NULL, "procSelChangeBtn", 1010, runProc, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "procSelLoadBtn", 2007, loadProc, 0, 0, 0, 0, },
	{ 8, 2, 108L, 2L, 0L, NULL, NULL, "procSelLoadBtn", 1010, loadProc, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_rootSelectorDialog[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rootFileDirText", 2009, setRootDir, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rootFileSelList", 47007, getRootFileName, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rootFileSelList", 47013, getRootFileName, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rootSelReadBtn", 2007, readRoots, 0, 0, 0, 0, },
	{ 8, 2, 114L, 2L, 0L, NULL, NULL, "rootSelReadBtn", 1010, readRoots, 0, 0, 0, 0, },
	{ 8, 2, 97L, 2L, 0L, NULL, NULL, "rootSelAllBtn", 1010, writeAllRoots, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rootSelAllBtn", 2009, writeAllRoots, 0, 0, 0, 0, },
	{ 8, 2, 61L, 2L, 0L, NULL, NULL, "rootSelSysUsrBtn", 1010, writeSysUserRoots, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rootSelSysUsrBtn", 2009, writeSysUserRoots, 0, 0, 0, 0, },
	{ 8, 2, 115L, 2L, 0L, NULL, NULL, "rootSelSysBtn", 1010, writeSysRoots, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rootSelSysBtn", 2009, writeSysRoots, 0, 0, 0, 0, },
	{ 8, 2, 117L, 2L, 0L, NULL, NULL, "rootSelUsrBtn", 1010, writeUserRoots, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rootSelUsrBtn", 2009, writeUserRoots, 0, 0, 0, 0, },
	{ 8, 2, 116L, 2L, 0L, NULL, NULL, "rootSelStatesBtn", 1010, writeStateRoots, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rootSelStatesBtn", 2009, writeStateRoots, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_dataSelectorDialog[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "dataFileName", 2009, openData, 0, 0, 0, 0, },
	{ 9, 0, 0L, 0L, 0L, NULL, NULL, "dataFileName", 2009, NULL, 0, 0, 0, 0, },
	{ 8, 2, 111L, 2L, 0L, NULL, NULL, "dataSelLoadBtn", 1010, openData, 0, 0, 0, 0, },
	{ 9, 2, 111L, 2L, 0L, NULL, NULL, "dataSelLoadBtn", 1010, NULL, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "dataSelLoadBtn", 2009, openData, 0, 0, 0, 0, },
	{ 9, 0, 0L, 0L, 0L, NULL, NULL, "dataSelLoadBtn", 2009, NULL, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "dataFileDirText", 2009, setDataDir, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "dataFileSelList", 47007, getDataFileName, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "dataFileSelList", 47013, getDataFileName, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_rexErrorDialog[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rexErrorDialog", 18017, closeErrorDialog, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_displayMenu[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "displayWindow", 2009, startWindow, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "displayRaster", 2009, startRaster, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "displayRunLine", 2009, startRunLine, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "xcorrrDisplay", 2009, startXcorr, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_filesMenu[] = {
	{ 4, 0, 0L, 0L, 0L, &processSelectorDialog, NULL, "filesMenuProcesses", 2009, initProcSelDialog, 0, 11, 0, 100, },
	{ 4, 0, 0L, 0L, 0L, &rootSelectorDialog, NULL, "fileRootsRead", 2009, initRootSelDialog, 0, 11, 0, 100, },
	{ 4, 0, 0L, 0L, 0L, &dataSelectorDialog, NULL, "filesDataOpen", 2009, initDataSelDialog, 0, 11, 0, 100, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "filesDataClose", 2009, closeDataFile, 0, 0, 0, 0, },
	{ 0 }
	};


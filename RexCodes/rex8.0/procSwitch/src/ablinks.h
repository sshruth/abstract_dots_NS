/* Link header for application - AppBuilder 2.03  */

extern ApContext_t AbContext;

ApWindowLink_t base = {
	"base.wgtw",
	&AbContext,
	AbLinks_base, 0, 10
	};

ApDialogLink_t processSelectorDialog = {
	"processSelectorDialog.wgtd",
	&AbContext,
	AbLinks_processSelectorDialog, 7, 7
	};

ApDialogLink_t rootSelectorDialog = {
	"rootSelectorDialog.wgtd",
	&AbContext,
	AbLinks_rootSelectorDialog, 13, 15
	};

ApDialogLink_t dataSelectorDialog = {
	"dataSelectorDialog.wgtd",
	&AbContext,
	AbLinks_dataSelectorDialog, 23, 9
	};

ApDialogLink_t rexErrorDialog = {
	"rexErrorDialog.wgtd",
	&AbContext,
	AbLinks_rexErrorDialog, 28, 1
	};

static ApItem_t ApItems_displayMenu[ 5 ] = {
	{ 1, 1, 0, NULL, 0, "displayWindow", "&Window Display", NULL },
	{ 1, 1, 0, NULL, 0, "displayRaster", "&Raster Display", NULL },
	{ 1, 1, 0, NULL, 0, "displayRunLine", "Running &Line Display", NULL },
	{ 1, 1, 0, NULL, 0, "xcorrrDisplay", "&Xcorr Display", NULL },
	{ 0, 0, NULL, NULL, 0, NULL, NULL, NULL } };

ApMenuLink_t displayMenu = {
	"displayMenu",
	"",
	NULL,
	"Helvetica09ba",
	2,
	ApItems_displayMenu,
	& AbContext,
	AbLinks_displayMenu,
	30, 4, 4
	};

static ApItem_t ApItems_filesMenu[ 6 ] = {
	{ 1, 1, 0,"Cntrl-P", 0, "filesMenuProcesses", "&Processes", NULL },
	{ 1, 1, 0,"Cntrl-R", 0, "fileRootsRead", "&Root Files", NULL },
	{ 1, 16, 0, NULL, 4, "", "", NULL },
	{ 1, 1, 0,"Cntrl-O", 0, "filesDataOpen", "&Open Data File", NULL },
	{ 1, 1, 0,"Cntrl-C", 0, "filesDataClose", "&Close Data File", NULL },
	{ 0, 0, NULL, NULL, 0, NULL, NULL, NULL } };

ApMenuLink_t filesMenu = {
	"filesMenu",
	"",
	NULL,
	"Helvetica09ba",
	2,
	ApItems_filesMenu,
	& AbContext,
	AbLinks_filesMenu,
	35, 4, 5
	};


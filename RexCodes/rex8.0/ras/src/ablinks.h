/* Link header for application - AppBuilder 2.03  */

extern ApContext_t AbContext;

ApWindowLink_t base = {
	"base.wgtw",
	&AbContext,
	AbLinks_base, 0, 15
	};

ApWindowLink_t classesSelect = {
	"classesSelect.wgtw",
	&AbContext,
	AbLinks_classesSelect, 12, 9
	};

ApWindowLink_t rastersPage = {
	"rastersPage.wgtw",
	&AbContext,
	AbLinks_rastersPage, 23, 111
	};

ApDialogLink_t fileSelectorDialog = {
	"fileSelectorDialog.wgtd",
	&AbContext,
	AbLinks_fileSelectorDialog, 135, 7
	};

ApWindowLink_t globalSettings = {
	"globalSettings.wgtw",
	&AbContext,
	AbLinks_globalSettings, 142, 29
	};

static ApItem_t ApItems_filesMenu[ 4 ] = {
	{ 1, 1, 0, NULL, 0, "fileRootSave", "Write Root", NULL },
	{ 1, 1, 0, NULL, 0, "fileRootRead", "Read Root", NULL },
	{ 1, 1, 0, NULL, 0, "fileRasterSave", "Save Rasters", NULL },
	{ 0, 0, NULL, NULL, 0, NULL, NULL, NULL } };

ApMenuLink_t filesMenu = {
	"filesMenu",
	"FILES",
	"Helvetica12ba",
	"Helvetica09ba",
	2,
	ApItems_filesMenu,
	& AbContext,
	AbLinks_filesMenu,
	172, 3, 3
	};

static ApItem_t ApItems_classesMenu[ 3 ] = {
	{ 1, 1, 0, NULL, 0, "classesBuild", "Build Select Specs", NULL },
	{ 1, 1, 0, NULL, 0, "classesClear", "Clear Select Specs", NULL },
	{ 0, 0, NULL, NULL, 0, NULL, NULL, NULL } };

ApMenuLink_t classesMenu = {
	"classesMenu",
	"CLASSES",
	"Helvetica09ba",
	"Helvetica09ba",
	2,
	ApItems_classesMenu,
	& AbContext,
	AbLinks_classesMenu,
	176, 2, 2
	};

static ApItem_t ApItems_rastersMenu[ 4 ] = {
	{ 1, 1, 0, NULL, 0, "rastersBuild", "Build Raster Specs", NULL },
	{ 1, 1, 0, NULL, 0, "rastersGlobal", "Global Settings", NULL },
	{ 1, 1, 0, NULL, 0, "rastersClear", "Clear Raster Specs", NULL },
	{ 0, 0, NULL, NULL, 0, NULL, NULL, NULL } };

ApMenuLink_t rastersMenu = {
	"rastersMenu",
	"RASTERS",
	"Helvetica09ba",
	"Helvetica09ba",
	2,
	ApItems_rastersMenu,
	& AbContext,
	AbLinks_rastersMenu,
	179, 3, 3
	};


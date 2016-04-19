/* Link header for application - AppBuilder 2.03  */

#if defined(__cplusplus)
extern "C" {
#endif

extern ApContext_t AbContext;

ApWindowLink_t rebelBaseWindow = {
	"rebelBaseWindow.wgtw",
	&AbContext,
	AbLinks_rebelBaseWindow, 0, 22
	};

ApWindowLink_t rebelWindow = {
	"rebelWindow.wgtw",
	&AbContext,
	AbLinks_rebelWindow, 13, 5
	};

ApPictureLink_t rebelPictures = {
	"rebelPictures.wgtp",
	&AbContext,
	AbLinks_rebelPictures,
	15, 17
	};

ApDialogLink_t rebelOpenSelector = {
	"rebelOpenSelector.wgtd",
	&AbContext,
	AbLinks_rebelOpenSelector, 31, 6
	};

ApDialogLink_t rebelSaveAsSelector = {
	"rebelSaveAsSelector.wgtd",
	&AbContext,
	AbLinks_rebelSaveAsSelector, 36, 6
	};

ApDialogLink_t rebelResourceDialog = {
	"rebelResourceDialog.wgtd",
	&AbContext,
	AbLinks_rebelResourceDialog, 41, 104
	};

ApDialogLink_t rebelWidgetDialog = {
	"rebelWidgetDialog.wgtd",
	&AbContext,
	AbLinks_rebelWidgetDialog, 89, 7
	};

ApDialogLink_t rebelActionDialog = {
	"rebelActionDialog.wgtd",
	&AbContext,
	AbLinks_rebelActionDialog, 97, 39
	};

ApDialogLink_t rebelGlobalsDialog = {
	"rebelGlobalsDialog.wgtd",
	&AbContext,
	AbLinks_rebelGlobalsDialog, 121, 1
	};

ApDialogLink_t rebelEscapeDialog = {
	"rebelEscapeDialog.wgtd",
	&AbContext,
	AbLinks_rebelEscapeDialog, 124, 9
	};

ApDialogLink_t rebelAbortDialog = {
	"rebelAbortDialog.wgtd",
	&AbContext,
	AbLinks_rebelAbortDialog, 137, 3
	};

ApDialogLink_t rebelLabActionsDialog = {
	"rebelLabActionsDialog.wgtd",
	&AbContext,
	AbLinks_rebelLabActionsDialog, 141, 1
	};

static ApItem_t ApItems_rebelFileMenu[ 10 ] = {
	{ 1, 1, 1,"N", 0, "rebelFileMenuNew", "&New", NULL },
	{ 1, 1, 1,"O", 0, "rebelFileMenuOpen", "&Open", NULL },
	{ 1, 1, 0, NULL, 0, "fileMenuPrototypes", "Prototypes", NULL },
	{ 1, 1, 1,"S", 0, "rebelFileMenuSave", "&Save", NULL },
	{ 1, 1, 4,"A", 0, "rebelFileMenuSaveAs", "Save &As", NULL },
	{ 1, 1, 0, NULL, 0, "rebelFileMenuSpot", "Save With Spot", NULL },
	{ 1, 1, 0, NULL, 0, "rebelFileMenuSaveSnapshot", "Save Snapshot", NULL },
	{ 1, 1, 0, NULL, 0, "rebelFileMenuClose", "&Close", NULL },
	{ 1, 1, 0, NULL, 0, "rebelFileMenuExit", "Exit", NULL },
	{ 0, 0, NULL, NULL, 0, NULL, NULL, NULL } };

ApMenuLink_t rebelFileMenu = {
	"rebelFileMenu",
	"",
	NULL,
	NULL,
	2,
	ApItems_rebelFileMenu,
	& AbContext,
	AbLinks_rebelFileMenu,
	144, 9, 9
	};

static ApItem_t ApItems_rebelEditMenu[ 16 ] = {
	{ 1, 1, 1,"x", 0, "rebelEditMenuCut", "Cut", NULL },
	{ 1, 1, 1,"c", 0, "rebelEditMenuCopy", "Copy", NULL },
	{ 1, 1, 1,"v", 0, "rebelEditMenuPaste", "Paste", NULL },
	{ 1, 1, 0, NULL, 0, "rebelEditMenuSnap", "Snap to Grid", NULL },
	{ 1, 16, 0, NULL, 4, "", "", NULL },
	{ 1, 2, 0, NULL, 0, "rebelEditSrcMenu", "Edit Source", NULL },
	{ 2, 1, 0, NULL, 0, "rebelEditSrcJedEdit", "Jed", NULL },
	{ 2, 1, 0, NULL, 0, "rebelEditSrcWsEdit", "Workspace", NULL },
	{ 2, 1, 0, NULL, 0, "rebelEditSrcViEdit", "Vi", NULL },
	{ 2, 1, 0, NULL, 0, "rebelEditSrcPedEdit", "Ped", NULL },
	{ 1, 2, 0, NULL, 0, "rebelEditHdrMenu", "Edit Header", NULL },
	{ 2, 1, 0, NULL, 0, "rebelEditHdrJedEdit", "Jed", NULL },
	{ 2, 1, 0, NULL, 0, "rebelEditHdrWsEdit", "Workspace", NULL },
	{ 2, 1, 0, NULL, 0, "rebelEditHdrViEdit", "Vi", NULL },
	{ 2, 1, 0, NULL, 0, "rebelEditHdrPedEdit", "Ped", NULL },
	{ 0, 0, NULL, NULL, 0, NULL, NULL, NULL } };

ApMenuLink_t rebelEditMenu = {
	"rebelEditMenu",
	"",
	NULL,
	NULL,
	2,
	ApItems_rebelEditMenu,
	& AbContext,
	AbLinks_rebelEditMenu,
	154, 12, 15
	};

static ApItem_t ApItems_rebelAppMenu[ 11 ] = {
	{ 1, 1, 0, NULL, 0, "rebelAppMenuChain", "&Chain", NULL },
	{ 1, 1, 0, NULL, 0, "rebelAppMenuMenu", "&Menu", NULL },
	{ 1, 1, 0, NULL, 0, "rebelAppMenuVariable", "&Variable", NULL },
	{ 1, 1, 0, NULL, 0, "rebelAppMenuFunction", "&Function", NULL },
	{ 1, 16, 0, NULL, 4, "", "", NULL },
	{ 1, 1, 0, NULL, 0, "rebelAppMenuGlobals", "&Globals", NULL },
	{ 1, 1, 0, NULL, 0, "rebelAppMenuActions", "&Actions", NULL },
	{ 1, 16, 0, NULL, 4, "", "", NULL },
	{ 1, 1, 0, NULL, 0, "rebelAppMenuMake", "Make", NULL },
	{ 1, 16, 0, NULL, 4, "", "", NULL },
	{ 0, 0, NULL, NULL, 0, NULL, NULL, NULL } };

ApMenuLink_t rebelAppMenu = {
	"rebelAppMenu",
	"",
	NULL,
	NULL,
	2,
	ApItems_rebelAppMenu,
	& AbContext,
	AbLinks_rebelAppMenu,
	170, 7, 10
	};

static ApItem_t ApItems_rebelViewMenu[ 7 ] = {
	{ 1, 1, 0, NULL, 0, "rebelViewMenuWidgets", "&Widgets", NULL },
	{ 1, 1, 0, NULL, 0, "rebelViewMenuResources", "&Resources", NULL },
	{ 1, 1, 0, NULL, 0, "rebelViewMenuHide", "Hide All", NULL },
	{ 1, 1, 0, NULL, 0, "rebelViewMenuShow", "Show All", NULL },
	{ 1, 16, 0, NULL, 4, "", "", NULL },
	{ 1, 8, 0, NULL, 0, "", (char*) rebelApp::listWindowsCallback, NULL },
	{ 0, 0, NULL, NULL, 0, NULL, NULL, NULL } };

ApMenuLink_t rebelViewMenu = {
	"rebelViewMenu",
	"",
	NULL,
	NULL,
	2,
	ApItems_rebelViewMenu,
	& AbContext,
	AbLinks_rebelViewMenu,
	181, 4, 6
	};


#if defined(__cplusplus)
}
#endif


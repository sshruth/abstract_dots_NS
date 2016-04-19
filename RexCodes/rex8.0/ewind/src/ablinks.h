/* Link header for application - AppBuilder 2.03  */

extern ApContext_t AbContext;

ApWindowLink_t base = {
	"base.wgtw",
	&AbContext,
	AbLinks_base, 0, 45
	};

ApDialogLink_t windowOptionsDialog = {
	"windowOptionsDialog.wgtd",
	&AbContext,
	AbLinks_windowOptionsDialog, 40, 13
	};

ApDialogLink_t vexName = {
	"vexName.wgtd",
	&AbContext,
	AbLinks_vexName, 55, 4
	};

ApDialogLink_t vexDebugDialog = {
	"vexDebugDialog.wgtd",
	&AbContext,
	AbLinks_vexDebugDialog, 59, 1
	};

static ApItem_t ApItems_ewinMenu[ 3 ] = {
	{ 1, 1, 0, NULL, 0, "ewinOptions", "Options", NULL },
	{ 1, 1, 0, NULL, 0, "vexConnect", "Connect Vex", NULL },
	{ 0, 0, NULL, NULL, 0, NULL, NULL, NULL } };

ApMenuLink_t ewinMenu = {
	"ewinMenu",
	"",
	NULL,
	"Helvetica09ba",
	2,
	ApItems_ewinMenu,
	& AbContext,
	AbLinks_ewinMenu,
	61, 2, 2
	};


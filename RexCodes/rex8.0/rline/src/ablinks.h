/* Link header for application - AppBuilder 2.03  */

extern ApContext_t AbContext;

ApWindowLink_t base = {
	"base.wgtw",
	&AbContext,
	AbLinks_base, 0, 22
	};

ApDialogLink_t runLineDialog = {
	"runLineDialog.wgtd",
	&AbContext,
	AbLinks_runLineDialog, 25, 14
	};


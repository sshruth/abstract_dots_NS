/* Y o u r   D e s c r i p t i o n                       */
/*                            AppBuilder Photon Code Lib */
/*                                         Version 2.01  */

/* Standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* Local headers */
#include "ablibs.h"
#include "rebel.h"
#include "abimport.h"
#include "proto.h"

extern rebelApp *app;
extern rebelResource *rResources;

rebelResource::rebelResource()
{
	PtCallbackInfo_t cbinfo;
	cbinfo.cbdata = (void *)this;
	ApModuleParent(ABM_rebelResourceDialog, AB_PARENT, ABW_rebelBkgd);
	_window = ApCreateModule(ABM_rebelResourceDialog, NULL, &cbinfo);
	_panelGroup = ApGetWidgetPtr(_window, ABN_resourcePanelGroup);
}

rebelResource::~rebelResource()
{
	// this call destroys all of the widgets associated with this window
	PtDestroyWidget(_window);
}


int
rebelResource::init( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	if(apinfo->reason == ABR_PRE_REALIZE) {
		rebelResource *rsrc = (rebelResource *)cbinfo->cbdata;

		char title[64];
		strcpy(title, "RESOURCES");

		PtArg_t args[2];
		int nArgs = 0;
		PtSetArg(&args[nArgs], Pt_ARG_POINTER, rsrc, 0); ++nArgs;
		PtSetArg(&args[nArgs], Pt_ARG_WINDOW_TITLE, title, 0); ++nArgs;
		PtSetResources(link_instance, nArgs, args);
	}

	return( Pt_CONTINUE );

}


int
rebelResource::initActionCallback( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	rResources->initAction(link_instance, apinfo, cbinfo);

	return( Pt_CONTINUE );

}

void
rebelResource::initAction( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	rebelState *state;
	if(apinfo->reason == ABR_PRE_REALIZE) {
		PtGetResource(_panelGroup, Pt_ARG_POINTER, &state, 0);
		PtSetResource(link_instance, Pt_ARG_POINTER, state, 0);
	}
	else {
		PtGetResource(_panelGroup, Pt_ARG_POINTER, &state, 0);
		PtSetResource(link_instance, Pt_ARG_POINTER, state, 0);
		ACTION device = state->device();

		if(strcmp(device.action, "0")) {
			PtSetResource(ABW_actionComboBox, Pt_ARG_TEXT_STRING, device.action, 0);
			setActionArgLabels(device.action);
		}
		else PtSetResource(ABW_actionComboBox, Pt_ARG_TEXT_STRING, "Actions", 0);

		if(strcmp(device.act_code, "0")) PtSetResource(ABW_arg1Text, Pt_ARG_TEXT_STRING, device.act_code, 0);
		else PtSetResource(ABW_arg1Text, Pt_ARG_TEXT_STRING, "", 0);

		if(strcmp(device.act_2code, "0")) PtSetResource(ABW_arg2Text, Pt_ARG_TEXT_STRING, device.act_2code, 0);
		else PtSetResource(ABW_arg2Text, Pt_ARG_TEXT_STRING, "", 0);

		if(strcmp(device.act_3code, "0")) PtSetResource(ABW_arg3Text, Pt_ARG_TEXT_STRING, device.act_3code, 0);
		else PtSetResource(ABW_arg3Text, Pt_ARG_TEXT_STRING, "", 0);

		if(strcmp(device.act_4code, "0")) PtSetResource(ABW_arg4Text, Pt_ARG_TEXT_STRING, device.act_4code, 0);
		else PtSetResource(ABW_arg4Text, Pt_ARG_TEXT_STRING, "", 0);

		if(strcmp(device.act_5code, "0")) PtSetResource(ABW_arg5Text, Pt_ARG_TEXT_STRING, device.act_5code, 0);
		else PtSetResource(ABW_arg5Text, Pt_ARG_TEXT_STRING, "", 0);

		if(strcmp(device.act_6code, "0")) PtSetResource(ABW_arg6Text, Pt_ARG_TEXT_STRING, device.act_6code, 0);
		else PtSetResource(ABW_arg6Text, Pt_ARG_TEXT_STRING, "", 0);

		if(strcmp(device.act_7code, "0")) PtSetResource(ABW_arg7Text, Pt_ARG_TEXT_STRING, device.act_7code, 0);
		else PtSetResource(ABW_arg7Text, Pt_ARG_TEXT_STRING, "", 0);

		if(strcmp(device.act_8code, "0")) PtSetResource(ABW_arg8Text, Pt_ARG_TEXT_STRING, device.act_8code, 0);
		else PtSetResource(ABW_arg8Text, Pt_ARG_TEXT_STRING, "", 0);

		if(strcmp(device.act_9code, "0")) PtSetResource(ABW_arg9Text, Pt_ARG_TEXT_STRING, device.act_9code, 0);
		else PtSetResource(ABW_arg9Text, Pt_ARG_TEXT_STRING, "", 0);

		if(strcmp(device.act_10code, "0")) PtSetResource(ABW_arg10Text, Pt_ARG_TEXT_STRING, device.act_10code, 0);
		else PtSetResource(ABW_arg10Text, Pt_ARG_TEXT_STRING, "", 0);
	}

	return;

}


int
rebelResource::initEscapeCallback( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	rResources->initEscape(link_instance, apinfo, cbinfo);

	return( Pt_CONTINUE );

}

void
rebelResource::initEscape( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	ESCAPE *escp;
	if(apinfo->reason == ABR_PRE_REALIZE) {
		PtWidget_t *escpPnl = PtPGFindPanelByTitle(_panelGroup, "Escape");
		PtGetResource(escpPnl, Pt_ARG_POINTER, &escp, 0);
		PtSetResource(link_instance, Pt_ARG_POINTER, escp, 0);
	}
	else if(apinfo->reason == ABR_POST_REALIZE) {
		PtGetResource(link_instance, Pt_ARG_POINTER, &escp, 0);

		if(strlen(escp->escapeOn) == 0) {
			PtSetResource(ABW_escapePlusToggleButton, Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
			PtSetResource(ABW_escapeMinusToggleButton, Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);

			PtSetResource(ABW_escapeAmpersandToggleButton, Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
			PtSetResource(ABW_escapeConstantText, Pt_ARG_TEXT_STRING, "", 0);
			PtSetResource(ABW_escapeVariableText, Pt_ARG_TEXT_STRING, "", 0);
		}
		else {
			if(escp->smask[0] == '+') {
				PtSetResource(ABW_escapePlusToggleButton, Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
				PtSetResource(ABW_escapeConstantText, Pt_ARG_TEXT_STRING, &escp->smask[1], 0);
			}
			else if(escp->smask[0] == '-') {
				PtSetResource(ABW_escapeMinusToggleButton, Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
				PtSetResource(ABW_escapeConstantText, Pt_ARG_TEXT_STRING, &escp->smask[1], 0);
			}
			else {
				PtSetResource(ABW_escapePlusToggleButton, Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
				PtSetResource(ABW_escapeMinusToggleButton, Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
				PtSetResource(ABW_escapeConstantText, Pt_ARG_TEXT_STRING, escp->smask, 0);
			}

			if(!strcmp(escp->sflags, "BITON")) {
				PtSetResource(ABW_escapePlusToggleButton, Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
				PtSetResource(ABW_escapeAmpersandToggleButton, Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
			}
			else if(!strcmp(escp->sflags, "BITOFF")) {
				PtSetResource(ABW_escapeMinusToggleButton, Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
				PtSetResource(ABW_escapeAmpersandToggleButton, Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
			}
			else if(!strcmp(escp->sflags, "FUNC")) {
				PtSetResource(ABW_escapePercentToggleButton, Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
			}
			else if(!strcmp(escp->sflags, "GREATER")) {
				PtSetResource(ABW_escapeGreaterToggleButton, Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
			}
			else if(!strcmp(escp->sflags, "LESSOR")) {
				PtSetResource(ABW_escapeLessToggleButton, Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
			}
			else if(!strcmp(escp->sflags, "EQUAL")) {
				PtSetResource(ABW_escapeEqualToggleButton, Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
			}
			else if(!strcmp(escp->sflags, "QUERY")) {
				PtSetResource(ABW_escapeQuestionToggleButton, Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
			}

			if(escp->funct[0] == '&') {
				PtSetResource(ABW_escapeVariableText, Pt_ARG_TEXT_STRING, &escp->funct[1], 0);
			}
			else {
				PtSetResource(ABW_escapeVariableText, Pt_ARG_TEXT_STRING, escp->funct, 0);
			}
		}
	}

	return;
}


int
rebelResource::resizeCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	rebelResource *rsrc;
	PtGetResource(widget, Pt_ARG_POINTER, &rsrc, 0);

	if(!rsrc) return( Pt_CONTINUE );

	rsrc->resize(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );
}

void
rebelResource::resize( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	PhRect_t extent;
	PhArea_t area;
	PtWidgetExtent(widget, &extent);
	PhRectToArea(&extent, &area);
	_origin = extent.ul;
	_size = area.size;

	return;
}


int
rebelResource::winOpenCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	rebelResource *rsrc;
	PtGetResource(widget, Pt_ARG_POINTER, &rsrc, 0);

	if(!rsrc) return( Pt_CONTINUE );

	rsrc->winOpen(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );
}

void
rebelResource::winOpen( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	PtSetResource(widget, Pt_ARG_DIM, &_size, 0);

	return;
}


int
rebelResource::textNotify( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	PtArg_t args[2];

	int nArgs = 0;
	PtSetArg(&args[nArgs], Pt_ARG_COLOR, Pg_WHITE, 0); ++nArgs;

	if(PtWidgetClass(widget) == PtText) {
		PtSetArg(&args[nArgs], Pt_ARG_FILL_COLOR, Pg_DCYAN, 0); ++nArgs;
	}

	else if(PtWidgetClass(widget) == PtComboBox) {
		PtSetArg(&args[nArgs], Pt_ARG_CBOX_TEXT_FILL_COLOR, Pg_DCYAN, 0); ++nArgs;
	}

	PtSetResources(widget, nArgs, args);

	return( Pt_CONTINUE );
}


int
rebelResource::clearActionArgsCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	rebelResource *rsrc;
	PtGetResource(widget, Pt_ARG_POINTER, &rsrc, 0);

	if(!rsrc) return( Pt_CONTINUE );

	rsrc->clearActionArgs(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );
}

void
rebelResource::clearActionArgs( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if((cbinfo->reason != Pt_CB_ACTIVATE) && (cbinfo->reason != Pt_CB_SELECTION)) return;

	PtSetResource(ABW_arg1Text, Pt_ARG_TEXT_STRING, "", 0);
	PtSetResource(ABW_arg2Text, Pt_ARG_TEXT_STRING, "", 0);
	PtSetResource(ABW_arg3Text, Pt_ARG_TEXT_STRING, "", 0);
	PtSetResource(ABW_arg4Text, Pt_ARG_TEXT_STRING, "", 0);
	PtSetResource(ABW_arg5Text, Pt_ARG_TEXT_STRING, "", 0);
	PtSetResource(ABW_arg6Text, Pt_ARG_TEXT_STRING, "", 0);
	PtSetResource(ABW_arg7Text, Pt_ARG_TEXT_STRING, "", 0);
	PtSetResource(ABW_arg8Text, Pt_ARG_TEXT_STRING, "", 0);
	PtSetResource(ABW_arg9Text, Pt_ARG_TEXT_STRING, "", 0);
	PtSetResource(ABW_arg10Text, Pt_ARG_TEXT_STRING, "", 0);

	return;
}


int
rebelResource::setMenuAffText( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(cbinfo->reason_subtype == Pt_LIST_SELECTION_FINAL) {
		PtListCallback_t *lstData = (PtListCallback_t *)cbinfo->cbdata;
		char item[32];
		strcpy(item, lstData->item);
		PtSetResource(ABW_menuAccessFlagText, Pt_ARG_TEXT_STRING, item, 0);
		PtSetResource(widget, Pt_ARG_TEXT_STRING, "Function Access Flags", 0);
	}

	return( Pt_CONTINUE );
}


int
rebelResource::setVlistAffText( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(cbinfo->reason_subtype == Pt_LIST_SELECTION_FINAL) {
		PtListCallback_t *lstData = (PtListCallback_t *)cbinfo->cbdata;

		char *str;
		PtGetResource(ABW_vlistAccessFlagText, Pt_ARG_TEXT_STRING, &str, 0);

		if(!strcmp(str, "0")) {
			char item[32];
			strcpy(item, lstData->item);
			PtSetResource(ABW_vlistAccessFlagText, Pt_ARG_TEXT_STRING, item, 0);
		}
		else if(!strcmp(lstData->item, "0")) {
			char item[32];
			strcpy(item, lstData->item);
			PtSetResource(ABW_vlistAccessFlagText, Pt_ARG_TEXT_STRING, item, 0);
		}
		else {
			string temp = str;
			temp += "|";
			temp += lstData->item;
			PtSetResource(ABW_vlistAccessFlagText, Pt_ARG_TEXT_STRING, temp.c_str(), 0);
			
		}
		PtSetResource(widget, Pt_ARG_TEXT_STRING, "Function Access Flags", 0);
	}

	return( Pt_CONTINUE );
}


int
rebelResource::setVlistTypeText( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(cbinfo->reason_subtype == Pt_LIST_SELECTION_FINAL) {
		PtListCallback_t *lstData = (PtListCallback_t *)cbinfo->cbdata;
		char item[32];
		strcpy(item, lstData->item);
		PtSetResource(ABW_vlistTypeText, Pt_ARG_TEXT_STRING, item, 0);
		PtSetResource(widget, Pt_ARG_TEXT_STRING, "Variable Type", 0);
	}

	return( Pt_CONTINUE );
}


void
rebelResource::setChainWidgets(rebelChain *actChain)
{
	PtArg_t args[2];
	int nArgs = 0;
	PtSetArg(&args[nArgs], Pt_ARG_POINTER, actChain, 0); ++nArgs;
	PtSetArg(&args[nArgs], Pt_ARG_PG_CURRENT_INDEX, 0, 0); ++nArgs;
	PtSetResources(_panelGroup, nArgs, args);

	if(actChain) {
		PtSetResource(ABW_chainNameText, Pt_ARG_TEXT_STRING, actChain->name(), 0);
		PtSetResource(ABW_restartText, Pt_ARG_TEXT_STRING, actChain->restartFunction(), 0);
	}
	else {
		PtSetResource(ABW_chainNameText, Pt_ARG_TEXT_STRING, "", 0);
		PtSetResource(ABW_restartText, Pt_ARG_TEXT_STRING, "", 0);
	}
}

void
rebelResource::setStateWidgets(rebelState *actState)
{
	PtArg_t args[2];
	int nArgs = 0;
	PtSetArg(&args[nArgs], Pt_ARG_POINTER, actState, 0); ++nArgs;
	PtSetArg(&args[nArgs], Pt_ARG_PG_CURRENT_INDEX, 1, 0); ++nArgs;
	PtSetResources(_panelGroup, nArgs, args);

	if(actState) {
		char *str;

		str = actState->name();
		PtSetResource(ABW_stateNameText, Pt_ARG_TEXT_STRING, str, 0);
		str = actState->ecode();
		PtSetResource(ABW_ecodeText, Pt_ARG_TEXT_STRING, str, 0);
		str = actState->action();
		PtSetResource(ABW_actionText, Pt_ARG_TEXT_STRING, str, 0);
		str = actState->runLineLevel();
		PtSetResource(ABW_runningLineText, Pt_ARG_TEXT_STRING, str, 0);
		str = actState->time();
		PtSetResource(ABW_timeText, Pt_ARG_TEXT_STRING, str, 0);
		str = actState->rand();
		PtSetResource(ABW_randText, Pt_ARG_TEXT_STRING, str, 0);
		str = actState->comment();
		PtSetResource(ABW_stateCommentText, Pt_ARG_TEXT_STRING, str, 0);
	}
	else {
		PtSetResource(ABW_stateNameText, Pt_ARG_TEXT_STRING, "", 0);
		PtSetResource(ABW_ecodeText, Pt_ARG_TEXT_STRING, "", 0);
		PtSetResource(ABW_actionText, Pt_ARG_TEXT_STRING, "", 0);
		PtSetResource(ABW_runningLineText, Pt_ARG_TEXT_STRING, "", 0);
		PtSetResource(ABW_timeText, Pt_ARG_TEXT_STRING, "", 0);
		PtSetResource(ABW_randText, Pt_ARG_TEXT_STRING, "", 0);
		PtSetResource(ABW_stateCommentText, Pt_ARG_TEXT_STRING, "", 0);
	}
}

void
rebelResource::setActionArgLabels(char *actName)
{
	PtSetResource(ABW_arg1Label, Pt_ARG_TEXT_STRING, "", 0);
	PtSetResource(ABW_arg2Label, Pt_ARG_TEXT_STRING, "", 0);
	PtSetResource(ABW_arg3Label, Pt_ARG_TEXT_STRING, "", 0);
	PtSetResource(ABW_arg4Label, Pt_ARG_TEXT_STRING, "", 0);
	PtSetResource(ABW_arg5Label, Pt_ARG_TEXT_STRING, "", 0);
	PtSetResource(ABW_arg6Label, Pt_ARG_TEXT_STRING, "", 0);
	PtSetResource(ABW_arg7Label, Pt_ARG_TEXT_STRING, "", 0);
	PtSetResource(ABW_arg8Label, Pt_ARG_TEXT_STRING, "", 0);
	PtSetResource(ABW_arg9Label, Pt_ARG_TEXT_STRING, "", 0);
	PtSetResource(ABW_arg10Label, Pt_ARG_TEXT_STRING, "", 0);

	if(!actName) return;

	char argList[1024];
	strcpy(argList, app->protoType(actName).c_str());

	if(!strcmp(argList, "-1")) return;

	// get the first argument
	int n = 1;
	char *tok;
	if((tok = strtok(argList, ",")) == (char *)NULL) return;
	do {
		while(isspace((int)(*tok))) ++tok;

		switch(n) {
		case 1:
			PtSetResource(ABW_arg1Label, Pt_ARG_TEXT_STRING, tok, 0);
			break;
		case 2:
			PtSetResource(ABW_arg2Label, Pt_ARG_TEXT_STRING, tok, 0);
			break;
		case 3:
			PtSetResource(ABW_arg3Label, Pt_ARG_TEXT_STRING, tok, 0);
			break;
		case 4:
			PtSetResource(ABW_arg4Label, Pt_ARG_TEXT_STRING, tok, 0);
			break;
		case 5:
			PtSetResource(ABW_arg5Label, Pt_ARG_TEXT_STRING, tok, 0);
			break;
		case 6:
			PtSetResource(ABW_arg6Label, Pt_ARG_TEXT_STRING, tok, 0);
			break;
		case 7:
			PtSetResource(ABW_arg7Label, Pt_ARG_TEXT_STRING, tok, 0);
			break;
		case 8:
			PtSetResource(ABW_arg8Label, Pt_ARG_TEXT_STRING, tok, 0);
			break;
		case 9:
			PtSetResource(ABW_arg9Label, Pt_ARG_TEXT_STRING, tok, 0);
			break;
		case 10:
			PtSetResource(ABW_arg10Label, Pt_ARG_TEXT_STRING, tok, 0);
			break;
		}
		++n;
	} while((tok = strtok(NULL, ",")) != (char *)NULL);
}

void
rebelResource::setEscapeWidgets(ESCAPE *ptr)
{
	rebelState *fromState = (rebelState *)NULL;

	if(ptr) PtGetResource(ptr->ePath, Pt_ARG_POINTER, &fromState, 0);

	PtArg_t args[2];
	int nArgs = 0;
	PtSetArg(&args[nArgs], Pt_ARG_PG_CURRENT_INDEX, 2, 0); ++nArgs;
	PtSetArg(&args[nArgs], Pt_ARG_POINTER, fromState, 0); ++nArgs;
	PtSetResources(_panelGroup, nArgs, args);

	PtWidget_t *escpPnl = PtPGFindPanelByTitle(_panelGroup, "Escape");
	PtSetResource(escpPnl, Pt_ARG_POINTER, ptr, 0);

	if(ptr) {
		PtSetResource(ABW_escapeFromText, Pt_ARG_TEXT_STRING, fromState->name(), 0);
		PtSetResource(ABW_escapeToText, Pt_ARG_TEXT_STRING, ptr->toState->name(), 0);
		PtSetResource(ABW_escapeOnText, Pt_ARG_TEXT_STRING, ptr->escapeOn, 0);
		PtSetResource(ABW_escapeCommentText, Pt_ARG_TEXT_STRING, ptr->comment, 0);
	}
	else {
		PtSetResource(ABW_escapeFromText, Pt_ARG_TEXT_STRING, "", 0);
		PtSetResource(ABW_escapeToText, Pt_ARG_TEXT_STRING, "", 0);
		PtSetResource(ABW_escapeOnText, Pt_ARG_TEXT_STRING, "", 0);
		PtSetResource(ABW_escapeCommentText, Pt_ARG_TEXT_STRING, "", 0);
	}
}

void
rebelResource::setMenuWidgets(rebelMenu *actMenu)
{
	PtArg_t(args[2]);
	int nArgs = 0;
	PtSetArg(&args[nArgs], Pt_ARG_POINTER, actMenu, 0); ++nArgs;
	PtSetArg(&args[nArgs], Pt_ARG_PG_CURRENT_INDEX, 3, 0); ++nArgs;
	PtSetResources(_panelGroup, nArgs, args);

	if(actMenu) {
		char *str;
  
		str = actMenu->name();
		PtSetResource(ABW_menuNameText, Pt_ARG_TEXT_STRING, str, 0);
		str = actMenu->vlistPtr();
		PtSetResource(ABW_menuVariableListText, Pt_ARG_TEXT_STRING, str, 0);
		str = actMenu->basePtr();
		PtSetResource(ABW_menuBasePtrText, Pt_ARG_TEXT_STRING, str, 0);
		str = actMenu->maf();
		PtSetResource(ABW_menuAccessFuncText, Pt_ARG_TEXT_STRING, str, 0);
		str = actMenu->aff();
		PtSetResource(ABW_menuAccessFlagText, Pt_ARG_TEXT_STRING, str, 0);
		str = actMenu->agf();
		PtSetResource(ABW_menuRootGenText, Pt_ARG_TEXT_STRING, str, 0);
		str = actMenu->helpName();
		PtSetResource(ABW_menuHelpNameText, Pt_ARG_TEXT_STRING, str, 0);
		str = actMenu->help();
		PtSetResource(ABW_menuHelpMultiText, Pt_ARG_TEXT_STRING, str, 0);
	}
	else {
		PtSetResource(ABW_menuNameText, Pt_ARG_TEXT_STRING, "", 0);
		PtSetResource(ABW_menuVariableListText, Pt_ARG_TEXT_STRING, "", 0);
		PtSetResource(ABW_menuBasePtrText, Pt_ARG_TEXT_STRING, "NP", 0);
		PtSetResource(ABW_menuAccessFuncText, Pt_ARG_TEXT_STRING, "NP", 0);
		PtSetResource(ABW_menuAccessFlagText, Pt_ARG_TEXT_STRING, "0", 0);
		PtSetResource(ABW_menuRootGenText, Pt_ARG_TEXT_STRING, "NP", 0);
		PtSetResource(ABW_menuHelpNameText, Pt_ARG_TEXT_STRING, "hm_null", 0);
		PtSetResource(ABW_menuHelpMultiText, Pt_ARG_TEXT_STRING, "", 0);
	}
}

void
rebelResource::setVlistWidgets(rebelMenuItem *actItem)
{
	PtArg_t(args[2]);
	int nArgs = 0;
	PtSetArg(&args[nArgs], Pt_ARG_POINTER, actItem, 0); ++nArgs;
	PtSetArg(&args[nArgs], Pt_ARG_PG_CURRENT_INDEX, 4, 0); ++nArgs;
	PtSetResources(_panelGroup, nArgs, args);

	if(actItem) {
		char *str;
  
		str = actItem->name();
		PtSetResource(ABW_vlistNameText, Pt_ARG_TEXT_STRING, str, 0);
		str = actItem->variable();
		PtSetResource(ABW_vlistVariableText, Pt_ARG_TEXT_STRING, str, 0);
		str = actItem->basePtr();
		PtSetResource(ABW_vlistBasePtrText, Pt_ARG_TEXT_STRING, str, 0);
		str = actItem->vaf();
		PtSetResource(ABW_vlistAccessFuncText, Pt_ARG_TEXT_STRING, str, 0);
		str = actItem->aff();
		PtSetResource(ABW_vlistAccessFlagText, Pt_ARG_TEXT_STRING, str, 0);
		str = actItem->varType();
		PtSetResource(ABW_vlistTypeText, Pt_ARG_TEXT_STRING, str, 0);
	}
	else {
		PtSetResource(ABW_vlistNameText, Pt_ARG_TEXT_STRING, "", 0);
		PtSetResource(ABW_vlistVariableText, Pt_ARG_TEXT_STRING, "", 0);
		PtSetResource(ABW_vlistBasePtrText, Pt_ARG_TEXT_STRING, "", 0);
		PtSetResource(ABW_vlistAccessFuncText, Pt_ARG_TEXT_STRING, "", 0);
		PtSetResource(ABW_vlistAccessFlagText, Pt_ARG_TEXT_STRING, "", 0);
		PtSetResource(ABW_vlistTypeText, Pt_ARG_TEXT_STRING, "", 0);
	}
}

void
rebelResource::setFunctionWidgets(rebelFunctionItem *actItem)
{
	PtArg_t(args[2]);
	int nArgs = 0;
	PtSetArg(&args[nArgs], Pt_ARG_POINTER, actItem, 0); ++nArgs;
	PtSetArg(&args[nArgs], Pt_ARG_PG_CURRENT_INDEX, 5, 0); ++nArgs;
	PtSetResources(_panelGroup, nArgs, args);

	if(actItem) {
		char *str;
		str = actItem->name();
		PtSetResource(ABW_functionLabelText, Pt_ARG_TEXT_STRING, str, 0);
		str = actItem->function();
		PtSetResource(ABW_functionNameText, Pt_ARG_TEXT_STRING, str, 0);
		str = actItem->format();
		PtSetResource(ABW_functionFormatText, Pt_ARG_TEXT_STRING, str, 0);
	}
	else {
		PtSetResource(ABW_functionLabelText, Pt_ARG_TEXT_STRING, "", 0);
		PtSetResource(ABW_functionNameText, Pt_ARG_TEXT_STRING, "", 0);
		PtSetResource(ABW_functionFormatText, Pt_ARG_TEXT_STRING, "", 0);
	}
}

void
rebelResource::setVariableWidgets(rebelVariableItem *actItem)
{
	PtArg_t(args[2]);
	int nArgs = 0;
	PtSetArg(&args[nArgs], Pt_ARG_POINTER, actItem, 0); ++nArgs;
	PtSetArg(&args[nArgs], Pt_ARG_PG_CURRENT_INDEX, 6, 0); ++nArgs;
	PtSetResources(_panelGroup, nArgs, args);

	if(actItem) {
		char *str;
		str = actItem->name();
		PtSetResource(ABW_variableNameText, Pt_ARG_TEXT_STRING, str, 0);
		str = actItem->variable();
		PtSetResource(ABW_variableVariableText, Pt_ARG_TEXT_STRING, str, 0);
	}
	else {
		PtSetResource(ABW_variableNameText, Pt_ARG_TEXT_STRING, "", 0);
		PtSetResource(ABW_variableVariableText, Pt_ARG_TEXT_STRING, "", 0);
	}
}

void
rebelResource::setCommentWidgets(rebelComment *actCmnt)
{
	PtArg_t(args[2]);
	int nArgs = 0;
	PtSetArg(&args[nArgs], Pt_ARG_POINTER, actCmnt, 0); ++nArgs;
	PtSetArg(&args[nArgs], Pt_ARG_PG_CURRENT_INDEX, 7, 0); ++nArgs;
	PtSetResources(_panelGroup, nArgs, args);

	if(actCmnt) {
		char *str = actCmnt->comment();
		PtSetResource(ABW_commentMultiText, Pt_ARG_TEXT_STRING, str, 0);
	}
	else {
		PtSetResource(ABW_commentMultiText, Pt_ARG_TEXT_STRING, "", 0);
	}

}

int
rebelResource::switchCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	return( Pt_END );

}


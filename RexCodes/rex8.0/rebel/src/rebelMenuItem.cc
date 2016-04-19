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
extern rebelEdit rEdit;

rebelMenuItem::rebelMenuItem()
{
	_butType = REBEL_MENU_ITEM;

	strcpy(_rexItem.name, "");
	strcpy(_rexItem.variable, "NP");
	strcpy(_rexItem.basePtr, "NP");
	strcpy(_rexItem.vaf, "NP");
	strcpy(_rexItem.aff, "0");
	strcpy(_rexItem.type, "ME_DEC");

	_button = (PtWidget_t *)NULL;
	PhPoint_t p = { 0, 0 };
	_origin = p;

}

rebelMenuItem::rebelMenuItem(PtWidget_t *parent, const char *name, PhPoint_t pos) : rebelButton(name, pos)
{
	extern ApDBase_t *rPictures;

	_butType = REBEL_MENU_ITEM;

	strcpy(_rexItem.name, name);
	if(_rexItem.name[0] == '#') {
		strcpy(_rexItem.variable, "DONT_USE");
		strcpy(_rexItem.basePtr, "DONT_USE");
		strcpy(_rexItem.vaf, "DONT_USE");
		strcpy(_rexItem.aff, "DONT_USE");
		strcpy(_rexItem.type, "DONT_USE");
	}
	else {
		strcpy(_rexItem.variable, "NP");
		strcpy(_rexItem.basePtr, "NP");
		strcpy(_rexItem.vaf, "NP");
		strcpy(_rexItem.aff, "0");
		strcpy(_rexItem.type, "ME_DEC");
	}

	PtArg_t arg;
	PtSetArg(&arg, Pt_ARG_TEXT_STRING, name, 0);
	_button = ApCreateDBWidget(rPictures, "rebelItemButton", parent, &_origin, 1, &arg);
	PtRealizeWidget(_button);

	PtSetResource(_button, Pt_ARG_POINTER, this, 0);
}

rebelMenuItem::rebelMenuItem(rebelMenuItem& mi)
{
	_butType = mi._butType;
	_rexItem = mi._rexItem;
	_button = mi._button;
	_origin = mi._origin;
}

rebelMenuItem::~rebelMenuItem()
{
	if(_rexItem.name[0] != '#') {
		if(strcmp(_rexItem.vaf, "NP")) {
			app->deleteAction(_rexItem.vaf);
		}
		if(strcmp(_rexItem.variable, "NP")) {
			app->deleteGlobalVariable(_rexItem.variable);
		}
	}
}

rebelMenuItem& rebelMenuItem::operator=(const rebelMenuItem& mi)
{
	_butType = mi._butType;
	_rexItem = mi._rexItem;
	_button = mi._button;
	_origin = mi._origin;

	return(*this);
}

int
rebelMenuItem::resourceCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// get a pointer to the chain instance containing this state
	PtWidget_t *win = ApGetInstance(widget);
	rebelMenu *menu;
	PtGetResource(win, Pt_ARG_POINTER, &menu, 0);

	// get a pointer to this state instance
	rebelMenuItem *item;
	PtGetResource(widget, Pt_ARG_POINTER, &item, 0);

	// get the pointer event type
	PhPointerEvent_t *pe;
	pe = (PhPointerEvent_t *)PhGetData(cbinfo->event);

	// if no modifier keys were pressed, set the resource panel
	if(!(pe->key_mods & (KEYMOD_SHIFT | KEYMOD_CTRL | KEYMOD_ALT))) {
		if(item) item->resource(widget, apinfo, cbinfo);
		if(menu) menu->ungroupButtons();
	}

	// if modifier key was pressed, add the widget to the group
	else {
		if(menu) menu->groupButton(item, pe->key_mods);
	}

	return( Pt_CONTINUE );

}

void
rebelMenuItem::resource( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	extern rebelResource *rResources;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	rResources->setVlistWidgets(this);

	return;

}

void
rebelMenuItem::cut( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	char text[128];
	sprintf(text, "Do you really want to cut menu item %s?", _rexItem.name);
	char *btns[] = { "Cancel", "Cut" };
	int answer = PtAlert(ABW_rebelBaseWindow, NULL,
						 "Cut State", NULL,
						 text, NULL,
						 2, (char const **)btns, NULL,
						 1, 1, Pt_LEFT | Pt_MODAL);
	switch(answer) {
	case 1:
		return;
		break;
	case 2:
		PtWidget_t *window;
		if((window = ApGetInstance(_button)) != NULL) {
			rebelWin *win;
			PtGetResource(window, Pt_ARG_POINTER, &win, 0);
			if(win) win->cutButton(this);
		}
		break;
	}
}

int
rebelMenuItem::itemNameCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// don't execute if the user hasn't changed the value
	PgColor_t *fillColor;
	PtGetResource(widget, Pt_ARG_FILL_COLOR, &fillColor, 0);
	if(*fillColor != Pg_DCYAN) return( Pt_CONTINUE );

	// get a pointer to this state instance
	PtWidget_t *w = ApGetInstance(widget);
	rebelResource *rsrc;
	PtGetResource(w, Pt_ARG_POINTER, &rsrc, 0);
	if(rsrc) {
		PtWidget_t *pg = rsrc->panelGroup();
		rebelMenuItem *item;
		PtGetResource(pg, Pt_ARG_POINTER, &item, 0);

		// call this state instance of the stateName function
		if(item) item->itemName(widget, apinfo, cbinfo);
	}

	// reset the colors indicating that the input has been processed
	PtArg_t args[2];
	int nArgs = 0;
	PtSetArg(&args[nArgs], Pt_ARG_COLOR, Pg_BLACK, 0); ++nArgs;
	PtSetArg(&args[nArgs], Pt_ARG_FILL_COLOR, Pg_WHITE, 0); ++nArgs;
	PtSetResources(widget, nArgs, args);

	return( Pt_CONTINUE );

}

void
rebelMenuItem::itemName( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	char *str;
	PtGetResource(widget, Pt_ARG_TEXT_STRING, &str, 0);
	PtSetResource(_button, Pt_ARG_TEXT_STRING, str, 0);

	strcpy(_name, str);
	strcpy(_rexItem.name, str);
	if(_rexItem.name[0] == '#') {
		strcpy(_rexItem.variable, "DONT_USE");
		strcpy(_rexItem.basePtr, "DONT_USE");
		strcpy(_rexItem.vaf, "DONT_USE");
		strcpy(_rexItem.aff, "DONT_USE");
		strcpy(_rexItem.type, "DONT_USE");
		this->resource(widget, apinfo, cbinfo);
	}

	app->saved(false);

	return;

}

int
rebelMenuItem::varTypeCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// determine what type if widget called this function
	if(PtWidgetClass(widget) == PtComboBox) {
		// get a pointer to this item instance
		PtWidget_t *w = ApGetInstance(widget);
		rebelResource *rsrc;
		PtGetResource(w, Pt_ARG_POINTER, &rsrc, 0);
		if(rsrc) {
			PtWidget_t *pg = rsrc->panelGroup();
			rebelMenuItem *item;
			PtGetResource(pg, Pt_ARG_POINTER, &item, 0);

			// call this state instance of the stateName function
			if(item && (cbinfo->reason_subtype == Pt_LIST_SELECTION_FINAL)) {
				item->varType(widget, apinfo, cbinfo);
			}
		}
	}
	else if(PtWidgetClass(widget) == PtText) {
		// don't execute if the user hasn't changed the value
		PgColor_t *fillColor;
		PtGetResource(widget, Pt_ARG_FILL_COLOR, &fillColor, 0);
		if(*fillColor != Pg_DCYAN) return( Pt_CONTINUE );

		// get a pointer to this state instance
		PtWidget_t *w = ApGetInstance(widget);
		rebelResource *rsrc;
		PtGetResource(w, Pt_ARG_POINTER, &rsrc, 0);
		if(rsrc) {
			PtWidget_t *pg = rsrc->panelGroup();
			rebelMenuItem *item;
			PtGetResource(pg, Pt_ARG_POINTER, &item, 0);

			// call this state instance of the stateName function
			if(item) item->varType(widget, apinfo, cbinfo);
		}

		// reset the colors indicating that the input has been processed
		PtArg_t args[2];
		int nArgs = 0;
		PtSetArg(&args[nArgs], Pt_ARG_COLOR, Pg_BLACK, 0); ++nArgs;
		PtSetArg(&args[nArgs], Pt_ARG_FILL_COLOR, Pg_WHITE, 0); ++nArgs;
		PtSetResources(widget, nArgs, args);
	}
	return( Pt_CONTINUE );

}

void
rebelMenuItem::varType( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// determine what widget called this function
	if(PtWidgetClass(widget) == PtComboBox) {
		PtListCallback_t *lstData = (PtListCallback_t *)cbinfo->cbdata;
		strcpy(_rexItem.type, lstData->item);
	}
	else if(PtWidgetClass(widget) == PtText) {
		char *str;
		PtGetResource(widget, Pt_ARG_TEXT_STRING, &str, 0);
		strcpy(_rexItem.type, str);
	}

	app->saved(false);

	return;

}

int
rebelMenuItem::variableCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// don't execute if the user hasn't changed the value
	PgColor_t *fillColor;
	PtGetResource(widget, Pt_ARG_FILL_COLOR, &fillColor, 0);
	if(*fillColor != Pg_DCYAN) return( Pt_CONTINUE );

	// get a pointer to this state instance
	PtWidget_t *w = ApGetInstance(widget);
	rebelResource *rsrc;
	PtGetResource(w, Pt_ARG_POINTER, &rsrc, 0);
	if(rsrc) {
		PtWidget_t *pg = rsrc->panelGroup();
		rebelMenuItem *item;
		PtGetResource(pg, Pt_ARG_POINTER, &item, 0);

		// call this state instance of the stateName function
		if(item) item->variable(widget, apinfo, cbinfo);
	}

	// reset the colors indicating that the input has been processed
	PtArg_t args[2];
	int nArgs = 0;
	PtSetArg(&args[nArgs], Pt_ARG_COLOR, Pg_BLACK, 0); ++nArgs;
	PtSetArg(&args[nArgs], Pt_ARG_FILL_COLOR, Pg_WHITE, 0); ++nArgs;
	PtSetResources(widget, nArgs, args);

	return( Pt_CONTINUE );

}

void
rebelMenuItem::variable( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	char *str;
	PtGetResource(widget, Pt_ARG_TEXT_STRING, &str, 0);
	if(strcmp(_rexItem.type, "ME_STR")) {
		if(str[0] != '&') {
			_rexItem.variable[0] = '&';
			strcpy(&_rexItem.variable[1], str);
		}
	}

	strcpy(_rexItem.variable, str);


	// don't make a global variable declaration for items that are structure members
	string tmp = _rexItem.variable;
	if(tmp.find("NP)->") != string::npos) return;
	else if(tmp.find(".") != string::npos) return;
	else if(!strcmp(_rexItem.type, "ME_FLOAT")) {
		app->addGlobalVariables("float", _rexItem.variable);
	}
	else if(!strcmp(_rexItem.type, "ME_STR")) {
		app->addGlobalVariables("char", _rexItem.variable, "[]");
	}
	else if(!strcmp(_rexItem.type, "ME_ACHAR")) {
		app->addGlobalVariables("char", _rexItem.variable);
	}
	else app->addGlobalVariables("int", _rexItem.variable);

	app->saved(false);

	return;

}

int
rebelMenuItem::basePtrCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// don't execute if the user hasn't changed the value
	PgColor_t *fillColor;
	PtGetResource(widget, Pt_ARG_FILL_COLOR, &fillColor, 0);
	if(*fillColor != Pg_DCYAN) return( Pt_CONTINUE );

	// get a pointer to this state instance
	PtWidget_t *w = ApGetInstance(widget);
	rebelResource *rsrc;
	PtGetResource(w, Pt_ARG_POINTER, &rsrc, 0);
	if(rsrc) {
		PtWidget_t *pg = rsrc->panelGroup();
		rebelMenuItem *item;
		PtGetResource(pg, Pt_ARG_POINTER, &item, 0);

		// call this state instance of the stateName function
		if(item) item->basePtr(widget, apinfo, cbinfo);
	}

	// reset the colors indicating that the input has been processed
	PtArg_t args[2];
	int nArgs = 0;
	PtSetArg(&args[nArgs], Pt_ARG_COLOR, Pg_BLACK, 0); ++nArgs;
	PtSetArg(&args[nArgs], Pt_ARG_FILL_COLOR, Pg_WHITE, 0); ++nArgs;
	PtSetResources(widget, nArgs, args);

	return( Pt_CONTINUE );

}

void
rebelMenuItem::basePtr( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	char *str;
	PtGetResource(widget, Pt_ARG_TEXT_STRING, &str, 0);
	strcpy(_rexItem.basePtr, str);

	app->saved(false);

	return;

}

int
rebelMenuItem::vafCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// don't execute if the user hasn't changed the value
	PgColor_t *fillColor;
	PtGetResource(widget, Pt_ARG_FILL_COLOR, &fillColor, 0);
	if(*fillColor != Pg_DCYAN) return( Pt_CONTINUE );

	// get a pointer to this state instance
	PtWidget_t *w = ApGetInstance(widget);
	rebelResource *rsrc;
	PtGetResource(w, Pt_ARG_POINTER, &rsrc, 0);
	if(rsrc) {
		PtWidget_t *pg = rsrc->panelGroup();
		rebelMenuItem *item;
		PtGetResource(pg, Pt_ARG_POINTER, &item, 0);

		// call this state instance of the stateName function
		if(item) item->vaf(widget, apinfo, cbinfo);
	}

	// reset the colors indicating that the input has been processed
	PtArg_t args[2];
	int nArgs = 0;
	PtSetArg(&args[nArgs], Pt_ARG_COLOR, Pg_BLACK, 0); ++nArgs;
	PtSetArg(&args[nArgs], Pt_ARG_FILL_COLOR, Pg_WHITE, 0); ++nArgs;
	PtSetResources(widget, nArgs, args);

	return( Pt_CONTINUE );

}

void
rebelMenuItem::vaf( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	char *str;
	PtGetResource(widget, Pt_ARG_TEXT_STRING, &str, 0);
	strcpy(_rexItem.vaf, str);

	string prototype = "int ";
	prototype += _rexItem.vaf;
	prototype += "(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd)";
	app->addFunction(_rexItem.vaf, prototype);

	app->saved(false);

	return;

}

int
rebelMenuItem::accessFlagCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// determine what type of widget called this function
	if(PtWidgetClass(widget) == PtComboBox) {
		// get a pointer to this item instance
		PtWidget_t *w = ApGetInstance(widget);
		rebelResource *rsrc;
		PtGetResource(w, Pt_ARG_POINTER, &rsrc, 0);
		if(rsrc) {
			PtWidget_t *pg = rsrc->panelGroup();
			rebelMenuItem *item;
			PtGetResource(pg, Pt_ARG_POINTER, &item, 0);

			// call this state instance of the stateName function
			if(item && (cbinfo->reason_subtype == Pt_LIST_SELECTION_FINAL)) {
				item->accessFlag(widget, apinfo, cbinfo);
			}
		}
	}
	else if(PtWidgetClass(widget) == PtText) {
		// don't execute if the user hasn't changed the value
		PgColor_t *fillColor;
		PtGetResource(widget, Pt_ARG_FILL_COLOR, &fillColor, 0);
		if(*fillColor != Pg_DCYAN) return( Pt_CONTINUE );

		// get a pointer to this item instance
		PtWidget_t *w = ApGetInstance(widget);
		rebelResource *rsrc;
		PtGetResource(w, Pt_ARG_POINTER, &rsrc, 0);
		if(rsrc) {
			PtWidget_t *pg = rsrc->panelGroup();
			rebelMenuItem *item;
			PtGetResource(pg, Pt_ARG_POINTER, &item, 0);

			// call this state instance of the stateName function
			if(item) item->accessFlag(widget, apinfo, cbinfo);
		}

		// reset the colors indicating that the input has been processed
		PtArg_t args[2];
		int nArgs = 0;
		PtSetArg(&args[nArgs], Pt_ARG_COLOR, Pg_BLACK, 0); ++nArgs;
		PtSetArg(&args[nArgs], Pt_ARG_FILL_COLOR, Pg_WHITE, 0); ++nArgs;
		PtSetResources(widget, nArgs, args);
	}

	return( Pt_CONTINUE );

}

void
rebelMenuItem::accessFlag( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// determine what widget called this function
	if(PtWidgetClass(widget) == PtComboBox) {
		PtListCallback_t *lstData = (PtListCallback_t *)cbinfo->cbdata;
		if(!strcmp(_rexItem.aff, "0")) {
			strcpy(_rexItem.aff, lstData->item);
		}
		else if(!strcmp(lstData->item, "0")) {
			strcpy(_rexItem.aff, lstData->item);
		}
		else {
			string temp = _rexItem.aff;
			temp += "|";
			temp += lstData->item;
			strcpy(_rexItem.aff, temp.c_str());
		}
	}
	else if(PtWidgetClass(widget) == PtText) {
		char *str;
		PtGetResource(widget, Pt_ARG_TEXT_STRING, &str, 0);
		strcpy(_rexItem.aff, str);
	}

	app->saved(false);

	return;

}

void
rebelMenuItem::saveButton(FILE *fp)
{
}

void
rebelMenuItem::genCode(FILE *fp)
{
	if(_rexItem.name[0] == '#') {
		fprintf(fp, "%s // %hd, %hd\n",
				_rexItem.name, _origin.x, _origin.y);
	}
	else {
		fprintf(fp, "\t{\"%s\",\t%s, %s, %s, %s, %s}, // %hd %hd\n",
				_rexItem.name, _rexItem.variable, _rexItem.basePtr,
				_rexItem.vaf, _rexItem.aff, _rexItem.type,
				_origin.x, _origin.y);
	}

	return;
}

void
rebelMenuItem::portButton(rebelWin *w, string line)
{
	string tmp = line;

	// get the name of the variable item
	if(line.find("&") != string::npos) {
		w->extractString(line, tmp, "&", ",");
		if(!tmp.empty()) {
			strcpy(_rexItem.variable, "&");
			strcat(_rexItem.variable, tmp.c_str());
		}
	}
	else {
		w->extractString(line, tmp, ",", ",");

		// get rid of leading white space
		size_t i = 0;
		while(isalpha(tmp[i]) == false) {
			++i;
		}
		if(i != 0) tmp.erase(0, i);

		strcpy(_rexItem.variable, tmp.c_str());
	}
											

	// the name of the VLIST item has already been used
	// in creating the class instance  so get rid of it
	tmp = line;
	size_t np;
	if((np = tmp.find(",")) != string::npos) tmp.erase(0, np);

	// convert punctuation to spaces for sscanf
	for(size_t i = 0; i < tmp.size(); ++i) {
		if((tmp[i] == '{') || (tmp[i] == '\"') || (tmp[i] == '}') || (tmp[i] == ',')) {
			tmp[i] = ' ';
		}
	}
	
	sscanf(tmp.c_str(), "%s %s %s %s",
		   _rexItem.basePtr, _rexItem.vaf,
		   _rexItem.aff, _rexItem.type);

	// if there is a vaf function, add it to the prototype list
	if(strcmp(_rexItem.vaf, "NP")) {
		string prototype = "int ";
		prototype += _rexItem.vaf;
		prototype += "(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd)";
		app->addFunction(_rexItem.vaf, prototype);
	}

	// don't make a global variable declaration for items that are structure members
	tmp = _rexItem.variable;
	if(tmp.find("->") != string::npos) return;
	else if (tmp.find(".") != string::npos) return;
	else if(!strcmp(_rexItem.type, "ME_FLOAT")) {
		app->addGlobalVariables("float", _rexItem.variable);
	}
	else if(!strcmp(_rexItem.type, "ME_STR")) {
		app->addGlobalVariables("char", _rexItem.variable, "[]");
	}
	else if(!strcmp(_rexItem.type, "ME_ACHAR")) {
		app->addGlobalVariables("char", _rexItem.variable);
	}
	else {
		app->addGlobalVariables("int", _rexItem.variable);
	}

	return;
}

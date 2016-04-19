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

rebelVariableItem::rebelVariableItem()
{
	_butType = REBEL_VARIABLE_ITEM;

	_button = (PtWidget_t *)NULL;
	PhPoint_t p = { 0, 0 };
	_origin = p;

}

rebelVariableItem::rebelVariableItem(PtWidget_t *parent, const char *name, PhPoint_t pos) : rebelButton(name, pos)
{
	extern ApDBase_t *rPictures;

	_butType = REBEL_VARIABLE_ITEM;

	if(_name[0] == '#') {
		strcpy(_variable, "DONT_USE");
	}
	else {
		strcpy(_variable, "");
	}
	PtArg_t arg;
	PtSetArg(&arg, Pt_ARG_TEXT_STRING, name, 0);
	_button = ApCreateDBWidget(rPictures, "rebelVariableButton", parent, &_origin, 1, &arg);
	PtRealizeWidget(_button);

	PtSetResource(_button, Pt_ARG_POINTER, this, 0);
}

rebelVariableItem::rebelVariableItem(rebelVariableItem& mi)
{
	_butType = mi._butType;

	_button = mi._button;
	_origin = mi._origin;
}

rebelVariableItem::~rebelVariableItem()
{
	if((_name[0] != '#') &&(strlen(_variable))) {
		app->deleteGlobalVariable(_variable);
	}
}

rebelVariableItem& rebelVariableItem::operator=(const rebelVariableItem& mi)
{
	_butType = mi._butType;

	_button = mi._button;
	_origin = mi._origin;

	return(*this);
}

int
rebelVariableItem::resourceCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// get a pointer to the chain instance containing this state
	PtWidget_t *win = ApGetInstance(widget);
	rebelVariable *var;
	PtGetResource(win, Pt_ARG_POINTER, &var, 0);

	// get a pointer to this state instance
	rebelVariableItem *item;
	PtGetResource(widget, Pt_ARG_POINTER, &item, 0);

	// get the pointer event type
	PhPointerEvent_t *pe;
	pe = (PhPointerEvent_t *)PhGetData(cbinfo->event);

	// if no modifier keys were pressed, set the resource panel
	if(!(pe->key_mods & (KEYMOD_SHIFT | KEYMOD_CTRL | KEYMOD_ALT))) {
		if(item) item->resource(widget, apinfo, cbinfo);
		if(var) var->ungroupButtons();
	}

	// if modifier key was pressed, add the widget to the group
	else {
		if(var) var->groupButton(item, pe->key_mods);
	}

	return( Pt_CONTINUE );

}

void
rebelVariableItem::resource( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	extern rebelResource *rResources;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	rResources->setVariableWidgets(this);

	return;

}

void
rebelVariableItem::cut( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	char text[128];
	sprintf(text, "Do you really want to cut variable item %s?", _name);
	char *btns[] = { "Cancel", "Cut" };
	int answer = PtAlert(ABW_rebelBaseWindow, NULL,
						 "Cut Variable", NULL,
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
rebelVariableItem::variableNameCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
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
		rebelVariableItem *item;
		PtGetResource(pg, Pt_ARG_POINTER, &item, 0);

		// call this state instance of the stateName function
		if(item) item->variableName(widget, apinfo, cbinfo);
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
rebelVariableItem::variableName( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	char *str;
	PtGetResource(widget, Pt_ARG_TEXT_STRING, &str, 0);
	PtSetResource(_button, Pt_ARG_TEXT_STRING, str, 0);

	strcpy(_name, str);
	if(_name[0] == '#') {
		strcpy(_variable, "DONT_USE");
		this->resource(widget, apinfo, cbinfo);
	}

	app->saved(false);

	return;

}

int
rebelVariableItem::variableCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
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
		rebelVariableItem *item;
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
rebelVariableItem::variable( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	char *str;
	PtGetResource(widget, Pt_ARG_TEXT_STRING, &str, 0);
	if(str[0] == '&') strcpy(_variable, str);
	else {
		strcpy(_variable, "&");
		strcat(_variable, str);
	}

	app->addGlobalVariables("int", &_variable[1]);

	app->saved(false);

	return;

}


void
rebelVariableItem::saveButton(FILE *fp)
{
}


void
rebelVariableItem::portButton(rebelWin *w, string line)
{
	string tmp = line;

	// the name of the variable item has already been used
	// in creating the class instance  so get rid of it
	size_t np;
	if((np = tmp.find("&")) != string::npos) tmp.erase(0, np);

	// convert punctuation to spaces for sscanf
	for(size_t i = 0; i < tmp.size(); ++i) {
		if((tmp[i] == '{') || (tmp[i] == '\"') || (tmp[i] == '}') || (tmp[i] == ',')) {
			tmp[i] = ' ';
		}
	}

	sscanf(tmp.c_str(), "%s", _variable);

	app->addGlobalVariables("int", &_variable[1]);

	return;
}


void
rebelVariableItem::genCode(FILE *fp)
{
	if(_name[0] == '#') {
		fprintf(fp, "%s // %hd %hd\n",
				_name, _origin.x, _origin.y);
	}
	else {
		fprintf(fp, "\t{\"%s\",\t%s}, // %hd %hd\n",
				_name, _variable, _origin.x, _origin.y);
	}

	return;
}

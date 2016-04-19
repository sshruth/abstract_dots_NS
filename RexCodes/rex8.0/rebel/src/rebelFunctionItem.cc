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

rebelFunctionItem::rebelFunctionItem()
{
	_butType = REBEL_VARIABLE_ITEM;
}

rebelFunctionItem::rebelFunctionItem(PtWidget_t *parent, const char *name, PhPoint_t pos) : rebelButton(name, pos)
{
	extern ApDBase_t *rPictures;

	_butType = REBEL_VARIABLE_ITEM;

	if(_name[0] == '#') {
		strcpy(_function, "DONT_USE");
		strcpy(_format, "DONT_USE");
	}
	else {
		strcpy(_function, "");
		strcpy(_format, "void");
	}
	PtArg_t arg;
	PtSetArg(&arg, Pt_ARG_TEXT_STRING, name, 0);
	_button = ApCreateDBWidget(rPictures, "rebelFunctionButton", parent, &_origin, 1, &arg);
	PtRealizeWidget(_button);

	PtSetResource(_button, Pt_ARG_POINTER, this, 0);
}

rebelFunctionItem::rebelFunctionItem(rebelFunctionItem& mi)
{
	_butType = mi._butType;

	strcpy(_name, mi._name);
	strcpy(_function, mi._function);
	strcpy(_format, mi._format);

	_button = mi._button;
	_origin = mi._origin;
}

rebelFunctionItem::~rebelFunctionItem()
{
	if((_name[0] != '#') && (strlen(_function))) {
		app->deleteAction(_function);
	}
}

rebelFunctionItem& rebelFunctionItem::operator=(const rebelFunctionItem& mi)
{
	_butType = mi._butType;

	strcpy(_name, mi._name);
	strcpy(_function, mi._function);
	strcpy(_format, mi._format);

	_button = mi._button;
	_origin = mi._origin;

	return(*this);
}

int
rebelFunctionItem::resourceCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// get a pointer to the chain instance containing this state
	PtWidget_t *win = ApGetInstance(widget);
	rebelFunction *func;
	PtGetResource(win, Pt_ARG_POINTER, &func, 0);

	// get a pointer to this state instance
	rebelButton *b;
	PtGetResource(widget, Pt_ARG_POINTER, &b, 0);

	// get the pointer event type
	PhPointerEvent_t *pe;
	pe = (PhPointerEvent_t *)PhGetData(cbinfo->event);

	// if no modifier keys were pressed, set the resource panel
	if(!(pe->key_mods & (KEYMOD_SHIFT | KEYMOD_CTRL | KEYMOD_ALT))) {
		if(b) b->resource(widget, apinfo, cbinfo);
		if(func) func->ungroupButtons();
	}

	// if modifier key was pressed, add the widget to the group
	else {
		if(func) func->groupButton(b, pe->key_mods);
	}

	return( Pt_CONTINUE );

}

void
rebelFunctionItem::resource( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	extern rebelResource *rResources;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	rResources->setFunctionWidgets(this);

	return;

}


void
rebelFunctionItem::cut( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	char text[128];
	sprintf(text, "Do you really want to cut function item %s?", _name);
	char *btns[] = { "Cancel", "Cut" };
	int answer = PtAlert(ABW_rebelBaseWindow, NULL,
						 "Cut Function", NULL,
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

	return;
}


int
rebelFunctionItem::functionNameCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// don't execute if the user hasn't changed the value
	PgColor_t *fillColor;
	PtGetResource(widget, Pt_ARG_FILL_COLOR, &fillColor, 0);
	if(*fillColor != Pg_DCYAN) return( Pt_CONTINUE );

	// get a pointer to this function instance
	PtWidget_t *w = ApGetInstance(widget);
	rebelResource *rsrc;
	PtGetResource(w, Pt_ARG_POINTER, &rsrc, 0);
	if(rsrc) {
		PtWidget_t *pg = rsrc->panelGroup();
		rebelFunctionItem *item;
		PtGetResource(pg, Pt_ARG_POINTER, &item, 0);

		// call this state instance of the stateName function
		if(item) item->functionName(widget, apinfo, cbinfo);
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
rebelFunctionItem::functionName( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	char *str;
	PtGetResource(widget, Pt_ARG_TEXT_STRING, &str, 0);
	PtSetResource(_button, Pt_ARG_TEXT_STRING, str, 0);

	strcpy(_name, str);
	if(_name[0] == '#') {
		strcpy(_function, "DONT_USE");
		strcpy(_format, "DONT_USE");
		this->resource(widget, apinfo, cbinfo);
	}

	app->saved(false);

	return;

}


int
rebelFunctionItem::functionCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
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
		rebelFunctionItem *item;
		PtGetResource(pg, Pt_ARG_POINTER, &item, 0);

		// call this state instance of the stateName function
		if(item) item->function(widget, apinfo, cbinfo);
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
rebelFunctionItem::function( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// if the user is changing a function name
	// delete the old function
	if(strlen(_function)) {
		app->deleteAction(_function);
	}

	char *str;
	PtGetResource(widget, Pt_ARG_TEXT_STRING, &str, 0);
	if(str[0] == '&') strcpy(_function, &str[1]);
	else strcpy(_function, str);

	if(strlen(_format)) {
		string proto = "void ";
		proto += _function;
		proto += "(";
		proto += _format;
		proto += ")";
		app->addFunction(_function, proto);
	}

	app->saved(false);

	return;

}


int
rebelFunctionItem::formatCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
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
		rebelFunctionItem *item;
		PtGetResource(pg, Pt_ARG_POINTER, &item, 0);

		// call this state instance of the stateName function
		if(item) item->format(widget, apinfo, cbinfo);
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
rebelFunctionItem::format( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	char *str;
	PtGetResource(widget, Pt_ARG_TEXT_STRING, &str, 0);
	while(isspace((int)(*str))) ++str;
	string f = str;

	// if the user used quotes, get rid of them
	if(f[0] == '"') {
		f.erase(0, 1);
		size_t quotePos;
		if((quotePos = f.find("\"")) != string::npos) f.erase(quotePos);
	}
	strcpy(_format, f.c_str());
	if(strlen(_function)) {
		// if the user is changing a function format
		// delete the old function
		app->deleteAction(_function);

		string proto = "void ";
		proto += _function;
		proto += "(";
		proto += _format;
		proto += ")";
		app->addFunction(_function, proto);
	}
	app->saved(false);

	return;

}


void
rebelFunctionItem::genCode(FILE *fp)
{
	if(_name[0] == '#') {
		fprintf(fp, "%s // %hd %hd\n",
				_name, _origin.x, _origin.y);
	}
	else {
		fprintf(fp, "\t{\"%s\",\t&%s, \"%s\"}, // %hd %hd\n",
				_name, _function, _format, _origin.x, _origin.y);
	}

	return;
}

void
rebelFunctionItem::saveButton(FILE *fp)
{
}

void
rebelFunctionItem::portButton(rebelWin *w, string line)
{
	string tmp;

	// the name of the Function item has already been used
	// in creating the class instance  so get rid of it
	size_t np;
	if((np = line.find("&")) != string::npos) line.erase(0, np);

	// extract the name of the function
	// if the user provided a format string
	if(line.find("\"") != string::npos) {
		w->extractString(line, tmp, "&", ",");
		if(!tmp.empty()) sscanf(tmp.c_str(), "%s", _function);

		// extract the format string
		w->extractString(line, tmp, "\"", "\"");
		if(!tmp.empty()) strcpy(_format, tmp.c_str());
		else strcpy(_format, "void");
	}
	else if(line.find("\"") == string::npos) {
		w->extractString(line, tmp, "&", "}");
		if(!tmp.empty()) sscanf(tmp.c_str(), "%s", _function);
		strcpy(_format, "void");
	}

	// build a function prototype
	string proto = "void ";
	proto += _function;
	proto += "(";

	// if the format string is NOT void
	if(strcmp(_format, "void")) {
		char form[32];
		vector<char> fmstr;
		for(int i = 0; i < 32; ++i) {
			if(isalpha((int)_format[i])) {
				if(_format[i] == 'd') {
					sprintf(form, "int, v%d", i);
					fmstr.insert(fmstr.end(), form, (form + strlen(form)));
				}
				else if(_format[i] == 'f') {
					sprintf(form, "float, v%d", i);
					fmstr.insert(fmstr.end(), form, (form + strlen(form)));
				}
				else if(_format[i] == 'l') {
					if(_format[i + 1] == 'd') {
						sprintf(form, "long, v%d", i);
						fmstr.insert(fmstr.end(), form, (form + strlen(form)));
					}
					else if(_format[i + 1] == 'f') {
						sprintf(form, "double, v%d", i);
						fmstr.insert(fmstr.end(), form, (form +  strlen(form)));
					}
					++i;
				}
			}
		}
		fmstr.pop_back();
		fmstr.pop_back();
		vector<char>::iterator m;
		for(m = fmstr.begin(); m != fmstr.end(); ++m) {
			proto.push_back(*m);
		}
	}
	else proto += _format;
	proto += ")";

	app->addFunction(_function, proto);

	return;
}

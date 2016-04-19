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
extern rebelEdit rEdit;

vector<string> rebelMenu::_helpList;

rebelMenu::rebelMenu() : rebelWin()
{
	_winTyp = REBEL_MENU;

	strcpy(_rexMenu.name, "");
	strcpy(_rexMenu.vlistPtr, "");
	strcpy(_rexMenu.basePtr, "NP");
	strcpy(_rexMenu.maf, "NP");
	strcpy(_rexMenu.aff, "0");
	strcpy(_rexMenu.agf, "NP");
	_rexMenu.help.clear();
}

rebelMenu::rebelMenu(const char *n, PtCallbackInfo_t *cbinfo) : rebelWin(n)
{
	_winTyp = REBEL_MENU;

	PhDim_t s =  { 150, 350 };
	_prefSize = s;

	cbinfo->cbdata = (void *)this;
	ApModuleParent(ABM_rebelWindow, AB_PARENT, ABW_rebelBkgd);
	ApModuleLocation(ABM_rebelWindow, AB_LOC_REL_MODULE, _nextOrigin.x, _nextOrigin.y);
	_window = ApCreateModule(ABM_rebelWindow, NULL, cbinfo);

	strncpy(_rexMenu.name, n, 32);
	strcpy(_rexMenu.vlistPtr, n);
	strcat(_rexMenu.vlistPtr, "_vl");
	strcpy(_rexMenu.basePtr, "NP");
	strcpy(_rexMenu.maf, "NP");
	strcpy(_rexMenu.aff, "0");
	strcpy(_rexMenu.agf, "NP");
	strcpy(_rexMenu.helpName, "hm_null");
	_rexMenu.help.clear();
}

rebelMenu::rebelMenu(const char *n, PhPoint_t origin, PhDim_t size, PtCallbackInfo_t *cbinfo) : rebelWin(n, size)
{
	_winTyp = REBEL_MENU;

	cbinfo->cbdata = (void *)this;
	ApModuleParent(ABM_rebelWindow, AB_PARENT, ABW_rebelBkgd);
	ApModuleLocation(ABM_rebelWindow, AB_LOC_REL_MODULE, origin.x, origin.y);
	_window = ApCreateModule(ABM_rebelWindow, NULL, cbinfo);

	strncpy(_rexMenu.name, n, 32);
	strcpy(_rexMenu.vlistPtr, n);
	strcat(_rexMenu.vlistPtr, "_vl");
	strcpy(_rexMenu.basePtr, "NP");
	strcpy(_rexMenu.maf, "NP");
	strcpy(_rexMenu.aff, "0");
	strcpy(_rexMenu.agf, "NP");
	strcpy(_rexMenu.helpName, "hm_null");
	_rexMenu.help.clear();
}

rebelMenu::rebelMenu(const rebelMenu& rm) : rebelWin(rm)
{
	_rexMenu = rm._rexMenu;

}

rebelMenu::~rebelMenu()
{
	if(strlen(_rexMenu.maf)) {
		app->deleteAction(_rexMenu.maf);
	}
	if(strlen(_rexMenu.agf)) {
		app->deleteAction(_rexMenu.agf);
	}
}

rebelMenu& rebelMenu::operator=(const rebelMenu& rm)
{
	_winTyp = rm._winTyp;
	strcpy(_name, rm._name);

	_rexMenu = rm._rexMenu;

	_addButton = rm._addButton;

	_window = rm._window;
	_scrollContainer = rm._scrollContainer;
	_scrollCanvas = rm._scrollCanvas;
	_scrollCanvasExtent = rm._scrollCanvasExtent;
	_draggedWidget = rm._draggedWidget;

	_origin = rm._origin;
	_size = rm._size;

	return(*this);
}


void rebelMenu::cut( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	char text[128];
	sprintf(text, "Do you really want to cut menu %s?", _rexMenu.name);
	char *btns[] = { "Cancel", "Cut" };
	int answer = PtAlert(ABW_rebelBaseWindow, NULL,
						 "Cut Menu", NULL,
						 text, NULL,
						 2, (char const **)btns, NULL,
						 1, 1, Pt_LEFT | Pt_MODAL);
	switch(answer) {
	case 1:
		return;
		break;
	case 2:
		app->cutWindow(this);
		break;
	}
}


void
rebelMenu::addButton( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// the base class function selects this window and
	// gets a position for the new button if one is to be created
	rebelWin::addButton(widget, apinfo, cbinfo);

	// create a button to represent the new menu item
	int n = _buttons.size();
	char defName[64];
	sprintf(defName, "%s%d", _rexMenu.name, n);
	rebelMenuItem *item = new rebelMenuItem(_scrollCanvas, defName, _newPos);
	_buttons.push_back(item);

	// mark the newly created button as selected.
	rEdit.selectObject(item->button());
	_activeBtnPtr = _buttons.end() - 1;

	// if the control key is not depressed, get out of state adding mode
	PhPointerEvent_t *pe;
	pe = (PhPointerEvent_t *)PhGetData(cbinfo->event);
	if(!(pe->key_mods & KEYMOD_CTRL)) {
		PtSetResource(widget, Pt_ARG_CURSOR_TYPE, Ph_CURSOR_INHERIT, 0);
		_addButton = false;
	}

	item->resource(widget, apinfo, cbinfo);

	app->saved(false);

	return;

}


void
rebelMenu::clearHelpList()
{
	_helpList.clear();
}

int
rebelMenu::newItemCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(app == (rebelApp *)NULL) return( Pt_CONTINUE );

	// get pointer to this menu instance
	rebelWin *w = app->activeWindow();
	if(w == (rebelWin *)NULL) return( Pt_CONTINUE );
	if(w->winTyp() != REBEL_MENU) return( Pt_CONTINUE );

	w->newButton(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );

}


int
rebelMenu::menuNameCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// don't execute if the user hasn't changed the value
	PgColor_t *fillColor;
	PtGetResource(widget, Pt_ARG_FILL_COLOR, &fillColor, 0);
	if(*fillColor != Pg_DCYAN) return( Pt_CONTINUE );

	// get a pointer to this menu instance
	PtWidget_t *w = ApGetInstance(widget);
	rebelResource *rsrc;
	PtGetResource(w, Pt_ARG_POINTER, &rsrc, 0);
	if(rsrc) {
		PtWidget_t *pg = rsrc->panelGroup();
		rebelMenu *menu;
		PtGetResource(pg, Pt_ARG_POINTER, &menu, 0);

		// call this menu instance of the menuName function
		if(menu) menu->menuName(widget, apinfo, cbinfo);
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
rebelMenu::menuName( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	char *str;
	PtGetResource(widget, Pt_ARG_TEXT_STRING, &str, 0);
	strcpy(_name, str);

	char title[64];
	strcpy(title, "MENU : ");
	strcat(title, _name);
	PtSetResource(_window, Pt_ARG_WINDOW_TITLE, title, 0);

	strcpy(_rexMenu.name, str);

	app->saved(false);

	return;

}


int
rebelMenu::variableListCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// don't execute if the user hasn't changed the value
	PgColor_t *fillColor;
	PtGetResource(widget, Pt_ARG_FILL_COLOR, &fillColor, 0);
	if(*fillColor != Pg_DCYAN) return( Pt_CONTINUE );

	// get a pointer to this menu instance
	PtWidget_t *w = ApGetInstance(widget);
	rebelResource *rsrc;
	PtGetResource(w, Pt_ARG_POINTER, &rsrc, 0);
	if(rsrc) {
		PtWidget_t *pg = rsrc->panelGroup();
		rebelMenu *menu;
		PtGetResource(pg, Pt_ARG_POINTER, &menu, 0);

		// call this menu instance of the stateName function
		if(menu) menu->variableList(widget, apinfo, cbinfo);
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
rebelMenu::variableList( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	char *str;
	PtGetResource(widget, Pt_ARG_TEXT_STRING, &str, 0);
	strcpy(_rexMenu.vlistPtr, str);

	return;

}


int
rebelMenu::basePtrCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// don't execute if the user hasn't changed the value
	PgColor_t *fillColor;
	PtGetResource(widget, Pt_ARG_FILL_COLOR, &fillColor, 0);
	if(*fillColor != Pg_DCYAN) return( Pt_CONTINUE );

	// get a pointer to this menu instance
	PtWidget_t *w = ApGetInstance(widget);
	rebelResource *rsrc;
	PtGetResource(w, Pt_ARG_POINTER, &rsrc, 0);
	if(rsrc) {
		PtWidget_t *pg = rsrc->panelGroup();
		rebelMenu *menu;
		PtGetResource(pg, Pt_ARG_POINTER, &menu, 0);

		// call this menu instance of the stateName function
		if(menu) menu->basePtr(widget, apinfo, cbinfo);
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
rebelMenu::basePtr( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	char *str;
	PtGetResource(widget, Pt_ARG_TEXT_STRING, &str, 0);
	strcpy(_rexMenu.basePtr, str);

	return;

}


int
rebelMenu::mafCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// don't execute if the user hasn't changed the value
	PgColor_t *fillColor;
	PtGetResource(widget, Pt_ARG_FILL_COLOR, &fillColor, 0);
	if(*fillColor != Pg_DCYAN) return( Pt_CONTINUE );

	// get a pointer to this menu instance
	PtWidget_t *w = ApGetInstance(widget);
	rebelResource *rsrc;
	PtGetResource(w, Pt_ARG_POINTER, &rsrc, 0);
	if(rsrc) {
		PtWidget_t *pg = rsrc->panelGroup();
		rebelMenu *menu;
		PtGetResource(pg, Pt_ARG_POINTER, &menu, 0);

		// call this menu instance of the stateName function
		if(menu) menu->maf(widget, apinfo, cbinfo);
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
rebelMenu::maf( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	char *str;
	PtGetResource(widget, Pt_ARG_TEXT_STRING, &str, 0);
	strcpy(_rexMenu.maf, str);

	string prototype = "int ";
	prototype += _rexMenu.maf;
	prototype += "(int flag, MENU *mp, char *astr, ME_RECUR *rp)";

	app->addFunction(_rexMenu.maf, prototype);

	app->saved(false);

	return;

}


int
rebelMenu::accessFlagCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
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
			rebelMenu *menu;
			PtGetResource(pg, Pt_ARG_POINTER, &menu, 0);

			// call this menu instance of the stateName function
			if(menu && (cbinfo->reason_subtype == Pt_LIST_SELECTION_FINAL)) {
				menu->accessFlag(widget, apinfo, cbinfo);
			}
		}
	}
	else if(PtWidgetClass(widget) == PtText) {
		// don't execute if the user hasn't changed the value
		PgColor_t *fillColor;
		PtGetResource(widget, Pt_ARG_FILL_COLOR, &fillColor, 0);
		if(*fillColor != Pg_DCYAN) return( Pt_CONTINUE );

		// get a pointer to this menu instance
		PtWidget_t *w = ApGetInstance(widget);
		rebelResource *rsrc;
		PtGetResource(w, Pt_ARG_POINTER, &rsrc, 0);
		if(rsrc) {
			PtWidget_t *pg = rsrc->panelGroup();
			rebelMenu *menu;
			PtGetResource(pg, Pt_ARG_POINTER, &menu, 0);

			// call this menu instance of the stateName function
			if(menu) menu->accessFlag(widget, apinfo, cbinfo);
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
rebelMenu::accessFlag( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// determine what widget called this function
	if(PtWidgetClass(widget) == PtComboBox) {
		PtListCallback_t *lstData = (PtListCallback_t *)cbinfo->cbdata;
		strcpy(_rexMenu.aff, lstData->item);
	}
	else if(PtWidgetClass(widget) == PtText) {
		char *str;
		PtGetResource(widget, Pt_ARG_TEXT_STRING, &str, 0);
		strcpy(_rexMenu.aff, str);
	}

	return;

}


int
rebelMenu::agfCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// don't execute if the user hasn't changed the value
	PgColor_t *fillColor;
	PtGetResource(widget, Pt_ARG_FILL_COLOR, &fillColor, 0);
	if(*fillColor != Pg_DCYAN) return( Pt_CONTINUE );

	// get a pointer to this menu instance
	PtWidget_t *w = ApGetInstance(widget);
	rebelResource *rsrc;
	PtGetResource(w, Pt_ARG_POINTER, &rsrc, 0);
	if(rsrc) {
		PtWidget_t *pg = rsrc->panelGroup();
		rebelMenu *menu;
		PtGetResource(pg, Pt_ARG_POINTER, &menu, 0);

		// call this menu instance of the stateName function
		if(menu) menu->agf(widget, apinfo, cbinfo);
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
rebelMenu::agf( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	char *str;
	PtGetResource(widget, Pt_ARG_TEXT_STRING, &str, 0);
	strcpy(_rexMenu.agf, str);

	string prototype = "int ";
	prototype += _rexMenu.maf;
	prototype += "(int call_cnt, MENU *mp, char *astr)";

	app->addFunction(_rexMenu.agf, prototype);

	app->saved(false);

	return;

}


int
rebelMenu::helpNameCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// don't execute if the user hasn't changed the value
	PgColor_t *fillColor;
	PtGetResource(widget, Pt_ARG_FILL_COLOR, &fillColor, 0);
	if(*fillColor != Pg_DCYAN) return( Pt_CONTINUE );

	// get a pointer to this menu instance
	PtWidget_t *w = ApGetInstance(widget);
	rebelResource *rsrc;
	PtGetResource(w, Pt_ARG_POINTER, &rsrc, 0);
	if(rsrc) {
		PtWidget_t *pg = rsrc->panelGroup();
		rebelMenu *menu;
		PtGetResource(pg, Pt_ARG_POINTER, &menu, 0);

		// call this menu instance of the stateName function
		if(menu) menu->helpName(widget, apinfo, cbinfo);
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
rebelMenu::helpName( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	char *str;
	PtGetResource(widget, Pt_ARG_TEXT_STRING, &str, 0);
	strcpy(_rexMenu.helpName, str);

	// if the user choose the null help message, delete the current message
	if(!strcmp(_rexMenu.helpName, "hm_null")) {
		PtSetResource(ABW_menuHelpMultiText, Pt_ARG_TEXT_STRING, "", 0);
	}

	return;

}


int
rebelMenu::helpCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	
	// get a pointer to this menu item instance
	PtWidget_t *w = ApGetInstance(widget);
	rebelResource *rsrc;
	PtGetResource(w, Pt_ARG_POINTER, &rsrc, 0);
	if(rsrc) {
		PtWidget_t *pg = rsrc->panelGroup();
		rebelMenu *menu;
		PtGetResource(pg, Pt_ARG_POINTER, &menu, 0);

		// call this menu item instance's help function
		if(menu) menu->help(widget, apinfo, cbinfo);
	}

	return( Pt_CONTINUE );

}

void
rebelMenu::help( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	char *str;
	PtGetResource(ABW_menuHelpMultiText, Pt_ARG_TEXT_STRING, &str, 0);

	_rexMenu.help.clear();
	_rexMenu.help.insert(_rexMenu.help.end(), str, (str + strlen(str)));
	_rexMenu.help.push_back('\0');

	// if the help menu name is hm_null, set a new default
	if(!strcmp(_rexMenu.helpName, "hm_null")) {
		strcpy(_rexMenu.helpName, "hm_");
		strcat(_rexMenu.helpName, _rexMenu.name);
		PtSetResource(ABW_menuHelpNameText, Pt_ARG_TEXT_STRING, _rexMenu.helpName, 0);
	}
	return;

}


void
rebelMenu::saveWindow(FILE *fp)
{
	// save the name of the chain
	fprintf(fp, "MENU %s\n", _name);

	// save the descriptions of the comments
	vector<rebelButton *>::iterator bi;
	for(bi = _buttons.begin(); bi != _buttons.end(); ++bi) {
		if((*bi)->butType() == REBEL_COMMENT) {
			(*bi)->saveButton(fp);
		}
	}
	fprintf(fp, "MENU_DONE\n");
}

void rebelMenu::portWindow(rebelApp *app, string first, string second)
{
	string tmp = first;

	// the name of the menu has already been used in creating the class instance
	// so get rid of it
	size_t np;
	if((np = tmp.find("&")) != string::npos) tmp.erase(0, (np + 1));

	// convert punctuation to spaces for sscanf
	for(size_t i = 0; i < tmp.size(); ++i) {
		if((tmp[i] == '{') || (tmp[i] == '\"') || (tmp[i] == '}') || (tmp[i] == ',')) {
			tmp[i] = ' ';
		}
	}

	sscanf(tmp.c_str(), "%s %s %s %s %s %s",
		   _rexMenu.vlistPtr, _rexMenu.basePtr, _rexMenu.maf,
		   _rexMenu.aff, _rexMenu.agf, _rexMenu.helpName);

	if(strcmp(_rexMenu.maf, "NP")) {
		string prototype = "int ";
		prototype += _rexMenu.maf;
		prototype += "(int flag, MENU *mp, char *astr, ME_RECUR *rp)";
		app->addFunction(_rexMenu.maf, prototype);
	}

	if(strcmp(_rexMenu.agf, "NP")) {
		string prototype = "int ";
		prototype += _rexMenu.agf;
		prototype += "(int call_cnt, MENU *mp, char *astr)";
		app->addFunction(_rexMenu.agf, prototype);
	}

	// port the vlist array
	portVlist(app);

	// port the help message
	portHelp(app);

	// read the properties to get the comments
	readWindow();

	// set the active button pointer for this menu
	_activeBtnPtr = _buttons.begin();

	return;

}


void
rebelMenu::portVlist(rebelApp *app)
{
	PhPoint_t o;
	string tmp;

	// read through the source to find the variable list array
	// for this menu
	// make a copy of the file for lineTok to mangle
	_sourceTemp.clear();
	_sourceTemp.insert(_sourceTemp.end(), app->srcBeg(), app->srcEnd());
	_sourceTemp.push_back('\0');

	string line;
	char *tok;
	if((tok = _lineTok->tok(&_sourceTemp[0])) == (char *)NULL) return;
	do {
		line = tok;
		if((line.find("VLIST") != string::npos) &&
		   (line.find(_rexMenu.vlistPtr) != string::npos) &&
		   (line.find("=") != string::npos)) {
			o.x = 0;
			o.y = 0;
			break;
		}
	} while((tok = _lineTok->tok((char *)NULL)) != (char *)NULL);

	// scan the VLIST array for this menu to get the items
	bool inComment = false;
	while((tok = _lineTok->tok((char *)NULL)) != (char *)NULL) {
		line = tok;

		// handle multi line comments
		if((line.find("/*") != string::npos) &&
		   (line.find("*/") == string::npos)) {
			inComment = true;

			if(line[0] == '/') continue;
		}
		else if(inComment == true) {
			if(line.find("*/") != string::npos) inComment = false;
			continue;
		}

		// skip lines that have been commented out
		if(inComment == false) {
			if(line[0] == '/') continue;
		}

		// break if this is the vlist terminating string
		if(line.find("NS") != string::npos) break;

		// get the position of the button for apps developed with Rebel
		string temp = line;
		size_t slashPos;
		if((slashPos = temp.find("//")) != string::npos) {
			temp.erase(0, (slashPos + 2));
			sscanf(temp.c_str(), "%hd %hd", &o.x, &o.y);
		}

		// if this line is a preprocessor directive,
		// create a special preprocessor menu item
		rebelMenuItem *item;
		string name;
		if(line[0] == '#') {
			size_t nlPos;
			if((nlPos = line.find("\n")) != string::npos) line.erase(nlPos);
			item = new rebelMenuItem(_scrollCanvas, line.c_str(), o);
			_buttons.push_back(item);
		}
		// otherwise create a regular menu item.
		else {
			_lineTok->extractString(line, tmp, "\"", "\"");
			if(!tmp.empty()) {
				item = new rebelMenuItem(_scrollCanvas, tmp.c_str(), o);
				item->portButton((rebelWin *)this, line);
				_buttons.push_back(item);
			}
		}
		o = getNextCell(o);
	}

	// set the active button pointer for this chain
	_activeBtnPtr = _buttons.begin();
}


void
rebelMenu::portHelp(rebelApp *app)
{
	// read through the source to find the help message for this menu
	// make a copy of the file for lineTok to mangle
	_sourceTemp.clear();
	_sourceTemp.insert(_sourceTemp.end(), app->srcBeg(), app->srcEnd());
	_sourceTemp.push_back('\0');

	string line;
	char *tok;
	if((tok = _lineTok->tok(&_sourceTemp[0])) == (char *)NULL) return;
	do {
		line = tok;
		if(islower(line[0]) &&
		   (line.find("char") != string::npos) &&
		   (line.find(_rexMenu.helpName) != string::npos) &&
		   (line.find("=") != string::npos)) {
			// return if this is an empty help message
			if(line.find(";") != string::npos) return;
			else break;
		}
	} while((tok = _lineTok->tok((char *)NULL)) != (char *)NULL);

	_rexMenu.help.clear();
	while((tok = _lineTok->tok((char *)NULL)) != (char *)NULL) {
		line = tok;

		// check for the end of the help message
		size_t colonPos;
		if((colonPos = line.find("\";")) != string::npos) {
			line.erase(colonPos);
			if(line.size()) {
				_rexMenu.help.insert(_rexMenu.help.end(), line.begin(), line.end());
			}
			break;
		}
		else if((colonPos = line.find("\"")) != string::npos) {
			line.erase(colonPos);
			if(line.size()) {
				_rexMenu.help.insert(_rexMenu.help.end(), line.begin(), line.end());
			}
			break;
		}
		else if(line[0] == ';') {
			break;
		}
		else if((colonPos = line.find("\\")) != string::npos) {
			line.erase(colonPos);
			if(line.size()) {
				_rexMenu.help.insert(_rexMenu.help.end(), line.begin(), line.end());
			}
		}
		else {
			_rexMenu.help.insert(_rexMenu.help.end(), line.begin(), line.end());
		}
	}

	_rexMenu.help.push_back('\0');

	return;
}

void
rebelMenu::genVarLists(FILE *fp)
{
	fprintf(fp, "VLIST %s[] = {\n", _rexMenu.vlistPtr);

	// sort the items in the menu
	stable_sort(_buttons.begin(), _buttons.end(), buttonLessThan<rebelButton *>());

	vector<rebelButton *>::iterator bi;
	for(bi = _buttons.begin(); bi != _buttons.end(); ++bi) {
		rebelMenuItem *mi = (rebelMenuItem *)(*bi);
		mi->genCode(fp);
	}
	fprintf(fp, "\tNS\n");
	fprintf(fp, "};\n");

	vector<string>::iterator hi;
	for(hi = _helpList.begin(); hi != _helpList.end(); ++hi) {
		if(hi->find(_rexMenu.helpName) != string::npos) return;
	}
	if(!strcmp(_rexMenu.helpName, "hm_null")) return;

	// define the help message if it is not the null message
	if(!_rexMenu.help.empty()) {
		fprintf(fp, "char %s[] = \"\n%s", _rexMenu.helpName, &_rexMenu.help[0]);
		fprintf(fp, "\";\n");
	}
	else {
		fprintf(fp, "char %s[] = \"\";\n", _rexMenu.helpName);
	}

	_helpList.push_back(_rexMenu.helpName);

}

void
rebelMenu::genMenuArray(FILE *fp)
{
	PhPoint_t o = this->origin();
	PhDim_t s = this->size();
	fprintf(fp, "\t{\"%s\",\t&%s, %s, %s, %s, %s, %s}, // %hd, %hd, %hd, %hd, %ld\n",
			_rexMenu.name, _rexMenu.vlistPtr, _rexMenu.basePtr,
			_rexMenu.maf, _rexMenu.aff, _rexMenu.agf, _rexMenu.helpName,
			o.x, o.y, s.w, s.h, PtWidgetIsRealized(_window));
}

void
rebelMenu::readWindow()
{
	rebelComment *cmnt = (rebelComment *)NULL;
	PhPoint_t o;


	// skip down the buffer to the beginning of the properties
	string line;
	char *tok;
	if((tok = _lineTok->tok((char *)NULL)) == (char *)NULL) return;
	do {
		line = tok;
		if(line.find("/**********************************") != string::npos) {
			break;
		}
	} while((tok = _lineTok->tok((char *)NULL)) != (char *)NULL);

	
	// now find the description of this menu
	while((tok = _lineTok->tok((char *)NULL)) != (char *)NULL) {
		line = tok;

		if((line.find("MENU") != string::npos) &&
		   (line.find(_name) != string::npos)) {
			break;
		}
	}

	// now process the comments for this menu
	while((tok = _lineTok->tok((char *)NULL)) != (char *)NULL) {
		line = tok;

		// done with this menu
		if(line.find("MENU_DONE") != string::npos) return;

		// end of properties
		if(line.find(" **********************************/") != string::npos) return;

		if(line.find("COMMENT") != string::npos) {
			sscanf(line.c_str(), "COMMENT %hd %hd", &o.x, &o.y);
			cmnt = new rebelComment(_scrollCanvas, o);
		}
			
		else if(line.find("LINE") != string::npos) {
			size_t spacePos;
			if((spacePos = line.find(" ")) != string::npos) line.erase(0, (spacePos + 1));
			cmnt->readButton(line);
		}
		else if(line.find("DONE") != string::npos) {
			cmnt->readButton("NL");
			_buttons.push_back(cmnt);
		}
	}
}

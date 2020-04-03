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
#include "rexActionList.h"
#include "rexIncludeList.h"

extern PhPoint_t rOrigin;
extern PhDim_t rSize;
extern PhDim_t bkgdSize;
extern PhRect_t bkgdBoundary;
extern PhRid_t rRid;

extern rebelFile rFile;
extern rebelEdit rEdit;
extern rebelResource *rResources;
extern rebelApp *app;

rebelApp::rebelApp(PtCallbackInfo_t *cbinfo)
{
	_lineTok = new rebelLineTok;
	_saved = false;
	_generatedCode = false;
	_pdgmNum = 100;
	_prefSize.w = 0;
	_prefSize.h = 0;
	_chains.clear();
	_menus.clear();
	_function = (rebelWin *)NULL;
	_variable = (rebelWin *)NULL;
	_header = (char *)NULL;
	_source = (char *)NULL;
	_headerTemp.clear();
	_sourceTemp.clear();
	_listBuffer.clear();
	_rexIncludeList.clear();
	_rexActionList.clear();
	_labActionList.clear();
	_localActionList.clear();
	_globalVariables.clear();
	_neededGlobals.clear();
	_neededActions.clear();

	setActionList();

	app = this;

	// create a chain with the default name
	char name[32] = "main_set";
	_activeWindow = new rebelChain(name, cbinfo);

	// add the default restart function
	rebelChain *c = (rebelChain *)_activeWindow;
	c->restartFunction("rinitf");

	_chains.push_back(_activeWindow);
	_activeWindow->select();
	rResources->setChainWidgets((rebelChain *)_activeWindow);

	const char *btns[] = { "&Yes", "&No" };
	char text[32];
	sprintf(text, "%d", _pdgmNum);
	int answer = PtPrompt(ABW_rebelBkgd, NULL,
						  "Paradigm Number", NULL,
						  "Change paradigm number?", NULL,
						  2, btns, NULL, 1, 1,
						  31, text, NULL, NULL, Pt_LEFT | Pt_MODAL);

	switch(answer) {
	case 1:
		sscanf(text, "%d", &_pdgmNum);
		break;
	case 2:
		break;
	}
}

rebelApp::rebelApp(string name, PtCallbackInfo_t *cbinfo)
{
	_lineTok = new rebelLineTok;
	_saved = false;
	_generatedCode = false;
	_pdgmNum = 100;
	_prefSize.w = 0;
	_prefSize.h = 0;
	_chains.clear();
	_menus.clear();
	_function = (rebelFunction *)NULL;
	_variable = (rebelVariable *)NULL;
	_header = (char *)NULL;
	_source = (char *)NULL;
	_headerTemp.clear();
	_sourceTemp.clear();
	_listBuffer.clear();
	_rexIncludeList.clear();
	_rexActionList.clear();
	_labActionList.clear();
	_localActionList.clear();
	_globalVariables.clear();
	_neededGlobals.clear();
	_neededActions.clear();

	setActionList();

	app = this;

	// if this is a spot source file
	if(name.find(".d.c") != string::npos) portApp(name, cbinfo);
	
	// if this is a rebel source file
	else if(name.find(".r.c") != string::npos) {
		_Cname = name;

		size_t extnPos;
		_name = _Cname;
		if((extnPos = _name.find(".r")) != string::npos) {
			_name.erase(extnPos);
		}
		_Hname = _name;
		_Hname += ".r.h";

		portApp(_Cname, cbinfo);
	}
	else {
		const char *btns[] = { "&Ok" };
		char text[512];
		char message[512];
		sprintf(message, "%s is not valid for input; enter new name:", name.c_str());
		int answer = PtPrompt(ABW_rebelBkgd, NULL,
							  "Paradigm Name", NULL,
							  message, NULL,
							  1, btns, NULL, 1, 1,
							  31, text, NULL, NULL, Pt_LEFT | Pt_MODAL);

		switch(answer) {
		case 1:
			string newName = text;
			portApp(newName, cbinfo);
			break;
		}
	}

	const char *btns[] = { "&Yes", "&No" };
	char text[32];
	sprintf(text, "%d", _pdgmNum);
	int answer = PtPrompt(ABW_rebelBkgd, NULL,
						  "Paradigm Number", NULL,
						  "Change paradigm number?", NULL,
						  2, btns, NULL, 1, 1,
						  31, text, NULL, NULL, Pt_LEFT | Pt_MODAL);

	switch(answer) {
	case 1:
		sscanf(text, "%d", &_pdgmNum);
		if((_prefSize.w != 0) && (_prefSize.h != 0)) {
			PtSetResource(ABW_rebelBaseWindow, Pt_ARG_DIM, &_prefSize, 0);
		}
		break;
	case 2:
		if((_prefSize.w != 0) && (_prefSize.h != 0)) {
			PtSetResource(ABW_rebelBaseWindow, Pt_ARG_DIM, &_prefSize, 0);
		}
		break;
	}
}

rebelApp::~rebelApp()
{
	rEdit.selectObject((PtWidget_t *)NULL);
	rResources->setVariableWidgets((rebelVariableItem *)NULL);
	rResources->setFunctionWidgets((rebelFunctionItem *)NULL);
	rResources->setVlistWidgets((rebelMenuItem *)NULL);
	rResources->setMenuWidgets((rebelMenu *)NULL);
	rResources->setEscapeWidgets((ESCAPE *)NULL);
	rResources->setStateWidgets((rebelState *)NULL);
	rResources->setChainWidgets((rebelChain *)NULL);

	delete _lineTok;

	vector<rebelWin *>::iterator wi;

	for(wi = _chains.begin(); wi != _chains.end(); ++wi) delete (*wi);
	_chains.clear();

	for(wi = _menus.begin(); wi != _menus.end(); ++wi) delete (*wi);
	_menus.clear();

	if(_function) delete _function;
	if(_variable) delete _variable;
	if(_header) delete[] _header;
	if(_source) delete[] _source;

	_name.erase(_name.begin(), _name.end());
	_Hname.erase(_Hname.begin(), _Hname.end());
	_Cname.erase(_Cname.begin(), _Cname.end());
	_Dname.erase(_Dname.begin(), _Dname.end());

	_headerTemp.clear();
	_sourceTemp.clear();
	_listBuffer.clear();
	_rexIncludeList.clear();
	_rexActionList.clear();
	_labActionList.clear();
	_localActionList.clear();
	_globalVariables.clear();
	_neededGlobals.clear();
	_neededActions.clear();

	rebelWin::resetNextOrigin();
	app = (rebelApp *)NULL;
}


void
rebelApp::readPrototypes(const char *prototype, size_t size)
{
	_Pname = prototype;

	FILE *pfp;
	if((pfp = fopen(_Pname.c_str(), "r")) != NULL) {
		_prototype = new char[size + 10];
		fread(_prototype, sizeof(char), size, pfp);
		fclose(pfp);
	}
	else {
		char errorStr[256];
		sprintf(errorStr, "%s: %s", _Pname.c_str(), strerror(errno));
		PtNotice(ABW_rebelBaseWindow, NULL,
				 "File Error", NULL,
				 errorStr, NULL,
				 "&OK", NULL, Pt_MODAL);
		return;
	}
	
	// parse the prototype buffer to find the function prototypes
	_prototypeTemp.clear();
	_prototypeTemp.insert(_prototypeTemp.end(), _prototype, (_prototype + size));
	_prototypeTemp.push_back('\0');

	string thisProto;
	bool inComment = false;
	bool inProto = false;
	char *tok;
	if((tok = _lineTok->tok(&_prototypeTemp[0])) == (char *)NULL) return;
	do {
		string line = tok;

		// get rid of the trailing newline
		size_t nlPos;
		if((nlPos = line.find("\n")) != string::npos) line.erase(nlPos);

		// skip multi line comments
		if(inComment == false) {
			if((line.find("/*") != string::npos) &&
			   (line.find("*/") == string::npos)) {
				inComment = true;

				if(line[0] == '/') continue;
			}
		}
		else if(inComment == true) {
			if(line.find("*/") != string::npos) inComment = false;
			continue;
		}

		// skip single lines that have been commented out
		if(inComment == false) {
			if(line[0] == '/') continue;
		}

		// take care of the situation in which the return type is on its own line
		if(!strcmp(line.c_str(), "void") ||
		   !strcmp(line.c_str(), "int") ||
		   !strcmp(line.c_str(), "long") ||
		   !strcmp(line.c_str(), "float") ||
		   !strcmp(line.c_str(), "double"))	{
			thisProto.erase(thisProto.begin(), thisProto.end());
			thisProto = line;
			thisProto.push_back((char)32);
			inProto = true;
		}
		else if(isalpha((int)line[0]) && line.find("(")) {
			// get rid of any trailing comments
			size_t slashPos;
			if((slashPos = line.find("/")) != string::npos) line.erase(slashPos);

			thisProto.erase(thisProto.begin(), thisProto.end());
			thisProto = line;

			// check to see if the prototype continues on the next line
			if(line.find(")") == string::npos) inProto = true;
			else {
				_labActionList.push_back(thisProto);
				thisProto.erase(thisProto.begin(), thisProto.end());
			}
		}
		else if(inProto == true) {
			int i = 0;
			while(isspace((int)line[i])) ++i;
			if(i) --i;
			thisProto += &line.c_str()[i];

			// check to see if this line contains the end of the prototype
			if(line.find(")") != string::npos) {
				_labActionList.push_back(thisProto);
				thisProto.erase(thisProto.begin(), thisProto.end());
				inProto = false;
			}
		}
		
		// get the next line of source code
	} while((tok = _lineTok->tok((char *)NULL)) != (char *)NULL);
}


int
rebelApp::newChainCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(app) app->newChain(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );

}

void
rebelApp::newChain( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	const char *btns[] = { "&OK", "&Cancel" };
	char text[32] = "";
	int answer = PtPrompt(ABW_rebelBkgd, NULL,
						  "Chain Name", NULL,
						  "Enter chain name", NULL,
						  2, btns, NULL, 1, 2,
						  31, text, NULL, NULL, Pt_LEFT | Pt_MODAL);

	vector<rebelWin *>::iterator wi;
	int duplicate = 0;
	switch(answer) {
	case 1:
		for(wi = _chains.begin(); wi != _chains.end(); ++wi) {
			if(!strcmp((*wi)->name(), text)) {
				duplicate = 1;
				break;
			}
		}
		if(duplicate) {
			PtNotice(ABW_rebelBaseWindow, NULL,
					"Duplicate Chain Name", NULL,
					"This name is not unique\nSelect another", NULL,
					"&OK", NULL, Pt_MODAL);
		}
		else {
			// set the backgrounds of all other windows to light grey
			clearCursor(widget, apinfo, cbinfo);

			// make a new instance of a rebel chain class
			_activeWindow = new rebelChain(text, cbinfo);
			rResources->setChainWidgets((rebelChain *)_activeWindow);

			// add the new chain's pointer to the list of chains in this paradigm
			_chains.push_back(_activeWindow);

			// make this window the current selection
			selectWindow(_activeWindow);

			// mark this application as not saved
			_saved = false;
		}
		break;
	case 2:
		break;
	}
}


int
rebelApp::newMenuCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(app) app->newMenu(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );

}

void
rebelApp::newMenu( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	const char *btns[] = { "&OK", "&Cancel" };
	char text[32] = "";
	int answer = PtPrompt(ABW_rebelBkgd, NULL,
						  "Menu Name", NULL,
						  "Enter menu name", NULL,
						  2, btns, NULL, 1, 2,
						  31, text, NULL, NULL, Pt_LEFT | Pt_MODAL);

	vector<rebelWin *>::iterator mi;
	int duplicate = 0;
	switch(answer) {
	case 1:
		for(mi = _menus.begin(); mi != _menus.end(); ++mi) {
			if(!strcmp((*mi)->name(), text)) {
				duplicate = 1;
				break;
			}
		}
		if(duplicate) {
			PtNotice(ABW_rebelBaseWindow, NULL,
					"Duplicate Menu Name", NULL,
					"This name is not unique\nSelect another", NULL,
					"&OK", NULL, Pt_LEFT | Pt_MODAL);
		}
		else {
			// set the backgrounds of all other windows to light grey
			clearCursor(widget, apinfo, cbinfo);

			// make a new instance of a rebel menu class
			_activeWindow = new rebelMenu(text, cbinfo);

			// add the new menu's pointer to the list of menus in this paradigm
			_menus.push_back(_activeWindow);

			// make this window the current selection
			selectWindow(_activeWindow);

			// mark this application as not saved
			_saved = false;
		}
		break;
	case 2:
		break;
	}
}


int
rebelApp::newVarCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(app) app->newVar(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );

}

void
rebelApp::newVar( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(_variable) {
		PtNotice(ABW_rebelBaseWindow, NULL,
				 "Duplicate Variable Window", NULL,
				 "A variable window has already been created", NULL,
				 "&OK", NULL, Pt_MODAL);
	}
	else {
		// set the backgrounds of all other windows to light grey
		clearCursor(widget, apinfo, cbinfo);

		// make a new instance of a rebel variable class
		_activeWindow = new rebelVariable(cbinfo);

		// add the variable's pointer to this paradigm
		_variable = _activeWindow;

		// make this window the current selection
		selectWindow(_activeWindow);

		// mark this application as not saved
		_saved = false;
	}
}


int
rebelApp::newFuncCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(app) app->newFunc(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );

}

void
rebelApp::newFunc( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(_function) {
		PtNotice(ABW_rebelBaseWindow, NULL,
				 "Duplicate Function Window", NULL,
				 "A function window has already been created", NULL,
				 "&OK", NULL, Pt_MODAL);
	}
	else {
		// set the backgrounds of all other windows to light grey
		clearCursor(widget, apinfo, cbinfo);

		// make a new instance of a rebel function class
		_activeWindow = new rebelFunction(cbinfo);

		// add the function's pointer to this paradigm
		_function = _activeWindow;

		// make this window the current selection
		selectWindow(_activeWindow);

		// mark this application as not saved
		_saved = false;
	}
}


// these two functions close all of an application's open windows to clean up the workspace
int
rebelApp::hideAllCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(app) app->hideAll(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );

}

void
rebelApp::hideAll( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// hide all of the chain windows
	vector<rebelWin *>::iterator wi;
	for(wi = _chains.begin(); wi != _chains.end(); ++wi) {
		if(PtWidgetIsRealized((*wi)->window())) {
			(*wi)->closeWin(widget, apinfo, cbinfo);
		}
	}

	// hide all of the menu windows
	for(wi = _menus.begin(); wi != _menus.end(); ++wi) {
		if(PtWidgetIsRealized((*wi)->window())) {
			(*wi)->closeWin(widget, apinfo, cbinfo);
		}
	}

	// hide the user function window if it exists
	if(_function && PtWidgetIsRealized(_function->window())) {
		_function->closeWin(widget, apinfo, cbinfo);
	}

	// hide the real time variable window if it exists
	if(_variable && PtWidgetIsRealized(_variable->window())) {
		_variable->closeWin(widget, apinfo, cbinfo);
	}

	return;

}


// these two functions open all of an application's open windows
int
rebelApp::showAllCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(app) app->showAll(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );

}

void
rebelApp::showAll( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// show all of the chain windows
	vector<rebelWin *>::iterator wi;
	for(wi = _chains.begin(); wi != _chains.end(); ++wi) {
		if(!PtWidgetIsRealized((*wi)->window())) {
			(*wi)->openWin(widget, apinfo, cbinfo);
		}
	}

	// show all of the menu windows
	for(wi = _menus.begin(); wi != _menus.end(); ++wi) {
		if(!PtWidgetIsRealized((*wi)->window())) {
			(*wi)->openWin(widget, apinfo, cbinfo);
		}
	}

	// show the user function window if it exists
	if(_function && !PtWidgetIsRealized(_function->window())) {
		_function->openWin(widget, apinfo, cbinfo);
	}

	// show the real time variable window if it exists
	if(_variable && !PtWidgetIsRealized(_variable->window())) {
		_variable->openWin(widget, apinfo, cbinfo);
	}

	return;

}


// these two functions list all of an application's windows to construct the View menu
int
rebelApp::listWindowsCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(app) app->listWindows(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );

}

void
rebelApp::listWindows( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	PtWidget_t *w;
	PtArg_t args[2];
	int nArgs;
	vector<rebelWin *>::iterator wi;
	char name[64];

	// list the chain windows
	for(wi = _chains.begin(); wi != _chains.end(); ++wi) {
		if(PtWidgetIsRealized((*wi)->window())) sprintf(name, "* Chain %s", (*wi)->name());
		else sprintf(name, "  Chain %s", (*wi)->name());

		nArgs = 0;
		PtSetArg(&args[nArgs], Pt_ARG_TEXT_STRING, name, 0); ++nArgs;
		PtSetArg(&args[nArgs], Pt_ARG_POINTER, *wi, 0); ++nArgs;

		w = PtCreateWidget(PtMenuButton, Pt_DEFAULT_PARENT, nArgs, args);
		PtAddCallback(w, Pt_CB_ACTIVATE, rebelWin::openWinCallback, NULL);
	}

	// list the menu windows
	for(wi = _menus.begin(); wi != _menus.end(); ++wi) {
		if(PtWidgetIsRealized((*wi)->window())) sprintf(name, "* Menu %s", (*wi)->name());
		else sprintf(name, "  Menu %s", (*wi)->name());
		
		nArgs = 0;
		PtSetArg(&args[nArgs], Pt_ARG_TEXT_STRING, name, 0); ++nArgs;
		PtSetArg(&args[nArgs], Pt_ARG_POINTER, *wi, 0); ++nArgs;

		w = PtCreateWidget(PtMenuButton, Pt_DEFAULT_PARENT, nArgs, args);
		PtAddCallback(w, Pt_CB_ACTIVATE, rebelWin::openWinCallback, NULL);
	}

	// list the user function window if it exists
	if(_function) {
		if(PtWidgetIsRealized(_function->window())) sprintf(name, "* %s", _function->name());
		else sprintf(name, "  %s", _function->name());

		nArgs = 0;
		PtSetArg(&args[nArgs], Pt_ARG_TEXT_STRING, name, 0); ++nArgs;
		PtSetArg(&args[nArgs], Pt_ARG_POINTER, _function, 0); ++nArgs;

		w = PtCreateWidget(PtMenuButton, Pt_DEFAULT_PARENT, nArgs, args);
		PtAddCallback(w, Pt_CB_ACTIVATE, rebelWin::openWinCallback, NULL);
	}

	// list the real time variable window if it exists
	if(_variable) {
		if(PtWidgetIsRealized(_variable->window())) sprintf(name, "* %s", _variable->name());
		else sprintf(name, "  %s", _variable->name());
		nArgs = 0;
		PtSetArg(&args[nArgs], Pt_ARG_TEXT_STRING, name, 0); ++nArgs;
		PtSetArg(&args[nArgs], Pt_ARG_POINTER, _variable, 0); ++nArgs;

		w = PtCreateWidget(PtMenuButton, Pt_DEFAULT_PARENT, nArgs, args);
		PtAddCallback(w, Pt_CB_ACTIVATE, rebelWin::openWinCallback, NULL);
	}

	return;
}


// this function returns the prototype of a rex action
string
rebelApp::protoType(char *actName)
{
	string temp;
	string argList = "-1";

	// first check the rex action list
	vector<string>::iterator si;
	for(si = _rexActionList.begin(); si != _rexActionList.end(); ++si) {
		if(si->find(actName) != string::npos) {
			temp = *si;
			_lineTok->extractString(temp, argList, "(", ")");
			return(argList);
		}
	}

	// if the action is not a rex action, check the lab action list
	for(si = _labActionList.begin(); si != _labActionList.end(); ++si) {
		if(si->find(actName) != string::npos) {
			temp = *si;
			_lineTok->extractString(temp, argList, "(", ")");
			break;
		}
	}
	return(argList);
}

// this function changes the focus from one window to another
void
rebelApp::selectWindow(rebelWin *w)
{
	// deselect the old active window if there is one.
	if(_activeWindow) _activeWindow->deselect();

	// make the new window active
	_activeWindow = w;

	// select the new window
	_activeWindow->select();
}


// this function deletes a chain, menu, function, or variable window
void
rebelApp::cutWindow(rebelWin *w)
{
	_activeWindow = (rebelWin *)NULL;

	vector<rebelWin *>::iterator wi;
	for(wi = _chains.begin(); wi != _chains.end(); ++wi) {
		if(*wi == w) {
			delete *wi;
			_chains.erase(wi);
			return;
		}
	}
	for(wi = _menus.begin(); wi != _menus.end(); ++wi) {
		if(*wi == w) {
			delete *wi;
			_menus.erase(wi);
			return;
		}
	}
	if(w == _function) {
		delete _function;
		_function = (rebelWin *)NULL;
		return;
	}
	if(w == _variable) {
		delete _variable;
		_variable = (rebelWin *)NULL;
		return;
	}
}


// these two functions set the cursor to select mode
int
rebelApp::clearCursorCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(app) app->clearCursor(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );

}

void
rebelApp::clearCursor( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	vector<rebelWin *>::iterator wi;
	for(wi = _chains.begin(); wi != _chains.end(); ++wi) {
		(*wi)->deselect();
	}

	for(wi = _menus.begin(); wi != _menus.end(); ++wi) {
		(*wi)->deselect();
	}

	if(_function) {
		_function->deselect();
	}

	if(_variable) {
		_variable->deselect();
	}

	_activeWindow = (rebelWin *)NULL;
}


int
rebelApp::initActionListCallback( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	if(app) app->initActionList(link_instance, apinfo, cbinfo);

	return( Pt_CONTINUE );

}

void
rebelApp::initActionList( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	// clear the text widget
	PtSetResource(ABW_actionsMultiText, Pt_ARG_TEXT_STRING, "", 0);

	// now fill the widget with lab actions
	vector<string>::iterator ai;
	for(ai = _localActionList.begin(); ai != _localActionList.end(); ++ai) {
		char str[128];
		strcpy(str, ai->c_str());
		strcat(str, "\n");
		PtTextModifyText(ABW_actionsMultiText, 0, 0, -1, str, strlen(str));
	}

	return;

}


int
rebelApp::initGlobalListCallback( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	if(app) app->initGlobalList(link_instance, apinfo, cbinfo);

	return( Pt_CONTINUE );

}

void
rebelApp::initGlobalList( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	// clear the text widget
	PtSetResource(ABW_globalsMultiText, Pt_ARG_TEXT_STRING, "", 0);

	// now fill the widget with globals
	vector<string>::iterator ai;
	for(ai = _globalVariables.begin(); ai != _globalVariables.end(); ++ai) {
		char str[80];
		strcpy(str, ai->c_str());
		PtTextModifyText(ABW_globalsMultiText, 0, 0, -1, str, strlen(str));
	}

	return;

}


void
rebelApp::addFunction(char *funcName, string proto)
{
	if(!strcmp(funcName, "0")) return;
	if(!strcmp(funcName, "NP")) return;

	// check to see if this action is one of the rex actions
	bool found = false;
	vector<string>::iterator ai;
	for(ai = _rexActionList.begin(); ai != _rexActionList.end(); ++ai) {
		if(ai->find(funcName) != string::npos) {
			found = true;
			break;
		}
	}
	if(found == true) return;

	// if it's not a rex action, see if it is in the lab actions list
	for(ai = _labActionList.begin(); ai != _labActionList.end(); ++ai) {
		if(ai->find(funcName) != string::npos) {
			found = true;
			break;
		}
	}
	if(found == true) return;

	// if it's not a rex action, see if it is already in the local actions list
	for(ai = _localActionList.begin(); ai != _localActionList.end(); ++ai) {
		if(ai->find(funcName) != string::npos) {
			found = true;
			break;
		}
	}
	if(found == true) return;

	// if it's not in the rex, lab, or local action lists, add it to the local action list
	_localActionList.push_back(proto);
}


void
rebelApp::deleteAction(char *action)
{
	vector<string>::iterator ai;

	for(ai = _localActionList.begin(); ai != _localActionList.end(); ++ai) {
		if(ai->find(action) != string::npos) {
			_localActionList.erase(ai);
			break;
		}
	}
	
}


void
rebelApp::addGlobalVariables(char *type, char *var, char *arrayInd)
{
	string codeTemp;
	if(var[0] == '&') codeTemp = &var[1];
	else codeTemp = var;

	// get rid of array indicators, if any
	size_t brackPos;
	if((brackPos = codeTemp.find("[")) != string::npos) {
		codeTemp.erase(brackPos);
	}

	// search the current list for the variable
	bool found = false;
	vector<string>::iterator ai;
	for(ai = _globalVariables.begin(); ai != _globalVariables.end(); ++ai) {
		if(ai->find(codeTemp) != string::npos) {
			found = true;
			break;
		}
	}

	// if the variable is not in the list, add it
	if(found == false) {
		// get rid of pointer indicator, if any
		char typeTemp[16];
		strcpy(typeTemp, type);
		char *varType = strtok(typeTemp, " ");

		string globalVar = varType;
		globalVar += " ";
		globalVar += codeTemp;
		if(arrayInd[0] == '[') globalVar += arrayInd;
		globalVar += ";\n";

		_globalVariables.push_back(globalVar);
	}
}


void
rebelApp::deleteGlobalVariable(char *variable)
{
	string codeTemp;
	if(variable[0] == '&') codeTemp = &variable[1];
	else codeTemp = variable;

	vector<string>::iterator ai;
	for(ai = _globalVariables.begin(); ai != _globalVariables.end(); ++ai) {
		if(ai->find(codeTemp) != string::npos) {
			_globalVariables.erase(ai);
			break;
		}
	}
}


bool
rebelApp::saveApp(string fileName, bool spot)
{
	// save as a spot file also?
	_spot = spot;

	// this is the base file name
	_name = fileName;

	// this is the name of the header file
	_Hname = _name;
	_Hname += ".r.h";

	// this is the name of the r.c file
	_Cname = _name;
	_Cname += ".r.c";

	if(spot == true) {
		// this is the name of the spot file
		_Dname = _name;
		_Dname += ".d";

	}
	_saved = generate();

	return(_saved);
}


// this function generates the C source code file
bool
rebelApp::generate()
{
	bool st;

	// first, try to save the header file
	// if that fails, just return with an error value
	if((st = generateHeader()) == false) return(false);

	// now try to save the source file
	if((st = generateSource()) == false) return(false);

	// now save the spot file
	if(_spot == true) st = generateSpot();

	return(st);

}

bool
rebelApp::generateHeader()
{
	int fileStat = stat(_Hname.c_str(), &_HstatBuf);

	// if the header file exists, read it into a buffer
	// so that it can be checked to see what needs to be added
	if(!fileStat && (_HstatBuf.st_size > 0)) {
		FILE *hfp;
		if((hfp = fopen(_Hname.c_str(), "r")) != NULL) {
			_header = new char[_HstatBuf.st_size + 10];
			fread(_header, sizeof(char), _HstatBuf.st_size, hfp);
			fclose(hfp);
		}
		else {
			char errorStr[256];
			sprintf(errorStr, "%s: %s", _Hname.c_str(), strerror(errno));
			PtNotice(ABW_rebelBaseWindow, NULL,
					 "File Error", NULL,
					 errorStr, NULL,
					 "&OK", NULL, Pt_MODAL);
			return(false);
		}
	}
	else _HstatBuf.st_size = 0;

	// reopen the header file for writing
	FILE *hfp;
	if((hfp = fopen(_Hname.c_str(), "w")) != NULL) {
		// print any extern declarations
		printHeader(hfp, _HstatBuf.st_size);

		fflush(hfp);
		fclose(hfp);
	}
	else {
		char errorStr[256];
		sprintf(errorStr, "%s: %s", _Hname.c_str(), strerror(errno));
		PtNotice(ABW_rebelBaseWindow, NULL,
				 "File Error", NULL,
				 errorStr, NULL,
				 "&OK", NULL, Pt_MODAL);
		return(false);
	}
	return(true);

}

bool
rebelApp::generateSource()
{
	int fileStat = stat(_Cname.c_str(), &_SstatBuf);

	// if the source file exists, read it into a buffer
	// so that it can be checked to see what needs to be added
	if(!fileStat && (_SstatBuf.st_size > 0)) {
		FILE *sfp;
		if((sfp = fopen(_Cname.c_str(), "r")) != NULL) {
			_source = new char[_SstatBuf.st_size + 10];
			fread(_source, sizeof(char), _SstatBuf.st_size, sfp);
			fclose(sfp);
		}
		else {
			char errorStr[256];
			sprintf(errorStr, "%s: %s", _Cname.c_str(), strerror(errno));
			PtNotice(ABW_rebelBaseWindow, NULL,
					 "File Error", NULL,
					 errorStr, NULL,
					 "&OK", NULL, Pt_MODAL);
			return(false);
		}
	}
	else _SstatBuf.st_size = 0;

	// build a list of the headers that rex needs
	// and check to see which ones need to be added to the source

	// construct the name of the extern header
	string tmp = _Hname;
	size_t slashPos;
	while((slashPos = tmp.find("/")) != string::npos) {
		tmp.erase(0, slashPos + 1);
	}
	string htmp = "#include \"";
	htmp += tmp;
	htmp += "\"\n";
		
	_rexIncludeList.clear();
	setIncludeList();
	_rexIncludeList.push_back(htmp);

	// build a list of the global variables that are needed and
	// check to see which ones need to be added to the source
	checkGlobals(_SstatBuf.st_size);

	// build a list of the action definitions that are needed and
	// check to see which ones need to be added to the source
	checkActions(_SstatBuf.st_size);

	// reopen the source code file for writing
	FILE *sfp;
	if((sfp = fopen(_Cname.c_str(), "w")) != NULL) {
		if(_SstatBuf.st_size > 0) {
			// make a copy of the source for _lineTok->tok to mangle
			_sourceTemp.clear();
			_sourceTemp.insert(_sourceTemp.end(), _source, (_source + _SstatBuf.st_size));
			_sourceTemp.push_back('\0');	// make sure that there is a NULL character to terminate the string
		}

		// the following functions must be called in this order
		// to correctly parse the input file
		// print out any include directives that are needed
		printIncludes(sfp, _SstatBuf.st_size);

		// print out any global variables that are needed
		printGlobals(sfp, _SstatBuf.st_size);

		// print out any actions that are needed
		printActions(sfp);

		// if the user is using rebel to maintain menus
		// print the source code to the first VLIST entry
		// then print rebel's list of menus,
		// user functions and real time variables
		printUserSource(sfp, _SstatBuf.st_size);

		// print a comment indicating the start of generated code
		fprintf(sfp, "// BEGIN GENERATED CODE\n\n");

		// print out the user menus
		printUserMenu(sfp);

		// print out the user functions
		printUserFunction(sfp);

		// print out the real time variables
		printRealTimeVar(sfp);

		// print out the state definitions
		printStateDef(sfp);

		// print out the state name array
		printStateName(sfp);

		// print out the abort lists
		printAbortLists(sfp);

		// print out the last stuff
		printInitLists(sfp);

		// print out the properties that are not included
		// in rex structures
		printProperties(sfp);

		fflush(sfp);
		fclose(sfp);
	}
	else {
		char errorStr[256];
		sprintf(errorStr, "%s: %s", _Cname.c_str(), strerror(errno));
		PtNotice(ABW_rebelBaseWindow, NULL,
				 "File Error", NULL,
				 errorStr, NULL,
				 "&OK", NULL, Pt_MODAL);
		return(false);
	}

	return(true);
}


bool
rebelApp::generateSpot()
{
	// reopen the source code file for writing
	FILE *sfp;
	if((sfp = fopen(_Dname.c_str(), "w")) != NULL) {
		if(_SstatBuf.st_size > 0) {
			// make a copy of the source for _lineTok->tok to mangle
			_sourceTemp.clear();
			_sourceTemp.insert(_sourceTemp.end(), _source, (_source + _SstatBuf.st_size));
			_sourceTemp.push_back('\0');
		}

		// print out any user include directives that are needed
		printUserIncludes(sfp, _SstatBuf.st_size);

		// print out any global variables that are needed
		printGlobals(sfp, _SstatBuf.st_size);

		// print out any actions that are needed
		printActions(sfp);

		// if the user is using rebel to maintain menus
		// print the source code to the first VLIST entry
		// then print rebel's list of menus,
		// user functions and real time variables
		printUserSource(sfp, _SstatBuf.st_size);

		printUserMenu(sfp);

		printUserFunction(sfp);

		printRealTimeVar(sfp);

		// print the spot code delimiter and the ID number
		fprintf(sfp, "%%%%\n");
		fprintf(sfp, "id %d\n", _pdgmNum);

		// print out the spot code
		printSpot(sfp);

		fflush(sfp);
		fclose(sfp);
	}
	else {
		char errorStr[256];
		sprintf(errorStr, "%s: %s", _Dname.c_str(), strerror(errno));
		PtNotice(ABW_rebelBaseWindow, NULL,
				 "File Error", NULL,
				 errorStr, NULL,
				 "&OK", NULL, Pt_MODAL);
		return(false);
	}

	return(true);
}


// these functions launch code editors
int
rebelApp::jedCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(app) app->jed(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );

}

void
rebelApp::jed( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	char command[1024];
	sprintf(command, "pterm -f pcterm14 -g 45x100 -t \"JED %s\" /usr/local/bin/jed %s&",
			_Cname.c_str(), _Cname.c_str());
	system(command);

	return;

}


int
rebelApp::jedHdrCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(app) app->jedHdr(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );

}

void
rebelApp::jedHdr( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	char command[1024];
	sprintf(command, "pterm -f pcterm14 -g 45x100 -t \"JED %s\" /usr/local/bin/jed %s&",
			_Hname.c_str(), _Hname.c_str());
	system(command);

	return;

}

int
rebelApp::wsCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(app) app->ws(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );

}

void
rebelApp::ws( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	char command[1024];
	sprintf(command, "ws %s&",_Cname.c_str());
	system(command);

	return;

}

int
rebelApp::wsHdrCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(app) app->wsHdr(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );

}

void
rebelApp::wsHdr( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	char command[1024];
	sprintf(command, "ws %s&",_Hname.c_str());
	system(command);

	return;

}


int
rebelApp::viCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(app) app->vi(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );

}

void
rebelApp::vi( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	char command[1024];
	sprintf(command, "pterm -f pcterm14 -g 45x100 -t \"VI %s\" /bin/vi %s&",
			_Cname.c_str(), _Cname.c_str());
	system(command);

	return;

}

int
rebelApp::viHdrCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(app) app->viHdr(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );

}

void
rebelApp::viHdr( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	char command[1024];
	sprintf(command, "pterm -f pcterm14 -g 45x100 -t \"VI %s\" /bin/vi %s&",
			_Hname.c_str(), _Hname.c_str());
	system(command);

	return;

}


int
rebelApp::pedCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(app) app->ped(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );

}

void
rebelApp::ped( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	char command[1024];
	sprintf(command, "ped %s&",_Cname.c_str());
	system(command);

	return;

}


int
rebelApp::pedHdrCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(app) app->pedHdr(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );

}


void
rebelApp::pedHdr( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	char command[1024];
	sprintf(command, "ped %s&",_Hname.c_str());
	system(command);

	return;

}

// these functions run make to build the paradigm
int
rebelApp::makeAppCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(app) app->makeApp(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );

}

void
rebelApp::makeApp( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(_name.empty()) {
		size_t extnPos;
		_name = _Cname;
		if((extnPos = _name.find(".r")) != string::npos) {
			_name.erase(extnPos);
		}
	}
	string baseName = _name;
	size_t slashPos;
	while((slashPos = baseName.find("/")) != string::npos) {
		baseName.erase(0, slashPos + 1);
	}

	string ssetDir = rFile.ssetDir();
	string errName = baseName;
	errName += ".err";

	char command[1024];
	sprintf(command, "cd %s; /usr/bin/make REBEL=rebel sf=%s 2>&1 | cat > %s; ped %s&",
			ssetDir.c_str(), baseName.c_str(), errName.c_str(), errName.c_str());

	system(command);

	return;
}


void
rebelApp::portApp(string fileName, PtCallbackInfo_t *cbinfo)
{
    char errorStr[256];

    int fileStat = stat(fileName.c_str(), &_SstatBuf);
    if(fileStat || (_SstatBuf.st_size == 0)) {
        sprintf(errorStr, "%s: %s", fileName.c_str(), strerror(errno));
        PtNotice(ABW_rebelBaseWindow, NULL,
                 "File error", NULL,
                 errorStr, NULL,
                 "&OK", NULL, Pt_MODAL);
        return;
    }

    FILE *sfp;
    if((sfp = fopen(fileName.c_str(), "r")) == NULL) {
        sprintf(errorStr, "%s: %s", fileName.c_str(), strerror(errno));
        PtNotice(ABW_rebelBaseWindow, NULL,
                 "File error", NULL,
                 errorStr, NULL,
                 "&OK", NULL, Pt_MODAL);
        return;
    }

    // read in the file
    _source = new char[_SstatBuf.st_size + 10];

    fread(_source, sizeof(char), _SstatBuf.st_size, sfp);
    fclose(sfp);

	// these functions must be called in this order
	portChains(cbinfo);
	portMenus(cbinfo);
	portRealTimeVars(cbinfo);
	portUserFunctions(cbinfo);
	portRestartFunctions();

	// get the properties
	readApp();

	// make the first chain in the app the active window
	if(_chains.size() != 0) {
		_activeWindow = _chains[0];
	}
	else if(_menus.size() != 0) {
		_activeWindow = _menus[0];
	}
	else if(_function != (rebelWin *)NULL) {
		_activeWindow = _function;
	}
	else if(_variable != (rebelWin *)NULL) {
		_activeWindow = _variable;
	}
	return;
}


void rebelApp::portChains(PtCallbackInfo_t *cbinfo)
{
    // make a copy of the file for rebelline to mangle
    _sourceTemp.clear();
    _sourceTemp.insert(_sourceTemp.end(), _source, (_source + _SstatBuf.st_size));
	_sourceTemp.push_back('\0');

	// get the first line of the source

    // read through the source to find the nowstate array definition
	char *tok;
	if((tok = _lineTok->tok(&_sourceTemp[0])) == (char *)NULL) return;
    do {
		string line = tok;
		if(line.find("BEGIN GENERATED CODE") != string::npos) _generatedCode = true;
        if(line.find("nowstate") != string::npos) {
            break;
        }
    } while((tok = _lineTok->tok((char *)NULL)) != (char *)NULL);

    // now read through the entries in the nowstate array
    int chainNum = 0;
    char chainName[32] = "main_set";
    string first;
    string second;
	first.erase(first.begin(), first.end());
    second.erase(second.begin(), second.end());
	PhPoint_t o;
	PhDim_t s;
	int isRealized;
    
	// get the first line in the nowstate array
	while((tok = _lineTok->tok((char *)NULL)) != (char *)NULL) {
		string line = tok;

        // if this is the first line in the nowstate array
        // save the line and get the next
        if(first.empty()) {
            first.insert(first.end(), line.begin(), line.end());
            continue;
        }

        // otherwise, save this line and create a new chain
		// from the first line
		second.erase(second.begin(), second.end());
		second.insert(second.end(), line.begin(), line.end());

		// process the previous line
		string temp = first;
		size_t slashPos;
		if((slashPos = temp.find("//")) != string::npos) {
			temp.erase(0, (slashPos + 2));
			sscanf(temp.c_str(), " %s %hd %hd %hd %hd %d}",
				   chainName, &o.x, &o.y, &s.w, &s.h, &isRealized);

			_activeWindow = new rebelChain(chainName, o, s, cbinfo);
		}
		else {
			isRealized = 1;
			_activeWindow = new rebelChain(chainName, cbinfo);
		}

		_activeWindow->portWindow(this, first, second);
		_chains.push_back(_activeWindow);

		if(!isRealized) {
			PtUnrealizeWidget(_activeWindow->window());
			_activeWindow = (rebelWin *)NULL;
		}

		// if this string doesn't contain a state name,
		// this is the nowstate terminating string so break out of the loop
		if(second.find("ON,&") == string::npos) break;

		// otherwise copy this chain's second string to first for the next chain
		// get the next line, and increment the chain name
		first.erase(first.begin(), first.end());
		first.insert(first.end(), second.begin(), second.end());

		// create a new default chain name
		++chainNum;
		sprintf(chainName, "chain%d", chainNum);
   }

    return;
}

void
rebelApp::portMenus(PtCallbackInfo_t *cbinfo)
{
    // make a copy of the file for rebelline to mangle
    _sourceTemp.clear();
    _sourceTemp.insert(_sourceTemp.end(), _source, (_source + _SstatBuf.st_size));
	_sourceTemp.push_back('\0');

	// get the first line of the source
	string line;

    // read through the source to find the umenus array
	bool inGenerated = false;
	char *tok;
	if((tok = _lineTok->tok(&_sourceTemp[0])) == (char *)NULL) return;
	do {
		line = tok;

		if(line.find("BEGIN GENERATED CODE") != string::npos) inGenerated = true;

        if((line.find("MENU") != string::npos)  &&
		   (line.find("umenus") != string::npos) &&
		   (line.find("=") != string::npos)) {
            break;
        }
    } while((tok = _lineTok->tok((char *)NULL)) != (char *)NULL);

	// don't create a menu window
	// if the user is handling the menu descriptions outside of rebel
	if((_generatedCode == true) && (inGenerated == false)) return;

	// don't create a menu window if the umenus has only
	// the default empty VLIST entry
	if(line.find("EMPTY") != string::npos) return;

	// now read through the entries in the umenus array
	// get the first line in the umenus array
	PhPoint_t o;
	PhDim_t s;
	int isRealized;

	bool inComment = false;
	while ((tok = _lineTok->tok((char *)NULL)) != (char *)NULL) {
		line = tok;

		// skip multi line comments
		if(inComment == false) {
			if((line.find("/*") != string::npos) &&
			   (line.find("*/") == string::npos)) {
				inComment = true;

				if(line[0] == '/') continue;
			}
		}
		else if(inComment == true) {
			if(line.find("*/") != string::npos) inComment = false;
			continue;
		}

		// skip single lines that have been commented out
		if(inComment == false) {
			if(line[0] == '/') continue;
		}

		// don't make menus for separators
		if(line.find("separator") != string::npos) continue;

		// break if this is the umenus terminating string
		if(line.find("NS,") != string::npos) break;

		// extract the name of the menu
		string tmp;
		_lineTok->extractString(line, tmp, "\"", "\"");

		if(!tmp.empty()) {
			string temp = line;
			size_t cmntPos;
			if((cmntPos = temp.find("//")) != string::npos) {
				temp.erase(0, (cmntPos + 2));

				sscanf(temp.c_str(), " %hd, %hd, %hd, %hd, %d",
					   &o.x, &o.y, &s.w, &s.h, &isRealized);
				_activeWindow = new rebelMenu(tmp.c_str(), o, s, cbinfo);
			}
			else  {
				isRealized = 1;
				_activeWindow = new rebelMenu(tmp.c_str(), cbinfo);
			}

			_activeWindow->portWindow(this, line);
			_menus.push_back(_activeWindow);

			if(!isRealized) {
				PtUnrealizeWidget(_activeWindow->window());
				_activeWindow = (rebelWin *)NULL;
			}
		}
	}

	return;
}


void
rebelApp::portRealTimeVars(PtCallbackInfo_t *cbinfo)
{
    // make a copy of the file for rebelline to mangle
    _sourceTemp.clear();
    _sourceTemp.insert(_sourceTemp.end(), _source, (_source + _SstatBuf.st_size));
	_sourceTemp.push_back('\0');

    // read through the source to find the rtvars array definition
	// get the first line of the source
	PhPoint_t o;
	PhDim_t s;
	int isRealized;

	bool inGenerated = false;
	bool inComment = false;
	char *tok;
	if((tok = _lineTok->tok(&_sourceTemp[0])) == (char *)NULL) return;
	do {
		string line = tok;

		if(line.find("BEGIN GENERATED CODE") != string::npos) inGenerated = true;

		// skip multi line comments
		if(inComment == false) {
			if((line.find("/*") != string::npos) &&
			   (line.find("*/") == string::npos)) {
				inComment = true;

				if(line[0] == '/') continue;
			}
		}
		else if(inComment == true) {
			if(line.find("*/") != string::npos) inComment = false;
			continue;
		}

		// skip single lines that have been commented out
		if(inComment == false) {
			if(line[0] == '/') continue;
		}

        if((line.find("RTVAR") != string::npos) &&
		   (line.find("rtvars") != string::npos) &&
		   (line.find("=") != string::npos)) {

			if((_generatedCode == true) && (inGenerated == false)) return;

			// if the user didn't define any real time variables
			if(line.find("EMPTY") != string::npos) break;

			string temp = line;
			size_t slashPos;
			if((slashPos = temp.find("//")) != string::npos) {
				temp.erase(0, (slashPos + 2));
				sscanf(temp.c_str(), " %hd %hd %hd %hd %d",
					   &o.x, &o.y, &s.w, &s.h, &isRealized);
				_activeWindow = new rebelVariable(o, s, cbinfo);
			}
			else {
				isRealized = 1;
				_activeWindow = new rebelVariable(cbinfo);
			}

			_activeWindow->portWindow(this, line);
			_variable = _activeWindow;

			if(!isRealized) {
				PtUnrealizeWidget(_activeWindow->window());
				_activeWindow = (rebelWin *)NULL;
			}
            break;
        }
    } while((tok = _lineTok->tok((char *)NULL)) != (char *)NULL);

	return;
}

void
rebelApp::portUserFunctions(PtCallbackInfo_t *cbinfo)
{
    // make a copy of the file for rebelline to mangle
    _sourceTemp.clear();
    _sourceTemp.insert(_sourceTemp.end(), _source, (_source + _SstatBuf.st_size));
	_sourceTemp.push_back('\0');

    // read through the source to find the ufuncs array definition
	// get the first line of the source
	PhPoint_t o;
	PhDim_t s;
	int isRealized;

	bool inGenerated = false;
	bool inComment = false;
	char *tok;
	if((tok = _lineTok->tok(&_sourceTemp[0])) == (char *)NULL) return;
	do {
		string line = tok;

		if(line.find("BEGIN GENERATED CODE") != string::npos) inGenerated = true;

		// skip multi line comments
		if(inComment == false) {
			if((line.find("/*") != string::npos) &&
			   (line.find("*/") == string::npos)) {
				inComment = true;

				if(line[0] == '/') continue;
			}
		}
		else if(inComment == true) {
			if(line.find("*/") != string::npos) inComment = false;
			continue;
		}

		// skip single lines that have been commented out
		if(inComment == false) {
			if(line[0] == '/') continue;
		}

		if((line.find("USER_FUNC") != string::npos) &&
		   (line.find("ufuncs") != string::npos) &&
		   (line.find("=") != string::npos)) {

			if((_generatedCode == true) && (inGenerated == false)) return;

			// if the user didn't define any functions
			if(line.find("EMPTY") != string::npos) break;

			string temp = line;
			size_t slashPos;
			if((slashPos = temp.find("//")) != string::npos) {
				temp.erase(0, (slashPos + 2));
				sscanf(temp.c_str(), " %hd %hd %hd %hd %d",
					   &o.x, &o.y, &s.w, &s.h, &isRealized);
				_activeWindow = new rebelFunction(o, s, cbinfo);
			}
			else {
				isRealized = 1;
				_activeWindow = new rebelFunction(cbinfo);
			}

			_activeWindow->portWindow(this, line);
			_function = _activeWindow;

			if(!isRealized) {
				PtUnrealizeWidget(_activeWindow->window());
				_activeWindow = (rebelWin *)NULL;
			}
			break;
		}
	} while((tok = _lineTok->tok((char *)NULL)) != (char *)NULL);

	return;
}


void
rebelApp::portRestartFunctions()
{
    // make a copy of the file for rebelline to mangle
    _sourceTemp.clear();
    _sourceTemp.insert(_sourceTemp.end(), _source, (_source + _SstatBuf.st_size));
	_sourceTemp.push_back('\0');

    // read through the source to find the nowstate array definition
	char *tok;
	string line;
	if((tok = _lineTok->tok(&_sourceTemp[0])) == (char *)NULL) return;
	do {
		line = tok;

		if(line.find("BEGIN GENERATED CODE") != string::npos) _generatedCode = true;
        if(line.find("(*init_list[])()") != string::npos) break;

    } while((tok = _lineTok->tok((char *)NULL)) != (char *)NULL);

	// now read the entries in the init_list
	int i = 0;
	while((tok = _lineTok->tok((char *)NULL)) != (char *)NULL) {
		line = tok;

		// if this is the terminating line in the init_list definition
		if(line.find("0") != string::npos) break;
		if(line.find("};") != string::npos) break;

		size_t commaPos;
		if((commaPos = line.find(",")) != string::npos) line.erase(commaPos);

		char rfunc[32];
		sscanf(line.c_str(), "%s", rfunc);

		rebelChain *c = (rebelChain *)_chains[i];
		c->restartFunction(rfunc);
		++i;
	}

	return;
}


bool
rebelApp::checkIncludes(string line)
{
	int flag = false;

	// if a line of codes has the string "#include"
	// check to see if it matches one of the needed includes
	if((line[0] == '#') && (line.find("#include") != string::npos)) {
		// see if this is an old r.h file
		if(line.find(".r.h") != string::npos) {
			flag = true;
			return(flag);
		}
		// walk through the list of needed includes to check if any
		// are on this line
		for(size_t i = 0; i <  _rexIncludeList.size(); ++i) {
			// copy the include directive to a temporary string for strtok to mangle
			char directiveTemp[512];
			strcpy(directiveTemp, _rexIncludeList[i].c_str());

			// throw away the include directive itself
			strtok(directiveTemp, " ");

			// extract the name of the file to include
			char *incName = strtok(NULL, "\"<>");
			if(!incName) continue;

			// if this line is one of the needed includes
			// erase that include from the needed include list
			if(line.find(incName) != string::npos) {
				flag = true;
				return(flag);
			}
		}
	}

	return(flag);
}


void rebelApp::checkGlobals(size_t s)
{
	// make a list of the global variables needed in the source
	_neededGlobals.clear();
	if(!_globalVariables.empty()) {
		_neededGlobals.insert(_neededGlobals.end(), _globalVariables.begin(), _globalVariables.end());
	}

	// if there are no global variables, just return;
	if(_neededGlobals.empty()) return;

	// if there is no source, just return
	if(s == 0) return;

	// if there is a source file
	// parse it to see how many of the needed global variables
	// are already in it
	// make a copy of the source for strtok to mangle
	_sourceTemp.clear();
	_sourceTemp.insert(_sourceTemp.end(), _source, (_source + _SstatBuf.st_size));
	_sourceTemp.push_back('\0');

	// get the first line of the source
	bool inComment = false;
	char *tok;
	if((tok = _lineTok->tok(&_sourceTemp[0])) == (char *)NULL) return;
	do {
		string line = tok;
		if(line.find("BEGIN GENERATED CODE") != string::npos) break;

		// skip multi line comments
		if((line.find("/*") != string::npos) &&
		   (line.find("*/") == string::npos)) {
			inComment = true;

			if(line[0] == '/') continue;
		}
		else if(inComment == true) {
			if(line.find("*/") != string::npos) inComment = false;
			continue;
		}

		// skip single lines that have been commented out
		if(inComment == false) {
			if(line[0] == '/') continue;
		}

		// line defining global variables must start with an alphabetic character
		// and have a semicolon or an opening brace
		if(isalpha((int)line[0]) &&
		   ((line.find(";") != string::npos) ||
			(line.find("{") != string::npos) ||
			(line.find(",") != string::npos))) {
			// check through the list of needed globals to check if any are on this line
			for(size_t i = 0; i < _neededGlobals.size(); ++i) {
				// copy the definition to a temporary string for strtok to mangle
				char globalTemp[512];
				strcpy(globalTemp, _neededGlobals[i].c_str());

				// throw away the variable type specifier
				strtok(globalTemp, " ");

				// extract the name of the needed variable
				char *varName = strtok(NULL, " =[;");
				if(!varName) continue;
					
				// at this point varName has the name of the global variable needed
				// if this name is in the input string, the global is already in the file
				if(line.find(varName) != string::npos) {
					for(size_t j = i; j < _neededGlobals.size() - 1; ++j) {
						_neededGlobals[j] = _neededGlobals[j + 1];
					}
					_neededGlobals.pop_back();
					--i;
				}
			}

			// if the source contains all of the needed globals, stop looking
			if(_neededGlobals.empty()) break;
		}
	} while((tok = _lineTok->tok((char *)NULL)) != (char *)NULL);

	return;
}

void rebelApp::checkActions(size_t s)
{
	// make a list of the actions needed in the source
	_neededActions.clear();
	if(!_localActionList.empty()) {
		_neededActions.insert(_neededActions.end(), _localActionList.begin(), _localActionList.end());
	}

	// if there are no actions, just return;
	if(_neededActions.empty()) return;

	// if there is no source, just return
	if(s == 0) return;

	// if there is a source file
	// parse it to see how many of the needed actions
	// are already in it
	// make a copy of the source for strtok to mangle
	_sourceTemp.clear();
	_sourceTemp.insert(_sourceTemp.end(), _source, (_source + s));
	_sourceTemp.push_back('\0');

	// get the first line of the source
	bool inComment = false;
	char *tok;
	if((tok = _lineTok->tok(&_sourceTemp[0])) == (char *)NULL) return;
	do {
		string line = tok;
		if(line.find("BEGIN GENERATED CODE") != string::npos) break;

		// skip multi line comments
		if((line.find("/*") != string::npos) &&
		   (line.find("*/") == string::npos)) {
			inComment = true;

			if(line[0] == '/') continue;
		}
		else if(inComment == true) {
			if(line.find("*/") != string::npos) inComment = false;
			continue;
		}

		// skip single lines that have been commented out
		if(inComment == false) {
			if(line[0] == '/') continue;
		}
			
		// only check lines that start with an alphabetic character,
		// that have an opening parenthesis, and that have no semicolons
		if(isalpha((int)line[0]) &&
		   (line.find("(") != string::npos) &&
		   (line.find(";") == string::npos)) {
			string funcTemp = line;
			size_t spacePos;
			
			// remove the argument list from the function definition
			if((spacePos = funcTemp.find("(")) != string::npos) {
				funcTemp.erase(spacePos);
			}
			
			// get rid of the type specification if there is one
			if((spacePos = funcTemp.find(" ")) != string::npos) {
				funcTemp.erase(0, (spacePos + 1));
			}

			// check the list of needed actions to see if this one matches
			for(size_t i = 0; i < _neededActions.size(); ++i) {
				// copy the definition to a temporary string for strtok to mangle
				char actionTemp[512];
				strcpy(actionTemp, _neededActions[i].c_str());

				// throw away the variable type specifier
				strtok(actionTemp, " ");

				// extract the name of the action
				char *actName = strtok(NULL, "(");
				if(!actName) continue;

				// at this point actName has the name of the action needed
				// if this name is in the input string, the action is already in the file
				//				if(line.find(actName) != string::npos) {
				if(!strcmp(funcTemp.c_str(), actName)) {
					for(; i < _neededActions.size() - 1; ++i) {
						_neededActions[i] = _neededActions[i + 1];
					}
					_neededActions.pop_back();
					break;
				}
			}

			// if the source contains all of the needed actions, stop looking
			if(_neededActions.empty()) break;
		}
	} while((tok = _lineTok->tok((char *)NULL)) != (char *)NULL);

	return;
}


void
rebelApp::printHeader(FILE *fp, size_t s)
{
	// if there is a source file skip to the generated code block
	if(s > 0) {
		// make a copy of the header for lineTok->tok to mangle
		_headerTemp.clear();
		_headerTemp.insert(_headerTemp.end(), _header, (_header + _HstatBuf.st_size));
		_headerTemp.push_back('\0');

		if((_tok = _lineTok->tok(&_headerTemp[0])) != (char *)NULL) {
			do {
				string line = _tok;

				// print all lines until the generated code block
				if(line.find("BEGIN GENERATED CODE") != string::npos) break;
				else fprintf(fp, "%s", line.c_str());

			} while((_tok = _lineTok->tok((char *)NULL)) != (char *)NULL);
		}
	}

	fprintf(fp, "// BEGIN GENERATED CODE\n\n");

	// print out the extern state declarations
	printStateDecl(fp);
}


void
rebelApp::printIncludes(FILE *fp, size_t s)
{

	// print any needed rex includes at the top of the file
	vector<string>::iterator si;
	if(!_rexIncludeList.empty()) {
		for(si = _rexIncludeList.begin(); si != _rexIncludeList.end(); ++si) {
			fprintf(fp, "%s", si->c_str());
		}
	}

	printUserIncludes(fp, s);
}

void
rebelApp::printUserIncludes(FILE *fp, size_t s)
{
	// if there is a source file
	// print out the include directives in it
	if(s > 0) {
		bool inComment = false;

		// while there is code
		// get the first line of the source
		if((_tok = _lineTok->tok(&_sourceTemp[0])) != (char *)NULL) {
			string line;
			do {
				line = _tok;

				// check for multi line comments
				if(inComment == false) {
					if((line.find("/*") != string::npos) &&
					   (line.find("*/") == string::npos)) {
						inComment = false;
					}
				}
				else if(inComment == true) {
					if(line.find("*/") != string::npos) inComment = false;
				}

				// if the line is not in a multi line comment,
				// test for break conditions
				if(inComment == false) {
					// break; if this line is the beginning of a global variable declaration
					if(isalpha((int)line[0]) && (line.find("[") != string::npos)) break;
					else if(isalpha((int)line[0]) && (line.find("]") != string::npos)) break;
					else if(isalpha((int)line[0]) && (line.find("{") != string::npos)) break;
					else if(isalpha((int)line[0]) && (line.find("}") != string::npos)) break;
					else if(isalpha((int)line[0]) && (line.find(";") != string::npos)) break;

					// break if this line is a function definition
					// check the case where the function type specifier is on its own line
					else if(!strcmp(line.c_str(), "int")) break;
					else if(!strcmp(line.c_str(), "long")) break;
					else if(!strcmp(line.c_str(), "float")) break;
					else if(!strcmp(line.c_str(), "double")) break;
					else if(!strcmp(line.c_str(), "void")) break;

					// check the case where the function type specifier is on the same line
					else if(isalpha((int)line[0]) && (line.find("(") != string::npos)) break;
				}

				// print the line if it is not one of the rex include directives
				if(checkIncludes(line) == false) fprintf(fp, "%s", line.c_str());
			} while((_tok = _lineTok->tok((char *)NULL)) != (char *)NULL);

			// at this point, _tok contains a global variable declaration
			// or the first line of a function definition.
			// _tok must be processed by printGlobals before getting any more code
		}
	}
}

void
rebelApp::printGlobals(FILE *fp, size_t s)
{
	// if there is a source file
	// print out the existing global variables
	if((s > 0) && (_tok != (char *)NULL)) {
		bool inComment = false;
		// at this point, _tok contains the function or variable declaration line
		// that caused the printIncludes loop to break.
		// It must be processed before getting a new line of code
		do {
			string line = _tok;

			// check for multi line comments
			if(inComment == false) {
				if((line.find("/*") != string::npos) &&
				   (line.find("*/") == string::npos)) {
					inComment = true;
				}
			}
			else if(inComment == true) {
				if(line.find("*/") != string::npos) {
					inComment = false;
				}
			}

			// if the line is not in a multi line comment,
			// test for break conditions
			if(inComment == false) {
				// break if the line is the beginning of a function definition
				if(!strcmp(line.c_str(), "int")) break;
				else if(!strcmp(line.c_str(), "long")) break;
				else if(!strcmp(line.c_str(), "float")) break;
				else if(!strcmp(line.c_str(), "double")) break;
				else if(!strcmp(line.c_str(), "void")) break;
				else if((line[0] != '#') && (line.find("(") != string::npos)) break;

				// break if this line is a VLIST array definition
				else if((line.find("VLIST") != string::npos) &&
						(line.find("=") != string::npos) &&
						(line.find("{") != string::npos)) break;

			}

			// print the line.
			if(line.size() > 0) fprintf(fp, "%s", line.c_str());
		} while((_tok = _lineTok->tok((char *)NULL)) != (char *)NULL);

		// at this point, _tok contains the first line of a function definition.
		// _tok must be processed by printUserSource before getting any more code
	}

	if(_neededGlobals.empty()) return;

	// print out the new global variable declarations
	vector<string>::iterator si;
	for(si = _neededGlobals.begin(); si != _neededGlobals.end(); ++si) {
		fprintf(fp, "%s", si->c_str());
	}
}

void
rebelApp::printActions(FILE *fp)
{
	// print out the new action definitions
	vector<string>::iterator si;
	for(si = _neededActions.begin(); si != _neededActions.end(); ++si) {
		if(si->find("void") != string::npos) {
			fprintf(fp, "%s", si->c_str());
			fprintf(fp, "\n{\n");
			fprintf(fp, "\n");
			fprintf(fp, "\n");
			fprintf(fp, "}\n");
			fprintf(fp, "\n");
			fprintf(fp, "\n");
		}
		else if(si->find("ME_RECUR *rp") != string::npos) {
			fprintf(fp, "%s", si->c_str());
			fprintf(fp, "\n{\n");
			fprintf(fp, "\tint code = 0;\n");
			fprintf(fp, "\n");
			fprintf(fp, "\tif(flag & ME_BEF) {\n");
			fprintf(fp, "\n");
			fprintf(fp, "\t}");
			fprintf(fp, "\n");
			fprintf(fp, "\tif(flag & ME_AFT) {\n");
			fprintf(fp, "\n");
			fprintf(fp, "\t}");
			fprintf(fp, "\n");
			fprintf(fp, "\treturn(code);\n");
			fprintf(fp, "}\n");
			fprintf(fp, "\n");
			fprintf(fp, "\n");
		}
		else if(si->find("VLIST *vlp") != string::npos) {
			fprintf(fp, "%s", si->c_str());
			fprintf(fp, "\n{\n");
			fprintf(fp, "\tint code = 0;\n");
			fprintf(fp, "\n");
			fprintf(fp, "\tif(flag & ME_BEF) {\n");
			fprintf(fp, "\n");
			fprintf(fp, "\t}");
			fprintf(fp, "\n");
			fprintf(fp, "\tif(flag & ME_AFT) {\n");
			fprintf(fp, "\n");
			fprintf(fp, "\t}");
			fprintf(fp, "\n");
			fprintf(fp, "\treturn(code);\n");
			fprintf(fp, "}\n");
			fprintf(fp, "\n");
			fprintf(fp, "\n");
		}
		else {
			fprintf(fp, "%s", si->c_str());
			fprintf(fp, "\n{\n");
			fprintf(fp, "\tint code = 0;\n");
			fprintf(fp, "\n");
			fprintf(fp, "\n");
			fprintf(fp, "\treturn(code);\n");
			fprintf(fp, "}\n");
			fprintf(fp, "\n");
			fprintf(fp, "\n");
		}
	}
}


void
rebelApp::printUserSource(FILE *fp, size_t s)
{
	// if there is a source file
	// finish printing it
	if((s > 0) && (_tok != (char *)NULL)) {
		bool inComment = false;
		// at this point, _tok contains the function or variable declaration line
		// that caused the printGlobals loop to break.
		// It must be processed before getting a new line of code
		do {
			string line = _tok;

			// check for multi line comments
			if(inComment == false) {
				if((line.find("/*") != string::npos) &&
				   (line.find("*/") == string::npos)) {
					inComment = true;
				}
			}
			else if(inComment == true) {
				if(line.find("*/") != string::npos) {
					inComment = false;
				}
			}

			// if the line is not in a multi line comment,
			// test for break conditions
			if(inComment == false) {
				// break if this line is the beginning of generated code
				if(line.find("BEGIN GENERATED CODE") != string::npos) break;

				// break if this line is a STATE structure declaration
				else if(line.find("STATE") != string::npos) break;
			}

			// otherwise print the line
			if(line.size() > 0) fprintf(fp, "%s", line.c_str());
		} while((_tok = _lineTok->tok((char *)NULL)) != (char *)NULL);
	}

	return;
}


void
rebelApp::printUserMenu(FILE *fp)
{
	rebelMenu::clearHelpList();

	if(!_menus.empty()) {
		vector<rebelWin *>::iterator wi;
		for(wi = _menus.begin(); wi != _menus.end(); ++wi) {
			rebelMenu *m = (rebelMenu *)(*wi);
			m->genVarLists(fp);

			fprintf(fp, "\n");
		}

		fprintf(fp, "MENU umenus[] = {\n");
		for(wi = _menus.begin(); wi != _menus.end(); ++wi) {
			rebelMenu *m = (rebelMenu *)(*wi);
			m->genMenuArray(fp);
		}
		fprintf(fp, "\tNS,\n");
		fprintf(fp, "};\n\n");
	}
	else {
		// generate an empty VLIST array
		fprintf(fp, "VLIST state_vl[] = { // EMPTY\n");
		fprintf(fp, "NS,\n");
		fprintf(fp, "};\n\n");

		// generate an empty MENU array
		fprintf(fp, "MENU umenus[] = { // EMPTY\n");
		fprintf(fp, "\t{\"state_vars\",\t&state_vl, NP, NP, 0, NP, NS},\n");
		fprintf(fp, "\tNS,\n");
		fprintf(fp, "};\n\n");
	}
}

void
rebelApp::printUserFunction(FILE *fp)
{
	if(_function != (rebelFunction *)NULL) {
		PhPoint_t o = _function->origin();
		PhDim_t s = _function->size();
		fprintf(fp, "USER_FUNC ufuncs[] = { // %hd %hd %hd %hd %ld\n",
				o.x, o.y, s.w, s.h, PtWidgetIsRealized(_function->window()));
		rebelFunction *f = (rebelFunction *)_function;
		f->genFunctions(fp);
		fprintf(fp, "\t{\"\"},\n");
		fprintf(fp, "};\n\n");
	}
	else {
		// generate an empty USER_FUNC array
		fprintf(fp, "USER_FUNC ufuncs[] = { // EMPTY\n");
		fprintf(fp, "\t{\"\"},\n");
		fprintf(fp, "};\n\n");
	}
}

void
rebelApp::printRealTimeVar(FILE *fp)
{
	if(_variable != (rebelVariable *)NULL) {
		PhPoint_t o = _variable->origin();
		PhDim_t s = _variable->size();
		fprintf(fp, "RTVAR rtvars[] = { // %hd %hd %hd %hd %ld\n",
				o.x, o.y, s.w, s.h, PtWidgetIsRealized(_variable->window()));
		rebelVariable *v = (rebelVariable *)_variable;
		v->genVariables(fp);
		fprintf(fp, "\t{\"\"},\n");
		fprintf(fp, "};\n\n");
	}
	else {
		// generate an empty RTVAR array
		fprintf(fp, "RTVAR rtvars[] = { // EMPTY\n");
		fprintf(fp, "\t{\"\"},\n");
		fprintf(fp, "};\n\n");
	}
}


void
rebelApp::printStateDecl(FILE *fp)
{
	vector<rebelWin *>::iterator wi;

	// print out the state declarations
	for(wi = _chains.begin(); wi != _chains.end(); ++wi) {
		rebelChain *c = (rebelChain *)(*wi);
		c->genStateDecls(fp);
	}
	fprintf(fp, "\n\n");
}

void
rebelApp::printStateDef(FILE *fp)
{
	vector<rebelWin *>::iterator wi;

	for(wi = _chains.begin(); wi != _chains.end(); ++wi) {
		rebelChain *c = (rebelChain *)(*wi);
		c->genStateDefs(fp);
	}
	fprintf(fp, "\n");
	fprintf(fp, "\n");
}

void
rebelApp::printStateName(FILE *fp)
{
	vector<rebelWin *>::iterator wi;

	fprintf(fp, "STATE *snames[] = {\n");
	for(wi = _chains.begin(); wi != _chains.end(); ++wi) {
		rebelChain *c = (rebelChain *)(*wi);
		c->genStateArrayNames(fp);
	}
	fprintf(fp, "0};\n\n\n");
}

void
rebelApp::printAbortLists(FILE *fp)
{
	vector<rebelWin *>::iterator wi;

	char ab = 'a';
	for(wi = _chains.begin(); wi != _chains.end(); ++wi) {
		fprintf(fp, "STATE *abort%c[] = {\n", ab++);
		rebelChain *c = (rebelChain *)(*wi);
		c->genAbortList(fp);
	}
}

void
rebelApp::printInitLists(FILE *fp)
{
	vector<rebelWin *>::iterator wi;

	fprintf(fp, "int sf_init();\n\n");

	fprintf(fp, "STATE sps_state;\n\n");

	fprintf(fp, "AWAKE init_state[] = {\n");
	fprintf(fp, "\t{0,ON,ON,&sps_state,&sps_state,0,(STATE *)&init_state[1],0, &sps_state.escape},\n");
	fprintf(fp, "\t{0,0,0,0,0,0,0,0,&sps_state.escape[1]}\n");
	fprintf(fp, "};\n");

	fprintf(fp, "STATE sps_state = {\n");
	fprintf(fp, "\t{\"spec\"},1,0,1,0,\n");
	fprintf(fp, "\t{sf_init,%d,0,0,0,0,0,0,0,0,0},\n", _pdgmNum);
	fprintf(fp, "\t0,0,\n");
	fprintf(fp, "\t{\n");
	fprintf(fp, "\t\t{0,init_state,&sps_state,TIME,0,0},\n");
	fprintf(fp, "\t\t{0,0,0,0,0,0},\n");
	fprintf(fp, "\t\t{0,0,0,0,0,0},\n");
	fprintf(fp, "\t\t{0,0,0,0,0,0},\n");
	fprintf(fp, "\t\t{0,0,0,0,0,0},\n");
	fprintf(fp, "\t\t{0,0,0,0,0,0},\n");
	fprintf(fp, "\t\t{0,0,0,0,0,0},\n");
	fprintf(fp, "\t\t{0,0,0,0,0,0},\n");
	fprintf(fp, "\t\t{0,0,0,0,0,0},\n");
	fprintf(fp, "\t\t{0,0,0,0,0,0}\n");
	fprintf(fp, "\t}\n");
	fprintf(fp, "};\n");

	char ab = 'a';
	fprintf(fp,"AWAKE nowstate[] = {\n");
	for(wi = _chains.begin(); wi != _chains.end(); ++wi) {
		rebelChain *c = (rebelChain *)(*wi);
		string begState = c->firstState();
		PhPoint_t o = c->origin();
		PhDim_t s = c->size();
		fprintf(fp,"\t{0,ON,ON,&%s,&%s,abort%c,0,0,0}, // %s %hd %hd %hd %hd %d\n",
				begState.c_str(), begState.c_str(), ab++, c->name(), o.x, o.y,
				s.w, s.h, c->isRealized());
	}
	fprintf(fp,"\t{0,0,0,0,0,0,0,0,0}\n");
	fprintf(fp,"};\n\n");

	// print out the restart functions
	for(wi = _chains.begin(); wi != _chains.end(); ++wi) {
		rebelChain *c = (rebelChain *)(*wi);
		char *fnptr = c->restartFunction();
		if(strlen(fnptr)) fprintf(fp, "void %s();\n", fnptr);
	}

	// print out the initlist
	fprintf(fp,"int (*init_list[])() = {\n");
	for(wi = _chains.begin(); wi != _chains.end(); ++wi) {
		rebelChain *c = (rebelChain *)(*wi);
		char *fnptr = c->restartFunction();
		if(strlen(fnptr)) fprintf(fp, "\t%s,\n", fnptr);
	}
	fprintf(fp,"\t0\n};\n");

}

void
rebelApp::printProperties(FILE *fp)
{
	extern PhPoint_t bkgdOrigin;
	
	fprintf(fp, "/**********************************\n");
	fprintf(fp, "APPLICATION PROPERTIES\n");

	// write the paradigm number
	fprintf(fp, "PARADIGM_NUMBER %d\n", _pdgmNum);

	// save the size of the base window
	fprintf(fp, "BASE_WINDOW %hd %hd\n", rSize.w, rSize.h);

	// save the name of the prototypes file
	if(!_Pname.empty()) fprintf(fp, "PROTOTYPES %s\n", _Pname.c_str());
	
	// get the latest position of the background widget
	PtGetAbsPosition(ABW_rebelBkgd, &bkgdOrigin.x, &bkgdOrigin.y);

	// save the locations and sizes of the widget and resource dialogs
	PhPoint_t *pwo;
	PtGetResource(ABW_rebelWidgetDialog, Pt_ARG_POS, &pwo, 0);
	PhPoint_t wo;
	wo.x = pwo->x - bkgdOrigin.x;
	wo.y = pwo->y - bkgdOrigin.y;
	fprintf(fp, "WIDGET_DIALOG %hd %hd\n", wo.x, wo.y);

	PhArea_t *pra;
	PtGetResource(ABW_rebelResourceDialog, Pt_ARG_AREA, &pra, 0);
	PhPoint_t ro;
	ro.x = pra->pos.x - bkgdOrigin.x;
	ro.y = pra->pos.y - bkgdOrigin.y;
	PhDim_t rs;
	rs = pra->size;
	fprintf(fp, "RESOURCE_DIALOG %hd %hd %hd %hd\n", ro.x, ro.y, rs.w, rs.h);

	// print the chain comments
	vector<rebelWin *>::iterator wi;
	for(wi = _chains.begin(); wi != _chains.end(); ++wi) {
		(*wi)->saveWindow(fp);
	}
	// print the menu comments
	for(wi = _menus.begin(); wi != _menus.end(); ++wi) {
		(*wi)->saveWindow(fp);
	}

	// print the function comments
	if(_function) _function->saveWindow(fp);

	// print the variable comments
	if(_variable) _variable->saveWindow(fp);

	fprintf(fp, "APP_DONE\n");

	fprintf(fp, " **********************************/");
}

void
rebelApp::printSpot(FILE *fp)
{
	vector<rebelWin *>::iterator wi;

	for(wi = _chains.begin(); wi != _chains.end(); ++wi) {
		rebelChain *c = (rebelChain *)(*wi);
		c->genSpotDefs(fp);
	}
}


void
rebelApp::readApp()
{
	extern PhPoint_t bkgdOrigin;

    // make a copy of the file for rebelline to mangle
    _sourceTemp.clear();
    _sourceTemp.insert(_sourceTemp.end(), _source, (_source + _SstatBuf.st_size));
	_sourceTemp.push_back('\0');

	// get the latest position of the background widget
	PtGetAbsPosition(ABW_rebelBkgd, &bkgdOrigin.x, &bkgdOrigin.y);

    // read through the source to find the beginning of the property comments
	// get the first line of the source
	string line;
	char *tok;
	if((tok = _lineTok->tok(&_sourceTemp[0])) == (char *)NULL) return;
	do {
		line = tok;
		// skip down the buffer to the beginning of the properties
		if(line.find("/**********************************") != string::npos) break;
	} while((tok = _lineTok->tok((char *)NULL)) != (char *)NULL);

	while((tok = _lineTok->tok((char *)NULL)) != (char *)NULL) {
		line = tok;
		
		// end of properties
		if(line.find(" **********************************/") != string::npos) {
			break;
		}

		// look for the paradigm
		if(line.find("PARADIGM_NUMBER") != string::npos) {
			sscanf(line.c_str(), "PARADIGM_NUMBER %d", &_pdgmNum);
		}
		else if(line.find("BASE_WINDOW") != string::npos) {
			sscanf(line.c_str(), "BASE_WINDOW %hd %hd", &_prefSize.w, &_prefSize.h);
		}
		else if(line.find("PROTOTPYES") != string::npos) {
			size_t pos;
			char name[256];
			if((pos = line.find(" ")) != string::npos) {
				line.erase(0, (pos + 1));
				sscanf(line.c_str(), "%s", name);
				_Pname = name;
				struct stat statBuf;
				int fileStat = stat(_Pname.c_str(), &statBuf);
				if(!fileStat && (statBuf.st_size > 0)) {
					readPrototypes(_Pname.c_str(), statBuf.st_size);
				}
			}
		}
		else if(line.find("WIDGET_DIALOG") != string::npos) {
			PhPoint_t o;
			sscanf(line.c_str(), "WIDGET_DIALOG %hd %hd", &o.x, &o.y);
			o.x += bkgdOrigin.x;
			o.y += bkgdOrigin.y;
			PtSetResource(ABW_rebelWidgetDialog, Pt_ARG_POS, &o, 0);
		}
		else if(line.find("RESOURCE_DIALOG") != string::npos) {
			PhPoint_t o;
			PhDim_t s;
			sscanf(line.c_str(), "RESOURCE_DIALOG %hd %hd %hd %hd",
				   &o.x, &o.y, &s.w, &s.h);
			o.x += bkgdOrigin.x;
			o.y += bkgdOrigin.y;
			rResources->size(s);
			PtSetResource(ABW_rebelResourceDialog, Pt_ARG_POS, &o, 0);
			PtSetResource(ABW_rebelResourceDialog, Pt_ARG_DIM, &s, 0);
			break;
		}
	}

	return;
}

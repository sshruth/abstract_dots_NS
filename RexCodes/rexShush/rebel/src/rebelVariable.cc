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
extern rebelResource *rResources;

rebelVariable::rebelVariable(PtCallbackInfo_t *cbinfo) : rebelWin("Real Time Variables")
{
	_winTyp = REBEL_VARIABLE;

	PhDim_t s =  { 150, 350 };
	_prefSize = s;

	cbinfo->cbdata = (void *)this;
	ApModuleParent(ABM_rebelWindow, AB_PARENT, ABW_rebelBkgd);
	ApModuleLocation(ABM_rebelWindow, AB_LOC_REL_MODULE, _nextOrigin.x, _nextOrigin.y);
	_window = ApCreateModule(ABM_rebelWindow, NULL, cbinfo);
}

rebelVariable::rebelVariable(PhPoint_t origin, PhDim_t size, PtCallbackInfo_t *cbinfo) : rebelWin("Real Time Variables", size)
{
	_winTyp = REBEL_VARIABLE;

	cbinfo->cbdata = (void *)this;
	ApModuleParent(ABM_rebelWindow, AB_PARENT, ABW_rebelBkgd);
	ApModuleLocation(ABM_rebelWindow, AB_LOC_REL_MODULE, origin.x, origin.y);
	_window = ApCreateModule(ABM_rebelWindow, NULL, cbinfo);

}

rebelVariable::~rebelVariable()
{

}


void
rebelVariable::addButton( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// the base class function selects this window and
	// gets a position for the new button if one is to be created
	rebelWin::addButton(widget, apinfo, cbinfo);

	// create a button to represent the new state
	int n = _buttons.size();
	char defName[64];
	sprintf(defName, "Variable%d", n);
	rebelVariableItem *item = new rebelVariableItem(_scrollCanvas, defName, _newPos);
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


void rebelVariable::cut( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	char text[128];
	sprintf(text, "Do you really want to the real time variable list?");
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
		app->cutWindow(this);
		break;
	}
}

int
rebelVariable::newVariableCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(app == (rebelApp *)NULL) return( Pt_CONTINUE );

	// get pointer to this variable list instance
	rebelWin *w = app->activeWindow();
	if(w == (rebelWin *)NULL) return( Pt_CONTINUE );
	if(w->winTyp() != REBEL_VARIABLE) return( Pt_CONTINUE );

	w->newButton(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );

}


void rebelVariable::saveWindow(FILE *fp)
{
	// save the name of the chain
	fprintf(fp, "VARIABLE %s\n", _name);

	// save the descriptions of the comments
	vector<rebelButton *>::iterator bi;
	for(bi = _buttons.begin(); bi != _buttons.end(); ++bi) {
		if((*bi)->butType() == REBEL_COMMENT) {
			(*bi)->saveButton(fp);
		}
	}
	fprintf(fp, "VARIABLE_DONE\n");
}

void
rebelVariable::readWindow()
{
	rebelComment *cmnt = (rebelComment *)NULL;
	PhPoint_t o;
	string line;

	// skip down the buffer to the beginning of the properties
	char *tok;
	if((tok = _lineTok->tok((char *)NULL)) == (char *)NULL) return;
	do {
		line = tok;

		if(line.find("/**********************************") != string::npos) {
			break;
		}
	} while((tok = _lineTok->tok((char *)NULL)) != (char *)NULL);
	
	// now find the description of this variable
	while((tok = _lineTok->tok((char *)NULL)) != (char *)NULL) {
		line = tok;

		if((line.find("VARIABLE") != string::npos) &&
		   (line.find(_name) != string::npos)) {
			break;
		}
	}

	// now process the comments for this variable
	while((tok = _lineTok->tok((char *)NULL)) != (char *)NULL) {
		line = tok;

		// done with this variable
		if(line.find("VARIABLE_DONE") != string::npos) return;

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

void rebelVariable::portWindow(rebelApp *app, string first, string second)
{
	PhPoint_t o;
	string line;
	string tmp;

	// make a copy of the file for lineTok to mangle
	_sourceTemp.clear();
	_sourceTemp.insert(_sourceTemp.end(), app->srcBeg(), app->srcEnd());
	_sourceTemp.push_back('\0');

    // read through the source to find the rtvars array definition
    char *tok;
	if((tok = _lineTok->tok(&_sourceTemp[0])) == (char *)NULL) return;
	do {
		string line = tok;
        if((line.find("RTVAR") != string::npos) &&
		   (line.find("rtvars") != string::npos) &&
		   (line.find("=") != string::npos)) {
			o.x = 0;
			o.y = 0;
            break;
        }
    } while((tok = _lineTok->tok((char *)NULL)) != (char *)NULL);

	// scan the RTVAR array to get the real time variables
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

		// check this this line to see if it contains the terminator string
		if(line.find("\"\"") != string::npos) break;
		if(line.find("NS") != string::npos) break;

		// get the position of the button for rebel apps
		string temp = line;
		size_t slashPos;
		if((slashPos = temp.find("//")) != string::npos) {
			temp.erase(0, (slashPos + 2));
			sscanf(temp.c_str(), "%hd %hd", &o.x, &o.y);
		}

		// if this line is a preprocessor directive,
		// create a special preprocessor variable item
		rebelVariableItem *item;
		if(line[0] == '#') {
			size_t nlPos;
			if((nlPos = line.find("\n")) != string::npos) {
				line.erase(nlPos);
			}
			item = new rebelVariableItem(_scrollCanvas, line.c_str(), o);
			_buttons.push_back(item);
		}
		// otherwise create a regular variable item
		else {
			_lineTok->extractString(line, tmp, "\"", "\"");
			if(!tmp.empty()) {
				item = new rebelVariableItem(_scrollCanvas, tmp.c_str(), o);
				item->portButton((rebelWin *)this, line);
				_buttons.push_back(item);
			}
		}
		o = getNextCell(o);
	}

	// read the properties to get the comments
	readWindow();

	// set the active button pointer for this chain
	_activeBtnPtr = _buttons.begin();

	return;
}

void
rebelVariable::genVariables(FILE *fp)
{
	vector<rebelButton *>::iterator bi;
	for(bi = _buttons.begin(); bi != _buttons.end(); ++bi) {
		rebelVariableItem *vi = (rebelVariableItem *)(*bi);
		vi->genCode(fp);
	}
}

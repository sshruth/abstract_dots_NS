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

rebelState::rebelState()
{
	_butType = REBEL_STATE;

	strcpy(_rexState.statename, "");
	strcpy(_rexState.code, "0");
	strcpy(_rexState.ptrCode, "0");
	strcpy(_rexState.rlflag, "0");
	strcpy(_rexState.rlval, "0");
	strcpy(_rexState.device.action, "0");
	strcpy(_rexState.device.act_code, "0");
	strcpy(_rexState.device.act_2code, "0");
	strcpy(_rexState.device.act_3code, "0");
	strcpy(_rexState.device.act_4code, "0");
	strcpy(_rexState.device.act_5code, "0");
	strcpy(_rexState.device.act_6code, "0");
	strcpy(_rexState.device.act_7code, "0");
	strcpy(_rexState.device.act_8code, "0");
	strcpy(_rexState.device.act_9code, "0");
	strcpy(_rexState.device.act_10code, "0");
	strcpy(_rexState.preset, "0");
	strcpy(_rexState.random, "0");
	memset(_rexState.comment, 0, maxCmnt);

	for(uint32_t ei = 0; ei < maxEscp; ++ei) {
		ESCAPE *e = &_rexState.stateEscapes[ei];
		strcpy(e->nextesc, "0");
		strcpy(e->awakestate, "0");
		strcpy(e->stateptr, "0");
		strcpy(e->sflags, "0");
		strcpy(e->funct, "0");
		strcpy(e->smask, "0");
		e->ePath = (PtWidget_t *)NULL;
		e->toState = (rebelState *)NULL;
		strcpy(e->escapeOn, "");
		memset(e->comment, 0, maxCmnt);
	}

	strcpy(_name, "");
	strcpy(_action, "");
	strcpy(_runLineLevel, "");
	for(uint32_t i = 0; i < maxEscp; ++i) {
		_rightSockets[i] = (i * 3) + 3;
		_leftSockets[i] = (i * 3) + 3;
	}
	_rightSocketIndx = 0;
	_leftSocketIndx = 0;
	_escapeIndx = -1;

	_button = (PtWidget_t *)NULL;
	PhPoint_t p = { 0, 0 };
	_origin = p;
}

rebelState::rebelState(PtWidget_t *parent, char *name) : rebelButton(name)
{
	extern ApDBase_t *rPictures;

	_butType = REBEL_STATE;

	strcpy(_rexState.statename, name);
	strcpy(_rexState.code, "0");
	strcpy(_rexState.ptrCode, "0");
	strcpy(_rexState.rlflag, "0");
	strcpy(_rexState.rlval, "0");
	strcpy(_rexState.device.action, "0");
	strcpy(_rexState.device.act_code, "0");
	strcpy(_rexState.device.act_2code, "0");
	strcpy(_rexState.device.act_3code, "0");
	strcpy(_rexState.device.act_4code, "0");
	strcpy(_rexState.device.act_5code, "0");
	strcpy(_rexState.device.act_6code, "0");
	strcpy(_rexState.device.act_7code, "0");
	strcpy(_rexState.device.act_8code, "0");
	strcpy(_rexState.device.act_9code, "0");
	strcpy(_rexState.device.act_10code, "0");
	strcpy(_rexState.preset, "0");
	strcpy(_rexState.random, "0");
	memset(_rexState.comment, 0, maxCmnt);

	for(uint32_t ei = 0; ei < maxEscp; ++ei) {
		ESCAPE *e = &_rexState.stateEscapes[ei];
		strcpy(e->nextesc, "0");
		strcpy(e->awakestate, "0");
		strcpy(e->stateptr, "0");
		strcpy(e->sflags, "0");
		strcpy(e->funct, "0");
		strcpy(e->smask, "0");
		e->ePath = (PtWidget_t *)NULL;
		e->toState = (rebelState *)NULL;
		strcpy(e->escapeOn, "");
		memset(e->comment, 0, maxCmnt);
	}

	strcpy(_sname, "s");
	strcat(_sname, name);

	strcpy(_action, "");
	strcpy(_runLineLevel, "");
	for(uint32_t i = 0; i < maxEscp; ++i) {
		_rightSockets[i] = (i  * 3) + 3;
		_leftSockets[i] = (i * 3) + 3;
	}
	_rightSocketIndx = 0;
	_leftSocketIndx = 0;
	_escapeIndx = -1;

	// create the button but don't realize it yet
	PtArg_t arg;
	PtSetArg(&arg, Pt_ARG_TEXT_STRING, name, 0);
	_button = ApCreateDBWidget(rPictures, "rebelStateButton", parent, &_origin, 1, &arg);

	PtSetResource(_button, Pt_ARG_POINTER, this, 0);
}

rebelState::rebelState(PtWidget_t *parent, char *name, PhPoint_t pos) : rebelButton(name, pos)
{
	extern ApDBase_t *rPictures;

	_butType = REBEL_STATE;

	strcpy(_rexState.statename, name);
	strcpy(_rexState.code, "0");
	strcpy(_rexState.ptrCode, "0");
	strcpy(_rexState.rlflag, "0");
	strcpy(_rexState.rlval, "0");
	strcpy(_rexState.device.action, "0");
	strcpy(_rexState.device.act_code, "0");
	strcpy(_rexState.device.act_2code, "0");
	strcpy(_rexState.device.act_3code, "0");
	strcpy(_rexState.device.act_4code, "0");
	strcpy(_rexState.device.act_5code, "0");
	strcpy(_rexState.device.act_6code, "0");
	strcpy(_rexState.device.act_7code, "0");
	strcpy(_rexState.device.act_8code, "0");
	strcpy(_rexState.device.act_9code, "0");
	strcpy(_rexState.device.act_10code, "0");
	strcpy(_rexState.preset, "0");
	strcpy(_rexState.random, "0");
	memset(_rexState.comment, 0, maxCmnt);

	for(uint32_t ei = 0; ei < maxEscp; ++ei) {
		ESCAPE *e = &_rexState.stateEscapes[ei];
		strcpy(e->nextesc, "0");
		strcpy(e->awakestate, "0");
		strcpy(e->stateptr, "0");
		strcpy(e->sflags, "0");
		strcpy(e->funct, "0");
		strcpy(e->smask, "0");
		e->ePath = (PtWidget_t *)NULL;
		e->toState = (rebelState *)NULL;
		strcpy(e->escapeOn, "");
		memset(e->comment, 0, maxCmnt);
	}

	strcpy(_sname, "s");
	strcat(_sname, name);

	strcpy(_action, "");
	strcpy(_runLineLevel, "");
	for(uint32_t i = 0; i < maxEscp; ++i) {
		_rightSockets[i] = (i  * 3) + 3;
		_leftSockets[i] = (i * 3) + 3;
	}
	_rightSocketIndx = 0;
	_leftSocketIndx = 0;
	_escapeIndx = -1;

	// create the button realize it
	PtArg_t arg;
	PtSetArg(&arg, Pt_ARG_TEXT_STRING, name, 0);
	_button = ApCreateDBWidget(rPictures, "rebelStateButton", parent, &_origin, 1, &arg);
	PtRealizeWidget(_button);

	PtSetResource(_button, Pt_ARG_POINTER, this, 0);
}

rebelState::rebelState(rebelState& rs)
{
	_butType = rs._butType;

	_rexState = rs._rexState;

	strcpy(_name, rs._name);
	strcpy(_action, rs._action);
	strcpy(_runLineLevel, rs._runLineLevel);

	for(int i = 0; i < MAXESC; ++i) {
		_rightSockets[i] = rs._rightSockets[i];
		_leftSockets[i] = rs._leftSockets[i];
	}

	_rightSocketIndx = rs._rightSocketIndx;
	_leftSocketIndx = rs._leftSocketIndx;
	_escapeIndx = rs._escapeIndx;

	_button = rs._button;
	_origin = rs._origin;
}

rebelState::~rebelState()
{
	for(uint32_t ei = 0; ei != maxEscp; ++ei) {
		if(_rexState.stateEscapes[ei].ePath) {
			PtDestroyWidget(_rexState.stateEscapes[ei].ePath);
		}
	}

	// if the action string is not "0"
	if(strcmp(_rexState.device.action, "0")) {
		app->deleteAction(_rexState.device.action);
	}
	if(strcmp(_rexState.ptrCode, "0")) {
		app->deleteGlobalVariable(_rexState.ptrCode);
	}
}

rebelState& rebelState::operator=(rebelState& rs)
{
	_butType = rs._butType;

	_rexState = rs._rexState;

	strcpy(_name, rs._name);
	strcpy(_action, rs._action);
	strcpy(_runLineLevel, rs._runLineLevel);

	for(int i = 0; i < MAXESC; ++i ) {
		_rightSockets[i] = rs._rightSockets[i];
		_leftSockets[i] = rs._leftSockets[i];
	}

	_rightSocketIndx = rs._rightSocketIndx;
	_leftSocketIndx = rs._leftSocketIndx;
	_escapeIndx = rs._escapeIndx;

	_button = rs._button;
	_origin = rs._origin;

	return(*this);
}


int
rebelState::resourceCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// get a pointer to the chain instance containing this state
	PtWidget_t *win = ApGetInstance(widget);
	rebelChain *chain;
	PtGetResource(win, Pt_ARG_POINTER, &chain, 0);

	// get a pointer to this state instance
	rebelButton *b;
	PtGetResource(widget, Pt_ARG_POINTER, &b, 0);

	// get the pointer event type
	PhPointerEvent_t *pe;
	pe = (PhPointerEvent_t *)PhGetData(cbinfo->event);

	// if no modifier keys were pressed, set the resource panel
	if(!(pe->key_mods & (KEYMOD_SHIFT | KEYMOD_CTRL | KEYMOD_ALT))) {
		if(b) b->resource(widget, apinfo, cbinfo);
		if(chain) chain->ungroupButtons();
	}

	// if modifier key was pressed, add the widget to the group
	else {
		if(chain) chain->groupButton(b, pe->key_mods);
	}

	return( Pt_CONTINUE );

}

void
rebelState::resource( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	extern rebelResource *rResources;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	rResources->setStateWidgets(this);

	return;

}


void
rebelState::cut( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	char text[128];
	sprintf(text, "Do you really want to cut state %s?", _name);
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
rebelState::stateNameCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
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
		rebelState *state;
		PtGetResource(pg, Pt_ARG_POINTER, &state, 0);

		// call this state instance of the stateName function
		if(state) state->stateName(widget, apinfo, cbinfo);
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
rebelState::stateName( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	char *str;
	PtGetResource(widget, Pt_ARG_TEXT_STRING, &str, 0);
	PtSetResource(_button, Pt_ARG_TEXT_STRING, str, 0);

	// the name the user gave the state
	strncpy(_rexState.statename, str, 29);
	strncpy(_name, str, 29);

	// the name rex gives to this structure
	strcpy(_sname, "s");
	strcat(_sname, _name);

	app->saved(false);

	return;

}

int
rebelState::ecodeCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
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
	PtWidget_t *pg = rsrc->panelGroup();
	rebelState *state;
	PtGetResource(pg, Pt_ARG_POINTER, &state, 0);

	// call this state instance of the ecode function
	state->ecode(widget, apinfo, cbinfo);

	// reset the colors indicating that the input has been processed
	PtArg_t args[2];
	int nArgs = 0;
	PtSetArg(&args[nArgs], Pt_ARG_COLOR, Pg_BLACK, 0); ++nArgs;
	PtSetArg(&args[nArgs], Pt_ARG_FILL_COLOR, Pg_WHITE, 0); ++nArgs;
	PtSetResources(widget, nArgs, args);
															  
	return( Pt_CONTINUE );

}

void
rebelState::ecode( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	extern rebelApp *app;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	char *str;
	PtGetResource(widget, Pt_ARG_TEXT_STRING, &str, 0);

	// if the ecode item is a pointer to a global variable
	if(str[0] == '&') {
		strcpy(_rexState.code, "0");
		strncpy(_rexState.ptrCode, str, 32);
		app->addGlobalVariables("int", &str[1]);
	}

	// if the ecode item is a constant
	else {
		strncpy(_rexState.code, str, 32);
		strcpy(_rexState.ptrCode, "0");
	}

	app->saved(false);

	return;

}


int
rebelState::rexActionCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
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
	PtWidget_t *pg = rsrc->panelGroup();
	rebelState *state;
	PtGetResource(pg, Pt_ARG_POINTER, &state, 0);

	// call this state instance of the rexAction function
	state->rexAction(widget, apinfo, cbinfo);

	// reset the colors indicating that the input has been processed
	PtArg_t args[2];
	int nArgs = 0;
	PtSetArg(&args[nArgs], Pt_ARG_COLOR, Pg_BLACK, 0); ++nArgs;
	PtSetArg(&args[nArgs], Pt_ARG_FILL_COLOR, Pg_WHITE, 0); ++nArgs;
	PtSetResources(widget, nArgs, args);

	return( Pt_CONTINUE );

}

void
rebelState::rexAction( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	char *str;
	PtGetResource(widget, Pt_ARG_TEXT_STRING, &str, 0);

	// an action was previously defined,
	// clear the old name and arguments
	if(strlen(_action)) {
		strcpy(_rexState.device.action, "0");
		strcpy(_rexState.device.act_code, "0");
		strcpy(_rexState.device.act_2code, "0");
		strcpy(_rexState.device.act_3code, "0");
		strcpy(_rexState.device.act_4code, "0");
		strcpy(_rexState.device.act_5code, "0");
		strcpy(_rexState.device.act_6code, "0");
		strcpy(_rexState.device.act_7code, "0");
		strcpy(_rexState.device.act_8code, "0");
		strcpy(_rexState.device.act_9code, "0");
		strcpy(_rexState.device.act_10code, "0");
	}
	strncpy(_action, str, 384);

	int nArgs = parseAction();

	setActionArgTypes();
	setActionArgArrays();

	string prototype = "int ";
	prototype += _rexState.device.action;
	prototype += "()";
	app->addFunction(_rexState.device.action, prototype);

	if(nArgs > 0) checkGlobalVariables();
	app->saved(false);
}

void rebelState::checkGlobalVariables()
{
	if(islower((int)_rexState.device.act_code[0]) || (_rexState.device.act_code[0] == '&')) {
		app->addGlobalVariables(_rexState.device.act_code_type,	_rexState.device.act_code,
								_rexState.device.act_code_array);
	}
	if(islower((int)_rexState.device.act_2code[0]) || (_rexState.device.act_2code[0] == '&')) {
		app->addGlobalVariables(_rexState.device.act_2code_type, _rexState.device.act_2code,
								_rexState.device.act_2code_array);
	}
	if(islower((int)_rexState.device.act_3code[0]) || (_rexState.device.act_3code[0] == '&')) {
		app->addGlobalVariables(_rexState.device.act_3code_type, _rexState.device.act_3code,
								_rexState.device.act_3code_array);
	}
	if(islower((int)_rexState.device.act_4code[0]) || (_rexState.device.act_4code[0] == '&')) {
		app->addGlobalVariables(_rexState.device.act_4code_type, _rexState.device.act_4code,
								_rexState.device.act_4code_array);
	}
	if(islower((int)_rexState.device.act_5code[0]) || (_rexState.device.act_5code[0] == '&')) {
		app->addGlobalVariables(_rexState.device.act_5code_type, _rexState.device.act_5code,
								_rexState.device.act_5code_array);
	}
	if(islower((int)_rexState.device.act_6code[0]) || (_rexState.device.act_6code[0] == '&')) {
		app->addGlobalVariables(_rexState.device.act_6code_type, _rexState.device.act_6code,
								_rexState.device.act_6code_array);
	}
	if(islower((int)_rexState.device.act_7code[0]) || (_rexState.device.act_7code[0] == '&')) {
		app->addGlobalVariables(_rexState.device.act_7code_type, _rexState.device.act_7code,
								_rexState.device.act_7code_array);
	}
	if(islower((int)_rexState.device.act_8code[0]) || (_rexState.device.act_8code[0] == '&')) {
		app->addGlobalVariables(_rexState.device.act_8code_type, _rexState.device.act_8code,
								_rexState.device.act_8code_array);
	}
	if(islower((int)_rexState.device.act_9code[0]) || (_rexState.device.act_9code[0] == '&')) {
		app->addGlobalVariables(_rexState.device.act_9code_type, _rexState.device.act_9code,
								_rexState.device.act_9code_array);
	}
	if(islower((int)_rexState.device.act_10code[0]) || (_rexState.device.act_10code[0] == '&')) {
		app->addGlobalVariables(_rexState.device.act_10code_type, _rexState.device.act_10code,
								_rexState.device.act_10code_array);
	}


}

// this function extracts the action name and the names of the
// input variables from a declaration string
int 
rebelState::parseAction()
{
	char temp[384];

	// make a copy of the declaration string for strtok to mangle
	strcpy(temp, _action);

	// parse the action string for the action name
	char *tok;
	if((tok = strtok(temp, "(")) != (char *)NULL) strcpy(_rexState.device.action, tok);
	else return(0);

	// now parse the rest of the string for the arguments
	if((tok = strtok(NULL, ", )"))) strcpy(_rexState.device.act_code, tok);
	else return(0);

	if((tok = strtok(NULL, ", )"))) strcpy(_rexState.device.act_2code, tok);
	else return(1);
		
	if((tok = strtok(NULL, ", )"))) strcpy(_rexState.device.act_3code, tok);
	else return(2);
		
	if((tok = strtok(NULL, ", )"))) strcpy(_rexState.device.act_4code, tok);
	else return(3);

	if((tok = strtok(NULL, ", )"))) strcpy(_rexState.device.act_5code, tok);
	else return(4);
		
	if((tok = strtok(NULL, ", )"))) strcpy(_rexState.device.act_6code, tok);
	else return(5);
		
	if((tok = strtok(NULL, ", )"))) strcpy(_rexState.device.act_7code, tok);
	else return(6);
		
	if((tok = strtok(NULL, ", )"))) strcpy(_rexState.device.act_8code, tok);
	else return(7);
		
	if((tok = strtok(NULL, ", )"))) strcpy(_rexState.device.act_9code, tok);
	else return(8);
		
	if((tok = strtok(NULL, ", )"))) strcpy(_rexState.device.act_10code, tok);
	else return(9);

	return(10);

}


int
rebelState::runLineCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
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
	PtWidget_t *pg = rsrc->panelGroup();
	rebelState *state;
	PtGetResource(pg, Pt_ARG_POINTER, &state, 0);

	// call this state instance of the runLine function
	state->runLine(widget, apinfo, cbinfo);

	// reset the colors indicating that the input has been processed
	PtArg_t args[2];
	int nArgs = 0;
	PtSetArg(&args[nArgs], Pt_ARG_COLOR, Pg_BLACK, 0); ++nArgs;
	PtSetArg(&args[nArgs], Pt_ARG_FILL_COLOR, Pg_WHITE, 0); ++nArgs;
	PtSetResources(widget, nArgs, args);

	return( Pt_CONTINUE );

}

void
rebelState::runLine( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	char *str;
	PtGetResource(widget, Pt_ARG_TEXT_STRING, &str, 0);
	strncpy(_runLineLevel, str, 8);

	if(_runLineLevel[0] == '+') {
		strcpy(_rexState.rlflag, "2");
		strcpy(_rexState.rlval, &_runLineLevel[1]);
	}
	else if(_runLineLevel[0] == '-') {
		strcpy(_rexState.rlflag, "3");
		strcpy(_rexState.rlval, &_runLineLevel[1]);
	}
	else {
		strcpy(_rexState.rlflag, "1");
		strcpy(_rexState.rlval, &_runLineLevel[0]);
	}

	app->saved(false);

	return;

}


int
rebelState::rexTimeCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
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
	PtWidget_t *pg = rsrc->panelGroup();
	rebelState *state;
	PtGetResource(pg, Pt_ARG_POINTER, &state, 0);

	// call this state instance of the rexTime function
	state->rexTime(widget, apinfo, cbinfo);

	// reset the colors indicating that the input has been processed
	PtArg_t args[2];
	int nArgs = 0;
	PtSetArg(&args[nArgs], Pt_ARG_COLOR, Pg_BLACK, 0); ++nArgs;
	PtSetArg(&args[nArgs], Pt_ARG_FILL_COLOR, Pg_WHITE, 0); ++nArgs;
	PtSetResources(widget, nArgs, args);

	return( Pt_CONTINUE );

}

void
rebelState::rexTime( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	char *str;
	PtGetResource(widget, Pt_ARG_TEXT_STRING, &str, 0);
	strcpy(_rexState.preset, str);

	app->saved(false);

	return;

}

int
rebelState::rexRandCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
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
	PtWidget_t *pg = rsrc->panelGroup();
	rebelState *state;
	PtGetResource(pg, Pt_ARG_POINTER, &state, 0);

	// call this state instance of the rexRand function
	state->rexRand(widget, apinfo, cbinfo);

	// reset the colors indicating that the input has been processed
	PtArg_t args[2];
	int nArgs = 0;
	PtSetArg(&args[nArgs], Pt_ARG_COLOR, Pg_BLACK, 0); ++nArgs;
	PtSetArg(&args[nArgs], Pt_ARG_FILL_COLOR, Pg_WHITE, 0); ++nArgs;
	PtSetResources(widget, nArgs, args);

	return( Pt_CONTINUE );

}

void
rebelState::rexRand( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	char *str;
	PtGetResource(widget, Pt_ARG_TEXT_STRING, &str, 0);
	strncpy(_rexState.random, str, 8);

	app->saved(false);

	return;

}


int
rebelState::stateCommentCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
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
	PtWidget_t *pg = rsrc->panelGroup();
	rebelState *state;
	PtGetResource(pg, Pt_ARG_POINTER, &state, 0);

	// call this state instance of the stateComment function
	state->stateComment(widget, apinfo, cbinfo);

	// reset the colors indicating that the input has been processed
	PtArg_t args[2];
	int nArgs = 0;
	PtSetArg(&args[nArgs], Pt_ARG_COLOR, Pg_BLACK, 0); ++nArgs;
	PtSetArg(&args[nArgs], Pt_ARG_FILL_COLOR, Pg_WHITE, 0); ++nArgs;
	PtSetResources(widget, nArgs, args);

	return( Pt_CONTINUE );

}

void
rebelState::stateComment( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	char *str;
	PtGetResource(widget, Pt_ARG_TEXT_STRING, &str, 0);
	size_t len = strlen(str);
	if(len == 0) memset(_rexState.comment, 0, maxCmnt);
	else {
		strncpy(_rexState.comment, str, (maxCmnt - 1));
		_rexState.comment[maxCmnt - 1] = 0;
	}

	app->saved(false);

	return;

}

int
rebelState::actionNameCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// don't execute if the user hasn't changed the value
	PgColor_t *fillColor;
	PtGetResource(widget, Pt_ARG_CBOX_TEXT_FILL_COLOR, &fillColor, 0);
	if(*fillColor != Pg_DCYAN) return( Pt_CONTINUE );

	// get a pointer to this state instance
	PtWidget_t *w = ApGetInstance(widget);
	rebelState *state;
	PtGetResource(w, Pt_ARG_POINTER, &state, 0);

	// call this state instance of the actionName function
	state->actionName(widget, apinfo, cbinfo);

	// reset the colors indicating that the input has been processed
	PtArg_t args[2];
	int nArgs = 0;
	PtSetArg(&args[nArgs], Pt_ARG_COLOR, Pg_BLACK, 0); ++nArgs;
	PtSetArg(&args[nArgs], Pt_ARG_CBOX_TEXT_FILL_COLOR, Pg_WHITE, 0); ++nArgs;
	PtSetResources(widget, nArgs, args);

	return( Pt_CONTINUE );

}

void
rebelState::actionName( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	extern rebelResource *rResources;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// determine how this function was called
	if(cbinfo->reason == Pt_CB_ACTIVATE) {
		if((cbinfo->reason_subtype == Pt_EDIT_ACTIVATE) || (cbinfo->reason_subtype == Pt_CHANGE_ACTIVATE)) {
			PtTextCallback_t *txtData = (PtTextCallback_t *)cbinfo->cbdata;
			strcpy(_rexState.device.action, txtData->text);
			rResources->setActionArgLabels(txtData->text);
		}
	}
	else if(cbinfo->reason == Pt_CB_SELECTION) {
		if(cbinfo->reason_subtype == Pt_LIST_SELECTION_FINAL) {
			PtListCallback_t *lstData = (PtListCallback_t *)cbinfo->cbdata;
			strcpy(_rexState.device.action, lstData->item);
			rResources->setActionArgLabels(lstData->item);
		}
	}

	return;
}


int
rebelState::actionArgCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// get a pointer to this state instance
	PtWidget_t *w = ApGetInstance(widget);
	rebelState *state;
	PtGetResource(w, Pt_ARG_POINTER, &state, 0);

	// call this state instance of the actionArg1 function
	state->actionArg(widget, apinfo, cbinfo);

	// reset the colors indicating that the input has been processed
	PtArg_t args[2];
	int nArgs = 0;
	PtSetArg(&args[nArgs], Pt_ARG_COLOR, Pg_BLACK, 0); ++nArgs;
	PtSetArg(&args[nArgs], Pt_ARG_FILL_COLOR, Pg_WHITE, 0); ++nArgs;
	PtSetResources(widget, nArgs, args);

	return( Pt_CONTINUE );

}

void
rebelState::actionArg( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	char *str;
	PtGetResource(widget, Pt_ARG_TEXT_STRING, &str, 0);

	char temp[32];
	if(strlen(str) == 0) strcpy(temp, "0");
	else strcpy(temp, str);

	// determine which argument
	if(ApName(widget) == ABN_arg1Text) {
		strncpy(_rexState.device.act_code, temp, 32);
	}
	else if(ApName(widget) == ABN_arg2Text) {
		strncpy(_rexState.device.act_2code, temp, 32);
	}
	else if(ApName(widget) == ABN_arg3Text) {
		strncpy(_rexState.device.act_3code, temp, 32);
	}
	else if(ApName(widget) == ABN_arg4Text) {
		strncpy(_rexState.device.act_4code, temp, 32);
	}
	else if(ApName(widget) == ABN_arg5Text) {
		strncpy(_rexState.device.act_5code, temp, 32);
	}
	else if(ApName(widget) == ABN_arg6Text) {
		strncpy(_rexState.device.act_6code, temp, 32);
	}
	else if(ApName(widget) == ABN_arg7Text) {
		strncpy(_rexState.device.act_7code, temp, 32);
	}
	else if(ApName(widget) == ABN_arg8Text) {
		strncpy(_rexState.device.act_8code, temp, 32);
	}
	else if(ApName(widget) == ABN_arg9Text) {
		strncpy(_rexState.device.act_9code, temp, 32);
	}
	else if(ApName(widget) == ABN_arg10Text) {
		strncpy(_rexState.device.act_10code, temp, 32);
	}

	return;

}


int
rebelState::rexActionOkCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// get a pointer to this state instance
	PtWidget_t *w = ApGetInstance(widget);
	rebelState *state;
	PtGetResource(w, Pt_ARG_POINTER, &state, 0);

	// call this state instance of the rexActionOk function
	state->rexActionOk(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );

}

void
rebelState::rexActionOk( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	extern rebelResource *rResources;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	int nArgs = buildActionString();

	rResources->setStateWidgets(this);

	setActionArgTypes();
	setActionArgArrays();

	string prototype = "int ";
	prototype += _rexState.device.action;
	prototype += "()";
	app->addFunction(_rexState.device.action, prototype);

	if(nArgs > 0) checkGlobalVariables();

	app->saved(false);

	return;
}

int
rebelState::buildActionString()
{
	if(!strcmp(_rexState.device.action, "0")) return(0);
	else {
		strcpy(_action, _rexState.device.action);
		strcat(_action, "(");
	}

	if(!strcmp(_rexState.device.act_code, "0")) {
		strcat(_action, ")");
		return(0);
	}
	else {
		strcat(_action, _rexState.device.act_code);
	}

	if(!strcmp(_rexState.device.act_2code, "0")) {
		strcat(_action, ")");
		return(1);
	}
	else {
		strcat(_action, ", ");
		strcat(_action, _rexState.device.act_2code);
	}

	if(!strcmp(_rexState.device.act_3code, "0")) {
		strcat(_action, ")");
		return(2);
	}
	else {
		strcat(_action, ", ");
		strcat(_action, _rexState.device.act_3code);
	}

	if(!strcmp(_rexState.device.act_4code, "0")) {
		strcat(_action, ")");
		return(3);
	}
	else {
		strcat(_action, ", ");
		strcat(_action, _rexState.device.act_4code);
	}

	if(!strcmp(_rexState.device.act_5code, "0")) {
		strcat(_action, ")");
		return(4);
	}
	else {
		strcat(_action, ", ");
		strcat(_action, _rexState.device.act_5code);
	}

	if(!strcmp(_rexState.device.act_6code, "0")) {
		strcat(_action, ")");
		return(5);
	}
	else {
		strcat(_action, ", ");
		strcat(_action, _rexState.device.act_6code);
	}

	if(!strcmp(_rexState.device.act_7code, "0")) {
		strcat(_action, ")");
		return(6);
	}
	else {
		strcat(_action, ", ");
		strcat(_action, _rexState.device.act_7code);
	}

	if(!strcmp(_rexState.device.act_8code, "0")) {
		strcat(_action, ")");
		return(7);
	}
	else {
		strcat(_action, ", ");
		strcat(_action, _rexState.device.act_8code);
	}

	if(!strcmp(_rexState.device.act_9code, "0")) {
		strcat(_action, ")");
		return(8);
	}
	else {
		strcat(_action, ", ");
		strcat(_action, _rexState.device.act_9code);
	}

	if(!strcmp(_rexState.device.act_10code, "0")) {
		strcat(_action, ")");
		return(9);
	}
	else {
		strcat(_action, ", ");
		strcat(_action, _rexState.device.act_10code);
		strcat(_action, ")");
	}

	return(10);

}


void
rebelState::setActionArgTypes()
{
	strcpy(_rexState.device.act_code_type, "0");
	strcpy(_rexState.device.act_2code_type, "0");
	strcpy(_rexState.device.act_3code_type, "0");
	strcpy(_rexState.device.act_4code_type, "0");
	strcpy(_rexState.device.act_5code_type, "0");
	strcpy(_rexState.device.act_6code_type, "0");
	strcpy(_rexState.device.act_7code_type, "0");
	strcpy(_rexState.device.act_8code_type, "0");
	strcpy(_rexState.device.act_9code_type, "0");
	strcpy(_rexState.device.act_10code_type, "0");
	
	char argList[1024];
	strcpy(argList, app->protoType(_rexState.device.action).c_str());
	if(!strcmp(argList, "-1")) return;

	// get the first argument type
	int n = 1;
	char *tok;
	if((tok = strtok(argList, ",")) == (char *)NULL) return;
	do {
		//skip any leading white space
		while(isspace((int)(*tok))) ++tok;

		// get the variable type
		int i = 0;
		if(strchr(tok, '*')) {
			while(tok[i] != '*') ++i;
			++i;
		}
		else {
			while(isalpha((int)tok[i])) ++i;
		}

		char argType[16];
		int j;
		for(j = 0; j < i; ++j) argType[j] = tok[j];
		for(; j < 16; ++j) argType[j] = 0;
			
		switch(n) {
		case 1:
			strcpy(_rexState.device.act_code_type, argType);
			break;
		case 2:
			strcpy(_rexState.device.act_2code_type, argType);
			break;
		case 3:
			strcpy(_rexState.device.act_3code_type, argType);
			break;
		case 4:
			strcpy(_rexState.device.act_4code_type, argType);
			break;
		case 5:
			strcpy(_rexState.device.act_5code_type, argType);
			break;
		case 6:
			strcpy(_rexState.device.act_6code_type, argType);
			break;
		case 7:
			strcpy(_rexState.device.act_7code_type, argType);
			break;
		case 8:
			strcpy(_rexState.device.act_8code_type, argType);
			break;
		case 9:
			strcpy(_rexState.device.act_9code_type, argType);
			break;
		case 10:
			strcpy(_rexState.device.act_10code_type, argType);
			break;
			
		}

		// increment the variable number
		++n;

	} while((tok = strtok(NULL, ",")) != (char *)NULL);

	return;
}


void rebelState::setActionArgArrays()
{
	strcpy(_rexState.device.act_code_array, "0");
	strcpy(_rexState.device.act_2code_array, "0");
	strcpy(_rexState.device.act_3code_array, "0");
	strcpy(_rexState.device.act_4code_array, "0");
	strcpy(_rexState.device.act_5code_array, "0");
	strcpy(_rexState.device.act_6code_array, "0");
	strcpy(_rexState.device.act_7code_array, "0");
	strcpy(_rexState.device.act_8code_array, "0");
	strcpy(_rexState.device.act_9code_array, "0");
	strcpy(_rexState.device.act_10code_array, "0");

	string argType;
	string argCode;
	string argArray;
	for(int n = 1; n <= 10; ++n) {
		switch(n) {
		case 1:
			argType = _rexState.device.act_code_type;
			argCode = _rexState.device.act_code;
			break;
		case 2:
			argType = _rexState.device.act_2code_type;
			argCode = _rexState.device.act_2code;
			break;
		case 3:
			argType = _rexState.device.act_3code_type;
			argCode = _rexState.device.act_3code;
			break;
		case 4:
			argType = _rexState.device.act_4code_type;
			argCode = _rexState.device.act_4code;
			break;
		case 5:
			argType = _rexState.device.act_5code_type;
			argCode = _rexState.device.act_5code;
			break;
		case 6:
			argType = _rexState.device.act_6code_type;
			argCode = _rexState.device.act_6code;
			break;
		case 7:
			argType = _rexState.device.act_7code_type;
			argCode = _rexState.device.act_7code;
			break;
		case 8:
			argType = _rexState.device.act_8code_type;
			argCode = _rexState.device.act_8code;
			break;
		case 9:
			argType = _rexState.device.act_9code_type;
			argCode = _rexState.device.act_9code;
			break;
		case 10:
			argType = _rexState.device.act_10code_type;
			argCode = _rexState.device.act_10code;
			break;
		}

		// if there is no argument, just return
		if(!strcmp(argType.c_str(), "0")) return;
		
		// if the argument type is not a pointer,
		// the argument is not an array
		if(argType.find("*") == string::npos) argArray = "0";
		else {
			// if there is an ampersand but no bracket,
			// the argument is not an array
			if((argCode[0] == '&') && (argCode.find("[") ==  string::npos)) argArray = "0";

			// if there is a bracket
			// the argument is an array
			else argArray = "[]";
		}

		switch(n) {
		case 1:
			strcpy(_rexState.device.act_code_array, argArray.c_str());
			break;
		case 2:
			strcpy(_rexState.device.act_2code_array, argArray.c_str());
			break;
		case 3:
			strcpy(_rexState.device.act_3code_array, argArray.c_str());
			break;
		case 4:
			strcpy(_rexState.device.act_4code_array, argArray.c_str());
			break;
		case 5:
			strcpy(_rexState.device.act_5code_array, argArray.c_str());
			break;
		case 6:
			strcpy(_rexState.device.act_6code_array, argArray.c_str());
			break;
		case 7:
			strcpy(_rexState.device.act_7code_array, argArray.c_str());
			break;
		case 8:
			strcpy(_rexState.device.act_8code_array, argArray.c_str());
			break;
		case 9:
			strcpy(_rexState.device.act_9code_array, argArray.c_str());
			break;
		case 10:
			strcpy(_rexState.device.act_10code_array, argArray.c_str());
			break;
		}
	}
}

int
rebelState::clearActionArgsCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// get a pointer to this state instance
	PtWidget_t *w = ApGetInstance(widget);
	rebelState *state;
	PtGetResource(w, Pt_ARG_POINTER, &state, 0);

	// call this state instance of the clearActionArgs function
	state->clearActionArgs(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );

}

void
rebelState::clearActionArgs( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	strcpy(_rexState.device.action, "0");
	strcpy(_rexState.device.act_code, "0");
	strcpy(_rexState.device.act_2code, "0");
	strcpy(_rexState.device.act_3code, "0");
	strcpy(_rexState.device.act_4code, "0");
	strcpy(_rexState.device.act_5code, "0");
	strcpy(_rexState.device.act_6code, "0");
	strcpy(_rexState.device.act_7code, "0");
	strcpy(_rexState.device.act_8code, "0");
	strcpy(_rexState.device.act_9code, "0");
	strcpy(_rexState.device.act_10code, "0");

	return;

}

int
rebelState::escapeOnCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
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
	PtWidget_t *pg = rsrc->panelGroup();
	rebelState *state;
	PtGetResource(pg, Pt_ARG_POINTER, &state, 0);

	// call this state instance of the escapeOn function
	state->escapeOn(widget, apinfo, cbinfo);

	// reset the colors indicating that the input has been processed
	PtArg_t args[2];
	int nArgs = 0;
	PtSetArg(&args[nArgs], Pt_ARG_COLOR, Pg_BLACK, 0); ++nArgs;
	PtSetArg(&args[nArgs], Pt_ARG_FILL_COLOR, Pg_WHITE, 0); ++nArgs;
	PtSetResources(widget, nArgs, args);

	return( Pt_CONTINUE );

}

void
rebelState::escapeOn( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	ESCAPE *escp;
	PtGetResource(ABW_escapeResourceScrollContainer, Pt_ARG_POINTER, &escp, 0);

	char *str;
	PtGetResource(widget, Pt_ARG_TEXT_STRING, &str, 0);
	strncpy(escp->escapeOn, str, 128);

	parseEscapeOn(escp);

	app->saved(false);

	return;

}


void
rebelState::parseEscapeOn(ESCAPE *escp)
{
	char temp[128];

	// make a cpy of the escape string for strtok to mangle
	strcpy(temp, escp->escapeOn);

	// the escape on string must contain three tokens;
	// a constant, a test, and a variable name or address
	char *cnstnt = strtok(temp, " ");
	if(!cnstnt) return;
	char *test = strtok(NULL, " ");
	if(!test) return;
	char *name = strtok(NULL, " ");
	if(!name) return;

	if(!strcmp(test, "&")) {
		if(cnstnt[0] == '+') {
			strcpy(escp->sflags, "BITON");
			strcpy(escp->smask, &cnstnt[1]);
		}
		else if(cnstnt[0] == '-') {
			strcpy(escp->sflags, "BITOFF");
			strcpy(escp->smask, &cnstnt[1]);
		}
	}
	else if(!strcmp(test, "%")) {
		strcpy(escp->sflags, "FUNC");
		strcpy(escp->smask, cnstnt);
	}
	else if(!strcmp(test, "<")) {
		strcpy(escp->sflags, "LESSOR");
		strcpy(escp->smask, cnstnt);
	}
	else if(!strcmp(test, ">")) {
		strcpy(escp->sflags, "GREATER");
		strcpy(escp->smask, cnstnt);
	}
	else if(!strcmp(test, "=")) {
		strcpy(escp->sflags, "EQUAL");
		strcpy(escp->smask, cnstnt);
	}
	else if(!strcmp(test, "?")) {
		strcpy(escp->sflags, "QUERY");
		strcpy(escp->smask, cnstnt);
	}

	if(islower((int)name[0])) sprintf(escp->funct, "&%s", name);
	else strcpy(escp->funct, name);

	return;
	
}


int
rebelState::escapeCommentCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
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
	PtWidget_t *pg = rsrc->panelGroup();
	rebelState *state;
	PtGetResource(pg, Pt_ARG_POINTER, &state, 0);

	// call this state instance of the escapeOn function
	state->escapeComment(widget, apinfo, cbinfo);

	// reset the colors indicating that the input has been processed
	PtArg_t args[2];
	int nArgs = 0;
	PtSetArg(&args[nArgs], Pt_ARG_COLOR, Pg_BLACK, 0); ++nArgs;
	PtSetArg(&args[nArgs], Pt_ARG_FILL_COLOR, Pg_WHITE, 0); ++nArgs;
	PtSetResources(widget, nArgs, args);

	return( Pt_CONTINUE );

}

void
rebelState::escapeComment( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	ESCAPE *escp;
	PtGetResource(ABW_escapeResourceScrollContainer, Pt_ARG_POINTER, &escp, 0);

	char *str;
	PtGetResource(widget, Pt_ARG_TEXT_STRING, &str, 0);
	size_t len = strlen(str);
	if(len == 0) memset(escp->comment, 0, maxCmnt);
	else {
		strncpy(escp->comment, str, (maxCmnt - 1));
		escp->comment[maxCmnt - 1] = 0;
	}

	app->saved(false);

	return;

}


int
rebelState::escapeDialogCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// reset the colors indicating that the input has been processed
	PtArg_t args[2];
	int nArgs = 0;
	PtSetArg(&args[nArgs], Pt_ARG_COLOR, Pg_BLACK, 0); ++nArgs;
	PtSetArg(&args[nArgs], Pt_ARG_FILL_COLOR, Pg_WHITE, 0); ++nArgs;
	PtSetResources(widget, nArgs, args);

	return( Pt_CONTINUE );

}


int
rebelState::escapeOkCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// get a pointer to the from state instance
	PtWidget_t *w = ApGetInstance(widget);
	ESCAPE *escp;
	PtGetResource(w, Pt_ARG_POINTER, &escp, 0);
	rebelState *fromState;
	PtGetResource(escp->ePath, Pt_ARG_POINTER, &fromState, 0);

	// call the from state instance of the sign callback
	fromState->escapeOk(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );

}

void
rebelState::escapeOk( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	extern rebelResource *rResources;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// get pointer to the escape instance
	PtWidget_t *w = ApGetInstance(widget);
	ESCAPE *escp;
	PtGetResource(w, Pt_ARG_POINTER, &escp, 0);

	// determine if the sign toggles were set
	long plusFlag = PtWidgetFlags(ABW_escapePlusToggleButton);
	long minusFlag = PtWidgetFlags(ABW_escapeMinusToggleButton);
	char escapeSign = '+';
	if(plusFlag & Pt_SET) escapeSign = '+';
	else if(minusFlag & Pt_SET) escapeSign  = '-';
	else if((plusFlag & !Pt_SET) && (minusFlag & !Pt_SET)) escapeSign = '+';

	// set this escape instance's sflags string
	long condFlag;
	condFlag = PtWidgetFlags(ABW_escapeAmpersandToggleButton);
	if(condFlag & Pt_SET) {
		if(escapeSign == '+') strcpy(escp->sflags, "BITON");
		else if(escapeSign == '-') strcpy(escp->sflags, "BITOFF");
	}

	condFlag = PtWidgetFlags(ABW_escapePercentToggleButton);
	if(condFlag & Pt_SET) {
		strcpy(escp->sflags, "FUNC");
	}

	condFlag = PtWidgetFlags(ABW_escapeGreaterToggleButton);
	if(condFlag & Pt_SET) {
		strcpy(escp->sflags, "GREATER");
	}

	condFlag = PtWidgetFlags(ABW_escapeLessToggleButton);
	if(condFlag & Pt_SET) {
		strcpy(escp->sflags, "LESSOR");
	}

	condFlag = PtWidgetFlags(ABW_escapeEqualToggleButton);
	if(condFlag & Pt_SET) {
		strcpy(escp->sflags, "EQUAL");
	}

	condFlag = PtWidgetFlags(ABW_escapeQuestionToggleButton);
	if(condFlag & Pt_SET) {
		strcpy(escp->sflags, "QUERY");
	}

	// set this escape instance's funct string
	char *str;
	PtGetResource(ABW_escapeVariableText, Pt_ARG_TEXT_STRING, &str, 0);
	if(islower(str[0])) sprintf(escp->funct, "&%s", str);
	else strcpy(escp->funct, str);

	// set this escape instance's smask string
	PtGetResource(ABW_escapeConstantText, Pt_ARG_TEXT_STRING, &str, 0);
	strcpy(escp->smask, str);

	// build this escape's escapeOn string
	buildEscapeString(escp);

	rResources->setEscapeWidgets(escp);

	app->saved(false);

	return;

}


void
rebelState::buildEscapeString(ESCAPE *escp)
{
	// clear the escapeOn string
	strcpy(escp->escapeOn, "");

	// copy the global smask string and sign characters from the dialog
	// to the escapeOn string
	if(!strcmp(escp->sflags, "TIME")) {
		strcpy(escp->escapeOn, escp->sflags);
		return;
	}
	else if(!strcmp(escp->sflags, "BITON")) {
		escp->escapeOn[0] = '+';
		strcpy(&escp->escapeOn[1], escp->smask);
		strcat(escp->escapeOn, " & ");
	}
	else if(!strcmp(escp->sflags, "BITOFF")) {
		escp->escapeOn[0] = '-';
		strcpy(&escp->escapeOn[1], escp->smask);
		strcat(escp->escapeOn, " & ");
	}
	else {
		strcpy(escp->escapeOn, escp->smask);
	}

	// add the sflags string to the escapeOn string
	if(!strcmp(escp->sflags, "FUNC")) strcat(escp->escapeOn, " % ");
	else if(!strcmp(escp->sflags, "GREATER")) strcat(escp->escapeOn, " > ");
	else if(!strcmp(escp->sflags, "LESSOR")) strcat(escp->escapeOn, " < ");
	else if(!strcmp(escp->sflags, "EQUAL")) strcat(escp->escapeOn, " = ");
	else if(!strcmp(escp->sflags, "QUERY")) strcat(escp->escapeOn, " ? ");

	// add the funct string to the escapeOn string
	if(escp->funct[0] == '&') strcat(escp->escapeOn, &escp->funct[1]);
	else strcat(escp->escapeOn, &escp->funct[0]);
}

ESCAPE *
rebelState::newEscape(rebelState *toState)
{
	// don't let the user define more than 10 escapes for this state
	if(++_escapeIndx >= (int)maxEscp) return((ESCAPE *)NULL);

	sprintf(_rexState.stateEscapes[_escapeIndx].stateptr, "&%s", toState->sname());

	ESCAPE *escp = &_rexState.stateEscapes[_escapeIndx];
	strcpy(escp->sflags, "TIME");
	strcpy(escp->escapeOn, escp->sflags);

	return(escp);
}


int
rebelState::escapeResourcesCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// get pointer to this state
	rebelState *state;
	PtGetResource(widget, Pt_ARG_POINTER, &state, 0);

	// call this state's escape resources function
	if(state) state->escapeResources(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );
}

void
rebelState::escapeResources( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	extern rebelResource *rResources;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// make this widget the selected object
	rEdit.selectObject(widget);

	ESCAPE **escp;
	PtGetResource(widget, Pt_ARG_USER_DATA, &escp, 0);
	sprintf((*escp)->stateptr, "&%s", (*escp)->toState->sname());
	rResources->setEscapeWidgets(*escp);

	return;
}


int
rebelState::nextRightSocket()
{
	int nxtSocket = _rightSockets[_rightSocketIndx];
	++_rightSocketIndx;
	if(_rightSocketIndx >= 10) _rightSocketIndx = 0;
	return(nxtSocket);
}

int
rebelState::nextLeftSocket()
{
	int nxtSocket = _leftSockets[_leftSocketIndx];
	++_leftSocketIndx;
	if(_leftSocketIndx >= 10) _leftSocketIndx = 0;
	return(nxtSocket);
}

void
rebelState::cutEscape( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	ESCAPE **pescp;
	PtGetResource(widget, Pt_ARG_USER_DATA, &pescp, 0);
	ESCAPE *escps = _rexState.stateEscapes;
	int escpIndx = *pescp - escps;

	char text[128];
	sprintf(text, "Do you really want to cut state %s escape to %s?", _name,
			_rexState.stateEscapes[escpIndx].toState->name());
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
		this->cutEscape(escpIndx);
		break;
	}
}

void rebelState::cutEscape(int i)
{
	ESCAPE *e;

	// delete the path widget
	PtDestroyWidget(_rexState.stateEscapes[i].ePath);

	// move all higher escapes up
	for(uint32_t j = i; j < maxEscp - 1; ++j) {
		_rexState.stateEscapes[j] = _rexState.stateEscapes[j + 1];
		e = &_rexState.stateEscapes[j];
		if(e->ePath != (PtWidget_t *)NULL) {
			PtSetResource(e->ePath, Pt_ARG_USER_DATA, &e, sizeof(ESCAPE *));
		}
	}

	e = &_rexState.stateEscapes[maxEscp - 1];
	strcpy(e->nextesc, "0");
	strcpy(e->awakestate, "0");
	strcpy(e->stateptr, "0");
	strcpy(e->sflags, "0");
	strcpy(e->funct, "0");
	strcpy(e->smask, "0");
	e->ePath = (PtWidget_t *)NULL;
	e->toState = (rebelState *)NULL;
	strcpy(e->escapeOn, "");

	--_escapeIndx;

	app->saved(false);

	return;
}

void
rebelState::saveButton(FILE *fp)
{
}

void
rebelState::portButton(rebelWin *w, string line)
{
	string localLine;
	
	// get the first line of this state
	localLine = w->lineTok((char *)NULL);

	// get the state comment if there is one
	size_t slashPos;
	if((slashPos = localLine.find("// ")) != string::npos) {
		localLine.erase(0, (slashPos + 3));
		size_t nlPos;
		if((nlPos = localLine.find("\n")) != string::npos) {
			localLine.erase(nlPos);
		}
		strncpy(_rexState.comment, localLine.c_str(), (maxCmnt - 1));
		_rexState.comment[maxCmnt - 1] = 0;

		// get the line containing the state and ecode
		localLine = w->lineTok((char *)NULL);
	}

	// get the name of the state and the ecode
	scanStateName(w, localLine);

	// get the running line value
	localLine = w->lineTok((char *)NULL);
	scanRunLine(w, localLine);

	// get the action and arguments
	localLine = w->lineTok((char *)NULL);
	scanAction(w, localLine);
	if(strcmp(_rexState.device.action, "0")) {
		buildActionString();
	}

	// get the time and rand values
	localLine = w->lineTok((char *)NULL);
	scanTime(w, localLine);

	// discard the opening brace of the escape definitions
	localLine = w->lineTok((char *)NULL);

	// get the escapes
	for(uint32_t i = 0; i < maxEscp; ++i) {
		localLine = w->lineTok((char *)NULL);
		if(localLine.find("&") != string::npos) {
			scanEscape(w, localLine, &_rexState.stateEscapes[i]);
			++_escapeIndx;
		}
	}

	// discard the closing brace of the escape definitions
	localLine = w->lineTok((char *)NULL);

	// if this line is not the closing brace of the state definition
	// get one more line
	localLine = w->lineTok((char *)NULL);

	if(localLine.find("}") == string::npos) {
		localLine = w->lineTok((char *)NULL);
	}
	return;
}

void
rebelState::scanStateName(rebelWin *w, string line)
{
	string tmp = line;

	// convert punctuation to spaces for sscanf
	for(size_t i = 0; i < tmp.size(); ++i) {
		if((tmp[i] == '{') || (tmp[i] == '\"') || (tmp[i] == '}') || (tmp[i] == ',')) {
			tmp[i] = ' ';
		}
	}

	sscanf(tmp.c_str(), "%s %s %s",
		   _rexState.statename, _rexState.code, _rexState.ptrCode);

	return;
}

void
rebelState::scanRunLine(rebelWin *w, string line)
{
	string tmp = line;
	
	// convert punctuation to spaces for sscanf
	for(size_t i = 0; i < tmp.size(); ++i) {
		if((tmp[i] == '{') || (tmp[i] == '\"') || (tmp[i] == '}') || (tmp[i] == ',')) {
			tmp[i] = ' ';
		}
	}

	sscanf(tmp.c_str(), "%s %s", _rexState.rlflag, _rexState.rlval);

	if(!strcmp(_rexState.rlflag, "1")) {
		strcpy(_runLineLevel, _rexState.rlval);
	}
	else if(!strcmp(_rexState.rlflag, "2")) {
		strcpy(_runLineLevel, "+");
		strcat(_runLineLevel, _rexState.rlval);
	}
	else if(!strcmp(_rexState.rlflag, "3")) {
		strcpy(_runLineLevel, "-");
		strcat(_runLineLevel, _rexState.rlval);
	}

	return;
}

void
rebelState::scanAction(rebelWin *w, string line)
{
	string tmp = line;

	// convert punctuation to spaces for sscanf
	for(size_t i = 0; i < tmp.size(); ++i) {
		if((tmp[i] == '{') || (tmp[i] == '\"') || (tmp[i] == '}') || (tmp[i] == ',')) {
			tmp[i] = ' ';
		}
	}

	sscanf(tmp.c_str(), "%s %s %s %s %s %s %s %s %s %s %s",
		   _rexState.device.action, _rexState.device.act_code, _rexState.device.act_2code,
		   _rexState.device.act_3code, _rexState.device.act_4code,
		   _rexState.device.act_5code, _rexState.device.act_6code,
		   _rexState.device.act_7code, _rexState.device.act_8code,
		   _rexState.device.act_9code, _rexState.device.act_10code);

	// if there is no action, just return
	if(!strcmp(_rexState.device.action, "0")) return;

	// otherwise set the action types and check global variables
	setActionArgTypes();
	setActionArgArrays();

	string prototype = "int ";
	prototype += _rexState.device.action;
	prototype += "()";
	app->addFunction(_rexState.device.action, prototype);

	checkGlobalVariables();

	return;
}

void
rebelState::scanTime(rebelWin *w, string line)
{
	string tmp = line;

	// convert punctuation to spaces for sscanf
	for(size_t i = 0; i < tmp.size(); ++i) {
		if((tmp[i] == '{') || (tmp[i] == '\"') || (tmp[i] == '}') || (tmp[i] == ',')) {
			tmp[i] = ' ';
		}
	}

	sscanf(tmp.c_str(), "%s %s", _rexState.preset, _rexState.random);
	
	return;
}

void
rebelState::scanEscape(rebelWin *w, string line, ESCAPE *escp)
{
	string tmp = line;

	// look for a comment
	size_t slashPos;
	if((slashPos = tmp.find("// ")) != string::npos) {
		tmp.erase(0, (slashPos + 3));
		size_t nlPos;
		if((nlPos = tmp.find("\n")) != string::npos) {
			tmp.erase(nlPos);
		}
		strncpy(escp->comment, tmp.c_str(), (maxCmnt - 1));
		escp->comment[maxCmnt -1] = 0;

		// reset the tmp string
		tmp = line;
		slashPos = tmp.find("//");
		tmp.erase(slashPos);
	}
	
	// convert punctuation to spaces for sscanf
	for(size_t i = 0; i < tmp.size(); ++i) {
		if((tmp[i] == '{') || (tmp[i] == '\"') || (tmp[i] == '}') || (tmp[i] == ',')) {
			tmp[i] = ' ';
		}
	}

	sscanf(tmp.c_str(), "%s %s %s %s %s %s",
		   escp->nextesc, escp->awakestate, escp->stateptr,
		   escp->sflags, escp->funct, escp->smask);

	// build the escapeOn string
	buildEscapeString(escp);

	return;
}

void
rebelState::genCode(FILE *fp)
{
	int rebelFlag1 = REBEL_FLAG1;
	int rebelFlag2 = REBEL_FLAG2;

	fprintf(fp, "STATE %s = {  // %hd %hd\n", _sname, _origin.x, _origin.y);
	if(strlen(_rexState.comment)) fprintf(fp, "// %s\n", _rexState.comment);
	fprintf(fp, "\t{\"%s\"},%s,%s,\n", _rexState.statename, _rexState.code, _rexState.ptrCode);
	fprintf(fp, "\t%s,%s,\n", _rexState.rlflag, _rexState.rlval);
	fprintf(fp, "\t{%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s},\n", _rexState.device.action,
			_rexState.device.act_code,
			_rexState.device.act_2code,
			_rexState.device.act_3code,
			_rexState.device.act_4code,
			_rexState.device.act_5code,
			_rexState.device.act_6code,
			_rexState.device.act_7code,
			_rexState.device.act_8code,
			_rexState.device.act_9code,
			_rexState.device.act_10code);
	fprintf(fp, "\t%s,%s,\n", _rexState.preset, _rexState.random);
	fprintf(fp, "\t{\n");
	for(uint32_t i = 0; i < maxEscp; ++i) {
		if(_rexState.stateEscapes[i].toState != (rebelState *)NULL) {
			sprintf(_rexState.stateEscapes[i].stateptr, "&%s", _rexState.stateEscapes[i].toState->sname());
		}
		fprintf(fp, "\t\t{%s,%s,%s,%s,%s,%s},",
				_rexState.stateEscapes[i].nextesc,
				_rexState.stateEscapes[i].awakestate,
				_rexState.stateEscapes[i].stateptr,
				_rexState.stateEscapes[i].sflags,
				_rexState.stateEscapes[i].funct,
				_rexState.stateEscapes[i].smask);
		if(strlen(_rexState.stateEscapes[i].comment)) {
			fprintf(fp, " // %s\n", _rexState.stateEscapes[i].comment);
		}
		else fprintf(fp, "\n");
	}
	fprintf(fp, "\t},\n");
	fprintf(fp, "\t0,%d,%d,%hd,%hd\n", rebelFlag1, rebelFlag2, _origin.x, _origin.y);
	fprintf(fp, "};\n");

}

void
rebelState::genSpot(FILE *fp)
{
	fprintf(fp, "\t%s:\n", _rexState.statename);

	// print the ecode command if there is one
	if(strcmp(_rexState.code, "0")) fprintf(fp, "\t\tcode %s\n", _rexState.code);
	else if(strcmp(_rexState.ptrCode, "0")) fprintf(fp, "\t\tcode %s\n", _rexState.ptrCode);

	// print the action if there is one
	if(strcmp(_rexState.device.action, "0")) {
		fprintf(fp, "\t\tdo %s(", _rexState.device.action);
		if(strcmp(_rexState.device.act_code, "0")) fprintf(fp, "%s", _rexState.device.act_code);
		else {
			fprintf(fp, ")\n");
			goto doRunLine;
		}
		if(strcmp(_rexState.device.act_2code, "0")) fprintf(fp, ", %s", _rexState.device.act_2code);
		else {
			fprintf(fp, ")\n");
			goto doRunLine;
		}
		if(strcmp(_rexState.device.act_3code, "0")) fprintf(fp, ", %s", _rexState.device.act_3code);
		else {
			fprintf(fp, ")\n");
			goto doRunLine;
		}
		if(strcmp(_rexState.device.act_4code, "0")) fprintf(fp, ", %s", _rexState.device.act_4code);
		else {
			fprintf(fp, ")\n");
			goto doRunLine;
		}
		if(strcmp(_rexState.device.act_5code, "0")) fprintf(fp, ", %s", _rexState.device.act_5code);
		else {
			fprintf(fp, ")\n");
			goto doRunLine;
		}
		if(strcmp(_rexState.device.act_6code, "0")) fprintf(fp, ", %s", _rexState.device.act_6code);
		else {
			fprintf(fp, ")\n");
			goto doRunLine;
		}
		if(strcmp(_rexState.device.act_7code, "0")) fprintf(fp, ", %s", _rexState.device.act_7code);
		else {
			fprintf(fp, ")\n");
			goto doRunLine;
		}
		if(strcmp(_rexState.device.act_8code, "0")) fprintf(fp, ", %s", _rexState.device.act_8code);
		else {
			fprintf(fp, ")\n");
			goto doRunLine;
		}
		if(strcmp(_rexState.device.act_9code, "0")) fprintf(fp, ", %s", _rexState.device.act_9code);
		else {
			fprintf(fp, ")\n");
			goto doRunLine;
		}
		if(strcmp(_rexState.device.act_10code, "0")) fprintf(fp, ", %s)\n", _rexState.device.act_10code);
	}

	doRunLine:
	// print the running line value it there is one
	if(strcmp(_rexState.rlval, "0")) {
		if(!strcmp(_rexState.rlflag, "1")) fprintf(fp, "\t\trl %s\n", _rexState.rlval);
		else if(!strcmp(_rexState.rlflag, "2")) fprintf(fp, "\t\trl +%s\n", _rexState.rlval);
		else if(!strcmp(_rexState.rlflag, "3")) fprintf(fp, "\t\trl -%s\n", _rexState.rlval);
	}

	// print out the time value if there is one
	if(strcmp(_rexState.preset, "0")) fprintf(fp, "\t\ttime %s\n", _rexState.preset);

	// print out the rand value if there is one
	if(strcmp(_rexState.random, "0")) fprintf(fp, "\t\trand %s\n", _rexState.random);

	// print out the escapes
	for(uint32_t ei = 0; ei < maxEscp; ++ei) {
		ESCAPE *e = &_rexState.stateEscapes[ei];
		if(strcmp(e->stateptr, "0")) {
			if(strstr(e->escapeOn, "TIME") != (char *)NULL) {
				fprintf(fp, "\t\tto %s\n", &e->stateptr[2]);
			}
			else {
				fprintf(fp, "\t\tto %s on %s\n", &e->stateptr[2], e->escapeOn);
			}
		}
		else break;
	}
}

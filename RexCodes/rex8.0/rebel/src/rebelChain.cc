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
extern ApDBase_t *rPictures;

PgColor_t rebelChain::_colorList[10] = {
	PgRGB(255, 0, 0),
	PgRGB(255, 128, 0),
	PgRGB(255, 255, 0),
	PgRGB(0, 255, 0),
	PgRGB(0, 255, 255),
	PgRGB(0, 0, 255),
	PgRGB(255, 0, 255),
	PgRGB(160, 0, 255),
	PgRGB(254, 85, 160),
	PgRGB(254, 170, 170),
};
int rebelChain::_colorListIndx = 0;

rebelChain::rebelChain() : rebelWin()
{
	_winTyp = REBEL_CHAIN;

	strcpy(_restartFunc, "");

	_abortList.clear();

	_addEscape = false;

	_fromState = (rebelState *)NULL;
	_toState = (rebelState *)NULL;
}

rebelChain::rebelChain(const char *n, PtCallbackInfo_t *cbinfo) : rebelWin(n)
{
	_winTyp = REBEL_CHAIN;

	PhDim_t s =  { 450, 350 };
	_prefSize = s;

	cbinfo->cbdata = (void *)this;
	ApModuleParent(ABM_rebelWindow, AB_PARENT, ABW_rebelBkgd);
	ApModuleLocation(ABM_rebelWindow, AB_LOC_REL_MODULE, _nextOrigin.x, _nextOrigin.y);
	_window = ApCreateModule(ABM_rebelWindow, NULL, cbinfo);

	strcpy(_restartFunc, "");

	_abortList.clear();

	_addEscape = false;

	_fromState = (rebelState *)NULL;
	_toState = (rebelState *)NULL;
}

rebelChain::rebelChain(const char *n, PhPoint_t origin, PhDim_t size, PtCallbackInfo_t *cbinfo) : rebelWin(n, size)
{
	_winTyp = REBEL_CHAIN;

	cbinfo->cbdata = (void *)this;
	ApModuleParent(ABM_rebelWindow, AB_PARENT, ABW_rebelBkgd);
	ApModuleLocation(ABM_rebelWindow, AB_LOC_REL_MODULE, origin.x, origin.y);
	_window = ApCreateModule(ABM_rebelWindow, NULL, cbinfo);

	strcpy(_restartFunc, "");

	_abortList.clear();

	_addEscape = false;

	_fromState = (rebelState *)NULL;
	_toState = (rebelState *)NULL;
}

rebelChain::rebelChain(const rebelChain& rc) : rebelWin(rc)
{
	_addEscape = rc._addEscape;

	for(int i = 0; i < 12; ++i) _escpPoints[i] = rc._escpPoints[i];

	_abortList = rc._abortList;

	_fromState = rc._fromState;
	_toState = rc._toState;
	if(strlen(rc._restartFunc)) strcpy(_restartFunc, rc._restartFunc);
}


rebelChain::~rebelChain()
{
	_abortList.clear();

	if(strlen(_restartFunc)) {
		app->deleteAction(_restartFunc);
	}
}

rebelChain& rebelChain::operator=(const rebelChain& rc)
{
	_winTyp = rc._winTyp;

	strcpy(_name, rc._name);
	_addButton = rc._addButton;
	_addEscape = rc._addEscape;

	_window = rc._window;
	_scrollContainer = rc._scrollContainer;
	_scrollCanvas = rc._scrollCanvas;
	_scrollCanvasExtent = rc._scrollCanvasExtent;
	_draggedWidget = rc._draggedWidget;
	for(int i = 0; i < 12; ++i) _escpPoints[i] = rc._escpPoints[i];

	_origin = rc._origin;
	_size = rc._size;
	_abortList = rc._abortList;

	_fromState = rc._fromState;
	_toState = rc._toState;

	return(*this);
}


void rebelChain::cut( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	char text[128];
	sprintf(text, "Do you really want to cut chain %s?", _name);
	char *btns[] = { "Cancel", "Cut" };
	int answer = PtAlert(ABW_rebelBaseWindow, NULL,
						 "Cut Chain", NULL,
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
rebelChain::chainNameCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// don't execute if the user hasn't changed the value
	PgColor_t *fillColor;
	PtGetResource(widget, Pt_ARG_FILL_COLOR, &fillColor, 0);
	if(*fillColor != Pg_DCYAN) return( Pt_CONTINUE );

	// get a pointer to this chain instance
	PtWidget_t *w = ApGetInstance(widget);
	rebelResource *rsrc;
	PtGetResource(w, Pt_ARG_POINTER, &rsrc, 0);
	if(rsrc) {
		PtWidget_t *pg = rsrc->panelGroup();
		rebelChain *chain;
		PtGetResource(pg, Pt_ARG_POINTER, &chain, 0);

		// call this chain instance of the chainName function
		if(chain) chain->chainName(widget, apinfo, cbinfo);
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
rebelChain::chainName( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	char *str;
	PtGetResource(widget, Pt_ARG_TEXT_STRING, &str, 0);
	strcpy(_name, str);

	char title[64];
	strcpy(title, "CHAIN : ");
	strcat(title, _name);
	PtSetResource(_window, Pt_ARG_WINDOW_TITLE, title, 0);

	return;

}


int
rebelChain::restartFunctionCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
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
	rebelChain *chain;
	PtGetResource(pg, Pt_ARG_POINTER, &chain, 0);

	// call this state instance of the ecode function
	chain->restartFunction(widget, apinfo, cbinfo);

	// reset the colors indicating that the input has been processed
	PtArg_t args[2];
	int nArgs = 0;
	PtSetArg(&args[nArgs], Pt_ARG_COLOR, Pg_BLACK, 0); ++nArgs;
	PtSetArg(&args[nArgs], Pt_ARG_FILL_COLOR, Pg_WHITE, 0); ++nArgs;
	PtSetResources(widget, nArgs, args);

	return( Pt_CONTINUE );

}

void
rebelChain::restartFunction( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	char *str;
	PtGetResource(widget, Pt_ARG_TEXT_STRING, &str, 0);
	strcpy(_restartFunc, str);

	string prototype = "void ";
	prototype += _restartFunc;
	prototype += "()";
	if(app) app->addFunction(_restartFunc, prototype);

	return;

}


void
rebelChain::addButton( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// the base class function selects this window and
	// gets a position for the new button if one is to be created
	rebelWin::addButton(widget, apinfo, cbinfo);

	// create a button to represent the new state
	int n = _buttons.size();
	char defName[64];
	sprintf(defName, "%s%d", _name, n);
	rebelState *state = new rebelState(_scrollCanvas, defName, _newPos);
	_buttons.push_back(state);

	// mark the newly created button as selected.
	rEdit.selectObject(state->button());
	_activeBtnPtr = _buttons.end() - 1;

	// if the control key is not depressed, get out of state adding mode
	PhPointerEvent_t *pe;
	pe = (PhPointerEvent_t *)PhGetData(cbinfo->event);
	if(!(pe->key_mods & KEYMOD_CTRL)) {
		PtSetResource(widget, Pt_ARG_CURSOR_TYPE, Ph_CURSOR_INHERIT, 0);
		_addButton = false;
	}

	state->resource(widget, apinfo, cbinfo);

	app->saved(false);

	return;

}


int
rebelChain::newStateCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(app == (rebelApp *)NULL) return( Pt_CONTINUE );

	// get pointer to this chain instance
	rebelWin *w = app->activeWindow();
	if(w == (rebelWin *)NULL) return( Pt_CONTINUE );
	if(w->winTyp() != REBEL_CHAIN) return( Pt_CONTINUE );

	w->newButton(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );

}

int
rebelChain::newEscapeCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// get pointer to this chain instance
	if(app == (rebelApp *)NULL) return( Pt_CONTINUE );

	rebelChain *c = (rebelChain *)app->activeWindow();
	if(c == (rebelWin *)NULL) return( Pt_CONTINUE );
	if(c->winTyp() != REBEL_CHAIN) return( Pt_CONTINUE );

	c->newEscape(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );

}

void
rebelChain::newEscape( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	_addEscape = true;

	return;

}


void
rebelChain::eventDrag( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// the base class function actually moves the button
	rebelWin::eventDrag(widget, apinfo, cbinfo);

	// calculate new escape paths for the states in this chain
	if((*_activeBtnPtr)->butType() == REBEL_STATE) {
		escapePath();
	}

	return;
}


void
rebelChain::snapToGrid()
{
	rebelWin::snapToGrid();

	// calculate new escape paths for the states in this chain
	escapePath();
}

void
rebelChain::cutButton(rebelButton *b)
{
	// if this is a state button
	// first delete all escapes pointing to this state
	vector<rebelButton *>::iterator bi;
	for(bi = _buttons.begin(); bi != _buttons.end(); ++bi) {
		if((*bi)->butType() != REBEL_STATE) continue;

		rebelState *s = (rebelState *)(*bi);
		ESCAPE *escps = s->escapes();
		for(int i = 0; i < s->numEscapes(); ++i) {
			if(!strcmp(escps[i].toState->name(), b->name())) {
				s->cutEscape(i);
			}
		}
	}

	// now delete the state or comment
	rebelWin::cutButton(b);
}

int
rebelChain::addEscapeCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// get pointer to this chain instance
	if(app == (rebelApp *)NULL) return( Pt_CONTINUE );

	rebelChain *c = (rebelChain *)app->activeWindow();
	if(c == (rebelWin *)NULL) return( Pt_CONTINUE );
	if(c->winTyp() != REBEL_CHAIN) return( Pt_CONTINUE );

	c->addEscape(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );

}

void
rebelChain::addEscape( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(_addEscape == false) return;

	rebelState *state;
	PtGetResource(widget, Pt_ARG_POINTER, &state, 0);

	if(_fromState == (rebelState *)NULL) {
		_fromState = state;
	}
	else {
		_toState = state;
		ESCAPE *escp;
		if((escp = _fromState->newEscape(_toState)) != (ESCAPE *)NULL) {
			createEscapeWidget(escp, _fromState, _toState);
		}

		_addEscape = false;
		_fromState = (rebelState *)NULL;
		_toState = (rebelState *)NULL;
		app->saved(false);
	}
	
	return;

}

void rebelChain::createEscapeWidget(ESCAPE *escp, rebelState *fromState, rebelState *toState)
{
	escp->toState = toState;

	// compute the path between the from and to states
	escapePath(fromState, toState);

	// create a polygon widget connecting the two states
	PhPoint_t eOrig;
	eOrig.x = _scrollCanvasExtent.ul.x;
	eOrig.y = _scrollCanvasExtent.ul.y;

	PhPoint_t pOrig;
	pOrig.x = 0;
	pOrig.y = 0;

	PgColor_t pClr = _colorList[_colorListIndx];
	++_colorListIndx;
	if(_colorListIndx >= 10) _colorListIndx = 0;

	PtArg_t args[5];
	int nArgs = 0;
	PtSetArg(&args[nArgs], Pt_ARG_POINTS, _escpPoints, 12); ++nArgs;
	PtSetArg(&args[nArgs], Pt_ARG_COLOR, pClr, 0); ++nArgs;
	PtSetArg(&args[nArgs], Pt_ARG_ORIGIN, &pOrig, 0); ++nArgs;
	PtSetArg(&args[nArgs], Pt_ARG_POINTER, fromState, 0); ++nArgs;
	PtSetArg(&args[nArgs], Pt_ARG_USER_DATA, &escp, sizeof(ESCAPE *)); ++nArgs;
	escp->ePath = ApCreateDBWidget(rPictures, "rebelEscapePoly", _scrollCanvas, &eOrig, nArgs, args);
	PtRealizeWidget(escp->ePath);
	PtWidgetToBack(escp->ePath);

	// set up the resource dialog to show the escape parameters
	rResources->setEscapeWidgets(escp);

	// give keyboard focus to the "escape on" text widget
	PtWindowFocus(ABW_rebelResourceDialog);
	PtGiveFocus(ABW_escapeOnText, NULL);
}

void rebelChain::escapePath()
{
	ESCAPE *pescp;

	// calculate new escape paths for the states in this chain
	vector<rebelButton *>::iterator bi;
	for(bi = _buttons.begin(); bi != _buttons.end(); ++bi) {
		if((*bi)->butType() != REBEL_STATE) continue;

		rebelState *s = (rebelState *)(*bi);
		ESCAPE *escps = s->escapes();
		for(uint32_t ei = 0; ei != maxEscp; ++ei) {
			// if this state has not been defined
			pescp = &escps[ei];
			if(pescp->toState == (rebelState *)NULL) continue;

			// destroy the old polygon widget connecting the two states
			if(pescp->ePath) {
				PtDestroyWidget(pescp->ePath);
				pescp->ePath = (PtWidget_t *)NULL;
			}

			// compute the new path between the from and to states
			escapePath(s, pescp->toState);

			// create a polygon widget connecting the two states
			PhPoint_t eOrig;
			eOrig.x = _scrollCanvasExtent.ul.x;
			eOrig.y = _scrollCanvasExtent.ul.y;

			PhPoint_t pOrig;
			pOrig.x = 0;
			pOrig.y = 0;

			PgColor_t pClr = _colorList[_colorListIndx];
			++_colorListIndx;
			if(_colorListIndx >= 10) _colorListIndx = 0;

			PtArg_t args[5];
			int nArgs = 0;
			PtSetArg(&args[nArgs], Pt_ARG_POINTS, _escpPoints, 12); ++nArgs;
			PtSetArg(&args[nArgs], Pt_ARG_COLOR, pClr, 0); ++nArgs;
			PtSetArg(&args[nArgs], Pt_ARG_ORIGIN, &pOrig, 0); ++nArgs;
			PtSetArg(&args[nArgs], Pt_ARG_POINTER, s, 0); ++nArgs;
			PtSetArg(&args[nArgs], Pt_ARG_USER_DATA, &pescp, sizeof(ESCAPE **)); ++nArgs;
			pescp->ePath = ApCreateDBWidget(rPictures, "rebelEscapePoly", _scrollCanvas, &eOrig, nArgs, args);
			PtRealizeWidget(pescp->ePath);
			PtWidgetToBack(pescp->ePath);
		}
	}
}

void rebelChain::escapePath(rebelState *fromState, rebelState *toState)
{
	// draw the path between the two states
	PhPoint_t *fromPos;
	PtGetResource(fromState->button(), Pt_ARG_POS, &fromPos, 0);

	PhDim_t *fromDim;
	PtGetResource(fromState->button(), Pt_ARG_DIM, &fromDim, 0);

	PhPoint_t *toPos;
	PtGetResource(toState->button(), Pt_ARG_POS, &toPos, 0);

	PhDim_t *toDim;
	PtGetResource(toState->button(), Pt_ARG_DIM, &toDim, 0);

	if(toPos->y > fromPos->y) {
		int ydiff = ((toPos->y - fromPos->y) / _gridVertStep) - 1;
		if(toPos->x > fromPos->x) {
			_escpPoints[0].x = fromPos->x + fromDim->w;
			_escpPoints[0].y = fromPos->y + fromState->nextRightSocket();

			// draw a small box as the arrow start
			_escpPoints[1].x = _escpPoints[0].x;
			_escpPoints[1].y = _escpPoints[0].y - 1;
			_escpPoints[2].x = _escpPoints[1].x + 2;
			_escpPoints[2].y = _escpPoints[1].y;
			_escpPoints[3].x = _escpPoints[2].x;
			_escpPoints[3].y = _escpPoints[2].y + 2;
			_escpPoints[4].x = _escpPoints[3].x - 2;
			_escpPoints[4].y = _escpPoints[3].y;
			_escpPoints[5].x = _escpPoints[4].x;
			_escpPoints[5].y = _escpPoints[4].y - 1;

			// draw the shaft of the arrow
			_escpPoints[6].x = toPos->x - (ydiff * 3) - 3;
			_escpPoints[6].y = _escpPoints[0].y;
			_escpPoints[7].x = _escpPoints[6].x;
			_escpPoints[7].y = toPos->y + toState->nextLeftSocket();
			_escpPoints[8].x = toPos->x;
			_escpPoints[8].y = _escpPoints[7].y;

			// draw the arrow head
			_escpPoints[9].x = _escpPoints[8].x - 3;
			_escpPoints[9].y = _escpPoints[8].y + 3;
			_escpPoints[10].x = _escpPoints[9].x;
			_escpPoints[10].y = _escpPoints[9].y - 6;
			_escpPoints[11].x = _escpPoints[10].x + 3;
			_escpPoints[11].y = _escpPoints[10].y + 3;
		}
		else if(toPos->x == fromPos->x) {
			_escpPoints[0].x = fromPos->x + fromDim->w;
			_escpPoints[0].y = fromPos->y + fromState->nextRightSocket();

			// draw a small box as the arrow start
			_escpPoints[1].x = _escpPoints[0].x;
			_escpPoints[1].y = _escpPoints[0].y - 1;
			_escpPoints[2].x = _escpPoints[1].x + 2;
			_escpPoints[2].y = _escpPoints[1].y;
			_escpPoints[3].x = _escpPoints[2].x;
			_escpPoints[3].y = _escpPoints[2].y + 2;
			_escpPoints[4].x = _escpPoints[3].x - 2;
			_escpPoints[4].y = _escpPoints[3].y;
			_escpPoints[5].x = _escpPoints[4].x;
			_escpPoints[5].y = _escpPoints[4].y - 1;

			// draw the shaft of the arrow
			_escpPoints[6].x = _escpPoints[0].x + (ydiff * 3) + 3;
			_escpPoints[6].y = _escpPoints[0].y;
			_escpPoints[7].x = _escpPoints[6].x;
			_escpPoints[7].y = toPos->y + toState->nextRightSocket();
			_escpPoints[8].x = toPos->x + toDim->w;
			_escpPoints[8].y = _escpPoints[7].y;

			// draw the arrow head
			_escpPoints[9].x = _escpPoints[8].x + 3;
			_escpPoints[9].y = _escpPoints[8].y + 3;
			_escpPoints[10].x = _escpPoints[9].x;
			_escpPoints[10].y = _escpPoints[9].y - 6;
			_escpPoints[11].x = _escpPoints[10].x - 3;
			_escpPoints[11].y = _escpPoints[10].y + 3;
		}
		else if(toPos->x < fromPos->x) {
			_escpPoints[0].x = fromPos->x;
			_escpPoints[0].y = fromPos->y + fromState->nextLeftSocket();

			// draw a small box as the arrow start
			_escpPoints[1].x = _escpPoints[0].x;
			_escpPoints[1].y = _escpPoints[0].y - 1;
			_escpPoints[2].x = _escpPoints[1].x - 2;
			_escpPoints[2].y = _escpPoints[1].y;
			_escpPoints[3].x = _escpPoints[2].x;
			_escpPoints[3].y = _escpPoints[2].y + 2;
			_escpPoints[4].x = _escpPoints[3].x + 2;
			_escpPoints[4].y = _escpPoints[3].y;
			_escpPoints[5].x = _escpPoints[4].x;
			_escpPoints[5].y = _escpPoints[4].y - 1;

			// draw the shaft of the arrow
			_escpPoints[6].x = _escpPoints[0].x - (ydiff * 3) - 3;
			_escpPoints[6].y = _escpPoints[0].y;
			_escpPoints[7].x = _escpPoints[6].x;
			_escpPoints[7].y = toPos->y + toState->nextRightSocket();
			_escpPoints[8].x = toPos->x + toDim->w;
			_escpPoints[8].y = _escpPoints[7].y;

			// draw the arrow head
			_escpPoints[9].x = _escpPoints[8].x + 3;
			_escpPoints[9].y = _escpPoints[8].y + 3;
			_escpPoints[10].x = _escpPoints[9].x;
			_escpPoints[10].y = _escpPoints[9].y - 6;
			_escpPoints[11].x = _escpPoints[10].x - 3;
			_escpPoints[11].y = _escpPoints[10].y + 3;
		}
	}
	else if(toPos->y == fromPos->y) {
		if(toPos->x > fromPos->x) {
			_escpPoints[0].x = fromPos->x + fromDim->w;
			_escpPoints[0].y = fromPos->y + fromState->nextRightSocket();

			// draw a small box as the arrow start
			_escpPoints[1].x = _escpPoints[0].x;
			_escpPoints[1].y = _escpPoints[0].y - 1;
			_escpPoints[2].x = _escpPoints[1].x + 2;
			_escpPoints[2].y = _escpPoints[1].y;
			_escpPoints[3].x = _escpPoints[2].x;
			_escpPoints[3].y = _escpPoints[2].y + 2;
			_escpPoints[4].x = _escpPoints[3].x - 2;
			_escpPoints[4].y = _escpPoints[3].y;
			_escpPoints[5].x = _escpPoints[4].x;
			_escpPoints[5].y = _escpPoints[4].y - 1;

			// draw the shaft of the arrow
			_escpPoints[6].x = toPos->x - 3;
			_escpPoints[6].y = _escpPoints[0].y;
			_escpPoints[7].x = _escpPoints[6].x;
			_escpPoints[7].y = toPos->y + toState->nextLeftSocket();
			_escpPoints[8].x = toPos->x;
			_escpPoints[8].y = _escpPoints[7].y;

			// draw the arrow head
			_escpPoints[9].x = _escpPoints[8].x - 3;
			_escpPoints[9].y = _escpPoints[8].y + 3;
			_escpPoints[10].x = _escpPoints[9].x;
			_escpPoints[10].y = _escpPoints[9].y - 6;
			_escpPoints[11].x = _escpPoints[10].x + 3;
			_escpPoints[11].y = _escpPoints[10].y + 3;
		}
		else if(toPos->x < fromPos->x) {
			_escpPoints[0].x = fromPos->x;
			_escpPoints[0].y = fromPos->y + fromState->nextLeftSocket();

			// draw a small box as the arrow start
			_escpPoints[1].x = _escpPoints[0].x;
			_escpPoints[1].y = _escpPoints[0].y - 1;
			_escpPoints[2].x = _escpPoints[1].x - 2;
			_escpPoints[2].y = _escpPoints[1].y;
			_escpPoints[3].x = _escpPoints[2].x;
			_escpPoints[3].y = _escpPoints[2].y + 2;
			_escpPoints[4].x = _escpPoints[3].x + 2;
			_escpPoints[4].y = _escpPoints[3].y;
			_escpPoints[5].x = _escpPoints[4].x;
			_escpPoints[5].y = _escpPoints[4].y - 1;

			// draw the shaft of the arrow
			_escpPoints[6].x = _escpPoints[0].x - 3;
			_escpPoints[6].y = _escpPoints[0].y;
			_escpPoints[7].x = _escpPoints[6].x;
			_escpPoints[7].y = toPos->y + toState->nextRightSocket();
			_escpPoints[8].x = toPos->x + toDim->w;
			_escpPoints[8].y = _escpPoints[7].y;

			// draw the arrow head
			_escpPoints[9].x = _escpPoints[8].x + 3;
			_escpPoints[9].y = _escpPoints[8].y + 3;
			_escpPoints[10].x = _escpPoints[9].x;
			_escpPoints[10].y = _escpPoints[9].y - 6;
			_escpPoints[11].x = _escpPoints[10].x - 3;
			_escpPoints[11].y = _escpPoints[10].y + 3;
		}
	}
	else if(toPos->y < fromPos->y) {
		int ydiff = ((fromPos->y - toPos->y) / _gridVertStep) - 1;
		if(toPos->x > fromPos->x) {
			_escpPoints[0].x = fromPos->x + fromDim->w;
			_escpPoints[0].y = fromPos->y + fromState->nextRightSocket();

			// draw a small box as the arrow start
			_escpPoints[1].x = _escpPoints[0].x;
			_escpPoints[1].y = _escpPoints[0].y - 1;
			_escpPoints[2].x = _escpPoints[1].x + 2;
			_escpPoints[2].y = _escpPoints[1].y;
			_escpPoints[3].x = _escpPoints[2].x;
			_escpPoints[3].y = _escpPoints[2].y + 2;
			_escpPoints[4].x = _escpPoints[3].x - 2;
			_escpPoints[4].y = _escpPoints[3].y;
			_escpPoints[5].x = _escpPoints[4].x;
			_escpPoints[5].y = _escpPoints[4].y - 1;

			// draw the shaft of the arrow
			_escpPoints[6].x = toPos->x - (ydiff * 3) - 3;
			_escpPoints[6].y = _escpPoints[0].y;
			_escpPoints[7].x = _escpPoints[6].x;
			_escpPoints[7].y = toPos->y + toState->nextLeftSocket();
			_escpPoints[8].x = toPos->x;
			_escpPoints[8].y = _escpPoints[7].y;

			// draw the arrow head
			_escpPoints[9].x = _escpPoints[8].x - 3;
			_escpPoints[9].y = _escpPoints[8].y + 3;
			_escpPoints[10].x = _escpPoints[9].x;
			_escpPoints[10].y = _escpPoints[9].y - 6;
			_escpPoints[11].x = _escpPoints[10].x + 3;
			_escpPoints[11].y = _escpPoints[10].y + 3;
		}
		else if(toPos->x == fromPos->x) {
			_escpPoints[0].x = fromPos->x;
			_escpPoints[0].y = fromPos->y  + fromState->nextLeftSocket();

			// draw a small box as the arrow start
			_escpPoints[1].x = _escpPoints[0].x;
			_escpPoints[1].y = _escpPoints[0].y - 1;
			_escpPoints[2].x = _escpPoints[1].x - 2;
			_escpPoints[2].y = _escpPoints[1].y;
			_escpPoints[3].x = _escpPoints[2].x;
			_escpPoints[3].y = _escpPoints[2].y + 2;
			_escpPoints[4].x = _escpPoints[3].x + 2;
			_escpPoints[4].y = _escpPoints[3].y;
			_escpPoints[5].x = _escpPoints[4].x;
			_escpPoints[5].y = _escpPoints[4].y - 1;

			// draw the shaft of the arrow
			_escpPoints[6].x = _escpPoints[0].x - (ydiff * 3) - 3;
			_escpPoints[6].y = _escpPoints[0].y;
			_escpPoints[7].x = _escpPoints[6].x;
			_escpPoints[7].y = toPos->y + toState->nextLeftSocket();
			_escpPoints[8].x = toPos->x;
			_escpPoints[8].y = _escpPoints[7].y;

			// draw the arrow head
			_escpPoints[9].x = _escpPoints[8].x - 3;
			_escpPoints[9].y = _escpPoints[8].y + 3;
			_escpPoints[10].x = _escpPoints[9].x;
			_escpPoints[10].y = _escpPoints[9].y - 6;
			_escpPoints[11].x = _escpPoints[10].x + 3;
			_escpPoints[11].y = _escpPoints[10].y + 3;
		}
		else if(toPos->x < fromPos->x) {
			_escpPoints[0].x = fromPos->x;
			_escpPoints[0].y = fromPos->y + fromState->nextLeftSocket();

			// draw a small box as the arrow start
			_escpPoints[1].x = _escpPoints[0].x;
			_escpPoints[1].y = _escpPoints[0].y - 1;
			_escpPoints[2].x = _escpPoints[1].x - 2;
			_escpPoints[2].y = _escpPoints[1].y;
			_escpPoints[3].x = _escpPoints[2].x;
			_escpPoints[3].y = _escpPoints[2].y + 2;
			_escpPoints[4].x = _escpPoints[3].x + 2;
			_escpPoints[4].y = _escpPoints[3].y;
			_escpPoints[5].x = _escpPoints[4].x;
			_escpPoints[5].y = _escpPoints[4].y - 1;

			// draw the shaft of the arrow
			_escpPoints[6].x = _escpPoints[0].x - (ydiff * 3) - 3;
			_escpPoints[6].y = _escpPoints[0].y;
			_escpPoints[7].x = _escpPoints[6].x;
			_escpPoints[7].y = toPos->y + toState->nextRightSocket();
			_escpPoints[8].x = toPos->x + toDim->w;
			_escpPoints[8].y = _escpPoints[7].y;

			// draw the arrow head
			_escpPoints[9].x = _escpPoints[8].x + 3;
			_escpPoints[9].y = _escpPoints[8].y + 3;
			_escpPoints[10].x = _escpPoints[9].x;
			_escpPoints[10].y = _escpPoints[9].y - 6;
			_escpPoints[11].x = _escpPoints[10].x - 3;
			_escpPoints[11].y = _escpPoints[10].y + 3;
		}
	}
}


int
rebelChain::initAbortListCallback( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	// get pointer to this chain instance
	if(app == (rebelApp *)NULL) return( Pt_CONTINUE );

	rebelChain *c = (rebelChain *)app->activeWindow();
	if(c == (rebelWin *)NULL) return( Pt_CONTINUE );
	if(c->winTyp() != REBEL_CHAIN) return( Pt_CONTINUE );

	c->initAbortList(link_instance, apinfo, cbinfo);

	return( Pt_CONTINUE );

}

void
rebelChain::initAbortList( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	if(apinfo->reason == ABR_PRE_REALIZE) {
		PtSetResource(link_instance, Pt_ARG_POINTER, this, 0);

		PtListDeleteAllItems(ABW_abortList);
		char **items;
		int nItems = _buttons.size();
		if(nItems > 0) {
			size_t listSize = nItems * sizeof(char *);
			items = (char **)malloc(listSize);
			vector<rebelButton *>::iterator bi;
			int li;
			for(bi = _buttons.begin(), li = 0; bi != _buttons.end(); ++bi, ++li) {
				rebelState *s = (rebelState *)(*bi);
				items[li] = (char *)malloc(32);
				strcpy(items[li], s->name());
			}
			PtListAddItems(ABW_abortList, (const char **)items, nItems, 1);
		}
	}
	else {
		if(!_abortList.empty()) {
			vector<string>::iterator li;
			for(li = _abortList.begin(); li != _abortList.end(); ++li) {
				int pos = PtListItemPos(ABW_abortList, &(*li).c_str()[1]);
				if(pos > 0) PtListSelectPos(ABW_abortList, pos);
			}
		}
	}

	return;

}


int
rebelChain::addAbortStateCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(cbinfo->reason_subtype != Pt_LIST_SELECTION_FINAL) return( Pt_CONTINUE );

	// get pointer to this chain instance
	PtWidget_t *window = ApGetInstance(widget);
	rebelChain *chain;
	PtGetResource(window, Pt_ARG_POINTER, &chain, 0);

	chain->addAbortState(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );

}

void
rebelChain::addAbortState( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	PtListCallback_t *lstData = (PtListCallback_t *)cbinfo->cbdata;

	// clear the abort image from all states
	setAbortImage();

	// clear the abort list
	_abortList.clear();

	// now, for each selected item, set the abort image
	// and add the state name to the abort list
	for(int i = 0; i < lstData->sel_item_count; ++i) {
		setAbortImage(lstData->sel_items[i]);
		string a = "s";
		a += lstData->sel_items[i];
		_abortList.push_back(a);
	}

	return;

}

void
rebelChain::saveWindow(FILE *fp)
{
	// save the name of the chain
	fprintf(fp, "CHAIN %s\n", _name);

	// save the descriptions of the comments
	vector<rebelButton *>::iterator bi;
	for(bi = _buttons.begin(); bi != _buttons.end(); ++bi) {
		if((*bi)->butType() == REBEL_COMMENT) {
			(*bi)->saveButton(fp);
		}
	}
	fprintf(fp, "CHAIN_DONE\n");
}

void
rebelChain::readWindow()
{
	rebelComment *cmnt = (rebelComment *)NULL;
	PhPoint_t o;
	string line;

	// skip down the buffer to the beginning of the properties
	char *tok;
	while((tok = _lineTok->tok((char *)NULL)) != (char *)NULL) {
		line = tok;

		if(line.find("/**********************************") != string::npos) break;
	}
	
	// now find the description of this chain
	while((tok = _lineTok->tok((char *)NULL)) != (char *)NULL) {
		line = tok;

		if((line.find("CHAIN") != string::npos) &&
		   (line.find(_name) != string::npos)) break;
	}

	// now process the comments for this chain
	while((tok = _lineTok->tok((char *)NULL)) != (char *)NULL) {
		line = tok;

		// done with this chain
		if(line.find("CHAIN_DONE") != string::npos) return;

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

void rebelChain::portWindow(rebelApp *app, string first, string second)
{
    PhPoint_t o;
	size_t ampPos;
    size_t commaPos;
    
    // get the name of the first state in this chain
	char tmp[64];
    string firstName;

	// get the name of the first state in this chain
    ampPos = first.find("&");
    first.erase(0, (ampPos + 1));
    commaPos = first.find(",&");
	strcpy(tmp, first.c_str());
    firstName.insert(firstName.end(), tmp, (tmp + commaPos));

    // get the name of the abort list for this chain
    ampPos = first.find(",abort");
    first.erase(0, (ampPos + 1));
    commaPos = first.find(",0");

	strcpy(tmp, first.c_str());
    string abortName;
    abortName.insert(abortName.end(), tmp, (tmp + commaPos));

    // get the name of the first state in the next chain
    string nextName;
    if((ampPos = second.find("&")) != string::npos) {
		second.erase(0, (ampPos + 1));
		commaPos = second.find(",&");
		strcpy(tmp, second.c_str());
		nextName.insert(nextName.end(), tmp, (tmp + commaPos));
	}
	else {
		nextName = second;
	}
    
    // make a copy of the file for lineTok to mangle
    _sourceTemp.clear();
    _sourceTemp.insert(_sourceTemp.end(), app->srcBeg(), app->srcEnd());
	_sourceTemp.push_back('\0');

    // read through the source to find
	// the definition of the first state in this chain
	rebelState *state = (rebelState *)NULL;
	string line;
	char *tok;
	if((tok = _lineTok->tok(&_sourceTemp[0])) == (char *)NULL) return;
	do {
		line = tok;

        // this test will be true for a state defintion
        if((line.find("STATE") != string::npos) &&
           (line.find("=") != string::npos) &&
		   (line.find("{") != string::npos)) {
			char strng[64];
			strcpy(strng, line.c_str());
			char *stok = strtok(strng, " ");		// stok now contains the string "STATE"
			stok = strtok(NULL, " ");				// stok now contains the name of the state

			// if this is the definition of first state in this chain,
			// create a button for this state and break out of this loop
			if(!strcmp(stok, firstName.c_str())) {
				size_t slashPos;
				if((slashPos = line.find("//")) != string::npos) {
					line.erase(0, (slashPos + 2));
					sscanf(line.c_str(), "%hd %hd", &o.x, &o.y);
					state = new rebelState(_scrollCanvas, &stok[1], o);
				}
				else {
					o.x = 162;
					o.y = 0;
					state = new rebelState(_scrollCanvas, &stok[1], o);
				}
				state->portButton((rebelWin *)this);
				
                _buttons.push_back(state);
            			
				break;                
            }
		}
    } while((tok = _lineTok->tok((char *)NULL)) != (char *)NULL);

	// having read the first state in the chain
	// continue reading each state definition
	// until reaching the the first state of the next chain
	while((tok = _lineTok->tok((char *)NULL)) != (char *)NULL) {
		line = tok;

		if((line.find("STATE") != string::npos) &&
		   (line.find("=") != string::npos) &&
		   (line.find("{") != string::npos)) {
			char strng[64];
			strcpy(strng, line.c_str());
			char *stok = strtok(strng, " ");		// stok now contains the string "STATE"
			stok = strtok(NULL, " ");				// stok now contains the name of the state

			// if this is the first state in the next chain,
			if(!strcmp(stok, nextName.c_str())) break;
			else if(line.find("*snames[]") != string::npos) break;
			else if(line.find("abort") != string::npos) break;


			size_t slashPos;
			if((slashPos = line.find("//")) != string::npos) {
				line.erase(0, (slashPos + 2));
				sscanf(line.c_str(), "%hd %hd", &o.x, &o.y);
				state = new rebelState(_scrollCanvas, &stok[1], o);
			}
			else {
				o = getNextCell(o);
				state = new rebelState(_scrollCanvas, &stok[1], o);
			}
			state->portButton((rebelWin *)this);
			
			_buttons.push_back(state);			
		}
	}

	// now process the abort list for this chain
	portAborts(tok, abortName);

	// read the properties to get the comments
	readWindow();

	// set the active button pointer for this chain
	_activeBtnPtr = _buttons.begin();

	setupEscapes();

	return;
}

void
rebelChain::restartFunction(char *f)
{
	strcpy(_restartFunc, f);

	string prototype = "void ";
	prototype += _restartFunc;
	prototype += "()";
	if(app) app->addFunction(_restartFunc, prototype);
}


void
rebelChain::portAborts(char *tok, string abortName)
{
	if(tok == (char *)NULL) return;
	
	// find this chain's abort list
	string line;
	do {
		line = tok;

		if((line.find("STATE") != string::npos) &&
		   (line.find(abortName) != string::npos)) break;
	} while((tok = _lineTok->tok((char *)NULL)) != (char *)NULL);

	while((tok = _lineTok->tok((char *)NULL)) != (char *)NULL) {
		line = tok;

		if(line.find("0") != string::npos) break;

		size_t f;
		if((f = line.find("&")) != string::npos) {
			line.erase(0, (f + 1));

			if((f = line.find(",")) != string::npos) line.erase(f);
			_abortList.push_back(line);
		}

		// set the image label of the abort state
		setAbortImage(&line.c_str()[1]);
	}
}

void
rebelChain::setupEscapes()
{
	vector<rebelButton *>::iterator bi;
	for(bi = _buttons.begin(); bi != _buttons.end(); ++bi) {
		if((*bi)->butType() != REBEL_STATE) continue;

		rebelState *s = (rebelState *)(*bi);
		int numEscapes = s->numEscapes();
		for(int i = 0; i < numEscapes; ++i) {
			ESCAPE *escp = s->escapes(i);
			rebelState *ts = toState(&escp->stateptr[2]);
			if(!ts) {
				s->cutEscape(i);
				numEscapes = s->numEscapes();
				continue;
			}

			createEscapeWidget(escp, s, ts);
		}
	}
	
}

void
rebelChain::setAbortImage()
{
	vector<rebelButton *>::iterator bi;
	for(bi = _buttons.begin(); bi != _buttons.end(); ++bi) {
		if((*bi)->butType() != REBEL_STATE) continue;

		PtWidget_t *but = (*bi)->button();
		PhImage_t *image = ApGetImageRes(rPictures, "blankImageButton");
		if(image) PtSetResource(but, Pt_ARG_LABEL_IMAGE, image, 0);
	}	
}

void
rebelChain::setAbortImage(const char *name)
{
	vector<rebelButton *>::iterator bi;
	for(bi = _buttons.begin(); bi != _buttons.end(); ++bi) {
		if((*bi)->butType() != REBEL_STATE) continue;

		char *stateName = (*bi)->name();
		if(!strcmp(name, stateName)) {
			PtWidget_t *but = (*bi)->button();
			PhImage_t *image = ApGetImageRes(rPictures, "abortImageButton");
			if(image) PtSetResource(but, Pt_ARG_LABEL_IMAGE, image, 0);
			break;
		}
	}
}

rebelState *
rebelChain::toState(char *name)
{
	vector<rebelButton *>::iterator bi;
	for(bi = _buttons.begin(); bi != _buttons.end(); ++bi) {
		if((*bi)->butType() != REBEL_STATE) continue;

		rebelState *s = (rebelState *)(*bi);
		if(!strcmp(s->name(), name)) return(s);
	}
	return((rebelState *)NULL);
}

void
rebelChain::genStateDecls(FILE *fp)
{
	vector<rebelButton *>::iterator bi;
	for(bi = _buttons.begin(); bi != _buttons.end(); ++bi) {
		if((*bi)->butType() != REBEL_STATE) continue;

		rebelState *s = (rebelState *)(*bi);
		s->genDecl(fp);
	}
}

void
rebelChain::genStateArrayNames(FILE *fp)
{
	vector<rebelButton *>::iterator bi;
	for(bi = _buttons.begin(); bi != _buttons.end(); ++bi) {
		if((*bi)->butType() != REBEL_STATE) continue;

		rebelState *s = (rebelState *)(*bi);
		s->genArrayName(fp);
	}
}

void
rebelChain::genStateDefs(FILE *fp)
{
	vector<rebelButton *>::iterator bi;
	for(bi = _buttons.begin(); bi != _buttons.end(); ++bi) {
		if((*bi)->butType() != REBEL_STATE) continue;

		rebelState *s = (rebelState *)(*bi);
		s->genCode(fp);
	}
}

void
rebelChain::genAbortList(FILE *fp)
{
	if(!_abortList.empty()) {
		vector<string>::iterator stri;
		for(stri = _abortList.begin(); stri != _abortList.end(); ++stri) {
			fprintf(fp, "\t&%s,\n", stri->c_str());
		}
	}
	fprintf(fp, "\t0};\n\n");

}

void
rebelChain::genSpotDefs(FILE *fp)
{
	if(strlen(_restartFunc)) fprintf(fp, "restart %s\n", _restartFunc);
	fprintf(fp, "%s {\n", _name);
	fprintf(fp, "status ON\n");
	fprintf(fp, "begin");
	vector<rebelButton *>::iterator bi;
	for(bi = _buttons.begin(); bi != _buttons.end(); ++bi) {
		if((*bi)->butType() != REBEL_STATE) continue;

		rebelState *s = (rebelState *)(*bi);
		s->genSpot(fp);
	}

	if(!_abortList.empty()) {
		fprintf(fp, "abort\tlist:\n");
		vector<string>::iterator si;
		for(si = _abortList.begin(); si != _abortList.end(); ++si) {
			fprintf(fp, " %s", si->c_str());
		}
		fprintf(fp, "\n");
	}

	fprintf(fp, "}\n");
}

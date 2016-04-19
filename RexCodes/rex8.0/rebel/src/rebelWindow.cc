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

int rebelWin::_gridVertStep = 35;
int rebelWin::_gridHorzStep = 150;
int rebelWin::_moveDamp = 4;
PhPoint_t rebelWin::_nextOrigin = { 0, 0 };

rebelWin::rebelWin()
{
	_lineTok = new rebelLineTok;
	_winTyp = REBEL_WINDOW;
	strcpy(_name, "");
	_addButton = false;
	_addComment = false;

	_window = (PtWidget_t *)NULL;
	_scrollContainer = (PtWidget_t *)NULL;
	_scrollCanvas = (PtWidget_t *)NULL;
	_draggedWidget = (PtWidget_t *)NULL;

	PhPoint_t p = { 0, 0 };
	_origin = p;
	PhDim_t s = { 450, 350 };
	_size = s;
	_prefSize = s;

	vector<rebelButton *>::iterator bi;
	for(bi = _buttons.begin(); bi != _buttons.end(); ++bi) delete *bi;
	_buttons.clear();
	_buttonGroup.clear();
	_activeBtnPtr = (vector<rebelButton *>::iterator) NULL;
}

rebelWin::rebelWin(const char *n)
{
	_lineTok = new rebelLineTok;
	_winTyp = REBEL_WINDOW;
	strncpy(_name, n, 32);
	_addButton = false;
	_addComment = false;

	_draggedWidget = (PtWidget_t *)NULL;

	vector<rebelButton *>::iterator bi;
	for(bi = _buttons.begin(); bi != _buttons.end(); ++bi) delete *bi;
	_buttons.clear();
	_buttonGroup.clear();
	_activeBtnPtr = (vector<rebelButton *>::iterator) NULL;
}

rebelWin::rebelWin(const char *n, const PhDim_t size)
{
	_lineTok = new rebelLineTok;
	_winTyp = REBEL_WINDOW;
	strncpy(_name, n, 32);
	_prefSize = size;
	_addButton = false;
	_addComment = false;

	_draggedWidget = (PtWidget_t *)NULL;

	vector<rebelButton *>::iterator bi;
	for(bi = _buttons.begin(); bi != _buttons.end(); ++bi) delete *bi;
	_buttons.clear();
	_buttonGroup.clear();
	_activeBtnPtr = (vector<rebelButton *>::iterator) NULL;
}

rebelWin::rebelWin(const rebelWin& rw)
{
	_lineTok = new rebelLineTok;
	_winTyp = rw._winTyp;

	strcpy(_name, rw._name);
	_addButton = rw._addButton;
	_addComment = rw._addComment;
	_buttons = rw._buttons;
	_buttonGroup = rw._buttonGroup;
	_activeBtnPtr = rw._activeBtnPtr;

	_window = rw._window;
	_scrollContainer = rw._scrollContainer;
	_scrollCanvas = rw._scrollCanvas;
	_scrollCanvasExtent = rw._scrollCanvasExtent;
	_draggedWidget = rw._draggedWidget;

	_origin = rw._origin;
	_size = rw._size;
	_prefSize = rw._prefSize;
}


rebelWin::~rebelWin()
{
	delete _lineTok;
	vector<rebelButton *>::iterator bi;
	for(bi = _buttons.begin(); bi != _buttons.end(); ++bi) delete *bi;
	_buttons.clear();
	_buttonGroup.clear();

	// this call destroys all of the widgets associated with this window
	PtDestroyWidget(_window);

}


int
rebelWin::init( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	extern PhDim_t bkgdSize;

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;


	if(apinfo->reason == ABR_PRE_REALIZE) {
		rebelWin *w = (rebelWin *)cbinfo->cbdata;
		char title[64];
		if(w->_winTyp == REBEL_CHAIN) strcpy(title, "CHAIN : ");
		else if(w->_winTyp == REBEL_MENU) strcpy(title, "MENU : ");
		else if(w->_winTyp == REBEL_VARIABLE) strcpy(title, "VARIABLE : ");
		else if(w->_winTyp == REBEL_FUNCTION) strcpy(title, "FUNCTION : ");
		strcat(title, w->_name);

		PtArg_t args[2];
		int nArgs = 0;
		PtSetArg(&args[nArgs], Pt_ARG_POINTER, w, 0); ++nArgs;
		PtSetArg(&args[nArgs], Pt_ARG_WINDOW_TITLE, title, 0); ++nArgs;
		PtSetResources(link_instance, nArgs, args);
		w->_scrollContainer = ApGetWidgetPtr(link_instance, ABN_windowScrollContainer);
		w->_scrollCanvas = PtValidParent(w->_scrollContainer, PtButton);

		// mark this window as being active by
		// setting the background to grey
		PtSetResource(w->_scrollContainer, Pt_ARG_FILL_COLOR, Pg_GREY, 0);
	}
	else if(apinfo->reason == ABR_POST_REALIZE) {
		rebelWin *w;
		PtGetResource(link_instance, Pt_ARG_POINTER, &w, 0);
		
		_nextOrigin.x += 10;

		if((w->_origin.y + w->_size.h) > bkgdSize.h) _nextOrigin.y = 0;
		else _nextOrigin.y += 20;
	}

	return( Pt_CONTINUE );

}


void
rebelWin::resetNextOrigin()
{
	_nextOrigin.x = 0;
	_nextOrigin.y = 0;
}

int
rebelWin::resizeCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	rebelWin *pWin;
	PtGetResource(widget, Pt_ARG_POINTER, &pWin, 0);

	if(!pWin) return( Pt_CONTINUE );

	pWin->resize(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );

}

void
rebelWin::resize( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	extern PhPoint_t bkgdOrigin;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	PhArea_t *a;
	PtGetResource(widget, Pt_ARG_AREA, &a, 0);
	_origin.x = a->pos.x - bkgdOrigin.x;
	_origin.y = a->pos.y - bkgdOrigin.y;
	_size = a->size;

	if(_scrollCanvas) {
		PtWidgetExtent(_scrollCanvas, &_scrollCanvasExtent);
	}

	return;
}


PhPoint_t
rebelWin::origin()
{
	extern PhPoint_t bkgdOrigin;

	PhArea_t *a;
	PtGetResource(_window, Pt_ARG_AREA, &a, 0);

	_origin.x = a->pos.x - bkgdOrigin.x;
	_origin.y = a->pos.y - bkgdOrigin.y;

	return(_origin);
}


int
rebelWin::winOpenCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	rebelWin *win;
	PtGetResource(widget, Pt_ARG_POINTER, &win, 0);

	if(!win) return( Pt_CONTINUE );

	win->winOpen(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );

}

void
rebelWin::winOpen( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	PtSetResource(widget, Pt_ARG_DIM, &_prefSize, 0);

	return;

}


int
rebelWin::closeWinCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	PhWindowEvent_t *data = (PhWindowEvent_t *)cbinfo->cbdata;

	if(data->event_f == Ph_WM_CLOSE) {
		rebelWin *win;
		PtGetResource(widget, Pt_ARG_POINTER, &win, 0);

		if(!win) return( Pt_CONTINUE );

		win->closeWin(widget, apinfo, cbinfo);
	}

	return( Pt_CONTINUE );

}

void
rebelWin::closeWin( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	_prefSize = _size;

	PtUnrealizeWidget(_window);

	return;

}


int
rebelWin::openWinCallback( PtWidget_t *widget, void *client_data, PtCallbackInfo_t *cbinfo)
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, client_data = client_data, cbinfo = cbinfo;

	rebelWin *win;
	PtGetResource(widget, Pt_ARG_POINTER, &win, 0);

	if(!win) return( Pt_CONTINUE );

	win->openWin(widget, client_data, cbinfo);

	return( Pt_CONTINUE );

}

void
rebelWin::openWin( PtWidget_t *widget, void *client_data, PtCallbackInfo_t *cbinfo )
{
	/* eliminate 'unreferenced' warnings */
	widget = widget, client_data = client_data, cbinfo = cbinfo;

	// realize the window if it is not visible
	if(PtWidgetIsRealized(_window) == 0) PtRealizeWidget(_window);
	else if(PtWidgetIsRealized(_window) != 0) PtUnrealizeWidget(_window);

	return;
}


int
rebelWin::initMoveCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	PtWidget_t *window = ApGetInstance(widget);
	rebelWin *win;
	PtGetResource(window, Pt_ARG_POINTER, &win, 0);
	app->selectWindow(win);

	win->initMove(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );

}

void
rebelWin::initMove( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// set the boundary of the drag to the boundary of the scroll canvas
	PhRect_t boundary;
	boundary.ul.x = 0;
	boundary.ul.y = 0;
	boundary.lr.x = _scrollCanvasExtent.lr.x * 2;
	boundary.lr.y = _scrollCanvasExtent.lr.y * 2;

	// set the drag rectangle to the size of a single button
	PhRect_t buttonRect;
	PtWidgetExtent(widget, &buttonRect);

	// initiate opaque dragging
	PhCursorDescription_t cursDscrpt;
	cursDscrpt.type = Ph_CURSOR_MOVE;
	cursDscrpt.length = 40;

	PhInitDrag(PtWidgetRid(_scrollContainer),
			   Ph_TRACK_DRAG | Ph_DRAG_TRACK,
			   &buttonRect, &boundary,
			   PhInputGroup(cbinfo->event),
			   NULL, NULL, NULL, NULL, &cursDscrpt);

	_draggedWidget = widget;

	// mark the original location of the button
	// to use as a base for a damping factor
	PtGetResource(_draggedWidget, Pt_ARG_POS, &_orgPos, 0);

	return;
}


int
rebelWin::eventCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// get pointer to this chain instance
	PtWidget_t *window = ApGetInstance(widget);
	rebelWin *w;
	PtGetResource(window, Pt_ARG_POINTER, &w, 0);

	// get the event that triggered this callback and the cursor location
	if(cbinfo->event->type == Ph_EV_BOUNDARY) {
		w->eventBoundary(widget, apinfo, cbinfo);
	}
	else if((cbinfo->event->type == Ph_EV_DRAG) && (cbinfo->event->subtype == Ph_EV_DRAG_MOVE)) {
		w->eventDrag(widget, apinfo, cbinfo);
	}

	return( Pt_CONTINUE );

}

void
rebelWin::eventBoundary( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(cbinfo->event->subtype == Ph_EV_PTR_ENTER) {
		if(_addButton == true) PtSetResource(widget, Pt_ARG_CURSOR_TYPE, Ph_CURSOR_CROSSHAIR, 0);
		else if(_addComment == true) PtSetResource(widget, Pt_ARG_CURSOR_TYPE, Ph_CURSOR_PASTE, 0);
	}
	else if(cbinfo->event->subtype == Ph_EV_PTR_LEAVE) {
		PtSetResource(widget, Pt_ARG_CURSOR_TYPE, Ph_CURSOR_INHERIT, 0);
	}

	return;
}


void
rebelWin::eventDrag( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(_draggedWidget == (PtWidget_t *)NULL) return;

	// set the location of a single widget
	PhPoint_t *oldPos;
	PtGetResource(_draggedWidget, Pt_ARG_POS, &oldPos, 0);

	PhDragEvent_t *dragData = (PhDragEvent_t *)PhGetData(cbinfo->event);

	PhPoint_t newPos;
	newPos.x = dragData->rect.ul.x + cbinfo->event->translation.x;
	newPos.y = dragData->rect.ul.y + cbinfo->event->translation.y;

	// return if the cursor hasn't moved to another cell
	if((oldPos->x == newPos.x) && (oldPos->y == newPos.y)) return;

	// return if the cursor hasn't moved beyond the damping factor
	PhPoint_t damp;
	damp.x = (newPos.x > _orgPos.x) ? newPos.x - _orgPos.x : _orgPos.x - newPos.x;
	damp.y = (newPos.y > _orgPos.y) ? newPos.y - _orgPos.y : _orgPos.y - newPos.y;
	if((damp.x < _moveDamp) && (damp.y < _moveDamp)) return;

	vector<rebelButton *>::iterator bi;

	if(_buttonGroup.empty()) {
		PtSetResource(_draggedWidget, Pt_ARG_POS, &newPos, 0);
		rebelState *draggedState;
		PtGetResource(_draggedWidget, Pt_ARG_POINTER, &draggedState, 0);
		draggedState->origin(newPos);
	}
	else {
		PhPoint_t diff;
		diff.x = oldPos->x - newPos.x;
		diff.y = oldPos->y - newPos.y;
		for(bi = _buttonGroup.begin(); bi != _buttonGroup.end(); ++bi) {
			PtWidget_t *w = (*bi)->button();
			PhPoint_t *op;
			PtGetResource(w, Pt_ARG_POS, &op, 0);
			newPos.x = op->x - diff.x;
			newPos.y = op->y - diff.y;
			PtSetResource(w, Pt_ARG_POS, &newPos, 0);
			(*bi)->origin(newPos);
		}
	}

	return;
}

void
rebelWin::newButton( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	_addButton = true;
	_addComment = false;

	return;

}


int
rebelWin::newCommentCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(app == (rebelApp *)NULL) return( Pt_CONTINUE );

	// get pointer to this window instance
	rebelWin *w = app->activeWindow();
	if(w == (rebelWin *)NULL) return( Pt_CONTINUE );

	w->newComment(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );

}

void
rebelWin::newComment( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	_addComment = true;
	_addButton = false;

	return;

}


int
rebelWin::addButtonCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	PtWidget_t *window = ApGetInstance(widget);
	rebelWin *w;
	PtGetResource(window, Pt_ARG_POINTER, &w, 0);

	if(w->_addButton == true) {
		w->addButton(widget, apinfo, cbinfo);
	}
	else if(w->_addComment == true) {
		w->addComment(widget, apinfo, cbinfo);
	}
	else {
		app->selectWindow(w);

		switch(w->_winTyp) {
		case REBEL_CHAIN:
			rResources->setChainWidgets((rebelChain *)w);
			break;
		case REBEL_MENU:
			rResources->setMenuWidgets((rebelMenu *)w);
			break;
		case REBEL_VARIABLE:
			if(!w->_buttons.empty()) {
				rResources->setVariableWidgets((rebelVariableItem *)*w->_activeBtnPtr);
			}
			break;
		case REBEL_FUNCTION:
			if(!w->_buttons.empty()) {
				rResources->setFunctionWidgets((rebelFunctionItem *)*w->_activeBtnPtr);
			}
			break;
		default:
			break;
		}
	}

	return( Pt_CONTINUE );

}

void
rebelWin::addComment( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	PhPoint_t canvasPos;
	PtGetAbsPosition(widget, &canvasPos.x, &canvasPos.y);

	// set the location of the comment
	int inputgroup = PhInputGroup(cbinfo->event);
	PhCursorInfo_t cursor;
	PhQueryCursor(inputgroup, &cursor);
	_newPos.x = cursor.pos.x - canvasPos.x;
	_newPos.y = cursor.pos.y - canvasPos.y;

	// create a multiline text widget to hold the comments
	rebelComment *cmnt = new rebelComment(_scrollCanvas, _newPos);
	_buttons.push_back(cmnt);

	// mark the newly created text item as selected
	rEdit.selectObject(cmnt->button());
	_activeBtnPtr = _buttons.end() - 1;

	// get out of comment adding mode
	PtSetResource(widget, Pt_ARG_CURSOR_TYPE, Ph_CURSOR_INHERIT, 0);
	_addComment = false;

	cmnt->resource(widget, apinfo, cbinfo);

	app->saved(false);

	return;

}

void
rebelWin::addButton( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// get the location of the widget's canvas
	PhPoint_t canvasPos;
	PtGetAbsPosition(widget, &canvasPos.x, &canvasPos.y);

	// set the location of the widget to one of the grid locations
	int inputgroup = PhInputGroup(cbinfo->event);
	PhCursorInfo_t cursor;
	PhQueryCursor(inputgroup, &cursor);
	int cellx = (int)((double)(cursor.pos.x - canvasPos.x) / (double)(_gridHorzStep) + 0.5);
	_newPos.x = cellx * _gridHorzStep;
	int celly = (int)((double)(cursor.pos.y - canvasPos.y) / (double)(_gridVertStep) + 0.5);
	_newPos.y = celly * _gridVertStep;

	// make sure that the new button does not obscure any of the older buttons
	_newPos = getNextCell(_newPos);

	if(!_buttons.empty()) {
		_activeBtnPtr = _buttons.begin();
	}
	
	return;
}


void
rebelWin::select()
{
	rEdit.selectObject(_window);

	PtSetResource(_scrollContainer, Pt_ARG_FILL_COLOR, Pg_GREY, 0);

	_addButton = false;
	_addComment = false;
	_draggedWidget = (PtWidget_t *)NULL;

	if(_buttons.empty()) {
		_activeBtnPtr = (vector<rebelButton *>::iterator) NULL;
	}
	else {
		_activeBtnPtr = _buttons.begin();
	}

	switch(_winTyp) {
	case REBEL_CHAIN:
		rResources->setChainWidgets((rebelChain *)this);
		break;
	case REBEL_MENU:
		rResources->setMenuWidgets((rebelMenu *)this);
		break;
	default:
		break;
	}
}

void
rebelWin::cutButton(rebelButton *b)
{
	vector<rebelButton *>::iterator bi;
	for(bi = _buttons.begin(); bi != _buttons.end(); ++bi) {
		if(*bi == b) {
			delete b;
			_buttons.erase(bi);
			break;
		}
	}
}

void
rebelWin::groupButton(rebelButton *b, long int key_mods)
{

	PtWidget_t *button = b->button();
	PtSetResource(button, Pt_ARG_FILL_COLOR, Pg_GREY, 0);

	if(key_mods & KEYMOD_CTRL) {
		_buttonGroup.push_back(b);
	}
	else if(key_mods & KEYMOD_SHIFT) {
		this->ungroupButtons();

		_buttonGroup.push_back(b);
		PhPoint_t top = b->origin();

		vector<rebelButton *>::iterator bi;
		for(bi = _buttons.begin(); bi != _buttons.end(); ++bi) {
			PhPoint_t o = (*bi)->origin();
			if((o.x == top.x) && (o.y > top.y)) {
				button = (*bi)->button();
				PtSetResource(button, Pt_ARG_FILL_COLOR, Pg_GREY, 0);
				_buttonGroup.push_back(*bi);
			}
		}
	}
}

void
rebelWin::ungroupButtons()
{
	vector<rebelButton *>::iterator bi;
	for(bi = _buttonGroup.begin(); bi != _buttonGroup.end(); ++bi) {
		PtWidget_t *button = (*bi)->button();
		PtSetResource(button, Pt_ARG_FILL_COLOR, Pg_LGREY, 0);
	}
	_buttonGroup.clear();
}

void rebelWin::snapToGrid()
{
	vector<rebelButton *>::iterator bi;

	for(bi = _buttons.begin(); bi != _buttons.end(); ++bi) {
		PhPoint_t o = (*bi)->origin();
		int cellx = (int)(((double)o.x / (double)_gridHorzStep) + 0.5);
		int celly = (int)(((double)o.y / (double)_gridVertStep) + 0.5);
		o.x = cellx * _gridHorzStep;
		o.y = celly * _gridVertStep;
		(*bi)->origin(o);
	}
}

PhPoint_t
rebelWin::getNextCell(PhPoint_t pos)
{
	vector<rebelButton *>::iterator bi;
	for(bi = _buttons.begin(); bi != _buttons.end(); ++bi) {
		PhPoint_t o = (*bi)->origin();
		if((o.x == pos.x) && (o.y == pos.y)) {
			pos.y += _gridVertStep;
			pos = getNextCell(pos);
		}
	}
	return(pos);
}


int
rebelWin::activeButtonCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// get pointer to this chain instance
	if(app == (rebelApp *)NULL) return( Pt_CONTINUE );

	rebelWin *w = app->activeWindow();
	if(w == (rebelWin *)NULL) return( Pt_CONTINUE );

	w->activeButton(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );

}

void
rebelWin::activeButton( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	rEdit.selectObject(widget);

	rebelButton *b;
	PtGetResource(widget, Pt_ARG_POINTER, &b, 0);
	vector<rebelButton *>::iterator bi;
	for(bi = _buttons.begin(); bi != _buttons.end(); ++bi) {
		if(*bi == b) {
			_activeBtnPtr = bi;
			break;
		}
	}

	_draggedWidget = (PtWidget_t *)NULL;
	return;

}


int
rebelWin::nextButtonCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// get pointer to this chain instance
	if(app == (rebelApp *)NULL) return( Pt_CONTINUE );

	rebelWin *w = app->activeWindow();
	if(w == (rebelWin *)NULL) return( Pt_CONTINUE );

	w->nextButton(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );

}

void
rebelWin::nextButton( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(_buttons.empty()) return;

	// increment the active state pointer
	++_activeBtnPtr;

	// if the pointer now points to one past the end of the vector,
	// make it point to the first state in the vector
	if(_activeBtnPtr == _buttons.end()) _activeBtnPtr = _buttons.begin();

	// mark the selected button as active
	rEdit.selectObject((*_activeBtnPtr)->button());

	setButtonResources(_activeBtnPtr);

	return;

}


int
rebelWin::prevButtonCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// get pointer to this chain instance
	if(app == (rebelApp *)NULL) return( Pt_CONTINUE );

	rebelWin *w = app->activeWindow();
	if(w == (rebelWin *)NULL) return( Pt_CONTINUE );

	w->prevButton(widget, apinfo, cbinfo);


	return( Pt_CONTINUE );

}

void
rebelWin::prevButton( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(_buttons.empty()) return;

	// if the active state pointer is pointing to the first state in the vector,
	// make it point to one past the end of the vector
	if(_activeBtnPtr == _buttons.begin()) _activeBtnPtr = _buttons.end();

	// now decrement the pointer
	--_activeBtnPtr;

	// mark the selected button as active
	rEdit.selectObject((*_activeBtnPtr)->button());

	setButtonResources(_activeBtnPtr);

	return;

}

void
rebelWin::setButtonResources(vector<rebelButton *>::iterator bp)
{
	extern rebelResource *rResources;

	// set the correct resource panel group
	switch(_winTyp) {
		rebelState *actState;
		rebelMenuItem *actMenuItem;
		rebelFunctionItem *actFuncItem;
		rebelVariableItem *actVarItem;
	case REBEL_WINDOW:
		break;
	case REBEL_CHAIN:
		actState = (rebelState *)*bp;
		rResources->setStateWidgets(actState);
		break;
	case REBEL_MENU:
		actMenuItem = (rebelMenuItem *)*bp;
		rResources->setVlistWidgets(actMenuItem);
		break;
	case REBEL_VARIABLE:
		actVarItem = (rebelVariableItem *)*bp;
		rResources->setVariableWidgets(actVarItem);
		break;
	case REBEL_FUNCTION:
		actFuncItem = (rebelFunctionItem *)*bp;
		rResources->setFunctionWidgets(actFuncItem);
		break;
	};

	return;

}

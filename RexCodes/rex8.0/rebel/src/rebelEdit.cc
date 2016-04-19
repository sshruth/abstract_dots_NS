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


extern rebelEdit rEdit;
extern rebelApp *app;

rebelEdit::rebelEdit()
{
	_selectedObject = (PtWidget_t *)NULL;
	_copiedObject = (PtWidget_t *)NULL;
	_focusWidget = (PtWidget_t *)NULL;
}

rebelEdit::~rebelEdit()
{
}

int
rebelEdit::cutCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	rEdit.cut(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );

}

void
rebelEdit::cut( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(_selectedObject == (PtWidget_t *)NULL) return;

	if(PtWidgetClass(_selectedObject) == PtWindow) {
		rebelWin *window;
		PtGetResource(_selectedObject, Pt_ARG_POINTER, &window, 0);
		window->cut(_selectedObject, apinfo, cbinfo);
		_selectedObject = (PtWidget_t *)NULL;
	}
	else if(PtWidgetClass(_selectedObject) == PtButton) {
		rebelButton *button;
		PtGetResource(_selectedObject, Pt_ARG_POINTER, &button, 0);
		button->cut(_selectedObject, apinfo, cbinfo);
		_selectedObject = (PtWidget_t *)NULL;
	}
	else if(PtWidgetClass(_selectedObject) == PtPolygon) {
		rebelState *state;
		PtGetResource(_selectedObject, Pt_ARG_POINTER, &state, 0);
		state->cutEscape(_selectedObject, apinfo, cbinfo);
		_selectedObject = (PtWidget_t *)NULL;
	}
	return;

}


int
rebelEdit::copyCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	rEdit.copy(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );

}

void
rebelEdit::copy( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(_focusWidget == (PtWidget_t *)NULL) return;

	copiedString.clear();

	int start;
	int end;
	int numChars = PtTextGetSelection(_focusWidget, &start, &end);
	if(numChars > 0) {
		char *str;
		PtGetResource(widget, Pt_ARG_TEXT_STRING, &str, 0);
		for(int i = start; i < end; ++i) copiedString.push_back(str[i]);
		copiedString.push_back('\0');
	}

	return;

}


int
rebelEdit::pasteCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	rEdit.paste(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );

}

void
rebelEdit::paste( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(_focusWidget == (PtWidget_t *)NULL) return;

	if(PtWidgetClass(_focusWidget) != PtText) return;

	if(!copiedString.empty()) PtSetResource(_focusWidget, Pt_ARG_TEXT_STRING, &copiedString[0], 0);

	PtArg_t args[2];
	int nArgs = 0;
	PtSetArg(&args[nArgs], Pt_ARG_COLOR, Pg_WHITE, 0); ++nArgs;
	PtSetArg(&args[nArgs], Pt_ARG_FILL_COLOR, Pg_DCYAN, 0); ++nArgs;
	PtSetResources(_focusWidget, nArgs, args);

	return;

}


int
rebelEdit::snapCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	rEdit.snap(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );

}

void
rebelEdit::snap( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(_selectedObject == (PtWidget_t *)NULL) return;

	if(PtWidgetClass(_selectedObject) == PtWindow) {
		rebelWin *window;
		PtGetResource(_selectedObject, Pt_ARG_POINTER, &window, 0);
		window->snapToGrid();
	}

	return;

}

void
rebelEdit::selectObject(PtWidget_t *object)
{
	// if the object is null, clear the selected object
	if(object == (PtWidget_t *)NULL) {
		_selectedObject = (PtWidget_t *)NULL;
		return;
	}

	// if there is already a selected object
	// deselect it
	if(_selectedObject != (PtWidget_t *)NULL) {
		if(PtWidgetClass(_selectedObject) == PtWindow) {
			PtSetResource(_selectedObject, Pt_ARG_FILL_COLOR, PgGrey(217), 0);
		}
		else if(PtWidgetClass(_selectedObject) == PtButton) {
			PtSetResource(_selectedObject, Pt_ARG_OUTLINE_COLOR, PgGrey(75), 0);
			PtSetResource(_selectedObject, Pt_ARG_COLOR, Pg_BLACK, 0);
		}
		else if(PtWidgetClass(_selectedObject) == PtPolygon) {
			PtSetResource(_selectedObject, Pt_ARG_LINE_WIDTH, 1, 0);
		}
		_selectedObject = (PtWidget_t *)NULL;
	}

	// if the user has selected a new object
	if(object) {
		_selectedObject = object;
		if(PtWidgetClass(_selectedObject) == PtWindow) {
			PtSetResource(_selectedObject, Pt_ARG_FILL_COLOR, Pg_RED, 0);
		}
		else if(PtWidgetClass(_selectedObject) == PtButton) {
			PtSetResource(_selectedObject, Pt_ARG_OUTLINE_COLOR, Pg_RED, 0);
			PtSetResource(_selectedObject, Pt_ARG_COLOR, Pg_BLUE, 0);
		}
		else if(PtWidgetClass(_selectedObject) == PtPolygon) {
			PtSetResource(_selectedObject, Pt_ARG_LINE_WIDTH, 2, 0);
			PtWidgetToBack(_selectedObject);
		}
	}
}

void
rebelEdit::copyObject()
{
	_copiedObject = _selectedObject;
}


int
rebelEdit::textGotFocusCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	rEdit.textGotFocus(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );

}

void
rebelEdit::textGotFocus( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	_focusWidget = widget;

	return;
}

int
rebelEdit::deleteCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	return( Pt_CONTINUE );

}


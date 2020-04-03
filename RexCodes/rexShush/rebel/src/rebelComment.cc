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

rebelComment::rebelComment()
{
	_butType = REBEL_COMMENT;
}

rebelComment::rebelComment(PtWidget_t *parent, PhPoint_t pos) : rebelButton("Comment", pos)
{
	extern ApDBase_t *rPictures;

	_butType = REBEL_COMMENT;
	_comment.clear();

	_button = ApCreateDBWidget(rPictures, "rebelCommentButton", parent, &_origin, 0, NULL);
	PtRealizeWidget(_button);

	PtSetResource(_button, Pt_ARG_POINTER, this, 0);
}

rebelComment::rebelComment(const rebelComment& c)
{
	_butType = c._butType;

	_button = c._button;
	_origin = c._origin;
	_comment.clear();
	_comment.insert(_comment.end(), c._comment.begin(), c._comment.end());
}

rebelComment::~rebelComment()
{
	_comment.clear();
}

rebelComment& rebelComment::operator=(const rebelComment& c)
{
	_butType = c._butType;

	_button = c._button;
	_origin = c._origin;
	_comment.clear();
	_comment.insert(_comment.end(), c._comment.begin(), c._comment.end());

	return(*this);
}


int
rebelComment::resourceCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// get a pointer to the window instance containing this comment
	PtWidget_t *wndw = ApGetInstance(widget);
	rebelWin *win;
	PtGetResource(wndw, Pt_ARG_POINTER, &win, 0);

	// get a pointer to this comment instance
	rebelButton *b;
	PtGetResource(widget, Pt_ARG_POINTER, &b, 0);
	
	// get the pointer event type
	PhPointerEvent_t *pe;
	pe = (PhPointerEvent_t *)PhGetData(cbinfo->event);

	// if no modifier keys were pressed, set the resource panel
	if(!(pe->key_mods & (KEYMOD_SHIFT | KEYMOD_CTRL | KEYMOD_ALT))) {
		if(win) win->ungroupButtons();
		b->resource(widget, apinfo, cbinfo);
	}

	// if modifier key was pressed, add the widget to the group
	else {
		if(win) win->groupButton(b, pe->key_mods);
	}

	return( Pt_CONTINUE );

}

void
rebelComment::resource( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	extern rebelResource *rResources;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	rResources->setCommentWidgets(this);

	return;
}



int
rebelComment::textCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// get a pointer to this comment instance
	PtWidget_t *w = ApGetInstance(widget);
	rebelResource *rsrc;
	PtGetResource(w, Pt_ARG_POINTER, &rsrc, 0);
	if(rsrc) {
		PtWidget_t *pg = rsrc->panelGroup();
		rebelComment *cmnt;
		PtGetResource(pg, Pt_ARG_POINTER, &cmnt, 0);

		// call this comment instance's text function
		if(cmnt) cmnt->text(widget, apinfo, cbinfo);
	}

	return( Pt_CONTINUE );

}

void
rebelComment::text( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	char *str;
	PtGetResource(ABW_commentMultiText, Pt_ARG_TEXT_STRING, &str, 0);

	_comment.clear();
	_comment.insert(_comment.end(), str, (str + strlen(str)));
	_comment.push_back('\0');

	PtSetResource(_button, Pt_ARG_TEXT_STRING, (char *)&_comment[0], 0);

	return;

}


void rebelComment::cut( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	char text[128];
	sprintf(text, "Do you really want to cut this comment?");
	char *btns[] = { "Cancel", "Cut" };
	int answer = PtAlert(ABW_rebelBaseWindow, NULL,
						 "Cut Comment", NULL,
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


void
rebelComment::saveButton(FILE *fp)
{
	fprintf(fp, "COMMENT %hd %hd\n", _origin.x, _origin.y);

	char *tok;
	if((tok = strtok(&_comment[0], "\n")) == (char *)NULL) return;
	do {
		fprintf(fp, "LINE %s\n", tok);
	} while ((tok = strtok(NULL, "\n")) != (char *)NULL);

	fprintf(fp, "DONE\n");
}


void
rebelComment::readButton(string line)
{
	if(line.find("NL") != string::npos) {
		_comment.push_back('\0');
		PtSetResource(_button, Pt_ARG_TEXT_STRING, (char *)&_comment[0], 0);
	}
	else _comment.insert(_comment.end(), line.begin(), line.end());

	return;

}

void rebelComment::genCode(FILE *fp) {}
void rebelComment::portButton(rebelWin *w, string line) {}

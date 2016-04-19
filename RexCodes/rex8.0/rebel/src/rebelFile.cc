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

extern rebelFile rFile;
extern rebelApp *app;

rebelFile::rebelFile()
{
	_ssetDir = "/rex/sset";
}

rebelFile::~rebelFile()
{
}

int
rebelFile::newCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	rFile.newPdgm(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );

}

void
rebelFile::newPdgm( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(app) {
		if(app->saved() == false) {
			char *btns[] = { "&Save", "&Discard" };
			int answer = PtAlert(ABW_rebelBaseWindow, NULL,
								 "Paradigm Save", NULL,
								 "Save current paradigm?", NULL,
								 2, (char const **)btns, NULL,
								 1, 2, Pt_LEFT | Pt_MODAL);
			switch(answer) {
			case 1:
				if(_name.empty()) {
					ApCreateModule(ABM_rebelSaveAsSelector, widget, cbinfo);
				}
				else {
					bool saved = savePdgm(widget, apinfo, cbinfo);
					if(saved == true) {
						delete app;

						new rebelApp(cbinfo);
						_name = "";
					}
				}
				break;
			case 2:
				delete app;

				new rebelApp(cbinfo);
				_name = "";
				break;
			}
		}
		else {
			delete app;

			new rebelApp(cbinfo);
			_name = "";
		}
	}
	else {
		new rebelApp(cbinfo);
		_name = "";
	}

	char title[128];
	strcpy(title, "Rebel Untitled");
	PtSetResource(ABW_rebelBaseWindow, Pt_ARG_WINDOW_TITLE, title, 0);
	return;

}


int
rebelFile::openCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	rFile.openPdgm(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );

}

void
rebelFile::openPdgm( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	char *str;
	PtGetResource(ABW_rebelOpenFileText, Pt_ARG_TEXT_STRING, &str, 0);
	string temp = str;

	// if there is an app, see if the user wants to save it
	// before starting the new app.
	if(app) {
		if(app->saved() == false) {
			char *btns[] = { "&Save", "&Discard" };
			int answer = PtAlert(ABW_rebelBaseWindow, NULL,
								 "Paradigm Save", NULL,
								 "Save current paradigm?", NULL,
								 2, (char const **)btns, NULL,
								 1, 2, Pt_LEFT | Pt_MODAL);
			switch(answer) {
			case 1:
				if(_name.empty()) {
					ApCreateModule(ABM_rebelSaveAsSelector, widget, cbinfo);
				}
				else {
					bool saved = savePdgm(widget, apinfo, cbinfo);
					if(saved == true) {
						delete app;

						new rebelApp(temp, cbinfo);
					}
				}
				break;
			case 2:
				delete app;

				new rebelApp(temp, cbinfo);
				break;
			}
		}
		else {
			delete app;

			new rebelApp(temp, cbinfo);
		}
	}

	// create a new paradigm
	else {
		new rebelApp(temp, cbinfo);
	}

	_name = temp;
	size_t extnPos;
	if((extnPos = _name.find(".d")) != string::npos) _name.erase(extnPos);
	if((extnPos = _name.find(".r")) != string::npos) _name.erase(extnPos);

	char title[128];
	strcpy(title, "Rebel ");
	strcat(title, _name.c_str());
	PtSetResource(ABW_rebelBaseWindow, Pt_ARG_WINDOW_TITLE, title, 0);

	return;

}


int
rebelFile::prototypeCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	rFile.prototype(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );

}

void
rebelFile::prototype( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(app == (rebelApp *)NULL) {
		PtNotice(ABW_rebelBaseWindow, NULL,
				 "Prototype Error", NULL,
				 "Create or open application before reading action prototypes", NULL,
				 "&OK", NULL, Pt_MODAL);
		return;
	}

	const char *btns[] = { "&Ok", "&Cancel" };
	char text[512];
	strcpy(text, _ssetDir.c_str());
	strcat(text, "/");
	int answer = PtPrompt(ABW_rebelBkgd, NULL,
						  "Prototype File Name", NULL,
						  "Open file:", NULL,
						  2, btns, NULL, 1, 1,
						  31, text, NULL, NULL, Pt_LEFT | Pt_MODAL);

	struct stat statBuf;
	int fileStat;
	char errorStr[256];
	
	switch(answer) {
	case 1:
		_prototype = text;

		fileStat = stat(_prototype.c_str(), &statBuf);
		if(!fileStat && (statBuf.st_size > 0)) {
			app->readPrototypes(_prototype.c_str(), statBuf.st_size);
		}
		else {
			sprintf(errorStr, "%s: %s", _prototype.c_str(), strerror(errno));
			PtNotice(ABW_rebelBaseWindow, NULL,
					 "Prototype file error", NULL,
					 errorStr, NULL,
					 "&OK", NULL, Pt_MODAL);
		}
		break;
	case 2:
		break;
	}

	return;

}


int
rebelFile::saveCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	rFile.savePdgm(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );

}

bool
rebelFile::savePdgm( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	
	bool saved = false;

	// if the user hasn't named the paradigm
	// prompt for a name
	if(_name.empty()) {
		const char *btns[] = { "&Ok", "&Cancel" };
		char text[512];
		strcpy(text, _ssetDir.c_str());
		strcat(text, "/");
		int answer = PtPrompt(ABW_rebelBkgd, NULL,
							  "Paradigm Name", NULL,
							  "Save As:", NULL,
							  2, btns, NULL, 1, 1,
							  31, text, NULL, NULL, Pt_LEFT | Pt_MODAL);

		switch(answer) {
		case 1:
			_name = text;
			size_t extnPos;
			if((extnPos = _name.find(".d")) != string::npos) _name.erase(extnPos);
			if((extnPos = _name.find(".r")) != string::npos) _name.erase(extnPos);

			saved = app->saveApp(_name);

			if(saved == true) {
				char title[128];
				strcpy(title, "Rebel ");
				strcat(title, _name.c_str());
				PtSetResource(ABW_rebelBaseWindow, Pt_ARG_WINDOW_TITLE, title, 0);
			}
			break;
		case 2:
			break;
		}
	}
	else {
		saved = app->saveApp(_name);
	}

	return(saved);
}

int
rebelFile::saveAsCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	rFile.savePdgmAs(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );

}

void
rebelFile::savePdgmAs( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// if the user is renaming a paradigm
	size_t extnPos;
	string oldName = "";
	if(!_name.empty()) oldName = _name;

	// build the file name
	char *str;
	PtGetResource(ABW_rebelSaveFileText, Pt_ARG_TEXT_STRING, &str, 0);
	_name = str;

	// make the variable _name contain only the base name
	if((extnPos = _name.find(".d")) != string::npos) _name.erase(extnPos);
	if((extnPos = _name.find(".r")) != string::npos) _name.erase(extnPos);

	// if there is an old file, copy it to the new name
	bool saved = false;
	if(!oldName.empty()) {
		// build a valid source file name from the old base name
		string oldSource = oldName;
		oldSource += ".r.c";

		// build a valid header file name from the old base name
		string oldHeader = oldName;
		oldHeader += ".r.h";

		// see if the old source file exists and is not empty
		struct stat statBuf;
		int fileStat = stat(oldSource.c_str(), &statBuf);

		// if the old source file exists and is not empty,
		// copy it to the new file name
		if(!fileStat && (statBuf.st_size > 0)) {
			// build a valid source file name from the new base name
			string newSource = _name;
			newSource += ".r.c";

			// build a valid header file name from the new base name
			string newHeader = _name;
			newHeader += ".r.h";

			// if the new source file exists, ask if the user wants to overwrite it
			bool doOverwrite = true;
			fileStat = stat(newSource.c_str(), &statBuf);
			if(!fileStat && (statBuf.st_size > 0)) {
				char *btns[] = { "&Overwrite", "&Cancel" };
				int answer = PtAlert(ABW_rebelBaseWindow, NULL,
									 "Paradigm Save As", NULL,
									 "File already exits", NULL,
									 2, (char const **)btns, NULL,
									 1, 2, Pt_LEFT | Pt_MODAL);
				switch(answer) {
				case 1:
					doOverwrite = true;
					break;
				case 2:
					doOverwrite = false;
					break;
				}
			}

			if(doOverwrite == true) {
				// copy the header if not to self
				if(strcmp(newHeader.c_str(), oldHeader.c_str())) {
					string command = "(cp ";
					command += oldHeader;
					command += " ";
					command += newHeader;
					command += ")";
					system(command.c_str());
				}
				sleep(1);

				// copy the source if not to self
				if(strcmp(newSource.c_str(), oldSource.c_str())) {
					string command = "(cp ";
					command += oldSource;
					command += " ";
					command += newSource;
					command += ")";
					system(command.c_str());
				}
				sleep(1);

				// save the paradigm
				saved = app->saveApp(_name);
			}
		}
	}
	else {
		// save the paradigm
		saved = app->saveApp(_name);
	}

	// if the save was successful and if there is an old header file,
	// copy the header using the new name
	if(saved == true) {
		char title[128];
		strcpy(title, "Rebel ");
		strcat(title, _name.c_str());
		PtSetResource(ABW_rebelBaseWindow, Pt_ARG_WINDOW_TITLE, title, 0);
	}

	return;
}


int
rebelFile::saveAsSpotCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	rFile.saveAsSpot(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );
}

void
rebelFile::saveAsSpot( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	bool saved;

	if(_name.empty()) {
		const char *btns[] = { "&Ok", "&Cancel" };
		char text[512];
		strcpy(text, _ssetDir.c_str());
		strcat(text, "/");
		int answer = PtPrompt(ABW_rebelBkgd, NULL,
							  "File Name", NULL,
							  "Open file:", NULL,
							  2, btns, NULL, 1, 1,
							  31, text, NULL, NULL, Pt_LEFT | Pt_MODAL);

		switch(answer) {
		case 1:
			size_t extnPos;
			_name = text;
			if((extnPos = _name.find(".d")) != string::npos) _name.erase(extnPos);
			if((extnPos = _name.find(".r")) != string::npos) _name.erase(extnPos);

			saved = app->saveApp(_name, true);

			if(saved == true) {
				char title[128];
				strcpy(title, "Rebel ");
				strcat(title, _name.c_str());
				PtSetResource(ABW_rebelBaseWindow, Pt_ARG_WINDOW_TITLE, title, 0);
			}
			break;
		case 2:
			break;
		}
	}
	else {
		saved = app->saveApp(_name, true);
	}

	return;

}

int
rebelFile::saveSnapshotCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
		
	rFile.saveSnapshot(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );

}

void
rebelFile::saveSnapshot( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	system("/usr/photon/bin/snapshot&");
	
	return;
}

int
rebelFile::closeCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	rFile.closePdgm(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );

}

void
rebelFile::closePdgm( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	bool saved = true;
	if(app) {
		if(app->saved() == false) {
			char *btns[] = { "&Save", "&Discard" };
			int answer = PtAlert(ABW_rebelBaseWindow, NULL,
								 "Paradigm Save", NULL,
								 "Save current paradigm?", NULL,
								 2, (char const **)btns, NULL,
								 1, 2, Pt_LEFT | Pt_MODAL);
			switch(answer) {
			case 1:
				if(_name.empty()) {
					ApCreateModule(ABM_rebelSaveAsSelector, widget, cbinfo);
				}
				else {
					saved = savePdgm(widget, apinfo, cbinfo);
					if(saved == true) {
						delete app;

						app = (rebelApp *)NULL;
					}
				}
				break;
			case 2:
				delete app;
				app = (rebelApp *)NULL;
				break;
			}
		}
		else {
			delete app;
			app = (rebelApp *)NULL;
		}
	}

	if(saved == true) {
		char title[128];
		strcpy(title, "Rebel Untitled");
		PtSetResource(ABW_rebelBaseWindow, Pt_ARG_WINDOW_TITLE, title, 0);
	}
	return;
}


int
rebelFile::exitCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	rFile.exitPdgm(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );
}

void
rebelFile::exitPdgm( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	if(app) {
		if(app->saved() == false) {
			char *btns[] = { "&Save", "&Discard" };
			int answer = PtAlert(ABW_rebelBaseWindow, NULL,
								 "Paradigm Save", NULL,
								 "Save current paradigm?", NULL,
								 2, (char const **)btns, NULL,
								 1, 2, Pt_LEFT | Pt_MODAL);
			switch(answer) {
			case 1:
				if(_name.length()) {
					bool saved = savePdgm(widget, apinfo, cbinfo);
					if(saved == true) exit(0);
				}
				else {
					ApCreateModule(ABM_rebelSaveAsSelector, widget, cbinfo);
				}
				break;
			case 2:
				exit(0);
				break;
			}
		}
		else {
			exit(0);
		}
	}
	else {
		exit(0);
	}
	return;

}


int
rebelFile::initSelectorCallback( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	rFile.initSelector(link_instance, apinfo, cbinfo);

	return( Pt_CONTINUE );

}

void
rebelFile::initSelector( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;
	if(ApName(link_instance) == ABN_rebelOpenSelector) {
		PtSetResource(ABW_rebelOpenFileSel, Pt_ARG_FS_ROOT_DIR, _ssetDir.c_str(), 0);
		PtSetResource(ABW_rebelOpenDirText, Pt_ARG_TEXT_STRING, _ssetDir.c_str(), 0);
		PtSetResource(ABW_rebelOpenFileText, Pt_ARG_TEXT_STRING, _ssetDir.c_str(), 0);
	}
	else if(ApName(link_instance) == ABN_rebelSaveAsSelector) {
		PtSetResource(ABW_rebelSaveFileSel, Pt_ARG_FS_ROOT_DIR, _ssetDir.c_str(), 0);
		PtSetResource(ABW_rebelSaveDirText, Pt_ARG_TEXT_STRING, _ssetDir.c_str(), 0);
		PtSetResource(ABW_rebelSaveFileText, Pt_ARG_TEXT_STRING, _ssetDir.c_str(), 0);
	}

	return;

}


int
rebelFile::setSsetDirCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	rFile.setSsetDir(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );

}

void
rebelFile::setSsetDir( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	char *str;
	PtGetResource(widget, Pt_ARG_TEXT_STRING, &str, 0);

	struct stat statBuf;
	int fileStat = stat(str, &statBuf);
	if(fileStat == -1) {
		char errorStr[256];
		sprintf(errorStr, "%s: %s", str, strerror(errno));
		PtNotice(ABW_rebelBaseWindow, NULL,
				 "Directory Error", NULL,
				 errorStr, NULL,
				 "&OK", NULL, Pt_MODAL);
		return;
	}

	// set the directory part of the file name
	PtWidget_t *dialog = PtWidgetParent(widget);
	if(ApName(dialog) == ABN_rebelOpenSelector) {
		PtSetResource(ABW_rebelOpenFileText, Pt_ARG_TEXT_STRING, str, 0);
		PtSetResource(ABW_rebelOpenFileSel, Pt_ARG_FS_ROOT_DIR, str, 0);
	}
	else if(ApName(dialog) == ABN_rebelSaveAsSelector) {
		PtSetResource(ABW_rebelSaveFileText, Pt_ARG_TEXT_STRING, str, 0);
		PtSetResource(ABW_rebelSaveFileSel, Pt_ARG_FS_ROOT_DIR, str, 0);
	}

	_ssetDir = str;

	return;

}


void rebelFile::fileName(string name)
{
	size_t slashPos;
	if((slashPos = name.rfind("/")) != string::npos) {
		string path = name;
		path.erase((slashPos + 1), (path.length() - slashPos - 1));
		_ssetDir = path;
	}
	else _ssetDir = "./";

	size_t extnPos;
	if((extnPos = name.find(".d")) != string::npos) name.erase(extnPos);
	if((extnPos = name.find(".r")) != string::npos) name.erase(extnPos);
	_name = name;
}

int
rebelFile::fileNameCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	rFile.fileName(widget, apinfo, cbinfo);

	return( Pt_CONTINUE );

}

void
rebelFile::fileName( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	PtFileSelItem_t *name;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// return immediately on mouse button press
	if(cbinfo->reason_subtype) return;

	PtWidget_t *txtWidget = (PtWidget_t *)NULL;
	PtWidget_t *dialog = PtWidgetParent(widget);
	if(ApName(dialog) == ABN_rebelOpenSelector) {
		txtWidget = ABW_rebelOpenFileText;
	}
	else if(ApName(dialog) == ABN_rebelSaveAsSelector) {
		txtWidget = ABW_rebelSaveFileText;
	}

	// process the file name on the mouse button release
	name = PtFSGetCurrent(widget);
	if(!name) _name = "invalid file name";
	else {
		if((name->type == Pt_FS_DIR_OP) || (name->type == Pt_FS_DIR_CL)) {
			_ssetDir = name->fullpath;
			_ssetDir += "/";
			PtSetResource(txtWidget, Pt_ARG_TEXT_STRING, _ssetDir.c_str(), 0);
		}
		else {
			_name = name->fullpath;
			PtSetResource(txtWidget, Pt_ARG_TEXT_STRING, _name.c_str(), 0);
		}
	}
	return;

}

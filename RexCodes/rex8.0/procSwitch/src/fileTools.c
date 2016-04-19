/* Y o u r   D e s c r i p t i o n                       */
/*                            AppBuilder Photon Code Lib */
/*                                         Version 1.14C */

/* Standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <process.h>
#include <sys/stat.h>

/* Toolkit headers */
#include <Ph.h>
#include <Pt.h>
#include <Ap.h>

/* Local headers */
#include "ph_procSwitch.h"
#include "abimport.h"
#include "proto.h"

char procRootDir[128] = { 0 };
char rootDir[128] = { 0 };
char dataDir[128] = { 0 };
char processList[P_NPROC][P_LPROCNAME];
char rootFile[P_LROOTNAME];

int dataState = 0;
int xloc = 0;
int yloc = 0;
int wsize = 0;
int hsize = 0;

int initProcSelDialog( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;
	char wd[PATH_MAX + 2];

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	/* get current working directory */
	getcwd(wd, PATH_MAX + 1);
	
	/* set up the default process run directory */
	if(!strlen(procRootDir)) strcpy(procRootDir, wd);

	/* set the root directories */
	PtSetArg(&arg, Pt_ARG_TEXT_STRING, procRootDir, 0);
	PtSetResources(ABW_procFileDirText, 1, &arg);
	PtSetResources(ABW_procFileName, 1, &arg);

	/* set the root directory of the file selection widget */
	PtSetArg(&arg, Pt_ARG_FS_ROOT_DIR, procRootDir, 0);
	PtSetResources(ABW_procFileSelList, 1, &arg);

	return( Pt_CONTINUE );
}

int setProcDir( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;
	char *string;
	
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the directory */
	PtSetArg(&arg, Pt_ARG_TEXT_STRING, &string, 0);
	PtGetResources(widget, 1, &arg);

	strcpy(procRootDir, string);

	/* set the directory part of the file name */
	PtSetArg(&arg, Pt_ARG_TEXT_STRING, string, 0);
	PtSetResources(ABW_procFileName, 1, &arg);

	/* set the root directory of the file selection widget */
	PtSetArg(&arg, Pt_ARG_FS_ROOT_DIR, string, 0);
	PtSetResources(ABW_procFileSelList, 1, &arg);

	return( Pt_CONTINUE );
}

int getProcFileName( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;
	PtFileSelItem_t *fileName;
	char file[80];
	
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// return immediately on mouse button press
	if(cbinfo->reason_subtype) return(Pt_CONTINUE);

	// process the file name on the mouse button release
	fileName = PtFSGetCurrent(widget);
	if(!fileName) strcpy(file, "invalid file name");
	else {
		strcpy(file, fileName->fullpath);
		if((fileName->type == Pt_FS_DIR_OP) || (fileName->type == Pt_FS_DIR_CL)) {
			strcat(file, "/");
		}
	}

	// set the text value of the file name text widget
	PtSetArg(&arg, Pt_ARG_TEXT_STRING, &file, 0);
	PtSetResources(ABW_procFileName, 1, &arg);

	return( Pt_CONTINUE );
}

int runProc( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PROCTBL_P p;			/* pointer to rex process table */
	PtArg_t args[2];
	PhWindowEvent_t winEvent;
	char *fileName;
	char errmsg[128];

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the name of the process file */
	fileName = extractProcFileProp();
	
	if(!fileName) {
		PtNotice(ABW_base, NULL, "Process Warning", NULL,
				 "runproc(): Bad process name", "Helvetica09ba",
				 "OK", "Helvetica09ba",
				 Pt_CENTER | Pt_BLOCK_PARENT);
		return(Pt_CONTINUE);
	}

	/* if saving data, stop saving */
	if(i_b->i_flags & (I_EOUT | I_AOUT)) {
		sendnm((PROCTBL_P)&i_b->ptbl[i_b->scrb_pi], "Bdata", "i", NS, 0);

		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetArg(&args[1], Pt_ARG_FILL_COLOR, Pg_RED, 0);
		PtSetResources(ABW_dataKeepBothToggle, 2, args);
	}
	else if(i_b->i_flags & I_EOUT) {
		sendnm((PROCTBL_P)&i_b->ptbl[i_b->scrb_pi], "Edata", "i", NS, 0);

		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetArg(&args[1], Pt_ARG_FILL_COLOR, Pg_RED, 0);
		PtSetResources(ABW_dataKeepEventsToggle, 2, args);
	}

	p = getptp(fileName);

	/* if this is a new int process and if another int process is running,
	 * stop it the other int process
	 */
	if((p == NP) && (i_b->ph_int_pi > 0)) {
		memset(&winEvent, 0, sizeof(winEvent));
		winEvent.event_f = Ph_WM_HIDE;
		winEvent.state_f = Ph_WM_STATE_ISICONIFIED;
		winEvent.event_state = Ph_WM_EVSTATE_HIDE;
		PtForwardWindowTaskEvent(i_b->ptbl[i_b->ph_int_pi].p_coid, &winEvent);
	}

	/* if the process doesn't exist, create it */
	if(p == NP) {
		memset(rootFile, 0, P_LROOTNAME);
		p = newproc(fileName, 'r', 'n');
	}
	/* if the process exists inform user of error */
	else {
		sprintf(errmsg, "runProc(): process with name %s already loaded", fileName);
		PtNotice(ABW_base, NULL, "Process Warning", NULL,
				 errmsg, "Helvetica09ba",
				 "OK", "Helvetica09ba",
				 Pt_CENTER | Pt_BLOCK_PARENT);
		return( Pt_CONTINUE );
	}

	/* if newproc fails notify user */
	if(p == NP) {
		sprintf(errmsg, "runProc() couldn't start %s", fileName);
		PtNotice(ABW_base, NULL, "Process Warning", NULL,
				 errmsg, "Helvetica09ba",
				 "OK", "Helvetica09ba",
				 Pt_CENTER | Pt_BLOCK_PARENT);
	}

	return( Pt_CONTINUE );
}

int loadProc( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PROCTBL_P p;			/* pointer to rex process table */
	char *fileName;
	char errmsg[128];

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the name of the process file */
	fileName = extractProcFileProp();

	if(!fileName) {
		PtNotice(ABW_base, NULL, "Process Warning", NULL,
				 "loadproc(): Bad process name", "Helvetica09ba",
				 "OK", "Helvetica09ba",
				 Pt_CENTER | Pt_BLOCK_PARENT);
		return(Pt_CONTINUE);
	}

	/* if the process doesn't exist, create it */
	if((p = getptp(fileName)) == NP) {
		memset(rootFile, 0, P_LROOTNAME);

		p = newproc(fileName, 's', 'i');

		/* if there is no process running make this the running process */
		/*		if(i_b->ph_int_pi < 0) p = newproc(fileName, 'r', 'n'); */

		/* other wise make this a stopped process */
		/*		else p = newproc(fileName, 's', 'i'); */
	}
	/* if the process exists inform user of error */
	else {
		sprintf(errmsg, "loadProc(): process with name %s already loaded", fileName);
		PtNotice(ABW_base, NULL, "Process Warning", NULL,
				 errmsg, "Helvetica09ba",
				 "OK", "Helvetica09ba",
				 Pt_CENTER | Pt_BLOCK_PARENT);
		return( Pt_CONTINUE );
	}

	/* if newproc fails notify user */
	if(p == NP) {
		sprintf(errmsg, "loadProc() couldn't load %s", fileName);
		PtNotice(ABW_base, NULL, "Process Warning", NULL,
				 errmsg, "Helvetica09ba",
				 "OK", "Helvetica09ba",
				 Pt_CENTER | Pt_BLOCK_PARENT);
	}
	return( Pt_CONTINUE );
}

/* this function extracts the name of the process
 * from the full path name in the text widget
 * and sets the location of the process switcher window
 * so that the process window can positioned
 */
char *extractProcFileProp(void)
{
	PhPoint_t loc;
	PhRect_t frame;
	PhRect_t extent;
	PtArg_t arg;
	int slen;
	char *cPos;
	char *pathFileName;
	char *fileName;

	/* get the path and file name from the text widget */
	PtSetArg(&arg, Pt_ARG_TEXT_STRING, 0, 0);
	PtGetResources(ABW_procFileName, 1, &arg);
	pathFileName = (char *)arg.value;
	
	/* extract the file name from the full path name */
	cPos = (char *)NULL;
	slen = strlen(pathFileName);
	if(slen) {
		cPos = strrchr(pathFileName, (int)'/');
		if(cPos == NULL) fileName = pathFileName;
		else if((int)(cPos - pathFileName) != (slen - 1)) {
			fileName = cPos + 1;
		}
		else fileName = (char *)NULL;
	}
	else fileName = (char *)NULL;
	
	/* get the position of the base window
	 * and set the location of the toolbar window
	 */
	if(fileName) {
		PtWindowGetFrameSize(ABW_base, &frame);
		PtGetAbsPosition(ABW_base, &loc.x, &loc.y);
		loc.x -= frame.ul.x;
		loc.y -= frame.ul.y;

		PtWidgetExtent(ABW_base, &extent);
		xloc = loc.x + frame.ul.x + extent.lr.x + frame.lr.x;
		yloc = loc.y;
	}
	
	return(fileName);
}

int initDataSelDialog( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	if(!strlen(dataDir)) strcpy(dataDir, "/");

	/* set the root directories */
	PtSetArg(&arg, Pt_ARG_TEXT_STRING, dataDir, 0);
	PtSetResources(ABW_dataFileDirText, 1, &arg);
	PtSetResources(ABW_dataFileName, 1, &arg);

	/* set the root directory of the file selection widget */
	PtSetArg(&arg, Pt_ARG_FS_ROOT_DIR, dataDir, 0);
	PtSetResources(ABW_dataFileSelList, 1, &arg);

	return( Pt_CONTINUE );
}

int setDataDir( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;
	char *string;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the directory */
	PtSetArg(&arg, Pt_ARG_TEXT_STRING, &string, 0);
	PtGetResources(widget, 1, &arg);

	strcpy(dataDir, string);

	/* set the directory part of the file name */
	PtSetArg(&arg, Pt_ARG_TEXT_STRING, string, 0);
	PtSetResources(ABW_dataFileName, 1, &arg);

	/* set the root directory of the file selection widget */
	PtSetArg(&arg, Pt_ARG_FS_ROOT_DIR, string, 0);
	PtSetResources(ABW_dataFileSelList, 1, &arg);

	return( Pt_CONTINUE );
}

int getDataFileName( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;
	PtFileSelItem_t *fileName;
	char file[80];

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// return immediately on mouse button press
	if(cbinfo->reason_subtype) return(Pt_CONTINUE);

	// process the file name on the mouse button release
	fileName = PtFSGetCurrent(widget);
	if(!fileName) strcpy(file, "invalid file name");
	else {
		strcpy(file, fileName->fullpath);
		if((fileName->type == Pt_FS_DIR_OP) || (fileName->type == Pt_FS_DIR_CL)) {
			strcat(file, "/");
		}
	}

	// set the text value of the file name text widget
	PtSetArg(&arg, Pt_ARG_TEXT_STRING, &file, 0);
	PtSetResources(ABW_dataFileName, 1, &arg);

	return( Pt_CONTINUE );
}

int openData( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PROCTBL_P pScribe;
	char *pathFileName;
	char winLabel[1028];
	
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* initialize pointer to scribe */
	pScribe = (PROCTBL_P)&i_b->ptbl[i_b->scrb_pi];

	/* get the path and file name from the text widget */
	PtGetResource(ABW_dataFileName, Pt_ARG_TEXT_STRING, &pathFileName, 0);

	/* send the command to open the data file to scribe */
	sendnm(pScribe, "file", "o", pathFileName, 0);

	/* set the name of the data file in the procSwitch toolbar label */
	strcpy(winLabel, "REX");
	strcat(winLabel, "  ");
	strcat(winLabel, pathFileName);
	PtSetResource(ABW_base, Pt_ARG_WINDOW_TITLE, winLabel, 0);

	PtSetResource(ABW_dataKeepBothToggle, Pt_ARG_FLAGS, Pt_TRUE, Pt_SELECTABLE);
	PtSetResource(ABW_dataKeepEventsToggle, Pt_ARG_FLAGS, Pt_TRUE, Pt_SELECTABLE);

	return( Pt_CONTINUE );
}

int closeDataFile( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PROCTBL_P pScribe;
	PtArg_t args[2];

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* initialize pointer to scribe */
	pScribe = (PROCTBL_P)&i_b->ptbl[i_b->scrb_pi];

	/* send the command to close the data file to scribe */
	sendnm(pScribe, "file", "c", NS, 0);

	PtSetResource(ABW_base, Pt_ARG_WINDOW_TITLE, "REX", 0);

	PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
	PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_FALSE, Pt_SELECTABLE);
	PtSetResources(ABW_dataKeepBothToggle, 2, args);
	PtSetResources(ABW_dataKeepEventsToggle, 2, args);

	return( Pt_CONTINUE );
}

int switchData( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[2];
	PtBasicCallback_t *data;
	PROCTBL_P pScribe;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* test to make sure clock is running */
	if( ! (i_b->i_flags & I_GO)) {
		rxerr("Can't toggle data saving if clock is stopped");

		PtSetArg(&args[0], Pt_ARG_FILL_COLOR, Pg_RED, 0);
		PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_dataKeepEventsToggle, 2, args);
		PtSetResources(ABW_dataKeepBothToggle, 2, args);

		return( Pt_END );
	}

	/* initialize pointer to scribe */
	pScribe = (PROCTBL_P)&i_b->ptbl[i_b->scrb_pi];

	data = (PtBasicCallback_t *)cbinfo->cbdata;
	if(data->value) {
		/*
		 * if data keeping is to start
		 */
		if(ApName(widget) == ABN_dataKeepBothToggle) {
			/*
			 * if we are already keeping event data
			 * just start keeping analog data
			 */
			if(dataState == 1) {
				/* set the keep events toggle red */
				PtSetArg(&args[0], Pt_ARG_FILL_COLOR, Pg_RED, 0);
				PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
				PtSetResources(ABW_dataKeepEventsToggle, 2, args);

				sendnm(pScribe, "Adata", "k", NS, 0);
			}
			else {
				sendnm(pScribe, "Bdata", "k", NS, 0);
			}

			dataState = 2;
			PtSetResource(ABW_dataKeepBothToggle, Pt_ARG_FILL_COLOR, Pg_GREEN, 0);
		}
		else if(ApName(widget) == ABN_dataKeepEventsToggle) {
			/*
			 * if we already keeping analog and event data,
			 * just stop keep analog data
			 */
			if(dataState == 2) {
				PtSetArg(&args[0], Pt_ARG_FILL_COLOR, Pg_RED, 0);
				PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
				PtSetResources(ABW_dataKeepBothToggle, 2, args);

				sendnm(pScribe, "Adata", "i", NS, 0);
			}
			else {
				sendnm(pScribe, "Edata", "k", NS, 0);
			}

			dataState = 1;
			PtSetResource(ABW_dataKeepEventsToggle, Pt_ARG_FILL_COLOR, Pg_GREEN, 0);
		}
	}
	else {
		/*
		 * if data keeping is to stop
		 */
		if(ApName(widget) == ABN_dataKeepBothToggle) {
			sendnm(pScribe, "Bdata", "i", NS, 0);

			PtSetResource(ABW_dataKeepBothToggle, Pt_ARG_FILL_COLOR, Pg_RED, 0);
		}
		else if(ApName(widget) == ABN_dataKeepEventsToggle) {
			sendnm(pScribe, "Edata", "i", NS, 0);

			PtSetResource(ABW_dataKeepEventsToggle, Pt_ARG_FILL_COLOR, Pg_RED, 0);
		}

		dataState = 0;
	}

	return( Pt_CONTINUE );
}

int initRootSelDialog( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;
	PtFileSelItem_t *fileName;
	char file[80];

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	/* set up the default process run directory */
	if(!strlen(rootDir)) {
		if(strlen(procRootDir)) {
			strcpy(rootDir, procRootDir);
			strcat(rootDir, "/rt");
		}
		else strcpy(rootDir, "/rex/run/rt");
	}

	/* set the root directories */
	PtSetArg(&arg, Pt_ARG_TEXT_STRING, rootDir, 0);
	PtSetResources(ABW_rootFileDirText, 1, &arg);
	PtSetResources(ABW_rootFileName, 1, &arg);

	/* set the root directory of the file selection widget */
	PtSetArg(&arg, Pt_ARG_FS_ROOT_DIR, rootDir, 0);
	PtSetResources(ABW_rootFileSelList, 1, &arg);
	return( Pt_CONTINUE );
}

int setRootDir( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;
	char *string;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the directory */
	PtSetArg(&arg, Pt_ARG_TEXT_STRING, &string, 0);
	PtGetResources(widget, 1, &arg);

	strcpy(rootDir, string);

	/* set the directory part of the file name */
	PtSetArg(&arg, Pt_ARG_TEXT_STRING, string, 0);
	PtSetResources(ABW_rootFileName, 1, &arg);

	/* set the root directory of the file selection widget */
	PtSetArg(&arg, Pt_ARG_FS_ROOT_DIR, string, 0);
	PtSetResources(ABW_rootFileSelList, 1, &arg);

	return( Pt_CONTINUE );
}

int getRootFileName( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;
	PtFileSelItem_t *fileName;
	char file[80];

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// return immediately on mouse button press
	if(cbinfo->reason_subtype) return(Pt_CONTINUE);

	// process the file name on the mouse button release
	fileName = PtFSGetCurrent(widget);
	if(!fileName) strcpy(file, "invalid file name");
	else {
		strcpy(file, fileName->fullpath);
		if((fileName->type == Pt_FS_DIR_OP) || (fileName->type == Pt_FS_DIR_CL)) {
			strcat(file, "/");
		}
	}

	// set the text value of the file name text widget
	PtSetArg(&arg, Pt_ARG_TEXT_STRING, &file, 0);
	PtSetResources(ABW_rootFileName, 1, &arg);

	return( Pt_CONTINUE );
}

int writeAllRoots( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;
	PtWidget_t *parent;
	struct stat statBuf;
	char messg[256];
	char *btns[] = { "&Over write", "&Append" };
	char *btnFonts[] = { "Helvetica09ba", "Helvetica09ba"	};
	char *file;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(!i_b->ph_int_pi) {
		parent = ApGetInstance(ApWidget(cbinfo));
		PtNotice(parent, NULL, "Root Warning", NULL,
				 "Must start int process\nbefore writing roots", "Helvetica09ba",
				 "OK", "Helvetica09ba",
				 Pt_CENTER | Pt_BLOCK_PARENT);
		return( Pt_CONTINUE );
	}

	/* set the root write flag */
	i_b->i_rtflag = RT_WMENU|RT_WSTATE|RT_WUMENU;

	/* copy the root file name to shared memory */
	PtSetArg(&arg, Pt_ARG_TEXT_STRING, &file, 0);
	PtGetResources(ABW_rootFileName, 1, &arg);
 
	/* check to see if root file exists */
	if(stat(file, &statBuf) != -1) {
		if(statBuf.st_size) {
			sprintf(messg, "Over write Root File %s?", file);
			switch(PtAlert(PtWidgetParent(widget), NULL, "Overwrite File", NULL,
						   messg, "Helvetical09ba",
						   2, (char const **)btns, (char const **)btnFonts, 1, 2, Pt_MODAL)) {
			case 1:
				i_b->i_rtappend = 0;
				break;
			case 2:
				i_b->i_rtappend = 1;
				break;
			}
		}
	}

	/* send message to running process to write its roots */
	strncpy((char *)i_b->i_rtname, file, P_ISLEN);
	r_sendmsg((PROCTBL_P)&i_b->ptbl[i_b->ph_int_pi], G_RTEXEC);

	return( Pt_CONTINUE );
}

int writeSysUserRoots( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;
	PtWidget_t *parent;
	struct stat statBuf;
	char messg[256];
	char *btns[] = { "&Over write", "&Append" };
	char *btnFonts[] = { "Helvetica09ba", "Helvetica09ba"	};
	char *file;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(!i_b->ph_int_pi) {
		parent = ApGetInstance(ApWidget(cbinfo));
		PtNotice(parent, NULL, "Root Warning", NULL,
				 "Must start int process\nbefore writing roots", "Helvetica09ba",
				 "OK", "Helvetica09ba",
				 Pt_CENTER | Pt_BLOCK_PARENT);
		return( Pt_CONTINUE );
	}

	/* set the root write flag */
	i_b->i_rtflag = RT_WMENU|RT_WUMENU;

	/* copy the root file name to shared memory */
	PtSetArg(&arg, Pt_ARG_TEXT_STRING, &file, 0);
	PtGetResources(ABW_rootFileName, 1, &arg);
 
	/* check to see if root file exists */
	if(stat(file, &statBuf) != -1) {
		if(statBuf.st_size) {
			sprintf(messg, "Over write Root File %s?", file);
			switch(PtAlert(PtWidgetParent(widget), NULL, "Overwrite File", NULL,
						   messg, "Helvetical09ba",
						   2, (char const **)btns, (char const **)btnFonts, 1, 2, Pt_MODAL)) {
			case 1:
				i_b->i_rtappend = 0;
				break;
			case 2:
				i_b->i_rtappend = 1;
				break;
			}
		}
	}

	/* send message to running process to write its roots */
	strncpy((char *)i_b->i_rtname, file, P_ISLEN);
	r_sendmsg((PROCTBL_P)&i_b->ptbl[i_b->ph_int_pi], G_RTEXEC);

	return( Pt_CONTINUE );
}

int writeSysRoots( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;
	PtWidget_t *parent;
	struct stat statBuf;
	char messg[256];
	char *btns[] = { "&Over write", "&Append" };
	char *btnFonts[] = { "Helvetica09ba", "Helvetica09ba"	};
	char *file;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(!i_b->ph_int_pi) {
		parent = ApGetInstance(ApWidget(cbinfo));
		PtNotice(parent, NULL, "Root Warning", NULL,
				 "Must start int process\nbefore writing roots", "Helvetica09ba",
				 "OK", "Helvetica09ba",
				 Pt_CENTER | Pt_BLOCK_PARENT);
		return( Pt_CONTINUE );
	}

	/* set the root write flag */
	i_b->i_rtflag = RT_WMENU;

	/* copy the root file name to shared memory */
	PtSetArg(&arg, Pt_ARG_TEXT_STRING, &file, 0);
	PtGetResources(ABW_rootFileName, 1, &arg);
 
	/* check to see if root file exists */
	if(stat(file, &statBuf) != -1) {
		if(statBuf.st_size) {
			sprintf(messg, "Over write Root File %s?", file);
			switch(PtAlert(PtWidgetParent(widget), NULL, "Overwrite File", NULL,
						   messg, "Helvetical09ba",
						   2, (char const **)btns, (char const **)btnFonts, 1, 2, Pt_MODAL)) {
			case 1:
				i_b->i_rtappend = 0;
				break;
			case 2:
				i_b->i_rtappend = 1;
				break;
			}
		}
	}

	/* send message to running process to write its roots */
	strncpy((char *)i_b->i_rtname, file, P_ISLEN);
	r_sendmsg((PROCTBL_P)&i_b->ptbl[i_b->ph_int_pi], G_RTEXEC);

	return( Pt_CONTINUE );
}

int writeUserRoots( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;
	PtWidget_t *parent;
	struct stat statBuf;
	char messg[256];
	char *btns[] = { "&Over write", "&Append" };
	char *btnFonts[] = { "Helvetica09ba", "Helvetica09ba"	};
	char *file;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(!i_b->ph_int_pi) {
		parent = ApGetInstance(ApWidget(cbinfo));
		PtNotice(parent, NULL, "Root Warning", NULL,
				 "Must start int process\nbefore writing roots", "Helvetica09ba",
				 "OK", "Helvetica09ba",
				 Pt_CENTER | Pt_BLOCK_PARENT);
		return( Pt_CONTINUE );
	}

	/* set the root write flag */
	i_b->i_rtflag = RT_WUMENU;

	/* copy the root file name to shared memory */
	PtSetArg(&arg, Pt_ARG_TEXT_STRING, &file, 0);
	PtGetResources(ABW_rootFileName, 1, &arg);
 
	/* check to see if root file exists */
	if(stat(file, &statBuf) != -1) {
		if(statBuf.st_size) {
			sprintf(messg, "Over write Root File %s?", file);
			switch(PtAlert(PtWidgetParent(widget), NULL, "Overwrite File", NULL,
						   messg, "Helvetical09ba",
						   2, (char const **)btns, (char const **)btnFonts, 1, 2, Pt_MODAL)) {
			case 1:
				i_b->i_rtappend = 0;
				break;
			case 2:
				i_b->i_rtappend = 1;
				break;
			}
		}
	}

	/* send message to running process to write its roots */
	strncpy((char *)i_b->i_rtname, file, P_ISLEN);
	r_sendmsg((PROCTBL_P)&i_b->ptbl[i_b->ph_int_pi], G_RTEXEC);

	return( Pt_CONTINUE );
}

int writeStateRoots( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;
	PtWidget_t *parent;
	struct stat statBuf;
	char messg[256];
	char *btns[] = { "&Over write", "&Append" };
	char *btnFonts[] = { "Helvetica09ba", "Helvetica09ba"	};
	char *file;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(!i_b->ph_int_pi) {
		parent = ApGetInstance(ApWidget(cbinfo));
		PtNotice(parent, NULL, "Root Warning", NULL,
				 "Must start int process\nbefore writing roots", "Helvetica09ba",
				 "OK", "Helvetica09ba",
				 Pt_CENTER | Pt_BLOCK_PARENT);
		return( Pt_CONTINUE );
	}

	/* set the root write flag */
	i_b->i_rtflag = RT_WSTATE;

	/* copy the root file name to shared memory */
	PtSetArg(&arg, Pt_ARG_TEXT_STRING, &file, 0);
	PtGetResources(ABW_rootFileName, 1, &arg);
 
	/* check to see if root file exists */
	if(stat(file, &statBuf) != -1) {
		if(statBuf.st_size) {
			sprintf(messg, "Over write Root File %s?", file);
			switch(PtAlert(PtWidgetParent(widget), NULL, "Overwrite File", NULL,
						   messg, "Helvetical09ba",
						   2, (char const **)btns, (char const **)btnFonts, 1, 2, Pt_MODAL)) {
			case 1:
				i_b->i_rtappend = 0;
				break;
			case 2:
				i_b->i_rtappend = 1;
				break;
			}
		}
	}

	/* send message to running process to write its roots */
	strncpy((char *)i_b->i_rtname, file, P_ISLEN);
	r_sendmsg((PROCTBL_P)&i_b->ptbl[i_b->ph_int_pi], G_RTEXEC);

	return( Pt_CONTINUE );
}

int readRoots( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;
	PtWidget_t *parent;
	char *file;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(i_b->i_flags & I_GO) {
		parent = ApGetInstance(ApWidget(cbinfo));
		PtNotice(parent, NULL, "Root Warning", NULL,
				 "Cannot read roots while clock is running", "Helvetica09ba",
				 "OK", "Helvetica09ba",
				 Pt_CENTER | Pt_BLOCK_PARENT);
		return( Pt_CONTINUE );
	}
	if(!i_b->ph_int_pi) {
		parent = ApGetInstance(ApWidget(cbinfo));
		PtNotice(parent, NULL, "Root Warning", NULL,
				 "Must start int process\nbefore reading roots", "Helvetica09ba",
				 "OK", "Helvetica09ba",
				 Pt_CENTER | Pt_BLOCK_PARENT);
		return( Pt_CONTINUE );
	}
			
	i_b->i_rtflag = RT_READ;

	/* copy the root file name to shared memory */
	PtSetArg(&arg, Pt_ARG_TEXT_STRING, &file, 0);
	PtGetResources(ABW_rootFileName, 1, &arg);
	strncpy((char *)i_b->i_rtname, file, P_ISLEN);

	/* send message to running process to write its roots */
	r_sendmsg((PROCTBL_P)&i_b->ptbl[i_b->ph_int_pi], G_RTEXEC);

	return( Pt_CONTINUE );
}

int startWindow( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PROCTBL_P p;			/* pointer to rex process table */
	PhPoint_t loc;
	PhRect_t extent;
	PtWidget_t *parent;

	char name[64];
	char numb[4];

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* if the window doesn't have a size or location
	 * compute defaults based on the procSwitch window
	 * get the position of the base window
	 * and set the location of the toolbar window
	 */
	PtGetAbsPosition(ABW_base, &loc.x, &loc.y);
	loc.x -= 6;
	loc.y -= 21;
	PtWidgetExtent(ABW_base, &extent);
	xloc = loc.x + 5;
	yloc = loc.y + 130;
	
	strcpy(name, "../ewind/src/gcc_ntox86/ewind");
	memset(rootFile, 0, P_LROOTNAME);
	if(!newproc(name, 'r', 'n')) {
		parent = ApGetInstance(ApWidget(cbinfo));
		PtNotice(parent, NULL, "Window Warning", NULL,
				 "procSwitch can't start ewind", "Helvetica09ba",
				 "OK", "Helvetica09ba",
				 Pt_CENTER | Pt_BLOCK_PARENT);
		return( Pt_CONTINUE );
	}

	return( Pt_CONTINUE );
}

int startRaster( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PhPoint_t loc;
	PhRect_t extent;
	PtWidget_t *parent;

	char name[64];
	char numb[4];

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* if the window doesn't have a size or location
	 * compute defaults based on the procSwitch window
	 * get the position of the base window
	 * and set the location of the toolbar window
	 */
	PtGetAbsPosition(ABW_base, &loc.x, &loc.y);
	loc.x -= 6;
	loc.y -= 21;
	PtWidgetExtent(ABW_base, &extent);
	xloc = loc.x + 5;
	yloc = loc.y + 130;
	
	strcpy(name, "../ras/src/gcc_ntox86/ras");
	memset(rootFile, 0, P_LROOTNAME);
	if(!newproc(name, 'r', 'n')) {
		parent = ApGetInstance(ApWidget(cbinfo));
		PtNotice(parent, NULL, "Raster Warning", NULL,
				 "procSwitch can't start ras", "Helvetica09ba",
				 "OK", "Helvetica09ba",
				 Pt_CENTER | Pt_BLOCK_PARENT);
		return( Pt_CONTINUE );
	}

	return( Pt_CONTINUE );
}

int startRunLine( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtWidget_t *parent;
	PhPoint_t loc;
	PhRect_t extent;

	char name[64];
	char numb[4];

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* if the window doesn't have a size or location
	 * compute defaults based on the procSwitch window
	 * get the position of the base window
	 * and set the location of the toolbar window
	 */
	PtGetAbsPosition(ABW_base, &loc.x, &loc.y);
	loc.x -= 6;
	loc.y -= 21;
	PtWidgetExtent(ABW_base, &extent);
	xloc = loc.x + 575;
	yloc = loc.y + 130;

	strcpy(name, "../rline/src/gcc_ntox86/rline");
	memset(rootFile, 0, P_LROOTNAME);
	if(!newproc(name, 'r', 'n')) {
		parent = ApGetInstance(ApWidget(cbinfo));
		PtNotice(parent, NULL, "Running Line Warning", NULL,
				 "procSwitch can't start rline", "Helvetica09ba",
				 "OK", "Helvetica09ba",
				 Pt_CENTER | Pt_BLOCK_PARENT);
		return( Pt_CONTINUE );
	}

	return( Pt_CONTINUE );
}

int startXcorr( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PhPoint_t loc;
	PhRect_t extent;
	PtWidget_t *parent;

	char name[64];
	char numb[4];

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(!i_b->ph_int_pi) {
		parent = ApGetInstance(ApWidget(cbinfo));
		PtNotice(parent, NULL, "Xcorr Error", NULL,
				 "Must start int process\nbefore starting Xcorr", "Helvetica09ba",
				 "OK", "Helvetica09ba",
				 Pt_CENTER | Pt_BLOCK_PARENT);

		return( Pt_END );
	}

	/* if the window doesn't have a size or location
	 * compute defaults based on the procSwitch window
	 * get the position of the base window
	 * and set the location of the toolbar window
	 */
	PtGetAbsPosition(ABW_base, &loc.x, &loc.y);
	loc.x -= 6;
	loc.y -= 21;
	PtWidgetExtent(ABW_base, &extent);
	xloc = loc.x + 5;
	yloc = loc.y + 130;
	
	strcpy(name, "../xcorr/src/gcc_ntox86/xcorr");
	memset(rootFile, 0, P_LROOTNAME);
	if(!newproc(name, 'r', 'n')) {
		parent = ApGetInstance(ApWidget(cbinfo));
		PtNotice(parent, NULL, "Xcorr Warning", NULL,
				 "procSwitch can't start Xcorr", "Helvetica09ba",
				 "OK", "Helvetica09ba",
				 Pt_CENTER | Pt_BLOCK_PARENT);
		return( Pt_CONTINUE );
	}

	return( Pt_CONTINUE );
}

int exitRex( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	char *btns[] = { "&Yes", "&No" };
	char *btnFonts[] = { "&Helvetica09ba", "&Helvetica09ba"	};

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	switch(PtAlert(PtWidgetParent(widget), NULL, "Quit Rex", NULL,
				   "Do you want to save session?", "Helvetica09ba",
				   2, (char const **)btns, (char const **)btnFonts, 1, 2, Pt_MODAL)) {
	case 1:
		writeSession();		/* kill all process */
	case 2:
		break;
	}

	endproc(NP);	/* kill all process */

	PtExit(EXIT_SUCCESS);

	return( Pt_CONTINUE );
}

void writeSession()
{
	PROCTBL_P p;
	PROCTBL_P bp;
	PROCTBL_P ep;
	FILE *fp = (FILE *)NULL;
	char *scribePtr;

	if((fp = fopen("./rt/rex_session", "w")) != (FILE *)NULL) {
		bp = (PROCTBL_P)&i_b->ptbl[0];
		ep = (PROCTBL_P)&i_b->ptbl[P_NPROC];

		fprintf(fp, "REX_SESSION BEGIN\n");
		for(p = bp; p < ep; p++) {
			/* skip empty entry */
			if(p->p_id == 0) continue;

			/* nothing to write for procswitch or scribe */
			if(p == &i_b->ptbl[i_b->ph_switch_pi]) continue;
			if(p == &i_b->ptbl[i_b->scrb_pi]) continue;

			fprintf(fp, "PROCESS START\n");
			fprintf(fp, "NAME %s\n", p->p_name);
			if(strlen(p->p_root) > 0) {
				fprintf(fp, "ROOT %s\n", p->p_root);
			}
			fprintf(fp, "STATE %d\n", (p->p_state & P_RUN_ST));
			fprintf(fp, "X %d\n", p->p_x);
			fprintf(fp, "Y %d\n", p->p_y);
			fprintf(fp, "WIDTH %d\n", p->p_w);
			fprintf(fp, "HEIGHT %d\n", p->p_h);
			fprintf(fp, "PROCESS STOP\n");
		}
		fprintf(fp, "REX_SESSION END\n");

		fclose(fp);
	}
}

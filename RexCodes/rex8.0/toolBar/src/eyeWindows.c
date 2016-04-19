/* Y o u r   D e s c r i p t i o n                       */
/*                            AppBuilder Photon Code Lib */
/*                                         Version 1.14C */

/* Standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* Toolkit headers */
#include <Ph.h>
#include <Pt.h>
#include <Ap.h>

/* Local headers */
#include "ph_toolBar.h"
#include "abimport.h"
#include "proto.h"

char *winSources[] = {
	"dir pos",
	"ramp_x",
	"ramp_y",
	"joystick_x",
	"joystick_y",
	"d/a",
	"signal",
	"mem_array"
};
char *winXSources[] = {
	"dir pos",
	"ramp_x",
	"joystick_x",
	"d/a",
	"signal",
	"mem_array"
};
char *winYSources[] = {
	"dir pos",
	"ramp_y",
	"joystick_y",
	"d/a",
	"signal",
	"mem_array"
};
int winNumbs[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
int eyeWinPos[8][2] = {
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 }
};
int eyeWinSiz[8][2] = {
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 }
};
int eyeWinSrcNameToggles[8][4][6];
int eyeWinSrcNumbs[8][4] = {
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 }
};
int eyeWinOvrRides[8][6] = {
	{ 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0 }
};
extern int eyeWinSrcNames[4][6];
extern int actEyeWinSrcNames[4];

int initEyeWinDialog( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtWidget_t *dialog;
	PtWidget_t *toggle;
	PtWidget_t *actName;
	PtArg_t args[3];
	PtListCallback_t *cbdata;
	int winNum;
	int toggleOvrride;
	int i, j;
	char title[16];

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	/* if this function was called from the list widget */
	if(PtWidgetClass(link_instance) == PtList) {
		if(cbinfo->reason_subtype != Pt_LIST_SELECTION_FINAL) return(Pt_CONTINUE);

		cbdata = (PtListCallback_t *)cbinfo->cbdata;
		sscanf(cbdata->item, "Win %d", &winNum); /* get the window number */
		dialog = ApGetInstance(link_instance); /* need to get pointer to the dialog from link_instance */
	}
	/* if this function was called from the dialog setup function */
	else {
		winNum = 0;				/* set window number to 0 */
		PtListSelectPos(ABW_eyeWinList, 1);
		dialog = link_instance;	/* link_instance is pointer to the dialog */
	}

	/* set the state and text string of the eyeWinDialog button */
	if(i_b->wd[winNum].m_wd_menu) {
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, "Dialog Enabled", 0);
		PtSetArg(&args[1], Pt_ARG_ONOFF_STATE, i_b->wd[winNum].m_wd_menu, 0);
		PtSetResources(ABW_eyeWinDialogButton, 2, args);
	}
	else {
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, "Dialog Disabled", 0);
		PtSetArg(&args[1], Pt_ARG_ONOFF_STATE, i_b->wd[winNum].m_wd_menu, 0);
		PtSetResources(ABW_eyeWinDialogButton, 2, args);
	}

	/* set the state of the window on-off toggles */
	if(i_b->wd[winNum].m_wd_cntrl == WD_ON) {
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
	}
	else if(i_b->wd[winNum].m_wd_cntrl == WD_OFF) {
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
	}
	else if(i_b->wd[winNum].m_wd_cntrl == NULLI) {
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
	}
	PtSetResources(ABW_eyeWinOnToggle, 1, &args[0]);
	PtSetResources(ABW_eyeWinOffToggle, 1, &args[1]);
	
	/* set the numeric values of the override widgets */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, eyeWinPos[winNum][0], 0);
	PtSetResources(ABW_eyeWinXpos, 1, &args[0]);

	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, eyeWinPos[winNum][1], 0);
	PtSetResources(ABW_eyeWinYpos, 1, &args[0]);

	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, eyeWinSiz[winNum][0], 0);
	PtSetResources(ABW_eyeWinWidth, 1, &args[0]);

	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, eyeWinSiz[winNum][1], 0);
	PtSetResources(ABW_eyeWinHeight, 1, &args[0]);

	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, eyeWinSrcNumbs[winNum][0], 0);
	PtSetResources(ABW_eyeWinXposSrcNum, 1, &args[0]);

	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, eyeWinSrcNumbs[winNum][1], 0);
	PtSetResources(ABW_eyeWinYposSrcNum, 1, &args[0]);

	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, eyeWinSrcNumbs[winNum][2], 0);
	PtSetResources(ABW_eyeWinXchkSrcNum, 1, &args[0]);

	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, eyeWinSrcNumbs[winNum][3], 0);
	PtSetResources(ABW_eyeWinYchkSrcNum, 1, &args[0]);

	/* set the states of the Menu Override buttons */
	/* window position over ride */
	if(eyeWinOvrRides[winNum][0]) {/* if window position is being overridden */
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, i_b->wd[winNum].m_wd_xdirpos, 0);
		PtSetArg(&args[2], Pt_ARG_NUMERIC_VALUE, i_b->wd[winNum].m_wd_ydirpos, 0);
	}
	else {						/* if window position is under control of spot file */
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, i_b->wd[winNum].wd_xdirpos, 0);
		PtSetArg(&args[2], Pt_ARG_NUMERIC_VALUE, i_b->wd[winNum].wd_ydirpos, 0);
	}
	PtSetResources(ABW_eyeWinPosOvrRide, 1, &args[0]); /* position override toggle button */
	PtSetResources(ABW_eyeWinXposAct, 1, &args[1]);	/* Actual X position widget */
	PtSetResources(ABW_eyeWinYposAct, 1, &args[2]);	/* Actual Y position widget */

	/* window size over ride */
	if(eyeWinOvrRides[winNum][1]) {	/* if window size is being overridden */
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, i_b->wd[winNum].m_wd_xsiz, 0);
		PtSetArg(&args[2], Pt_ARG_NUMERIC_VALUE, i_b->wd[winNum].m_wd_ysiz, 0);
	}
	else {						/* if window size is under control of spot file */
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, i_b->wd[winNum].wd_xsiz, 0);
		PtSetArg(&args[2], Pt_ARG_NUMERIC_VALUE, i_b->wd[winNum].wd_ysiz, 0);
	}
	PtSetResources(ABW_eyeWinSizOvrRide, 1, &args[0]); /* size override toggle button */
	PtSetResources(ABW_eyeWinWidthAct, 1, &args[1]); /* Actual width widget */
	PtSetResources(ABW_eyeWinHeightAct, 1, &args[2]); /* Actual height widget */

	/* window X position source number */
	if(eyeWinOvrRides[winNum][2]) {	/* if X position source number is being overridden */
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, i_b->wd[winNum].m_wd_xpos_src_num, 0);
	}
	else {						/* if X position source number is under control of spot file */
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, i_b->wd[winNum].wd_xpos_src_num, 0);
	}
	PtSetResources(ABW_eyeWinXposSrcNumOvrRide, 1, &args[0]); /* X position source number override toggle button */
	PtSetResources(ABW_eyeWinXposSrcNumAct, 1, &args[1]); /* Actual X position source number widget */

	/* window Y position source number */
	if(eyeWinOvrRides[winNum][3]) {	/* if Y position source number is being overridden */
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, i_b->wd[winNum].m_wd_ypos_src_num, 0);
	}
	else {						/* if Y position source number is under control of spot file */
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, i_b->wd[winNum].wd_ypos_src_num, 0);
	}
	PtSetResources(ABW_eyeWinYposSrcNumOvrRide, 1, &args[0]); /* Y  position source number override toggle button */
	PtSetResources(ABW_eyeWinYposSrcNumAct, 1, &args[1]); /* Actual Y position source number widget */

	/* window X Check source number */
	if(eyeWinOvrRides[winNum][4]) {	/* if X Check source number is being overridden */
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, i_b->wd[winNum].m_wd_xcheck_src_num, 0);
	}
	else {						/* if X Check source number is under control of spot file */
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, i_b->wd[winNum].wd_xcheck_src_num, 0);
	}
	PtSetResources(ABW_eyeWinXchkSrcNumOvrRide, 1, &args[0]); /* X Check source number override toggle button */
	PtSetResources(ABW_eyeWinXchkSrcNumAct, 1, &args[1]); /* Actual X check source number widget */

	/* window Y Check source number */
	if(eyeWinOvrRides[winNum][5]) {	/* if Y check source number is being overridden */
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, i_b->wd[winNum].m_wd_ycheck_src_num, 0);
	}
	else {						/* if Y check source number is under control of spot file */
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, i_b->wd[winNum].wd_ycheck_src_num, 0);
	}
	PtSetResources(ABW_eyeWinYchkSrcNumOvrRide, 1, &args[0]); /* Y check source number override toggle button */
	PtSetResources(ABW_eyeWinYchkSrcNumAct, 1, &args[1]); /* Actual Y check source number widget */

	/* set the states of the source name toggles */
	for(i = 0; i < 4; i++) {
		/* only one of the name toggle buttons can be set so j holds the number of the set toggle */
		toggleOvrride = -1;
		for(j = 0; j < 6; j++) {
			toggle = ApGetWidgetPtr(dialog, eyeWinSrcNames[i][j]);
			if(eyeWinSrcNameToggles[winNum][i][j]) { /* if source name is being overridden */
				PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
				toggleOvrride = j;
			}
			else {				/* if source name is under control of spot file */
				PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
			}
			PtSetResources(toggle, 1, &args[0]);
		}

		/* if a toggle is set, set the actual source name widget based on the toggle */
		if(toggleOvrride > -1) {
			if(i / 2) {			/* X source or check name */
				PtSetArg(&args[1], Pt_ARG_TEXT_STRING, winXSources[toggleOvrride], 0);
			}
			else {				/* Y source or check name */
				PtSetArg(&args[1], Pt_ARG_TEXT_STRING, winYSources[toggleOvrride], 0);
			}
		}

		/* if no toggle is set, set the actual source name widget based on spot file */
		else {
			switch(i) {
			case 0:				/* X position source name */
				switch(i_b->wd[winNum].wd_xpos_source) { /* value set by spot file */
				case 0:			/* dir pos */
					PtSetArg(&args[1], Pt_ARG_TEXT_STRING, winXSources[0], 0);
					break;
				case 1:			/* ramp_x */
					PtSetArg(&args[1], Pt_ARG_TEXT_STRING, winXSources[1], 0);
					break;
				case 3:			/* joystick_x */
					PtSetArg(&args[1], Pt_ARG_TEXT_STRING, winXSources[2], 0);
					break;
				case 5:			/* d/a */
					PtSetArg(&args[1], Pt_ARG_TEXT_STRING, winXSources[3], 0);
					break;
				case 6:			/* signal */
					PtSetArg(&args[1], Pt_ARG_TEXT_STRING, winXSources[4], 0);
					break;
				case 7:			/* mem_array */
					PtSetArg(&args[1], Pt_ARG_TEXT_STRING, winXSources[5], 0);
					break;
				default:		/* unknown */
					PtSetArg(&args[1], Pt_ARG_TEXT_STRING, "", 0);
					break;
				}
				break;
			case 1:				/* Y position source name */
				switch(i_b->wd[winNum].wd_ypos_source) { /* value set by spot file */
				case 0:			/* dir pos */
					PtSetArg(&args[1], Pt_ARG_TEXT_STRING, winYSources[0], 0);
					break;
				case 2:			/* ramp_y */
					PtSetArg(&args[1], Pt_ARG_TEXT_STRING, winYSources[1], 0);
					break;
				case 4:			/* joystick_y */
					PtSetArg(&args[1], Pt_ARG_TEXT_STRING, winYSources[2], 0);
					break;
				case 5:			/* d/a */
					PtSetArg(&args[1], Pt_ARG_TEXT_STRING, winYSources[3], 0);
					break;
				case 6:			/* signal */
					PtSetArg(&args[1], Pt_ARG_TEXT_STRING, winYSources[4], 0);
					break;
				case 7:			/* mem_array */
					PtSetArg(&args[1], Pt_ARG_TEXT_STRING, winYSources[5], 0);
					break;
				default:		/* unknown */
					PtSetArg(&args[1], Pt_ARG_TEXT_STRING, "", 0);
					break;
				}
				break;
			case 2:				/* X check source name */
				switch(i_b->wd[winNum].wd_xcheck_source) { /* value set by spot file */
				case 0:			/* dir pos */
					PtSetArg(&args[1], Pt_ARG_TEXT_STRING, winXSources[0], 0);
					break;
				case 1:			/* ramp_x */
					PtSetArg(&args[1], Pt_ARG_TEXT_STRING, winXSources[1], 0);
					break;
				case 3:			/* joystick_x */
					PtSetArg(&args[1], Pt_ARG_TEXT_STRING, winXSources[2], 0);
					break;
				case 5:			/* d/a */
					PtSetArg(&args[1], Pt_ARG_TEXT_STRING, winXSources[3], 0);
					break;
				case 6:			/* signal */
					PtSetArg(&args[1], Pt_ARG_TEXT_STRING, winXSources[4], 0);
					break;
				case 7:			/* mem_array */
					PtSetArg(&args[1], Pt_ARG_TEXT_STRING, winXSources[5], 0);
					break;
				default:		/* unknown */
					PtSetArg(&args[1], Pt_ARG_TEXT_STRING, "", 0);
					break;
				}
				break;
			case 3:				/* Y check source name */
				switch(i_b->wd[winNum].wd_ycheck_source) { /* value set by spot file */
				case 0:			/* dir pos */
					PtSetArg(&args[1], Pt_ARG_TEXT_STRING, winYSources[0], 0);
					break;
				case 2:			/* ramp_y */
					PtSetArg(&args[1], Pt_ARG_TEXT_STRING, winYSources[1], 0);
					break;
				case 4:			/* joystick_y */
					PtSetArg(&args[1], Pt_ARG_TEXT_STRING, winYSources[2], 0);
					break;
				case 5:			/* d/a */
					PtSetArg(&args[1], Pt_ARG_TEXT_STRING, winYSources[3], 0);
					break;
				case 6:			/* signal */
					PtSetArg(&args[1], Pt_ARG_TEXT_STRING, winYSources[4], 0);
					break;
				case 7:			/* mem_array */
					PtSetArg(&args[1], Pt_ARG_TEXT_STRING, winYSources[5], 0);
					break;
				default:		/* unknown */
					PtSetArg(&args[1], Pt_ARG_TEXT_STRING, "", 0);
					break;
				}
				break;
			}
		}
		actName = ApGetWidgetPtr(dialog, actEyeWinSrcNames[i]);	/* pointer to the actual name widget */
		PtSetResources(actName, 1, &args[1]);

		/* if a source or check name toggle is set, set the number override toggle */
		if(toggleOvrride > -1) {
			switch(i) {
			case 0:				/* X position source number */
				if((toggleOvrride == 0) || (toggleOvrride == 2)) {
					PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_GHOST);
					PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_FALSE, Pt_SELECTABLE);
					PtSetResources(ABW_eyeWinXposSrcNumOvrRide, 2, args);
				}
				else {
					PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_GHOST);
					PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_TRUE, Pt_SELECTABLE);
					PtSetResources(ABW_eyeWinXposSrcNumOvrRide, 2, args);
				}
				break;
			case 1:				/* Y position source number */
				if((toggleOvrride == 0) || (toggleOvrride == 2)) {
					PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_GHOST);
					PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_FALSE, Pt_SELECTABLE);
					PtSetResources(ABW_eyeWinYposSrcNumOvrRide, 2, args);
				}
				else {
					PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_GHOST);
					PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_TRUE, Pt_SELECTABLE);
					PtSetResources(ABW_eyeWinYposSrcNumOvrRide, 2, args);
				}
				break;
			case 2:				/* X check source number */
				if((toggleOvrride == 0) || (toggleOvrride == 2)) {
					PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_GHOST);
					PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_FALSE, Pt_SELECTABLE);
					PtSetResources(ABW_eyeWinXchkSrcNumOvrRide, 2, args);
				}
				else {
					PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_GHOST);
					PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_TRUE, Pt_SELECTABLE);
					PtSetResources(ABW_eyeWinXchkSrcNumOvrRide, 2, args);
				}
				break;
			case 3:				/* Y check source number */
				if((toggleOvrride == 0) || (toggleOvrride == 2)) {
					PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_GHOST);
					PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_FALSE, Pt_SELECTABLE);
					PtSetResources(ABW_eyeWinYchkSrcNumOvrRide, 2, args);
				}
				else {
					PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_GHOST);
					PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_TRUE, Pt_SELECTABLE);
					PtSetResources(ABW_eyeWinYchkSrcNumOvrRide, 2, args);
				}
				break;
			}
		}

		/* if no name toggle is set, enable the number menu override toggles */
		else {
			switch(i) {
			case 0:
				PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_GHOST);
				PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_TRUE, Pt_SELECTABLE);
				PtSetResources(ABW_eyeWinXposSrcNumOvrRide, 2, args);
				break;
			case 1:
				PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_GHOST);
				PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_TRUE, Pt_SELECTABLE);
				PtSetResources(ABW_eyeWinYposSrcNumOvrRide, 2, args);
				break;
			case 2:
				PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_GHOST);
				PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_TRUE, Pt_SELECTABLE);
				PtSetResources(ABW_eyeWinXchkSrcNumOvrRide, 2, args);
				break;
			case 3:
				PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_GHOST);
				PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_TRUE, Pt_SELECTABLE);
				PtSetResources(ABW_eyeWinYchkSrcNumOvrRide, 2, args);
				break;
			}
		}
	}

	/* set the module's title widget */
	sprintf(title, "Eye Window %d", winNum);
	PtSetArg(&args[0], Pt_ARG_TEXT_STRING, title, 0);
	PtSetResources(ABW_eyeWinLabel, 1, args);

	/* set the window number in all of this module's widgets */
	PtSetArg(&args[0], Pt_ARG_USER_DATA, &winNum, sizeof(int));
	PtSetResources(ABW_eyeWinDialogButton, 1, args);
	PtSetResources(ABW_eyeWinWindowOnOffGroup, 1, args);
	PtSetResources(ABW_eyeWinPosGroup, 1, args);
	PtSetResources(ABW_eyeWinSizGroup, 1, args);

	PtSetResources(ABW_eyeWinXposGroup, 1, args);
	PtSetResources(ABW_eyeWinXposNumGroup, 1, args);

	PtSetResources(ABW_eyeWinYposGroup, 1, args);
	PtSetResources(ABW_eyeWinYposNumGroup, 1, args);

	PtSetResources(ABW_eyeWinXchkGroup, 1, args);
	PtSetResources(ABW_eyeWinXchkNumGroup, 1, args);

	PtSetResources(ABW_eyeWinYchkGroup, 1, args);
	PtSetResources(ABW_eyeWinYchkNumGroup, 1, args);

	eyeWinDialogItemEnable(i_b->wd[winNum].m_wd_menu);

	return( Pt_CONTINUE );
}

int eyeWinDialogSet( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[2];
	int value;
	int winNum;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the value of the widget */
	PtSetArg(&args[0], Pt_ARG_ONOFF_STATE, 0, 0);
	PtSetArg(&args[1], Pt_ARG_USER_DATA, 0, 0);
	PtGetResources(widget, 2, args);
	value = (int)args[0].value;
	winNum = *(int *)args[1].value;

	if(value) {
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, "Dialog Enabled", 0);
		i_b->wd[winNum].m_wd_menu = 1;
	}
	else {
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, "Dialog Disabled", 0);
		i_b->wd[winNum].m_wd_menu = 0;
	}
	PtSetResources(widget, 1, args);

	wd_cntrl(winNum, NULLI);
	wd_src_pos(winNum, NULLI, NULLI, NULLI, NULLI);
	wd_src_check(winNum, NULLI, NULLI, NULLI, NULLI);
	wd_siz(winNum, NULLI, NULLI);
	wd_pos(winNum, NULLI, NULLI);

	eyeWinDialogItemEnable(i_b->wd[winNum].m_wd_menu);	/* enable or disable dialog widgets */

	return( Pt_CONTINUE );
}

void eyeWinDialogItemEnable(int eyeWinDialogEnabled)
{
	PtArg_t args[3];

	/* enable dialog items if the dialog is enabled */
	if(eyeWinDialogEnabled) {
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_GHOST);
		PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_TRUE, Pt_SELECTABLE);
	}
	/* disable dialog items if dialog is disabled */
	else {
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_GHOST);
		PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_FALSE, Pt_SELECTABLE);
	}

	PtSetResources(ABW_eyeWinOnToggle, 2, args);
	PtSetResources(ABW_eyeWinOffToggle, 2, args);

	PtSetResources(ABW_eyeWinPosOvrRide, 2, args);
	PtSetResources(ABW_eyeWinSizOvrRide, 2, args);
	PtSetResources(ABW_eyeWinXposSrcNumOvrRide, 2, args);
	PtSetResources(ABW_eyeWinYposSrcNumOvrRide, 2, args);
	PtSetResources(ABW_eyeWinXchkSrcNumOvrRide, 2, args);
	PtSetResources(ABW_eyeWinYchkSrcNumOvrRide, 2, args);

	PtSetResources(ABW_eyeWinXposDPtoggle, 2, args);
	PtSetResources(ABW_eyeWinXposDAtoggle, 2, args);
	PtSetResources(ABW_eyeWinXposRXtoggle, 2, args);
	PtSetResources(ABW_eyeWinXposJXtoggle, 2, args);
	PtSetResources(ABW_eyeWinXposSigtoggle, 2, args);
	PtSetResources(ABW_eyeWinXposMemtoggle, 2, args);

	PtSetResources(ABW_eyeWinYposDPtoggle, 2, args);
	PtSetResources(ABW_eyeWinYposDAtoggle, 2, args);
	PtSetResources(ABW_eyeWinYposRYtoggle, 2, args);
	PtSetResources(ABW_eyeWinYposJYtoggle, 2, args);
	PtSetResources(ABW_eyeWinYposSigtoggle, 2, args);
	PtSetResources(ABW_eyeWinYposMemtoggle, 2, args);

	PtSetResources(ABW_eyeWinXchkDPtoggle, 2, args);
	PtSetResources(ABW_eyeWinXchkDAtoggle, 2, args);
	PtSetResources(ABW_eyeWinXchkRXtoggle, 2, args);
	PtSetResources(ABW_eyeWinXchkJXtoggle, 2, args);
	PtSetResources(ABW_eyeWinXchkSigtoggle, 2, args);
	PtSetResources(ABW_eyeWinXchkMemtoggle, 2, args);

	PtSetResources(ABW_eyeWinYchkDPtoggle, 2, args);
	PtSetResources(ABW_eyeWinYchkDAtoggle, 2, args);
	PtSetResources(ABW_eyeWinYchkRYtoggle, 2, args);
	PtSetResources(ABW_eyeWinYchkJYtoggle, 2, args);
	PtSetResources(ABW_eyeWinYchkSigtoggle, 2, args);
	PtSetResources(ABW_eyeWinYchkMemtoggle, 2, args);
}

int eyeWinSwitch( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;
	PtWidget_t *parent;
	int winNum;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the widget's group */
	parent = PtWidgetParent(widget);

	/* get the window number from the group */
	PtSetArg(&arg, Pt_ARG_USER_DATA, 0, 0);
	PtGetResources(parent, 1, &arg);
	winNum = *(int *)arg.value;

	/* if this function called by a button release,
	 * clear the m_wd_cntrl flag
	 */
	if(cbinfo->reason == Pt_CB_DISARM) {
		i_b->wd[winNum].m_wd_cntrl = NULLI;
	}
	else {						/* this function called by a button press */
		/* figure out which button was pressed */
		if(ApName(widget) == ABN_eyeWinOnToggle) {
			i_b->wd[winNum].m_wd_cntrl = WD_ON;
		}
		else if(ApName(widget) == ABN_eyeWinOffToggle) {
			i_b->wd[winNum].m_wd_cntrl = WD_OFF;
		}
	}
	wd_cntrl(winNum, NULLI);

	return( Pt_CONTINUE );
}

int eyeWinMenuOverRide( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;
	PtWidget_t *parent;
	int winNum;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the widget's group */
	parent = PtWidgetParent(widget);

	/* get the window number from the widget */
	PtSetArg(&arg, Pt_ARG_USER_DATA, 0, 0);
	PtGetResources(parent, 1, &arg);
	winNum = *(int *)arg.value;
	
	/* determine which of the override buttons initiated this callback */
	/* Position override */
	if(ApName(widget) == ABN_eyeWinPosOvrRide) {
		if(cbinfo->reason == Pt_CB_ARM) {
			eyeWinOvrRides[winNum][0] = 1;
			i_b->wd[winNum].m_wd_xdirpos = eyeWinPos[winNum][0];
			i_b->wd[winNum].m_wd_ydirpos = eyeWinPos[winNum][1];

			PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, i_b->wd[winNum].m_wd_xdirpos, 0);
			PtSetResources(ABW_eyeWinXposAct, 1, &arg);

			PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, i_b->wd[winNum].m_wd_ydirpos, 0);
			PtSetResources(ABW_eyeWinYposAct, 1, &arg);
		}
		else if(cbinfo->reason == Pt_CB_DISARM) {
			eyeWinOvrRides[winNum][0] = 0;
			i_b->wd[winNum].m_wd_xdirpos = NULLI;
			i_b->wd[winNum].m_wd_ydirpos = NULLI;
		}
		wd_pos(winNum, NULLI, NULLI);
	}
	/* Size override */
	else if(ApName(widget) == ABN_eyeWinSizOvrRide) {
		if(cbinfo->reason == Pt_CB_ARM) {
			eyeWinOvrRides[winNum][1] = 1;
			i_b->wd[winNum].m_wd_xsiz = eyeWinSiz[winNum][0];
			i_b->wd[winNum].m_wd_ysiz = eyeWinSiz[winNum][1];

			PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, i_b->wd[winNum].m_wd_xsiz, 0);
			PtSetResources(ABW_eyeWinWidthAct, 1, &arg);

			PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, i_b->wd[winNum].m_wd_ysiz, 0);
			PtSetResources(ABW_eyeWinHeightAct, 1, &arg);
		}
		else if(cbinfo->reason == Pt_CB_DISARM) {
			eyeWinOvrRides[winNum][1] = 0;
			i_b->wd[winNum].m_wd_xsiz = NULLI;
			i_b->wd[winNum].m_wd_ysiz = NULLI;
		}
		wd_siz(winNum, NULLI, NULLI);
	}
	/* X position source number override */
	else if(ApName(widget) == ABN_eyeWinXposSrcNumOvrRide) {
		if(cbinfo->reason == Pt_CB_ARM) {
			eyeWinOvrRides[winNum][2] = 1;
			i_b->wd[winNum].m_wd_xpos_src_num = eyeWinSrcNumbs[winNum][0];

			PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, i_b->wd[winNum].m_wd_xpos_src_num, 0);
			PtSetResources(ABW_eyeWinXposSrcNumAct, 1, &arg);
		}
		else if(cbinfo->reason == Pt_CB_DISARM) {
			eyeWinOvrRides[winNum][2] = 0;
			i_b->wd[winNum].m_wd_xpos_src_num = NULLI;
		}
		wd_src_pos(winNum, NULLI, NULLI, NULLI, NULLI);
	}
	/* Y position source number override */
	else if(ApName(widget) == ABN_eyeWinYposSrcNumOvrRide) {
		if(cbinfo->reason == Pt_CB_ARM) {
			eyeWinOvrRides[winNum][3] = 1;
			i_b->wd[winNum].m_wd_ypos_src_num = eyeWinSrcNumbs[winNum][1];

			PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, i_b->wd[winNum].m_wd_ypos_src_num, 0);
			PtSetResources(ABW_eyeWinYposSrcNumAct, 1, &arg);
		}
		else if(cbinfo->reason == Pt_CB_DISARM) {
			eyeWinOvrRides[winNum][3] = 0;
			i_b->wd[winNum].m_wd_ypos_src_num = NULLI;
		}
		wd_src_pos(winNum, NULLI, NULLI, NULLI, NULLI);
	}
	/* X check source number override */
	else if(ApName(widget) == ABN_eyeWinXchkSrcNumOvrRide) {
		if(cbinfo->reason == Pt_CB_ARM) {
			eyeWinOvrRides[winNum][4] = 1;
			i_b->wd[winNum].m_wd_xcheck_src_num = eyeWinSrcNumbs[winNum][2];

			PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, i_b->wd[winNum].m_wd_xcheck_src_num, 0);
			PtSetResources(ABW_eyeWinXchkSrcNumAct, 1, &arg);
		}
		else if(cbinfo->reason == Pt_CB_DISARM) {
			eyeWinOvrRides[winNum][4] = 0;
			i_b->wd[winNum].m_wd_xcheck_src_num = NULLI;
		}
		wd_src_check(winNum, NULLI, NULLI, NULLI, NULLI);
	}
	/* Y check source number override */
	else if(ApName(widget) == ABN_eyeWinYchkSrcNumOvrRide) {
		if(cbinfo->reason == Pt_CB_ARM) {
			eyeWinOvrRides[winNum][5] = 1;
			i_b->wd[winNum].m_wd_ycheck_src_num = eyeWinSrcNumbs[winNum][3];

			PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, i_b->wd[winNum].m_wd_ycheck_src_num, 0);
			PtSetResources(ABW_eyeWinYchkSrcNumAct, 1, &arg);
		}
		else if(cbinfo->reason == Pt_CB_DISARM) {
			eyeWinOvrRides[winNum][5] = 0;
			i_b->wd[winNum].m_wd_ycheck_src_num = NULLI;
		}
		wd_src_check(winNum, NULLI, NULLI, NULLI, NULLI);
	}
	return( Pt_CONTINUE );
}

int eyeWinDirPos( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;
	PtWidget_t *parent;
	long flags;
	int winNum;
	int value;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the widget's group */
	parent = PtWidgetParent(widget);

	/* get the window number from the group */
	PtSetArg(&arg, Pt_ARG_USER_DATA, 0, 0);
	PtGetResources(parent, 1, &arg);
	winNum = *(int *)arg.value;

	/* get the value from the widget */
	PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, 0, 0);
	PtGetResources(widget, 1, &arg);
	value = (int)arg.value;

	/* figure out which widget initiated this call */
	if(ApName(widget) == ABN_eyeWinXpos) {
		eyeWinPos[winNum][0] = value;
	}

	else if(ApName(widget) == ABN_eyeWinYpos) {
		eyeWinPos[winNum][1] = value;
	}

	/* if the window position override toggle is set
	 * set the menu variable
	 */
	flags = PtWidgetFlags(ABW_eyeWinPosOvrRide);
	if(flags & Pt_SET) {
		i_b->wd[winNum].m_wd_xdirpos = eyeWinPos[winNum][0];
		i_b->wd[winNum].m_wd_ydirpos = eyeWinPos[winNum][1];
		wd_pos(winNum, NULLI, NULLI);
	}

	return( Pt_CONTINUE );
}

int eyeWinDirSiz( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;
	PtWidget_t *parent;
	long flags;
	int winNum;
	int value;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the widget's group */
	parent = PtWidgetParent(widget);

	/* get the window number from the group */
	PtSetArg(&arg, Pt_ARG_USER_DATA, 0, 0);
	PtGetResources(parent, 1, &arg);
	winNum = *(int *)arg.value;

	/* get the value from the widget */
	PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, 0, 0);
	PtGetResources(widget, 1, &arg);
	value = (int)arg.value;

	/* figure out which widget initiated this call */
	if(ApName(widget) == ABN_eyeWinWidth) {
		eyeWinSiz[winNum][0] = value;
	}

	else if(ApName(widget) == ABN_eyeWinHeight) {
		eyeWinSiz[winNum][1] = value;
	}

	/* if the window position override toggle is set
	 * set the menu variable
	 */
	flags = PtWidgetFlags(ABW_eyeWinSizOvrRide);
	if(flags & Pt_SET) {
		i_b->wd[winNum].m_wd_xsiz = eyeWinSiz[winNum][0];
		i_b->wd[winNum].m_wd_ysiz = eyeWinSiz[winNum][1];
		wd_siz(winNum, NULLI, NULLI);
	}

	return( Pt_CONTINUE );
}

int eyeWinSourceName( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[3];
	PtWidget_t *parent;
	int winNum;
	int source, type;
	int i, j;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the widget's group */
	parent = PtWidgetParent(widget);

	/* get the window number from the widget */
	PtSetArg(&args[0], Pt_ARG_USER_DATA, 0, 0);
	PtGetResources(parent, 1, &args[0]);
	winNum = *(int *)args[0].value;

	for(i = 0; i < 4; i++) {
		for(j = 0; j < 6; j++) {
			if(ApName(widget) == eyeWinSrcNames[i][j]) {
				type = i;		/* which type of source */
				source = j;		/* toggle button indicating source name */
			}
		}
	}

	if(cbinfo->reason == Pt_CB_ARM) {
		eyeWinSrcNameToggles[winNum][type][source] = 1;

		/* action depending on which panel in which the button was located */
		switch(type) {
		case 0:					/* X position source name */
			PtSetArg(&args[0], Pt_ARG_TEXT_STRING, winXSources[source], 0);
			PtSetResources(ABW_eyeWinXposNamAct, 1, &args[0]);

			/* set the source name in the window structure */
			for(i = 0; i < 8; i++) {
				if(!strcmp(winXSources[source], winSources[i])) {
					i_b->wd[winNum].m_wd_xpos_source = i;
				}
			}

			if((source == 0) || (source == 2)) {
				PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_GHOST);
				PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_FALSE, Pt_SELECTABLE);
				PtSetResources(ABW_eyeWinXposSrcNumOvrRide, 2, args);
			}
			else {
				PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_GHOST);
				PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_TRUE, Pt_SELECTABLE);
				PtSetResources(ABW_eyeWinXposSrcNumOvrRide, 2, args);
			}

			wd_src_pos(winNum, NULLI, NULLI, NULLI, NULLI);
			break;
		case 1:					/* Y position source name */
			PtSetArg(&args[0], Pt_ARG_TEXT_STRING, winYSources[source], 0);
			PtSetResources(ABW_eyeWinYposNamAct, 1, &args[0]);

			/* set the source name in the window structure */
			for(i = 0; i < 8; i++) {
				if(!strcmp(winYSources[source], winSources[i])) {
					i_b->wd[winNum].m_wd_ypos_source = i;
				}
			}

			if((source == 0) || (source == 2)) {
				PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_GHOST);
				PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_FALSE, Pt_SELECTABLE);
				PtSetResources(ABW_eyeWinYposSrcNumOvrRide, 2, args);
			}
			else {
				PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_GHOST);
				PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_TRUE, Pt_SELECTABLE);
				PtSetResources(ABW_eyeWinYposSrcNumOvrRide, 2, args);
			}

			wd_src_pos(winNum, NULLI, NULLI, NULLI, NULLI);
			break;
		case 2:					/* X check source name */
			PtSetArg(&args[0], Pt_ARG_TEXT_STRING, winXSources[source], 0);
			PtSetResources(ABW_eyeWinXchkNamAct, 1, &args[0]);

			/* set the source name in the window structure */
			for(i = 0; i < 8; i++) {
				if(!strcmp(winXSources[source], winSources[i])) {
					i_b->wd[winNum].m_wd_xcheck_source = i;
				}
			}

			if((source == 0) || (source == 2)) {
				PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_GHOST);
				PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_FALSE, Pt_SELECTABLE);
				PtSetResources(ABW_eyeWinXchkSrcNumOvrRide, 2, args);
			}
			else {
				PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_GHOST);
				PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_TRUE, Pt_SELECTABLE);
				PtSetResources(ABW_eyeWinXchkSrcNumOvrRide, 2, args);
			}

			wd_src_check(winNum, NULLI, NULLI, NULLI, NULLI);
			break;
		case 3:					/* Y check source name */
			PtSetArg(&args[0], Pt_ARG_TEXT_STRING, winYSources[source], 0);
			PtSetResources(ABW_eyeWinYchkNamAct, 1, &args[0]);

			/* set the source name in the window structure */
			for(i = 0; i < 8; i++) {
				if(!strcmp(winYSources[source], winSources[i])) {
					i_b->wd[winNum].m_wd_ycheck_source = i;
				}
			}

			if((source == 0) || (source == 2)) {
				PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_GHOST);
				PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_FALSE, Pt_SELECTABLE);
				PtSetResources(ABW_eyeWinYchkSrcNumOvrRide, 2, args);
			}
			else {
				PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_GHOST);
				PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_TRUE, Pt_SELECTABLE);
				PtSetResources(ABW_eyeWinYchkSrcNumOvrRide, 2, args);
			}

			wd_src_check(winNum, NULLI, NULLI, NULLI, NULLI);
			break;
		}
	}
	else if(cbinfo->reason == Pt_CB_DISARM) {
		eyeWinSrcNameToggles[winNum][type][source] = 0;

		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, "spot", 0);
		switch(type) {
		case 0:
			i_b->wd[winNum].m_wd_xpos_source = NULLI;

			if((source == 0) || (source == 2)) {
				PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_GHOST);
				PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_TRUE, Pt_SELECTABLE);
				PtSetResources(ABW_eyeWinXposSrcNumOvrRide, 2, args);
			}

			wd_src_pos(winNum, NULLI, NULLI, NULLI, NULLI);
			break;
		case 1:
			i_b->wd[winNum].m_wd_ypos_source = NULLI;

			if((source == 0) || (source == 2)) {
				PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_GHOST);
				PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_TRUE, Pt_SELECTABLE);
				PtSetResources(ABW_eyeWinYposSrcNumOvrRide, 2, args);
			}

			wd_src_pos(winNum, NULLI, NULLI, NULLI, NULLI);
			break;
		case 2:
			i_b->wd[winNum].m_wd_xcheck_source = NULLI;

			if((source == 0) || (source == 2)) {
				PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_GHOST);
				PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_TRUE, Pt_SELECTABLE);
				PtSetResources(ABW_eyeWinXchkSrcNumOvrRide, 2, args);
			}

			wd_src_check(winNum, NULLI, NULLI, NULLI, NULLI);
			break;
		case 3:
			i_b->wd[winNum].m_wd_ycheck_source = NULLI;

			if((source == 0) || (source == 2)) {
				PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_GHOST);
				PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_TRUE, Pt_SELECTABLE);
				PtSetResources(ABW_eyeWinYchkSrcNumOvrRide, 2, args);
			}

			wd_src_check(winNum, NULLI, NULLI, NULLI, NULLI);
			break;
		}
	}
		
	return( Pt_CONTINUE );
}

int eyeWinSourceNum( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;
	PtWidget_t *parent;
	int winNum;
	int value;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the widget's group */
	parent = PtWidgetParent(widget);

	/* get the window number from the group */
	PtSetArg(&arg, Pt_ARG_USER_DATA, 0, 0);
	PtGetResources(parent, 1, &arg);
	winNum = *(int *)arg.value;

	/* get the value from the widget */
	PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, 0, 0);
	PtGetResources(widget, 1, &arg);
	value = (int)arg.value;

	/* figure out which widget initiated this call */
	if(ApName(widget) == ABN_eyeWinXposSrcNum) {
		eyeWinSrcNumbs[winNum][0] = value;
	}
	else if(ApName(widget) == ABN_eyeWinYposSrcNum) {
		eyeWinSrcNumbs[winNum][1] = value;
	}
	else if(ApName(widget) == ABN_eyeWinXchkSrcNum) {
		eyeWinSrcNumbs[winNum][2] = value;
	}
	else if(ApName(widget) == ABN_eyeWinYchkSrcNum) {
		eyeWinSrcNumbs[winNum][3] = value;
	}
	return( Pt_CONTINUE );
}

int initEyeWinSum( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	ApDBase_t *eyeWinSumdbase;
	PtArg_t arg;
	int num;
	int i;
	int y;
	char label[4];

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	/* open the eye window summary data base */
	eyeWinSumdbase = ApOpenDBase(ABM_eyeWinSumPictures);

	/* set the parent for the widgets */
	PtSetParentWidget(ABW_eyeWinSumScrollArea);

	/* create 8 sets of widgets for the eye window summary */
	y = 0;
	for(i = 0; i < WD_MAXNUM; i++) {
		if(!i_b->wd[i].wd_cntrl) continue; /* don't list windows that aren't enabled */

		/* create the label widget for this signal */
		sprintf(label, "%d", i);
		PtSetArg(&arg, Pt_ARG_TEXT_STRING, label, 0);
		ApCreateWidget(eyeWinSumdbase, "eyeWinSumLabel", 10, y, 2, &arg);

		/* create the X position widget for this signal */
		PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, i_b->wd[i].wd_xdirpos, 0);
		ApCreateWidget(eyeWinSumdbase, "eyeWinSumXpos", 41, y, 2, &arg);

		/* create the Y position widget for this signal */
		PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, i_b->wd[i].wd_ydirpos, 0);
		ApCreateWidget(eyeWinSumdbase, "eyeWinSumYpos", 102, y, 2, &arg);

		/* create the width widget for this signal */
		PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, i_b->wd[i].wd_xsiz, 0);
		ApCreateWidget(eyeWinSumdbase, "eyeWinSumWidth", 163, y, 2, &arg);

		/* create the height widget for this signal */
		PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, i_b->wd[i].wd_ysiz, 0);
		ApCreateWidget(eyeWinSumdbase, "eyeWinSumHeight", 224, y, 2, &arg);

		/* create the X position name widget for this signal */
		num = i_b->wd[i].wd_xpos_source;
		PtSetArg(&arg, Pt_ARG_TEXT_STRING, winSources[num], 0);
		ApCreateWidget(eyeWinSumdbase, "eyeWinSumXposNam", 284, y, 2, &arg);

		/* create the X position source number widget for this signal */
		PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, i_b->wd[i].wd_xpos_src_num, 0);
		ApCreateWidget(eyeWinSumdbase, "eyeWinSumXposNum", 358, y, 2, &arg);

		/* create the Y position name widget for this signal */
		num = i_b->wd[i].wd_ypos_source;
		PtSetArg(&arg, Pt_ARG_TEXT_STRING, winSources[num], 0);
		ApCreateWidget(eyeWinSumdbase, "eyeWinSumYposNam", 420, y, 2, &arg);

		/* create the Y position source number widget for this signal */
		PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, i_b->wd[i].wd_ypos_src_num, 0);
		ApCreateWidget(eyeWinSumdbase, "eyeWinSumYposNum", 498, y, 2, &arg);

		/* create the X check name widget for this signal */
		num = i_b->wd[i].wd_xcheck_source;
		PtSetArg(&arg, Pt_ARG_TEXT_STRING, winSources[num], 0);
		ApCreateWidget(eyeWinSumdbase, "eyeWinSumXchkNam", 560, y, 2, &arg);

		/* create the X check source number widget for this signal */
		PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, i_b->wd[i].wd_xcheck_src_num, 0);
		ApCreateWidget(eyeWinSumdbase, "eyeWinSumXchkNum", 635, y, 2, &arg);

		/* create the Y check name widget for this signal */
		num = i_b->wd[i].wd_ycheck_source;
		PtSetArg(&arg, Pt_ARG_TEXT_STRING, winSources[num], 0);
		ApCreateWidget(eyeWinSumdbase, "eyeWinSumYchkNam", 700, y, 2, &arg);

		/* create the Y check source number widget for this signal */
		PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, i_b->wd[i].wd_ycheck_src_num, 0);
		ApCreateWidget(eyeWinSumdbase, "eyeWinSumYchkNum", 775, y, 2, &arg);

		/* increment the y value for widget placement */
		y += 30;
	}

	/* set the maximum range of the scroll widget */
	PtSetArg(&arg, Pt_ARG_SCROLL_AREA_MAX_Y, y, 0);
	PtSetResources(ABW_eyeWinSumScrollArea, 1, &arg);

	/* close the widget data base */
	ApCloseDBase(eyeWinSumdbase);

	return( Pt_CONTINUE );
}

int eyeWndChkRate( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, 0, 0);
	PtGetResources(widget, 1, &arg);

	i_b->t_wrate = (int)arg.value;

	return( Pt_CONTINUE );
}

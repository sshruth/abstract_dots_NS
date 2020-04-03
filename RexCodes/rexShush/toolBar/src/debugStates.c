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

PtWidget_t *debugWindow = (PtWidget_t *)NULL;
PtWidget_t *debugDialog = (PtWidget_t *)NULL;

typedef struct {
	PtWidget_t *stateButton;
	int stateIndex;
} STATE_INFO;

STATE_INFO chainStates[MAXCHAIN][64] = { 0 };

extern PgColor_t stateButtonColors[3];

int switchDebug( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[2];
	int *value;

	extern int errprt;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the value of the widget */
	PtSetArg(&args[0], Pt_ARG_ONOFF_STATE, &value, 0);
	PtGetResources(widget, 1, &args[0]);

	/* change the text of the widget to reflect current state */
	if(*value) {
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, "Debug\nOn", 0);
		PtSetArg(&args[1], Pt_ARG_FILL_COLOR, Pg_GREEN, 0);
		PtSetResources(widget, 2, args);
		errprt = 1;

		/* create a stateDebugWindow if switch is on */
		if(!debugWindow) {
			debugWindow = ApCreateModule(ABM_stateDebugWindow, widget, cbinfo);
		}

		/* enable the debugging timer */
		PtSetArg(&args[0], Pt_ARG_TIMER_INITIAL, 15, 0);
		PtSetResources(ABW_stateDebugTimer, 1, &args[0]);
	}
	else {
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, "Debug\nOff", 0);
		PtSetArg(&args[1], Pt_ARG_FILL_COLOR, Pg_RED, 0);
		PtSetResources(widget, 2, args);
		errprt = 0;
		
		/* destroy the stateDebugWindow if it is displayed */
		if(debugWindow) {
			PtDestroyWidget(debugWindow);
			debugWindow = (PtWidget_t *)NULL;
		}
		if(debugDialog) {
			PtDestroyWidget(debugDialog);
			debugDialog = (PtWidget_t *)NULL;
		}

		/* disable the debugging timer */
		PtSetArg(&args[0], Pt_ARG_TIMER_INITIAL, 0, 0);
		PtSetResources(ABW_stateDebugTimer, 1, &args[0]);
	}

	return( Pt_CONTINUE );
}

int initStateDebugWindow( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtWidget_t *scroll;
	PtWidget_t *allGroup;
	PtWidget_t *buttonGroup;
	PhPoint_t sPos;
	ApDBase_t *stateDebugdbase;
	PtArg_t args[6];
	int nArgs;
	int chainIndx;
	int chainStateIndx;
	int i, j;
	char buttonLabel[128];
	char timeStamp[32];

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	/* get a pointer to the scroll container */
	scroll = ApGetWidgetPtr(link_instance, ABN_debugScrollContainer);

	/* create a master group widget */
	nArgs = 0;
	PtSetArg(&args[nArgs], Pt_ARG_GROUP_ORIENTATION, Pt_GROUP_HORIZONTAL, 0); ++nArgs;
	PtSetArg(&args[nArgs], Pt_ARG_GROUP_SPACING, 30, 0); ++nArgs;
	allGroup = PtCreateWidget(PtGroup, scroll, nArgs, args);
	
	/* open the state debug variables data base */
	stateDebugdbase = ApOpenDBase(ABM_stateDebugPictures);

	i = 0;
	chainIndx = -1;

	// if this paradigm was built with rebel
	if((snames[0]->rebelFlag1 == REBEL_FLAG1) && (snames[0]->rebelFlag2 == REBEL_FLAG2)) {
		while(snames[i]) {
			j = 0;
			while(nowstate[j].initial_state) {
				if(!strcmp(snames[i]->statename, nowstate[j].initial_state->statename)) {
					chainIndx++;		/* increment chain index */
					chainStateIndx = 0;	/* initial this chain's state index */

					/* create a group widget to hold the state buttons */
					nArgs = 0;
					PtSetArg(&args[nArgs], Pt_ARG_GROUP_ORIENTATION, Pt_GROUP_ASIS, 0); ++nArgs;
					PtSetArg(&args[nArgs], Pt_ARG_ANCHOR_FLAGS, Pt_TRUE,
							 Pt_LEFT_ANCHORED_LEFT|Pt_TOP_ANCHORED_TOP|Pt_BOTTOM_ANCHORED_BOTTOM); ++nArgs;
					PtSetArg(&args[nArgs], Pt_ARG_GROUP_FLAGS, Pt_TRUE, Pt_GROUP_EQUAL_SIZE); ++nArgs;
					PtSetArg(&args[nArgs], Pt_ARG_FILL_COLOR, stateButtonColors[2], 0); ++nArgs;
					buttonGroup = PtCreateWidget(PtGroup, allGroup, nArgs, args);
				}
				j++;
			}

			/* create a set of buttons in the group */
			if(snames[i]->preset) strcpy(buttonLabel, "T");
			else strcpy(buttonLabel, "-");

			if(snames[i]->random) strcat(buttonLabel, "R");
			else strcat(buttonLabel, "-");

			strncat(buttonLabel, snames[i]->statename, 10);
			sprintf(timeStamp, "  %ld", stateDebug[i].time);
			strcat(buttonLabel, timeStamp);
			sPos.x = snames[i]->stateXpos;
			sPos.y = snames[i]->stateYpos;

			nArgs = 0;
			PtSetArg(&args[nArgs], Pt_ARG_TEXT_STRING, buttonLabel, 0); ++nArgs;
			PtSetArg(&args[nArgs], Pt_ARG_FILL_COLOR, stateDebug[i].color, 0); ++nArgs;
			PtSetArg(&args[nArgs], Pt_ARG_USER_DATA, &i, sizeof(int)); ++nArgs;
			chainStates[chainIndx][chainStateIndx].stateButton = ApCreateWidget(stateDebugdbase, "stateButton", sPos.x, sPos.y, nArgs, args);
			chainStates[chainIndx][chainStateIndx].stateIndex = i;
			chainStateIndx++;
		
			i++;
		}
	}
	// if this paradigm was built with spot
	else {
		while(snames[i]) {
			j = 0;
			while(nowstate[j].initial_state) {
				if(!strcmp(snames[i]->statename, nowstate[j].initial_state->statename)) {
					chainIndx++;		/* increment chain index */
					chainStateIndx = 0;	/* initial this chain's state index */

					/* create a group widget to hold the state buttons */
					nArgs = 0;
					PtSetArg(&args[nArgs], Pt_ARG_GROUP_ORIENTATION, Pt_GROUP_VERTICAL, 0); ++nArgs;
					PtSetArg(&args[nArgs], Pt_ARG_GROUP_ROWS_COLS, 24, 0); ++nArgs;
					PtSetArg(&args[nArgs], Pt_ARG_ANCHOR_FLAGS, Pt_TRUE,
							 Pt_LEFT_ANCHORED_LEFT|Pt_TOP_ANCHORED_TOP|Pt_BOTTOM_ANCHORED_BOTTOM); ++nArgs;
					PtSetArg(&args[nArgs], Pt_ARG_GROUP_SPACING, 4, 0); ++nArgs;
					PtSetArg(&args[nArgs], Pt_ARG_GROUP_FLAGS, Pt_TRUE, Pt_GROUP_EQUAL_SIZE); ++nArgs;
					PtSetArg(&args[nArgs], Pt_ARG_FILL_COLOR, stateButtonColors[2], 0); ++nArgs;
					buttonGroup = PtCreateWidget(PtGroup, allGroup, nArgs, args);
				}
				j++;
			}

			/* create a set of buttons in the group */
			if(snames[i]->preset) strcpy(buttonLabel, "T");
			else strcpy(buttonLabel, "-");

			if(snames[i]->random) strcat(buttonLabel, "R");
			else strcat(buttonLabel, "-");

			strncat(buttonLabel, snames[i]->statename, 10);
			sprintf(timeStamp, "  %ld", stateDebug[i].time);
			strcat(buttonLabel, timeStamp);

			nArgs = 0;
			PtSetArg(&args[nArgs], Pt_ARG_TEXT_STRING, buttonLabel, 0); ++nArgs;
			PtSetArg(&args[nArgs], Pt_ARG_FILL_COLOR, stateDebug[i].color, 0); ++nArgs;
			PtSetArg(&args[nArgs], Pt_ARG_USER_DATA, &i, sizeof(int)); ++nArgs;
			chainStates[chainIndx][chainStateIndx].stateButton = ApCreateWidget(stateDebugdbase, "stateButton", 0, 0, nArgs, args);
			chainStates[chainIndx][chainStateIndx].stateIndex = i;
			chainStateIndx++;
		
			i++;
		}
	}

	PtSetArg(&args[0], Pt_ARG_USER_DATA, 0, 0);
	PtGetResources(chainStates[0][0].stateButton, 1, &args[0]);
	ApCloseDBase(stateDebugdbase);

	return( Pt_CONTINUE );
}

int initStateDebugDialog( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtWidget_t *widget;
	PtArg_t arg;
	int stateIndex;

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	/* get the widget that initiated the link callback */
	widget = ApWidget(cbinfo);

	/* make sure that the dialog has not been destroyed */
	if(PtWidgetFlags(widget) & Pt_DESTROYED) {
		return( Pt_CONTINUE );
	}

	/* get the state index from the buttons user data */
	PtSetArg(&arg, Pt_ARG_USER_DATA, 0, 0);
	PtGetResources(widget, 1, &arg);
	stateIndex = *(int *)arg.value;

	/* set the text of the title widget */
	PtSetArg(&arg, Pt_ARG_WINDOW_TITLE, snames[stateIndex]->statename, 0);
	PtSetResources(ABW_stateDebugDialog, 1, &arg);

	/* initialize the widgets in the state dialog */
	stateDebugDialogSetUser(stateIndex);

	return( Pt_CONTINUE );
}

int closeDebugWindow( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PhWindowEvent_t *winEvent;
	PtArg_t args[2];
	int i, j;

	extern int errprt;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get pointer to the window event structure */
	winEvent = (PhWindowEvent_t *)cbinfo->cbdata;

	/* check to see which type event evoked this callback */
	if(winEvent->event_f == Ph_WM_CLOSE) {
		/* zero out the chainStates array */
		for(i = 0; i < MAXCHAIN; i++) {
			for(j = 0; j < 64; j++) {
				chainStates[i][j].stateButton = (PtWidget_t *)NULL;
				chainStates[i][j].stateIndex = -1;
			}
		}

		/* set the debug button to off */
		PtSetArg(&args[0], Pt_ARG_ONOFF_STATE, 0, 0);
		PtSetArg(&args[1], Pt_ARG_TEXT_STRING, "Debug\nOff", 0);
		PtSetResources(ABW_debugOnOffButton, 2, args);
		errprt = 0;
		debugWindow = (PtWidget_t *)NULL;
		
		if(debugDialog) {
			PtDestroyWidget(debugDialog);
			debugDialog = (PtWidget_t *)NULL;
		}
	}
	return( Pt_CONTINUE );
}

int swapColors( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[2];
	PtWidget_t *thisButton;
	long thisTime;
	int thisIndx;
	int i, j;
	char thisColor;
	char buttonLabel[128];
	char timeStamp[32];

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	for(i = 0; i < MAXCHAIN; i++) {
		for(j = 0; j < 64; j++) {
			thisButton = chainStates[i][j].stateButton;
			thisIndx = chainStates[i][j].stateIndex;
			thisTime = stateDebug[thisIndx].time;
			thisColor = stateDebug[thisIndx].color;
			if(thisButton) {
				if(snames[thisIndx]->preset) strcpy(buttonLabel, "T");
				else strcpy(buttonLabel, "-");
				if(snames[thisIndx]->random) strcat(buttonLabel, "R");
				else strcat(buttonLabel, "-");
				strcat(buttonLabel, snames[thisIndx]->statename);
				sprintf(timeStamp, "        %ld", stateDebug[thisIndx].time);
				strcat(buttonLabel, timeStamp);
				PtSetArg(&args[0], Pt_ARG_TEXT_STRING, buttonLabel, 0);
				PtSetArg(&args[1], Pt_ARG_FILL_COLOR, stateButtonColors[thisColor], 0);
				PtSetResources(thisButton, 2, args);
			}
		}
	}
	return( Pt_CONTINUE );
}

int showStateDebugDialog( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	int *stateIndex;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	
	if(!debugDialog) {
		debugDialog = ApCreateModule(ABM_stateDebugDialog, widget, cbinfo);
	}
	
	PtGetResource(widget, Pt_ARG_USER_DATA, &stateIndex, 0);
	
	stateDebugDialogSetUser(*stateIndex);

	return( Pt_CONTINUE );
}

int closeDebugDialog( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PhWindowEvent_t *winEvent;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get pointer to the window event structure */
	winEvent = (PhWindowEvent_t *)cbinfo->cbdata;

	/* check to see which type event evoked this callback */
	if(winEvent->event_f == Ph_WM_CLOSE) {
		debugDialog = (PtWidget_t *)NULL;
	}

	return( Pt_CONTINUE );
}

int initDebugPanel( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	extern STATE *snames[];

	int stateIndex;
	int i;
	char *name;
	
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the text from the button */
	PtGetResource(widget, Pt_ARG_TEXT_STRING, &name, 0);
	if(name != (char *)NULL) {
		stateIndex = -1;
		i = 0;
		while(snames[i]) {
			if(!strcmp(name, snames[i]->statename)) {
				stateIndex = i;
				break;
			}
			i++;
		}

		if(stateIndex > -1) {
			stateDebugDialogSetUser(stateIndex);
		}
	}

	return( Pt_CONTINUE );
}

void stateDebugDialogSetUser(int stateIndex)
{
	PtArg_t args[2];
	STATE *thisState;
	extern STATE *snames[];

	thisState = snames[stateIndex];

	PtSetResource(ABW_stateDebugDialog, Pt_ARG_WINDOW_TITLE, thisState->statename, 0);
	
	PtSetArg(&args[0], Pt_ARG_USER_DATA, &stateIndex, sizeof(int));
	PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, thisState->preset, 0);
	PtSetResources(ABW_stateTime, 2, args);

	PtSetArg(&args[0], Pt_ARG_USER_DATA, &stateIndex, sizeof(int));
	PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, thisState->random, 0);
	PtSetResources(ABW_stateRandom, 2, args);

	PtSetArg(&args[0], Pt_ARG_USER_DATA, &stateIndex, sizeof(int));
	PtSetResources(ABW_stateRunLineToggleGroup, 1, args);

	/* set the state of the running line flag toggles */
	if(thisState->rlflag == 0) {
		PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_stateRunLineAbsToggle, 1, &args[1]);

		PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_stateRunLineRelToggle, 1, &args[1]);

		PtSetArg(&args[0], Pt_ARG_USER_DATA, &stateIndex, sizeof(int));
		PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, thisState->rlval, 0);
		PtSetResources(ABW_runningLine, 2, args);
	}
	if(thisState->rlflag == 1) {
		PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(ABW_stateRunLineAbsToggle, 1, &args[1]);

		PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_stateRunLineRelToggle, 1, &args[1]);

		PtSetArg(&args[0], Pt_ARG_USER_DATA, &stateIndex, sizeof(int));
		PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, thisState->rlval, 0);
		PtSetResources(ABW_runningLine, 2, args);
	}
	else if(thisState->rlflag == 2) {
		PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_stateRunLineAbsToggle, 1, &args[1]);

		PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(ABW_stateRunLineRelToggle, 1, &args[1]);

		PtSetArg(&args[0], Pt_ARG_USER_DATA, &stateIndex, sizeof(int));
		PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, thisState->rlval, 0);
		PtSetResources(ABW_runningLine, 2, args);
	}
	else if(thisState->rlflag == 3) {
		PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_stateRunLineAbsToggle, 1, &args[1]);

		PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(ABW_stateRunLineRelToggle, 1, &args[1]);

		PtSetArg(&args[0], Pt_ARG_USER_DATA, &stateIndex, sizeof(int));
		PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, -thisState->rlval, 0);
		PtSetResources(ABW_runningLine, 2, args);
	}

	PtSetArg(&args[0], Pt_ARG_USER_DATA, &stateIndex, sizeof(int));
	PtSetResources(ABW_stateArg1ToggleGroup, 1, args);

	PtSetArg(&args[0], Pt_ARG_USER_DATA, &stateIndex, sizeof(int));
	PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, thisState->device.act_code, 0);
	PtSetResources(ABW_stateArg1, 2, args);

	PtSetArg(&args[0], Pt_ARG_USER_DATA, &stateIndex, sizeof(int));
	PtSetResources(ABW_stateArg2ToggleGroup, 1, args);

	PtSetArg(&args[0], Pt_ARG_USER_DATA, &stateIndex, sizeof(int));
	PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, thisState->device.act_2code, 0);
	PtSetResources(ABW_stateArg2, 2, args);

	PtSetArg(&args[0], Pt_ARG_USER_DATA, &stateIndex, sizeof(int));
	PtSetResources(ABW_stateArg3ToggleGroup, 1, args);

	PtSetArg(&args[0], Pt_ARG_USER_DATA, &stateIndex, sizeof(int));
	PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, thisState->device.act_3code, 0);
	PtSetResources(ABW_stateArg3, 2, args);

	PtSetArg(&args[0], Pt_ARG_USER_DATA, &stateIndex, sizeof(int));
	PtSetResources(ABW_stateArg4ToggleGroup, 1, args);

	PtSetArg(&args[0], Pt_ARG_USER_DATA, &stateIndex, sizeof(int));
	PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, thisState->device.act_4code, 0);
	PtSetResources(ABW_stateArg4, 2, args);

	PtSetArg(&args[0], Pt_ARG_USER_DATA, &stateIndex, sizeof(int));
	PtSetResources(ABW_stateArg5ToggleGroup, 1, args);

	PtSetArg(&args[0], Pt_ARG_USER_DATA, &stateIndex, sizeof(int));
	PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, thisState->device.act_5code, 0);
	PtSetResources(ABW_stateArg5, 2, args);

	PtSetArg(&args[0], Pt_ARG_USER_DATA, &stateIndex, sizeof(int));
	PtSetResources(ABW_stateArg6ToggleGroup, 1, args);

	PtSetArg(&args[0], Pt_ARG_USER_DATA, &stateIndex, sizeof(int));
	PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, thisState->device.act_6code, 0);
	PtSetResources(ABW_stateArg6, 2, args);

	if(thisState->escape[0].stateptr) {
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, thisState->escape[0].stateptr->statename, 0);
	}
	else PtSetArg(&args[0], Pt_ARG_TEXT_STRING, "", 0);
	PtSetResources(ABW_stateEsc1Button, 1, &args[0]);

	if(thisState->escape[1].stateptr) {
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, thisState->escape[1].stateptr->statename, 0);
	}
	else PtSetArg(&args[0], Pt_ARG_TEXT_STRING, "", 0);
	PtSetResources(ABW_stateEsc2Button, 1, &args[0]);

	if(thisState->escape[2].stateptr) {
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, thisState->escape[2].stateptr->statename, 0);
	}
	else PtSetArg(&args[0], Pt_ARG_TEXT_STRING, "", 0);
	PtSetResources(ABW_stateEsc3Button, 1, &args[0]);

	if(thisState->escape[3].stateptr) {
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, thisState->escape[3].stateptr->statename, 0);
	}
	else PtSetArg(&args[0], Pt_ARG_TEXT_STRING, "", 0);
	PtSetResources(ABW_stateEsc4Button, 1, &args[0]);

	if(thisState->escape[4].stateptr) {
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, thisState->escape[4].stateptr->statename, 0);
	}
	else PtSetArg(&args[0], Pt_ARG_TEXT_STRING, "", 0);
	PtSetResources(ABW_stateEsc5Button, 1, &args[0]);

	if(thisState->escape[5].stateptr) {
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, thisState->escape[5].stateptr->statename, 0);
	}
	else PtSetArg(&args[0], Pt_ARG_TEXT_STRING, "", 0);
	PtSetResources(ABW_stateEsc6Button, 1, &args[0]);

	if(thisState->escape[6].stateptr) {
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, thisState->escape[6].stateptr->statename, 0);
	}
	else PtSetArg(&args[0], Pt_ARG_TEXT_STRING, "", 0);
	PtSetResources(ABW_stateEsc7Button, 1, &args[0]);

	if(thisState->escape[7].stateptr) {
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, thisState->escape[7].stateptr->statename, 0);
	}
	else PtSetArg(&args[0], Pt_ARG_TEXT_STRING, "", 0);
	PtSetResources(ABW_stateEsc8Button, 1, &args[0]);

	if(thisState->escape[8].stateptr) {
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, thisState->escape[8].stateptr->statename, 0);
	}
	else PtSetArg(&args[0], Pt_ARG_TEXT_STRING, "", 0);
	PtSetResources(ABW_stateEsc9Button, 1, &args[0]);

	if(thisState->escape[9].stateptr) {
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, thisState->escape[9].stateptr->statename, 0);
	}
	else PtSetArg(&args[0], Pt_ARG_TEXT_STRING, "", 0);
	PtSetResources(ABW_stateEsc10Button, 1, &args[0]);

	return;
}

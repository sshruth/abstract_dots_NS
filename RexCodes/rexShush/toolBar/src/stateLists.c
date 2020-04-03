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

extern PtWidget_t *debugDialog;
extern int stateArguments[6][3];

int initAllStates( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;
	PtTreeItem_t *item;
	PtTreeItem_t *chainBrother;
	PtTreeItem_t *stateBrother;
	PtTreeItem_t *firstItem;
	PtTreeItem_t *firstChain;
	int newChain;
	int i, j;
	char chainName[16];
	extern STATE *snames[];
	extern AWAKE nowstate[];

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	PtSetArg(&arg, Pt_ARG_WINDOW_TITLE, "All States", 0);
	PtSetResources(link_instance, 1, &arg);

	/* delete all of the previous items from the list */
	PtTreeFreeAllItems(ABW_stateDialogTree);

	/* build the list */
	i = 0;
	newChain = 0;
	while(snames[i]) {
		/* check to see if this state is the start of a new chain */
		j = 0;
		while(nowstate[j].initial_state) {
			if(!strcmp(snames[i]->statename, nowstate[j].initial_state->statename)) {
				newChain = 1;
				sprintf(chainName, "Chain %d", j + 1);
				item = PtTreeAllocItem(ABW_stateDialogTree, chainName, -1, -1);

				if(!j) {	/* if this is the first state, make it the top root */
					PtTreeAddFirst(ABW_stateDialogTree, item, NULL);
					firstChain = item;
				}
				else {		/* otherwise make it a brother root */
					PtTreeAddAfter(ABW_stateDialogTree, item, chainBrother);
				}
				chainBrother = item;
			}
			j++;
		}
		item = PtTreeAllocItem(ABW_stateDialogTree, snames[i]->statename, -1, -1);

		if(newChain) {	/* if this is the first state in a chain */
			PtTreeAddFirst(ABW_stateDialogTree, item, chainBrother);
			newChain = 0;
		}
		else {
			PtTreeAddAfter(ABW_stateDialogTree, item, stateBrother);
		}
		stateBrother = item;
		i++;
	}

	/* make the first item selected */
	firstItem = PtTreeRootItem(ABW_stateDialogTree);
	PtTreeGoto(ABW_stateDialogTree, firstItem);

	/* expand the first chain */
	PtTreeExpand(ABW_stateDialogTree, firstChain, 0);

	/* set the initial user data in the dialog items */
	stateListDialogSetUser(0);

	return( Pt_CONTINUE );
}

int initTimedStates( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;
	PtTreeItem_t *item;
	PtTreeItem_t *chainBrother;
	PtTreeItem_t *stateBrother;
	PtTreeItem_t *firstItem;
	PtTreeItem_t *firstChain;
	int firstIndex;
	int newChain;
	int i, j;
	char chainName[16];
	extern STATE *snames[];
	extern AWAKE nowstate[];

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	PtSetArg(&arg, Pt_ARG_WINDOW_TITLE, "Timed States", 0);
	PtSetResources(link_instance, 1, &arg);

	/* delete all of the previous items from the list */
	PtTreeFreeAllItems(ABW_stateDialogTree);
	
	/* build the list */
	i = 0;
	firstIndex = -1;
	newChain = 0;
	while(snames[i]) {
		/* check to see if this state is the start of a new chain */
		j = 0;
		while(nowstate[j].initial_state) {
			if(!strcmp(snames[i]->statename, nowstate[j].initial_state->statename)) {
				newChain = 1;
				sprintf(chainName, "Chain %d", j + 1);
				item = PtTreeAllocItem(ABW_stateDialogTree, chainName, -1, -1);

				if(!j) {	/* if this is the first state, make it the top root */
					PtTreeAddFirst(ABW_stateDialogTree, item, NULL);
					firstChain = item;
				}
				else {		/* otherwise make it a brother root */
					PtTreeAddAfter(ABW_stateDialogTree, item, chainBrother);
				}
				chainBrother = item;
			}
			j++;
		}
		if((snames[i]->preset > 0) || (snames[i]->random > 0)) {
			if(firstIndex < 0) firstIndex = i;
			item = PtTreeAllocItem(ABW_stateDialogTree, snames[i]->statename, -1, -1);

			if(newChain) {	/* if this is the first state in a chain */
				PtTreeAddFirst(ABW_stateDialogTree, item, chainBrother);
				newChain = 0;
			}
			else {
				PtTreeAddAfter(ABW_stateDialogTree, item, stateBrother);
			}
			stateBrother = item;
		}
		i++;
	}

	/* make the first item selected */
	firstItem = PtTreeRootItem(ABW_stateDialogTree);
	PtTreeGoto(ABW_stateDialogTree, firstItem);

	/* expand the first chain */
	PtTreeExpand(ABW_stateDialogTree, firstChain, 0);

	/* set the initial user data in the dialog items */
	stateListDialogSetUser(firstIndex);

	return( Pt_CONTINUE );
}

int initStateList( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	extern STATE *snames[];

	PtArg_t arg;
	PtTreeCallback_t *listData;
	int stateIndex;
	int i;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(cbinfo->reason_subtype != Pt_LIST_SELECTION_FINAL) return(Pt_CONTINUE);

	/* get a pointer to the list */
	listData = (PtTreeCallback_t *)cbinfo->cbdata;

	if(strstr(listData->item->string, "Chain")) {
		return( Pt_CONTINUE );
	}

	/* set the user data of the widgets in the dialog */
	i = 0;
	stateIndex = -1;
	while(snames[i]) {
		if(!strcmp(listData->item->string, snames[i]->statename)) {
			stateIndex = i;
			break;
		}
		i++;
	}

	if(stateIndex > -1) {
		stateListDialogSetUser(stateIndex);
	}
	
	return( Pt_CONTINUE );
}

int initStatePanel( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	extern STATE *snames[];

	int stateIndex;
	int i;
	char *name = (char *)NULL;
	
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
			stateListDialogSetUser(stateIndex);
		}
	}

	return( Pt_CONTINUE );
}

void stateListDialogSetUser(int stateIndex)
{
	PtArg_t args[2];
	STATE *thisState;
	extern STATE *snames[];

	thisState = snames[stateIndex];

	PtSetResource(ABW_stateListLabel, Pt_ARG_TEXT_STRING, thisState->statename, 0);
	
	PtSetArg(&args[0], Pt_ARG_USER_DATA, &stateIndex, sizeof(int));
	PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, thisState->preset, 0);
	PtSetResources(ABW_statePresetTime, 2, args);

	PtSetArg(&args[0], Pt_ARG_USER_DATA, &stateIndex, sizeof(int));
	PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, thisState->random, 0);
	PtSetResources(ABW_stateRandomTime, 2, args);

	PtSetArg(&args[0], Pt_ARG_USER_DATA, &stateIndex, sizeof(int));
	PtSetResources(ABW_stateRunLineFlagGroup, 1, args);

	/* set the state of the running line flag toggles */
	if(thisState->rlflag == 0) {
		PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_stateRunLineAbsFlag, 1, &args[1]);

		PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_stateRunLineRelFlag, 1, &args[1]);

		PtSetArg(&args[0], Pt_ARG_USER_DATA, &stateIndex, sizeof(int));
		PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, thisState->rlval, 0);
		PtSetResources(ABW_stateRunLineVal, 2, args);
	}
	if(thisState->rlflag == 1) {
		PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(ABW_stateRunLineAbsFlag, 1, &args[1]);

		PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_stateRunLineRelFlag, 1, &args[1]);

		PtSetArg(&args[0], Pt_ARG_USER_DATA, &stateIndex, sizeof(int));
		PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, thisState->rlval, 0);
		PtSetResources(ABW_stateRunLineVal, 2, args);
	}
	else if(thisState->rlflag == 2) {
		PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_stateRunLineAbsFlag, 1, &args[1]);

		PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(ABW_stateRunLineRelFlag, 1, &args[1]);

		PtSetArg(&args[0], Pt_ARG_USER_DATA, &stateIndex, sizeof(int));
		PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, thisState->rlval, 0);
		PtSetResources(ABW_stateRunLineVal, 2, args);
	}
	else if(thisState->rlflag == 3) {
		PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetResources(ABW_stateRunLineAbsFlag, 1, &args[1]);

		PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetResources(ABW_stateRunLineRelFlag, 1, &args[1]);

		PtSetArg(&args[0], Pt_ARG_USER_DATA, &stateIndex, sizeof(int));
		PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, -thisState->rlval, 0);
		PtSetResources(ABW_stateRunLineVal, 2, args);
	}

	PtSetArg(&args[0], Pt_ARG_USER_DATA, &stateIndex, sizeof(int));
	PtSetResources(ABW_stateArg1FormatGroup, 1, args);

	PtSetArg(&args[0], Pt_ARG_USER_DATA, &stateIndex, sizeof(int));
	PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, thisState->device.act_code, 0);
	PtSetResources(ABW_stateArg1Val, 2, args);

	PtSetArg(&args[0], Pt_ARG_USER_DATA, &stateIndex, sizeof(int));
	PtSetResources(ABW_stateArg2FormatGroup, 1, args);

	PtSetArg(&args[0], Pt_ARG_USER_DATA, &stateIndex, sizeof(int));
	PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, thisState->device.act_2code, 0);
	PtSetResources(ABW_stateArg2Val, 2, args);

	PtSetArg(&args[0], Pt_ARG_USER_DATA, &stateIndex, sizeof(int));
	PtSetResources(ABW_stateArg3FormatGroup, 1, args);

	PtSetArg(&args[0], Pt_ARG_USER_DATA, &stateIndex, sizeof(int));
	PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, thisState->device.act_3code, 0);
	PtSetResources(ABW_stateArg3Val, 2, args);

	PtSetArg(&args[0], Pt_ARG_USER_DATA, &stateIndex, sizeof(int));
	PtSetResources(ABW_stateArg4FormatGroup, 1, args);

	PtSetArg(&args[0], Pt_ARG_USER_DATA, &stateIndex, sizeof(int));
	PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, thisState->device.act_4code, 0);
	PtSetResources(ABW_stateArg4Val, 2, args);

	PtSetArg(&args[0], Pt_ARG_USER_DATA, &stateIndex, sizeof(int));
	PtSetResources(ABW_stateArg5FormatGroup, 1, args);

	PtSetArg(&args[0], Pt_ARG_USER_DATA, &stateIndex, sizeof(int));
	PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, thisState->device.act_5code, 0);
	PtSetResources(ABW_stateArg5Val, 2, args);

	PtSetArg(&args[0], Pt_ARG_USER_DATA, &stateIndex, sizeof(int));
	PtSetResources(ABW_stateArg6FormatGroup, 1, args);

	PtSetArg(&args[0], Pt_ARG_USER_DATA, &stateIndex, sizeof(int));
	PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, thisState->device.act_6code, 0);
	PtSetResources(ABW_stateArg6Val, 2, args);

	if(thisState->escape[0].stateptr) {
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, thisState->escape[0].stateptr->statename, 0);
	}
	else PtSetArg(&args[0], Pt_ARG_TEXT_STRING, "", 0);
	PtSetResources(ABW_firstEscButton, 1, &args[0]);

	if(thisState->escape[1].stateptr) {
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, thisState->escape[1].stateptr->statename, 0);
	}
	else PtSetArg(&args[0], Pt_ARG_TEXT_STRING, "", 0);
	PtSetResources(ABW_secondEscButton, 1, &args[0]);

	if(thisState->escape[2].stateptr) {
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, thisState->escape[2].stateptr->statename, 0);
	}
	else PtSetArg(&args[0], Pt_ARG_TEXT_STRING, "", 0);
	PtSetResources(ABW_thirdEscButton, 1, &args[0]);

	if(thisState->escape[3].stateptr) {
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, thisState->escape[3].stateptr->statename, 0);
	}
	else PtSetArg(&args[0], Pt_ARG_TEXT_STRING, "", 0);
	PtSetResources(ABW_fourthEscButton, 1, &args[0]);

	if(thisState->escape[4].stateptr) {
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, thisState->escape[4].stateptr->statename, 0);
	}
	else PtSetArg(&args[0], Pt_ARG_TEXT_STRING, "", 0);
	PtSetResources(ABW_fifthEscButton, 1, &args[0]);

	if(thisState->escape[5].stateptr) {
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, thisState->escape[5].stateptr->statename, 0);
	}
	else PtSetArg(&args[0], Pt_ARG_TEXT_STRING, "", 0);
	PtSetResources(ABW_sixthEscButton, 1, &args[0]);

	if(thisState->escape[6].stateptr) {
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, thisState->escape[6].stateptr->statename, 0);
	}
	else PtSetArg(&args[0], Pt_ARG_TEXT_STRING, "", 0);
	PtSetResources(ABW_seventhEscButton, 1, &args[0]);

	if(thisState->escape[7].stateptr) {
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, thisState->escape[7].stateptr->statename, 0);
	}
	else PtSetArg(&args[0], Pt_ARG_TEXT_STRING, "", 0);
	PtSetResources(ABW_eighthEscButton, 1, &args[0]);

	if(thisState->escape[8].stateptr) {
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, thisState->escape[8].stateptr->statename, 0);
	}
	else PtSetArg(&args[0], Pt_ARG_TEXT_STRING, "", 0);
	PtSetResources(ABW_ninthEscButton, 1, &args[0]);

	if(thisState->escape[9].stateptr) {
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, thisState->escape[9].stateptr->statename, 0);
	}
	else PtSetArg(&args[0], Pt_ARG_TEXT_STRING, "", 0);
	PtSetResources(ABW_tenthEscButton, 1, &args[0]);

	return;
}

int setStateTime( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtWidget_t *dialog;
	PtArg_t args[2];
	STATE *thisState;
	int dialogName;
	int *time;
	int *stateIndex;
	extern STATE *snames[];

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the state name and time value from the widget */
	PtSetArg(&args[0], Pt_ARG_USER_DATA, &stateIndex, 0);
	PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, &time, 0);
	PtGetResources(widget, 2, args);

	/* get a pointer to this state */
	thisState = snames[*stateIndex];

	/* figure out which widget initiated this callback */
	if((dialog = ApGetInstance(widget)) == (PtWidget_t *)NULL) {
		return( Pt_CONTINUE );
	}
	if((dialogName = ApName(dialog)) == -1) {
		return( Pt_CONTINUE );
	}

	/* was this widget in the state list dialog */
	if(dialogName == ABN_stateListDialog) {
		if(ApName(widget) == ABN_statePresetTime) {
			thisState->preset = *time;
		}
		else if(ApName(widget) == ABN_stateRandomTime) {
			thisState->random = *time;
		}
	}
	/* or in the state debug dialog */
	else if(dialogName == ABN_stateDebugDialog) {
		if(ApName(widget) == ABN_stateTime) {
			thisState->preset = *time;
		}
		else if(ApName(widget) == ABN_stateRandom) {
			thisState->random = *time;
		}
	}

	return( Pt_CONTINUE );
}

int setStateRunLineFlag( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[2];
	PtWidget_t *parent;
	PtWidget_t *dialog;
	PtWidget_t *numInt;
	int dialogName;
	int rlToggleName;
	STATE *thisState;
	int value;
	int *stateIndex;
	extern STATE *snames[];

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the state name from the group */
	parent = PtWidgetParent(widget);
	PtSetArg(&args[0], Pt_ARG_USER_DATA, &stateIndex, 0);
	PtGetResources(parent, 1, &args[0]);
	thisState = snames[*stateIndex];

	/* get the running line value widget for this dialog */
	if((dialog = ApGetInstance(widget)) == (PtWidget_t *)NULL) {
		return( Pt_CONTINUE );
	}
	if((dialogName = ApName(dialog)) == -1) {
		return( Pt_CONTINUE );
	}
	if(dialogName == ABN_stateListDialog) {
		if((numInt = ApGetWidgetPtr(dialog, ABN_stateRunLineVal)) == (PtWidget_t *)NULL) {
			return( Pt_CONTINUE );
		}
	}
	else if(dialogName == ABN_stateDebugDialog) {
		if((numInt = ApGetWidgetPtr(dialog, ABN_runningLine)) == (PtWidget_t *)NULL) {
			return( Pt_CONTINUE );
		}
	}

	/* get the name of the toggle widget */
	if((rlToggleName = ApName(widget)) == -1) {
		return( Pt_CONTINUE );
	}

	/* if a button was released */
	if(cbinfo->reason == Pt_CB_DISARM) {
		thisState->rlflag = 0;
		thisState->rlval = 0;
		PtSetArg(&args[0], Pt_ARG_NUMERIC_MIN, 0, 0);
		PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, thisState->rlval, 0);
		PtSetResources(numInt, 2, args);
	}
	else if(cbinfo->reason == Pt_CB_ARM) {
		/* figure out which button was pressed */
		if((rlToggleName == ABN_stateRunLineAbsFlag) || (rlToggleName == ABN_stateRunLineAbsToggle)) {
			thisState->rlflag = 1;
			PtSetArg(&args[0], Pt_ARG_NUMERIC_MIN, 0, 0);
			PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, thisState->rlval, 0);
			PtSetResources(numInt, 2, args);
		}
		else if((rlToggleName == ABN_stateRunLineRelFlag) || (rlToggleName == ABN_stateRunLineRelToggle)) {
			if(thisState->rlflag == 3) value = -thisState->rlval;
			else value = thisState->rlval;
			thisState->rlflag = 2;
			PtSetArg(&args[0], Pt_ARG_NUMERIC_MIN, -128, 0);
			PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, value, 0);
			PtSetResources(numInt, 2, args);
		}
	}

	return( Pt_CONTINUE );
}

int setStateRunLineVal( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[2];
	PtWidget_t *dialog;
	PtWidget_t *rlAbsToggle;
	PtWidget_t *rlRelToggle;
	int dialogName;
	STATE *thisState;
	int *rlLevel;
	int *stateIndex;
	long *flags;
	extern STATE *snames[];

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the state name and the time value from the widget */
	PtSetArg(&args[0], Pt_ARG_USER_DATA, &stateIndex, 0);
	PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, &rlLevel, 0);
	PtGetResources(widget, 2, args);
	thisState = snames[*stateIndex];

	if(*rlLevel < 0) {
		thisState->rlval = -*rlLevel;
		thisState->rlflag = 3;
	}
	else {
		/* get the flag buttons for this dialog */
		if((dialog = ApGetInstance(widget)) == (PtWidget_t *)NULL) {
			return( Pt_CONTINUE );
		}
		if((dialogName = ApName(dialog)) == -1) {
			return( Pt_CONTINUE );
		}
		if(dialogName == ABN_stateListDialog) {
			if((rlAbsToggle = ApGetWidgetPtr(dialog, ABN_stateRunLineAbsFlag)) == (PtWidget_t *)NULL) {
				return( Pt_CONTINUE );
			}
			if((rlRelToggle = ApGetWidgetPtr(dialog, ABN_stateRunLineRelFlag)) == (PtWidget_t *)NULL) {
				return( Pt_CONTINUE );
			}
		}
		else if(dialogName == ABN_stateDebugDialog) {
			if((rlAbsToggle = ApGetWidgetPtr(dialog, ABN_stateRunLineAbsToggle)) == (PtWidget_t *)NULL) {
				return( Pt_CONTINUE );
			}
			if((rlRelToggle = ApGetWidgetPtr(dialog, ABN_stateRunLineRelToggle)) == (PtWidget_t *)NULL) {
				return( Pt_CONTINUE );
			}
		}
		
		/* get the state of the flag buttons */
		PtSetArg(&args[0], Pt_ARG_FLAGS, &flags, 0);
		PtGetResources(rlAbsToggle, 1, &args[0]);
		if(*flags & Pt_SET) {
			thisState->rlval = *rlLevel;
			thisState->rlflag = 1;
		}
		PtSetArg(&args[0], Pt_ARG_FLAGS, &flags, 0);
		PtGetResources(rlRelToggle, 1, &args[0]);
		if(*flags & Pt_SET) {
			thisState->rlval = *rlLevel;
			thisState->rlflag = 2;
		}
	}
	
	return( Pt_CONTINUE );
}

int setStateActionArgFormat( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[2];
	PtWidget_t *parent;
	PtWidget_t *dialog;
	PtWidget_t *argVal;
	int *stateIndex;
	int argument;
	int format;
	int i, j;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the widget's group */
	parent = PtWidgetParent(widget);

	/* get the state name from the group */
	PtSetArg(&args[0], Pt_ARG_USER_DATA, &stateIndex, 0);
	PtGetResources(parent, 1, args);

	/* figure out which argument and format was chosen */
	for(i = 0; i < 20; i++) {
		for(j = 0; j < 2; j++) {
			if(ApName(widget) == stateArguments[i][j]) {
				argument = i;
				format = j;
			}
		}
	}

	/* get pointer to this widget's dialog window */
	dialog = ApGetInstance(widget);

	/* get pointer to the corresponding argument value widget */
	argVal = ApGetWidgetPtr(dialog, stateArguments[argument][2]);

	/* set the format of the corresponding argument value widget */
	if(format) PtSetArg(&args[0], Pt_ARG_NUMERIC_FLAGS, Pt_TRUE, Pt_NUMERIC_HEXADECIMAL);
	else PtSetArg(&args[0], Pt_ARG_NUMERIC_FLAGS, Pt_FALSE, Pt_NUMERIC_HEXADECIMAL);
	PtSetResources(argVal, 1, args);

	return( Pt_CONTINUE );
}

int setStateActionArg( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[2];
	STATE *thisState;
	int *stateIndex;
	int *value;
	int argument;
	int i;
	extern STATE *snames[];

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the state name and value from the widget */
	PtSetArg(&args[0], Pt_ARG_USER_DATA, &stateIndex, 0);
	PtSetArg(&args[1], Pt_ARG_NUMERIC_VALUE, &value, 0);
	PtGetResources(widget, 2, args);

	thisState = snames[*stateIndex];

	/* figure out which argument was selected */
	for(i = 0; i < 20; i++) {
		if(ApName(widget) == stateArguments[i][2]) {
			argument = i + 1;
			break;
		}
	}

	switch(argument) {
	case 1:
		thisState->device.act_code = (long)*value;
		break;
	case 2:
		thisState->device.act_2code = (long)*value;
		break;
	case 3:
		thisState->device.act_3code = (long)*value;
		break;
	case 4:
		thisState->device.act_4code = (long)*value;
		break;
	case 5:
		thisState->device.act_5code = (long)*value;
		break;
	case 6:
		thisState->device.act_6code = (long)*value;
		break;
	case 7:
		thisState->device.act_7code = (long)*value;
		break;
	case 8:
		thisState->device.act_8code = (long)*value;
		break;
	case 9:
		thisState->device.act_9code = (long)*value;
		break;
	case 10:
		thisState->device.act_10code = (long)*value;
		break;
	}
	return( Pt_CONTINUE );
}

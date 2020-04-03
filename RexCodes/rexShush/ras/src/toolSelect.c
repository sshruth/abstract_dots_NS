/* Y o u r   D e s c r i p t i o n                       */
/*                            AppBuilder Photon Code Lib */
/*                                         Version 1.13X */

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
#include "ph_ras.h"
#include "abimport.h"
#include "proto.h"

int buildSelect( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[1];
	SELECTIONS *selections;
	SELECTSET *thisSet;
	char string[64];

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	/* get a pointer to the selections structure */
	selections = getSelections();

	/* set the current selection set index  to 0 */
	selections->curSetIndx = 0;
	
	/* create a label */
	sprintf(string, "Ecode Selection Set %d", selections->curSetIndx + 1);
	PtSetArg(&args[0], Pt_ARG_TEXT_STRING, string, 0);
	PtSetResources(ABW_selectSetLabel, 1, args);

	/* allocate space for a new set */
	/* this condition will be true if the user is defining selection sets
	 * for the first time
	 */
	if(!selections->nSelectSets) {
		selections->nSelectSets++;
		selections->selectSets = addSelectSet(selections->selectSets, selections->nSelectSets);

		/* set the newSets flag because a new set has been added */
		selections->newSelects = 1;

		/* initialize the new select set with some reasonable default values */
		thisSet = getSelectSet(selections->curSetIndx);
		initFirstSet(thisSet);
		
		/* set the widget values for the new set */
		initSetWidgets(link_instance, thisSet, selections->newSelects);
	}

	/* if the user is re-displaying the select sets page
	 * or the selection definitions have been read from a root
	 * get a pointer to the first select set and initialize the
	 * widgets to the values of that set
	 */
	else {
		thisSet = getSelectSet(selections->curSetIndx);

		/* unset the newSelects flag */
		selections->newSelects = 0;

		/* set the widget values on the new set */
		initSetWidgets(link_instance, thisSet, selections->newSelects);
	}

	/* set the maximum value on the select scroll bar */
	PtSetArg(&args[0], Pt_ARG_MAXIMUM, selections->nSelectSets - 1, 0);
	PtSetResources(ABW_selectScrollBar, 1, args);

	return( Pt_CONTINUE );

}

int newSelectSet(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
	PtArg_t args[2];
	SELECTIONS *selections;
	SELECTSET *thisSet;
	SELECTSET *lastSet;
	char string[64];
	
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get a pointer to the selections structure */
	selections = getSelections();

	/* increment the current selection set index */
	selections->curSetIndx++;
	
	/* create a window label */
	sprintf(string, "Ecode Selection Set %d", selections->curSetIndx + 1);
	PtSetArg(&args[0], Pt_ARG_TEXT_STRING, string, 0);
	PtSetResources(ABW_selectSetLabel, 1, args);

	/* allocate space for a new set */
	selections->nSelectSets++;
	selections->selectSets = addSelectSet(selections->selectSets, selections->nSelectSets);

	/* set the newSelects flag because a new set has been added */
	selections->newSelects = 1;

	/* initialize the new set with values from the previsou set */
	thisSet = getSelectSet(selections->curSetIndx);
	lastSet = getSelectSet(selections->curSetIndx - 1);
	initSubsequentSet(thisSet, lastSet);

	/* set the widget values for the new set */
	initSetWidgets(ABW_classesSelect, thisSet, selections->newSelects);

	/* set the maximum value on the set scroll bar */
	PtSetArg(&args[0], Pt_ARG_MAXIMUM, selections->nSelectSets - 1, 0);
	PtSetArg(&args[1], Pt_ARG_SCROLL_POSITION, selections->nSelectSets - 1, 0);
	PtSetResources(ABW_selectScrollBar, 2, args);

	return( Pt_CONTINUE );
}

int showSelectSet( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[1];
	PtScrollbarCallback_t *scroll = cbinfo->cbdata;
	SELECTIONS *selections;
	SELECTSET *thisSet;
	SELECT *thisSelect;
	char string[64];

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get a pointer to the selections structure */
	selections = getSelections();

	/* get the current select set index from the scroll bar */
	selections->curSetIndx = scroll->position;

	/* get pointer to the current selection set */
	thisSet = getSelectSet(selections->curSetIndx);
	thisSelect = &thisSet->select;

	/* create a window label */
	sprintf(string, "Ecode Selection Set %d", selections->curSetIndx + 1);
	PtSetArg(&args[0], Pt_ARG_TEXT_STRING, string, 0);
	PtSetResources(ABW_selectSetLabel, 1, args);

	/* unset the newSelects flag */
	selections->newSelects = 0;
	
	/* set the widget values for the new set */
	initSetWidgets(ABW_classesSelect, thisSet, selections->newSelects);

	return( Pt_CONTINUE );
}

int startCodeProc(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
	PtArg_t args[1];
	SELECTIONS *selections;
	int *code;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get a pointer to the selections structure */
	selections = getSelections();

	/* get the number entered by the user */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, &code, 0);
	PtGetResources(widget, 1, args);
	selections->startCode = *code;
	return( Pt_CONTINUE );
}


int preTimeProc( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[1];
	SELECTIONS *selections;
	int *code;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get a pointer to the selections structure */
	selections = getSelections();

	/* get the number entered by the user */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, &code, 0);
	PtGetResources(widget, 1, args);
	selections->preTime = *code;
	return( Pt_CONTINUE );

}

int alignCodeProc(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
	PtWidget_t *thisModule;
	PtArg_t args[1];
	SELECTIONS *selections;
	SELECTSET *thisSet;
	int *code;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get pointer to the selections structure */
	selections = getSelections();

	/* get the module that contains this widget */
	thisModule = ApGetInstance(widget);

	/* figure out this selection set */
	thisSet = getSelectSet(selections->curSetIndx);

	/* get the number entered by the user */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, &code, 0);
	PtGetResources(widget, 1, args);
	thisSet->alignCode = *code;

	return( Pt_CONTINUE );
}

int alignOffsetProc(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
	PtArg_t args[1];
	SELECTIONS *selections;
	SELECTSET *thisSelectSet;
	int *time;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get pointer to the selections structure */
	selections = getSelections();

	/* figure out this selection set */
	thisSelectSet = &selections->selectSets[selections->curSetIndx];

	/* get the number entered by the user */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, &time, 0);
	PtGetResources(widget, 1, args);
	thisSelectSet->alignOffsetTime = *time;

	return( Pt_CONTINUE );
}

int allOfProc(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
	PtArg_t args[1];
	SELECTIONS *selections;
	SELECTSET *thisSet;
	SELECT *thisSelect;
	char *string;
	char *tok;
	char stringCopy[1024];
	
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get pointer to the selections structure */
	selections = getSelections();

	/* figure out this selection set */
	thisSet = getSelectSet(selections->curSetIndx);
	thisSelect = &thisSet->select;	// get a pointer to the proper select struct
	
	/* get the list of all of codes */
	PtSetArg(&args[0], Pt_ARG_TEXT_STRING, &string, 0);
	PtGetResources(widget, 1, args);

	/* make a copy of the string */
	strcpy(stringCopy, string);
	
	thisSelect->nAllOf = 0;
	tok = strtok(stringCopy, "\n");
	if(!tok) {	/* return immediately if the the string is NULL */
		thisSelect->nAllOf = 0;
		return( Pt_CONTINUE );
	}

	sscanf(tok, "%d", &thisSelect->allOfList[thisSelect->nAllOf]);
	thisSelect->nAllOf++;
	while(tok = strtok(NULL, "\n")) {
		sscanf(tok, "%d", &thisSelect->allOfList[thisSelect->nAllOf]);
		thisSelect->nAllOf++;
	}

	return( Pt_CONTINUE );
}

int nonOfProc(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
	PtArg_t args[1];
	SELECTIONS *selections;
	SELECTSET *thisSet;
	SELECT *thisSelect;
	char *string;
	char *tok;
	char stringCopy[1024];

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get pointer to the selections structure */
	selections = getSelections();

	/* figure out this selection set */
	thisSet = getSelectSet(selections->curSetIndx);
	thisSelect = &thisSet->select;
	
	/* get the list of all of codes */
	PtSetArg(&args[0], Pt_ARG_TEXT_STRING, &string, 0);
	PtGetResources(widget, 1, args);

	/* make a copy of the string */
	strcpy(stringCopy, string);
	
	thisSelect->nNonOf = 0;
	tok = strtok(stringCopy, "\n");
	if(!tok) {	/* return immediately if the the string is NULL */
		thisSelect->nNonOf = 0;
		return( Pt_CONTINUE );
	}
	
	sscanf(tok, "%d", &thisSelect->nonOfList[thisSelect->nNonOf]);
	thisSelect->nNonOf++;
	while(tok = strtok(NULL, "\n")) {
		sscanf(tok, "%d", &thisSelect->nonOfList[thisSelect->nNonOf]);
		thisSelect->nNonOf++;
	}

	return( Pt_CONTINUE );
}

int anyOfProc(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
	PtArg_t args[1];
	SELECTIONS *selections;
	SELECTSET *thisSet;
	SELECT *thisSelect;
	int i, n, beg, end;
	int codeList[16];
	char *string;
	char stringCopy[1024];
	char *tok;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get pointer to the selections structure */
	selections = getSelections();

	/* figure out this selection set */
	thisSet = getSelectSet(selections->curSetIndx);
	thisSelect = &thisSet->select;	// get a pointer to the proper select struct
	
	/* get the list of all of codes */
	PtSetArg(&args[0], Pt_ARG_TEXT_STRING, &string, 0);
	PtGetResources(widget, 1, args);

	/* make a copy of the string */
	strcpy(stringCopy, string);
	
	thisSelect->nAnyOf = 0;
	tok = strtok(stringCopy, "\n");
	if(!tok) {	/* return immediately if the the string is NULL */
		thisSelect->nAnyOf = 0;
		return( Pt_CONTINUE );
	}

	/* first, scan tok assuming the user separated codes with " - " */
	n = sscanf(tok, "%d - %d", &beg, &end);
	if(n == 1) {
		/* if n is 1, scan tok again to see if user separated codes with a space */
		n = sscanf(tok, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
				   &codeList[0], &codeList[1], &codeList[2], &codeList[3], &codeList[4], &codeList[5],
				   &codeList[6], &codeList[7], &codeList[8], &codeList[9], &codeList[10], &codeList[11],
				   &codeList[12], &codeList[13], &codeList[14], &codeList[15]);
		if(n == 1) {
			/* if n is still 1, the user only entered 1 code */
			thisSelect->anyOfList[thisSelect->nAnyOf].beg = codeList[0];
			thisSelect->anyOfList[thisSelect->nAnyOf].end = codeList[0];

			for(i = 0; i < MAX_OR_CODES; i++) thisSelect->anyOfList[thisSelect->nAnyOf].list[i] = 0;
		}
		else if(n > 2) {
			/* if n is greater than 2 the user entered a set of codes to be or'ed */
			for(i = 0; i < MAX_OR_CODES; i++) thisSelect->anyOfList[thisSelect->nAnyOf].list[i] = 0;
			for(i = 0; i < n; i++) thisSelect->anyOfList[thisSelect->nAnyOf].list[i] = codeList[i];

			thisSelect->anyOfList[thisSelect->nAnyOf].beg = 0;
			thisSelect->anyOfList[thisSelect->nAnyOf].end = 0;
		}
	}
	else if(n == 2) {
		/* if n is 2, the user entered a range of codes separated by " - " */
		thisSelect->anyOfList[thisSelect->nAnyOf].beg = beg;
		thisSelect->anyOfList[thisSelect->nAnyOf].end = end;

		for(i = 0; i < MAX_OR_CODES; i++) thisSelect->anyOfList[thisSelect->nAnyOf].list[i] = 0;
	}
	thisSelect->nAnyOf++;
	while(tok = strtok(NULL, "\n")) {
		n = sscanf(tok, "%d - %d", &beg, &end);
		if(n == 1) {
			/* if n is 1, scan tok again to see if user separated codes with a space */
			n = sscanf(tok, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
					   &codeList[0], &codeList[1], &codeList[2], &codeList[3], &codeList[4], &codeList[5],
					   &codeList[6], &codeList[7], &codeList[8], &codeList[9], &codeList[10], &codeList[11],
					   &codeList[12], &codeList[13], &codeList[14], &codeList[15]);
			if(n == 1) {
				/* if n is still 1, the user only entered 1 code */
				thisSelect->anyOfList[thisSelect->nAnyOf].beg = codeList[0];
				thisSelect->anyOfList[thisSelect->nAnyOf].end = codeList[0];

				for(i = 0; i < MAX_OR_CODES; i++) thisSelect->anyOfList[thisSelect->nAnyOf].list[i] = 0;
			}
			else if(n > 2) {
				/* if n is greater than 2 the user entered a set of codes to be or'ed */
				for(i = 0; i < n; i++) thisSelect->anyOfList[thisSelect->nAnyOf].list[i] = codeList[i];

				thisSelect->anyOfList[thisSelect->nAnyOf].beg = 0;
				thisSelect->anyOfList[thisSelect->nAnyOf].end = 0;
			}
		}
		else if(n == 2) {	
			/* if n is 2, the user entered a range of codes separated by " - " */
			thisSelect->anyOfList[thisSelect->nAnyOf].beg = beg;
			thisSelect->anyOfList[thisSelect->nAnyOf].end = end;
			for(i = 0; i < MAX_OR_CODES; i++) thisSelect->anyOfList[thisSelect->nAnyOf].list[i] = 0;
		}
		thisSelect->nAnyOf++;
	}

	return( Pt_CONTINUE );
}

int clearSelections(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
	SELECTIONS *selections;
	
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	selections = getSelections();
	
	if(selections->selectSets) free(selections->selectSets);

	return( Pt_CONTINUE );
}

void initFirstSet(SELECTSET *thisSet)
{
	int i, j;
	
	thisSet->alignCode = 0;
	thisSet->alignOffsetTime = 0;

	thisSet->select.nAllOf = 0;
	thisSet->select.nNonOf = 0;
	thisSet->select.nAnyOf = 0;

	for(i = 0; i < ECODE_LIST_MAX; i++) {
		thisSet->select.allOfList[i] = 0;
		thisSet->select.nonOfList[i] = 0;
		thisSet->select.anyOfList[i].beg = 0;
		thisSet->select.anyOfList[i].end = 0;
		for(j = 0; j < MAX_OR_CODES; j++) thisSet->select.anyOfList[i].list[j] = 0;
	}

	return;
}

void initSubsequentSet(SELECTSET *thisSet, SELECTSET *lastSet)
{
	int i, j;

	thisSet->alignCode = lastSet->alignCode;
	thisSet->alignOffsetTime = lastSet->alignOffsetTime;

	thisSet->select.nAllOf = lastSet->select.nAllOf;
	thisSet->select.nNonOf = lastSet->select.nNonOf;
	thisSet->select.nAnyOf = lastSet->select.nAnyOf;

	for(i = 0; i < ECODE_LIST_MAX; i++) {
		thisSet->select.allOfList[i] = lastSet->select.allOfList[i];
		thisSet->select.nonOfList[i] = lastSet->select.nonOfList[i];
		thisSet->select.anyOfList[i].beg = lastSet->select.anyOfList[i].beg;
		thisSet->select.anyOfList[i].end = lastSet->select.anyOfList[i].end;
		for(j = 0; j < MAX_OR_CODES; j++) {
			thisSet->select.anyOfList[i].list[j] = lastSet->select.anyOfList[i].list[j];
		}
	}

	return;
}

void initSetWidgets(PtWidget_t *link_instance, SELECTSET *thisSet, int newSelects)
{
	PtWidget_t *widget;
	PtArg_t args[1];
	SELECT *thisSelect;
	int i, j;
	char string[1024];
	char code[32];

	thisSelect = &thisSet->select;
	
	/* set the value for the align code widget */
	widget = ApGetWidgetPtr(link_instance, ABN_alignCode);
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, thisSet->alignCode, 0);
	PtSetResources(widget, 1, args);

	/* set the value for the align offset widget */
	widget = ApGetWidgetPtr(link_instance, ABN_alignOffset);
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, thisSet->alignOffsetTime, 0);
	PtSetResources(widget, 1, args);

	/* set the values for the allOf widget */
	widget = ApGetWidgetPtr(link_instance, ABN_allOfCode);
	strcpy(string, "");
	for(i = 0; i < thisSelect->nAllOf; i++) {
		sprintf(code, "%d", thisSelect->allOfList[i]);
		strcat(string, code);
		strcat(string, "\n");
	}
	PtSetArg(&args[0], Pt_ARG_TEXT_STRING, &string, 0);
	PtSetResources(widget, 1, args);

	/* set the values for the nonOf widget */
	widget = ApGetWidgetPtr(link_instance, ABN_nonOfCode);
	strcpy(string, "");
	for(i = 0; i < thisSelect->nNonOf; i++) {
		sprintf(code, "%d", thisSelect->nonOfList[i]);
		strcat(string, code);
		strcat(string, "\n");
	}
	PtSetArg(&args[0], Pt_ARG_TEXT_STRING, &string, 0);
	PtSetResources(widget, 1, args);
	
	/* set the values for the anyOf widget */
	widget = ApGetWidgetPtr(link_instance, ABN_anyOfCode);
	strcpy(string, "");
	for(i = 0; i < thisSelect->nAnyOf; i++) {
		/* if the user entered a range of ecodes */
		if(thisSelect->anyOfList[i].beg != thisSelect->anyOfList[i].end) {
			sprintf(code, "%d - %d", thisSelect->anyOfList[i].beg, thisSelect->anyOfList[i].end);
			strcat(string, code);
			strcat(string, "\n");
		}
		/* if the user entered a single ecode */
		else if(thisSelect->anyOfList[i].beg) {
			sprintf(code, "%d", thisSelect->anyOfList[i].beg);
			strcat(string, code);
			strcat(string, "\n");
		}
		/* if the user entered a list of ecodes to be or'ed */
		else {
			j = 0;
			while(thisSelect->anyOfList[i].list[j]) {
				sprintf(code, "%d ", thisSelect->anyOfList[i].list[j]);
				strcat(string, code);
				j++;
			}
			strcat(string, "\n");
		}
	}
	PtSetArg(&args[0], Pt_ARG_TEXT_STRING, &string, 0);
	PtSetResources(widget, 1, args);

	return;
}

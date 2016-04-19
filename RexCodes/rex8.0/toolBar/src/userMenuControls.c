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

#define MENU_MAX 64

typedef struct userMenuStruct {
	PtWidget_t *menuWindow;
	PtWidget_t *valueWidgets[MENU_MAX];
	MENU *menuPtr;
} REX_MENU;

typedef union userFuncArgs {
	int intVal;
	float floatVal;
	char *stringVal;
} FUNC_ARG;

REX_MENU rexUserMenus[MENU_MAX];

int menuIndex = 0;

int initUserMenus( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[2];
	PtWidget_t *newItem;
	int i;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* create an item for each menu */
	i = 0;
	while(strlen(umenus[i].me_name)) {
		if(!strcmp(umenus[i].me_name, "separator")) {
			PtSetArg(&args[0], Pt_ARG_SEP_TYPE, Pt_ETCHED_IN, 0);
			newItem = PtCreateWidget(PtSeparator, NULL, 1, args);
		}
		else {
			PtSetArg(&args[0], Pt_ARG_TEXT_STRING, umenus[i].me_name, 0);
			PtSetArg(&args[1], Pt_ARG_TEXT_FONT, "Helvetical09ba", 0);
			newItem = PtCreateWidget(PtMenuButton, NULL, 2, args);
			PtAddCallback(newItem, Pt_CB_ACTIVATE, buildUserMenus, &umenus[i]);
		}
		i++;
	}

	return( Pt_CONTINUE );
}

int buildUserMenus(PtWidget_t *widget, void *client_data, PtCallbackInfo_t *cbinfo)
{
	/* eliminate 'unreferenced' warnings */
	widget = widget, client_data = client_data, cbinfo = cbinfo;

	/* attach the client_data to the callback struct */
 	cbinfo->cbdata = client_data;

	/* Create a user menu module */
	/* the initialization function initUserMenuWindow is called
	 * automatically whenever a userMenuWindow is created
	 */
 	ApCreateModule(ABM_userMenuWindow, widget, cbinfo);

	return( Pt_CONTINUE );
}

int closeMenuWindow( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PhWindowEvent_t *winEvent;
	int i, j;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get pointer to the window event structure */
	winEvent = (PhWindowEvent_t *)cbinfo->cbdata;

	/* check to see which type event evoked this callback */
	if(winEvent->event_f == Ph_WM_CLOSE) {
		for(i = 0; i < menuIndex; i++) {
			/* delete this window's entry in the rexUserMenus struct array */
			if(widget == rexUserMenus[i].menuWindow) {
				rexUserMenus[i].menuWindow = (PtWidget_t *)NULL;
				rexUserMenus[i].menuPtr = (MENU *)NULL;
				for(j = 0; j < MENU_MAX; j++) {
					rexUserMenus[i].valueWidgets[j] = (PtWidget_t *)NULL;
				}
				break;
			}
		}

		/* move all lower windows up one */
		for(; i < menuIndex - 1; i++) {
			rexUserMenus[i].menuWindow = rexUserMenus[i + 1].menuWindow;
			rexUserMenus[i].menuPtr = rexUserMenus[i + 1].menuPtr;
			for(j = 0; j < MENU_MAX; j++) {
				rexUserMenus[i].valueWidgets[j] = rexUserMenus[i + 1].valueWidgets[j];
			}
		}
		/* decrement the menu index */
		menuIndex--;
	}
	return( Pt_CONTINUE );
}

int initUserMenuWindow( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtWidget_t *userMenuWindow;
	PtWidget_t *arrayWidget;
	PtWidget_t *arrayLabelWidget;
	PtWidget_t *menuScrollArea;
	ApDBase_t *userMenusdbase;
	PhPoint_t loc;
	PtArg_t args[3];
	MENU *thisMenu;
	REX_MENU *thisMenuStruct;
	u_int tmpAddr;
	int menuErr;
	int y;
	int i;
	char label[64];
	char *value;

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	/* determine whether this function was called from the menu array index widget
	 * or from the user menu initialization function
	 */
	if(PtWidgetClass(link_instance) == PtWindow) {	/* called by initialization function */
		/* get pointer to the menu needed for this window */
		thisMenu = (MENU *)cbinfo->cbdata;

		/* save link_instance and menu pointer */
		rexUserMenus[menuIndex].menuWindow = link_instance;
		rexUserMenus[menuIndex].menuPtr = thisMenu;
		thisMenuStruct = &rexUserMenus[menuIndex];
		menuIndex++;

		/* if this menu doesn't access an array of structs
		 * disable the menu array index widget
		 */
		if(!thisMenu->me_accf) {
			PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_GHOST);
			PtSetArg(&args[1], Pt_ARG_TEXT_FLAGS, Pt_FALSE, Pt_EDITABLE);
		}
		else {
			PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_GHOST);
			PtSetArg(&args[1], Pt_ARG_TEXT_FLAGS, Pt_TRUE, Pt_EDITABLE);
		}
		arrayWidget = ApGetWidgetPtr(link_instance, ABN_userMenuArrayIndex);
		PtSetResources(arrayWidget, 2, args);
		arrayLabelWidget = ApGetWidgetPtr(link_instance, ABN_userMenuArrayLabel);
		PtSetResources(arrayLabelWidget, 1, args);

		userMenuWindow = link_instance;	/* set pointer to the window */
		menuScrollArea = ApGetWidgetPtr(userMenuWindow, ABN_userMenuScrollArea); /* get pointer to the scroll area widget */

		/* get the absolute position of the toolbar base widget,
		 * place the window 100 pixels below the base widget
		 * set the name of the dialog
		 */
		PtGetAbsPosition(ABW_base, &loc.x, &loc.y);
		loc.y += 100;
		PtSetArg(&args[0], Pt_ARG_POS, &loc, 0);
		PtSetArg(&args[1], Pt_ARG_WINDOW_TITLE, thisMenu->me_name, 0);
		PtSetResources(userMenuWindow, 2, args);

		/* open the user menu variables data base */
		userMenusdbase = ApOpenDBase(ABM_userMenuPictures);

		/* set the parent for the widgets */
		PtSetParentWidget(menuScrollArea);

		sprintf(label, "%d", 0);	/* argument string for the menu access function */
	}
	else {						/* called by menu array widget */
		/* get the index from the widget */
		PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, 0, 0);
		PtGetResources(link_instance, 1, args);
		
		sprintf(label, "%d", (int)args[0].value); /* argument string for menu access function */

		userMenuWindow = ApGetInstance(link_instance);/* get pointer to window holding this widget */
		menuScrollArea = ApGetWidgetPtr(userMenuWindow, ABN_userMenuScrollArea); /* get pointer to the scroll area widget */
		
		/* get the menu pointer */
		for(i = 0; i < MENU_MAX; i++) {
			if(userMenuWindow == rexUserMenus[i].menuWindow) {
				thisMenu = rexUserMenus[i].menuPtr;
				thisMenuStruct = &rexUserMenus[i];
				break;
			}
		}
		if(i == MENU_MAX) {
			PtAskQuestion(userMenuWindow, NULL,
						  "Menu window pointer not found", "Helvetical09ba",
						  "&Continue", NULL, NULL, 1);
			return( Pt_CONTINUE );
		}
	}

	/* call this menu's maf function if one exists */
	if(thisMenu->me_accf && (thisMenu->me_flag == ME_BEF)) {
		menuErr = (*thisMenu->me_accf)(thisMenu->me_flag, thisMenu, label, NULL);
		if(menuErr) {			/* if the maf returned an error */
			PtAskQuestion(userMenuWindow, NULL,
						  "Menu access function returned an error", "Helvetical09ba",
						  "&Abort", NULL, NULL, 1);
			return( Pt_CONTINUE );
		}
	}

	/* create or set the widget sets for the menu variables */
	y = 0;
	i = 0;
	while(strlen(thisMenu->me_vlp[i].vl_name)) {
		/* ignore submenus and action arguments */
		if(thisMenu->me_vlp[i].vl_type == ME_SUBMENU) continue;
		if(thisMenu->me_vlp[i].vl_type == ME_NVALACTARG) continue;

		/* offset the address of this menu entry's variable as necessary */
		tmpAddr = (u_int)thisMenu->me_vlp[i].vl_add;
		if(thisMenu->me_vlp[i].vl_flag & ME_LB) tmpAddr += (u_int)thisMenu->me_vlp[i].vl_basep;
		if(thisMenu->me_vlp[i].vl_flag & ME_ILB) tmpAddr += *(u_int *)thisMenu->me_vlp[i].vl_basep;
		if(thisMenu->me_vlp[i].vl_flag & ME_GB) tmpAddr += thisMenu->me_basep;
		if(thisMenu->me_vlp[i].vl_flag & ME_IGB) tmpAddr += *(u_int *)thisMenu->me_basep;

		/* convert this menu entry's value to an ascii string */
		value = menuVar2Ascii(&thisMenu->me_vlp[i], tmpAddr);

		/* set up the arguments this menu entry's value widget */
		PtSetArg(&args[0], Pt_ARG_USER_DATA, &thisMenu->me_vlp[i], sizeof(VLIST));
		PtSetArg(&args[1], Pt_ARG_TEXT_STRING, value, 0);

		if(PtWidgetClass(link_instance) == PtWindow) {
			/* create the label widget for this menu entry's variable */
			sprintf(label, "%d. %s", i, thisMenu->me_vlp[i].vl_name);
			PtSetArg(&args[2], Pt_ARG_TEXT_STRING, label, 0);
			ApCreateWidget(userMenusdbase, "userMenuItemLabel", 0, y, 1, &args[2]);

			/* create the value widget for this menu entry's variable */
			thisMenuStruct->valueWidgets[i] = ApCreateWidget(userMenusdbase, "userMenuTextValue", 180, y, 2, &args[0]);
		}
		else {
			/* set resources on the value widget for this menu entry's variable */
			PtSetResources(thisMenuStruct->valueWidgets[i], 2, &args[0]);
		}
		y += 30;
		i++;
		if(i == MENU_MAX) break;
	}

	/* set the maximum range of the scroll widget */
	PtSetArg(&args[0], Pt_ARG_SCROLL_AREA_MAX_Y, y, 0);
	PtSetResources(ABW_userMenuScrollArea, 1, &args[0]);

	if(PtWidgetClass(link_instance) == PtWindow) {
		ApCloseDBase(userMenusdbase);

		/* print this menu's help message if it has one */
		if(thisMenu->me_help && strlen(thisMenu->me_help)) {
			rxHelpMessage(thisMenu->me_help);
		}
	}
			
	return( Pt_CONTINUE );
}

int setUserMenuVal( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[2];
	PtWidget_t *userMenuWindow;
	PgColor_t *fillColor;
	MENU *thisMenu;
	VLIST *thisVlist;
	u_int tmpAddr;
	int i;
	char *value;
	
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	
	PtGetResource(widget, Pt_ARG_FILL_COLOR, &fillColor, 0);

	/* don't execute if the value is the same as before */
	if(*fillColor != Pg_RED) return( Pt_CONTINUE );

	PtSetResource(widget, Pt_ARG_COLOR, Pg_BLACK, 0);
	PtSetResource(widget, Pt_ARG_FILL_COLOR, Pg_WHITE, 0);
	
	/* get pointer to this widget's window */
	userMenuWindow = ApGetInstance(widget);

	/* get the menu pointer */
	for(i = 0; i < menuIndex; i++) {
		if(userMenuWindow == rexUserMenus[i].menuWindow) {
			thisMenu = rexUserMenus[i].menuPtr;
			break;
		}
	}
	if(i == menuIndex) {
		PtAskQuestion(ABW_base, NULL,
					  "Menu window pointer not found", "Helvetical09ba",
					  "&Continue", NULL, NULL, 1);
		return( Pt_CONTINUE );
	}

	/* get data and pointer to the vlist entry for this variable */
	PtSetArg(&args[0], Pt_ARG_USER_DATA, &thisVlist, 0);
	PtSetArg(&args[1], Pt_ARG_TEXT_STRING, &value, 0);
	PtGetResources(widget, 2, args);

	/* check vl_flag to see if the Address
	 * of the variable needs to be offset
	 */
	tmpAddr = (u_int)thisVlist->vl_add;
	if(thisVlist->vl_flag & ME_LB) tmpAddr += (u_int)thisVlist->vl_basep;
	if(thisVlist->vl_flag & ME_ILB) tmpAddr += *(u_int *)thisVlist->vl_basep;
	if(thisVlist->vl_flag & ME_GB) tmpAddr += thisMenu->me_basep;
	if(thisVlist->vl_flag & ME_IGB)	tmpAddr += *(u_int *)thisMenu->me_basep;

	/* convert the ascii value into the appropriate variable type
	 * and call any vaf's needed
	 */
	ascii2MenuVar(userMenuWindow, thisMenu, thisVlist, tmpAddr, value);
	return( Pt_CONTINUE );
}

int setBatchUserMenuVal( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtWidget_t *userMenuWindow;
	PtWidget_t *thisValueWidget;
	PtArg_t arg;
	PgColor_t *fillColor;
	REX_MENU *thisMenuStruct;
	MENU *thisMenu;
	VLIST *thisVlist;
	u_int tmpAddr;
	int i;
	int nVlist;
	int vlistIndx;
	char *argPtr;
	char *indxPtr;
	char argString[1024];

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	
	PtGetResource(widget, Pt_ARG_FILL_COLOR, &fillColor, 0);

	/* don't execute if the value is the same as before */
	if(*fillColor != Pg_RED) return( Pt_CONTINUE );

	PtSetResource(widget, Pt_ARG_COLOR, Pg_BLACK, 0);
	PtSetResource(widget, Pt_ARG_FILL_COLOR, Pg_WHITE, 0);

	/* get pointer to window holding this widget */
	userMenuWindow = ApGetInstance(widget);

	/* get pointer to this window's rex menu struct */
	for(i = 0; i < menuIndex; i++) {
		if(userMenuWindow == rexUserMenus[i].menuWindow) {
			thisMenuStruct = &rexUserMenus[i];
			break;
		}
	}
	if(i == menuIndex) {
		PtAskQuestion(userMenuWindow, NULL,
					  "Menu window pointer not found", "Helvetical09ba",
					  "&Continue", NULL, NULL, 1);
		return( Pt_CONTINUE );
	}
	thisMenu = thisMenuStruct->menuPtr;

	/* count the number of vlist elements in this menu's array */
	nVlist = 0;
	while(thisMenuStruct->valueWidgets[nVlist]) nVlist++;

	/* get argument string from widget */
	PtSetArg(&arg, Pt_ARG_TEXT_STRING, 0, 0);
	PtGetResources(widget, 1, &arg);
	argPtr = (char *)arg.value;

	/* copy the string to a local variable for strtok */
	strncpy(argString, argPtr, 1024);

	/* extract the first index and value from the argString */
	indxPtr = strtok(argString, " ");	/* string containing menu variable index */
	argPtr = strtok(NULL, " ");			/* string containing menu variable value*/

	/* if the user didn't enter a value, abort */
	if(!argPtr) {
		PtAskQuestion(userMenuWindow, NULL,
					  "First argument not found", "Helvetical09ba",
					  "&Abort", NULL, NULL, 1);
		/* clear the batch variable widget */
		PtSetArg(&arg, Pt_ARG_TEXT_STRING, "", 0);
		PtSetResources(widget, 1, &arg);
		return( Pt_CONTINUE );
	}

	/* convert index string to integer value */
	sscanf(indxPtr, "%d", &vlistIndx);

	/* if the index is out of range abort */
	if((vlistIndx < 0) || (vlistIndx >= nVlist)) {
		PtAskQuestion(userMenuWindow, NULL,
					  "Index out of range", "Helvetical09ba",
					  "&Abort", NULL, NULL, 1);
		/* clear the batch variable widget */
		PtSetArg(&arg, Pt_ARG_TEXT_STRING, "", 0);
		PtSetResources(widget, 1, &arg);
		return( Pt_CONTINUE );
	}
	
	/* get pointer to this menu variable struct */
	thisVlist = &thisMenu->me_vlp[vlistIndx];

	/* check vl_flag to see if the Address
	 * of the variable needs to be offset
	 */
	tmpAddr = (u_int)thisVlist->vl_add;
	if(thisVlist->vl_flag & ME_LB) tmpAddr += (u_int)thisVlist->vl_basep;
	if(thisVlist->vl_flag & ME_ILB) tmpAddr += *(u_int *)thisVlist->vl_basep;
	if(thisVlist->vl_flag & ME_GB) tmpAddr += thisMenu->me_basep;
	if(thisVlist->vl_flag & ME_IGB)	tmpAddr += *(u_int *)thisMenu->me_basep;

	/* convert the ascii value into the appropriate variable type
	 * and call any vaf's needed
	 */
	ascii2MenuVar(userMenuWindow, thisMenu, thisVlist, tmpAddr, argPtr);

	/* set the value of this variable's widget */
	thisValueWidget = thisMenuStruct->valueWidgets[vlistIndx];
	PtSetArg(&arg, Pt_ARG_TEXT_STRING, argPtr, 0);
	PtSetResources(thisValueWidget, 1, &arg);

	/* extract subsequent indices and values from the argString */
	while(indxPtr = strtok(NULL, " ")) {
		argPtr = strtok(NULL, " ");
		if(!argPtr) {
			PtAskQuestion(userMenuWindow, NULL,
						  "Argument not found", "Helvetical09ba",
						  "&Abort", NULL, NULL, 1);
			/* clear the batch variable widget */
			PtSetArg(&arg, Pt_ARG_TEXT_STRING, "", 0);
			PtSetResources(widget, 1, &arg);
			return( Pt_CONTINUE );
		}
		sscanf(indxPtr, "%d", &vlistIndx);
		if((vlistIndx < 0) || (vlistIndx >= nVlist)) {
			PtAskQuestion(userMenuWindow, NULL,
						  "Index out of range", "Helvetical09ba",
						  "&Abort", NULL, NULL, 1);
			/* clear the batch variable widget */
			PtSetArg(&arg, Pt_ARG_TEXT_STRING, "", 0);
			PtSetResources(widget, 1, &arg);
			return( Pt_CONTINUE );
		}

		thisVlist = &thisMenu->me_vlp[vlistIndx];

		/* check vl_flag to see if the Address
		 * of the variable needs to be offset
		 */
		tmpAddr = (u_int)thisVlist->vl_add;
		if(thisVlist->vl_flag & ME_LB) tmpAddr += (u_int)thisVlist->vl_basep;
		if(thisVlist->vl_flag & ME_ILB) tmpAddr += *(u_int *)thisVlist->vl_basep;
		if(thisVlist->vl_flag & ME_GB) tmpAddr += thisMenu->me_basep;
		if(thisVlist->vl_flag & ME_IGB)	tmpAddr += *(u_int *)thisMenu->me_basep;

		/* convert the ascii value into the appropriate variable type
		 * and call any vaf's needed
		 */
		ascii2MenuVar(userMenuWindow, thisMenu, thisVlist, tmpAddr, argPtr);

		/* set the value of this variable's widget */
		thisValueWidget = thisMenuStruct->valueWidgets[vlistIndx];
		PtSetArg(&arg, Pt_ARG_TEXT_STRING, argPtr, 0);
		PtSetResources(thisValueWidget, 1, &arg);
	}
	
	/* clear the batch variable widget */
	PtSetArg(&arg, Pt_ARG_TEXT_STRING, "", 0);
	PtSetResources(widget, 1, &arg);
	
	return( Pt_CONTINUE );
}

/* convert the menu variable to an ascii string */
char *menuVar2Ascii(VLIST *thisVlist, u_int tmpAddr)
{
	static char value[P_LNAME];

	/* convert this menu entry's variable to an ascii string */
	if(thisVlist->vl_type == ME_ACHAR) sprintf(value, "%c", *(char *)tmpAddr);
	else if(thisVlist->vl_type == ME_UDCHAR) sprintf(value, "%c", *(char *)tmpAddr);
	else if(thisVlist->vl_type == ME_STR) return((char *)tmpAddr);
	else if(thisVlist->vl_type == ME_OCT) sprintf(value, "%o", *(int *)tmpAddr);
	else if(thisVlist->vl_type == ME_DEC) sprintf(value, "%d", *(int *)tmpAddr);
	else if(thisVlist->vl_type == ME_HEX) sprintf(value, "%x", *(int *)tmpAddr);
	else if(thisVlist->vl_type == ME_LOCT) sprintf(value, "%lo",*(long *)tmpAddr);
	else if(thisVlist->vl_type == ME_LDEC) sprintf(value, "%d", *(long *)tmpAddr);
	else if(thisVlist->vl_type == ME_LHEX) sprintf(value, "%x", *(long *)tmpAddr);
	else if(thisVlist->vl_type == ME_NVALO) {
		if(*(int *)tmpAddr == NULLI) sprintf(value, "%s", "-");
		else sprintf(value, "%lo", *(int *)tmpAddr);
	}
	else if(thisVlist->vl_type == ME_NVALD) {
		if(*(int *)tmpAddr == NULLI) sprintf(value, "%s", "-");
		else sprintf(value, "%ld", *(int *)tmpAddr);
	}
	else if(thisVlist->vl_type == ME_NVALH) {
		if(*(int *)tmpAddr == NULLI) sprintf(value, "%s", "-");
		else sprintf(value, "%lx", *(int *)tmpAddr);
	}
	else if(thisVlist->vl_type == ME_NVALLO) {
		if(*(int *)tmpAddr == NULLI_L) sprintf(value, "%s", "-");
		else sprintf(value, "%lo", *(long *)tmpAddr);
	}
	else if(thisVlist->vl_type == ME_NVALLD) {
		if(*(int *)tmpAddr == NULLI_L) sprintf(value, "%s", "-");
		else sprintf(value, "%ld", *(long *)tmpAddr);
	}
	else if(thisVlist->vl_type == ME_NVALLH) {
		if(*(int *)tmpAddr == NULLI_L) sprintf(value, "%s", "-");
		else sprintf(value, "%lx", *(long *)tmpAddr);
	}
	else if(thisVlist->vl_type == ME_FLOAT) sprintf(value, "%f", *(float *)tmpAddr);

	return(value);
}

/* convert the ascii string entered in the widget to the appropriate variable */
void ascii2MenuVar(PtWidget_t *userMenuWindow, MENU *thisMenu, VLIST *thisVlist, u_int tmpAddr, char *value)
{
	union {
		char s[P_LNAME];
		char c;
		int i;
		long l;
		u_long u;
		float f;
	} tvadd;

	float tmpFloatVal;
	u_long tmpLongVal;
	int tmpIntVal;
	int longflag = 0;
	int floatflag = 0;
	int errvaf;
	char *rptr;
	char errmsg[512];

	/* convert the input string to the appropriate value */
	switch(thisVlist->vl_type) {
	case ME_ACHAR:
		if(isdigit(value[0])) {
			if(userMenuWindow) 
				PtAskQuestion(userMenuWindow, NULL,
							  "Numeric value entered in char variable", "Helvetica09ba",
							  "&Re-enter", NULL, NULL, 1);
			return;
		}
		tvadd.c = value[0];
		break;
	case ME_UDCHAR:
		tvadd.i = atoi(value);
		if((tvadd.i < 0) || (tvadd.i > 255)) {
			if(userMenuWindow) 
				PtAskQuestion(userMenuWindow, NULL,
							  "Invalid range for unsigned decimal char", "Helvetica09ba",
							  "&Re-enter", NULL, NULL, 1);
			return;
		}
		tvadd.i = tvadd.i &= 0377;
		break;
	case ME_STR:
		strncpy(tvadd.s, value, P_LNAME);
		break;
	case ME_OCT:
		longflag = 0;
		floatflag = 0;
	    tvadd.i = (int)strtol(value, &rptr, 8);
		if(rptr != (value + strlen(value))) {
			if(userMenuWindow) 
				PtAskQuestion(userMenuWindow, NULL,
							  "Invalid digit for octal value", "Helvetica09ba",
							  "&Re-enter", NULL, NULL, 1);
			return;
		}
		break;
	case ME_DEC:
		longflag = 0;
		floatflag = 0;
		tvadd.i = (int)strtol(value, &rptr, 10);
		if(rptr != (value + strlen(value))) {
			if(userMenuWindow) 
				PtAskQuestion(userMenuWindow, NULL,
							  "Invalid digit for decimal value", "Helvetica09ba",
							  "&Re-enter", NULL, NULL, 1);
			return;
		}
		break;
	case ME_HEX:
		longflag = 0;
		floatflag = 0;
		tvadd.i = (int)strtol(value, &rptr, 16);
		if(rptr != (value + strlen(value))) {
			if(userMenuWindow) 
				PtAskQuestion(userMenuWindow, NULL,
							  "Invalid digit for hexidecimal value", "Helvetica09ba",
							  "&Re-enter", NULL, NULL, 1);
			return;
		}
		break;
	case ME_LOCT:
		longflag = 1;
		floatflag = 0;
		tvadd.u = strtoul(value, &rptr, 8);
		if(rptr != (value + strlen(value))) {
			if(userMenuWindow) 
				PtAskQuestion(userMenuWindow, NULL,
							  "Invalid digit for long octal value", "Helvetica09ba",
							  "&Re-enter", NULL, NULL, 1);
			return;
		}
		break;
	case ME_LDEC:
		longflag = 1;
		floatflag = 0;
		tvadd.l = strtol(value, &rptr, 10);
		if(rptr != (value + strlen(value))) {
			if(userMenuWindow) 
				PtAskQuestion(userMenuWindow, NULL,
							  "Invalid digit for long decimal value", "Helvetica09ba",
							  "&Re-enter", NULL, NULL, 1);
			return;
		}
		break;
	case ME_LHEX:
		longflag = 1;
		floatflag = 0;
		tvadd.u = strtoul(value, &rptr, 16);
		if(rptr != (value + strlen(value))) {
			if(userMenuWindow) 
				PtAskQuestion(userMenuWindow, NULL,
							  "Invalid digit for long hexidecimal value", "Helvetica09ba",
							  "&Re-enter", NULL, NULL, 1);
			return;
		}
		break;
	case ME_NVALO:
		longflag = 0;
		floatflag = 0;
		if(!strcmp(value, "-")) tvadd.i = NULLI;
		else {
			tvadd.i = (int)strtol(value, &rptr, 8);
			if(rptr != (value + strlen(value))) {
				if(userMenuWindow) 
					PtAskQuestion(userMenuWindow, NULL,
								  "Invalid digit for octal value", "Helvetica09ba",
								  "&Re-enter", NULL, NULL, 1);
				return;
			}
		}
		break;
	case ME_NVALD:
		longflag = 0;
		floatflag = 0;
		if(!strcmp(value, "-")) tvadd.i = NULLI;
		else {
			tvadd.i = (int)strtol(value, &rptr, 10);
			if(rptr != (value + strlen(value))) {
				if(userMenuWindow) 
					PtAskQuestion(userMenuWindow, NULL,
								  "Invalid digit for decimal value", "Helvetica09ba",
								  "&Re-enter", NULL, NULL, 1);
				return;
			}
		}
		break;
	case ME_NVALH:
		longflag = 0;
		floatflag = 0;
		if(!strcmp(value, "-")) tvadd.i = NULLI;
		else {
			tvadd.i = (int)strtol(value, &rptr, 16);
			if(rptr != (value + strlen(value))) {
				if(userMenuWindow) 
					PtAskQuestion(userMenuWindow, NULL,
								  "Invalid digit for hexidecimal value", "Helvetica09ba",
								  "&Re-enter", NULL, NULL, 1);
				return;
			}
		}
		break;
	case ME_NVALLO:
		longflag = 1;
		floatflag = 0;
		if(!strcmp(value, "-")) tvadd.l = NULLI_L;
		else {
			tvadd.l = strtol(value, &rptr, 8);
			if(rptr != (value + strlen(value))) {
				if(userMenuWindow) 
					PtAskQuestion(userMenuWindow, NULL,
								  "Invalid digit for long octal value", "Helvetica09ba",
								  "&Re-enter", NULL, NULL, 1);
				return;
			}
		}
		break;
	case ME_NVALLD:
		longflag = 1;
		floatflag = 0;
		if(!strcmp(value, "-")) tvadd.l = NULLI_L;
		else {
			tvadd.l = strtol(value, &rptr, 10);
			if(rptr != (value + strlen(value))) {
				if(userMenuWindow) 
					PtAskQuestion(userMenuWindow, NULL,
								  "Invalid digit for long decimal value", "Helvetica09ba",
								  "&Re-enter", NULL, NULL, 1);
				return;
			}
		}
		break;
	case ME_NVALLH:
		longflag = 1;
		floatflag = 0;
		if(!strcmp(value, "-")) tvadd.u = NULLI_L;
		else {
			tvadd.l = strtol(value, &rptr, 16);
			if(rptr != (value + strlen(value))) {
				if(userMenuWindow) 
					PtAskQuestion(userMenuWindow, NULL,
								  "Invalid digit for long hexidecimal value", "Helvetica09ba",
								  "&Re-enter", NULL, NULL, 1);
				return;
			}
		}
		break;
	case ME_FLOAT:
		longflag = 0;
		floatflag = 1;
		tvadd.f = (float)strtod(value, &rptr);
		if(rptr != (value + strlen(value))) {
			if(userMenuWindow) 
				PtAskQuestion(userMenuWindow, NULL,
							  "Invalid digit for decimal value", "Helvetica09ba",
							  "&Re-enter", NULL, NULL, 1);
			return;
		}
		break;
	default:
		if(userMenuWindow) 
			PtAskQuestion(userMenuWindow, NULL,
						  "Undefined menu variable type", "Helvetica09ba",
						  "&Continue", NULL, NULL, 1);
		return;
		break;
	}

	/* call the variable pre access function if it is defined */
	if(thisVlist->vl_flag & ME_BEF) {
		errvaf = (*thisVlist->vl_accf)(ME_BEF, thisMenu, value, thisVlist, &tvadd);
		if(errvaf) {
			if(userMenuWindow) {
				sprintf(errmsg, "Error in variable access function for value %s variable %s in menu %s\n",
						value, thisVlist->vl_name, thisMenu->me_name);
				PtAskQuestion(userMenuWindow, NULL,
							  errmsg, "Helvetica09ba",
							  "&Continue", NULL, NULL, 1);
			}
			return;
		}
	}

	/* now change the variable */
	if((thisVlist->vl_type == ME_ACHAR) || (thisVlist->vl_type == ME_UDCHAR)) {
		*(char *)tmpAddr = tvadd.c;
	}
	else if(thisVlist->vl_type == ME_STR) {
		strncpy((char *)tmpAddr, tvadd.s, P_LNAME);
	}
	else if(longflag) {
		*(long *)tmpAddr = tvadd.l;
	}
	else if(floatflag) {
		*(float *)tmpAddr = tvadd.f;
	}
	else {
		*(int *)tmpAddr = tvadd.i;
	}

	/* call the variable post access function if it is defined */
	if(thisVlist->vl_flag & ME_AFT) {
		errvaf = (*thisVlist->vl_accf)(ME_AFT, thisMenu, value, thisVlist, &tvadd);
		if(errvaf) {
			if(userMenuWindow) {
				sprintf(errmsg, "Error in variable access function for value %s variable %s in menu %s\n",
						value, thisVlist->vl_name, thisMenu->me_name);
				PtAskQuestion(userMenuWindow, NULL,
							  errmsg, "Helvetica09ba",
							  "&Continue", NULL, NULL, 1);
			}
			return;
		}
	}
	return;
}

int initUserFunctionDialog( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	ApDBase_t *userFunctionsdbase;
	PhPoint_t loc;
	PtArg_t args[2];
	int i;
	int y;

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	/* get the absolute position of the base widget,
	 * place the dialog 100 pixels below the base widget
	 * set the name of the dialog
	 */
	PtGetAbsPosition(ABW_base, &loc.x, &loc.y);
	loc.y += 100;
	PtSetArg(&args[0], Pt_ARG_POS, &loc, 0);
	PtSetArg(&args[1], Pt_ARG_WINDOW_TITLE, cbinfo->cbdata, 0);
	PtSetResources(link_instance, 2, args);

	/* open the user functions data base */
	userFunctionsdbase = ApOpenDBase(ABM_userFunctionPictures);

	/* set the parent for the widgets */
	PtSetParentWidget(ABW_userFunctionScrollArea);

	/* create the widget sets for the user functions */
	y = 0;
	i = 0;
	while(strlen(ufuncs[i].n_name)) {
		/* create the label widget for this variable */
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, ufuncs[i].n_name, 0);
		ApCreateWidget(userFunctionsdbase, "userFunctionLabel", 5, y, 1, &args[0]);

		/* create the left parenthesis widget for this variable */
		ApCreateWidget(userFunctionsdbase, "userFunctionLeftParen", 186, y - 5, 0, NULL);

		/* create the argument text widet for this variable */
		PtSetArg(&args[0], Pt_ARG_USER_DATA, &i, sizeof(int));
		ApCreateWidget(userFunctionsdbase, "userFunctionArgumentText", 200, y, 1, &args[0]);

		/* create the right parenthesis widget for this variable */
		ApCreateWidget(userFunctionsdbase, "userFunctionRightParen", 411, y - 5, 0, NULL);

		y += 30;
		i++;
	}

	/* set the maximum range of the scroll widget */
	PtSetArg(&args[0], Pt_ARG_SCROLL_AREA_MAX_Y, y, 0);
	PtSetResources(ABW_userFunctionScrollArea, 1, &args[0]);

	/* close the widget data base */
	ApCloseDBase(userFunctionsdbase);

	return( Pt_CONTINUE );
}

int callUserFunction( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[2];
	PgColor_t *fillColor;
	USER_FUNC *thisFunc;
	FUNC_ARG argVals[16];
	int (*funcPtr)();		/* address of function */
	int nArgs;
	int nFrmts;
	int *funcIndx;
	int i;
	char *argPtr;
	char *vrbPtr;
	char *frmtPtr;
	char argStr[1024];
	char formatStr[1024];
	char fArgs[16][32];
	char fFormats[16][32];

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	
	PtGetResource(widget, Pt_ARG_FILL_COLOR, &fillColor, 0);

	/* don't execute if the value is the same as before */
	if(*fillColor != Pg_RED) return( Pt_CONTINUE );
	
	PtSetResource(widget, Pt_ARG_COLOR, Pg_BLACK, 0);
	PtSetResource(widget, Pt_ARG_FILL_COLOR, Pg_WHITE, 0);

	/* get the function name and argument string from the widget */
	PtSetArg(&args[0], Pt_ARG_USER_DATA, &funcIndx, 0);
	PtSetArg(&args[1], Pt_ARG_TEXT_STRING, &argPtr, 0);
	PtGetResources(widget, 2, args);

	/* get pointer to this user function struct */
	thisFunc = &ufuncs[*funcIndx];

	/* get pointer to the function */
	funcPtr = thisFunc->n_ptr;

	/* copy the format string to a local variable for strtok */
	if(strlen(thisFunc->formatString)) {
		strncpy(formatStr, thisFunc->formatString, 1024);
	}
	else memset(formatStr, 0, 1024);

	/* initialize the number of formats */
	nFrmts = 0;

	/* extract the first format from the formatStr */
	frmtPtr = strtok(formatStr, " ");
	if(frmtPtr) {
		strcpy(fFormats[nFrmts], frmtPtr);
		nFrmts++;
	}

	/* if the first format string is "void" set the number of formats to 0
	 * and don't try to extract any more format strings
	 */
	if(!strcmp(fFormats[nFrmts - 1], "void")) {
		nFrmts--;	/* make nFrmts equal 0 */
	}

	/* extract any remaining formats from the formatStr */
	else {
		while(frmtPtr = strtok(NULL, " ")) {
			strcpy(fFormats[nFrmts], frmtPtr);
			nFrmts++;
			if(nFrmts > 15) break;
		}
	}
	
	/* copy the argument string retrieved from the widget
	 * to a local variable for strtok
	 */
	strncpy(argStr, argPtr, 1024);

	/* if there is an argument string but no format string,
	 * call the function with vstr, astr format
	 * and return.
	 * need to do this here so that argPtr can point to a string
	 * of a variable number of arguments.
	 */
	if(!nFrmts && strlen(argStr)) {
		vrbPtr = strtok(argStr, " ");
		argPtr = strtok(NULL, "\t");
		(*funcPtr)(vrbPtr, argPtr);
		return( Pt_CONTINUE );
	}

	/* initialize number of arguments */
	nArgs = 0;

	/* extract the first argument from the argString */
	argPtr = strtok(argStr, " ");
	if(argPtr) {
		strcpy(fArgs[nArgs], argPtr);
		nArgs++;
	}

	/* extract any remaining arguments from the argString */
	while(argPtr = strtok(NULL, " ")) {
		strcpy(fArgs[nArgs], argPtr);
		nArgs++;
		if(nArgs > 15) break;
	}

	/* if the number of arguments doesn't match the number of format strings
	 * print error and return
	 */
	if(nArgs != nFrmts) {
		PtAskQuestion(ABW_userFunctionDialog, NULL,
					  "Error: invalid number of arguments entered", "Helvetical09ba",
					  "&Continue", NULL, NULL, 1);
		return( Pt_CONTINUE );
	}

	/* convert any arguments from strings to the appropriate type */
	for(i = 0; i < nArgs; i++) {
		if(!strcmp("%d", fFormats[i])) {
			sscanf(fArgs[i], fFormats[i], &argVals[i].intVal);
		}
		else if(!strcmp("%f", fFormats[i])) {
			sscanf(fArgs[i], fFormats[i], &argVals[i].floatVal);
		}
		else if(!strcmp("%s", fFormats[i])) {
			argVals[i].stringVal = fArgs[i];
		}
	}

	/* call the function with the appropriate arguments */
	switch(nArgs) {
	case 0:
		(*funcPtr)();
		break;
	case 1:
		(*funcPtr)(argVals[0]);
		break;
	case 2:
		(*funcPtr)(argVals[0], argVals[1]);
		break;
	case 3:
		(*funcPtr)(argVals[0], argVals[1], argVals[2]);
		break;
	case 4:
		(*funcPtr)(argVals[0], argVals[1], argVals[2], argVals[3]);
		break;
	case 5:
		(*funcPtr)(argVals[0], argVals[1], argVals[2], argVals[3], argVals[4]);
		break;
	case 6:
		(*funcPtr)(argVals[0], argVals[1], argVals[2], argVals[3], argVals[4], argVals[5]);
		break;
	case 7:
		(*funcPtr)(argVals[0], argVals[1], argVals[2], argVals[3], argVals[4], argVals[5], argVals[6]);
		break;
	case 8:
		(*funcPtr)(argVals[0], argVals[1], argVals[2], argVals[3], argVals[4], argVals[5], argVals[6], argVals[7]);
		break;
	case 9:
		(*funcPtr)(argVals[0], argVals[1], argVals[2], argVals[3], argVals[4], argVals[5], argVals[6], argVals[7],
				   argVals[8]);
		break;
	case 10:
		(*funcPtr)(argVals[0], argVals[1], argVals[2], argVals[3], argVals[4], argVals[5], argVals[6], argVals[7],
				   argVals[8], argVals[9]);
		break;
	case 11:
		(*funcPtr)(argVals[0], argVals[1], argVals[2], argVals[3], argVals[4], argVals[5], argVals[6], argVals[7],
				   argVals[8], argVals[9], argVals[10]);
		break;
	case 12:
		(*funcPtr)(argVals[0], argVals[1], argVals[2], argVals[3], argVals[4], argVals[5], argVals[6], argVals[7],
				   argVals[8], argVals[9], argVals[10], argVals[11]);
		break;
	case 13:
		(*funcPtr)(argVals[0], argVals[1], argVals[2], argVals[3], argVals[4], argVals[5], argVals[6], argVals[7],
				   argVals[8], argVals[9], argVals[10], argVals[11], argVals[12]);
		break;
	case 14:
		(*funcPtr)(argVals[0], argVals[1], argVals[2], argVals[3], argVals[4], argVals[5], argVals[6], argVals[7],
				   argVals[8], argVals[9], argVals[10], argVals[11], argVals[12], argVals[13]);
		break;
	case 15:
		(*funcPtr)(argVals[0], argVals[1], argVals[2], argVals[3], argVals[4], argVals[5], argVals[6], argVals[7],
				   argVals[8], argVals[9], argVals[10], argVals[11], argVals[12], argVals[13], argVals[14]);
		break;
	case 16:
		(*funcPtr)(argVals[0], argVals[1], argVals[2], argVals[3], argVals[4], argVals[5], argVals[6], argVals[7],
				   argVals[8], argVals[9], argVals[10], argVals[11], argVals[12], argVals[13], argVals[14], argVals[15]);
		break;
	}

	return( Pt_CONTINUE );
}

int notifyUserMenuVal( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	
	PtSetResource(widget, Pt_ARG_COLOR, Pg_WHITE, 0);
	PtSetResource(widget, Pt_ARG_FILL_COLOR, Pg_RED, 0);
	
	return( Pt_CONTINUE );
}

int notifyBatchUserMenuVal( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	PtSetResource(widget, Pt_ARG_COLOR, Pg_WHITE, 0);
	PtSetResource(widget, Pt_ARG_FILL_COLOR, Pg_RED, 0);

	return( Pt_CONTINUE );
}

int notifyUserFunction( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	
	PtSetResource(widget, Pt_ARG_COLOR, Pg_WHITE, 0);
	PtSetResource(widget, Pt_ARG_FILL_COLOR, Pg_RED, 0);

	return( Pt_CONTINUE );
}


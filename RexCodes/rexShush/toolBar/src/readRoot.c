/*
 *-----------------------------------------------------------------------*
 * NOTICE:  This code was developed by the US Government.  The original
 * versions, REX 1.0-3.12, were developed for the pdp11 architecture and
 * distributed without restrictions.  This version, REX 4.0, is a port of
 * the original version to the Intel 80x86 architecture.  This version is
 * distributed only under license agreement from the National Institutes 
 * of Health, Laboratory of Sensorimotor Research, Bldg 10 Rm 10C101, 
 * 9000 Rockville Pike, Bethesda, MD, 20892, (301) 496-9375.
 *-----------------------------------------------------------------------*
 */

/*
 * Photon Int interprocess communication.
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>

/* Toolkit headers */
#include <Ph.h>
#include <Pt.h>
#include <Ap.h>

/* Local headers */
#include "ph_toolBar.h"
#include "abimport.h"
#include "proto.h"

char errmsg[256];

int readRoot(char *fileName)
{
	FILE *rootIn;
	MENU *pMenu;
	STATE *pState;
	struct stat statBuf;
	int fileStat;
	int menuFlag;
	char *rootFile;
	char *linePtr;

	/* stat the root file to see if it exists and how bit it is */
	fileStat = stat(fileName, &statBuf);

	if(fileStat || !statBuf.st_size) {
		sprintf(errmsg, "%s: readRoot(): Cannot stat root file", myptp->p_name);
		rxerr(errmsg);
		return(-1);
	}

	/* try to open the root file */
	if((rootIn = fopen(fileName, "r")) == NULL) {
		sprintf(errmsg, "%s: readRoot(): Cannot open root file for read", myptp->p_name);
		rxerr(errmsg);
		return(-1);
	}

	/* try allocate space to hold the root file */
	rootFile = (char *)malloc((statBuf.st_size + 10) * sizeof(char));
	if(!rootFile) {
		sprintf(errmsg, "%s: readRoot(): Cannot malloc space for root file", myptp->p_name);
		rxerr(errmsg);
		return(-1);
	}

	/* initialize the root file buffer */
	memset(rootFile, '\0', (statBuf.st_size + 10));

	/* read in the root file */
	fread(rootFile, sizeof(char), statBuf.st_size, rootIn);

	/* get the first line from the root file */
	linePtr = rexlinetok(rootFile);

	/* enter loop to get the rest of the lines in the root file */
	pMenu = (MENU *)NULL;
	while(linePtr) {
		if(linePtr[0] == '~') {		/* process comment lines */
			if(strstr(linePtr, "Menu")) menuFlag = 1;
			else if(strstr(linePtr, "State")) menuFlag = 2;
			else if(strstr(linePtr, "submenu")) {
				/* this comment line indicates the end of a submenu,
				 * call parseVlist with null arguments so it will return
				 * a pointer to the previous level menu
				 */
				pMenu = parseVlist((char *)NULL, (MENU *)NULL);
				menuFlag = 0;
			}
			else {
				/* this is an empty comment line that separates menu and state entries */
				pState = (STATE *)NULL;
				pMenu = (MENU *)NULL;
			}
		}
		else {		/* process non-comment lines */
			if(menuFlag == 1) {				   		/* if this line is identifying a menu */
				pState = (STATE *)NULL;				/* pState pointer to NULL */

				pMenu = parseMenuName(linePtr);		/* pointer to the menu */

				menuFlag = 0;						/* the next line won't be a menu declaration */
			}
			else if(menuFlag == 2) {				/* if this line is identifying a state */
				pMenu = (MENU *)NULL;				/* set pMenu pointers to NULL */

				if(pState) {
					parseState(linePtr, pState);
				}
				else if(strcmp(linePtr, "set statelist")) {		/* if lineptr is NOT "set statelist" */
					pState = parseStateName(linePtr);			/* get a pointer to this state */
				}
			}
			else if(pMenu) {
				pMenu = parseVlist(linePtr, pMenu);
			}
		}

		/* get the next line from the root file */
		linePtr = rexlinetok((char *)NULL);
	}
	
	/* close the root file and free the in core memory */
	if(rootIn) fclose(rootIn);
	if(rootFile) free(rootFile);

	return(0);
}

char *rexlinetok(char *string)
{
	static char returnString[128];
	static char *currentPosition;
	char *start;

	/* if a string is passed in, initialize the currenPosition and start pointers */
	if(string) {
		currentPosition = string;
		start = currentPosition;
	}

	/* if NULL is passed in set start pointer to 1 after currentPosition */
	else start = currentPosition + 1;

	/* scan the string until a newline or NULL is found */
	while(*++currentPosition != '\n') {
		if(!*currentPosition) return((char *)NULL);
	}

	/* change the newline or NULL to NULL */
	*currentPosition = (char)NULL;

	/* if the string from start to currenposition is not NULL, return the string */
	if(strlen(start)) {
		strcpy(returnString, start);
		*currentPosition = '\n';	/* reset the newline */
		return(returnString);
	}

	/* otherwise return a newline */
	else {
		strcpy(returnString, "\n");
		*currentPosition = '\n';
		return(returnString);
	}
}

MENU *parseMenuName(char *pStr)
{
	PROCTBL_P p;
	MENU *pMenu;
	int menuErr;
	int i;
	char *pMenuName;
	char *pProcName;
	char *pIndex;
	char localString[128];

	pMenuName = (char *)NULL;
	pProcName = (char *)NULL;
	pIndex = (char *)NULL;

	/* make local copy of pStr for strtok */
	strcpy(localString, pStr);

	/* first check for a declaration of the form "menuname.process [argstr]" */
	pMenuName = strtok(localString, ".");

	/* if pMenuName is null, send error and return */
	if(!pMenuName) {
		sprintf(errmsg, "%s: parseMenuName() error in getting menu from string %s",
				myptp->p_name, pStr);
		rxerr(errmsg);
		return((MENU *)NULL);
	}

	/* if the length of pMenuName is less than pStr,
	 * then the menu name has a process name imbedded in it 
	 */
	if(strlen(pMenuName) < strlen(pStr)) {
		/* extract pointer to the process name */
		pProcName = strtok(NULL, " ");

		/* extract pointer to the menu index string */
		pIndex = strtok(NULL, " ");
	}
	else {
		/* recopy pStr to localString for strtok */
		strcpy(localString, pStr);

		/* extract pointer to the menu name */
		pMenuName = strtok(localString, " ");

		/* extract pointer to the menu index string */
		pIndex = strtok(NULL, " ");
	}

	/* if there was a process name imbedded the menu name
	 * check to see if this is the same as the current process
	 */
	if(pProcName) {
		if(strcmp(pProcName, myptp->p_name)) {
			sprintf(errmsg, "%s: parseMenuName() warning: menu is from process %s",
					myptp->p_name, pProcName);
			rxerr(errmsg);
		}
	}

	/* get pointer to the menu struct */
	/* first check the system menus */
	pMenu = (MENU *)NULL;
	i = 0;
	while(strlen(menus[i].me_name)) {
		if(!strcmp(menus[i].me_name, pMenuName)) {
			pMenu = &menus[i];
			break;
		}
		i++;
	}

	/* if the menu was not found in the system menus
	 * check the user menus
	 */
	if(!pMenu) {
		i = 0;
		while(strlen(umenus[i].me_name)) {
			if(!strcmp(umenus[i].me_name, pMenuName)) {
				pMenu = &umenus[i];
				break;
			}
			i++;
		}
	}

	/* if the menu still was not found,
	 * print error and return NULL
	 */
	if(!pMenu) {
		sprintf(errmsg, "%s: parseMenuName() Error: couldn't find menu %s",
				myptp->p_name, pMenuName);
		rxerr(errmsg);
		return((MENU *)NULL);
	}

	/* call the menu's access function if it has one */
	if(pMenu->me_accf) {
		menuErr = (*pMenu->me_accf)(pMenu->me_flag, pMenu, pIndex, NULL);

		/* if the access function returned an error
		 * post a notice and return a NULL
		 */
		if(menuErr) {
			sprintf(errmsg, "%s: parseMenuName() menu %s access function returned an error",
					myptp->p_name, pMenuName);
			rxerr(errmsg);
			return((MENU *)NULL);
		}
	}

	/* return a pointer to the menu */
	return(pMenu);
}

MENU *parseVlist(char *pStr, MENU *pMenu)
{
	static MENU *lastMenu;
	MENU *thisMenu;
	VLIST *thisVlist;
	u_int tmpAddr;
	int menuErr;
	int i;
	char *pVarName;
	char *pValue;
	char localString[128];

	/* if parseVlist was called from an "End submenu" comment line
	 * return a pointer to the previous menu
	 */
	if(!pStr && !pMenu) {
		return(lastMenu);
	}

	/* make a local copy of pStr for strtok */
	strcpy(localString, pStr);

	/* get pointer to the variable name */
	pVarName = strtok(localString, " \t");

	/* if pVarName is null send error and return */
	if(!pVarName) {
		sprintf(errmsg, "%s: parseVlist() error in getting variable from string %s",
				myptp->p_name, pStr);
		rxerr(errmsg);
		return(pMenu);
	}

	/* get pointer to this element of the variable list */
	thisVlist = (VLIST *)NULL;
	i = 0;
	while(strlen(pMenu->me_vlp[i].vl_name)) {
		if(strstr(pMenu->me_vlp[i].vl_name, pVarName)) {
			thisVlist = &pMenu->me_vlp[i];
			break;
		}
		i++;
	}

	/* if the variable was not found print warning */
	if(!thisVlist) {
		sprintf(errmsg, "%s: parseVlist() couldn't find variable %s in menu %s",
				myptp->p_name, pVarName, pMenu->me_name);
		rxerr(errmsg);
		return(pMenu);
	}

	/* if this variable points to a submenu
	 * return the pointer to that submenu
	 * to be used in future calls to parseVlist
	 */
	if(thisVlist->vl_type == ME_SUBMENU) {
		lastMenu = pMenu;		/* save pointer to current menu */
		thisMenu = thisVlist->vl_add;	/* get pointer to submenu */

		/* call the menu's access function if it has one */
		if(thisMenu->me_accf) {
			menuErr = (*thisMenu->me_accf)(thisMenu->me_flag, thisMenu, &thisVlist->vl_flag, lastMenu);

			/* if the access function returned an error
			 * post a notice and return a NULL
			 */
			if(menuErr) {
				sprintf(errmsg, "%s: parseVlist() menu %s access function returned an error",
						myptp->p_name, thisMenu->me_name);
				rxerr(errmsg);
				return((MENU *)NULL);
			}
		}
		return(thisMenu);
	}
	else {
		thisMenu = pMenu;
	}

	/* get pointer to the value string */
	pValue = strtok(NULL, " \t");

	/* if pVarName is null send error and return */
	if(!pValue) {
		sprintf(errmsg, "%s: parseVlist() error in getting value from string %s",
				myptp->p_name, pStr);
		rxerr(errmsg);
		return(thisMenu);
	}

	/* check the vl_flag to see if the address of the variable
	 * needs to be offset
	 */
	tmpAddr = (u_int)thisVlist->vl_add;
	if(thisVlist->vl_flag & ME_LB) tmpAddr += (u_int)thisVlist->vl_basep;
	if(thisVlist->vl_flag & ME_ILB) tmpAddr += *(u_int *)thisVlist->vl_basep;
	if(thisVlist->vl_flag & ME_GB) tmpAddr += thisMenu->me_basep;
	if(thisVlist->vl_flag & ME_IGB)	tmpAddr += *(u_int *)thisMenu->me_basep;

	/* convert the ascii value into the appropriate variable type
	 * and call any vaf's needed
	 */
	ascii2MenuVar((PtWidget_t *)NULL, thisMenu, thisVlist, tmpAddr, pValue);
	
	return(thisMenu);
}

STATE *parseStateName(char *pStr)
{
	STATE *pState;
	int i;

	i = 0;
	pState = (STATE *)NULL;

	while(snames[i]) {
		if(!strcmp(pStr, snames[i]->statename)) pState = snames[i];
		i++;
	}

	if(!pState) {
		sprintf(errmsg, "%s: parseStateName() can't find state %s",
				myptp->p_name, pStr);
		rxerr(errmsg);
	}

	return(pState);
}

void parseState(char *pStr, STATE *pState)
{
	char *pArg;
	char *pVal;
	char localString[128];

	/* make local copy of pStr for strtok */
	strcpy(localString, pStr);

	/* get the first argument from the input string */
	pArg = strtok(localString, " \t");	/* name of the state structure element */
	/* now parse the rest of the input string */
	while(pArg) {
		pVal = strtok(NULL, " \t");			/* value to assign to structure element */
		setStateArgument(pState, pArg, pVal);	/* set the argument */

		pArg = strtok(NULL, " \t");		/* get the name of the next structure element */
	}
	return;
}

void setStateArgument(STATE *pState, char *pArg, char *pVal)
{
	if(!strcmp(pArg, "preset")) sscanf(pVal, "%d", &pState->preset);
	else if(!strcmp(pArg, "random")) sscanf(pVal, "%d", &pState->random);
	else if(!strcmp(pArg, "type_flag_rl")) sscanf(pVal, "%d", &pState->rlflag);
	else if(!strcmp(pArg, "value_rl")) sscanf(pVal, "%d", &pState->rlval);
	else if(!strcmp(pArg, "a_arg")) sscanf(pVal, "%x", &pState->device.act_code);
	else if(!strcmp(pArg, "b_arg")) sscanf(pVal, "%x", &pState->device.act_2code);
	else if(!strcmp(pArg, "c_arg")) sscanf(pVal, "%x", &pState->device.act_3code);
	else if(!strcmp(pArg, "d_arg")) sscanf(pVal, "%x", &pState->device.act_4code);
	else if(!strcmp(pArg, "e_arg")) sscanf(pVal, "%x", &pState->device.act_5code);
	else if(!strcmp(pArg, "f_arg")) sscanf(pVal, "%x", &pState->device.act_6code);
	else if(!strcmp(pArg, "g_arg")) sscanf(pVal, "%x", &pState->device.act_7code);
	else if(!strcmp(pArg, "h_arg")) sscanf(pVal, "%x", &pState->device.act_8code);
	else if(!strcmp(pArg, "i_arg")) sscanf(pVal, "%x", &pState->device.act_9code);
	else if(!strcmp(pArg, "j_arg")) sscanf(pVal, "%x", &pState->device.act_10code);
}

/*
 * Get pointer to proc table entry.
 */
PROCTBL_P getptp(char *name)
{
	PROCTBL_P p;

	/* search process table for process named 'name' */
	for(p = &i_b->ptbl[0]; p < &i_b->ptbl[P_NPROC]; p++) {
		if(p->p_id && !strcmp(name, p->p_name)) break;
	}

	/* if process table pointer points to the end of the process table array */
	if(p == &i_b->ptbl[P_NPROC]) {
		/*
		 * Check for ".int", the default for the current
		 * int process.
		 */
		if(!strcmp(name, "int") && (i_b->ph_int_pi >= 0)) {
			p = &i_b->ptbl[i_b->ph_int_pi];
		}
		else p = NP;
	}
	return(p);
}

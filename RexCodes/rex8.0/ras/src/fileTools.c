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

char rootDir[128] = { 0 };
char errmsg[64];

int initRootSave( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[2];
	int fileFlag;
	char wd[PATH_MAX + 2];

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	/* get the current working directory */
	getcwd(wd, PATH_MAX+1);

	if(!strlen(rootDir)) {
		strcpy(rootDir, wd);
		strcat(rootDir, "/rt");
	}

	PtSetArg(&args[0], Pt_ARG_FS_ROOT_DIR, rootDir, 0);
	PtSetResources(ABW_fileSelFileList, 1, args);

	PtSetArg(&args[0], Pt_ARG_TEXT_STRING, rootDir, 0);
	PtSetResources(ABW_fileSelDirText, 1, args);
	PtSetResources(ABW_fileSelFileName, 1, args);

	/* set the user data to indicate a root file write */
	fileFlag = ROOT_FILE_WRITE;
	PtSetArg(&args[0], Pt_ARG_USER_DATA, &fileFlag, sizeof(int));
	PtSetResources(ABW_fileSelFileName, 1, args);
	PtSetArg(&args[1], Pt_ARG_TEXT_STRING, "Write File", 0);
	PtSetResources(ABW_fileSelOpenBtn, 2, args);

	return( Pt_CONTINUE );
}

int initRootRead( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[2];
	int fileFlag;
	char wd[PATH_MAX + 2];

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	/* get the current working directory */
	getcwd(wd, PATH_MAX+1);

	if(!strlen(rootDir)) {
		strcpy(rootDir, wd);
		strcat(rootDir, "/rt");
	}
	PtSetArg(&args[0], Pt_ARG_FS_ROOT_DIR, rootDir, 0);
	PtSetResources(ABW_fileSelFileList, 1, args);

	PtSetArg(&args[0], Pt_ARG_TEXT_STRING, rootDir, 0);
	PtSetResources(ABW_fileSelDirText, 1, args);
	PtSetResources(ABW_fileSelFileName, 1, args);

	/* set the user data to indicate a root file read */
	fileFlag = ROOT_FILE_READ;
	PtSetArg(&args[0], Pt_ARG_USER_DATA, &fileFlag, sizeof(int));
	PtSetResources(ABW_fileSelFileName, 1, args);
	PtSetArg(&args[1], Pt_ARG_TEXT_STRING, "Open File", 0);
	PtSetResources(ABW_fileSelOpenBtn, 2, args);

	return( Pt_CONTINUE );
}

int initRasterSave( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[2];
	int fileFlag;
	char dir[80];

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	strcpy(dir, "/");
	PtSetArg(&args[0], Pt_ARG_FS_ROOT_DIR, dir, 0);
	PtSetResources(ABW_fileSelFileList, 1, args);

	PtSetArg(&args[0], Pt_ARG_TEXT_STRING, dir, 0);
	PtSetResources(ABW_fileSelDirText, 1, args);
	PtSetResources(ABW_fileSelFileName, 1, args);

	/* set the user data to indicate a root file write */
	fileFlag = RASTER_FILE_SAVE;
	PtSetArg(&args[0], Pt_ARG_USER_DATA, &fileFlag, sizeof(int));
	PtSetResources(ABW_fileSelFileName, 1, args);
	PtSetArg(&args[1], Pt_ARG_TEXT_STRING, "Save Raster", 0);
	PtSetResources(ABW_fileSelOpenBtn, 2, args);

	return( Pt_CONTINUE );
}

int setDir( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[1];
	char *string;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the directory */
	PtSetArg(&args[0], Pt_ARG_TEXT_STRING, &string, 0);
	PtGetResources(widget, 1, args);

	/* set the directory part of the file name */
	PtSetArg(&args[0], Pt_ARG_TEXT_STRING, string, 0);
	PtSetResources(ABW_fileSelFileName, 1, args);

	/* set the root directory of the file selection widget */
	PtSetArg(&args[0], Pt_ARG_FS_ROOT_DIR, string, 0);
	PtSetResources(ABW_fileSelFileList, 1, args);

	/* save the new directory */
	strcpy(rootDir, string);

	return( Pt_CONTINUE );
}

int getFileName( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[1];
	PtFileSelItem_t *fileName;
	char file[80];

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	// return immediately on mouse button press
	if(cbinfo->reason_subtype) return(Pt_CONTINUE);

	// process the file name on the mouse button release
	fileName = PtFSGetCurrent(widget);
	if(!fileName) strcpy(file, "invalid file name");
	else strcpy(file, fileName->fullpath);

	// set the text value of the file name text widget
	PtSetArg(&args[0], Pt_ARG_TEXT_STRING, &file, 0);
	PtSetResources(ABW_fileSelFileName, 1, args);

	return( Pt_CONTINUE );
}

int openFile( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[1];
	int *fileFlag;
	char *fileName;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the type of file to open */
	PtSetArg(&args[0], Pt_ARG_USER_DATA, &fileFlag, sizeof(int));
	PtGetResources(widget, 1, args);

	/* get the file name from the text widget */
	PtSetArg(&args[0], Pt_ARG_TEXT_STRING, &fileName, 0);
	PtGetResources(ABW_fileSelFileName, 1, args);

	switch(*fileFlag) {
	case ROOT_FILE_READ:
		readRoot(fileName);

		/* save the name of the root file in the proc table for rex session */
		strncpy(myptp->p_root, fileName, P_LROOTNAME);
		break;
	case ROOT_FILE_WRITE:
		writeRoot(fileName);

		/* save the name of the root file in the proc table for rex session */
		strncpy(myptp->p_root, fileName, P_LROOTNAME);
		break;
	case RASTER_FILE_SAVE:
		saveRaster(fileName);
	default:
		break;
	}
	return( Pt_CONTINUE );
}

void writeRoot(char *fileName)
{
	SELECTIONS *selections;
	SELECTSET *thisSet;
	RASTER *rasters;
	PAGE *thisPage;
	LABEL *thisLabel;
	CELL *thisCell;
	PLOT *thisPlot;
	GRAPH *thisGraph;
	SIGNAL *thisSignal;
	ECODEMARK *thisMark;
	FILE *outFile;
	int i, j, k;
	char xlabel[64];
	char ylabel[64];
	
	/* open the file for writing */
	if((outFile = fopen(fileName, "w")) == NULL) {
		sprintf(errmsg, "can't open file %s", fileName);
		rxerr(errmsg);
		return;
	}

	selections = getSelections();
	rasters = getRasters();

	/* write comments to file */
	fprintf(outFile, "# Lines beginning with C list the start code\n");
	fprintf(outFile, "# Lines beginning with S list numbers of select sets\n");
	fprintf(outFile, "# Lines beginning with s list select set parameters\n");
	fprintf(outFile, "# Lines beginning with a list allOf codes\n");
	fprintf(outFile, "# Lines beginning with n list nonOf codes\n");
	fprintf(outFile, "# Lines beginning with y list anyOf codes\n");
	fprintf(outFile, "# \n");
	fprintf(outFile, "# Lines beginning with R list numbers of raster pages\n");
	fprintf(outFile, "# Lines beginning with r list raster page parameters\n");
	fprintf(outFile, "# Lines beginning with L list page label parameters\n");
	fprintf(outFile, "# \n");
	fprintf(outFile, "# Lines beginning with c list cell parameters\n");
	fprintf(outFile, "# Lines beginning with i list signal parameters\n");
	fprintf(outFile, "# Lines beginning with e list ecode mark parameters\n");
	fprintf(outFile, "# Lines beginning with l list cell label parameters\n");
	fprintf(outFile, "# Lines beginning with g list graph parameters\n");
	fprintf(outFile, "# Lines beginning with X list graph X axis parameters\n");
	fprintf(outFile, "# Lines beginning with Y list graph Y axis parameters\n");
	fprintf(outFile, "\n");

	/* write selections to file */
	fprintf(outFile, "# SELECTIONS START\n");
	/* C */
	fprintf(outFile, "C %d\n", selections->startCode);
	/* S */
	fprintf(outFile, "S %d\n", selections->nSelectSets);
	for(i = 0; i < selections->nSelectSets; i++) {
		thisSet = &selections->selectSets[i];
		/* s first */
		fprintf(outFile, "s %d ALIGNCODE %d ALIGNOFFSET %d ALLOFS %d NONOFS %d ANYOFS %d\n", i,
				thisSet->alignCode, thisSet->alignOffsetTime, thisSet->select.nAllOf,
				thisSet->select.nNonOf, thisSet->select.nAnyOf);

		printSelect(&thisSet->select, outFile);
	}
	fprintf(outFile, "# SELECTIONS END\n");
	fprintf(outFile, "\n");

	/* write rasters to file */
	fprintf(outFile, "# RASTERS START\n");

	/* write page specifications to file */
	/* R */
	fprintf(outFile, "R %d\n", rasters->nPages);
	for(i = 0; i < rasters->nPages; i++) {
		thisPage = getRasterPage(i);
		/* r first */
		fprintf(outFile, "r %d ROWS %2d COLUMNS %2d LABEL %2d NCELLS %d\n",
				i, thisPage->nRows, thisPage->nCols, thisPage->labelFlag, thisPage->nDefCells);
		thisLabel = &thisPage->pageLabel;
		if(strlen(thisLabel->label)) strcpy(xlabel, thisLabel->label);
		else strcpy(xlabel, "(null)");
		/* L first */
		fprintf(outFile, "L %d PAGE %d FONT %d SIZE %d X %.3f Y %.3f TEXT %s\n",
				thisPage->labelFlag, i, thisLabel->font, thisLabel->sz, thisLabel->x,
				thisLabel->y, xlabel);

		/* write cell specifications to file */
		for(j = 0; j < thisPage->nDefCells; j++) {
			thisCell = getRasterCell(thisPage->cellsInfo, j);
			if(thisCell->rowN < 0) continue; /* don't write out deleted rows */

			thisPlot = &thisCell->plotInfo;
			
			/* the cell description is split into two printfs to make the code a little neater */
			/* c first */
			fprintf(outFile, "c %d PAGE %d COL %2d ROW %2d INDEX %2d SET %d CODE %d",
					j, i, thisCell->colN, thisCell->rowN, thisCell->RCindex, thisPlot->setNum, thisPlot->cCode);
			fprintf(outFile, " SIGNALS %d TRIALS %d MEANS %d SIGMA %.2f MARKS %d\n",
					thisPlot->nSignals, thisPlot->trialFlag, thisPlot->meanFlag, thisPlot->sigma, thisPlot->markFlag);
				
			/* write the signal specifications to file */
			for(k = 0; k < thisPlot->nSignals; k++) {
				thisSignal = getRasterSignal(thisPlot->sigInfo, k);
				/* i first */
				fprintf(outFile, "i %d CELL %d PAGE %d SIGNAME %s\n",
						k, j, i, thisSignal->sigName);
			}

			/* write the graph specifications to file */
			thisGraph = &thisPlot->graph;
			/* g first */
			fprintf(outFile, "g 1 CELL %d PAGE %d START %4d INTERVAL %4d MAXFREQ %3d TICKMARK %3d YAXES %d NUMBER_AXES %d",
					j, i, thisGraph->start, thisGraph->interval, thisGraph->maxFreq, thisGraph->timeMark, thisGraph->yAxes,
					thisGraph->numberAxes);
			fprintf(outFile, " NFORMAT %d NFONT %d NSIZE %d LFONT %d LSIZE %d\n",
					thisGraph->Nformat, thisGraph->Nfont, thisGraph->Nsz, thisGraph->Lfont,
					thisGraph->Lsz);
			if(strlen(thisGraph->XaxisLabel)) strcpy(xlabel, thisGraph->XaxisLabel);
			else strcpy(xlabel, "(null)");
			/* X first */
			fprintf(outFile, "X 1 CELL %d PAGE %d TEXT %s\n", j, i, xlabel);
			if(strlen(thisGraph->YaxisLabel)) strcpy(ylabel, thisGraph->YaxisLabel);
			else strcpy(ylabel, "(null)");
			/* Y first */
			fprintf(outFile, "Y 1 CELL %d PAGE %d TEXT %s\n", j, i, ylabel);

			/* write the ecode mark specifications to file */
			if(thisPlot->markFlag) {
				thisMark = &thisPlot->ecodeMark;
				/* e first */
				fprintf(outFile, "e %d CELL %d PAGE %d MARK %d SIZE %d SYMBOL %s\n",
						thisPlot->markFlag, j, i, thisMark->rMark, thisMark->sz, thisMark->symbolString);
			}

			/* write the label specifications to file */
			thisLabel = &thisPlot->plotLabel;
			if(strlen(thisLabel->label)) strcpy(xlabel, thisLabel->label);
			else strcpy(xlabel, "(null)");
			/* l first */
			fprintf(outFile, "l %d CELL %d PAGE %d FONT %d SIZE %d X %.3f Y %.3f TEXT %s\n",
					thisPlot->labelFlag, j, i, thisLabel->font, thisLabel->sz, thisLabel->x, thisLabel->y,
					xlabel);
			fprintf(outFile, "\n");
		}
	}
	fprintf(outFile, "# RASTERS END\n");

	fflush(outFile);
	fclose(outFile);

	return;
}

void readRoot(char *fileName)
{
	SELECTIONS *selections;
	SELECTSET *thisSet;
	SELECT *thisSelect;
	RASTER *rasters;
	PAGE *thisPage;
	PAGE *lastPage;
	LABEL *thisLabel;
	CELL *thisCell;
	PLOT *thisPlot;
	GRAPH *thisGraph;
	SIGNAL *thisSignal;
	ECODEMARK *thisMark;
	FILE *inFile;
	int i, j, k, m;
	int nAllOfs, nNoneOfs, nAnyOfs;
	char *strPtr;
	char *valueStr;
	char *attrbStr;
	char token;
	char instring[1024];
	
	thisSelect = (SELECT *)NULL;
	thisPage = (PAGE *)NULL;
	thisLabel = (LABEL *)NULL;
	thisCell = (CELL *)NULL;
	thisPlot = (PLOT *)NULL;
	thisGraph = (GRAPH *)NULL;
	thisSignal = (SIGNAL *)NULL;
	thisMark = (ECODEMARK *)NULL;
	nAllOfs = 0;
	nNoneOfs = 0;
	nAnyOfs = 0;
	if((inFile = fopen(fileName, "r")) == NULL) {
		sprintf(errmsg, "can't open file %s", fileName);
		rxerr(errmsg);
		return;
	}

	selections = getSelections();
	selections->newSelects = 0;
	rasters = getRasters();
	rasters->newRasters = 0;

	while(fgets(instring, sizeof(instring), inFile)) {
		strPtr = strtok(instring, " ");		    /* get the first attribute from the input string */

		/* don't process blank lines or comments */
		if(!strPtr) continue;
 		if(!strcmp("\n", strPtr)) continue;
		if(!strcmp("#", strPtr)) continue;
		
		valueStr = strtok(NULL, " ");		    /* get the first value from the input string */
		if(!valueStr) {
			fprintf(stderr, "Invalid value for attribute %s\n", strPtr);
			continue;
		}

		sscanf(strPtr, "%c", &token);	    /* get the first character of the first attribute */ 
		switch(token) {
			/* process the ecode selection portion of the root file */
		case 'C':
			/* REX start code */
			sscanf(valueStr, "%d", &selections->startCode);
			break;
		case 'S':
			/* number of selection sets */
			sscanf(valueStr, "%d", &selections->nSelectSets);
			
			/* malloc space for the select sets in this file */
			selections->selectSets = (SELECTSET *)malloc(selections->nSelectSets * sizeof(SELECTSET));
			if(selections->selectSets == NULL) {
				rxerr("Can't malloc space for selectSets");
				return;
			}
			break;
		case 's':
			/* get details of this selection set */
			/* set number */
			sscanf(valueStr, "%d", &i);		/* this is the set index */
			if(i >= selections->nSelectSets) {
				rxerr("Corrupt select spec file: too many select sets");
				break;
			}
			thisSet = &selections->selectSets[i];
			thisSelect = &thisSet->select;

			nAllOfs = 0;
			nNoneOfs = 0;
			nAnyOfs = 0;

			/* now work through the rest of the attribute-value pairs on the selection set line */
			while(attrbStr = strtok(NULL, " ")) {	/* this statement gets the attribute */
				valueStr = strtok(NULL, " ");	/* this statement gets the value */
				if(!valueStr) {
					rxerr("Invalid value for attribute");
					break;
				}
				
				/* alignment code */
				if(!strcmp("ALIGNCODE", attrbStr)) {
					sscanf(valueStr, "%d", &thisSet->alignCode);
				}

				/* get the alignment offset */
				else if(!strcmp("ALIGNOFFSET", attrbStr)) {
					sscanf(valueStr, "%d", &thisSet->alignOffsetTime);
				}

				/* allOf number */
				else if(!strcmp("ALLOFS", attrbStr)) {
					sscanf(valueStr, "%d", &thisSelect->nAllOf);
				}

				/* Nonof number */
				else if(!strcmp("NONOFS", attrbStr)) {
					sscanf(valueStr, "%d", &thisSelect->nNonOf);
				}
				
				/* anyOf number */
				else if(!strcmp("ANYOFS", attrbStr)) {
					sscanf(valueStr, "%d", &thisSelect->nAnyOf);
				}
			}
			break;
		case 'a':
			if(nAllOfs >= thisSelect->nAllOf) {
				rxerr("Corrupt select spec file: too many allOf codes");
				break;
			}
			sscanf(valueStr, "%d", &thisSelect->allOfList[nAllOfs++]);
			break;
		case 'n':
			if(nNoneOfs >= thisSelect->nNonOf) {
				rxerr("Corrupt select spec file: too many nonOf codes");
				break;
			}
			sscanf(valueStr, "%d", &thisSelect->nonOfList[nNoneOfs++]);
			break;
		case 'y':
			if(nAnyOfs >= thisSelect->nAnyOf) {
				rxerr("Corrupt select spec file: too many anyOf codes");
				break;
			}

			i = sscanf(valueStr, "%d-%d", &thisSelect->anyOfList[nAnyOfs].beg,
					   &thisSelect->anyOfList[nAnyOfs].end);
			if(i == 2) { /* true if value string is a range of ecodes */
				for(j = 0; j < MAX_OR_CODES; j++) thisSelect->anyOfList[nAnyOfs].list[j] = 0;
			}
			else {	/* true if value string is the first of a set or'ed ecodes */
				thisSelect->anyOfList[nAnyOfs].beg = 0;
				thisSelect->anyOfList[nAnyOfs].end = 0;
				for(j = 0; j < MAX_OR_CODES; j++) thisSelect->anyOfList[nAnyOfs].list[j] = 0;
				m = 0;
				sscanf(valueStr, "%d", &thisSelect->anyOfList[nAnyOfs].list[m++]);
				while(valueStr = strtok(NULL, " ")) {
					sscanf(valueStr, "%d", &thisSelect->anyOfList[nAnyOfs].list[m++]);
					if(m >= MAX_OR_CODES) break;
				}
			}
			nAnyOfs++;
			break;

			/* proces the raster specification portion of the root file */
		case 'R':		/* number of pages */
			sscanf(valueStr, "%d", &rasters->nPages);

			/* malloc space for the pages in this file */
			if(rasters->nPages) {
				rasters->pages = addRasterPage(rasters->pages, rasters->nPages);
				if(rasters->pages == NULL) {
					rxerr("Can't allocate space for raster pages");
					return;
				}
			}

			/* set default values for all pages */
			thisPage = getRasterPage(0);
			initFirstPage(thisPage);
			for(i = 1; i < rasters->nPages; i++) {
				thisPage = getRasterPage(i);
				lastPage = getRasterPage(i - 1);
				initSubsequentPage(thisPage, lastPage);
			}
			break;
		case 'r':	/* details of a particular page */
			sscanf(valueStr, "%d", &i);	/* page index */
			if(i >= rasters->nPages) {
				rxerr("invalid page number");
				break;
			}
			thisPage = getRasterPage(i);	/* pointer to this page structure */

			/* walk through the attribute-value pairs for this page */
			while(attrbStr = strtok(NULL, " ")) {
				valueStr = strtok(NULL, " ");
				if(!valueStr) {
					rxerr("Invalid value for attribute");
					break;
				}
				if(!strcmp("ROWS", attrbStr)) sscanf(valueStr, "%d", &thisPage->nRows);
				else if(!strcmp("COLUMNS", attrbStr)) sscanf(valueStr, "%d", &thisPage->nCols);
				else if(!strcmp("LABEL", attrbStr)) sscanf(valueStr, "%d", &thisPage->labelFlag);
				else if(!strcmp("NCELLS", attrbStr)) sscanf(valueStr, "%d", &thisPage->nDefCells);
			}
			break;
		case 'L':	/* get the details of the page label */
			sscanf(valueStr, "%d", &j);

			/* walk through the attribute-value pairs for this label */
			while(attrbStr = strtok(NULL, " ")) {
				if(!strcmp("TEXT", attrbStr)) {
					/* for the text attribute, use the tab separator so that strtok will
					 * return the rest of the line including spaces in valueStr
					 */
					valueStr = strtok(NULL, "\t");
					valueStr[strlen(valueStr) - 1] = 0;	/* strip off carriage return */
					if(!strcmp(valueStr, "(null)")) strcpy(thisPage->pageLabel.label, "");
					else sprintf(thisPage->pageLabel.label, "%s", valueStr);
				}
				else {
					valueStr = strtok(NULL, " ");
					if(!valueStr) {
						rxerr("Invalid value for attribute");
						break;
					}
					
					if(!strcmp("PAGE", attrbStr)) {
						sscanf(valueStr, "%d", &i);
						if(i >= rasters->nPages) {
							rxerr("invalid page number");
							break;
						}
						thisPage = getRasterPage(i);
						thisLabel = &thisPage->pageLabel;
					}
					else if(!strcmp("FONT", attrbStr)) sscanf(valueStr, "%d", &thisLabel->font);
					else if(!strcmp("SIZE", attrbStr)) sscanf(valueStr, "%d", &thisLabel->sz);
					else if(!strcmp("X", attrbStr)) sscanf(valueStr, "%lf", &thisLabel->x);
					else if(!strcmp("Y", attrbStr)) sscanf(valueStr, "%lf", &thisLabel->y);
				}
			}
			break;
		case 'c':
			/* get the  details of this cell */
			sscanf(valueStr, "%d", &j);	/* cell index */
			while(attrbStr = strtok(NULL, " ")) {
				valueStr = strtok(NULL, " ");
				if(!valueStr) {
					rxerr("Invalid value for attribute");
					break;
				}
				if(!strcmp("PAGE", attrbStr)) {
					sscanf(valueStr, "%d", &i);
					if(i >= rasters->nPages) {
						rxerr("invalid page number");
						break;
					}
					thisPage = getRasterPage(i);
					if(j >= thisPage->nDefCells) {
						rxerr("invalid cell number for page");
						break;
					}
					thisCell = getRasterCell(thisPage->cellsInfo, j);
					thisPlot = &thisCell->plotInfo;
				}
				else if(!strcmp("COL", attrbStr)) sscanf(valueStr, "%d", &thisCell->colN);
				else if(!strcmp("ROW", attrbStr)) sscanf(valueStr, "%d", &thisCell->rowN);
				else if(!strcmp("INDEX", attrbStr)) sscanf(valueStr, "%d", &thisCell->RCindex);
				else if(!strcmp("SET", attrbStr)) sscanf(valueStr, "%d", &thisPlot->setNum);
				else if(!strcmp("CODE", attrbStr)) sscanf(valueStr, "%d", &thisPlot->cCode);
				else if(!strcmp("SIGNALS", attrbStr)) sscanf(valueStr, "%d", &thisPlot->nSignals);
				else if(!strcmp("TRIALS", attrbStr)) sscanf(valueStr, "%d", &thisPlot->trialFlag);
				else if(!strcmp("MEANS", attrbStr)) sscanf(valueStr, "%d", &thisPlot->meanFlag);
				else if(!strcmp("SIGMA", attrbStr)) sscanf(valueStr, "%lf", &thisPlot->sigma);
				else if(!strcmp("MARKS", attrbStr)) sscanf(valueStr, "%d", &thisPlot->markFlag);
			}
			break;
		case 'i':
			/* get the details of this signal */
			sscanf(valueStr, "%d", &k);	/* signal index */
			while(attrbStr = strtok(NULL, " ")) {
				valueStr = strtok(NULL, " ");
				if(!valueStr) {
					rxerr("Invalid value for attribute");
					break;
				}
				if(!strcmp("CELL", attrbStr)) sscanf(valueStr, "%d", &j);
				else if(!strcmp("PAGE", attrbStr)) {
					sscanf(valueStr, "%d", &i);
					if(i >= rasters->nPages) {
						rxerr("invalid page number");
						break;
					}
					thisPage = getRasterPage(i);
					if(j >= thisPage->nDefCells) {
						rxerr("invalid cell number for page");
						break;
					}
					thisCell = getRasterCell(thisPage->cellsInfo, j);
					thisPlot = &thisCell->plotInfo;
					if(k >= thisPlot->nSignals) {
						rxerr("invalid signal number");
						break;
					}
					thisSignal = getRasterSignal(thisPlot->sigInfo, k);
				}
				else if(!strcmp("SIGNAME", attrbStr)) {
					valueStr[strlen(valueStr) - 1] = 0;	/* strip off carriage return */
					sprintf(thisSignal->sigName, "%s", valueStr);
				}
			}
			break;
		case 'g':
			/* get details of this graph */
			sscanf(valueStr, "%d", &k);	/* dummy index number to maintain attribute-value pairs */
			while(attrbStr = strtok(NULL, " ")) {
				valueStr = strtok(NULL, " ");
				if(!valueStr) {
					rxerr("Invalid value for attribute");
					break;
				}
				if(!strcmp("CELL", attrbStr)) sscanf(valueStr, "%d", &j);
				else if(!strcmp("PAGE", attrbStr)) {
					sscanf(valueStr, "%d", &i);
					if(i >=rasters->nPages) {
						rxerr("invalid page number");
						break;
					}
					thisPage = getRasterPage(i);
					if(j >= thisPage->nDefCells) {
						rxerr("invalid cell number for page");
						break;
					}
					thisCell = getRasterCell(thisPage->cellsInfo, j);
					thisPlot = &thisCell->plotInfo;
					thisGraph = &thisPlot->graph;
				}
				else if(!strcmp("START", attrbStr)) sscanf(valueStr, "%d", &thisGraph->start);
				else if(!strcmp("INTERVAL", attrbStr)) sscanf(valueStr, "%d", &thisGraph->interval);
				else if(!strcmp("MAXFREQ", attrbStr)) sscanf(valueStr, "%d", &thisGraph->maxFreq);
				else if(!strcmp("TICKMARK", attrbStr)) sscanf(valueStr, "%d", &thisGraph->timeMark);
				else if(!strcmp("YAXES", attrbStr)) sscanf(valueStr, "%d", &thisGraph->yAxes);
				else if(!strcmp("NUMBER_AXES", attrbStr)) sscanf(valueStr, "%d", &thisGraph->numberAxes);
				else if(!strcmp("NFORMAT", attrbStr)) sscanf(valueStr, "%d", &thisGraph->Nformat);
				else if(!strcmp("NFONT", attrbStr)) sscanf(valueStr, "%d", &thisGraph->Nfont);
				else if(!strcmp("NSIZE", attrbStr)) sscanf(valueStr, "%d", &thisGraph->Nsz);
				else if(!strcmp("LFONT", attrbStr)) sscanf(valueStr, "%d", &thisGraph->Lfont);
				else if(!strcmp("LSIZE", attrbStr)) sscanf(valueStr, "%d", &thisGraph->Lsz);
			}
			break;
		case 'X':
			/* get the details of the X axis label */
			sscanf(valueStr, "%d", &k);	/* dummy index number to maintain attribute-value pairs */
			while(attrbStr = strtok(NULL, " ")) {
				if(!strcmp("TEXT", attrbStr)) {
					/* for the text attribute, use the tab separator so that strtok will
					 * return the rest of the line including spaces in valueStr
					 */
					valueStr = strtok(NULL, "\t");
					valueStr[strlen(valueStr) - 1] = 0;	/* strip off carriage return */
					if(!strcmp(valueStr, "(null)")) strcpy(thisGraph->XaxisLabel, "");
					else sprintf(thisGraph->XaxisLabel, "%s", valueStr);
				}
				else {
					valueStr = strtok(NULL, " ");
					if(!valueStr) {
						rxerr("Invalid value for attribute");
						break;
					}
					if(!strcmp("CELL", attrbStr)) sscanf(valueStr, "%d", &j);
					else if(!strcmp("PAGE", attrbStr)) {
						sscanf(valueStr, "%d", &i);
						if(i >=rasters->nPages) {
							rxerr("case X: invalid page number");
							break;
						}
						thisPage = getRasterPage(i);
						if(j >= thisPage->nDefCells) {
							rxerr("case X: invalid cell number for page");
							break;
						}
						thisCell = getRasterCell(thisPage->cellsInfo, j);
						thisPlot = &thisCell->plotInfo;
						thisGraph = &thisPlot->graph;
					}
				}
			}
			break;
		case 'Y':
			/* get the details of the X axis label */
			sscanf(valueStr, "%d", &k);	/* dummy index number to maintain attribute-value pairs */
			while(attrbStr = strtok(NULL, " ")) {
				if(!strcmp("TEXT", attrbStr)) {
					/* for the text attribute, use the tab separator so that strtok will
					 * return the rest of the line including spaces in valueStr
					 */
					valueStr = strtok(NULL, "\t");
					valueStr[strlen(valueStr) - 1] = 0;	/* strip off carriage return */
					if(!strcmp(valueStr, "(null)")) strcpy(thisGraph->YaxisLabel, "");
					else sprintf(thisGraph->YaxisLabel, "%s", valueStr);
				}
				else {
					valueStr = strtok(NULL, " ");
					if(!valueStr) {
						rxerr("Invalid value for attribute");
						break;
					}
					if(!strcmp("CELL", attrbStr)) sscanf(valueStr, "%d", &j);
					else if(!strcmp("PAGE", attrbStr)) {
						sscanf(valueStr, "%d", &i);
						if(i >=rasters->nPages) {
							rxerr("case Y: invalid page number");
							break;
						}
						thisPage = getRasterPage(i);
						if(j >= thisPage->nDefCells) {
							rxerr("case Y: invalid cell number for page");
							break;
						}
						thisCell = getRasterCell(thisPage->cellsInfo, j);
						thisPlot = &thisCell->plotInfo;
						thisGraph = &thisPlot->graph;
					}
				}
			}
			break;
		case 'e':
			/* get the details of this ecodeMark */
			sscanf(valueStr, "%d", &k);
			while(attrbStr = strtok(NULL, " ")) {
				valueStr = strtok(NULL, " ");
				if(!valueStr) {
					rxerr("Invalid value for attribute");
					break;
				}
				if(!strcmp("CELL", attrbStr)) sscanf(valueStr, "%d", &j);
				else if(!strcmp("PAGE", attrbStr)) {
					sscanf(valueStr, "%d", &i);
					if(i >= rasters->nPages) {
						rxerr("invalid page number");
						break;
					}
					thisPage = getRasterPage(i);
					if(j >= thisPage->nDefCells) {
						rxerr("invalid cell number for page");
						break;
					}
					thisCell = getRasterCell(thisPage->cellsInfo, j);
					thisPlot = &thisCell->plotInfo;
					thisMark = &thisPlot->ecodeMark;
					thisPlot->markFlag = k;
				}
				else if(!strcmp("MARK", attrbStr)) sscanf(valueStr, "%d", &thisMark->rMark);
				else if(!strcmp("SIZE", attrbStr)) sscanf(valueStr, "%d", &thisMark->sz);
				else if(!strcmp("SYMBOL", attrbStr)) {
					sscanf(valueStr, "%s", thisMark->symbolString);
					thisMark->symbolChar = decodeSymbolString(thisMark->symbolString);
				}
			}
			break;
		case 'l':
			/* get the details of the plot label */
			sscanf(valueStr, "%d", &k);
			while(attrbStr = strtok(NULL, " ")) {
				if(!strcmp("TEXT", attrbStr)) {
					/* for the text attribute, use the tab separator so that strtok will
					 * return the rest of the line including spaces in valueStr
					 */
					valueStr = strtok(NULL, "\t");
					valueStr[strlen(valueStr) - 1] = 0;	/* strip off carriage return */
					if(!strcmp("(null)", valueStr)) strcpy(thisLabel->label, "");
					else sprintf(thisLabel->label, "%s", valueStr);
				}
				else {
					valueStr = strtok(NULL, " ");
					if(!valueStr) {
						rxerr("Invalid value for attribute");
						break;
					}
					if(!strcmp("CELL", attrbStr)) sscanf(valueStr, "%d", &j);
					else if(!strcmp("PAGE", attrbStr)) {
						sscanf(valueStr, "%d", &i);
						if(i >= rasters->nPages) {
							rxerr("invalid page number");
							break;
						}
						thisPage = getRasterPage(i);
						if(j >= thisPage->nDefCells) {
							rxerr("invalid cell number for page");
							break;
						}
						thisCell = getRasterCell(thisPage->cellsInfo, j);
						thisPlot = &thisCell->plotInfo;
						thisPlot->labelFlag = k;
						thisLabel = &thisPlot->plotLabel;
					}
					else if(!strcmp("FONT", attrbStr)) sscanf(valueStr, "%d", &thisLabel->font);
					else if(!strcmp("SIZE", attrbStr)) sscanf(valueStr, "%d", &thisLabel->sz);
					else if(!strcmp("X", attrbStr)) sscanf(valueStr, "%lf", &thisLabel->x);
					else if(!strcmp("Y", attrbStr)) sscanf(valueStr, "%lf", &thisLabel->y);
				}
			}
			break;
		default:
			break;
		}
	}
	fclose(inFile);

	return;
}

void printSelect(SELECT *select, FILE *outFile)
{
	int i, j;
	
	/* a first */
	for(i = 0; i < select->nAllOf; i++) {
		fprintf(outFile, "a %d\n", select->allOfList[i]);
	}
	/* n first */
	for(i = 0; i < select->nNonOf; i++) {
		fprintf(outFile, "n %d\n", select->nonOfList[i]);
	}
	/* y first */
	for(i = 0; i < select->nAnyOf; i++) {
		if(select->anyOfList[i].beg) {
			fprintf(outFile, "y %d-%d\n", select->anyOfList[i].beg,
					select->anyOfList[i].end);
		}
		else {
			j = 0;
			fprintf(outFile, "y");
			while(select->anyOfList[i].list[j]) {
				fprintf(outFile, " %d", select->anyOfList[i].list[j]);
				j++;
			}
			fprintf(outFile, "\n");
		}
	}
}

void saveRaster(char *fileName)
{
	FILE *outFile;
	char *tigBuffer;
	size_t tigBufferLength;

	/* open the file for writing */
	if((outFile = fopen(fileName, "w")) == NULL) {
		sprintf(errmsg, "can't open file %s", fileName);
		rxerr(errmsg);
		return;
	}

	tigBuffer = getTigList();
	tigBufferLength = getTigListPtr();
	fwrite(tigBuffer, sizeof(*tigBuffer), tigBufferLength, outFile);

	fflush(outFile);
	fclose(outFile);
}

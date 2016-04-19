/* Y o u r   D e s c r i p t i o n                       */
/*                            AppBuilder Photon Code Lib */
/*                                         Version 0.83B */

/* Standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#include <signal.h>

/* Toolkit headers */
#include <Ph.h>
#include <photon/Pg.h>
#include <Pt.h>
#include <Ap.h>

/* Local headers */
#include "abimport.h"
#include "ph_rline.h"
#include "proto.h"
#include "timerFunction.h"
#include <signal.h>
#include <time.h>

extern lsrRunLine runLine;
extern TRACE rlTraces[RL_MAXCHAN + 2];

int init( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PhDim_t dim;
	PhRect_t frame;
	PtArg_t	arg;
	PgColor_t list[MAX_COLORS];
	TRACE *thisTrace;
	int scale;
	short int x;
	short int y;
	int i;

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	/* actions called before the oscope widget is realized */
	if(apinfo->reason == ABR_PRE_REALIZE) {
		list[0] = PgRGB(255, 0, 0);		/* red */
		list[1] = PgRGB(255, 128, 0);	/* orange */
		list[2] = PgRGB(255, 255, 0);	/* yellow */
		list[3] = PgRGB(0, 255, 0);		/* green */
		list[4] = PgRGB(0, 255, 255);	/* cyan */
		list[5] = PgRGB(0, 0, 255);		/* blue */
		list[6] = PgRGB(255, 0, 255);	/* magenta */
		list[7] = PgRGB(255, 0, 128);	/* pink */

		list[8] = PgRGB(254, 85, 85);
		list[9] = PgRGB(254, 170, 85);
		list[10] = PgRGB(254, 254, 85);
		list[11] = PgRGB(85, 254, 85);
		list[12] = PgRGB(85, 254, 254);
		list[13] = PgRGB(85, 85, 254);
		list[14] = PgRGB(254, 85, 254);
		list[15] = PgRGB(254, 85, 170);

		list[16] = PgRGB(254, 128, 128);
		list[17] = PgRGB(254, 191, 128);
		list[18] = PgRGB(254, 254, 128);
		list[19] = PgRGB(128, 254, 128);
		list[20] = PgRGB(128, 254, 254);
		list[21] = PgRGB(128, 128, 254);
		list[22] = PgRGB(254, 128, 254);
		list[23] = PgRGB(254, 128, 190);

		list[24] = PgRGB(254, 190, 190);
		list[25] = PgRGB(254, 222, 190);
		list[26] = PgRGB(254, 254, 190);
		list[27] = PgRGB(190, 254, 190);
		list[28] = PgRGB(190, 254, 254);
		list[29] = PgRGB(190, 190, 254);
		list[30] = PgRGB(254, 190, 254);
		list[31] = PgRGB(254, 190, 222);

		for(i = 0; i < MAX_COLORS; i++) runLine.traceColors[i] = list[i];

		runLine.scaleFactor = 0;

		/*
		 * increase the size of the drawing buffer to 16K, i.e., 16 * 1024 = 16384
		 */
		PgSetDrawBufferSize(16384);
	}

	/* actions called after the oscope widget is realized */
	else {
		PhSysInfo_t si;
		if(PtQuerySystemInfo( link_instance, &si) == NULL ||
		   !(si.gfx.valid_fields & Ph_GFX_CAPABILITIES) ||
		   (si.gfx.capabilities	& ( Ph_GCAP_MASKED_BLIT | Ph_GCAP_PALETTE)) !=
		   (Ph_GCAP_MASKED_BLIT | Ph_GCAP_PALETTE)) {

			PtSetArg(&arg, Pt_ARG_FLAGS, Pt_TRUE, Pt_GHOST | Pt_BLOCKED);
		}

		/* save the location and size of the main window for rex session */
		PtWindowGetFrameSize(link_instance, &frame);
		PtGetAbsPosition(link_instance, &x, &y);
		myptp->p_x = x - frame.ul.x;
		myptp->p_y = y - frame.ul.y;

		PtWidgetDim(link_instance, &dim);
		myptp->p_w = dim.w;
		myptp->p_h = dim.h;

		/* read root file from rex session */
		if(strlen(myptp->p_root) > 0) {
			readRoot(myptp->p_root);
		}

		/* set the select colors of the buttons */
		PtSetArg(&arg, Pt_ARG_SET_COLOR, list[0], 0);
		PtSetResources(ABW_chan0Toggle, 1, &arg);

		PtSetArg(&arg, Pt_ARG_SET_COLOR, list[1], 0);
		PtSetResources(ABW_chan1Toggle, 1, &arg);

		PtSetArg(&arg, Pt_ARG_SET_COLOR, list[2], 0);
		PtSetResources(ABW_chan2Toggle, 1, &arg);

		PtSetArg(&arg, Pt_ARG_SET_COLOR, list[3], 0);
		PtSetResources(ABW_chan3Toggle, 1, &arg);

		PtSetArg(&arg, Pt_ARG_SET_COLOR, list[4], 0);
		PtSetResources(ABW_chan4Toggle, 1, &arg);

		PtSetArg(&arg, Pt_ARG_SET_COLOR, list[5], 0);
		PtSetResources(ABW_chan5Toggle, 1, &arg);

		PtSetArg(&arg, Pt_ARG_SET_COLOR, list[6], 0);
		PtSetResources(ABW_chan6Toggle, 1, &arg);

		PtSetArg(&arg, Pt_ARG_SET_COLOR, list[7], 0);
		PtSetResources(ABW_chan7Toggle, 1, &arg);

		/* set the states of the toggle buttons */
		for(i = 0; i < RL_MAXCHAN + 2; i++) {
			thisTrace = &rlTraces[i];
			if(thisTrace->rl_on) {
				PtSetArg(&arg, Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
				switch(i) {
				case 0:
					PtSetResources(ABW_chan0Toggle, 1, &arg);
					break;
				case 1:
					PtSetResources(ABW_chan1Toggle, 1, &arg);
					break;
				case 2:
					PtSetResources(ABW_chan2Toggle, 1, &arg);
					break;
				case 3:
					PtSetResources(ABW_chan3Toggle, 1, &arg);
					break;
				case 4:
					PtSetResources(ABW_chan4Toggle, 1, &arg);
					break;
				case 5:
					PtSetResources(ABW_chan5Toggle, 1, &arg);
					break;
				case 6:
					PtSetResources(ABW_chan6Toggle, 1, &arg);
					break;
				case 7:
					PtSetResources(ABW_chan7Toggle, 1, &arg);
					break;
				case 8:
					PtSetResources(ABW_rlineToggle, 1, &arg);
					break;
				case 9:
					PtSetResources(ABW_unitsToggle, 1, &arg);
					break;
				}
			}
			else {
				PtSetArg(&arg, Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
				switch(i) {
				case 0:
					PtSetResources(ABW_chan0Toggle, 1, &arg);
					break;
				case 1:
					PtSetResources(ABW_chan1Toggle, 1, &arg);
					break;
				case 2:
					PtSetResources(ABW_chan2Toggle, 1, &arg);
					break;
				case 3:
					PtSetResources(ABW_chan3Toggle, 1, &arg);
					break;
				case 4:
					PtSetResources(ABW_chan4Toggle, 1, &arg);
					break;
				case 5:
					PtSetResources(ABW_chan5Toggle, 1, &arg);
					break;
				case 6:
					PtSetResources(ABW_chan6Toggle, 1, &arg);
					break;
				case 7:
					PtSetResources(ABW_chan7Toggle, 1, &arg);
					break;
				case 8:
					PtSetResources(ABW_rlineToggle, 1, &arg);
					break;
				case 9:
					PtSetResources(ABW_unitsToggle, 1, &arg);
					break;
				}
			}
		}
		
		/* update the toggle button labels */
		updateLabels();
	}

	return( Pt_CONTINUE );
}

void writeRoot()
{
	snprintf(myptp->p_root, P_LROOTNAME, "S %d O %d T %d TL %d D %d M %d R %d",
			 runLine.scaleFactor, rlTraces[RL_BAR_INDX].rl_offset, runLine.trigger,
			 runLine.triggerLevel, runLine.display, runLine.modeValue,
			 runLine.timeBase);
}

void readRoot(char *root)
{
	char *attrbStr;
	char *valueStr;
	char rootCopy[P_LROOTNAME];

	/* make a local copy of the root for strtok */
	strncpy(rootCopy, root, P_LROOTNAME);
	attrbStr = strtok(rootCopy, " ");
	while(attrbStr != (char *)NULL) {
		valueStr = strtok(NULL, " ");
		if(valueStr != (char *)NULL) {
			if(!strcmp(attrbStr, "S")) {
				sscanf(valueStr, "%hd", &runLine.scaleFactor);
			}
			else if(!strcmp(attrbStr, "O")) {
				sscanf(valueStr, "%d", &rlTraces[RL_BAR_INDX].rl_offset);
			}
			else if(!strcmp(attrbStr, "T")) {
				sscanf(valueStr, "%hd", &runLine.trigger);
			}
			else if(!strcmp(attrbStr, "TL")) {
				sscanf(valueStr, "%hd", &runLine.triggerLevel);
			}
			else if(!strcmp(attrbStr, "D")) {
				sscanf(valueStr, "%hd", &runLine.display);
			}
			else if(!strcmp(attrbStr, "M")) {
				sscanf(valueStr, "%hd", &runLine.modeValue);
			}
			else if(!strcmp(attrbStr, "R")) {
				sscanf(valueStr, "%hd", &runLine.timeBase);
				sscanf(valueStr, "%hd", &runLine.timeCounter);
			}
		}

		attrbStr = strtok(NULL, " ");
	}
	
	PtSetResource(ABW_rlTrigButton, Pt_ARG_ONOFF_STATE, runLine.trigger, 0);
	if(runLine.trigger) {
		PtSetResource(ABW_rlTrigButton, Pt_ARG_TEXT_STRING, "Triggered", 0);
	}
	else {
		PtSetResource(ABW_rlTrigButton, Pt_ARG_TEXT_STRING, "Continuous", 0);
	}
	
	PtSetResource(ABW_rlRepeatButton, Pt_ARG_ONOFF_STATE, runLine.display, 0);
	if(runLine.display) {
		PtSetResource(ABW_rlRepeatButton, Pt_ARG_TEXT_STRING, "One Shot", 0);
	}
	else {
		PtSetResource(ABW_rlRepeatButton, Pt_ARG_TEXT_STRING, "Repeat", 0);
	}
	
	PtSetResource(ABW_rlModeButton, Pt_ARG_ONOFF_STATE, runLine.modeValue, 0);
	if(runLine.display) {
		PtSetResource(ABW_rlModeButton, Pt_ARG_TEXT_STRING, "Superimpose", 0);
	}
	else {
		PtSetResource(ABW_rlModeButton, Pt_ARG_TEXT_STRING, "Single", 0);
	}
	
	PtSetResource(ABW_rlTrigLevel, Pt_ARG_NUMERIC_VALUE, runLine.triggerLevel, 0);
}

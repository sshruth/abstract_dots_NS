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
#include <time.h>

/* Toolkit headers */
#include <Ph.h>
#include <Pt.h>
#include <Ap.h>

/* Local headers */
#include "abimport.h"
#include "ph_ewind.h"
#include "proto.h"

extern CURS_DES cursDescrip[WDI_NCURS];
extern lsrDispWindow dispWind;

int init(PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
	PhDim_t dim;
	PhRect_t frame;
	PtArg_t	args[2];
	PgColor_t list[MAX_COLORS];
	short int x;
	short int y;
	int i;
	
	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	if(apinfo->reason == ABR_PRE_REALIZE) {
		list[0] = PgRGB(255, 0, 0);		/* red */
		list[1] = PgRGB(255, 128, 0);	/* orange */
		list[2] = PgRGB(255, 255, 0);	/* yellow */
		list[3] = PgRGB(0, 255, 0);		/* green */
		list[4] = PgRGB(0, 255, 255);	/* cyan */
		list[5] = PgRGB(0, 0, 255);		/* blue */
		list[6] = PgRGB(255, 0, 255);	/* magenta */
		list[7] = PgRGB(255, 96, 128);	/* pink */

		list[8] = PgRGB(254, 128, 85);
		list[9] = PgRGB(254, 192, 85);
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

		for(i = 0; i < MAX_COLORS; i++) dispWind.traceColors[i] = list[i];

		/*
		 * resize the current drawing buffer to 16K, i.e., 16 * 1024 = 16384
		 */
		PgSetDrawBufferSize(16384);

	}
	else {
		/* Post-realize */
		PhSysInfo_t si;

		if(PtQuerySystemInfo( link_instance, &si) == NULL ||
		   !(si.gfx.valid_fields & Ph_GFX_CAPABILITIES) ||
		   (si.gfx.capabilities	& ( Ph_GCAP_MASKED_BLIT | Ph_GCAP_PALETTE)) !=
		   (Ph_GCAP_MASKED_BLIT | Ph_GCAP_PALETTE)) {
			PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_GHOST | Pt_BLOCKED);
		}

		/* unrealize the text widgets */
		PtUnrealizeWidget(ABW_vexTerminal);
		PtUnrealizeWidget(ABW_vexCommand);

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

		/* set the scales */
		changeScales(dispWind.scaleFactor);
		
		/* set the initial cursor display flags */
		for(i = 0; i < 2; i++) cursDescrip[i].on = 1;	/* center and eye cursors */
		cursDescrip[0].type = CU_PLUS;
		cursDescrip[1].type = CU_BOX;
		for(; i < WDI_NDATACURS; i++) {
			cursDescrip[i].on = 0;
			cursDescrip[i].type = CU_NONE;
		}
		cursDescrip[i].on = 1;					/* wind0 cursor */
		cursDescrip[i].type = CU_EYEWIN;
		i++;
		for(; i < WDI_NCURS; i++) {
			cursDescrip[i].on = 0;
			cursDescrip[i].type = CU_EYEWIN;
		}

		for(i = 0; i < WDI_NCURS; i++) {
			dispWind.cursors[i].on = cursDescrip[i].on;
			dispWind.cursors[i].type = cursDescrip[i].type;
		}
		
		dispWind.numCurs = WDI_NCURS;
		
		/* set the initial states and colors of the cursor buttons */
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetArg(&args[1], Pt_ARG_SET_COLOR, list[0], 0);
		PtSetResources(ABW_curs0Btn, 2, args);

		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetArg(&args[1], Pt_ARG_SET_COLOR, list[1], 0);
		PtSetResources(ABW_curs1Btn, 2, args);

		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetArg(&args[1], Pt_ARG_SET_COLOR, list[2], 0);
		PtSetResources(ABW_curs2Btn, 2, args);

		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetArg(&args[1], Pt_ARG_SET_COLOR, list[3], 0);
		PtSetResources(ABW_curs3Btn, 2, args);

		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetArg(&args[1], Pt_ARG_SET_COLOR, list[4], 0);
		PtSetResources(ABW_curs4Btn, 2, args);

		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetArg(&args[1], Pt_ARG_SET_COLOR, list[5], 0);
		PtSetResources(ABW_curs5Btn, 2, args);

		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetArg(&args[1], Pt_ARG_SET_COLOR, list[6], 0);
		PtSetResources(ABW_curs6Btn, 2, args);

		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetArg(&args[1], Pt_ARG_SET_COLOR, list[7], 0);
		PtSetResources(ABW_curs7Btn, 2, args);

		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
		PtSetArg(&args[1], Pt_ARG_SET_COLOR, list[8], 0);
		PtSetResources(ABW_wnd0Btn, 2, args);

		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetArg(&args[1], Pt_ARG_SET_COLOR, list[9], 0);
		PtSetResources(ABW_wnd1Btn, 2, args);

		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetArg(&args[1], Pt_ARG_SET_COLOR, list[10], 0);
		PtSetResources(ABW_wnd2Btn, 2, args);

		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetArg(&args[1], Pt_ARG_SET_COLOR, list[11], 0);
		PtSetResources(ABW_wnd3Btn, 2, args);

		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetArg(&args[1], Pt_ARG_SET_COLOR, list[12], 0);
		PtSetResources(ABW_wnd4Btn, 2, args);

		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetArg(&args[1], Pt_ARG_SET_COLOR, list[13], 0);
		PtSetResources(ABW_wnd5Btn, 2, args);

		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetArg(&args[1], Pt_ARG_SET_COLOR, list[14], 0);
		PtSetResources(ABW_wnd6Btn, 2, args);

		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		PtSetArg(&args[1], Pt_ARG_SET_COLOR, list[15], 0);
		PtSetResources(ABW_wnd7Btn, 2, args);

		if(dispWind.grid) {
			int res;
			PtSetResource(ABW_gridToggleButton, Pt_ARG_FLAGS, Pt_TRUE, Pt_SET);
			if(PtWidgetIsRealized(ABW_eyeWinGrid) == 0) {
				res = PtRealizeWidget(ABW_eyeWinGrid);
			}
		}
		else {
			int res;
			PtSetResource(ABW_gridToggleButton, Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
			if(PtWidgetIsRealized(ABW_eyeWinGrid) != 0) {
				res = PtUnrealizeWidget(ABW_eyeWinGrid);
			}
		}
	}
	
	return( Pt_CONTINUE);
}

void writeRoot()
{
	snprintf(myptp->p_root, P_LROOTNAME, "G %d S %d R %d T %d TL %d AR %d AI %d",
			 dispWind.grid, dispWind.scaleFactor, dispWind.refreshRate, dispWind.trigger,
			 dispWind.triggerLevel, dispWind.autoRefresh, dispWind.autoInterval);
}

void readRoot(char *root)
{
	extern lsrDispWindow dispWind;

	char *attrbStr;
	char *valueStr;
	char rootCopy[P_LROOTNAME];

	/* make a local copy of the root for strtok */
	strncpy(rootCopy, root, P_LROOTNAME);
	attrbStr = strtok(rootCopy, " ");
	while(attrbStr != (char *)NULL) {
		valueStr = strtok(NULL, " ");
		if(valueStr != (char *)NULL) {
			if(!strcmp(attrbStr, "G")) {
				sscanf(valueStr, "%d", &dispWind.grid);
			}
			else if(!strcmp(attrbStr, "S")) {
				sscanf(valueStr, "%hd", &dispWind.scaleFactor);
			}
			else if(!strcmp(attrbStr, "R")) {
				sscanf(valueStr, "%d", &dispWind.refreshRate);
			}
			else if(!strcmp(attrbStr, "T")) {
				sscanf(valueStr, "%hd", &dispWind.trigger);
			}
			else if(!strcmp(attrbStr, "TL")) {
				sscanf(valueStr, "%hd", &dispWind.triggerLevel);
			}
			else if(!strcmp(attrbStr, "AR")) {
				sscanf(valueStr, "%hd", &dispWind.autoRefresh);
			}
			else if(!strcmp(attrbStr, "AI")) {
				sscanf(valueStr, "%hd", &dispWind.autoInterval);
			}
		}

		attrbStr = strtok(NULL, " ");
	}
}

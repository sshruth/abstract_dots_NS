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
#include "ph_procSwitch.h"
#include "abimport.h"
#include "proto.h"

extern int rexSession;
extern int xloc;
extern int yloc;
extern int wsize;
extern int hsize;
extern char rootFile[P_LROOTNAME];

FILE *session_fp;

int init( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	struct stat statBuf;
	char *btns[] = { "&Yes", "&No" };
	char *btnFonts[] = { "&Helvetica09ba", "&Helvetica09ba"	};

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	if(apinfo->reason == ABR_PRE_REALIZE) return( Pt_CONTINUE );

	if(stat("./rt/rex_session", &statBuf) == 0) {
		if(statBuf.st_size > 0) {
			switch(PtAlert(link_instance, NULL, "Rex Session", NULL,
						   "Do you want to restore the previous session?", "Helvetica09ba",
						   2, (char const **)btns, (char const **)btnFonts, 1, 2, Pt_MODAL)) {
			case 1:
				if((session_fp = fopen("./rt/rex_session", "r")) != (FILE *)NULL) {
					rexSession = 1;
				}
			case 2:
				break;
			}
		}
	}

	/* start scribe and set it's state to running */
	if(!newproc("../scribe/scribe", 'r', 's')) {
		fprintf(stderr, "procSwitch(): Can't start scribe, exiting\n");
		PtExit(EXIT_FAILURE);
	}

	return( Pt_CONTINUE );
}

void readSession()
{
	int p_state;
	int pulseValue;
	char runState = 's';
	char windowState = 'i';
	char *attrbStr;
	char *valueStr;
	char procName[P_LPROCNAME];
	char instring[1024];

	while(fgets(instring, sizeof(instring), session_fp)) {
		if((attrbStr = strtok(instring, " ")) != (char *)NULL) {
			if((valueStr = strtok(NULL, "\n")) != (char *)NULL) {
				if(!strcmp(attrbStr, "REX_SESSION")) {
					if(!strcmp(valueStr, "BEGIN")) {
						rexSession = 1;
					}
					else if(!strcmp(valueStr, "END")) {
						fclose(session_fp);
						rexSession = 0;
						return;
					}
				}
				else if(!strcmp(attrbStr, "PROCESS")) {
					if(!strcmp(valueStr, "START")) {
						xloc = 0;
						yloc = 0;
						wsize = 0;
						hsize = 0;
						p_state = 0;
						memset(rootFile, 0, P_LROOTNAME);
						memset(procName, 0, P_LPROCNAME);
					}
					else if(!strcmp(valueStr, "STOP")) {
						newproc(procName, runState, windowState);
						return;
					}
				}
				else if(!strcmp(attrbStr, "NAME")) {
					strncpy(procName, valueStr, P_LPROCNAME);
				}
				else if(!strcmp(attrbStr, "ROOT")) {
					strncpy(rootFile, valueStr, P_LROOTNAME);
				}
				else if(!strcmp(attrbStr, "STATE")) {
					sscanf(valueStr, "%d", &p_state);
					if(p_state == 0) {
						runState = 's';
						windowState = 'i';
					}
					else {
						runState = 'r';
						windowState = 'n';
					}
				}
				else if(!strcmp(attrbStr, "X")) {
					sscanf(valueStr, "%d", &xloc);
				}
				else if(!strcmp(attrbStr, "Y")) {
					sscanf(valueStr, "%d", &yloc);
				}
				else if(!strcmp(attrbStr, "WIDTH")) {
					sscanf(valueStr, "%d", &wsize);
				}
				else if(!strcmp(attrbStr, "HEIGHT")) {
					sscanf(valueStr, "%d", &hsize);
				}
			}
		}
	}

	fclose(session_fp);
	rexSession = 0;
	return;
}

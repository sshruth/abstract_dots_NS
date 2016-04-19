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
#include "configRex.h"
#include "abimport.h"
#include "proto.h"


int writeCnfFile( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	FILE *cnfFile;
	int user_defines;
	char *linePtr;
	
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* open the cnf.h file for writing */
	if((cnfFile = fopen("../hdr/cnf.h", "w")) == NULL) {
		PtAskQuestion(ABW_base, NULL,
					  "Can't open ../hdr/cnf.h for writing", "Helvetica09ba",
					  "&Exit", NULL, NULL, 1 );
		PtExit(EXIT_FAILURE);
	}

	user_defines = 0;
	linePtr = rexlinetok(cnfHeader);
	while(linePtr) {
		/* don't process blank lines */
		if(!strcmp(linePtr, "\n")) fprintf(cnfFile, "\n");

		/* if this line identifies the block of user defines */
		else if(strstr(linePtr, "BEGIN_USER_DEFINES")) {
			user_defines = 1;
			fprintf(cnfFile, "%s\n", linePtr);
		}
		else if(strstr(linePtr, "END_USER_DEFINES")) {
			user_defines = 0;
			fprintf(cnfFile, "%s\n", linePtr);
		}

		/* if this is a line in the user defines block, parse the line */
		else if(user_defines) writeLine(cnfFile, linePtr);

		/* if this is not a line in the user defines block, just print it */
		else fprintf(cnfFile, "%s\n", linePtr);

		/* get the next line from the header */
		linePtr = rexlinetok((char *)NULL);
	}

	/* close the cnf.h file */
	fclose(cnfFile);

	/* sleep for a couple of seconds */
	sleep(2);

	/* now make rex */
  	system("make complete");

	return( Pt_CONTINUE );
}

void writeLine(FILE *cnfFile, char *linePtr)
{
	int define;
	char lineCopy[256];
	char *token;
	char string[16];

	/* make a copy of the line */
	strcpy(lineCopy, linePtr);

	/* parse the line */
	token = strtok(lineCopy, " \t");

	/* don't process comment lines */
	if(strchr(token, '*')) {
		fprintf(cnfFile, "%s\n", linePtr);
		return;
	}
	else if(!strcmp(token, "#define")) define = YES;
	else if(!strcmp(token, "#undef")) define = NO;
	else if(!strcmp(token, "#ifndef")) define = NDEF;
	else if(!strcmp(token, "#endif")) {
		fprintf(cnfFile, "%s\n", linePtr);
		return;
	}
	else define = -1;
	
	while(token = strtok(NULL, " \t")) {	/* get the rest of the tokens in the line */
		if(!strcmp(token, "AD_BASE")) {
			sprintf(string, "0x%x", a2dbase);
			fprintf(cnfFile, "#define %s %s\t", token, string);
			token = strtok(NULL, " \t");	/* eat the next token */
		}
		else if(!strcmp(token, "DA_BASE")) {
			sprintf(string, "0x%x", d2abase);
			fprintf(cnfFile, "#define %s %s\t", token, string);
			token = strtok(NULL, " \t");	/* eat the next token */
		}
		else if(!strcmp(token, "DIO_BASE")) {
			sprintf(string, "0x%x", diobase);
			fprintf(cnfFile, "#define %s %s\t", token, string);
			token = strtok(NULL, " \t");	/* eat the next token */
		}
		else if(!strcmp(token, "AD_VECT")) {
			sprintf(string, "%d", a2dvect);
			fprintf(cnfFile, "#define %s %s\t", token, string);
			token = strtok(NULL, " \t");	/* eat the next token */
		}
		else if(!strcmp(token, "NIATMIOX")) {
			if(a2dboard == NIATMIOX) fprintf(cnfFile, "#define %s\t", token);
			else fprintf(cnfFile, "#undef %s\t", token);
		}
		else if(!strcmp(token, "PCI_AtoD")) {
			if(define == NDEF) fprintf(cnfFile, "#ifndef %s\t", token);
			else {
				if(a2dboard == PCI_AtoD) fprintf(cnfFile, "#define %s\t", token);
				else fprintf(cnfFile, "#undef %s\t", token);
			}
		}
		else if(!strcmp(token, "ANDAS12")) {
			if(a2dboard == ANDAS12) fprintf(cnfFile, "#define %s\t", token);
			else fprintf(cnfFile, "#undef %s\t", token);
		}
		else if(!strcmp(token, "CIO_DA")) {
			fprintf(cnfFile, "#define %s\t", token);
		}
		else if(!strcmp(token, "PCI_DtoA")) {
			if(d2aboard == PCI_DtoA) fprintf(cnfFile, "#define %s\t", token);
			else fprintf(cnfFile, "#undef %s\t", token);
		}
		else if(!strcmp(token, "PCDIO")) {
			fprintf(cnfFile, "#define %s\t", token);
		}
		else if(!strcmp(token, "PCI_DIO")) {
			if(dioboard == PCI_DIO) fprintf(cnfFile, "#define %s\t", token);
			else fprintf(cnfFile, "#undef %s\t", token);
		}
		else if(!strcmp(token, "ISA_CT")) {
			if(ctboard == ISA_CT) fprintf(cnfFile, "define %s\t", token);
			else fprintf(cnfFile, "undef %s\t", token);
		}
		else if(!strcmp(token, "PCI_CT")) {
			if(ctboard == PCI_CT) fprintf(cnfFile, "define %s\t", token);
			else fprintf(cnfFile, "undef %s\t", token);
		}
		else if(!strcmp(token, "DCC20")) {
			fprintf(cnfFile, "#define %s\t", token);
		}
		else if(!strcmp(token, "LATCH_UNITS")) {
			if(unit == LATCH_UNIT) fprintf(cnfFile, "#define %s\t", token);
			else fprintf(cnfFile, "#undef %s\t", token);
		}
		else if(!strcmp(token, "MEXWEX_UNITS")) {
			if(unit == MEXWEX_UNIT) fprintf(cnfFile, "#define %s\t", token);
			else fprintf(cnfFile, "#undef %s\t", token);
		}
		else if(!strcmp(token, "TC_UNITS")) {
			if(unit == PLEXON_UNIT) fprintf(cnfFile, "#define %s\t", token);
			else fprintf(cnfFile, "#undef %s\t", token);
		}
		else if(!strcmp(token, "FAKE_UNITS")) {
			if(unit == FAKE_UNIT) fprintf(cnfFile, "#define %s\t", token);
			else fprintf(cnfFile, "#undef %s\t", token);
		}
		else if(!strcmp(token, "PCMESSAGE")) {
			if(pcmessageSwitch == YES) fprintf(cnfFile, "#define %s\t", token);
			else fprintf(cnfFile, "#undef %s\t", token);
		}
		else if(!strcmp(token, "PCS_SOCKET")) {
			if(socketSwitch == YES) fprintf(cnfFile, "#define %s\t", token);
 			else fprintf(cnfFile, "#undef %s\t", token);
		}
		else if(!strcmp(token, "HARDWARE_SWITCH")) {
			if(hardwareSwitch == YES) fprintf(cnfFile, "#define %s\t", token);
			else fprintf(cnfFile, "#undef %s\t", token);
		}
		else if(!strcmp(token, "SMI_TRACK")) {
			if(smiSwitch == YES) fprintf(cnfFile, "#define %s\t", token);
			else fprintf(cnfFile, "#undef %s\t", token);
		}
		else if(!strcmp(token, "PLEX_ECODES")) {
			if(plexEcodeSwitch == YES) fprintf(cnfFile, "#define %s\t", token);
			else fprintf(cnfFile, "#undef %s\t", token);
		}
		else {
			fprintf(cnfFile, "%s ", token);
		}
	}
	fprintf(cnfFile, "\n");
	return;
}

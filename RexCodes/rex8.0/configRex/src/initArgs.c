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

/* Application Options string */
const char ApOptions[] =
	AB_OPTIONS ""; /* Add your options in the "" */

char *cnfHeader;

int a2dboard;
int a2dvect;
int a2dbase;
int d2aboard;
int d2abase;
int dioboard;
int diobase;
int ctboard;
int unit;
int pcmessageSwitch;
int socketSwitch;
int hardwareSwitch;
int smiSwitch;
int plexEcodeSwitch;

int initArgs( int argc, char *argv[] )
{
	FILE *cnfFile;
	struct stat statBuf;
	off_t bytesRead;
	int i;
	char cnfName[32];

	/* eliminate 'unreferenced' warnings */
	argc = argc, argv = argv;

	/* stat the cnf.h file */
	if(stat("../hdr/cnf.h", &statBuf) == 0) {
		strcpy(cnfName, "../hdr/cnf.h");
	}
	/* stat basecnf.h file */
	else if(stat("../hdr/protocnf.h", &statBuf) == 0) {
		strcpy(cnfName, "../hdr/protocnf.h");
	}
	else {
		fprintf(stderr, "can't stat header file\n");
		PtExit(EXIT_FAILURE);
	}

	if((cnfFile = fopen(cnfName, "r")) == NULL) {
		fprintf(stderr, "Can't open header file %s\n", cnfName);
		PtExit(EXIT_FAILURE);
	}

	/* allocate space to hold the cnf.h file */
	if((cnfHeader = (char *)malloc((statBuf.st_size + 10) * sizeof(char))) == 0) {
		fprintf(stderr, "Can't malloc space for header file %s\n", cnfName);
		PtExit(EXIT_FAILURE);
	}

	/* initialize the header file buffer */
	for(i = 0; i < statBuf.st_size + 10; i++) cnfHeader[i] = 0;

	/* read in the header file */
	if((bytesRead = fread(cnfHeader, sizeof(char), statBuf.st_size, cnfFile)) < statBuf.st_size) {
		fprintf(stderr, "Only read %d bytes of file %s\n", bytesRead, cnfName);
		PtExit(EXIT_FAILURE);
	}

	fclose(cnfFile);

	parseFile();

	return( Pt_CONTINUE );
}

void parseFile()
{
	int user_defines = 0;
	char *linePtr;

	linePtr = rexlinetok(cnfHeader);
	while(linePtr = rexlinetok((char *)NULL)) {
		if(strstr(linePtr, "BEGIN_USER_DEFINES")) user_defines = 1;
		else if(strstr(linePtr, "END_USER_DEFINES")) user_defines = 0;
		else if(user_defines) readLine(linePtr);
	}
}

void readLine(char *linePtr)
{
	char lineCopy[256];
	char *token;
	int define;

	/* make a copy of the line */
	strcpy(lineCopy, linePtr);

	/* parse the line */
	token = strtok(lineCopy, " \t");
	if(!strcmp(token, "#define")) define = YES;
	else if(!strcmp(token, "#undef")) define = NO;
	else if(!strcmp(token, "#ifndef")) define = NDEF;
	else define = -1;

	while(token = strtok(NULL, " \t")) {	/* get the rest of the tokens in the line */
		if(!strcmp(token, "AD_BASE")) {
			token = strtok(NULL, " \t");	/* get the next token */
			sscanf(token, "%x", &a2dbase);
			break;							/* don't process comments */
		}
		else if(!strcmp(token, "DA_BASE")) {
			token = strtok(NULL, " \t");	/* get the next token */
			sscanf(token, "%x", &d2abase);
			break;							/* don't process comments */
		}
		else if(!strcmp(token, "DIO_BASE")) {
			token = strtok(NULL, " \t");	/* get the next token */
			sscanf(token, "%x", &diobase);
			break;
		}
		else if(!strcmp(token, "AD_VECT")) {
			token = strtok(NULL, " \t");	/* get the next token */
			sscanf(token, "%d", &a2dvect);
			break;
		}
		else if(!strcmp(token, "DT2821")) {
			if(define == YES) a2dboard = DT2821;
			break;
		}
		else if(!strcmp(token, "NIATMIO")) {
			if(define == YES) a2dboard = NIATMIO;
			break;
		}
		else if(!strcmp(token, "NIATMIOX")) {
			if(define == YES) a2dboard = NIATMIOX;
			break;
		}
		else if(!strcmp(token, "AC5508HR")) {
			if(define == YES) a2dboard = AC5508HR;
			break;
		}
		else if(!strcmp(token, "AC5525MF")) {
			if(define == YES) a2dboard = AC5525MF;
			break;
		}
		else if(!strcmp(token, "ANLSDAS16")) {
			if(define == YES) a2dboard = ANLSDAS16;
			break;
		}
		else if(!strcmp(token, "ANDAS12")) {
			if(define == YES) a2dboard = ANDAS12;
			break;
		}
		else if(!strcmp(token, "PCI_AtoD")) {
			if(define == YES) a2dboard = PCI_AtoD;
			break;
		}
		else if(!strcmp(token, "PCI_DtoA")) {
			if(define == YES) d2aboard = PCI_DtoA;
			else if(define == NO) d2aboard = ISA_DtoA;
			break;
		}
		else if(!strcmp(token, "CIO_DA")) {
			break;
		}
		else if(!strcmp(token, "PCI_DIO")) {
			if(define == YES) dioboard = PCI_DIO;
			else if(define == NO) dioboard = ISA_DIO;
			break;
		}
		else if(!strcmp(token, "PCDIO")) {
			break;
		}
		else if(!strcmp(token, "ISA_CT")) {
			if(define == YES) ctboard = ISA_CT;
			break;
		}
		else if(!strcmp(token, "PCI_CT")) {
			if(define == YES) ctboard = PCI_CT;
			break;
		}
		else if(!strcmp(token, "DCC20")) {
			break;
		}
		else if(!strcmp(token, "LATCH_UNITS")) {
			if(define == YES) unit = LATCH_UNIT;
			break;
		}
		else if(!strcmp(token, "MEXWEX_UNITS")) {
			if(define == YES) unit = MEXWEX_UNIT;
			break;
		}
		else if(!strcmp(token, "TC_UNITS")) {
			if(define == YES) unit = PLEXON_UNIT;
			break;
		}
		else if(!strcmp(token, "FAKE_UNITS")) {
			if(define == YES) unit = FAKE_UNIT;
			break;
		}
		else if(!strcmp(token, "PCMESSAGE")) {
			pcmessageSwitch = define;
			break;
		}
		else if(!strcmp(token, "PCS_SOCKET")) {
			socketSwitch = define;
			break;
		}
		else if(!strcmp(token, "HARDWARE_SWITCH")) {
			hardwareSwitch = define;
			break;
		}
		else if(!strcmp(token, "SMI_TRACK")) {
			smiSwitch = define;
			break;
		}
		else if(!strcmp(token, "PLEX_ECODES")) {
			plexEcodeSwitch = define;
			break;
		}
	}
	return;
}


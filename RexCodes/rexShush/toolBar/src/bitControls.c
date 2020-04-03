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

/* variables controling how the bit toggles act */
#define BIT_TOGGLE 0
#define BIT_PULSE 1
#define BIT_SET 2
#define BIT_RESET 3

#define IMMEDIATE 0
#define BATCH 1
int sendMode[6];
unsigned char outBits[6];
int startPort = 0;

/* set up arrays for widget button names */
extern int portLabels[6];
extern int portBits[6][8];
extern int portControls[6][4];

int initBitsDialog( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;
	DIO_DEV *dp;
	int i;

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, m_bitdio, 0);
	PtSetResources(ABW_bitsDeviceNumb, 1, &arg);

	PtSetArg(&arg, Pt_ARG_FILL_COLOR, Pg_TRANSPARENT, 0);
	PtSetResources(ABW_bitsP0Group, 1, &arg);
	PtSetResources(ABW_bitsP1Group, 1, &arg);
	PtSetResources(ABW_bitsP2Group, 1, &arg);
	PtSetResources(ABW_bitsP4Group, 1, &arg);
	PtSetResources(ABW_bitsP5Group, 1, &arg);
	PtSetResources(ABW_bitsP6Group, 1, &arg);

	/* set the colors of the port bit groups */
	for(dp = dio_dev; dp->dio_devid != 0; dp++) {
		if(dp->dio_devid != PCDIO_DIO) continue;
		i = startPort - 1;
		while(dp->dio_pinit[++i]) {
			switch(i - startPort) {
			case 0:
				if(dp->dio_pinit[i] == (D_IN | D_REV)) {
					PtSetArg(&arg, Pt_ARG_FILL_COLOR, Pg_WHITE, 0);
				}
				else if(dp->dio_pinit[i] == (D_OUT | D_REV)) {
					PtSetArg(&arg, Pt_ARG_FILL_COLOR, Pg_RED, 0);
				}
				PtSetResources(ABW_bitsP0Group, 1, &arg);
				break;
			case 1:
				if(dp->dio_pinit[i] == (D_IN | D_REV)) {
					PtSetArg(&arg, Pt_ARG_FILL_COLOR, Pg_WHITE, 0);
				}
				else if(dp->dio_pinit[i] == (D_OUT | D_REV)) {
					PtSetArg(&arg, Pt_ARG_FILL_COLOR, Pg_RED, 0);
				}
				PtSetResources(ABW_bitsP1Group, 1, &arg);
				break;
			case 2:
				if(dp->dio_pinit[i] == (D_IN | D_REV)) {
					PtSetArg(&arg, Pt_ARG_FILL_COLOR, Pg_WHITE, 0);
				}
				else if(dp->dio_pinit[i] == (D_OUT | D_REV)) {
					PtSetArg(&arg, Pt_ARG_FILL_COLOR, Pg_RED, 0);
				}
				PtSetResources(ABW_bitsP2Group, 1, &arg);
				break;
			case 4:
				if(dp->dio_pinit[i] == (D_IN | D_REV)) {
					PtSetArg(&arg, Pt_ARG_FILL_COLOR, Pg_WHITE, 0);
				}
				else if(dp->dio_pinit[i] == (D_OUT | D_REV)) {
					PtSetArg(&arg, Pt_ARG_FILL_COLOR, Pg_RED, 0);
				}
				PtSetResources(ABW_bitsP4Group, 1, &arg);
				break;
			case 5:
				if(dp->dio_pinit[i] == (D_IN | D_REV)) {
					PtSetArg(&arg, Pt_ARG_FILL_COLOR, Pg_WHITE, 0);
				}
				else if(dp->dio_pinit[i] == (D_OUT | D_REV)) {
					PtSetArg(&arg, Pt_ARG_FILL_COLOR, Pg_RED, 0);
				}
				PtSetResources(ABW_bitsP5Group, 1, &arg);
				break;
			case 6:
				if(dp->dio_pinit[i] == (D_IN | D_REV)) {
					PtSetArg(&arg, Pt_ARG_FILL_COLOR, Pg_WHITE, 0);
				}
				else if(dp->dio_pinit[i] == (D_OUT | D_REV)) {
					PtSetArg(&arg, Pt_ARG_FILL_COLOR, Pg_RED, 0);
				}
				PtSetResources(ABW_bitsP6Group, 1, &arg);
				break;
			default:
				break;
			}
		}
	}

	return( Pt_CONTINUE );
}

int bitsIODevice( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t arg;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the value from the widget */
	PtSetArg(&arg, Pt_ARG_NUMERIC_VALUE, 0, 0);
	PtGetResources(widget, 1, &arg);
	m_bitdio = (int)arg.value;

	return( Pt_CONTINUE );
}

int bitsStartPortNumber( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[2];
	PtWidget_t *link_instance;
	PtWidget_t *bitLabel;
	DIO_DEV *dp;
	int i;
	char label[16];

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get the value from the widget */
	PtSetArg(&args[0], Pt_ARG_NUMERIC_VALUE, 0, 0);
	PtGetResources(widget, 1, args);
	startPort = (int)args[0].value;

	/* get a pointer to the window containing this widget
	 * (used to get pointers to other widgets)
	 */
	link_instance = ApGetInstance(widget);

	/* set the port labels */
	for(i = 0; i < 3; i++) {
		bitLabel = ApGetWidgetPtr(link_instance, portLabels[i]);
		sprintf(label, "Port %d", i + startPort);
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, label, 0);
		PtSetResources(bitLabel, 1, args);
	}
	for(i = 3; i < 6; i++) {
		bitLabel = ApGetWidgetPtr(link_instance, portLabels[i]);
		sprintf(label, "Port %d", i + 1 + startPort);
		PtSetArg(&args[0], Pt_ARG_TEXT_STRING, label, 0);
		PtSetResources(bitLabel, 1, args);
	}

	/* set the colors of the port bit groups */
	PtSetArg(&args[0], Pt_ARG_FILL_COLOR, Pg_TRANSPARENT, 0);
	PtSetResources(ABW_bitsP0Group, 1, args);
	PtSetResources(ABW_bitsP1Group, 1, args);
	PtSetResources(ABW_bitsP2Group, 1, args);
	PtSetResources(ABW_bitsP4Group, 1, args);
	PtSetResources(ABW_bitsP5Group, 1, args);
	PtSetResources(ABW_bitsP6Group, 1, args);
	for(dp = dio_dev; dp->dio_devid != 0; dp++) {
		if(dp->dio_devid != PCDIO_DIO) continue;
		i = startPort - 1;
		while(dp->dio_pinit[++i]) {
			PtSetArg(&args[0], Pt_ARG_FILL_COLOR, Pg_TRANSPARENT, 0);
			switch(i - startPort) {
			case 0:
				if(dp->dio_pinit[i] == (D_IN | D_REV)) {
					PtSetArg(&args[0], Pt_ARG_FILL_COLOR, Pg_WHITE, 0);
				}
				else if(dp->dio_pinit[i] == (D_OUT | D_REV)) {
					PtSetArg(&args[0], Pt_ARG_FILL_COLOR, Pg_RED, 0);
				}
				PtSetResources(ABW_bitsP0Group, 1, args);
				break;
			case 1:
				if(dp->dio_pinit[i] == (D_IN | D_REV)) {
					PtSetArg(&args[0], Pt_ARG_FILL_COLOR, Pg_WHITE, 0);
				}
				else if(dp->dio_pinit[i] == (D_OUT | D_REV)) {
					PtSetArg(&args[0], Pt_ARG_FILL_COLOR, Pg_RED, 0);
				}
				PtSetResources(ABW_bitsP1Group, 1, args);
				break;
			case 2:
				if(dp->dio_pinit[i] == (D_IN | D_REV)) {
					PtSetArg(&args[0], Pt_ARG_FILL_COLOR, Pg_WHITE, 0);
				}
				else if(dp->dio_pinit[i] == (D_OUT | D_REV)) {
					PtSetArg(&args[0], Pt_ARG_FILL_COLOR, Pg_RED, 0);
				}
				PtSetResources(ABW_bitsP2Group, 1, args);
				break;
			case 4:
				if(dp->dio_pinit[i] == (D_IN | D_REV)) {
					PtSetArg(&args[0], Pt_ARG_FILL_COLOR, Pg_WHITE, 0);
				}
				else if(dp->dio_pinit[i] == (D_OUT | D_REV)) {
					PtSetArg(&args[0], Pt_ARG_FILL_COLOR, Pg_RED, 0);
				}
				PtSetResources(ABW_bitsP4Group, 1, args);
				break;
			case 5:
				if(dp->dio_pinit[i] == (D_IN | D_REV)) {
					PtSetArg(&args[0], Pt_ARG_FILL_COLOR, Pg_WHITE, 0);
				}
				else if(dp->dio_pinit[i] == (D_OUT | D_REV)) {
					PtSetArg(&args[0], Pt_ARG_FILL_COLOR, Pg_RED, 0);
				}
				PtSetResources(ABW_bitsP5Group, 1, args);
				break;
			case 6:
				if(dp->dio_pinit[i] == (D_IN | D_REV)) {
					PtSetArg(&args[0], Pt_ARG_FILL_COLOR, Pg_WHITE, 0);
				}
				else if(dp->dio_pinit[i] == (D_OUT | D_REV)) {
					PtSetArg(&args[0], Pt_ARG_FILL_COLOR, Pg_RED, 0);
				}
				PtSetResources(ABW_bitsP6Group, 1, args);
				break;
			default:
				break;
			}
		}
	}

	return( Pt_CONTINUE );
}

int toggleBit( PtWidget_t*widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	int i, j;
	int bit, port;
	int portIndex, bitIndex;
	unsigned char mask;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* determine the port and bit number of this widget */
	for(i = 0; i < 3; i++) {
		for(j = 0; j < 8; j++) {
			if(ApName(widget) == portBits[i][j]) {
				portIndex = i;
				port = i + startPort;
				bitIndex = j;
				bit = (i * 8) + j;
			}
		}
	}
	for(i = 3; i < 6; i++) {
		for(j = 0; j < 8; j++) {
			if(ApName(widget) == portBits[i][j]) {
				portIndex = i;
				port = i + 1 + startPort;
				bitIndex = j;
				bit = ((i - 3) * 8) + j;
			}
		}
	}

	/* if the output for this port is in batch mode
	 * set or clear the appropriate bit of the output value
	 */
	if(sendMode[portIndex]) {
		mask = 0;
		mask = 1 << bitIndex;
		if(cbinfo->reason == Pt_CB_ARM) {
			outBits[portIndex] |= mask;
		}
		else if(cbinfo->reason == Pt_CB_DISARM) {
			outBits[portIndex] &= ~mask;
		}
	}

	/* if the output for this port is in immediate mode
	 * set or clear the appropriate bit of the output port
	 */
	else {
		mask = 0;
		mask = 1 << bitIndex;
		if(cbinfo->reason == Pt_CB_ARM) {
			outBits[portIndex] |= mask;
			dio_on(Dio_id(m_bitdio, port, outBits[portIndex]));
#ifdef NIH_ACUC			
			i_b->inthdr.i_lastCorrect = i_b->i_time;
#endif
		}
		else if(cbinfo->reason == Pt_CB_DISARM) {
			outBits[portIndex] &= ~mask;
			dio_off(Dio_id(m_bitdio, port, mask));
		}
	}
	return( Pt_CONTINUE );
}

int setToggleMode( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[2];
	PtWidget_t *link_instance;
	PtWidget_t *bitToggle;
	long *flags;
	int port;
	int i;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* determine the port number of this widget */
	for(i = 0; i < 6; i++) {
		if(ApName(widget) == portControls[i][BIT_TOGGLE]) port = i;
	}

	/* get a pointer to the window containing this widget
	 * (used to get pointers to other widgets)
	 */
	link_instance = ApGetInstance(widget);

	/* set the toggle flag on the bits of this port */
	PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_TOGGLE);
	PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
	for(i = 0; i < 8; i++) {
		bitToggle = ApGetWidgetPtr(link_instance, portBits[port][i]);
		PtSetResources(bitToggle, 2, args);
	}

	/* get the state of the toggle button */
	PtSetArg(&args[0], Pt_ARG_FLAGS, &flags, 0);
	PtGetResources(widget, 1, args);
	if(*flags & Pt_SET) sendMode[port] = IMMEDIATE;
	else sendMode[port] = BATCH;

	return( Pt_CONTINUE );
}

int setPulseMode( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[2];
	PtWidget_t *link_instance;
	PtWidget_t *bitToggle;
	long *flags;
	int port;
	int i;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* determine the port number of this widget */
	for(i = 0; i < 6; i++) {
		if(ApName(widget) == portControls[i][BIT_PULSE]) port = i;
	}

	/* get a pointer to the window containing this widget
	 * (used to get pointers to other widgets)
	 */
	link_instance = ApGetInstance(widget);

	/* get the state of the toggle button */
	PtSetArg(&args[0], Pt_ARG_FLAGS, &flags, 0);
	PtGetResources(widget, 1, args);

	if(*flags & Pt_SET) {
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_TOGGLE);
		PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		sendMode[port] = IMMEDIATE;
	}
	else {
		PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_TRUE, Pt_TOGGLE);
		PtSetArg(&args[1], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
		sendMode[port] = BATCH;
	}

	for(i = 0; i < 8; i++) {
		bitToggle = ApGetWidgetPtr(link_instance, portBits[port][i]);
		PtSetResources(bitToggle, 2, args);
	}

	return( Pt_CONTINUE );
}

int setBits( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[2];
	long *flags;
	int port;
	int portIndex;
	int i;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* determine the port number of this widget */
	for(i = 0; i < 3; i++) {
		if(ApName(widget) == portControls[i][BIT_SET]) {
			port = i + startPort;
			portIndex = i;
		}
	}
	for(i = 3; i < 6; i++) {
		if(ApName(widget) == portControls[i][BIT_SET]) {
			port = i + 1 + startPort;
			portIndex = i;
		}
	}

	/* get the state of the toggle button */
	PtSetArg(&args[0], Pt_ARG_FLAGS, &flags, 0);
	PtGetResources(widget, 1, args);
	if(*flags & Pt_SET) {
		dio_on(Dio_id(m_bitdio, port, outBits[portIndex]));
#ifdef NIH_ACUC
		i_b->inthdr.i_lastCorrect = i_b->i_time;
#endif
	}
	else {
		dio_off(Dio_id(m_bitdio, port, outBits[portIndex]));
	}

	return( Pt_CONTINUE );
}

int resetBits( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtArg_t args[2];
	PtWidget_t *link_instance;
	PtWidget_t *bitToggle;
	int port;
	int portIndex;
	int i;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* determine the port number of this widget */
	for(i = 0; i < 3; i++) {
		if(ApName(widget) == portControls[i][BIT_RESET]) {
			port = i + startPort;
			portIndex = i;
		}
	}
	for(i = 3; i < 6; i++) {
		if(ApName(widget) == portControls[i][BIT_RESET]) {
			port = i + 1 + startPort;
			portIndex = i;
		}
	}

	/* get a pointer to the window containing this widget
	 * (used to get pointers to other widgets)
	 */
	link_instance = ApGetInstance(widget);

	/* clear all of the bit toggles on this port */
	PtSetArg(&args[0], Pt_ARG_FLAGS, Pt_FALSE, Pt_SET);
	for(i = 0; i < 8; i++) {
		bitToggle = ApGetWidgetPtr(link_instance, portBits[portIndex][i]);
		PtSetResources(bitToggle, 1, args);
	}
	outBits[portIndex] = 0;
	dio_off(Dio_id(m_bitdio, port, 0xff));
	return( Pt_CONTINUE );

}

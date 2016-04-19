#include <stdio.h>
#include "cnf.h"
#include "GLcommand.h"

#ifdef PCS_SOCKET
#include "pcsSocket.h"
#else
#include "pcmsg.h"
#endif

unsigned char vexbuf[16384] = { 0 };
unsigned int recvdVexMsgLen;
long vx, vy;
union {
	unsigned char c[4];
	short s[2];
	float f;
} f2c;

/*
 * functions to handle REX to PC communications
 */

/*
 * This function returns 0 if a new message has arrived from
 * VEX.  It returns a 1 if no new messages are waiting.
 * This function can be used to escape from a state when a
 * message arrives from VEX.
 */
int tst_rx_new()
{
#ifdef PCS_SOCKET
	recvdVexMsgLen = pcsRecvdVexMsgLen();
	if(recvdVexMsgLen) return(0);
	else return(1);
#else
	if(pcm[0].pcm_status & PCM_MSG_WAITING) return(0);
	else return(1);
#endif
}
	
/*
 * This function returns a 1 if the transmission line is busy.
 * If the transmission line is ready to send a new message, this
 * function returns a 0.  This function should be used as the
 * only escape into a state that calls an action that sends
 * a message to VEX.  This will ensure that previous messages
 * to VEX are not overwritten by new messages.
 */
int tst_tx_rdy()
{
#ifdef PCS_SOCKET
	return(0);
#else
	if(pcm[0].pcm_status & PCM_TX_BUSY) return(1);
	else return(0);
#endif
}

/*
 * This action sends the message buffer "vexbuf" to VEX.
 */
to_vex(unsigned short len_of_msg)
{
#ifdef PCS_SOCKET
	pcsSendVex(vexbuf, len_of_msg);
#else
	/* short messages */
	if(len_of_msg <= 64) pcm_send(0, &vexbuf, PCM_TYPE1, len_of_msg);

	/* long messages */
	else pcm_send(0, &vexbuf, PCM_TYPE3, len_of_msg);
#endif
	return(0);
}

/*
 * This function returns the message sent from VEX.
 */
unsigned char *vex_message()
{
	unsigned char *msgfvex;

#ifdef PCS_SOCKET
	if(pcsRecvdVexMsgLen()) msgfvex = pcsRecvdVexMsg();
	else msgfvex = 0;
#else
	msgfvex = pcm[0].pcm_rbufp;
#endif
	return(msgfvex);
}

/*
 * This function extracts the header signal sent from VEX
 */
unsigned char vex_code(unsigned char *msgfvex)
{
	if(msgfvex) return(msgfvex[0]);
	else return(0);
}

/*
 * This action gets the location of an object
 * from VEX and loads it into vx and vy.
 */
vex_location(unsigned char *msgfvex)
{
	unsigned char code;
	short i;

	if(!msgfvex) return(0);

	code = *msgfvex++;
	
	for(i = 0; i < 4; i++) f2c.c[i] = *msgfvex++;
	vx = (long)f2c.f;

	for(i = 0; i < 4; i++) f2c.c[i] = *msgfvex++;
	vy = (long)f2c.f;

	return(0);
}

/*
 * This function is used to pack a floating point value
 * into a byte array for transmission to VEX.
 */
unsigned char *float2byte(float value)
{
	f2c.f = value;
	return(f2c.c);
}

/*
 * This function extracts the high order byte of a short int
 */
unsigned char hibyte(short sval)
{
	f2c.s[0] = sval;
	return(f2c.c[1]);
}

/*
 * This function extracts the low order byte of a short int
 */
unsigned char lobyte(short sval)
{
	f2c.s[0] = sval;
	return(f2c.c[0]);
}

/*
 * This action sets the vex erase method
 */
int PvexEraseMethod(int *method)
{
	unsigned short index;

	index = 0;
	vexbuf[index++] = SET_ERASE_METHOD;
	vexbuf[index++] = lobyte(*method);

	to_vex(index);
	return(0);
}

/*
 * This action turns the rex video sync on or off
 */
int PvexVideoSync(int *sync)
{
	unsigned short index;

	index = 0;
	if(*sync == 1) vexbuf[index++] = ENABLE_REX_VIDEO_SYNC;
	else if(*sync == 0) vexbuf[index++] = DISABLE_REX_VIDEO_SYNC;
	else {
		vexbuf[index++] = SET_REX_VIDEO_SYNC;
		vexbuf[index++] = hibyte(*sync);
		vexbuf[index++] = lobyte(*sync);
	}

	to_vex(index);
	return(0);
}

/*
 * This action turns the rex digital sync on or off
 */
int PvexDigitalSync(int *sync)
{
	unsigned short index;

	index = 0;
	if(*sync) vexbuf[index++] = ENABLE_REX_DIGITAL_SYNC;
	else vexbuf[index++] = DISABLE_REX_DIGITAL_SYNC;

	to_vex(index);
	return(0);
}

/*
 * This action sets the rex scale
 */
int PvexSetRexScale(int *distance, int *width)
{
	unsigned short index;

	index = 0;
	vexbuf[index++] = SET_REX_SCALE;
	vexbuf[index++] = hibyte(*distance);
	vexbuf[index++] = lobyte(*distance);
	vexbuf[index++] = hibyte(*width);
	vexbuf[index++] = lobyte(*width);

	to_vex(index);
	return(0);
}

/*
 * This action set the background luminance
 */
int PvexSetBackLum(int *r, int *g, int *b)
{
	unsigned short index;

	index = 0;
	vexbuf[index++] = SET_BACK_LUM;
	vexbuf[index++] = lobyte(*r);
	vexbuf[index++] = lobyte(*g);
	vexbuf[index++] = lobyte(*b);

	to_vex(index);
	return(0);
}

/*
 * This action sets the fixation point colors
 */
int PvexSetFixColors(int *nr, int *ng, int *nb, int *dr, int *dg, int *db)
{
	unsigned short index;

	index = 0;
	vexbuf[index++] = SET_FP_ON_CLR;
	vexbuf[index++] = lobyte(*nr);
	vexbuf[index++] = lobyte(*ng);
	vexbuf[index++] = lobyte(*nb);
	vexbuf[index++] = SET_FP_DIM_CLR;
	vexbuf[index++] = lobyte(*dr);
	vexbuf[index++] = lobyte(*dg);
	vexbuf[index++] = lobyte(*db);

	to_vex(index);
	return(0);
}

/*
 * This action sets the size of the fixation point
 */
int PvexSetFixSiz(int *size)
{
	unsigned short index;

	index = 0;
	vexbuf[index++] = SET_FP_SIZE;
	vexbuf[index++] = lobyte(*size);

	to_vex(index);
	return(0);
}

/*
 * This action sets the foreground and background luminances of
 * a list of objects
 */
int PvexSetStimLuminances(int *nObjects, int *objList, int *fgList, int *bgList)
{
	unsigned short index;
	int i;

	index = 0;
	vexbuf[index++] = SET_STIM_LUMINANCES;
	vexbuf[index++] = lobyte(*nObjects);
	for(i = 0; i < *nObjects; i++) {
		vexbuf[index++] = lobyte(objList[i]);
		vexbuf[index++] = lobyte(fgList[i]);
		vexbuf[index++] = lobyte(bgList[i]);
	}

	to_vex(index);
	return(0);
}

/*
 * This action sets the foreground and background colors of
 * a list of objects
 */
int PvexSetStimColors(int *nObjects, int *objList,
					  int *fgrList, int *fggList, int *fgbList,
					  int *bgrList, int *bggList, int *bgbList)
{
	unsigned short index;
	int i;

	index = 0;
	vexbuf[index++] = SET_STIM_COLORS;
	vexbuf[index++] = lobyte(*nObjects);
	for(i = 0; i < *nObjects; i++) {
		vexbuf[index++] = lobyte(objList[i]);
		vexbuf[index++] = lobyte(fgrList[i]);
		vexbuf[index++] = lobyte(fggList[i]);
		vexbuf[index++] = lobyte(fgbList[i]);
		vexbuf[index++] = lobyte(bgrList[i]);
		vexbuf[index++] = lobyte(bggList[i]);
		vexbuf[index++] = lobyte(bgbList[i]);
	}

	to_vex(index);
	return(0);
}

/*
 * This action sets the red, green, blue, and alpha channel flags of
 * a list of objects
 */
int PvexSetColorMask(int *nObjects, int *objList, int *redList,
					 int *greenList, int *blueList, int *alphaList)
{
	unsigned short index;
	int i;

	index = 0;
	vexbuf[index++] = SET_COLOR_MASK;
	vexbuf[index++] = lobyte(*nObjects);
	for(i = 0; i < *nObjects; i++) {
		vexbuf[index++] = lobyte(objList[i]);
		vexbuf[index++] = lobyte(redList[i]);
		vexbuf[index++] = lobyte(greenList[i]);
		vexbuf[index++] = lobyte(blueList[i]);
		vexbuf[index++] = lobyte(alphaList[i]);
	}

	to_vex(index);
	return(0);
}

/*
 * This action sets up a gray scale in each object's color lookup table
 */
int PvexSetGrayScale(int *start, int *length)
{
	unsigned short index;

	index = 0;
	vexbuf[index++] = SET_GRAY_SCALE;
	vexbuf[index++] = lobyte(*start);
	vexbuf[index++] = lobyte(*length);

	to_vex(index);
	return(0);
}

/*
 * This action sets up a gray scale in the specified objects' color lookup tables
 */
int PvexSetObjectGrayScale(int *nObjects, int *objList, int *startList, int *lengthList)
{
	unsigned short index;
	int i;

	index = 0;
	vexbuf[index++] = SET_OBJECT_GRAY_SCALE;
	vexbuf[index++] = lobyte(*nObjects);
	for(i = 0; i < *nObjects; i++) {
		vexbuf[index++] = lobyte(objList[i]);
		vexbuf[index++] = lobyte(startList[i]);
		vexbuf[index++] = lobyte(lengthList[i]);
	}

	to_vex(index);
	return(0);
}

/*
 * This action sets colors in each object's color lookup table
 */
int PvexSetLutEntryClr(int *nEntries, int *entryList, int *rList, int *gList, int *bList)
{
	unsigned short index;
	int i;

	index = 0;
	vexbuf[index++] = SET_LUT_ENTRY_CLR;
	vexbuf[index++] = lobyte(*nEntries);
	for(i = 0; i < *nEntries; i++) {
		vexbuf[index++] = lobyte(entryList[i]);
		vexbuf[index++] = lobyte(rList[i]);
		vexbuf[index++] = lobyte(gList[i]);
		vexbuf[index++] = lobyte(bList[i]);
	}

	to_vex(index);
	return(0);
}

/*
 * This action sets colors in the specified objects' color lookup tables
 * This action
 */
int PvexSetObjectLutEntryClr(int *nObjects, int *objList, int *nEntries,
							 int *entryList, int *rList, int *gList, int *bList)
{
	unsigned short index;
	int i, j;

	index = 0;
	vexbuf[index++] = SET_OBJECT_LUT_ENTRY_CLR;
	vexbuf[index++] = lobyte(*nObjects);
	for(i = 0; i < *nObjects; i++) {
		vexbuf[index++] = lobyte(objList[i]);
		vexbuf[index++] = lobyte(*nEntries);
		for(j = 0; j < *nEntries; j++) {
			vexbuf[index++] = lobyte(entryList[j]);
			vexbuf[index++] = lobyte(rList[j]);
			vexbuf[index++] = lobyte(gList[j]);
			vexbuf[index++] = lobyte(bList[j]);
		}
	}

	to_vex(index);
	return(0);
}

/*
 * This action turns off the fixation point and all stimuli
 */
int PvexAllOff(void)
{
	unsigned short index;

	index = 0;
	vexbuf[index++] = ALL_OFF;

	to_vex(index);
	return(0);
}

/*
 * This action switches the fixation point on or off
 */
int PvexSwitchFix(int *flag)
{
	unsigned short index;

	index = 0;
	vexbuf[index++] = SWITCH_FIX_POINT;
	vexbuf[index++] = lobyte(*flag);

	to_vex(index);
	return(0);
}
	
/*
 * This action turns off the fixation point
 */
int PvexDimFix(void)
{
	unsigned short index;

	index = 0;
	vexbuf[index++] = DIM_FIX_POINT;

	to_vex(index);
	return(0);
}
	
/*
 * This action draws a number of objects into the drawing page
 * but doesn't swap the buffers to display them
 */
int PvexPreloadStim(int *nObjects, int *objList, int *swtchList)
{
	unsigned short index;
	int i;

	index = 0;
	vexbuf[index++] = PRELOAD_STIM;
	vexbuf[index++] = lobyte(*nObjects);
	for(i = 0; i < *nObjects; i++) {
		vexbuf[index++] = lobyte(objList[i]);
		vexbuf[index++] = lobyte(swtchList[i]);
	}

	to_vex(index);
	return(0);
}

/*
 * This action swaps the display and draw buffers
 */
int PvexSwapBuffers(void)
{
	unsigned short index;

	index = 0;
	vexbuf[index++] = SWAP_BUFFERS;

	to_vex(index);
	return(0);
}

/*
 * This action switches a number of objects on or off
 */
int PvexSwitchStim(int *nObjects, int *objList, int *swtchList)
{
	unsigned short index;
	int i;

	index = 0;
	vexbuf[index++] = SWITCH_STIM;
	vexbuf[index++] = lobyte(*nObjects);
	for(i = 0; i < *nObjects; i++) {
		vexbuf[index++] = lobyte(objList[i]);
		vexbuf[index++] = lobyte(swtchList[i]);
	}

	to_vex(index);
	return(0);
}

/*
 * This action sets the switches a number of objects on or off
 */
int PvexSetStimSwitch(int *nObjects, int *objList, int *swtchList)
{
	unsigned short index;
	int i;

	index = 0;
	vexbuf[index++] = SET_STIM_SWITCH;
	vexbuf[index++] = lobyte(*nObjects);
	for(i = 0; i < *nObjects; i++) {
		vexbuf[index++] = lobyte(objList[i]);
		vexbuf[index++] = lobyte(swtchList[i]);
	}

	to_vex(index);
	return(0);
}

/*
 * This action presents a number of objects for a given number of fields
 */
int PvexTimeStim(int *nObjects, int *objList, int *nFields)
{
	unsigned short index;
	int i;

	index = 0;
	vexbuf[index++] = TIME_STIM;
	vexbuf[index++] = lobyte(*nObjects);
	for(i = 0; i < *nObjects; i++) {
		vexbuf[index++] = lobyte(objList[i]);
	}
	vexbuf[index++] = lobyte(*nFields);

	to_vex(index);
	return(0);
}

/*
 * This action presents one list of objects for a variable time followed by
 * a second list of objects.
 */
int PvexSequenceStim(int *nFrstObj, int *frstList, int *frstFields, int *gapFields,
					 int *nScndObj, int *scndList, int *scndFields)
{
	unsigned short index;
	int i;

	index = 0;
	vexbuf[index++] = SEQUENCE_STIM;
	vexbuf[index++] = lobyte(*nFrstObj);
	for(i = 0; i < *nFrstObj; i++) {
		vexbuf[index++] = lobyte(frstList[i]);
	}
	vexbuf[index++] = lobyte(*frstFields);
	vexbuf[index++] = lobyte(*gapFields);
	vexbuf[index++] = lobyte(*nScndObj);
	for(i = 0; i < *nScndObj; i++) {
		vexbuf[index++] = lobyte(scndList[i]);
	}
	vexbuf[index++] = lobyte(*scndFields);

	to_vex(index);
	return(0);
}

/*
 * This action sets the location of the fixation point
 */
int PvexSetFixLocation(float *x, float *y)
{
	unsigned short index;
	int i;
	char *flbuf;

	index = 0;
	vexbuf[index++] = SET_FP_LOCATION;
	flbuf = float2byte(*x);
	for(i = 0; i < 4; i++) vexbuf[index++] = flbuf[i];
	flbuf = float2byte(*y);
	for(i = 0; i < 4; i++) vexbuf[index++] = flbuf[i];

	to_vex(index);
	return(0);
}

/*
 * This action sets the location of a number of objects
 */
int PvexStimLocation(int *nObjects, int *objList, float *xList, float *yList)
{
	unsigned short index;
	int i, j;
	char *flbuf;

	index = 0;
	vexbuf[index++] = STIM_LOCATION;
	vexbuf[index++] = lobyte(*nObjects);
	for(i = 0; i < *nObjects; i++) {
		vexbuf[index++] = lobyte(objList[i]);
		flbuf = float2byte(xList[i]);
		for(j = 0; j < 4; j++) vexbuf[index++] = flbuf[j];
		flbuf = float2byte(yList[i]);
		for(j = 0; j < 4; j++) vexbuf[index++] = flbuf[j];
	}

	to_vex(index);
	return(0);
}

/*
 * This action sets the location of a number of objects relative to the fixation point
 */
int PvexStimFromFixPoint(int *nObjects, int *objList, float *xList, float *yList)
{
	unsigned short index;
	int i, j;
	char *flbuf;

	index = 0;
	vexbuf[index++] = STIM_FROM_FIX_POINT;
	vexbuf[index++] = lobyte(*nObjects);
	for(i = 0; i < *nObjects; i++) {
		vexbuf[index++] = lobyte(objList[i]);
		flbuf = float2byte(xList[i]);
		for(j = 0; j < 4; j++) vexbuf[index++] = flbuf[j];
		flbuf = float2byte(yList[i]);
		for(j = 0; j < 4; j++) vexbuf[index++] = flbuf[j];
	}

	to_vex(index);
	return(0);
}

/*
 * This action shifts the location of a number of objects
 * and signals after they are shifted
 */
int PvexShiftLocation(int *nObjects, int *objList, float *xList, float *yList)
{
	unsigned short index;
	int i, j;
	char *flbuf;

	index = 0;
	vexbuf[index++] = SHIFT_LOCATION;
	vexbuf[index++] = lobyte(*nObjects);
	for(i = 0; i < *nObjects; i++) {
		vexbuf[index++] = lobyte(objList[i]);
		flbuf = float2byte(xList[i]);
		for(j = 0; j < 4; j++) vexbuf[index++] = flbuf[j];
		flbuf = float2byte(yList[i]);
		for(j = 0; j < 4; j++) vexbuf[index++] = flbuf[j];
	}

	to_vex(index);
	return(0);
}

/*
 * This action reports the screen location of the specified object
 */
int PvexReportLocation(int *obj)
{
	unsigned short index;

	index = 0;
	vexbuf[index++] = REPORT_LOCATION;
	vexbuf[index++] = lobyte(*obj);

	to_vex(index);
	return(0);
}

/*
 * This action returns the code and object location from vex
 */
int PvexMessage(int *code, int *x, int *y)
{
	short i;
	unsigned char *msgfvex;

#ifdef PCS_SOCKET
	if(pcsRecvdVexMsgLen()) msgfvex = pcsRecvdVexMsg();
	else msgfvex = 0;
#else
	msgfvex = pcm[0].pcm_rbufp;
#endif
	
	if(msgfvex) {
		*code = msgfvex[0];
		if(*code == OBJECT_LOCATION) {
			for(i = 0; i < 4; i++) f2c.c[i] = msgfvex[i + 1];
			*x = (int)f2c.f;
			for(i = 0; i < 4; i++) f2c.c[i] = msgfvex[i + 5];
			*y = (int)f2c.f;
		}

	}
	else *code = 0;
	return(0);
}
	
/*
 * This action sets the vex active object
 */
int PvexSetActiveObject(int *obj)
{
	unsigned short index;

	index = 0;
	vexbuf[index++] = SET_ACTIVE_OBJECT;
	vexbuf[index++] = lobyte(*obj);

	to_vex(index);
	return(0);
}

/*
 * This action sets the clipping rectangles for a number of objects
 */
int PvexClipRectSet(int *nObjects, int *objList, int *widthList,
					int *heightList, int *xList, int *yList)
{
	unsigned short index;
	int i;

	index = 0;
	vexbuf[index++] = CLIP_RECT_SET;
	vexbuf[index++] = lobyte(*nObjects);
	for(i = 0; i < *nObjects; i++) {
		vexbuf[index++] = lobyte(objList[i]);
		vexbuf[index++] = hibyte(widthList[i]);
		vexbuf[index++] = lobyte(widthList[i]);
		vexbuf[index++] = hibyte(heightList[i]);
		vexbuf[index++] = lobyte(heightList[i]);
		vexbuf[index++] = hibyte(xList[i]);
		vexbuf[index++] = lobyte(xList[i]);
		vexbuf[index++] = hibyte(yList[i]);
		vexbuf[index++] = lobyte(yList[i]);
	}

	to_vex(index);
	return(0);
}

/*
 * This action sets the clipping rectangles for a number of objects
 * relative to the fixation point
 */
int PvexClipRectFromFixPoint(int *nObjects, int *objList, int *widthList,
					int *heightList, int *xList, int *yList)
{
	unsigned short index;
	int i;

	index = 0;
	vexbuf[index++] = CLIP_RECT_SET_FROM_FP;
	vexbuf[index++] = lobyte(*nObjects);
	for(i = 0; i < *nObjects; i++) {
		vexbuf[index++] = objList[i];
		vexbuf[index++] = hibyte(widthList[i]);
		vexbuf[index++] = lobyte(widthList[i]);
		vexbuf[index++] = hibyte(heightList[i]);
		vexbuf[index++] = lobyte(heightList[i]);
		vexbuf[index++] = hibyte(xList[i]);
		vexbuf[index++] = lobyte(xList[i]);
		vexbuf[index++] = hibyte(yList[i]);
		vexbuf[index++] = lobyte(yList[i]);
	}

	to_vex(index);
	return(0);
}

/*
 * This action clears the clipping rectangles for a number of objects
 */
int PvexClearClipRect(int *nObjects, int *objList)
{
	unsigned short index;
	int i;

	index = 0;
	vexbuf[index++] = FULL_CLIP_RECT;
	vexbuf[index++] = lobyte(*nObjects);
	for(i = 0; i < *nObjects; i++) {
		vexbuf[index++] = lobyte(objList[i]);
	}

	to_vex(index);
	return(0);
}

/*
 * This action draws Walsh patterns in a number of objects
 */
int PvexDrawWalsh(int *nObjects, int *objList, int *patList, int *sizList, int *cntrstList)
{
	unsigned short index;
	int i;

	index = 0;
	vexbuf[index++] = DRAW_WALSH_PATTERN;
	vexbuf[index++] = lobyte(*nObjects);
	for(i = 0; i < *nObjects; i++) {
		vexbuf[index++] = lobyte(objList[i]);
		vexbuf[index++] = lobyte(patList[i]);
		vexbuf[index++] = lobyte(sizList[i]);
		vexbuf[index++] = lobyte(cntrstList[i]);
	}

	to_vex(index);
	return(0);
}

/*
 * This action draws Haar patterns in a number of objects
 */
int PvexDrawHaar(int *nObjects, int *objList, int *patList, int *sizList, int *cntrstList)
{
	unsigned short index;
	int i;

	index = 0;
	vexbuf[index++] = DRAW_HAAR_PATTERN;
	vexbuf[index++] = lobyte(*nObjects);
	for(i = 0; i < *nObjects; i++) {
		vexbuf[index++] = lobyte(objList[i]);
		vexbuf[index++] = lobyte(patList[i]);
		vexbuf[index++] = lobyte(sizList[i]);
		vexbuf[index++] = lobyte(cntrstList[i]);
	}

	to_vex(index);
	return(0);
}

/*
 * This action draws random patterns in a number of objects
 */
int PvexDrawRandom(int *nObjects, int *objList, int *colList, int *rowList, int *sizList, int *pwhiteList)
{
	unsigned short index;
	int i;

	index = 0;
	vexbuf[index++] = DRAW_RANDOM_PATTERN;
	vexbuf[index++] = lobyte(*nObjects);
	for(i = 0; i < *nObjects; i++) {
		vexbuf[index++] = lobyte(objList[i]);
		vexbuf[index++] = lobyte(rowList[i]);
		vexbuf[index++] = lobyte(colList[i]);
		vexbuf[index++] = lobyte(sizList[i]);
		vexbuf[index++] = lobyte(pwhiteList[i]);
	}

	to_vex(index);
	return(0);
}

/*
 * This action draws annuli in a number of objects
 */
int PvexDrawAnnulus(int *nObjects, int *objList, int *outrList, int *innrList, int *cntrstList)
{
	unsigned short index;
	int i;

	index = 0;
	vexbuf[index++] = DRAW_BIG_ANNULUS;
	vexbuf[index++] = lobyte(*nObjects);
	for(i = 0; i < *nObjects; i++) {
		vexbuf[index++] = lobyte(objList[i]);
		vexbuf[index++] = hibyte(outrList[i]);
		vexbuf[index++] = lobyte(outrList[i]);
		vexbuf[index++] = hibyte(innrList[i]);
		vexbuf[index++] = lobyte(innrList[i]);
		vexbuf[index++] = lobyte(cntrstList[i]);
	}

	to_vex(index);
	return(0);
}

/*
 * This action draws oriented bars in a number of objects
 */
int PvexDrawBar(int *nObjects, int *objList, int *anglList, int *widList, int *lenList, int *cntrstList)
{
	unsigned short index;
	int i;

	index = 0;
	vexbuf[index++] = DRAW_BIG_BAR;
	vexbuf[index++] = lobyte(*nObjects);
	for(i = 0; i < *nObjects; i++) {
		vexbuf[index++] = lobyte(objList[i]);
		vexbuf[index++] = lobyte(anglList[i]);
		vexbuf[index++] = hibyte(widList[i]);
		vexbuf[index++] = lobyte(widList[i]);
		vexbuf[index++] = hibyte(lenList[i]);
		vexbuf[index++] = lobyte(lenList[i]);
		vexbuf[index++] = lobyte(cntrstList[i]);
	}

	to_vex(index);
	return(0);
}

/*
 * This action draws flow field patterns in a number of objects
 */
int PvexDrawFlowField(int *nObjects, int *objList, int *widthList, int *heightList,
					  int *nearList, int *farList, int *covList, int *sizeList)
{
	unsigned short index;
	int i;

	index = 0;
	vexbuf[index++] = DRAW_FLOW_PATTERN;
	vexbuf[index++] = lobyte(*nObjects);
	for(i = 0; i < *nObjects; i++) {
		vexbuf[index++] = lobyte(objList[i]);
		vexbuf[index++] = hibyte(widthList[i]);
		vexbuf[index++] = lobyte(widthList[i]);
		vexbuf[index++] = hibyte(heightList[i]);
		vexbuf[index++] = lobyte(heightList[i]);
		vexbuf[index++] = hibyte(nearList[i]);
		vexbuf[index++] = lobyte(nearList[i]);
		vexbuf[index++] = hibyte(farList[i]);
		vexbuf[index++] = lobyte(farList[i]);
		vexbuf[index++] = lobyte(covList[i]);
		vexbuf[index++] = lobyte(sizeList[i]);
	}

	to_vex(index);
	return(0);
}

/*
 * This action draws flow field patterns in a number of objects
 */
int PvexDrawEllipticalFlowField(int *nObjects, int *objList, int *radList, float *widthList,
								float *heightList, int *nearList, int *farList, int *covList, int *sizeList)
{
	unsigned short index;
	int i, j;
	char *flbuf;

	index = 0;
	vexbuf[index++] = DRAW_ELLIPTICAL_FLOW_PATTERN;
	vexbuf[index++] = lobyte(*nObjects);
	for(i = 0; i < *nObjects; i++) {
		vexbuf[index++] = lobyte(objList[i]);
		vexbuf[index++] = hibyte(radList[i]);
		vexbuf[index++] = lobyte(radList[i]);
		flbuf = float2byte(widthList[i]);
		for(j = 0; j < 4; j++) vexbuf[index++] = flbuf[j];
		flbuf = float2byte(heightList[i]);
		for(j = 0; j < 4; j++) vexbuf[index++] = flbuf[j];
		vexbuf[index++] = hibyte(nearList[i]);
		vexbuf[index++] = lobyte(nearList[i]);
		vexbuf[index++] = hibyte(farList[i]);
		vexbuf[index++] = lobyte(farList[i]);
		vexbuf[index++] = lobyte(covList[i]);
		vexbuf[index++] = lobyte(sizeList[i]);
	}

	to_vex(index);
	return(0);
}

/*
 * This action masks the flow field patterns in a number of objects
 */
int PvexMaskFlowField(int *nObjects, int *objList, int *widthList, int *heightList,
					  int *xList, int *yList)
{
	unsigned short index;
	int i;

	index = 0;
	vexbuf[index++] = MASK_FLOW;
	vexbuf[index++] = lobyte(*nObjects);
	for(i = 0; i < *nObjects; i++) {
		vexbuf[index++] = objList[i];
		vexbuf[index++] = hibyte(widthList[i]);
		vexbuf[index++] = lobyte(widthList[i]);
		vexbuf[index++] = hibyte(heightList[i]);
		vexbuf[index++] = lobyte(heightList[i]);
		vexbuf[index++] = hibyte(xList[i]);
		vexbuf[index++] = lobyte(xList[i]);
		vexbuf[index++] = hibyte(yList[i]);
		vexbuf[index++] = lobyte(yList[i]);
	}

	to_vex(index);
	return(0);
}

/*
 * This action draws user defined two-toned patterns in a number of objects
 */
int PvexDrawUserPattern(int *nObjects, int *objList, int *patList, int *sizeList, int *cntrstList)
{
	unsigned short index;
	int i;

	index = 0;
	vexbuf[index++] = DRAW_USER_PATTERN;
	vexbuf[index++] = lobyte(*nObjects);
	for(i = 0; i < *nObjects; i++) {
		vexbuf[index++] = lobyte(objList[i]);
		vexbuf[index++] = hibyte(patList[i]);
		vexbuf[index++] = lobyte(patList[i]);
		vexbuf[index++] = lobyte(sizeList[i]);
		vexbuf[index++] = lobyte(cntrstList[i]);
	}

	to_vex(index);
	return(0);
}

/*
 * This action draws user defined RGB patterns in a number of objects
 */
int PvexDrawRgbUserPattern(int *nObjects, int *objList, int *patList, int *sizeList)
{
	unsigned short index;
	int i;

	index = 0;
	vexbuf[index++] = DRAW_RGB_USER_PATTERN;
	vexbuf[index++] = lobyte(*nObjects);
	for(i = 0; i < *nObjects; i++) {
		vexbuf[index++] = lobyte(objList[i]);
		vexbuf[index++] = hibyte(patList[i]);
		vexbuf[index++] = lobyte(patList[i]);
		vexbuf[index++] = lobyte(sizeList[i]);
	}

	to_vex(index);
	return(0);
}

/*
 * This action draws Tiff images in a number of objects
 */
int PvexDrawTiffImage(int *nObjects, int *objList, int *patList)
{
	unsigned short index;
	int i;

	index = 0;
	vexbuf[index++] = DRAW_TIFF_IMAGE;
	vexbuf[index++] = lobyte(*nObjects);
	for(i = 0; i < *nObjects; i++) {
		vexbuf[index++] = lobyte(objList[i]);
		vexbuf[index++] = hibyte(patList[i]);
		vexbuf[index++] = lobyte(patList[i]);
	}

	to_vex(index);
	return(0);
}

/*
 * This action draws Png images in a number of objects
 */
int PvexDrawPngImage(int *nObjects, int *objList, int *patList)
{
	unsigned short index;
	int i;

	index = 0;
	vexbuf[index++] = DRAW_PNG_IMAGE;
	vexbuf[index++] = lobyte(*nObjects);
	for(i = 0; i < *nObjects; i++) {
		vexbuf[index++] = lobyte(objList[i]);
		vexbuf[index++] = hibyte(patList[i]);
		vexbuf[index++] = lobyte(patList[i]);
	}

	to_vex(index);
	return(0);
}

/*
 * This action draws an opto-kinetic grating in a number of objects
 */
int PvexDrawOknGrating(int *nObjects, int *objList, int *dirList, int *speedList, int *widList,
					   int *heightList, int *barwidList)
{
	unsigned short index;
	int i;

	index = 0;
	vexbuf[index++] = DRAW_OKN_PATTERN;
	vexbuf[index++] = lobyte(*nObjects);
	for(i = 0; i < *nObjects; i++) {
		vexbuf[index++] = lobyte(objList[i]);
		vexbuf[index++] = hibyte(dirList[i]);
		vexbuf[index++] = lobyte(dirList[i]);
		vexbuf[index++] = hibyte(speedList[i]);
		vexbuf[index++] = lobyte(speedList[i]);
		vexbuf[index++] = hibyte(widList[i]);
		vexbuf[index++] = lobyte(widList[i]);
		vexbuf[index++] = hibyte(heightList[i]);
		vexbuf[index++] = lobyte(heightList[i]);
		vexbuf[index++] = hibyte(barwidList[i]);
		vexbuf[index++] = lobyte(barwidList[i]);
	}

	to_vex(index);
	return(0);
}

/*
 * This action draws a sine wave grating in a number of objects
 */
int PvexDrawSineGrating(int *nObjects, int *objList, int *orList, float *spatFreqList, float *tempFreqList, 
						int *cntrPhaseList, float *redList, float *greenList, float *blueList, int *sizList)
{
	unsigned short index;
	int i, j;
	char *flbuf;

	index = 0;
	vexbuf[index++] = DRAW_SINE_WAVE;
	vexbuf[index++] = lobyte(*nObjects);
	for(i = 0; i < *nObjects; i++) {
		vexbuf[index++] = lobyte(objList[i]);
		vexbuf[index++] = hibyte(orList[i]);
		vexbuf[index++] = lobyte(orList[i]);
		flbuf = float2byte(spatFreqList[i]);
		for(j = 0; j < 4; j++) vexbuf[index++] = flbuf[j];
		flbuf = float2byte(tempFreqList[i]);
		for(j = 0; j < 4; j++) vexbuf[index++] = flbuf[j];
		vexbuf[index++] = hibyte(cntrPhaseList[i]);
		vexbuf[index++] = lobyte(cntrPhaseList[i]);
		flbuf = float2byte(redList[i]);
		for(j = 0; j < 4; j++) vexbuf[index++] = flbuf[j];
		flbuf = float2byte(greenList[i]);
		for(j = 0; j < 4; j++) vexbuf[index++] = flbuf[j];
		flbuf = float2byte(blueList[i]);
		for(j = 0; j < 4; j++) vexbuf[index++] = flbuf[j];
		vexbuf[index++] = hibyte(sizList[i]);
		vexbuf[index++] = lobyte(sizList[i]);
	}

	to_vex(index);
	return(0);
}

/*
 * This action draws half of the period of a sine wave grating in a number of objects
 */
int PvexDrawHalfSineGrating(int *nObjects, int *objList, int *orList, float *spatFreqList, float *tempFreqList, 
							int *cntrPhaseList, float *redList, float *greenList, float *blueList, int *sizList)
{
	unsigned short index;
	int i, j;
	char *flbuf;

	index = 0;
	vexbuf[index++] = DRAW_HALF_SINE_WAVE;
	vexbuf[index++] = lobyte(*nObjects);
	for(i = 0; i < *nObjects; i++) {
		vexbuf[index++] = lobyte(objList[i]);
		vexbuf[index++] = hibyte(orList[i]);
		vexbuf[index++] = lobyte(orList[i]);
		flbuf = float2byte(spatFreqList[i]);
		for(j = 0; j < 4; j++) vexbuf[index++] = flbuf[j];
		flbuf = float2byte(tempFreqList[i]);
		for(j = 0; j < 4; j++) vexbuf[index++] = flbuf[j];
		vexbuf[index++] = hibyte(cntrPhaseList[i]);
		vexbuf[index++] = lobyte(cntrPhaseList[i]);
		flbuf = float2byte(redList[i]);
		for(j = 0; j < 4; j++) vexbuf[index++] = flbuf[j];
		flbuf = float2byte(greenList[i]);
		for(j = 0; j < 4; j++) vexbuf[index++] = flbuf[j];
		flbuf = float2byte(blueList[i]);
		for(j = 0; j < 4; j++) vexbuf[index++] = flbuf[j];
		vexbuf[index++] = hibyte(sizList[i]);
		vexbuf[index++] = lobyte(sizList[i]);
	}

	to_vex(index);
	return(0);
}

/*
 * This action draws a square wave grating in a number of objects
 */
int PvexDrawSquareGrating(int *nObjects, int *objList, int *orList, float *spatFreqList, float *tempFreqList,
						  int *cntrPhaseList, float *redList, float *greenList, float *blueList, int *sizList)
{
	unsigned short index;
	int i, j;
	char *flbuf;

	index = 0;
	vexbuf[index++] = DRAW_SQUARE_WAVE;
	vexbuf[index++] = lobyte(*nObjects);
	for(i = 0; i < *nObjects; i++) {
		vexbuf[index++] = lobyte(objList[i]);
		vexbuf[index++] = hibyte(orList[i]);
		vexbuf[index++] = lobyte(orList[i]);
		flbuf = float2byte(spatFreqList[i]);
		for(j = 0; j < 4; j++) vexbuf[index++] = flbuf[j];
		flbuf = float2byte(tempFreqList[i]);
		for(j = 0; j < 4; j++) vexbuf[index++] = flbuf[j];
		vexbuf[index++] = hibyte(cntrPhaseList[i]);
		vexbuf[index++] = lobyte(cntrPhaseList[i]);
		flbuf = float2byte(redList[i]);
		for(j = 0; j < 4; j++) vexbuf[index++] = flbuf[j];
		flbuf = float2byte(greenList[i]);
		for(j = 0; j < 4; j++) vexbuf[index++] = flbuf[j];
		flbuf = float2byte(blueList[i]);
		for(j = 0; j < 4; j++) vexbuf[index++] = flbuf[j];
		vexbuf[index++] = hibyte(sizList[i]);
		vexbuf[index++] = lobyte(sizList[i]);
	}

	to_vex(index);
	return(0);
}

/*
 * This action draws a square wave grating in a number of objects
 */
int PvexDrawHalfSquareGrating(int *nObjects, int *objList, int *orList, float *spatFreqList, float *tempFreqList,
							  int *cntrPhaseList, float *redList, float *greenList, float *blueList, int *sizList)
{
	unsigned short index;
	int i, j;
	char *flbuf;

	index = 0;
	vexbuf[index++] = DRAW_HALF_SQUARE_WAVE;
	vexbuf[index++] = lobyte(*nObjects);
	for(i = 0; i < *nObjects; i++) {
		vexbuf[index++] = lobyte(objList[i]);
		vexbuf[index++] = hibyte(orList[i]);
		vexbuf[index++] = lobyte(orList[i]);
		flbuf = float2byte(spatFreqList[i]);
		for(j = 0; j < 4; j++) vexbuf[index++] = flbuf[j];
		flbuf = float2byte(tempFreqList[i]);
		for(j = 0; j < 4; j++) vexbuf[index++] = flbuf[j];
		vexbuf[index++] = hibyte(cntrPhaseList[i]);
		vexbuf[index++] = lobyte(cntrPhaseList[i]);
		flbuf = float2byte(redList[i]);
		for(j = 0; j < 4; j++) vexbuf[index++] = flbuf[j];
		flbuf = float2byte(greenList[i]);
		for(j = 0; j < 4; j++) vexbuf[index++] = flbuf[j];
		flbuf = float2byte(blueList[i]);
		for(j = 0; j < 4; j++) vexbuf[index++] = flbuf[j];
		vexbuf[index++] = hibyte(sizList[i]);
		vexbuf[index++] = lobyte(sizList[i]);
	}

	to_vex(index);
	return(0);
}

/*
 * This action loads a user-defined pattern from rex
 */
int PvexLoadPatterns(int *nObjects, int *objList, int *stimList, int *sizList, int *rowList,
					 int *colList, int *checkList)
{
	unsigned short index;
	int i, j, k;

	index = 0;
	k = 0;
	for(i = 0; i < *nObjects; i++) {
		vexbuf[index++] = LOAD_PATTERN;
		vexbuf[index++] = lobyte(objList[i]);
		vexbuf[index++] = lobyte(stimList[i]);
		vexbuf[index++] = lobyte(sizList[i]);
		vexbuf[index++] = lobyte(rowList[i]);
		vexbuf[index++] = lobyte(colList[i]);
		for(j = 0; j < rowList[i] * colList[i]; j++) {
			vexbuf[index++] = lobyte(checkList[k++]);
		}
	}

	to_vex(index);
	return(0);
}

/*
 * This action loads an array of points from rex
 */
int PvexLoadPointArray(int *obj, int *nPoints, int *siz, int *pointArray)
{
	unsigned short index;
	int i, l;

	index = 0;
	vexbuf[index++] = LOAD_REX_POINT_ARRAY;
	vexbuf[index++] = lobyte(*obj);
	vexbuf[index++] = hibyte(*nPoints);
	vexbuf[index++] = lobyte(*nPoints);
	vexbuf[index++] = lobyte(*siz);
	l = 0;

	for(i = 0; i < *nPoints; i++) {
		// X coordinate
		vexbuf[index++] = hibyte(pointArray[l]);
		vexbuf[index++] = lobyte(pointArray[l++]);

		// Y coordinate
		vexbuf[index++] = hibyte(pointArray[l]);
		vexbuf[index++] = lobyte(pointArray[l++]);
	}

	to_vex(index);
	return(0);
}

/*
 * This action copies the pattern from one object to another
 */
int PvexCopyObject(int *source, int *destination, float *xscale, float *yscale)
{
	unsigned short index;
	int i;
	char *flbuf;

	index = 0;
	vexbuf[index++] = COPY_OBJECT;
	vexbuf[index++] = lobyte(*source);
	vexbuf[index++] = lobyte(*destination);
	flbuf = float2byte(*xscale);
	for(i = 0; i < 4; i++) vexbuf[index++] = flbuf[i];
	flbuf = float2byte(*yscale);
	for(i = 0; i < 4; i++) vexbuf[index++] = flbuf[i];

	to_vex(index);
	return(0);
}

/*
 * This action turns aliasing on or off
 */
int PvexSetAliasing(int *nObjects, int *objList, int *aliasList)
{
	unsigned short index;
	int i;

	index = 0;
	vexbuf[index++] = SET_ALIASING;
	vexbuf[index++] = lobyte(*nObjects);
	for(i = 0; i < *nObjects; i++) {
		vexbuf[index++] = lobyte(objList[i]);
		vexbuf[index++] = lobyte(aliasList[i]);
	}

	to_vex(index);
	return(0);
}

/*
 * This actions sets the transparency of objects
 */
int PvexSetTransparency(int *nObjects, int *objList, float *transList)
{
	unsigned short index;
	int i, j;
	char *flbuf;

	index = 0;
	vexbuf[index++] = SET_TRANSPARENCY;
	vexbuf[index++] = lobyte(*nObjects);
	for(i = 0; i < *nObjects; i++) {
		vexbuf[index++] = lobyte(objList[i]);
		flbuf = float2byte(transList[i]);
		for(j = 0; j < 4; j++) vexbuf[index++] = flbuf[j];
	}

	to_vex(index);
	return(0);
}

/*
 * This action rotates objects around the X, Y, or Z axes
 */
int PvexRotateObject(int *nObjects, int *objList, float *Xlist, float *Ylist, float *Zlist)
{
	unsigned short index;
	int i, j;
	char *flbuf;

	index = 0;
	vexbuf[index++] = ROTATE_OBJECT;
	vexbuf[index++] = lobyte(*nObjects);
	for(i = 0; i < *nObjects; i++) {
		vexbuf[index++] = lobyte(objList[i]);
		flbuf = float2byte(Xlist[i]);
		for(j = 0; j < 4; j++) vexbuf[index++] = flbuf[j];
		flbuf = float2byte(Ylist[i]);
		for(j = 0; j < 4; j++) vexbuf[index++] = flbuf[j];
		flbuf = float2byte(Zlist[i]);
		for(j = 0; j < 4; j++) vexbuf[index++] = flbuf[j];
	}

	to_vex(index);
	return(0);
}

/*
 * This action computes a ramp
 */
int PvexNewRamp(int *nRamps, int *rampList, int *lenList,
				int *dirList, int *velList, int *xList,
				int *yList, int *typeList, int *actList)
{
	unsigned short index;
	int i;

	index = 0;
	vexbuf[index++] = NEW_RAMP;
	vexbuf[index++] = lobyte(*nRamps);
	for(i = 0; i < *nRamps; i++) {
		vexbuf[index++] = lobyte(rampList[i]);
		vexbuf[index++] = hibyte(lenList[i]);
		vexbuf[index++] = lobyte(lenList[i]);
		vexbuf[index++] = hibyte(dirList[i]);
		vexbuf[index++] = lobyte(dirList[i]);
		vexbuf[index++] = hibyte(velList[i]);
		vexbuf[index++] = lobyte(velList[i]);
		vexbuf[index++] = hibyte(xList[i]);
		vexbuf[index++] = lobyte(xList[i]);
		vexbuf[index++] = hibyte(yList[i]);
		vexbuf[index++] = lobyte(yList[i]);
		vexbuf[index++] = lobyte(typeList[i]);
		vexbuf[index++] = lobyte(actList[i]);
	}

	to_vex(index);
	return(0);
}

/*
 * This action computes a ramp
 */
int PvexNewRampFromFixPoint(int *nRamps, int *rampList, int *lenList,
							int *dirList, int *velList, int *xList,
							int *yList, int *typeList, int *actList)
{
	unsigned short index;
	int i;

	index = 0;
	vexbuf[index++] = NEW_RAMP_FROM_FP;
	vexbuf[index++] = lobyte(*nRamps);
	for(i = 0; i < *nRamps; i++) {
		vexbuf[index++] = lobyte(rampList[i]);
		vexbuf[index++] = hibyte(lenList[i]);
		vexbuf[index++] = lobyte(lenList[i]);
		vexbuf[index++] = hibyte(dirList[i]);
		vexbuf[index++] = lobyte(dirList[i]);
		vexbuf[index++] = hibyte(velList[i]);
		vexbuf[index++] = lobyte(velList[i]);
		vexbuf[index++] = hibyte(xList[i]);
		vexbuf[index++] = lobyte(xList[i]);
		vexbuf[index++] = hibyte(yList[i]);
		vexbuf[index++] = lobyte(yList[i]);
		vexbuf[index++] = lobyte(typeList[i]);
		vexbuf[index++] = lobyte(actList[i]);
	}

	to_vex(index);
	return(0);
}

/*
 * This action loads a ramp from rex
 */
int PvexLoadRamp(int *nRamps, int *rampList, int *actList, int *stpList,
				 float *xList, float *yList)
{
	unsigned short index;
	int i, j, k, l;
	char *flbuf;

	index = 0;
	vexbuf[index++] = LOAD_RAMP;
	vexbuf[index++] = lobyte(*nRamps);
	k = 0;
	for(i = 0; i < *nRamps; i++) {
		vexbuf[index++] = lobyte(rampList[i]);
		vexbuf[index++] = lobyte(actList[i]);
		vexbuf[index++] = hibyte(stpList[i]);
		vexbuf[index++] = lobyte(stpList[i]);
		for(j = 0; j < stpList[i]; j++) {
			flbuf = float2byte(xList[k]);
			for(l = 0; l < 4; l++) vexbuf[index++] = flbuf[l];
			flbuf = float2byte(yList[k]);
			for(l = 0; l < 4; l++) vexbuf[index++] = flbuf[l];
			k++;
		}
	}

	to_vex(index);
	return(0);
}

/*
 * This action loads a .wav sound file
 */
int PvexLoadSound(int *nObjects, int *objList, int *sndList)
{
	unsigned short index;
	int i;

	index = 0;
	vexbuf[index++] = LOAD_SOUND;
	vexbuf[index++] = lobyte(*nObjects);
	for(i = 0; i < *nObjects; i++) {
		vexbuf[index++] = lobyte(objList[i]);
		vexbuf[index++] = hibyte(sndList[i]);
		vexbuf[index++] = lobyte(sndList[i]);
	}

	to_vex(index);
	return(0);
}

/*
 * This action moves objects to the starting position of ramps
 */
int PvexToRampStart(int *nObjects, int *objList, int *rmpList)
{
	unsigned short index;
	int i;

	index = 0;
	vexbuf[index++] = TO_RAMP_START;
	vexbuf[index++] = lobyte(*nObjects);
	for(i = 0; i < *nObjects; i++) {
		vexbuf[index++] = lobyte(objList[i]);
		vexbuf[index++] = lobyte(rmpList[i]);
	}

	to_vex(index);
	return(0);
}

/*
 * This action starts a number of ramps
 */
int PvexStartRamp(int *nRamps, int *rmpList, int *objList, int *cycleList)
{
	unsigned short index;
	int i;

	index = 0;
	vexbuf[index++] = START_RAMP;
	vexbuf[index++] = lobyte(*nRamps);
	for(i = 0; i < *nRamps; i++) {
		vexbuf[index++] = lobyte(rmpList[i]);
		vexbuf[index++] = lobyte(objList[i]);
		vexbuf[index++] = lobyte(cycleList[i]);
	}

	to_vex(index);
	return(0);
}

/*
 * This action resets ramps that were run only once
 */
int PvexResetRamps(void)
{
	unsigned short index;

	index = 0;
	vexbuf[index++] = RESET_RAMPS;

	to_vex(index);
	return(0);
}

/*
 * This action starts the drift in a number of sine wave stimuli
 */
int PvexStartSine(int *nObjects, int *objList)
{
	unsigned short index;
	int i;

	index = 0;
	vexbuf[index++] = START_SINE;
	vexbuf[index++] = lobyte(*nObjects);
	for(i = 0; i < *nObjects; i++) {
		vexbuf[index++] = lobyte(objList[i]);
	}

	to_vex(index);
	return(0);
	
}

/*
 * This action starts the drift in a number of sine wave stimuli
 */
int PvexStartSquare(int *nObjects, int *objList)
{
	unsigned short index;
	int i;

	index = 0;
	vexbuf[index++] = START_SINE;
	vexbuf[index++] = lobyte(*nObjects);
	for(i = 0; i < *nObjects; i++) {
		vexbuf[index++] = lobyte(objList[i]);
	}

	to_vex(index);
	return(0);
}

/*
 * this action starts the drift in a number of sine wave stimuli for a given number of cycles
 */
int PvexTimeGrating(int *nObjects, int *objList, int *cycles)
{
	unsigned short index;
	int i;

	index = 0;
	vexbuf[index++] = TIME_GRATING;
	vexbuf[index++] = lobyte(*nObjects);
	for(i = 0; i < *nObjects; i++) {
		vexbuf[index++] = lobyte(objList[i]);
	}
	vexbuf[index++] = lobyte(*cycles);

	to_vex(index);
	return(0);
}

/*
 * This action starts a number of gratings on ramps
 */
int PvexStartGratingRamp(int *nObjects, int *gratingList, int *rmpList)
{
	unsigned short index;
	int i;

	index = 0;
	vexbuf[index++] = START_SINE_RAMP;
	vexbuf[index++] = lobyte(*nObjects);
	for(i = 0; i < *nObjects; i++) {
		vexbuf[index++] = lobyte(rmpList[i]);
		vexbuf[index++] = lobyte(gratingList[i]);
	}

	to_vex(index);
	return(0);
}

/*
 * This action stops the drift in all sine wave objects
 */
int PvexStopSine(void)
{
	unsigned short index;

	index = 0;
	vexbuf[index++] = STOP_SINE;

	to_vex(index);
	return(0);
}

/*
 * This action stops the drift in all square wave objects
 */
int PvexStopSquare(void)
{
	unsigned short index;

	index = 0;
	vexbuf[index++] = STOP_SINE;

	to_vex(index);
	return(0);
}

/*
 * This action starts the drift in a number of OKN stimuli
 */
int PvexStartOkn(int *nObjects, int *objList)
{
	unsigned short index;
	int i;

	index = 0;
	vexbuf[index++] = START_OKN;
	vexbuf[index++] = lobyte(*nObjects);
	for(i = 0; i < *nObjects; i++) {
		vexbuf[index++] = lobyte(objList[i]);
	}

	to_vex(index);
	return(0);
	
}

/*
 * This action stops the drift in all OKN objects
 */
int PvexStopOkn(void)
{
	unsigned short index;

	index = 0;
	vexbuf[index++] = STOP_OKN;

	to_vex(index);
	return(0);
}

/*
 * This action sets the limit of the random movement of flow checks
 */
int PvexFlowDirectionRange(int *nObjects, int *objList, int *rangeList)
{
	unsigned short index;
	int i;

	index = 0;
	vexbuf[index++] = RANDOM_DIRECTION_LIMIT;
	vexbuf[index++] = lobyte(*nObjects);
	for(i = 0; i < *nObjects; i++) {
		vexbuf[index++] = lobyte(objList[i]);
		vexbuf[index++] = hibyte(rangeList[i]);
		vexbuf[index++] = lobyte(rangeList[i]);
	}

	to_vex(index);
	return(0);
}

/* 
 * This action sets the limit of the random speeds of flow checks
 */
int PvexFlowSpeedRange(int *nObjects, int *objList, int *speedList)
{
	unsigned short index;
	int i;

	index = 0;
	vexbuf[index++] = RANDOM_SPEED_LIMIT;
	vexbuf[index++] = lobyte(*nObjects);
	for(i = 0; i < *nObjects; i++) {
		vexbuf[index++] = lobyte(objList[i]);
		vexbuf[index++] = hibyte(speedList[i]);
		vexbuf[index++] = lobyte(speedList[i]);
	}

	to_vex(index);
	return(0);
}

/*
 * This action sets the translation matrices of a number of flow field objects
 */
int PvexNewFlow(int *nObjects, int *objList, int *xyList, int *zList, int *velList,
				int *rollList, int *pitchList, int *yawList, int *spanList, int *coherList)
{
	unsigned short index;
	int i;

	index = 0;
	vexbuf[index++] = NEW_FLOW;
	vexbuf[index++] = lobyte(*nObjects);
	for(i = 0; i < *nObjects; i++) {
		vexbuf[index++] = lobyte(objList[i]);
		vexbuf[index++] = hibyte(xyList[i]);
		vexbuf[index++] = lobyte(xyList[i]);
		vexbuf[index++] = hibyte(zList[i]);
		vexbuf[index++] = lobyte(zList[i]);
		vexbuf[index++] = hibyte(velList[i]);
		vexbuf[index++] = lobyte(velList[i]);
		vexbuf[index++] = hibyte(rollList[i]);
		vexbuf[index++] = lobyte(rollList[i]);
		vexbuf[index++] = hibyte(pitchList[i]);
		vexbuf[index++] = lobyte(pitchList[i]);
		vexbuf[index++] = hibyte(yawList[i]);
		vexbuf[index++] = lobyte(yawList[i]);
		vexbuf[index++] = lobyte(spanList[i]);
		vexbuf[index++] = lobyte(coherList[i]);
	}

	to_vex(index);
	return(0);
}

/*
 * This action shifts the translation matrices of a number of flow field objects
 * and causes a trigger signal to be sent on the first video field in which
 * the new parameters are in effect
 */
int PvexShiftFlow(int *nObjects, int *objList, int *xyList, int *zList, int *velList,
				int *rollList, int *pitchList, int *yawList, int *spanList, int *coherList)
{
	unsigned short index;
	int i;

	index = 0;
	vexbuf[index++] = SHIFT_FLOW;
	vexbuf[index++] = lobyte(*nObjects);
	for(i = 0; i < *nObjects; i++) {
		vexbuf[index++] = lobyte(objList[i]);
		vexbuf[index++] = hibyte(xyList[i]);
		vexbuf[index++] = lobyte(xyList[i]);
		vexbuf[index++] = hibyte(zList[i]);
		vexbuf[index++] = lobyte(zList[i]);
		vexbuf[index++] = hibyte(velList[i]);
		vexbuf[index++] = lobyte(velList[i]);
		vexbuf[index++] = hibyte(rollList[i]);
		vexbuf[index++] = lobyte(rollList[i]);
		vexbuf[index++] = hibyte(pitchList[i]);
		vexbuf[index++] = lobyte(pitchList[i]);
		vexbuf[index++] = hibyte(yawList[i]);
		vexbuf[index++] = lobyte(yawList[i]);
		vexbuf[index++] = lobyte(spanList[i]);
		vexbuf[index++] = lobyte(coherList[i]);
	}

	to_vex(index);
	return(0);
}

/*
 * This action creates successive frames of flow field movies
 */
int PvexMakeFlowMovie(int *nObjects, int *nFrames, int *objList)
{
	unsigned short index;
	int i;

	index = 0;
	vexbuf[index++] = MAKE_FLOW_MOVIE;
	vexbuf[index++] = lobyte(*nObjects);
	vexbuf[index++] = lobyte(*nFrames);
	for(i = 0; i < *nObjects; i++) vexbuf[index++] = lobyte(objList[i]);

	to_vex(index);
	return(0);
}

/*
 * This action sets the starting frame for flow field moview
 */
int PvexToFlowMovieStart(int *nObjects, int *startFrame, int *objList)
{
	unsigned short index;
	int i;

	index = 0;
	vexbuf[index++] = TO_FLOW_MOVIE_START;
	vexbuf[index++] = lobyte(*nObjects);
	vexbuf[index++] = lobyte(*startFrame);
	for(i = 0; i < *nObjects; i++) vexbuf[index++] = lobyte(objList[i]);

	to_vex(index);
	return(0);
}

/*
 * This action starts the flow fields in a number of objects
 */
int PvexStartFlow(int *nObjects, int *objList)
{
	unsigned short index;
	int i;

	index = 0;
	vexbuf[index++] = START_FLOW;
	vexbuf[index++] = lobyte(*nObjects);
	for(i = 0; i < *nObjects; i++) vexbuf[index++] = lobyte(objList[i]);

	to_vex(index);
	return(0);
}

/*
 * This action starts the flow fields in a number of objects for a number of fields
 */
int PvexTimeFlow(int *nObjects, int *fields, int *objList)
{
	unsigned short index;
	int i;

	index = 0;
	vexbuf[index++] = TIME_FLOW;
	vexbuf[index++] = lobyte(*nObjects);
	for(i = 0; i < *nObjects; i++) vexbuf[index++] = lobyte(objList[i]);
	vexbuf[index++] = lobyte(*fields);

	to_vex(index);
	return(0);
}

/*
 * This action starts a number of flow field movies
 */
int PvexShowFlowMovie(int *nObjects, int *startFrame, int *lastFrame, int *objList)
{
	unsigned short index;
	int i;

	index = 0;
	vexbuf[index++] = SHOW_FLOW_MOVIE;
	vexbuf[index++] = lobyte(*nObjects);
	vexbuf[index++] = lobyte(*startFrame);
	vexbuf[index++] = lobyte(*lastFrame);
	for(i = 0; i < *nObjects; i++) vexbuf[index++] = lobyte(objList[i]);

	to_vex(index);
	return(0);
}

/*
 * This action starts a number of flow fields and ramps
 */
int PvexStartFlowRamp(int *nFlows, int *flwList, int *nRamps, int *rmpList, int *objList)
{
	unsigned short index;
	int i;

	index = 0;
	vexbuf[index++] = START_FLOW_RAMP;
	vexbuf[index++] = lobyte(*nFlows);
	for(i = 0; i < *nFlows; i++) vexbuf[index++] = lobyte(flwList[i]);

	vexbuf[index++] = lobyte(*nRamps);
	for(i = 0; i < *nRamps; i++) {
		vexbuf[index++] = lobyte(rmpList[i]);
		vexbuf[index++] = lobyte(objList[i]);
	}

	to_vex(index);
	return(0);
}

/*
 * This action stops all flow fields and ramps
 */
int PvexStopFlowRamp(void)
{
	unsigned short index;

	index = 0;
	vexbuf[index++] = STOP_FLOW_RAMP;

	to_vex(index);
	return(0);
}

/*
 * This action displays a sequence of objects for a number of fields each
 */
int PvexShowMovieClip(int *nClips, int *objList, int *nFrames, int *interval, int *cycles)
{
	unsigned short index;
	int i;

	index = 0;
	vexbuf[index++] = SHOW_MOVIE_CLIP;
	vexbuf[index++] = lobyte(*nClips);
	for(i = 0; i < *nClips; i++) vexbuf[index++] = lobyte(objList[i]);
	vexbuf[index++] = lobyte(*nFrames);
	vexbuf[index++] = lobyte(*interval);
	vexbuf[index++] = lobyte(*cycles);

	to_vex(index);
	return(0);
}

/*
 * This action stops all flow field and object movies
 */
int PvexStopMovie(void)
{
	unsigned short index;

	index = 0;
	vexbuf[index++] = STOP_MOVIE;

	to_vex(index);
	return(0);
}

/*
 * This action sets up a number of trigger lists
 */
int PvexSetTriggers(int *nTrigs, int *rmpList, int *trigList, int *frameList)
{
	unsigned short index;
	int i, j, k;

	index = 0;
	k = 0;
	vexbuf[index++] = SET_TRIGGERS;
	vexbuf[index++] = lobyte(*nTrigs);
	for(i = 0; i < *nTrigs; i++) {
		vexbuf[index++] = lobyte(rmpList[i]);
		vexbuf[index++] = lobyte(trigList[i]);
		for(j = 0; j < trigList[i]; j++) {
			vexbuf[index++] = hibyte(frameList[k]);
			vexbuf[index++] = lobyte(frameList[k]);
			k++;
		}
	}

	to_vex(index);
	return(0);
}


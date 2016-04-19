/* Y o u r   D e s c r i p t i o n                       */
/*                            AppBuilder Photon Code Lib */
/*                                         Version 2.01  */

/* Standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* Local headers */
#include "ablibs.h"
#include "ph_ewind.h"
#include "abimport.h"
#include "proto.h"
#include "GLcommand.h"

extern lsrDispWindow dispWind;
extern SOCKET pcsSock;

PtWidget_t *pVexDebugDialog = (PtWidget_t *)NULL;
PtWidget_t *pVexDebugText = (PtWidget_t *)NULL;

PtWorkProcId_t *debugId;
int vexDebug;
PhPoint_t rexLoc;
unsigned char ewindVexbuf[16384] = { 0 };
char errorString[MAXCHARS];
char vexHost[64];

int initPcsSocket( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;
	
	pcsSocket();

	return( Pt_CONTINUE );
}

int getVexName( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	char *name;
	
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	
	PtGetResource(widget, Pt_ARG_TEXT_STRING, &name, 0);
	strcpy(vexHost, name);

	return( Pt_CONTINUE );
}

int connectVex( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	int index = 0;
	char *port = "9998";
	char *subnet = 0;
	
	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;
	
	if(pcsSetPeerAddr(vexHost, port) == 0) {
		if(pcsAllocPassiveSocket(subnet, port) == 0) {
			PtRealizeWidget(ABW_vexTerminal);
			PtRealizeWidget(ABW_vexCommand);
			
			ewindVexbuf[index++] = HELLO;
			pcsSend(ewindVexbuf, index);
		}
		else {
			pcsSocket();
		}
	}
	else pcsSocket();

	PtDestroyWidget(ABW_vexName);

	return( Pt_CONTINUE );
}

int checkVexConnect( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;
	
	if(pcsSock) return( Pt_END );
	else pcsSocket();
	
	return( Pt_CONTINUE );
}

int closeSocket( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	int index = 0;
	
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	
	ewindVexbuf[index++] = GOODBYE;
	pcsSend(ewindVexbuf, index);
	
	pcsClose();
	
	return( Pt_CONTINUE );
}

int hideDialog( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	PtDestroyWidget(ABW_vexName);
	
	return( Pt_CONTINUE );
}

int vexMouse( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PhEvent_t *event;
	PhCursorInfo_t cursor;
	PhPointerEvent_t *buttonData;
	PhPoint_t absLoc;
	PhPoint_t winLoc;
	int inputGroup;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(!pcsSock) return( Pt_IGNORE );
	
	event = (PhEvent_t *)cbinfo->event;
	inputGroup = PhInputGroup(event);
	PhQueryCursor(inputGroup, &cursor);
	
	if(event->type == Ph_EV_BUT_PRESS) {
		buttonData = (PhPointerEvent_t *)PhGetData(event);
		if(buttonData->buttons & Ph_BUTTON_SELECT) {
			sendVexMouse(GLUT_DOWN);
		}
		else if(buttonData->buttons & Ph_BUTTON_MENU) {
			PtGetAbsPosition(widget, &absLoc.x, &absLoc.y);
			winLoc.x = cursor.pos.x - absLoc.x;
			winLoc.y = cursor.pos.y - absLoc.y;
			rexLoc.x = -(((winLoc.x * -dispWind.eyeRange) / dispWind.area.size.w) + dispWind.eyeMax);
			rexLoc.y = ((winLoc.y * -dispWind.eyeRange) / dispWind.area.size.h) + dispWind.eyeMax;
			sendVexMotion(rexLoc);
		}
	}
	else if(event->type == Ph_EV_BUT_RELEASE) {
		if(event->subtype == Ph_EV_RELEASE_REAL) {
			buttonData = (PhPointerEvent_t *)PhGetData(event);
			if(buttonData->buttons & Ph_BUTTON_SELECT) {
				sendVexMouse(GLUT_UP);
			}
		}
	}
	else if(event->type == Ph_EV_PTR_MOTION_BUTTON) {
		buttonData = (PhPointerEvent_t *)PhGetData(event);
		PtGetAbsPosition(widget, &absLoc.x, &absLoc.y);
		winLoc.x = cursor.pos.x - absLoc.x;
		winLoc.y = cursor.pos.y - absLoc.y;
		rexLoc.x = -(((winLoc.x * -dispWind.eyeRange) / dispWind.area.size.w) + dispWind.eyeMax);
		rexLoc.y = ((winLoc.y * -dispWind.eyeRange) / dispWind.area.size.h) + dispWind.eyeMax;
		sendVexMotion(rexLoc);
	}
	
	return( Pt_CONSUME );
}

int keyboard( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	double sf, tf, cp, rc, gc, bc, sz;
	float a, x, y, z;
	int i = 0;
	int j, k, l, m, n, o, p, q;
	char fu;
	char *buffer;
	
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	
	PtGetResource(widget, Pt_ARG_TEXT_STRING, &buffer, 0);

	switch(buffer[0]) {
	case 'a':
		i = sscanf(&buffer[1], "%d", &j);
		if(i == 1) sendVexString(buffer);
		else {
			sprintf(errorString, "a [0 1]: set aliasing [off on]\n");
			vexPrint(errorString);
		}
		break;
	case 'b':
		i = sscanf(&buffer[1], "%d %d %d", &j, &k, &l);
		if((i == 1) || (i == 3))sendVexString(buffer);
		else {
			sprintf(errorString, "b [r g b]: set Background color to rgb\n");
			vexPrint(errorString);
			sprintf(errorString, "b [n]: set Background luminance to n\n");
			vexPrint(errorString);
		}
		break;
	case 'C':
		i = sscanf(&buffer[1], "%d %d %d %d", &j, &k, &l, &m);
		if(i == 4) sendVexString(buffer);
		else {
			sprintf(errorString, "C mask [red, green, blue, alpha]\n");
			vexPrint(errorString);
		}
		break;
	case 'c':
		if(!strncmp(buffer, "clear", 5) || !strncmp(buffer, "cfull", 5)) sendVexString(buffer);
		else {
			i = sscanf(&buffer[1], "%c %lf %lf %lf %lf", &fu, &sf, &tf, &cp, &rc);
			if(i == 5) sendVexString(buffer);
			else {
				sprintf(errorString, "clear: clear the screen\n");
				vexPrint(errorString);
				sprintf(errorString, "cfull: set clipping rectangle to full screen\n");
				vexPrint(errorString);
				sprintf(errorString, "cr [x, y, w, h]: set clipping rectangle to location x, y, width w height h\n");
				vexPrint(errorString);
			}
		}
		break;
	case 'D':
		i = sscanf(&buffer[1], "%d", &j);
		if(i == 1) {
			if(j != 0) {
				if(pVexDebugDialog == (PtWidget_t *)NULL) {
					ApCreateModule(ABM_vexDebugDialog, ABW_base, NULL);
				}
				vexDebug = 1;
			}
			else {
				vexDebug = 0;
			}
			
			sendVexString(buffer);
		}
		else {
			sprintf(errorString, "D [0/1]: set Debug state to 0 = NO, 1 = YES\n");
			vexPrint(errorString);
		}
		break;
	case 'd':
		i = sscanf(&buffer[1], "%d %d", &j, &k);
		if(i == 2) sendVexString(buffer);
		else {
			sprintf(errorString, "d [d w]: set screen distance, pattern width in millimeters\n");
			vexPrint(errorString);
		}
		break;
	case 'e':
		i = sscanf(&buffer[1], "%d", &j);
		if(i == 1) sendVexString(buffer);
		else {
			sprintf(errorString, "e [0/1]: set erase method to 0 = each object 1 = whole screen\n");
			vexPrint(errorString);
		}
		break;
	case 'F':
		i = sscanf(&buffer[1], "%c %d %d %d", &fu, &j, &k, &l);
		if((fu == 'c') || (fu == 'm')){
			if((i == 2) || (i == 4)) sendVexString(buffer);
		}
		else if((fu == 'x') && (i == 3)) sendVexString(buffer);
		else if((fu == 'z') && (i == 2)) sendVexString(buffer);
		else {
			sprintf(errorString, "Fc [n]: set Fixation luminanCe to n\n");
			vexPrint(errorString);
			sprintf(errorString, "Fc [r g b]: set Fixation Color to r g b\n");
			vexPrint(errorString);
			sprintf(errorString, "Fm [n]: set Fixation diM level to n\n");
			vexPrint(errorString);
			sprintf(errorString, "Fm [r g b]: set Fixation diM color to r g b\n");
			vexPrint(errorString);
			sprintf(errorString, "Fx [n m]: move Fixation point to nX mY\n");
			vexPrint(errorString);
			sprintf(errorString, "Fz [n]: set Fixation point size to n pixels\n");
			vexPrint(errorString);
		}
		break;
	case 'f':
		if(!strncmp(buffer,"fpon", 4) || !strncmp(buffer,"fpoff", 5) || !strncmp(buffer,"fpdim", 5)) {
			sendVexString(buffer);
		}
		else {
			i = sscanf(&buffer[1], "%c %d", &fu, &j);
			if((i == 2) && (fu == 'r')) sendVexString(buffer);
			else {
				sprintf(errorString, "fpon: turn Fixation point on\n");
				vexPrint(errorString);
				sprintf(errorString, "fpoff: turn Fixation point off\n");
				vexPrint(errorString);
				sprintf(errorString, "fpdim: dim Fixation point\n");
				vexPrint(errorString);
				sprintf(errorString, "fr [n]: set frame rate to n Hz\n");
				vexPrint(errorString);
			}
		}
		break;
	case 'g':
		i = sscanf(&buffer[1], "%c %d %lf %lf %lf %lf %lf %lf %lf", &fu, &j, &sf, &tf, &cp, &rc, &gc, &bc, &sz);
		if(i == 1) {
			if((fu == 'b') || (fu == 'e')) sendVexString(buffer);
			else if(fu == 'k') {
				sprintf(errorString, "a = angle of bars [0, 90, 180, 270]\n");
				vexPrint(errorString);
				sprintf(errorString, "s = speed of bars (rex units per second)\n");
				vexPrint(errorString);
				sprintf(errorString, "w = width of bars in rex units\n");
				vexPrint(errorString);
				sprintf(errorString, "h = height of bars in rex units\n");
				vexPrint(errorString);
				sprintf(errorString, "t = width of patch in rex units\n");
				vexPrint(errorString);
			}
			else if((fu == 's') || (fu == 'q')) {
				sprintf(errorString, "o = orientation of the grating in degrees\n");
				vexPrint(errorString);
				sprintf(errorString, "f = spatial frequency in cycles per degree\n");
				vexPrint(errorString);
				sprintf(errorString, "t = temporal frequency in cycles per second\n");
				vexPrint(errorString);
				sprintf(errorString, "\tnegative temporal frequency reverses drift direction\n");
				vexPrint(errorString);
				sprintf(errorString, "p = counterphase [1], or drift [0]\n");
				vexPrint(errorString);
				sprintf(errorString, "c = percent contrast of all three colors\n");
				vexPrint(errorString);
				sprintf(errorString, "r, g, b = percent contrast of each of the\n\tred, green and blue colors\n");
				vexPrint(errorString);
				sprintf(errorString, "\t0 percent turns color off,\n\tnegative percent gives color - color grating\n");
				vexPrint(errorString);
				sprintf(errorString, "s = size of patch in rex units\n");
				vexPrint(errorString);
			}
			else if(fu == 'B') {
				sprintf(errorString, "o1 = the first grating object to display\n");
				vexPrint(errorString);
				sprintf(errorString, "o2 = the last grating object to display\n");
				vexPrint(errorString);
				sprintf(errorString, "t = optional number of cycles to run grating\n");
			}
			else if(fu == 'r') {
				sprintf(errorString, "r = number of ramp to run with grating\n");
				vexPrint(errorString);
				sprintf(errorString, "c = type of ramp cycle\n");
				vexPrint(errorString);
				sprintf(errorString, "     c = 0: ramp runs once\n");
				vexPrint(errorString);
				sprintf(errorString, "     c = 1: oscillation - ramp goes back and forth\n");
				vexPrint(errorString);
				sprintf(errorString, "     c = 2: loop - ramp runs over and over\n");
				vexPrint(errorString);
			}
		}
		else if((i == 2) || (i == 3) || (i == 4) || (i == 6) || (i == 7) || (i == 9)) sendVexString(buffer);
		else {
			sprintf(errorString, "gk [a s w h t]: draw OKN grating with a angle, s speed,\n\tw X h size bar, t width\n");
			vexPrint(errorString);
			sprintf(errorString, "gs [o f t p c s]: draw Sine wave with o orientation,\n\tspatial f, temporal t frequency, counterphase p [0/1],\n\tc %% contrast, s size\n");
			vexPrint(errorString);
			sprintf(errorString, "gs [o f t p r g b s]: draw Sine wave with o orientation,\n\tf spatial, t temporal frequency, counterphase p [0/1],\n\tr g b %% contrast, s size\n");
			vexPrint(errorString);
			sprintf(errorString, "gsh [o f t p h c s]: draw Sine wave with o orientation,\n\tspatial f, temporal t frequency, counterphase p [0/1], half wave h [-1/1], \n\tc %% contrast, s size\n");
			vexPrint(errorString);
			sprintf(errorString, "gsh [o f t p h r g b s]: draw Sine wave with o orientation,\n\tf spatial, t temporal frequency, counterphase p [0/1], half wave h [-1/1],\n\tr g b %% contrast, s size\n");
			vexPrint(errorString);
			sprintf(errorString, "gq [o f t p c s]: draw sQuare wave with o orientation,\n\tspatial f, temporal t frequency, counterphase p [0/1],\n\tc %% contrast, s size\n");
			vexPrint(errorString);
			sprintf(errorString, "gq [o f t p r g b s]: draw sQuare wave with o orientation,\n\tf spatial, t temporal frequency, counterphase p [0/1],\n\tr g b %% contrast, s size\n");
			vexPrint(errorString);
			sprintf(errorString, "gqh [o f t p h c s]: draw sQuare wave with o orientation,\n\tspatial f, temporal t frequency, counterphase p [0/1], half wave h [-1/1],\n\tc %% contrast, s size\n");
			vexPrint(errorString);
			sprintf(errorString, "gqh [o f t p h r g b s]: draw sQuare wave with o orientation,\n\tf spatial, t temporal frequency, counterphase p [0/1], half wave h [-1/1],\n\tr g b %% contrast, s size\n");
			vexPrint(errorString);
			sprintf(errorString, "gB [o1 o2]: begin grating drift in objects o1 through o2\n");
			vexPrint(errorString);
			sprintf(errorString, "gB [o1 o2 c]: begin grating drift in\n\tobjects o1 through o2 for c cycles\n");
			vexPrint(errorString);
			sprintf(errorString, "gb: begin grating drift in active object\n");
			vexPrint(errorString);
			sprintf(errorString, "gb [c]: begin drift in active object for c cycles\n");
			vexPrint(errorString);
			sprintf(errorString, "ge: stop grating drift\n");
			vexPrint(errorString);
			sprintf(errorString, "gr [r c]: begin drift on Ramp r with Cycle [0 1 2]\n");
			vexPrint(errorString);
		}
		break;
	case 'H':
	case 'h':
		printHelp();
		break;
	case 'L':
		i = sscanf(&buffer[1], "%d %d %d %d", &j, &k, &l, &m);
		if((i == 2) || (i == 4)) sendVexString(buffer);
		else {
			sprintf(errorString, "L [n r]: set color lookup table entry n to color r r r\n");
			vexPrint(errorString);
			sprintf(errorString, "L [n r g b]: set color lookup table entry n to color r g b\n");
			vexPrint(errorString);
		}
		break;
	case 'l':
		i = sscanf(&buffer[1], "%d %d", &j, &k);
		if(i == 2) sendVexString(buffer);
		else {
			sprintf(errorString, "l [n m]: set Luminances of fore/background checks to n/m\n");
			vexPrint(errorString);
		}
		break;
	case 'M':
		i = sscanf(&buffer[1], "%c %d %d %d %d", &fu, &j, &k, &l, &m);
		if(fu == 'f') {
			if(i == 5) sendVexString(buffer);
			else {
				sprintf(errorString, "o = first object with flow field movie\n");
				vexPrint(errorString);
				sprintf(errorString, "n = number of objects with movies\n");
				vexPrint(errorString);
				sprintf(errorString, "f = first frame of movie\n");
				vexPrint(errorString);
				sprintf(errorString, "l = last frame of movie\n");
				vexPrint(errorString);
			}
		}
		else if(fu == 'o') {
			if(i == 5) sendVexString(buffer);
			else {
				sprintf(errorString, "f = first object of movie\n");
				vexPrint(errorString);
				sprintf(errorString, "l = last object of movie\n");
				vexPrint(errorString);
				sprintf(errorString, "i = number of video field per movie frame\n");
				vexPrint(errorString);
				sprintf(errorString, "c = number of times to run movie\n");
				vexPrint(errorString);
			}
		}
		else if(fu == 's') sendVexString(buffer);
		else {
			sprintf(errorString, "Start a movie clip\n");
			vexPrint(errorString);
			sprintf(errorString, "Mo [f l i c]: start an Object movie\n");
			vexPrint(errorString);
			sprintf(errorString, "Mf [o n f l]: start a flow field movie\n");
			vexPrint(errorString);
			sprintf(errorString, "Ms: stop movie clip\n");
			vexPrint(errorString);
		}
		break;
	case 'm':
		i = sscanf(&buffer[1], "%d %d", &j, &k);
		if((i == 1) || (i == 2)) sendVexString(buffer);
		else {
			sprintf(errorString, "m [m s]: mouse acts on two objects [Master & Slave]\n");
			vexPrint(errorString);
			sprintf(errorString, "m [m]: mouse acts on Master only [default]\n");
			vexPrint(errorString);
		}
		break;
	case 'n':
		i = sscanf(&buffer[1], "%d", &j);
		if(i == 1) sendVexString(buffer);
		else {
			sprintf(errorString, "n [n]: play a sound from the file s[n].wav");
			vexPrint(errorString);
		}
		break;
	case 'O':
		switch(buffer[1]) {
		case 'p':
			i = sscanf(&buffer[2], "%d %d %d %d %d %d %d", &j, &k, &l, &m, &n, &o, &p);
			if((i == 6) || (i == 7)) sendVexString(buffer);
			else {
				sprintf(errorString, "w = width of flow field\n");
				vexPrint(errorString);
				sprintf(errorString, "h = height of flow field\n");
				vexPrint(errorString);
				sprintf(errorString, "n = distance to near plane\n");
				vexPrint(errorString);
				sprintf(errorString, "f = distance to far plane\n");
				vexPrint(errorString);
				sprintf(errorString, "p = tenths %% coverage\n");
				vexPrint(errorString);
				sprintf(errorString, "s = check size\n");
				vexPrint(errorString);
			}
			break;
		case 'm':
			i = sscanf(&buffer[2], "%lf %lf %lf %lf", &sf, &tf, &cp, &rc);
			if(i == 4) sendVexString(buffer);
			else {
				sprintf(errorString, "w = width of mask\n");
				vexPrint(errorString);
				sprintf(errorString, "h = height of mask\n");
				vexPrint(errorString);
				sprintf(errorString, "x = x coordinate of mask in object\n");
				vexPrint(errorString);
				sprintf(errorString, "y = y coordinate of mask in object\n");
				vexPrint(errorString);
			}
			break;
		case 'd':
		case 'l':
			i = sscanf(&buffer[2], "%d", &j);
			if(i == 1) sendVexString(buffer);
			else {	
				sprintf(errorString, "l = width in angles of the directions of random check movement");
				vexPrint(errorString);
				sprintf(errorString, "l = range of the speeds of random check movement");
				vexPrint(errorString);
			}
			break;
		case 't':
			i = sscanf(&buffer[2], "%d %d %lf %d %d %d %d %d", &j, &k, &sf, &m, &n, &o, &p, &q);
			if(i == 8) sendVexString(buffer);
			else {
				sprintf(errorString, "x = angle of 2-D translation in degrees [0 to 360]\n");
				vexPrint(errorString);
				sprintf(errorString, "z = angle of 3-D translation in degrees [-90 to 90]\n");
				vexPrint(errorString);
				sprintf(errorString, "v = translation velocity in degrees / sec\n");
				vexPrint(errorString);
				sprintf(errorString, "r = rate of roll in degrees / sec\n");
				vexPrint(errorString);
				sprintf(errorString, "p = rate of pitch in degrees / sec\n");
				vexPrint(errorString);
				sprintf(errorString, "y = rate of yaw in degrees / sec\n");
				vexPrint(errorString);
				sprintf(errorString, "s = life span of checks in fields\n");
				vexPrint(errorString);
				sprintf(errorString, "c = percent of checks coherent\n");
				vexPrint(errorString);
			}
			break;
		case 'v':
			i = sscanf(&buffer[2], "%d %d", &j, &k);
			if(i == 2) sendVexString(buffer);
			else {
				sprintf(errorString, "o = object for movie\n");
				vexPrint(errorString);
				sprintf(errorString, "l = number of frames for movie\n");
				vexPrint(errorString);
			}
			break;
		case 's':
			i = sscanf(&buffer[2], "%d %d", &j, &k);
			if(i == 2) sendVexString(buffer);
			else {
				sprintf(errorString, "o = object for movie\n");
				vexPrint(errorString);
				sprintf(errorString, "s = starting frame of the movie\n");
				vexPrint(errorString);
			}
			break;
		case 'b':
			sendVexString(buffer);
			break;
		case  'B':
			i = sscanf(&buffer[2], "%d %d %d", &j, &k, &l);
			if((i == 2) || (i == 3)) sendVexString(buffer);
			else {
				sprintf(errorString, "o1 = the first flow object to display\n");
				vexPrint(errorString);
				sprintf(errorString, "o2 = the last flow field object to display\n");
				vexPrint(errorString);
				sprintf(errorString, "t = optional number of fields to run flow");
				vexPrint(errorString);
			}
			break;
		case 'e':
			sendVexString(buffer);
			break;
		case 'r':
			i = sscanf(&buffer[2], "%d %d %d", &j, &k, &l);
			if(i == 3) sendVexString(buffer);
			else {
				sprintf(errorString, "r = number of ramp to run with flow field\n");
				vexPrint(errorString);
				sprintf(errorString, "o = number of object to put on ramp\n");
				vexPrint(errorString);
				sprintf(errorString, "c = type of ramp cycle\n");
				vexPrint(errorString);
				sprintf(errorString, "     c = 0: ramp runs once\n");
				vexPrint(errorString);
				sprintf(errorString, "     c = 1: oscillation - ramp goes back and forth\n");
				vexPrint(errorString);
				sprintf(errorString, "     c = 2: loop - ramp runs over and over\n");
				vexPrint(errorString);
			}
			break;
		default:
			sprintf(errorString, "Compute Optic flow field for active object\n");
			vexPrint(errorString);
			sprintf(errorString, "Op [w h n f p s]: set flow field Pattern\n");
			vexPrint(errorString);
			sprintf(errorString, "Om [w h x y]: set flow field Mask\n");
			vexPrint(errorString);
			sprintf(errorString, "Od [l]: set limits of random Directions");
			vexPrint(errorString);
			sprintf(errorString, "Ol [l]: set Limits of random speeds");
			vexPrint(errorString);
			sprintf(errorString, "Ot [x z v r p y s c]: set Translation of flow field\n");
			vexPrint(errorString);
			sprintf(errorString, "Ov [o l]: make a flow field moVie\n");
			vexPrint(errorString);
			sprintf(errorString, "Os [o n]: Set the first field of a flow field movie\n");
			vexPrint(errorString);
			sprintf(errorString, "OB [o1 o2]: begin flow fields in objects o1 through o2\n");
			vexPrint(errorString);
			sprintf(errorString, "OB [o1 o2 t]: begin flow fields in objects o1 through o2 for t fields\n");
			vexPrint(errorString);
			sprintf(errorString, "Ob: Begin flow field\n");
			vexPrint(errorString);
			sprintf(errorString, "Ob [t]: Begin flow field in active object for t fields\n");
			vexPrint(errorString);
			sprintf(errorString, "Oe: End flow field\n");
			vexPrint(errorString);
			sprintf(errorString, "Or [r o c]: begin flow in active object and Ramp r with Object o cycle [0 1 2]\n");
			vexPrint(errorString);
			break;
		}
		break;
	case 'o':
		i = sscanf(&buffer[1], "%c %d %d %d", &fu, &j, &k, &l);
		if(i == 4) sendVexString(buffer);
		else {
			sprintf(errorString, "of [r g b]: set color of object foreground to r, g, b\n");
			vexPrint(errorString);
			sprintf(errorString, "ob [r g b]: set color of object background to r, g, b\n");
			vexPrint(errorString);
		}
		break;
	case 'P':
		i = sscanf(&buffer[1], "%d %d %f %f", &j, &k, &x, &y);
		if((i == 2) || (i == 4)) sendVexString(buffer);
		else {
			sprintf(errorString, "P [s d]: coPy pattern from Source object to Destination object\n");
			vexPrint(errorString);
			sprintf(errorString, "P [s d x y]: coPy pattern from Source object to Destination object\n");
			vexPrint(errorString);
			sprintf(errorString, "	     and scale width by X%% and height by Y%%\n");
			vexPrint(errorString);
		}
		break;
	case 'p':
		switch(buffer[1]) {
		case 'a':
			i = sscanf(&buffer[2], "%lf %lf %d", &sf, &tf, &j);
			if(i == 3) sendVexString(buffer);
			else {
				sprintf(errorString, "pa [o i c]: draw Annulus with outer radius o, inner radius i, contrast c");
				vexPrint(errorString);
			}
			break;
		case 'b':
			i = sscanf(&buffer[2], "%d %lf %lf %d", &j, &sf, &tf, &k);
			if(i == 4) sendVexString(buffer);
			else {
				sprintf(errorString, "pb [a w h c]: draw a bar with angle a, width w, height h, contrast c");
				vexPrint(errorString);
			}
			break;
		case 'h':
		case 'w':
			i = sscanf(&buffer[2], "%d %d %d", &j, &k, &l);
			if(i == 3) sendVexString(buffer);
			else {
				sprintf(errorString, "ph [n c s]: draw Haar pattern n with contrast c, size s");
				vexPrint(errorString);
				sprintf(errorString, "pw [n c s]: draw Walsh pattern n with contrast c, size s");
				vexPrint(errorString);
			}
			break;
		case 'p':
			i = sscanf(&buffer[2], "%d", &j);
			if(i == 1) sendVexString(buffer);
			else {
				sprintf(errorString, "pp [n]: draw a PNG image from the file i[n].png");
				vexPrint(errorString);
			}
			break;
		case 'r':
			i = sscanf(&buffer[2], "%d %d %d %d", &j, &k, &l, &m);
			if(i == 4) sendVexString(buffer);
			else {
				sprintf(errorString, "pr [p r c s]: draw Random pattern with p%% white, r X c checks, size s");
				vexPrint(errorString);
			}
			break;
		case 't':
			i = sscanf(&buffer[2], "%d", &j);
			if(i == 1) sendVexString(buffer);
			else {
				sprintf(errorString, "pt [n]: draw a TIFF image from the file i[n].tif");
				vexPrint(errorString);
			}
			break;
		case 'u':
			i = sscanf(&buffer[2], "%d %d %d %d", &j, &k, &l, &m);
			if((i == 2) || (i == 3)) sendVexString(buffer);
			else {
				sprintf(errorString, "pu [n c s]: draw user pattern from file Pn with contrast c, size s");
				vexPrint(errorString);
				sprintf(errorString, "pu [n s]: draw user RGB pattern from file Pn, size s");
				vexPrint(errorString);
			}
		default:
			sprintf(errorString, "pa [o i c]: draw Annulus with outer radius o, inner radius i, contrast c\n");
			vexPrint(errorString);
			sprintf(errorString, "pb [a w h c]: draw a bar with angle a, width w, height h, contrast c\n");
			vexPrint(errorString);
			sprintf(errorString, "ph [n c s]: draw Haar pattern n with contrast c, size s\n");
			vexPrint(errorString);
			sprintf(errorString, "pp [n]: draw PNG image from the file i[n].png\n");
			vexPrint(errorString);
			sprintf(errorString, "pr [p r c s]: draw Random pattern with p%% white, r X c checks, size s\n");
			vexPrint(errorString);
			sprintf(errorString, "pt [n]: draw a TIFF image from the file i[n].tif\n");
			vexPrint(errorString);
			sprintf(errorString, "pu [n c s]: draw user pattern from file Pn with contrast c, size s\n");
			vexPrint(errorString);
			sprintf(errorString, "pu [n s]: draw user RGB pattern from file Pn, size s\n");
			vexPrint(errorString);
			sprintf(errorString, "pw [n c s]: draw Walsh pattern n with contrast c, size s\n");
			vexPrint(errorString);
			break;
		}
		break;
	case 'R':
		i = sscanf(&buffer[1], "%f, %f, %f", &x, &y, &z);
		if(i == 3) sendVexString(buffer);
		else {
			sprintf(errorString, "R [x, y, z]: set rotation about x, y, z axes\n");
			vexPrint(errorString);
		}
		break;
	case 'r':
		if(!strncmp(buffer, "rstop", 5)) sendVexString(buffer);
		else if(!strncmp(buffer, "reset", 5)) sendVexString(buffer);
		else {
			switch(buffer[1]) {
			case 'c':
				i = sscanf(&buffer[2], "%d %lf %d %lf %lf %lf %d %d", &j, &sf, &k, &tf, &cp, &rc, &l, &m);
				if(i == 8) sendVexString(buffer);
				else {
					sprintf(errorString, "n = number of ramp to compute\n");
					vexPrint(errorString);
					sprintf(errorString, "len = half of linear ramp length [tenths of a degree]\n");
					vexPrint(errorString);
					sprintf(errorString, "len = radius of circular ramp [tenths of a degree]\n");
					vexPrint(errorString);
					sprintf(errorString, "ang = slope of linear ramp in degrees\n");
					vexPrint(errorString);
					sprintf(errorString, "ang = starting point of circular ramp\n");
					vexPrint(errorString);
					sprintf(errorString, "vel = velocity of ramp in degrees / sec\n");
					vexPrint(errorString);
					sprintf(errorString, "xoff, yoff = location of ramp reference point\n");
					vexPrint(errorString);
					sprintf(errorString, "type = type and reference point of ramp\n");
					vexPrint(errorString);
					sprintf(errorString, "type = 2: linear ramp centered on the reference point\n");
					vexPrint(errorString);
					sprintf(errorString, "type = 4: linear ramp beginning at the reference point\n");
					vexPrint(errorString);
					sprintf(errorString, "type = 8: linear ramp ending at the reference point\n");
					vexPrint(errorString);
					sprintf(errorString, "type = 16: Clock-wise circular ramp centered on reference\n");
					vexPrint(errorString);
					sprintf(errorString, "type = 32: Counterclock-wise circular ramp centered on reference\n");
					vexPrint(errorString);
					sprintf(errorString, "action = object switch after end of ramp\n");
					vexPrint(errorString);
					sprintf(errorString, "action = 1: leave object on after end of ramp\n");
					vexPrint(errorString);
					sprintf(errorString, "action = 0: turn object off after end of ramp\n");
					vexPrint(errorString);
				}
				break;
			case 's':
				i = sscanf(&buffer[2], "%d %d %d %d %d %d", &j, &k, &l, &m, &n, &o);
				if((i == 3) || (i == 6)) sendVexString(buffer);
				else {
					sprintf(errorString, "r = number of ramp to run\n");
					vexPrint(errorString);
					sprintf(errorString, "o = object to put on the ramp\n");
					vexPrint(errorString);
					sprintf(errorString, "c = type of ramp cycle\n");
					vexPrint(errorString);
					sprintf(errorString, "c = 0: ramp runs once\n");
					vexPrint(errorString);
					sprintf(errorString, "c = 1: oscillation - ramp goes back and forth\n");
					vexPrint(errorString);
					sprintf(errorString, "c = 2: loop - ramp runs over and over\n");
					vexPrint(errorString);
				}
			default:
				sprintf(errorString,"rc [n len ang vel xoff yoff type action]: Compute a ramp\n");
				vexPrint(errorString);
				sprintf(errorString, "rs [r o c]: Start Ramp r with Object o, cycle c [0 1 2]\n");
				vexPrint(errorString);
				sprintf(errorString, "rstop: stop all ramps\n");
				vexPrint(errorString);
				sprintf(errorString, "reset: reset ramps\n");
				vexPrint(errorString);
				break;
			}
		}
		break;
	case 'S':
		if(!strncmp(buffer, "Ston", 4) || !strncmp(buffer, "Stoff", 5) || !strncmp(buffer, "Swap", 4)) {
			sendVexString(buffer);
		}
		else {
			sprintf(errorString, "Ston: Switch all objects on\n");
			vexPrint(errorString);
			sprintf(errorString, "Stoff: Switch all objects off\n");
			vexPrint(errorString);
		}
		break;
	case 's':
		if(!strncmp(buffer, "ston", 4) || !strncmp(buffer, "stoff", 5) || !strncmp(buffer, "syncVon", 7) ||
		   !strncmp(buffer, "syncVoff", 8) || !strncmp(buffer, "syncDon", 7) || !strncmp(buffer, "syncDoff", 8)) {
			sendVexString(buffer);
		}
		else {
			i = sscanf(&buffer[1], "%d", &j);
			if(i == 1) sendVexString(buffer);
			else {
				sprintf(errorString, "s [n]: make object n active [release slave]\n");
				vexPrint(errorString);
				sprintf(errorString, "ston: Switch active object on\n");
				vexPrint(errorString);
				sprintf(errorString, "stoff: Switch active object off\n");
				vexPrint(errorString);
				sprintf(errorString, "syncVon n: Switch n video syncs on\n");
				vexPrint(errorString);
				sprintf(errorString, "syncVoff: Switch all video syncs off\n");
				vexPrint(errorString);
				sprintf(errorString, "syncDon: Switch digital sync on\n");
				vexPrint(errorString);
				sprintf(errorString, "syncDoff: Switch digital sync off\n");
				vexPrint(errorString);
			}
		}
		break;
	case 'T':
		i = sscanf(&buffer[1], "%f", &a);
		if(i == 1) sendVexString(buffer);
		else {
			sprintf(errorString, "T [n]: set object transparency to n [0.0 - 1.0]");
			vexPrint(errorString);
		}
		break;
	case 't':
		i = sscanf(&buffer[1], "%d %d %d %d %d", &j, &k, &l, &m, &n);
		if((i == 1) || (i == 5)) sendVexString(buffer);
		else {
			sprintf(errorString, "t [n]: turn active object on for n fields\n");
			vexPrint(errorString);
			sprintf(errorString, "t [o1 m n o2 p]: object 1 for m fields, n gap, object 2 for p fields\n");
			vexPrint(errorString);
		}
		break;
	case 'W':
		i = sscanf(&buffer[1], "%c %d %d", &fu, &j, &k);
		if((fu == 's') && (i == 3)) sendVexString(buffer);
		else {
			sprintf(errorString, "Ws [w h]: set video Sync size to width w, height h\n");
			vexPrint(errorString);
			sprintf(errorString, "w = width of sync\n");
			vexPrint(errorString);
			sprintf(errorString, "h = height of sync\n");
			vexPrint(errorString);
		}
		break;
	case 'w':
		i = sscanf(&buffer[1], "%c %d %d", &fu, &j, &k);
		if((fu == 's') && (i == 3)) sendVexString(buffer);
		else if((fu == 's') && !strncmp(&buffer[3], "full screen", 11)) sendVexString(buffer);
		else {
			sprintf(errorString, "ws [w h]: set Window size to width w, height h\n");
			vexPrint(errorString);
			sprintf(errorString, "w = width of window\n");
			vexPrint(errorString);
			sprintf(errorString, "h = height of window\n");
			vexPrint(errorString);
			sprintf(errorString, "ws full screen: set Window size to full screen\n");
			vexPrint(errorString);
		}
		break;
	case 'X':
		i = sscanf(&buffer[1], "%d %f %f", &n, &x, &y);
		if(i == 3) sendVexString(buffer);
		else {
			sprintf(errorString, "X [n x y]: set video Sync n location to xX yY\n");
			vexPrint(errorString);
		}
		break;
	case 'x':
		i = sscanf(&buffer[1], "%d %d", &j, &k);
		if(i == 2) sendVexString(buffer);
		else {
			sprintf(errorString, "x [n m]: move active object to nX mY\n");
			vexPrint(errorString);
			sprintf(errorString, "Active object location X = %d Y = %d\n",
					rexLoc.x, rexLoc.y);
			vexPrint(errorString);
		}
		break;
	case 'Z':
		i = sscanf(&buffer[1], "%d", &j);
		if(i == 1) sendVexString(buffer);
		else {
			sprintf(errorString, "Z [s]: set check size of all objects to s\n");
			vexPrint(errorString);
		}
		break;
	case 'z':
		i = sscanf(&buffer[1], "%d", &j);
		if(i == 1) sendVexString(buffer);
		else {
			sprintf(errorString, "z [s]: set check size of active object to s\n");
			vexPrint(errorString);
		}
		break;
	}

	return( Pt_CONTINUE );
}

int character( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PhEvent_t *event;
	PhKeyEvent_t *keyData;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	
	event = (PhEvent_t *)cbinfo->event;
	keyData = (PhKeyEvent_t *)PhGetData(event);

	if(!(keyData->key_flags & Pk_KF_Key_Down)) return( Pt_PROCESS );

	switch(keyData->key_sym) {
	case Pk_Home:
		sendVexFuncKey(GLUT_KEY_HOME);
		return( Pt_CONSUME );
	case Pk_Left:
		sendVexFuncKey(GLUT_KEY_LEFT);
		return( Pt_CONSUME );
	case Pk_Up:
		if(keyData->key_scan == 0) sendVexFuncKey(GLUT_KEY_F1);
		else sendVexFuncKey(GLUT_KEY_UP);
		return( Pt_CONSUME );
	case Pk_Right:
		sendVexFuncKey(GLUT_KEY_RIGHT);
		return( Pt_CONSUME );
	case Pk_Down:
		if(keyData->key_scan == 0) sendVexFuncKey(GLUT_KEY_F2);
		else sendVexFuncKey(GLUT_KEY_DOWN);
		return( Pt_CONSUME );
	case Pk_Pg_Up:
		sendVexFuncKey(GLUT_KEY_PAGE_UP);
		return( Pt_CONSUME );
	case Pk_Pg_Down:
		sendVexFuncKey(GLUT_KEY_PAGE_DOWN);
		return( Pt_CONSUME );
	case Pk_End:
		sendVexFuncKey(GLUT_KEY_END);
		return( Pt_CONSUME );
	case Pk_Insert:
		sendVexFuncKey(GLUT_KEY_INSERT);
		return( Pt_CONSUME );
	case Pk_F1:
		sendVexFuncKey(GLUT_KEY_F1);
		return( Pt_CONSUME );
	case Pk_F2:
		sendVexFuncKey(GLUT_KEY_F2);
		return( Pt_CONSUME );
	case Pk_F3:
		sendVexFuncKey(GLUT_KEY_F3);
		return( Pt_CONSUME );
	case Pk_F4:
		sendVexFuncKey(GLUT_KEY_F4);
		return( Pt_CONSUME );
	case Pk_F5:
		sendVexFuncKey(GLUT_KEY_F5);
		return( Pt_CONSUME );
	case Pk_F6:
		sendVexFuncKey(GLUT_KEY_F6);
		return( Pt_CONSUME );
	case Pk_F7:
		sendVexFuncKey(GLUT_KEY_F7);
		return( Pt_CONSUME );
	case Pk_F8:
		sendVexFuncKey(GLUT_KEY_F8);
		return( Pt_CONSUME );
	case Pk_F9:
		sendVexFuncKey(GLUT_KEY_F9);
		return( Pt_CONSUME );
	case Pk_F10:
		sendVexFuncKey(GLUT_KEY_F10);
		return( Pt_CONSUME );
	case Pk_F11:
		sendVexFuncKey(GLUT_KEY_F11);
		return( Pt_CONSUME );
	case Pk_F12:
		sendVexFuncKey(GLUT_KEY_F12);
		return( Pt_CONSUME );
	default:
		return( Pt_PROCESS );
	}
	
	// shouldn't get here but need to keep compilers happy
	return( Pt_CONSUME );
}

int initVexDebug( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	if(apinfo->reason == ABR_PRE_REALIZE) {
		pVexDebugDialog = link_instance;
	}
	else {
		pVexDebugText = ApGetWidgetPtr(pVexDebugDialog, ABN_vexDebugText);

		debugId = PtAppAddWorkProc((PtAppContext_t)NULL, listen2Vex, (void *)NULL);
	}

	return( Pt_CONTINUE );
}

int closeDebugDialog( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PhWindowEvent_t *winEvent;
	int index = 0;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	/* get pointer to the window event structure */
	winEvent = (PhWindowEvent_t *)cbinfo->cbdata;

	if(winEvent->event_f == Ph_WM_CLOSE) {
		if(vexDebug != 0) {
			/* tell vex to stop sending debug output */
			ewindVexbuf[index++] = REX_KEYBOARD;
			ewindVexbuf[index++] = 'D';
			ewindVexbuf[index++] = ' ';
			ewindVexbuf[index++] = '0';
			pcsSend(ewindVexbuf, index);

			vexDebug = 0;
		}

		pVexDebugDialog = (PtWidget_t *)NULL;
		pVexDebugText = (PtWidget_t *)NULL;
		PtAppRemoveWorkProc(NULL, debugId);
	}

	return( Pt_CONTINUE );
}

void sendVexString(char *string)
{
	int index = 0;
	int i;
	
	ewindVexbuf[index++] = REX_KEYBOARD;
	for(i = 0; i < strlen(string); i++) {
		ewindVexbuf[index++] = string[i];
	}

	pcsSend(ewindVexbuf, index);

	PtSetResource(ABW_vexCommand, Pt_ARG_TEXT_STRING, "", 0);
	PtSetResource(ABW_vexTerminal, Pt_ARG_TEXT_STRING, "", 0);
	return;
}

void sendVexFuncKey(int key)
{
	union {
		unsigned char c[4];
		unsigned short s[2];
		float f;
	} f2c;

	int index = 0;

	f2c.s[0] = key;

	ewindVexbuf[index++] = REX_FUNCTION_KEY;
	ewindVexbuf[index++] = f2c.c[1];
	ewindVexbuf[index++] = f2c.c[0];
	pcsSend(ewindVexbuf, index);
	
	return;
}

void sendVexMouse(int action)
{
	int index = 0;

	ewindVexbuf[index++] = REX_MOUSE;
	ewindVexbuf[index++] = action;
	pcsSend(ewindVexbuf, index);
}

void sendVexMotion(PhPoint_t loc)
{
	union {
		unsigned char c[4];
		unsigned short s[2];
		float f;
	} f2c;

	int index = 0;

	ewindVexbuf[index++] = REX_MOTION;

	f2c.s[0] = loc.x;
	ewindVexbuf[index++] = f2c.c[1];
	ewindVexbuf[index++] = f2c.c[0];
	f2c.s[0] = loc.y;
	ewindVexbuf[index++] = f2c.c[1];
	ewindVexbuf[index++] = f2c.c[0];
	pcsSend(ewindVexbuf, index);
}

void vexPrint(char *text)
{
	PtMultiTextModifyText(ABW_vexTerminal,
						  0,
						  0,
						  -1,
						  text,
						  strlen(text),
						  0,
						  0);

	PtSetResource(ABW_vexCommand, Pt_ARG_TEXT_STRING, "", 0);
}

/*
 * Subroutine to print help messages
 */
void printHelp(void)
{

	sprintf(errorString, "Single key stoke commands are:\n");
	vexPrint(errorString);
	sprintf(errorString, "     h: print this message\n");
	vexPrint(errorString);
	sprintf(errorString, "     arrow keys: move active stimulus\n");
	vexPrint(errorString);
	sprintf(errorString, "     [Page Up]/[Page Down]: increment/decrement stimulus number\n\n");
	vexPrint(errorString);
	sprintf(errorString, "     function keys: increment/decrement stimulus parameters\n");
	vexPrint(errorString);
	sprintf(errorString, "Commands requiring arguments start with the letters:\n");
	vexPrint(errorString);
	sprintf(errorString, "     b, c, D, d, e, F, f, g, L, l, M, m, n, O\n");
	vexPrint(errorString);
	sprintf(errorString, "     o, P, p, r, S, s, T, t, w, x, Z, z\n");
	vexPrint(errorString);
	sprintf(errorString, "     Type the first letter of the command for a help message\n");
	vexPrint(errorString);
	return;
}

int listen2Vex(void *data)
{
	int recvMsgLen;
	char errorMsg[64];
	char msgRecvd[MSG_BUFFER_LEN];

	recvMsgLen = recvfromPackets(msgRecvd, sizeof(msgRecvd));
	if(recvMsgLen < 0) {
		sprintf(errorMsg, "listen2Vex(): error %d in receiving packets",
				-recvMsgLen);
		rxerr(errorMsg);
		recvMsgLen = 0;
	}

	msgRecvd[recvMsgLen] = 0;				// null terminate the received string
	
	if((recvMsgLen > 0) && (pVexDebugDialog != (PtWidget_t *)NULL)) {
		PtMultiTextModifyText(pVexDebugText,
							  0,
							  0,
							  -1,
							  msgRecvd,
							  strlen(msgRecvd),
							  0,
							  0);

		PtFlush();
	}

	return( Pt_CONTINUE );
}
